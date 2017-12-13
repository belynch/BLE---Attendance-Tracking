#include "fifo_queue.h"

void QueueInit(int * queue_in, int * queue_out)
{
    (*queue_in) = (*queue_out) = 0;
}

int QueuePut(rreq_packet * rreq, rrep_packet * rrep, stats_packet * stats, queue_element * queue, int * queue_in, int * queue_out)
{
    if((*queue_in) == (( (*queue_out) - 1 + QUEUE_SIZE) % QUEUE_SIZE))
    {
        return -1; /* Queue Full*/
    }
		//if its a rrep
		if(rreq == NULL && stats == NULL){
			queue[(*queue_in)].type = BLE_AODV_RREP;
			copy_rrep(rrep, &queue[(*queue_in)].rrep);
		//if its a rreq
		}else if (rrep == NULL && stats == NULL){
			queue[(*queue_in)].type = BLE_AODV_RREQ;
			copy_rreq(rreq, &queue[(*queue_in)].rreq);
		//if its a statistics packet
		}else{
			queue[(*queue_in)].type = BLE_AODV_STATS;
			copy_stats(stats, &queue[(*queue_in)].stats);
		}


    (*queue_in) = ((*queue_in) + 1) % QUEUE_SIZE;

    return 0; // No errors
}

int QueueGet(queue_element *old, queue_element * queue, int * queue_in, int * queue_out)
{
    if((*queue_in) == (*queue_out))
    {
        return -1; /* Queue Empty - nothing to get*/
    }

    *old = queue[(*queue_out)];

    (*queue_out) = ((*queue_out) + 1) % QUEUE_SIZE;

    return 0; // No errors
}
