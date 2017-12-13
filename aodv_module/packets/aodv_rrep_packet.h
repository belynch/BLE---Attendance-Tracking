/**
 * This file contains functionality for the creation and processing of RREP packets
 *
 * @author  Ben Lynch
 */

#ifndef __AODV_RREP_PACKET_H__
#define __AODV_RREP_PACKET_H__

#include "aodv_packets_utility.h"

/**
 * @brief Contains information related to AODV RREP packets.
 */
typedef struct rrep_packet{
	uint8_t type;
	uint8_t flags;
	uint8_t hopCount;
	//destination ip address
	uint32_t dstAddress;
	//destination sequence number
	int32_t dstSequenceNum;
	//originator ip address
	uint32_t srcAddress;
	//the length of time the packet has been alive for
	uint32_t lifetime;
	//address of the next node on the route to the destination
	uint32_t nextHop;
} rrep_packet;

/**
 * Converts a high level RREP packet into the correct format for a BLE adveritising data packet.
 *
 * @param adv_data  a 24 byte array for BLE advertising data
 * @param rrep  		the RREP packet to be sent over BLE
 */
void convert_rrep_to_adv_data(uint8_t * adv_data, rrep_packet * rrep);

/**
 * Debug function for printing the entire contents of a RREP packet.
 *
 * @param packet  RREP packet to be printed
 */
void print_rrep_packet(rrep_packet * packet);

/**
 * Debug function for printing the source an destination address of a RREP packet.
 *
 * @param packet  RREP packet to be printed
 */
void print_rrep_src_dst(rrep_packet * packet);

/**
 * Creates a new RREP packet.
 *
 * @param rrep  					an uninitialised RREP packet
 * @param dstAddress  		the destination address of the RREP
 * @param dstSequenceNum  the destination sequence number of the RREP
 * @param srcAddress 			the source address of the RREP
 * @param lifetime  			the number of hops a packet will live for
 * @param nextHop  				the address of the next hop for the RREP
 */
void generate_rrep_packet(rrep_packet * rrep, uint32_t dstAddress, uint32_t dstSequenceNum, uint32_t srcAddress, uint32_t lifetime, uint32_t nextHop);

/**
 * Converts BLE advertising packet data into a RREP packet.
 *
 * @param rrep  		a RREP packet which will have its values overwritten
 * @param adv_data  BLE advertising data containing a an AODV RREP packet
 */
void create_rrep_from_advertising(rrep_packet * rrep, uint8_t * adv_data);

/**
 * Copies the contents of one RREP packet to another.
 *
 * @param old  	The RREP being copied
 * @param copy  The RREP being copied to
 */
void copy_rrep(rrep_packet * old, rrep_packet * copy);

#endif //__AODV_RREP_PACKET_H__
