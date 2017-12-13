#include "aodv_rreq_packet.h"
#include <string.h>

void convert_rreq_to_adv_data(uint8_t * adv_data, rreq_packet * rreq){
	memset(adv_data, 0, 23);
	adv_data[0] = rreq->type;
	adv_data[1] = rreq->flags;
	adv_data[2] = rreq->hopCount;
	convert_uint32_to_bytes(adv_data, 3, rreq->broadcastId);
	convert_uint32_to_bytes(adv_data, 7, rreq->dstAddress);
	convert_uint32_to_bytes(adv_data, 11, rreq->dstSequenceNum);
	convert_uint32_to_bytes(adv_data, 15, rreq->srcAddress);
	convert_uint32_to_bytes(adv_data, 19, rreq->srcSequenceNum);	
}

void create_rreq_from_advertising(rreq_packet * rreq, uint8_t * adv_data){
	rreq->type 						= adv_data[0];
	rreq->flags						= adv_data[1];
	rreq->hopCount				= adv_data[2];
	rreq->broadcastId 		= (adv_data[3] << SHIFT_THREE_BYTES) | (adv_data[4] << SHIFT_TWO_BYTES) | (adv_data[5] << SHIFT_ONE_BYTE) | (adv_data[6] << SHIFT_ZERO_BYTES);
	rreq->dstAddress			= (adv_data[7] << SHIFT_THREE_BYTES) | (adv_data[8] << SHIFT_TWO_BYTES) | (adv_data[9] << SHIFT_ONE_BYTE) | (adv_data[10] << SHIFT_ZERO_BYTES);
	rreq->dstSequenceNum 	= (adv_data[11] << SHIFT_THREE_BYTES) | (adv_data[12] << SHIFT_TWO_BYTES) | (adv_data[13] << SHIFT_ONE_BYTE) | (adv_data[14] << SHIFT_ZERO_BYTES);
	rreq->srcAddress 			= (adv_data[15] << SHIFT_THREE_BYTES) | (adv_data[16] << SHIFT_TWO_BYTES) | (adv_data[17] << SHIFT_ONE_BYTE) | (adv_data[18] << SHIFT_ZERO_BYTES);
	rreq->srcSequenceNum 	= (adv_data[19] << SHIFT_THREE_BYTES) | (adv_data[20] << SHIFT_TWO_BYTES) | (adv_data[21] << SHIFT_ONE_BYTE) | (adv_data[22] << SHIFT_ZERO_BYTES);
}

void print_rreq_packet(rreq_packet * packet){
	SEGGER_RTT_printf(0, "%sPrinting RREQ Packet\n", RTT_CTRL_BG_BLUE, RTT_CTRL_RESET);
	SEGGER_RTT_printf(0, "%stype: %u\n", RTT_CTRL_BG_BLUE, packet->type, RTT_CTRL_RESET);	
	SEGGER_RTT_printf(0, "%sflags: %u\n", RTT_CTRL_BG_BLUE, packet->flags, RTT_CTRL_RESET);	
	SEGGER_RTT_printf(0, "%shopCount: %u\n", RTT_CTRL_BG_BLUE, packet->hopCount, RTT_CTRL_RESET);	
	SEGGER_RTT_printf(0, "%sbroadcastId: %u\n", RTT_CTRL_BG_BLUE, packet->broadcastId, RTT_CTRL_RESET);
	SEGGER_RTT_printf(0, "%sdstAddress: %u\n", RTT_CTRL_BG_BLUE, packet->dstAddress, RTT_CTRL_RESET);	
	SEGGER_RTT_printf(0, "%sdstSequenceNum: %d\n", RTT_CTRL_BG_BLUE, packet->dstSequenceNum, RTT_CTRL_RESET);	
	SEGGER_RTT_printf(0, "%ssrcAddress: %u\n", RTT_CTRL_BG_BLUE, packet->srcAddress, RTT_CTRL_RESET);	
	SEGGER_RTT_printf(0, "%ssrcSequenceNum: %d\n", RTT_CTRL_BG_BLUE, packet->srcSequenceNum, RTT_CTRL_RESET);	
	SEGGER_RTT_WriteString(0, "\r\n");
}
void print_rreq_src_dst(rreq_packet * packet){
	SEGGER_RTT_printf(0, "%sdstAddress: %u\n", RTT_CTRL_BG_BLUE, packet->dstAddress, RTT_CTRL_RESET);	
	SEGGER_RTT_printf(0, "%ssrcAddress: %u\n", RTT_CTRL_BG_BLUE, packet->srcAddress, RTT_CTRL_RESET);	
	SEGGER_RTT_WriteString(0, "\r\n");
}

void generate_rreq_packet(rreq_packet * rreq, uint32_t * broadcastid, uint32_t dstAddress, uint32_t srcAddress, uint32_t srcSequenceNum){
	rreq->broadcastId = *broadcastid;
	rreq->type = BLE_AODV_RREQ;
	rreq->flags = 0;
	rreq->hopCount = 0;
	rreq->dstAddress = dstAddress;
	rreq->dstSequenceNum = 0;
	rreq->srcAddress = srcAddress;
	rreq->srcSequenceNum = srcSequenceNum;
}

void copy_rreq(rreq_packet * old, rreq_packet * copy){
	copy->type = old->type;
	copy->flags = old->flags;
	copy->hopCount = old->hopCount;
	copy->broadcastId = old->broadcastId;
	copy->dstAddress = old->dstAddress;
	copy->dstSequenceNum = old->dstSequenceNum;
	copy->srcAddress = old->srcAddress;
	copy->srcSequenceNum = old->srcSequenceNum;
}
