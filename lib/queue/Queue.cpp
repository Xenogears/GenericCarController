/*!\file Queue.cpp
** \author SMFSW
** \version 1.0
** \date 2017/03/22
** \copyright BSD 3-Clause License (c) 2017, SMFSW
** \brief Queue handling library (designed on Arduino)
** \details Queue handling library (designed on Arduino)
**			This library was designed for Arduino, yet may be compiled without change with gcc for other purporses/targets
**/


#include <string.h>
#include <stdlib.h>

#include "Queue.h"


#define INC_IDX(ctr, end, start)	if (ctr < (end-1))	{ ctr++; }		\
									else				{ ctr = start; }	//!< Increments buffer index \b cnt rolling back to \b start when limit \b end is reached

#define DEC_IDX(ctr, end, start)	if (ctr > (start))	{ ctr--; }		\
									else				{ ctr = end-1; }	//!< Decrements buffer index \b cnt rolling back to \b end when limit \b start is reached


Queue::Queue(uint16_t size_rec, uint16_t nb_recs, QueueType type, bool overwrite)
{
	rec_nb = nb_recs;
	rec_sz = size_rec;
	impl = type;
	ovw = overwrite;
	
	if (queue)	{ free(queue); }	// Free existing data (if any)
	queue = (uint8_t *) malloc(nb_recs * size_rec);

	clean();
}

Queue::~Queue()
{
	free(queue);
}


void Queue::clean(void)
{
	uint8_t oldSREG = SREG;
	noInterrupts();

	in = 0;
	out = 0;
	cnt = 0;

	SREG = oldSREG;
}


bool Queue::push(void * record)
{
	if ((!ovw) && isFull())	{ return false; }
	
	uint8_t oldSREG = SREG;
	noInterrupts();

		uint8_t * pStart = queue + (rec_sz * in);
		memcpy(pStart, record, rec_sz);
	
		INC_IDX(in, rec_nb, 0);
	
		if (!isFull())	{ cnt++; }	// Increase records count
		else if (ovw)				// Queue is full and ovwite is allowed
		{
			if (impl == FIFO)			{ INC_IDX(out, rec_nb, 0); }	// as oldest record is overwriten, increment out
			//else if (impl == LIFO)	{}								// Nothing to do in this case
		}

	SREG = oldSREG;
	
	return true;
}

bool Queue::pull(void * record)
{
	uint8_t * pStart;
	
	if (isEmpty())	{ return false; }	// No more records
	
	uint8_t oldSREG = SREG;
	noInterrupts();

		if (impl == FIFO)
		{
			pStart = queue + (rec_sz * out);
			INC_IDX(out, rec_nb, 0);
		}
		else if (impl == LIFO)
		{
			DEC_IDX(in, rec_nb, 0);
			pStart = queue + (rec_sz * in);
		}
		else	
		{ 
			SREG = oldSREG;
			return false; 
		}
	
		memcpy(record, pStart, rec_sz);
		cnt--;	// Decrease records count

	SREG = oldSREG;

	return true;
}
