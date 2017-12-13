/**
 * This file contains function declarations for managing lists of previously received_rreq
 * packets. These functions comprise of adding a packet to a list, and looking up
 * a packet in a list.
 *
 * @author  Ben Lynch
 */

#ifndef __AODV_PACKET_LIST_H__
#define __AODV_PACKET_LIST_H__

#include "aodv_rreq_packet.h"
#include "aodv_rrep_packet.h"
#include "aodv_stats_packet.h"

#define MAX_RREQS										16
#define UNRECEIVED_RREQ							-1
#define UNRECEIVED_RREP							-1
#define UNRECEIVED_STATS						-1

/**
 * Adds a given RREQ packet to the list containing all previously received RREQ packets
 *
 * @param previous_rreqs  list of all previously received RREQ packets
 * @param received_rreq   a newly received RREQ packet
 * @param index           a global value pointing to the next space in the list of previous RREQs
 */
void add_rreq_to_previously_received(rreq_packet * previous_rreqs, rreq_packet * received_rreq, int * index);

/**
 * Identify whether a RREQ has been previously received. The combination of a source address
 * and RREQ ID will result in the identification of a unique RREQ packet.
 *
 * @param previous_rreqs  list of all previously received RREQ packets
 * @param srcAdress       the source address of a node
 * @param rreqId          the unique identifier for a RREQ packet
 */
int rreq_lookup(rreq_packet * previous_rreqs, uint32_t srcAdress, uint32_t rreqId);

/**
 * Adds a given RREP packet to the list containing all previously received RREP packets
 *
 * @param previous_rreps  list of all previously received RREP packets
 * @param received_rrep   a newly received RREP packet
 * @param index           a global value pointing to the next space in the list of previous RREPs
 */
void add_rrep_to_previously_received(rrep_packet * previous_rreps, rrep_packet * received_rrep, int * index);

/**
 * Identify whether a RREP has been previously received. The combination of a source address
 * and RREP destination sequence number will result in the identification of a unique RREP packet.
 *
 * @param previous_rreps  list of all previously received RREP packets
 * @param srcAdress       the source address of a node
 * @param dstSequenceNum          the unique identifier for a RREP packet
 */
int rrep_lookup(rrep_packet * previous_rreps, uint32_t dstAddress, uint32_t srcAdress, uint32_t dstSequenceNum);

/**
 * Adds a given STATS packet to the list containing all previously received STATS packets
 *
 * @param previous_stats  list of all previously received STATS packets
 * @param received_stats  a newly received STATS packet
 * @param index           a global value pointing to the next space in the list of previous STATS
 */
void add_stats_to_previously_received(stats_packet * previous_stats, stats_packet * received_stats, int * index);

/**
 * Identify whether a STATS has been previously received. Only one STATS packet can be
 * sent from a node meaning the source address is sufficient to determine if a packet
 * has been previously received
 *
 * @param previous_rreps  list of all previously received RREP packets
 * @param srcAdress       the source address of a node
 * @param dstSequenceNum          the unique identifier for a RREP packet
 */
int stats_lookup(stats_packet * previous_stats, uint32_t srcAdress);

#endif // __AODV_PACKET_LIST_H__
