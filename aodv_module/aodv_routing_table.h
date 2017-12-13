/**
 * This file contains a structure to represent AODV routing table entries, and a functionality
 * to perform lookups on the oruting table.
 *
 * @author  Ben Lynch
 */

#ifndef __AODV_ROUTING_TABLE_H__
#define __AODV_ROUTING_TABLE_H__

#include <stdint.h>

#define MAX_TABLE_ENTRIES						16
#define ENTRY_NOT_PRESENT						-1

/**
 * @brief Contains information related to each known route in the network.
 */
typedef struct route_table_entry{

	uint32_t dstAddress;
	int32_t dstSeqNumber;
	uint8_t validSeqNumberFlag;
	uint8_t hopCount;
	uint32_t nextHop;
	uint32_t listOfPrecursors;
	uint32_t lifetime;
} route_table_entry;

/**
 * Formats STATS packets for BLE advertising packets and begins BLE advertising
 *
 * @param	routing_table		a pointer to the routing table
 * @param	dstAddress			the address of the device being looked up
 * @return the index of the device if present, otherwise -1
 */
int routing_table_lookup(route_table_entry * routing_table, uint32_t dstAddress);

#endif //__AODV_ROUTING_TABLE_H__
