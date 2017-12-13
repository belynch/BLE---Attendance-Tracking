#include "aodv_packets_utility.h"
#include "aodv_rrep_packet.h"
#include <string.h>

void convert_rrep_to_adv_data(uint8_t * adv_data, rrep_packet * rrep){
	memset(adv_data, 0, 24);
	adv_data[0] = rrep->type;
	adv_data[1] = rrep->flags;
	adv_data[2] = rrep->hopCount;
	convert_uint32_to_bytes(adv_data, 3, rrep->dstAddress);
	convert_uint32_to_bytes(adv_data, 7, rrep->dstSequenceNum);
	convert_uint32_to_bytes(adv_data, 11, rrep->srcAddress);
	convert_uint32_to_bytes(adv_data, 15, rrep->lifetime);
	convert_uint32_to_bytes(adv_data, 19, rrep->nextHop);
}

void create_rrep_from_advertising(rrep_packet * rrep, uint8_t * adv_data){
	rrep->type 						= adv_data[0];
	rrep->flags						= adv_data[1];
	rrep->hopCount				= adv_data[2];
	rrep->dstAddress			= (adv_data[3] << SHIFT_THREE_BYTES) | (adv_data[4] << SHIFT_TWO_BYTES) | (adv_data[5] << SHIFT_ONE_BYTE) | (adv_data[6] << SHIFT_ZERO_BYTES);
	rrep->dstSequenceNum 	= (adv_data[7] << SHIFT_THREE_BYTES) | (adv_data[8] << SHIFT_TWO_BYTES) | (adv_data[9] << SHIFT_ONE_BYTE) | (adv_data[10] << SHIFT_ZERO_BYTES);
	rrep->srcAddress 			= (adv_data[11] << SHIFT_THREE_BYTES) | (adv_data[12] << SHIFT_TWO_BYTES) | (adv_data[13] << SHIFT_ONE_BYTE) | (adv_data[14] << SHIFT_ZERO_BYTES);
	rrep->lifetime 				= (adv_data[15] << SHIFT_THREE_BYTES) | (adv_data[16] << SHIFT_TWO_BYTES) | (adv_data[17] << SHIFT_ONE_BYTE) | (adv_data[18] << SHIFT_ZERO_BYTES);
	rrep->nextHop 				= (adv_data[19] << SHIFT_THREE_BYTES) | (adv_data[20] << SHIFT_TWO_BYTES) | (adv_data[21] << SHIFT_ONE_BYTE) | (adv_data[22] << SHIFT_ZERO_BYTES);
}

void print_rrep_packet(rrep_packet * packet){
	SEGGER_RTT_printf(0, "%sPrinting RREP Packet\n", RTT_CTRL_BG_BLUE, RTT_CTRL_RESET);
	SEGGER_RTT_printf(0, "%stype: %u\n", RTT_CTRL_BG_BLUE, packet->type, RTT_CTRL_RESET);
	SEGGER_RTT_printf(0, "%sflags: %u\n", RTT_CTRL_BG_BLUE, packet->flags, RTT_CTRL_RESET);
	SEGGER_RTT_printf(0, "%shopCount: %u\n", RTT_CTRL_BG_BLUE, packet->hopCount, RTT_CTRL_RESET);
	SEGGER_RTT_printf(0, "%sdstAddress: %u\n", RTT_CTRL_BG_BLUE, packet->dstAddress, RTT_CTRL_RESET);
	SEGGER_RTT_printf(0, "%sdstSequenceNum: %d\n", RTT_CTRL_BG_BLUE, packet->dstSequenceNum, RTT_CTRL_RESET);
	SEGGER_RTT_printf(0, "%ssrcAddress: %u\n", RTT_CTRL_BG_BLUE, packet->srcAddress, RTT_CTRL_RESET);
	SEGGER_RTT_printf(0, "%slifetime: %u\n", RTT_CTRL_BG_BLUE, packet->lifetime, RTT_CTRL_RESET);
	SEGGER_RTT_printf(0, "%snextHop: %u\n", RTT_CTRL_BG_BLUE, packet->nextHop, RTT_CTRL_RESET);
	SEGGER_RTT_WriteString(0, "\r\n");
}

void print_rrep_src_dst(rrep_packet * packet){
	SEGGER_RTT_printf(0, "%ssrcAddress: %u\n", RTT_CTRL_BG_BLUE, packet->srcAddress, RTT_CTRL_RESET);
	SEGGER_RTT_printf(0, "%snextHop: %u\n", RTT_CTRL_BG_BLUE, packet->nextHop, RTT_CTRL_RESET);
	SEGGER_RTT_printf(0, "%sdstAddress: %u\n", RTT_CTRL_BG_BLUE, packet->dstAddress, RTT_CTRL_RESET);
	SEGGER_RTT_WriteString(0, "\r\n");
}

void generate_rrep_packet(rrep_packet * rrep, uint32_t dstAddress, uint32_t dstSequenceNum, uint32_t srcAddress, uint32_t lifetime, uint32_t nextHop){
	rrep->type = BLE_AODV_RREP;
	rrep->flags = 0;
	rrep->hopCount = 0;
	rrep->dstAddress = dstAddress;
	rrep->dstSequenceNum = dstSequenceNum;
	rrep->srcAddress = srcAddress;
	rrep->lifetime = lifetime;
	rrep->nextHop = nextHop;
}

void copy_rrep(rrep_packet * old, rrep_packet * copy){
	copy->type = old->type;
	copy->flags = old->flags;
	copy->hopCount = old->hopCount;
	copy->dstAddress = old->dstAddress;
	copy->dstSequenceNum = old->dstSequenceNum;
	copy->srcAddress = old->srcAddress;
	copy->lifetime = old->lifetime;
	copy->nextHop = old->nextHop;
}
