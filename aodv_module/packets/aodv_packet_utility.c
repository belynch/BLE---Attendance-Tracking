#include "aodv_packets_utility.h"
#include <string.h>

void convert_uint32_to_bytes(uint8_t * bytes, int start_indx, uint32_t value){
	bytes[start_indx+3] = value;       								/* least significant byte (right most) */
	bytes[start_indx+2] = value >> SHIFT_ONE_BYTE;  	/*  */
	bytes[start_indx+1] = value >> SHIFT_TWO_BYTES; 	/*  */
	bytes[start_indx] = value >> SHIFT_THREE_BYTES;  	/* most significant byte (left-most) */
}

uint32_t convert_bytes_to_uint32(uint8_t * bytes){
	return (bytes[0] << SHIFT_THREE_BYTES) | (bytes[1] << SHIFT_TWO_BYTES) | (bytes[2] << SHIFT_ONE_BYTE) | (bytes[3] << SHIFT_ZERO_BYTES);
}

void print_adv_aodv_data(uint8_t * adv_data){
	SEGGER_RTT_printf(0, "%sBytes 0 - 2: %u, %u, %u\n", RTT_CTRL_BG_BLUE, adv_data[0], adv_data[1], adv_data[2], RTT_CTRL_RESET);
	SEGGER_RTT_printf(0, "%sBytes 3 - 6: %u, %u, %u, %u\n", RTT_CTRL_BG_BLUE, adv_data[3], adv_data[4], adv_data[5], adv_data[6], RTT_CTRL_RESET);	
	SEGGER_RTT_printf(0, "%sBytes 7 - 10: %u, %u, %u, %u\n", RTT_CTRL_BG_BLUE, adv_data[7], adv_data[8], adv_data[9], adv_data[10], RTT_CTRL_RESET);	
	SEGGER_RTT_printf(0, "%sBytes 11 - 14: %u, %u, %u, %u\n", RTT_CTRL_BG_BLUE, adv_data[11], adv_data[12], adv_data[13], adv_data[14], RTT_CTRL_RESET);	
	SEGGER_RTT_printf(0, "%sBytes 15 - 18: %u, %u, %u, %u\n", RTT_CTRL_BG_BLUE, adv_data[15], adv_data[16], adv_data[17], adv_data[18], RTT_CTRL_RESET);	
	SEGGER_RTT_printf(0, "%sBytes 19 - 22: %u, %u, %u, %u\n", RTT_CTRL_BG_BLUE, adv_data[19], adv_data[20], adv_data[21], adv_data[22], RTT_CTRL_RESET);		
}

