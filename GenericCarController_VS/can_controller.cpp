#include "can_controller.h"
#include "cmd_serial.h"

#pragma region //******* PRIVATE ********//

#pragma region /** SEND **/  
bool CAN_CONTROLLER::sendCmdToCANController(CAN_COMMAND *cmd, uint8_t nextCmdDelay = CAN_MIN_COMMAND_INTERVAL)
{      
	#if CAN_DISABLE_INTERRUPTS_AT_TRANSFER
		uint8_t oldSREG = SREG;
		noInterrupts();
	#endif

		MCP2515::ERROR ret = CAN->sendMessage(cmd);

	#if CAN_DISABLE_INTERRUPTS_AT_TRANSFER
		SREG = oldSREG;
	#endif

	if (ret == MCP2515::ERROR_OK)
	{
		#if CAN_ENABLE_MIN_DELAY_BETWEEN_COMMANDS
			this->nextTXmillis = (millis() + nextCmdDelay);
		#endif

		if (CONFIG.get(PRINT_CAN_DATA))
			cmd->print_P(sSent);

		return true;
	}
	else
	{
		#if CAN_ENABLE_MIN_DELAY_BETWEEN_COMMANDS
			this->nextTXmillis = (millis() + CAN_TX_RETRY_TIMEOUT);
		#endif

		return false;
	}
}
#pragma endregion

#pragma region /** RECEIVE **/
bool CAN_CONTROLLER::fetchCmdFromController(CAN_COMMAND *cmd)
{        
	#if COMPILE_CAN_INTERRUPT && CAN_INTERRUPT_MODE == CAN_INTERRUPT_MODE_FLAG
		MCP2515::ERROR res;
		
		if (this->currentInterruptsReg & MCP2515::CANINTF_RX0IF) //BUFFER 0			
			res = this->CAN->readMessage(MCP2515::RXB0, cmd);		
		else if (this->currentInterruptsReg & MCP2515::CANINTF_RX1IF) //BUFFER 1		
			res = this->CAN->readMessage(MCP2515::RXB1, cmd);			
		else 
			return false;

		uint8_t oldSREG = SREG;
		noInterrupts();

			this->currentInterruptsReg = this->CAN->getInterrupts();

		SREG = oldSREG;
	#else
		MCP2515::ERROR res = this->CAN->readMessage(cmd);
		if (res != MCP2515::ERROR_OK)
			return false;
	#endif	

    #if COMPILE_CAN_BLACKLIST && CAN_BLACKLIST_LENGTH > 0
		for(unsigned char i = 0; i < this->blacklistIDsLength; i++)
			if(this->blacklistIDs[i] == cmd->can_id)
				return false;        
    #endif       
      
    return true;
}
#pragma endregion

#pragma region /** INTERRUPT **/
#if COMPILE_CAN && COMPILE_CAN_INTERRUPT      
	void CAN_CONTROLLER::handleInterrupt()
	{   
		CAN_CONTROLLER *can = &DEFAULT_CAN;
		uint8_t iReg = can->CAN->getInterrupts();

		#if CAN_HANDLE_ERRORS_ON_ONTERRUPT			
			bool iRegModified = false;
			if (iReg & MCP2515::CANINTF_ERRIF) //ERROR
			{
				uint8_t error = can->CAN->getErrorFlags();
				if (error & (MCP2515::EFLG_RX0OVR | MCP2515::EFLG_RX1OVR))
				{
					cont->modifyRegister(MCP2515::MCP_EFLG, (MCP2515::EFLG_RX0OVR | MCP2515::EFLG_RX1OVR), 0);

					iReg &= ~MCP2515::CANINTF_ERRIF; //CLEAR ERRIF
					iRegModified = true;
				}
			}

			if (iReg & MCP2515::CANINTF_MERRF)
			{
				can->CAN->clearMERR();

				iReg &= ~MCP2515::CANINTF_MERRF;
				iRegModified = true;
			}

			if (iRegModified)
				can->CAN->setRegister(MCP2515::MCP_CANINTF, iReg);
		#endif

		//FETCH RX
		#if CAN_INTERRUPT_MODE == CAN_INTERRUPT_MODE_FETCH
			CAN_COMMAND cmd;
			MCP2515::ERROR res;

			if (iReg & MCP2515::CANINTF_RX0IF)
			{
				res = can->CAN->readMessage(MCP2515::RXB0, &cmd);
				if (res == MCP2515::ERROR_OK)
					DEFAULT_CAN.rxQueue.push(&cmd);
			}

			if (iReg & MCP2515::CANINTF_RX1IF)
			{
				res = can->CAN->readMessage(MCP2515::RXB1, &cmd);
				if (res == MCP2515::ERROR_OK)
					DEFAULT_CAN.rxQueue.push(&cmd);
			}
		#elif CAN_INTERRUPT_MODE == CAN_INTERRUPT_MODE_FLAG
			#if CAN_HANDLE_ERRORS_ON_ONTERRUPT
				uint8_t rxMask = (MCP2515::CANINTF_RX0IF | MCP2515::CANINTF_RX1IF);
				DEFAULT_CAN.currentInterruptsReg = (iReg & rxMask);								
			#else
				DEFAULT_CAN.currentInterruptsReg = iReg;
			#endif
		#endif	
	} 
