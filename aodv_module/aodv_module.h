/**
 * This file contains the main AODV functionality. It provides methods for sending
 * and processing AODV packets. The aodv_module maintains a nodes routing table,
 * packet buffer, and all the logic required by a node using the AODV protocol.
 *
 * @author  Ben Lynch
 */

#ifndef __AODV_MODULE_H__
#define __AODV_MODULE_H__

#include <stdint.h>
#include "packets/aodv_rreq_packet.h"
#include "packets/aodv_rrep_packet.h"
#include "packets/aodv_stats_packet.h"
#include "../custom_timer.h"
#include "ble.h"
#include "SEGGER_RTT.h"

#define FALSE												0
#define TRUE												1
#define NET_TRAVERSAL_TIME 					500							/** if a RREP isn't received within this time limit, a RREQ can be re-sent */
#define STATISTICS_TIME 						20000						/** the time which a stats packet is sent after receiving the first RREQ */
#define RREQ_RATELIMIT 							1								/** Number of RREQ messages a node can originate per second */
#define RREQ_RETRIES								3								/** Number of times a RREQ can be re-sent */
#define AODV_ADV_PACKET_SIZE				25
#define MAX_PROCESSED_RREQS					16
#define MY_ROUTE_TIMEOUT 						6
#define NUM_DEVICES									9								/** not including root */
#define ROOT_BLE_ADDRESS						3348609015			/** address/id of the root node */

/**
 * @brief Contains a pointer to an active rreq packet and the corresponding net traversal timer.
 */
typedef struct active_rreq{
	rreq_packet rreq;
	app_timer_id_t const * timer_handle;
} active_rreq;

/**
 * Intialises lists and member data for a node. Creates and starts timers for the packet
 * buffer(at every interval i, the packet at the top of the buffer is sent) and the
 * statistics packets (used as an overall application timeout to obtain list of
 * nodes)
 */
void init_aodv_module(void);

/**
 * Resets lists and member data for a node.
 */
void reset_aodv_module(void);

/**
 * Getter function to retrieve a nodes BLE address
 *
 * @return	the nodes 32 bit BLE address
 */
uint32_t get_ble_address(void);

/**
 * Formats AODV RREQ packets for BLE advertising packets and begins BLE advertising
 *
 * @param	rreq	the RREQ packet to be broadcasted
 */
uint8_t send_rreq(rreq_packet * rreq);

/**
 * Formats AODV RREP packets for BLE advertising packets and begins BLE advertising
 *
 * @param	rrep	the RREP packet to be broadcasted
 */
uint8_t send_rrep(rrep_packet * rrep);

/**
 * Formats STATS packets for BLE advertising packets and begins BLE advertising
 *
 * @param	stats	the STATS packet to be broadcasted
 */
uint8_t send_stats(stats_packet * stats);

/**
 * Handles the processing and forwarding of RREQs aswell as the generationg of RREPS.
 * Follows rfc3561 sections 6.5 and 6.6.
 *
 * @param		rreq			the newly received RREQ packet
 * @param		peer_addr	the address the packet was received from
 * @return	1 if successful, otherwise 0
 */
uint32_t process_rreq(rreq_packet * rreq, uint32_t peer_addr);

/**
 * Handles the receiving and forwarding of rreps_sent
 * Follows rfc3561 section 6.7.
 *
 * @param		rrep			the newly received RREP packet
 * @param		peer_addr	the address the packet was received from
 * @return	1 if successful, otherwise 0
 */
uint32_t process_rrep(rrep_packet * rrep, uint32_t peer_addr);

/**
 * Handles the receiving and forwarding of STATS packets. STATS packets
 * operate similarly to RREQ packets. They are forwarded unless the receiving nodes
 * is the destination (the root node in the attendance tracking application)
 *
 * @param		stats			the newly received STATS packet
 * @param		peer_addr	the address the packet was received from
 * @return	1 if successful, otherwise 0
 */
uint32_t process_stats(stats_packet * stats, uint32_t peer_addr);

/**
 * Processes advertising reports dispatched by the BLE stack event handler. This
 * method retrieves the AODV packet data from a BLE advertising packet, identifies
 * what type of AODV packet it has received, and processes the AODV packet based on
 * this.
 *
 * @param		p_adv_report	the advertising report received over BLE advertising
 * @return	1 if successful, otherwise 0
 */
uint32_t process_adv_report(const ble_gap_evt_adv_report_t * p_adv_report);

/**
 * Determines if a device is present (A RREP was received from the device after
 * sending a RREQ to it).
 *
 * @param		device_addr	the BLE address of the device
 * @return	1 if successful, otherwise 0
 */
int is_device_present(uint32_t device_addr);

/**
 * Creates a RREQ packet for each device to be found and adds them to the packet
 * buffer.
 */
void initialse_roll_call_rreqs(void);

/**
 * Creates a single RREQ which is added to the packet buffer.
 */
void initialse_route_to_zero_rreqs(void);

/**
 * Prints the time taken to discover a node in seconds.
 */
void print_node_discovery_times(void);

/**
 * Used to send a stats packet. The time is started after receiving the first_rreq
 * RREQ packet.
 */
void stats_timer_handler(void * p_context);

/**
 * Contains the packet buffer functionality. At a regular interval the top of
 * the packet buffer is retieved and the packet is sent.
 */
void buffer_timer_handler(void * p_context);

#endif //__AODV_MODULE_H__
