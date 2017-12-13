/**
 * This file contains function declarations for converting between byte arrays
 * and 32 bit integers.
 *
 * @author  Ben Lynch
 */

#ifndef __AODV_PACKETS_UTILITY_H__
#define __AODV_PACKETS_UTILITY_H__

#include <stdint.h>
#include <stdint.h>
#include <stddef.h>
#include "nrf_error.h"
#include "ble.h"
#include "SEGGER_RTT.h"

/*packet types */
#define BLE_AODV_RREQ 			0u
#define BLE_AODV_RREP 			1u
#define BLE_AODV_STATS 			2u
/* end packet types */
#define SHIFT_THREE_BYTES 	24
#define SHIFT_TWO_BYTES 		16
#define SHIFT_ONE_BYTE 			8
#define SHIFT_ZERO_BYTES 		0

/**
 * Converts a 32 bit integer (unit_32) into an array of bytes, starting in the
 * byte array at the specified start index.
 *
 * @param bytes       an array of 8 bit integers
 * @param start_indx  the index of the byte array specfying the location for the conversion result
 * @param value       the 32 bit integer being converted
 */
void convert_uint32_to_bytes(uint8_t * bytes, int start_indx, uint32_t value);

/**
 * Converts a byte array of length 4 into a 32 bit integer (uint_32)
 *
 * @param bytes   a byte array of length 4
 * @return        a 32 bit integer containing the concatenation of the byte array values
 */
uint32_t convert_bytes_to_uint32(uint8_t * bytes);

/**
 * A debug function for printing the contents of BLE advertising data which contains
 * aodv packets. 
 *
 * @param bytes   a byte array of length 4
 * @return        a 32 bit integer containing the concatenation of the byte array values
 */
void print_adv_aodv_data(uint8_t * adv_data);

#endif