#endif
#pragma endregion

#pragma region /** RX & TX QUEUE **/
bool CAN_CONTROLLER::readyToSendCmd()
{
	#if CAN_ENABLE_MIN_DELAY_BETWEEN_COMMANDS
		return (this->nextTXmillis <= millis());
	#else
		return true;
	#endif
}

bool CAN_CONTROLLER::cmdAvailableInRXQueue()
{
	return !rxQueue.isEmpty();
}

bool CAN_CONTROLLER::cmdAvailableInController()
{	
	#if COMPILE_CAN_PIN_CMD_AVAILABLE
		#if COMPILE_CAN_INTERRUPT && CAN_INTERRUPT_MODE == CAN_INTERRUPT_MODE_FLAG
			return (this->currentInterruptsReg);
		#else
			return (digitalRead(this->intPin) == LOW);
		#endif
	#else		
		return this->CAN->checkReceive();
	#endif
};

uint8_t CAN_CONTROLLER::fillRXQueueFromCANController(uint8_t maxCmds = CAN_RX_QUEUE_LENGTH)
{
	uint8_t i = 0;
	for (; cmdAvailableInController() && !this->rxQueue.isFull() && i < maxCmds; i++)
	{
		CAN_COMMAND cmd;
		if (this->fetchCmdFromController(&cmd))
			this->rxQueue.push(&cmd);
		else
			break;
	}

	return i;
}

#if CAN_ENABLE_TX_QUEUE && CAN_TX_QUEUE_LENGTH > 0
	bool CAN_CONTROLLER::pullTXCmd(CAN_COMMAND *cmd)
	{
		return txQueue.pull(cmd);
	}

	bool CAN_CONTROLLER::pushTXCmd(CAN_COMMAND *cmd)
	{
		return txQueue.push(cmd);
	}
#endif

#pragma endregion

#pragma region /* PERIODICAL CMDS */
#if COMPILE_CAN_PERIODICAL && CAN_PERIODICAL_CMDS_LENGTH > 0
void CAN_CONTROLLER::checkPeriodicalCmds()
{
	unsigned long now = millis();
	for (uint8_t i = 0; i < this->periodicalCmdsLength; i++)
	{
		CAN_COMMAND_PERIODICAL *per = this->periodicalCmds[i];
		if (now >= per->nextMillis)
		{
			if (sendCmd(per->cmd))
				per->nextMillis += per->interval;
			else
				break;
		}
	}
}
#endif
#pragma endregion

#pragma endregion

