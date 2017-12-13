/**
 * This file contains functionality for the creation and processing of RREQ packets
 *
 * @author  Ben Lynch
 */

#ifndef __AODV_RREQ_PACKET_H__
#define __AODV_RREQ_PACKET_H__

#include "aodv_packets_utility.h"

/**
 * @brief Contains information related to AODV RREQ packets.
 */
typedef struct rreq_packet{
	uint8_t type;
	uint8_t flags;
	uint8_t hopCount;
	//RREQ id
	uint32_t broadcastId;
	//destination ip address
	uint32_t dstAddress;
	//destination sequence number
	int32_t dstSequenceNum;
	//originator ip address
	uint32_t srcAddress;
	//originator sequence number
	int32_t srcSequenceNum;
} rreq_packet;

/**
 * Converts a high level RREQ packet into the correct format for a BLE adveritising data packet.
 *
 * @param adv_data  a 24 byte array for BLE advertising data
 * @param rreq  		the RREQ packet to be sent over BLE
 */
void convert_rreq_to_adv_data(uint8_t * adv_data, rreq_packet * rreq);

/**
 * Debug function for printing the entire contents of a RREQ packet.
 *
 * @param packet  RREQ packet to be printed
 */
void print_rreq_packet(rreq_packet * packet);

/**
 * Debug function for printing the source an destination address of a RREQ packet.
 *
 * @param packet  RREQ packet to be printed
 */
void print_rreq_src_dst(rreq_packet * packet);

/**
 * Creates a new RREQ packet.
 *
 * @param rreq  					an uninitialised RREQ packet
 * @param broadcastid  		a unique RREQ broadcast ID
 * @param dstAddress  		the destination address of the RREQ
 * @param srcAddress 			the source address of the RREQ
 * @param srcSequenceNum  the known sequence number for the given destination
 */
void generate_rreq_packet(rreq_packet * rreq, uint32_t * broadcastid, uint32_t dstAddress, uint32_t srcAddress, uint32_t srcSequenceNum);

/**
 * Converts BLE advertising packet data into a RREQ packet.
 *
 * @param rreq  		a RREQ packet which will have its values overwritten
 * @param adv_data  BLE advertising data containing a an AODV RREQ packet
 */
void create_rreq_from_advertising(rreq_packet * rreq, uint8_t * adv_data);

/**
 * Copies the contents of one RREQ packet to another.
 *
 * @param old  	The RREQ being copied
 * @param copy  The RREQ being copied to
 */
void copy_rreq(rreq_packet * old, rreq_packet * copy);

#endif //__AODV_RREQ_PACKET_H__
