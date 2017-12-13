/**
 * This file contains functionality for the creation and processing of STATS packets.
 * STATS packets are not part of the AODV routing protocol but where added for
 * capturing performance statistics.
 *
 * @author  Ben Lynch
 */

#ifndef __AODV_STATS_PACKET_H__
#define __AODV_STATS_PACKET_H__

#include "aodv_packets_utility.h"

/**
 * @brief Contains information related to AODV STATS packets.
 */
typedef struct stats_packet{
	uint8_t type;
	uint8_t rreqs_sent;
	uint8_t rreps_sent;
	uint8_t rreqs_processed;
	uint8_t rreps_processed;
	uint32_t next_hop;
	uint32_t src_address;
} stats_packet;

/**
 * Converts a high level STATS packet into the correct format for a BLE adveritising data packet.
 *
 * @param adv_data  a 24 byte array for BLE advertising data
 * @param stats  		the STATS packet to be sent over BLE
 */
void convert_stats_to_adv_data(uint8_t * adv_data, stats_packet * stats);

/**
 * Debug function for printing the entire contents of a STATS packet.
 *
 * @param packet  STATS packet to be printed
 */
void print_stats_packet(stats_packet * packet);

/**
 * Creates a new STATS packet.
 *
 * @param stats  						an uninitialised STATS packet
 * @param rreqs_sent  			the number of RREQ packets sent from the node
 * @param rreps_sent  			the number of RREP packets sent from the node
 * @param rreqs_processed 	the number of RREQ packets processed at the node
 * @param rreps_processed  	the number of RREP packets processed at the node
 * @param next_hop  				the next hop address for the STATS packet
 * @param src_address  			the address the STATS packet originates from
 */
void generate_stats_packet(stats_packet * stats, uint8_t rreqs_sent, uint8_t rreps_sent, uint8_t rreqs_processed, uint8_t rreps_processed, uint32_t next_hop, uint32_t src_address);

/**
 * Converts BLE advertising packet data into a STATS packet.
 *
 * @param stats  		a STATS packet which will have its values overwritten
 * @param adv_data  BLE advertising data containing a an AODV STATS packet
 */
void create_stats_from_advertising(stats_packet * stats, uint8_t * adv_data);

/**
 * Copies the contents of one STATS packet to another.
 *
 * @param old  	The STATS being copied
 * @param copy  The STATS being copied to
 */
void copy_stats(stats_packet * old, stats_packet * copy);

#endif //__AODV_STATS_PACKET_H__
