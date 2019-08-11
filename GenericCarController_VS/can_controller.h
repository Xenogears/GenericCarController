//
// Author:	Álvaro Graciá Gil
// License:	This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License 
//	 		(http://creativecommons.org/licenses/by-nc-sa/4.0/) 
//

#ifndef __CAN_CONTROLLER__H__
  #define __CAN_CONTROLLER__H__ 

  #include "configuration.h"
  #include "lib/can/mcp2515.h"
  #include "lib/queue/Queue_static.h"  
  #include "can_command.h" 

  class CAN_CONTROLLER
  {
	private:
		MCP2515 *CAN = NULL;

		#if COMPILE_CAN_WHITELIST
			uint8_t whitelistLength;
		#endif
		#if COMPILE_CAN_BLACKLIST && CAN_BLACKLIST_LENGTH > 0
			uint16_t *blacklistIDs[CAN_BLACKLIST_LENGTH];
			uint8_t blacklistIDsLength;
		#endif
		#if COMPILE_CAN_PERIODICAL && CAN_PERIODICAL_CMDS_LENGTH > 0
			CAN_COMMAND_PERIODICAL *periodicalCmds[CAN_PERIODICAL_CMDS_LENGTH];
			uint8_t periodicalCmdsLength;
		#endif

		const uint8_t intPin;

		#if COMPILE_CAN_INTERRUPT   
			static void handleInterrupt();
			
			#if CAN_INTERRUPT_MODE == CAN_INTERRUPT_MODE_FLAG
				volatile uint8_t currentInterruptsReg = 0;
				Queue<CAN_COMMAND, CAN_RX_QUEUE_LENGTH, false> rxQueue;
			#endif

			#if CAN_INTERRUPT_MODE == CAN_INTERRUPT_MODE_FETCH
				Queue<CAN_COMMAND, CAN_RX_QUEUE_LENGTH, true> rxQueue = Queue<CAN_COMMAND, CAN_RX_QUEUE_LENGTH, true>(FIFO, true);
			#endif
		#else
			Queue<CAN_COMMAND, CAN_RX_QUEUE_LENGTH, false> rxQueue;
		#endif

		#if CAN_ENABLE_TX_QUEUE && CAN_TX_QUEUE_LENGTH > 0
			Queue<CAN_COMMAND, CAN_TX_QUEUE_LENGTH, false> txQueue;
			bool pullTXCmd(CAN_COMMAND *cmd);
			bool pushTXCmd(CAN_COMMAND *cmd);
		#endif
		
		#if CAN_ENABLE_MIN_DELAY_BETWEEN_COMMANDS	
			unsigned long nextTXmillis;
		#endif 

		bool readyToSendCmd();
		bool cmdAvailableInRXQueue();
		bool cmdAvailableInController();
		uint8_t CAN_CONTROLLER::fillRXQueueFromCANController(uint8_t maxCmds = CAN_RX_QUEUE_LENGTH);
		bool pullRXCmd(CAN_COMMAND *cmd);
		bool pushRXCmd(CAN_COMMAND *cmd);

		bool fetchCmdFromController(CAN_COMMAND *cmd);
		bool sendCmdToCANController(CAN_COMMAND *cmd, uint8_t nextCmdDelay = CAN_MIN_COMMAND_INTERVAL);
		void(*receiveHandler)(CAN_CONTROLLER *can, CAN_COMMAND *cmd);

		#if COMPILE_CAN_PERIODICAL && CAN_PERIODICAL_CMDS_LENGTH > 0
			void checkPeriodicalCmds();
		#endif

	public:   
		#ifdef CAN_FRIEND_FUNCTION
			friend CAN_FRIEND_FUNCTION;
		#endif

		CAN_CONTROLLER(const uint8_t csPin, const uint8_t intPin, void (*h)(CAN_CONTROLLER*,CAN_COMMAND*));

		void checkCAN();
		void setupPins();
		bool begin(const CAN_SPEED bitrate, const CAN_CLOCK clockSet);

		/** SEND **/      
		bool sendCmd(CAN_COMMAND *cmd, uint8_t extraByte, bool immediately = false);
		bool sendCmd(CAN_COMMAND *cmd, bool immediately = false);
		bool sendCmdSequence(CAN_COMMAND *cmd1, CAN_COMMAND *cmd2, uint8_t sequenceCount = 1, boolean addSequenceToCommand = false, uint8_t delayBetweenSequences = CAN_MIN_COMMAND_INTERVAL, bool immediately = false);
		
		bool sendCmd_P(PROGMEM CAN_COMMAND *cmd, uint8_t extraByte, bool immediately = false);
		bool sendCmd_P(PROGMEM CAN_COMMAND *cmd, bool immediately = false);
		bool sendCmdSequence_P(PROGMEM CAN_COMMAND *cmd1, PROGMEM CAN_COMMAND *cmd2, uint8_t sequenceCount = 1, boolean addSequenceToCommand = false, uint8_t delayBetweenSequences = CAN_MIN_COMMAND_INTERVAL, bool immediately = false);

		#if COMPILE_CAN_INTERRUPT   
			void enableInterrupts();
			void disableInterrupts();
		#endif
    		
		#if CAN_ENABLE_TX_QUEUE && CAN_TX_QUEUE_LENGTH > 0
			uint8_t getTXQueueAvailableSlots();
		#endif

		/** PERIODICAL CMDS **/
		#if COMPILE_CAN_PERIODICAL && CAN_PERIODICAL_CMDS_LENGTH > 0
			void addPeriodicalCmd(CAN_COMMAND_PERIODICAL *per, uint16_t initialDelay = 0);
			void clearPeriodicalCmds();
		#endif
      
		/** WHITE LIST **/ 
		#if COMPILE_CAN_WHITELIST
			bool setupWhitelist();
			bool setWhitelistID(uint16_t Id, MCP2515::RXF index);
		#endif

		/** BLACK LIST **/
		#if COMPILE_CAN_BLACKLIST && CAN_BLACKLIST_LENGTH > 0
			void addIDToBlacklist(uint16_t Id);
			void clearBlacklist();  
			uint8_t getBlacklistLength();
			uint16_t getIDFromBlacklistByIndex(uint8_t index);
		#endif
 };

	#if COMPILE_CAN
		extern CAN_CONTROLLER DEFAULT_CAN;    
	#endif
#endif