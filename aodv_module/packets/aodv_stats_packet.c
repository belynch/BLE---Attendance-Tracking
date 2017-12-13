#include "aodv_packets_utility.h"
#include "aodv_stats_packet.h"
#include <string.h>

void convert_stats_to_adv_data(uint8_t * adv_data, stats_packet * stats){	
	memset(adv_data, 0, 24);
	adv_data[0] = stats->type;
	adv_data[1] = stats->rreqs_sent;
	adv_data[2] = stats->rreps_sent;
	adv_data[3] = stats->rreqs_processed;
	adv_data[4] = stats->rreps_processed;
	convert_uint32_to_bytes(adv_data, 5, stats->next_hop);
	convert_uint32_to_bytes(adv_data, 9, stats->src_address);
}

void create_stats_from_advertising(stats_packet * stats, uint8_t * adv_data){
	stats->type 								= adv_data[0];
	stats->rreqs_sent						= adv_data[1];
	stats->rreps_sent						= adv_data[2];
	stats->rreqs_processed			= adv_data[3];
	stats->rreps_processed			= adv_data[4];
	stats->next_hop							= (adv_data[5] << SHIFT_THREE_BYTES) | (adv_data[6] << SHIFT_TWO_BYTES) | (adv_data[7] << SHIFT_ONE_BYTE) | (adv_data[8] << SHIFT_ZERO_BYTES);
	stats->src_address					= (adv_data[9] << SHIFT_THREE_BYTES) | (adv_data[10] << SHIFT_TWO_BYTES) | (adv_data[11] << SHIFT_ONE_BYTE) | (adv_data[12] << SHIFT_ZERO_BYTES);
}

void print_stats_packet(stats_packet * packet){
	SEGGER_RTT_printf(0, "%sPrinting STATS Packet\n", RTT_CTRL_BG_BLUE, RTT_CTRL_RESET);
	SEGGER_RTT_printf(0, "%stype: %u\n", RTT_CTRL_BG_BLUE, packet->type, RTT_CTRL_RESET);	
	SEGGER_RTT_printf(0, "%srreqs sent: %u\n", RTT_CTRL_BG_BLUE, packet->rreqs_sent, RTT_CTRL_RESET);	
	SEGGER_RTT_printf(0, "%srreps sent: %u\n", RTT_CTRL_BG_BLUE, packet->rreps_sent, RTT_CTRL_RESET);	
	SEGGER_RTT_printf(0, "%srreqs processed: %u\n", RTT_CTRL_BG_BLUE, packet->rreqs_processed, RTT_CTRL_RESET);		
	SEGGER_RTT_printf(0, "%srreps processed: %u\n", RTT_CTRL_BG_BLUE, packet->rreps_processed, RTT_CTRL_RESET);	
	SEGGER_RTT_printf(0, "%snext hop: %u\n", RTT_CTRL_BG_BLUE, packet->next_hop, RTT_CTRL_RESET);	
	SEGGER_RTT_printf(0, "%ssrc address: %u\n", RTT_CTRL_BG_BLUE, packet->src_address, RTT_CTRL_RESET);	
	SEGGER_RTT_WriteString(0, "\r\n");
}

void generate_stats_packet(stats_packet * stats, uint8_t rreqs_sent, uint8_t rreps_sent, uint8_t rreqs_processed, uint8_t rreps_processed, uint32_t next_hop, uint32_t src_address){
	stats->type 								= BLE_AODV_STATS;
	stats->rreqs_sent 					= rreqs_sent;
	stats->rreps_sent						= rreps_sent;
	stats->rreqs_processed			= rreqs_processed;
	stats->rreps_processed			= rreps_processed;
	stats->next_hop							= next_hop;
	stats->src_address					= src_address;
}

void copy_stats(stats_packet * old, stats_packet * copy){
	copy->type 							= old->type;
	copy->rreqs_sent 				= old->rreqs_sent;
	copy->rreps_sent				= old->rreps_sent;
	copy->rreqs_processed		= old->rreqs_processed;
	copy->rreps_processed		= old->rreps_processed;
	copy->next_hop					= old->next_hop;
	copy->src_address				= old->src_address;
}
