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

#include <inttypes.h>
#include <Arduino.h>

typedef enum enumQueueType {
	FIFO = 0,
	LIFO = 1
} QueueType;


/*!	\class Queue Queue.h "Queue/Queue.h"
**	\brief Class containing the required methods for handling the queue
**/
class Queue
{
private:
	QueueType	impl;		//!< Queue implementation: FIFO LIFO
	bool		ovw;		//!< Overwrite previous records when queue is full allowed
	uint16_t	rec_nb;		//!< number of records in the queue
	uint16_t	rec_sz;		//!< Size of a record
	uint8_t *	queue;		//!< Queue start pointer (when allocated)
	
	uint16_t	in;			//!< number of records pushed into the queue
	uint16_t	out;		//!< number of records pulled from the queue (only for FIFO)
	uint16_t	cnt;		//!< number of records not retrieved from the queue
	
public:
	/*!	\brief Queue constructor
	**	\param [in] size_rec - size of a record in the queue
	**	\param [in] nb_recs - number of records in the queue
	**	\param [in] type - Queue implementation type: FIFO, LIFO
	**	\param [in] overwrite - Overwrite previous records when queue is full
	**	\return nothing
	**/
	Queue(uint16_t size_rec, uint16_t nb_recs=20, QueueType type=FIFO, bool overwrite=false);
	
	/*!	\brief Queue desructor: release dynamically allocated queue
	**/
	~Queue();
	
	/*!	\brief Clean queue, restarting from empty queue
	**/
	void clean(void);

	/*!	\brief get emptiness state of the queue
	**	\return Queue emptiness status
	**	\retval true if queue is empty
	**	\retval false is not empty
	**/
	bool isEmpty(void) __attribute__((always_inline)) {
		return (!cnt) ? true : false;
	}
	
	/*!	\brief get fullness state of the queue
	**	\return Queue fullness status
	**	\retval true if queue is full
	**	\retval false is not full
	**/
	bool isFull(void) __attribute__((always_inline)) {
		return (cnt == rec_nb) ? true : false;
	}
	
	/*!	\brief get number of records in the queue
	**	\return Number of records left in the queue
	**/
	uint16_t nbRecs(void) __attribute__((always_inline)) {
		return cnt;
	}
	
	/*!	\brief Push record to queue
	**	\param [in] record - pointer to record to be pushed into queue
	**	\return Push status
	**	\retval true if succefully pushed into queue
	**	\retval false if queue is full
	**/
	bool push(void * record);

	/*!	\brief Pull record from queue
	**	\param [in,out] record - pointer to record to be pulled from queue
	**	\return Pull status
	**	\retval true if succefully pulled from queue
	**	\retval false if queue is empty
	**/
	bool pull(void * record);
};


#endif /* __QUEUE_H__ */