#pragma region //******* PUBLIC ********//
CAN_CONTROLLER::CAN_CONTROLLER(const uint8_t csPin, const uint8_t intPin, void (*h)(CAN_CONTROLLER*, CAN_COMMAND*)) : intPin(intPin), receiveHandler(h)
{
	this->CAN = new MCP2515(csPin);

	#if CAN_ENABLE_MIN_DELAY_BETWEEN_COMMANDS
		this->nextTXmillis = 0;
	#endif  
    #if COMPILE_CAN_WHITELIST && CAN_WHITELIST_LENGTH > 0
		this->whitelistLength = 0;
    #endif
    #if COMPILE_CAN_BLACKLIST && CAN_BLACKLIST_LENGTH > 0
		this->blacklistIDsLength = 0;   
    #endif        
    #if COMPILE_CAN_PERIODICAL && CAN_PERIODICAL_CMDS_LENGTH > 0
		this->periodicalCmdsLength = 0;     
    #endif
}

void CAN_CONTROLLER::setupPins()
{
	#if COMPILE_CAN_PIN_CMD_AVAILABLE
		pinMode(this->intPin, INPUT_PULLUP);
	#endif
}

bool CAN_CONTROLLER::begin(const CAN_SPEED bitrate, const CAN_CLOCK clockSet)
{     	
	SPI.begin();
	
	this->CAN->reset();
	if (this->CAN->setBitrate(bitrate, clockSet) != MCP2515::ERROR_OK)
		return false;

	#if COMPILE_CAN_WHITELIST && CAN_WHITELIST_LENGTH > 0
		if (!this->setupWhitelist())
			return false;
	#endif

	#if CAN_MODE_LOOPBACK	
		if (this->CAN->setLoopbackMode() != MCP2515::ERROR_OK)
	#else
		if (this->CAN->setNormalMode() != MCP2515::ERROR_OK)
	#endif
		return false;

	#if COMPILE_CAN_PIN_CMD_AVAILABLE && COMPILE_CAN_INTERRUPT
		this->enableInterrupts();
	#endif   
    
    return true;    
}

void CAN_CONTROLLER::checkCAN()
{
	#if COMPILE_DEBUG && CAN_DEBUG_PRINT_QUEUE_STATS
		if (CONFIG.get(PRINT_DEBUG))
		{
			static uint8_t maxRX = 0;
			uint8_t currRX = this->rxQueue.nbRecs();
			if (maxRX < currRX)
				maxRX = currRX;

			utilsPrint_P(PSTR("RX:"));
			cmdSerial.print(currRX);
			utilsPrint_P(PSTR("/"));
			cmdSerial.print(CAN_RX_QUEUE_LENGTH);
			utilsPrint_P(PSTR(" MAX: "));
			cmdSerial.println(maxRX);

			#if CAN_ENABLE_TX_QUEUE && CAN_TX_QUEUE_LENGTH > 0
				static uint8_t maxTX = 0;
				uint8_t currTX = this->txQueue.nbRecs();
				if (maxTX < currTX)
					maxTX = currTX;

				utilsPrint_P(PSTR("TX:"));
				cmdSerial.print(currTX);
				utilsPrint_P(PSTR("/"));
				cmdSerial.print(CAN_TX_QUEUE_LENGTH);
				utilsPrint_P(PSTR(" MAX: "));
				cmdSerial.println(maxTX);
			#endif
		}
	#endif

	/*** RX **/
	#if !COMPILE_CAN_INTERRUPT || CAN_INTERRUPT_MODE == CAN_INTERRUPT_MODE_FLAG
		this->fillRXQueueFromCANController();
	#endif	
		
	CAN_COMMAND cmd;
	while (this->pullRXCmd(&cmd))
	{
		if (CONFIG.get(PRINT_CAN_DATA))
			cmd.print_P(sReceived);

		this->receiveHandler(this, &cmd);
	}

	/*** TX ***/
	#if CAN_ENABLE_TX_QUEUE && CAN_TX_QUEUE_LENGTH > 0
		while (this->readyToSendCmd() && this->pullTXCmd(&cmd))
		{	
			if (!this->sendCmdToCANController(&cmd))
			{
				this->pushTXCmd(&cmd);
				break;
			}
		}
	#endif

	/*** PERIODICAL CMDs ***/
	#if COMPILE_CAN_PERIODICAL && CAN_PERIODICAL_CMDS_LENGTH > 0
		this->checkPeriodicalCmds();
	#endif
}

