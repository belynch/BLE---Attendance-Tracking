//http://stackoverflow.com/questions/215557/how-do-i-implement-a-circular-list-ring-buffer-in-c
/* Implementation of a FIFO queue which discards the new data when full.
 *
 * Queue is empty when in == out.
 * If in != out, then
 *  - items are placed into in before incrementing in
 *  - items are removed from out before incrementing out
 * Queue is full when in == (out-1 + QUEUE_SIZE) % QUEUE_SIZE;
 *
 * The queue will hold QUEUE_ELEMENTS number of items before the
 * calls to QueuePut fail.
 * @see http://stackoverflow.com/questions/215557/how-do-i-implement-a-circular-list-ring-buffer-in-c
 */
#ifndef __FIFO_QUEUE_H__
#define __FIFO_QUEUE_H__

#include "packets/aodv_rreq_packet.h"
#include "packets/aodv_rrep_packet.h"
#include "packets/aodv_stats_packet.h"

typedef struct queue_element{
	uint8_t type;
	rreq_packet rreq;
	rrep_packet rrep;
	stats_packet stats;
} queue_element;

/* Queue structure */
#define QUEUE_ELEMENTS 32
#define QUEUE_SIZE (QUEUE_ELEMENTS + 1)

void QueueInit(int * queue_in, int * queue_out);

int QueuePut(rreq_packet * rreq, rrep_packet * rrep, stats_packet * stats, queue_element * queue, int * queue_in, int * queue_out);

int QueueGet(queue_element *old, queue_element * queue, int * queue_in, int * queue_out);

#endif //__FIFO_QUEUE_H__
