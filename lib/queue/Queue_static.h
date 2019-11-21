/*!\file Queue.h
** \author SMFSW
** \version 1.0
** \date 2017/03/22
** \copyright BSD 3-Clause License (c) 2017, SMFSW
** \brief Queue handling library (designed on Arduino)
** \details Queue handling library (designed on Arduino)
**			This library was designed for Arduino, yet may be compiled without change with gcc for other purporses/targets
**/

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "../../customArduino.h"

typedef enum enumQueueType {
	FIFO = 0,
	LIFO = 1
} QueueType;


#define INC_IDX(ctr, end, start)	if (ctr < (end-1))	{ ctr++; }		\
									else				{ ctr = start; }	//!< Increments buffer index \b cnt rolling back to \b start when limit \b end is reached

#define DEC_IDX(ctr, end, start)	if (ctr > (start))	{ ctr--; }		\
									else				{ ctr = end-1; }	//!< Decrements buffer index \b cnt rolling back to \b end when limit \b start is reached


/*!	\class Queue Queue.h "Queue/Queue.h"
**	\brief Class containing the required methods for handling the queue
**/
template <class T, uint8_t elemCount, bool disableInterrupts = false>
class Queue
{
	private:
		QueueType	impl;		//!< Queue implementation: FIFO LIFO
		bool		ovw;		//!< Overwrite previous records when queue is full allowed
		T			queue[elemCount];
	
		uint8_t		in;			//!< number of records pushed into the queue
		uint8_t		out;		//!< number of records pulled from the queue (only for FIFO)
		uint8_t		cnt;		//!< number of records not retrieved from the queue
	
	public:
		/*!	\brief Queue constructor
		**	\param [in] type - Queue implementation type: FIFO, LIFO
		**	\param [in] overwrite - Overwrite previous records when queue is full
		**	\return nothing
		**/
		Queue(QueueType type=FIFO, bool overwrite=false)
		{
			impl = type;
			ovw = overwrite;

			memset(queue, 0, elemCount * sizeof(T));

			this->clean();
		}

		/*!	\brief get emptiness state of the queue
		**	\return Queue emptiness status
		**	\retval true if queue is empty
		**	\retval false is not empty
		**/
		bool isEmpty(bool di = disableInterrupts) __attribute__((always_inline))
		{
			if (di)
			{
				uint8_t oldSREG = SREG;
				noInterrupts();

					bool r = (!cnt) ? true : false;

				SREG = oldSREG;
				return r;
			}
			else return (!cnt) ? true : false;
		}
	
		/*!	\brief get fullness state of the queue
		**	\return Queue fullness status
		**	\retval true if queue is full
		**	\retval false is not full
		**/
		bool isFull(bool di = disableInterrupts) __attribute__((always_inline))
		{
			if (di)
			{
				uint8_t oldSREG = SREG;
				noInterrupts();

					bool r = (cnt == elemCount) ? true : false;

				SREG = oldSREG;
				return r;
			}
			else return (cnt == elemCount) ? true : false;			
		}
	
		/*!	\brief get number of records in the queue
		**	\return Number of records left in the queue
		**/
		uint8_t nbRecs(void) __attribute__((always_inline)) {
			return cnt;
		}

		/*!	\brief Clean queue, restarting from empty queue
		**/
		void clean()
		{
			uint8_t oldSREG = SREG;
			if (disableInterrupts)
				noInterrupts();			

				in = 0;
				out = 0;
				cnt = 0;

			if (disableInterrupts)
				SREG = oldSREG;
		}

		/*!	\brief Push record to queue
		**	\param [in] record - pointer to record to be pushed into queue
		**	\return Push status
		**	\retval true if succefully pushed into queue
		**	\retval false if queue is full
		**/
		bool push(T *record)
		{
			uint8_t oldSREG = SREG;
			if (disableInterrupts)
				noInterrupts();

				bool s = (ovw || !isFull(false));
				if (s)
				{
					void *pStart = &queue[in];
					memcpy(pStart, record, sizeof(T));

					INC_IDX(in, elemCount, 0);

					if (!isFull()) { cnt++; }	// Increase records count
					else if (ovw)				// Queue is full and ovwite is allowed
					{
						if (impl == FIFO) { INC_IDX(out, elemCount, 0); }	// as oldest record is overwriten, increment out
						//else if (impl == LIFO)	{}					// Nothing to do in this case
					}
				}

			if (disableInterrupts)
				SREG = oldSREG;

			return s;
		}

		/*!	\brief Pull record from queue
		**	\param [in,out] record - pointer to record to be pulled from queue
		**	\return Pull status
		**	\retval true if succefully pulled from queue
		**	\retval false if queue is empty
		**/
		bool pull(T *record)
		{
			void *pStart;

			uint8_t oldSREG = SREG;
			if (disableInterrupts)
				noInterrupts();

				bool e = !isEmpty(false);
				if(e)
				{
					if (impl == FIFO)
					{
						pStart = &queue[out];
						INC_IDX(out, elemCount, 0);
					}
					else if (impl == LIFO)
					{
						DEC_IDX(in, elemCount, 0);
						pStart = &queue[in];
					}
					else
					{
						SREG = oldSREG;
						return false;
					}

					memcpy(record, pStart, sizeof(T));
					cnt--;	// Decrease records count
				}				

			if (disableInterrupts)
				SREG = oldSREG;

			return e;
		}
};


#endif /* __QUEUE_H__ */