#pragma region /** SEND **/
bool CAN_CONTROLLER::sendCmd(CAN_COMMAND *cmd, uint8_t extraByte, bool immediately = false)
{
	if (cmd->can_dlc >= CAN_MAX_DLEN)
		return false;

	cmd->data[cmd->can_dlc++] = extraByte;
	bool result = this->sendCmd(cmd, immediately);
	cmd->can_dlc--;

	return result;
}

bool CAN_CONTROLLER::sendCmd(CAN_COMMAND *cmd, bool immediately = false)
{
	#if CAN_ENABLE_TX_QUEUE && CAN_TX_QUEUE_LENGTH > 0
		if (immediately)
			return sendCmdToCANController((CAN_COMMAND*)cmd);
		else
			return this->pushTXCmd((CAN_COMMAND*)cmd);
	#else
		#if ENABLE_CAN_MIN_DELAY_BETWEEN_COMMANDS 
			if (immediately || this->readyToSendCmd())
				return sendCmdToCANController((CAN_COMMAND*)cmd);
			else
				return false;
		#else
			return sendCmdToCANController((CAN_COMMAND*)cmd);
		#endif					
	#endif
}

bool CAN_CONTROLLER::sendCmdSequence(CAN_COMMAND *cmd1, CAN_COMMAND *cmd2, uint8_t sequenceCount = 1, boolean addSequenceToCommand = false, uint8_t nextCmdDelay = CAN_MIN_COMMAND_INTERVAL, bool immediately = false)
{
		#if CAN_ENABLE_TX_QUEUE && CAN_TX_QUEUE_LENGTH > 0
			if (!immediately)
			{
				uint8_t totalCmds = sequenceCount + (cmd2 != NULL ? 1 : 0);
				if (this->getTXQueueAvailableSlots() < totalCmds)
					return false;
			}
		#endif

	uint8_t i = 0;
	for (; i < sequenceCount; i++)
	{
		if (addSequenceToCommand)
			this->sendCmd(cmd1, i);
		else
			this->sendCmd(cmd1);

		#if CAN_ENABLE_TX_QUEUE == 0 || CAN_TX_QUEUE_LENGTH <= 0
			delay(delayBetweenSequences);
		#endif
	}

	if (cmd2 != NULL)
	{
		if (addSequenceToCommand)
			this->sendCmd(cmd2, sequenceCount, immediately);
		else
			this->sendCmd(cmd2, immediately);
	}

	return true;
}

bool CAN_CONTROLLER::sendCmd_P(PROGMEM CAN_COMMAND *cmd, uint8_t extraByte, bool immediately = false)
{    
	CAN_COMMAND ramCmd;
	memcpy_P(&ramCmd, cmd, sizeof(CAN_COMMAND));

	return sendCmd(&ramCmd, extraByte, immediately);
}

bool CAN_CONTROLLER::sendCmd_P(PROGMEM CAN_COMMAND *cmd, bool immediately = false)
{       
	CAN_COMMAND ramCmd;
	memcpy_P(&ramCmd, cmd, sizeof(CAN_COMMAND));

	return sendCmd(&ramCmd, immediately);
}      
  
bool CAN_CONTROLLER::sendCmdSequence_P(PROGMEM CAN_COMMAND *cmd1, PROGMEM CAN_COMMAND *cmd2, uint8_t sequenceCount = 1, boolean addSequenceToCommand = false, uint8_t delayBetweenSequences = CAN_MIN_COMMAND_INTERVAL, bool immediately = false)
{
	CAN_COMMAND ramCmd1, ramCmd2;
	if (cmd1 != NULL)
		memcpy_P(&ramCmd1, cmd1, sizeof(CAN_COMMAND));

	if (cmd2 != NULL)
		memcpy_P(&ramCmd2, cmd2, sizeof(CAN_COMMAND));

	return sendCmdSequence((cmd1 != NULL ? &ramCmd1 : NULL), (cmd2 != NULL ? &ramCmd2 : NULL), sequenceCount, addSequenceToCommand, delayBetweenSequences, immediately);
}
#pragma endregion

