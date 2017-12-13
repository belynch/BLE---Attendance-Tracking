#include "aodv_packet_list.h"

/************************************************************************/
/* RREQ */
void add_rreq_to_previously_received(rreq_packet * previous_rreqs, rreq_packet * received_rreq, int * index){
	previous_rreqs[*index].broadcastId = received_rreq->broadcastId;
	previous_rreqs[*index].srcAddress = received_rreq->srcAddress;
	(*index)++;
}

int rreq_lookup(rreq_packet * previous_rreqs, uint32_t srcAdress, uint32_t rreqId){	
	for(int i = 0; i < MAX_RREQS; i++){
		if(previous_rreqs[i].srcAddress == srcAdress && previous_rreqs[i].broadcastId == rreqId){
			//return the index if a matching RREQ is found
			return i;
		}
	}
	//no matching RREQ found
	return UNRECEIVED_RREQ;
}

/************************************************************************/
/* RREP */
void add_rrep_to_previously_received(rrep_packet * previous_rreps, rrep_packet * received_rrep, int * index){
	previous_rreps[*index].dstAddress = received_rrep->dstAddress;
	previous_rreps[*index].srcAddress = received_rrep->srcAddress;
	previous_rreps[*index].dstSequenceNum = received_rrep->dstSequenceNum;
	(*index)++;
}

int rrep_lookup(rrep_packet * previous_rreps, uint32_t dstAddress, uint32_t srcAdress, uint32_t dstSequenceNum){	
	for(int i = 0; i < MAX_RREQS; i++){
		if(previous_rreps[i].srcAddress == srcAdress && previous_rreps[i].dstAddress == dstAddress && previous_rreps[i].dstSequenceNum == dstSequenceNum){
			//return the index if a matching RREP is found
			return i;
		}
	}
	//no matching RREP found
	return UNRECEIVED_RREP;
}

/************************************************************************/
/* STATS */
void add_stats_to_previously_received(stats_packet * previous_stats, stats_packet * received_stats, int * index){
	previous_stats[*index].src_address = received_stats->src_address;
	(*index)++;
}

int stats_lookup(stats_packet * previous_stats, uint32_t srcAdress){
	for(int i = 0; i < MAX_RREQS; i++){
		if(previous_stats[i].src_address == srcAdress){
			//return the index if a matching STATS is found
			return i;
		}
	}
	//no matching STATS found
	return UNRECEIVED_STATS;
}