#pragma region /** INTERRUPT **/
#if COMPILE_CAN_INTERRUPT      
	void CAN_CONTROLLER::enableInterrupts()
	{		
		uint8_t interrupt = digitalPinToInterrupt(this->intPin);
		if (interrupt == NOT_AN_INTERRUPT)
			return;

		#if CAN_INTERRUPT_MODE == CAN_INTERRUPT_MODE_FETCH

			SPI.usingInterrupt(interrupt);
			attachInterrupt(interrupt, handleInterrupt, LOW);

		#elif CAN_INTERRUPT_MODE == CAN_INTERRUPT_MODE_FLAG

			attachInterrupt(interrupt, handleInterrupt, FALLING);

		#endif
	}

	void CAN_CONTROLLER::disableInterrupts()
	{
		uint8_t interrupt = digitalPinToInterrupt(this->intPin);
		if (interrupt == NOT_AN_INTERRUPT)
			return;

		detachInterrupt(interrupt);
	}
#endif

#pragma endregion

#pragma region /** RX & TX QUEUE **/ 
bool CAN_CONTROLLER::pullRXCmd(CAN_COMMAND *cmd)
{    	
	return rxQueue.pull(cmd);
}

bool CAN_CONTROLLER::pushRXCmd(CAN_COMMAND *cmd)
{
	return this->rxQueue.push(cmd);
}

#if CAN_ENABLE_TX_QUEUE && CAN_TX_QUEUE_LENGTH > 0
	uint8_t CAN_CONTROLLER::getTXQueueAvailableSlots()
	{
		return (CAN_RX_QUEUE_LENGTH - this->rxQueue.nbRecs());
	}
#endif
#pragma endregion
    
#pragma region /** PERIODICAL CMDS **/
#if COMPILE_CAN_PERIODICAL && CAN_PERIODICAL_CMDS_LENGTH > 0
	void CAN_CONTROLLER::addPeriodicalCmd(CAN_COMMAND_PERIODICAL *per, uint16_t initialDelay = 0)
	{    
		unsigned long now = millis();
		this->periodicalCmds[this->periodicalCmdsLength++] = (CAN_COMMAND_PERIODICAL*)per;
		per->nextMillis = (now + per->interval + initialDelay);
	}

	void CAN_CONTROLLER::clearPeriodicalCmds()
	{                
		this->periodicalCmdsLength = 0;  
	}
#endif
#pragma endregion
   
#pragma region /** WHITE LIST **/ 
#if COMPILE_CAN_WHITELIST && CAN_WHITELIST_LENGTH > 0
	bool CAN_CONTROLLER::setupWhitelist()
	{           	
		bool stat = true;
		stat &= (this->CAN->setFilterMask(MCP2515::MASK::MASK0, false, CAN_SFF_MASK) == MCP2515::ERROR_OK);
		stat &= (this->CAN->setFilterMask(MCP2515::MASK::MASK1, false, CAN_SFF_MASK) == MCP2515::ERROR_OK);

		return stat;
	}     
  
	bool CAN_CONTROLLER::setWhitelistID(uint16_t Id, MCP2515::RXF index)
	{	
		return (this->CAN->setFilter(index, false, Id) == MCP2515::ERROR_OK);
	}
#endif
#pragma endregion

#pragma region /** BLACK LIST **/
#if COMPILE_CAN_BLACKLIST && CAN_BLACKLIST_LENGTH > 0
	void CAN_CONTROLLER::addIDToBlacklist(uint16_t Id)
	{    
		this->blacklistIDs[this->blacklistIDsLength++] = Id;
	}

	void CAN_CONTROLLER::clearBlacklist()
	{                
		blacklistIDsLength = 0;  
	}

	uint8_t CAN_CONTROLLER::getBlacklistLength(){ return this->blacklistIDsLength; }
	uint16_t CAN_CONTROLLER::getIDFromBlacklistByIndex(uint8_t index){ return this->blacklistIDs[index]; }
#endif
#pragma endregion

#pragma endregion