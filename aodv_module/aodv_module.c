#include "aodv_module.h"
#include "aodv_routing_table.h"
#include "packets/aodv_packet_list.h"
#include "fifo_queue.h"
#include "../ble_custom_advertising.h"
#include "../ble_custom_scanning.h"
#include "../custom_timer.h"
#include "nrf_error.h"
#include <string.h>
#include "SEGGER_RTT.h"

static uint8_t	m_adv_data[AODV_ADV_PACKET_SIZE];
static uint32_t m_ble_address;
//AODV variables
static int32_t m_sequence_num;
static uint32_t m_broadcast_id;
//routing table
static route_table_entry m_routing_table[MAX_TABLE_ENTRIES];
static int m_next_entry_indx; //this should always point to the next free index
/***********************************************************/
//These arrays hold previously received data packets
//processed RREQs
static rreq_packet m_received_rreqs[MAX_PROCESSED_RREQS];
static int m_next_rreq_indx; //this should always point to the next free index
//processed RREPs
static rrep_packet m_received_rreps[MAX_PROCESSED_RREQS];
static int m_next_rrep_indx; //this should always point to the next free index
//processed STATS
static stats_packet m_received_stats[MAX_PROCESSED_RREQS];
static int m_next_stats_indx; //this should always point to the next free index
/***********************************************************/
static uint32_t m_devices[NUM_DEVICES] = {1175370077, 4128504232, 3971270553, 3071807003, 2014370772, 508664203, 3612749982, 1, 1}; 
static uint32_t m_found_devices[NUM_DEVICES] = {0};
static int m_next_found_device_index = 0;
/***********************************************************/
//These variables are for statistics gathering
static uint32_t m_rreqs_sent = 0;
static uint32_t m_rreps_sent = 0;
static uint32_t m_rreqs_received = 0;
static uint32_t m_rreqs_processed = 0;
static uint32_t m_rreps_received = 0;
static uint32_t m_rreps_processed = 0;
static double m_time_of_node_discovery[NUM_DEVICES] = {0.0f};
static int m_next_discovered_node_index = 0;
/***********************************************************/
//Packet buffer is a FIFO queue
static queue_element m_queue[QUEUE_SIZE] = {0};
static int m_queueIn, m_queueOut;
/***********************************************************/

uint32_t start_ticks;
uint32_t end_ticks;

uint32_t first_rreq = 0;

APP_TIMER_DEF(m_stats_timer_id);
APP_TIMER_DEF(m_buffer_timer_id);

void init_aodv_module(){	
	//api call to get the 48bit address
	ble_gap_addr_t this_address;
	sd_ble_gap_address_get(&this_address);
	
	//we'er cuting off the last 16 bits because AODV address size is limited to 32 bits
	uint8_t * address_array = this_address.addr;
	m_ble_address = convert_bytes_to_uint32(address_array);
	
	m_next_entry_indx = 0; //no entries
	m_sequence_num = 0; //initlaise sequence number
	m_broadcast_id = 1;
	
	app_timer_create(&m_buffer_timer_id,APP_TIMER_MODE_REPEATED,  buffer_timer_handler);
	app_timer_start(m_buffer_timer_id, APP_TIMER_TICKS(NET_TRAVERSAL_TIME, APP_TIMER_PRESCALER), NULL);
	
	app_timer_create(&m_stats_timer_id,APP_TIMER_MODE_SINGLE_SHOT,  stats_timer_handler);
	app_timer_cnt_get(&start_ticks);
	
	SEGGER_RTT_printf(0, "%sThis device's address: %u\n", RTT_CTRL_BG_MAGENTA, m_ble_address, RTT_CTRL_RESET);
	
}

void reset_aodv_module(){
	SEGGER_RTT_printf(0, "%s[APPL] RESETTING AODV MODULE\n", RTT_CTRL_BG_MAGENTA, RTT_CTRL_RESET);
	memset(m_routing_table, 0, MAX_TABLE_ENTRIES * sizeof *m_routing_table);
	m_next_entry_indx = 0;
	memset(m_received_rreqs, 0, MAX_PROCESSED_RREQS * sizeof *m_received_rreqs);
	m_next_rreq_indx = 0;
	m_next_rrep_indx = 0;
	m_next_stats_indx = 0;
	m_sequence_num = 0;
	m_broadcast_id = 1;
}

uint32_t get_ble_address(){
	return m_ble_address;
}

uint8_t send_rreq(rreq_packet * rreq){
	SEGGER_RTT_printf(0, "%s[AODV_MODULE] Sending RREQ\n", RTT_CTRL_BG_BRIGHT_MAGENTA, RTT_CTRL_RESET);
	print_rreq_src_dst(rreq);
	
	m_rreqs_sent++;
	uint32_t err_code;
	
	//populate ble advertising array from rreq struct
	convert_rreq_to_adv_data(m_adv_data, rreq);
	
	//broadcast the packet
	err_code = set_manufacturer_adv_data(m_adv_data, 31);
	if(err_code != NRF_SUCCESS){
		SEGGER_RTT_printf(0, "%sFailed to set adv data: %d\n", RTT_CTRL_BG_BRIGHT_RED, err_code, RTT_CTRL_RESET);
		return err_code;
	}
	
	err_code = custom_advertising_start();
	if(err_code != NRF_SUCCESS){
		SEGGER_RTT_printf(0, "%sFailed to start advertising: %d\n", RTT_CTRL_BG_BRIGHT_RED, err_code, RTT_CTRL_RESET);
		return err_code;
	}
	return NRF_SUCCESS;
}

uint8_t send_rrep(rrep_packet * rrep){
	SEGGER_RTT_printf(0, "%s[AODV_MODULE] Sending RREP\n", RTT_CTRL_BG_BRIGHT_MAGENTA, RTT_CTRL_RESET);
	print_rrep_src_dst(rrep);

	m_rreps_sent++;
	uint32_t err_code;
	
	//populate ble advertising array from rreq struct
	convert_rrep_to_adv_data(m_adv_data, rrep);
	
	//broadcast the packet
	err_code = set_manufacturer_adv_data(m_adv_data, 31);
	custom_advertising_start();
	
	if(err_code != NRF_SUCCESS){
		SEGGER_RTT_printf(0, "%sFailed to set adv data: %d\n", RTT_CTRL_BG_BRIGHT_RED, err_code, RTT_CTRL_RESET);
		return err_code;
	}	
	return NRF_SUCCESS;
}

uint8_t send_stats(stats_packet * stats){
	SEGGER_RTT_printf(0, "%s[AODV_MODULE] Sending STATS\n", RTT_CTRL_BG_BRIGHT_MAGENTA, RTT_CTRL_RESET);
	//print_stats_packet(stats);
	
	uint32_t err_code;
	
	//populate ble advertising array from rreq struct
	convert_stats_to_adv_data(m_adv_data, stats);
	
	//broadcast the packet
	err_code = set_manufacturer_adv_data(m_adv_data, 31);
	custom_advertising_start();
	
	if(err_code != NRF_SUCCESS){
		SEGGER_RTT_printf(0, "%sFailed to set adv data: %d\n", RTT_CTRL_BG_BRIGHT_RED, err_code, RTT_CTRL_RESET);
		return err_code;
	}	
	return NRF_SUCCESS;
}

/**
 *
 * rfc3561 - 6.5. Processing and Forwarding Route Requests
 * rfc3561 - 6.6. Route Reply Generation by the Destination	
 * 		gratuitous RREPs not implemented
 *
 */
uint32_t process_rreq(rreq_packet * rreq, uint32_t peer_addr){
	
	m_rreqs_received++;
	//ignore RREQs that you sent and that are being rebroadcast by nearby nodes
	if(rreq->srcAddress == m_ble_address){
		return NRF_SUCCESS;
	}
	//start the timer for statistics on the first RREQ received
	if(first_rreq == 0){
		SEGGER_RTT_printf(0, "%s[AODV_MODULE] starting stats timer\n", RTT_CTRL_BG_GREEN, RTT_CTRL_RESET);
		app_timer_start(m_stats_timer_id, APP_TIMER_TICKS(STATISTICS_TIME, APP_TIMER_PRESCALER), NULL);
		first_rreq = 1;
	}

	/********************************************************************************/
	//DUPLICATE RREQ
	//determine whether it has received this RREQ before
	int indx = rreq_lookup(m_received_rreqs, rreq->srcAddress, rreq->broadcastId);
	//add to history if previously unreceived
	if(indx == UNRECEIVED_RREQ){
		add_rreq_to_previously_received(m_received_rreqs, rreq, &m_next_rreq_indx);
		m_rreqs_processed++;
	}
	//silently discards the newly received RREQ if it has already been received
	else{
		return NRF_SUCCESS;
	}
	
	/********************************************************************************/
	//FORWARD ROUTE SETUP
	//check if routing table entry already exists for previous hop
	indx = routing_table_lookup(m_routing_table, peer_addr);
	//creates or updates a route to the previous hop without a valid sequence number
	if(indx == ENTRY_NOT_PRESENT){
		m_routing_table[m_next_entry_indx].dstAddress = peer_addr;
		m_routing_table[m_next_entry_indx].validSeqNumberFlag = 0;
		m_routing_table[m_next_entry_indx].dstSeqNumber = -1;
		m_routing_table[m_next_entry_indx].nextHop = peer_addr;
		m_next_entry_indx++;
	}
	
	/********************************************************************************/
	//increments the hop count value in the RREQ by one
	rreq->hopCount = rreq->hopCount + 1;
	
	/********************************************************************************/
	//REVERSE ROUTE SETUP
	indx = routing_table_lookup(m_routing_table, rreq->srcAddress);
	//create reverse route if doesnt already exists
	if(indx == ENTRY_NOT_PRESENT){
		m_routing_table[m_next_entry_indx].dstAddress = rreq->srcAddress;
		m_routing_table[m_next_entry_indx].validSeqNumberFlag = 1;
		m_routing_table[m_next_entry_indx].hopCount = rreq->hopCount;
		m_routing_table[m_next_entry_indx].nextHop = peer_addr;
		m_routing_table[m_next_entry_indx].dstSeqNumber = rreq->srcSequenceNum;
		m_next_entry_indx++;
	}
	//update reverse route
	else{
		//overite entry sequence number if rreq originator sequence number is greater
		if(m_routing_table[indx].dstSeqNumber < rreq->dstSequenceNum){
			m_routing_table[indx].dstSeqNumber = rreq->dstSequenceNum;
			m_routing_table[indx].nextHop = peer_addr;
			m_routing_table[indx].hopCount = rreq->hopCount;
		}
	}
	
	//if this is the Route-to-Zero wake-up packet
	if(rreq->srcAddress == rreq->dstAddress){
		SEGGER_RTT_printf(0, "%s[AODV_MODULE] RTZ wake-up received\n", RTT_CTRL_BG_BRIGHT_GREEN, RTT_CTRL_RESET);
		//we first want to re-broadcast the packet
		QueuePut(rreq, NULL, NULL, m_queue, &m_queueIn, &m_queueOut);
		
		//we then want to generate and send a RREP
		rrep_packet rrep;
		memset(&rrep, 0, sizeof(rrep));
		generate_rrep_packet(&rrep, rreq->srcAddress, m_sequence_num, m_ble_address, MY_ROUTE_TIMEOUT, peer_addr);
		QueuePut(NULL, &rrep, NULL, m_queue, &m_queueIn, &m_queueOut);
		return NRF_SUCCESS;
	}
	
	/********************************************************************************/
	//RREP GENERATION
	indx = routing_table_lookup(m_routing_table, rreq->dstAddress);
	//this is the destination - we dont have a routing table entry for ourselves so indx = -1
	if(rreq->dstAddress == m_ble_address){
		SEGGER_RTT_printf(0, "%s[AODV_MODULE] RREQ dst - Generating RREP\n", RTT_CTRL_BG_BRIGHT_GREEN, RTT_CTRL_RESET);
		rrep_packet rrep;
		memset(&rrep, 0, sizeof(rrep));
		
		if(rreq->dstSequenceNum == (m_sequence_num + 1)){
			m_sequence_num++;
		}
		generate_rrep_packet(&rrep, rreq->srcAddress, m_sequence_num, m_ble_address, MY_ROUTE_TIMEOUT, peer_addr);
		QueuePut(NULL, &rrep, NULL, m_queue, &m_queueIn, &m_queueOut);
	}
	//no idea about the destination (we aren't the destination and it's not in our table) -> forward RREQ
	else{
		SEGGER_RTT_printf(0, "%s[AODV_MODULE] Forwarding RREQ\n", RTT_CTRL_BG_GREEN, RTT_CTRL_RESET);
		QueuePut(rreq, NULL, NULL, m_queue, &m_queueIn, &m_queueOut);
	}
	return NRF_SUCCESS;
}

/**
 *
 * rfc3561 - 6.7. Receiving and Forwarding Route Replies
 *
 */
uint32_t process_rrep(rrep_packet * rrep, uint32_t peer_addr){
	
	m_rreps_received++;
	//RREPs are designed as unicast messages. Because BLE advertising is multicast the RREP
	//packet was modified to include a nextHop address to emulate unicast communication.
	//nearby nodes will receive all RREPs but will ignore them unless they are part of the route.
	if(rrep->nextHop != m_ble_address){
		return NRF_SUCCESS;
	}
	
	/********************************************************************************/
	//DUPLICATE RREP - not in the RFC but required because in BLE, RREPs are broadcast (unicast in RFC)
	//determine whether it has received this RREP before
	int indx = rrep_lookup(m_received_rreps, rrep->dstAddress, rrep->srcAddress, rrep->dstSequenceNum);
	//add to history if previously unreceived
	if(indx == UNRECEIVED_RREP){
		add_rrep_to_previously_received(m_received_rreps, rrep, &m_next_rrep_indx);
		m_rreps_processed++;
	}
	//silently discards the newly received RREQ if it has already been received
	else{
		return NRF_SUCCESS;
	}
	
	//the node then increments the hop count value in the RREP by one, to account for the new hop through the
  //intermediate node
	rrep->hopCount = rrep->hopCount + 1;
	
	/********************************************************************************/
	//PREVIOUS HOP SETUP
	//determine if route to previous hop exists
	indx = routing_table_lookup(m_routing_table, peer_addr);
	//the route to previous hop is created if it does not already exist
	if(indx == ENTRY_NOT_PRESENT){
		m_routing_table[m_next_entry_indx].dstAddress = peer_addr;
		m_routing_table[m_next_entry_indx].validSeqNumberFlag = 0;
		m_routing_table[m_next_entry_indx].dstSeqNumber = -1;
		m_routing_table[m_next_entry_indx].nextHop = peer_addr;
		m_next_entry_indx++;
	}
	/********************************************************************************/
	//FORWARD ROUTE SETUP
	//determine if route to  RREP src exists
	indx = routing_table_lookup(m_routing_table, rrep->srcAddress);
	//the forward route for this destination is created if it does not already exist
	if(indx == ENTRY_NOT_PRESENT){
		m_routing_table[m_next_entry_indx].dstAddress = rrep->srcAddress;
		m_routing_table[m_next_entry_indx].validSeqNumberFlag = 0;
		m_routing_table[m_next_entry_indx].dstSeqNumber = -1;
		m_next_entry_indx++;
	}
	//otherwise, compare message dstSeqNum with the node's table's seqNum and update entry if:
	//(i)       the sequence number in the routing table is marked as
  //          invalid in route table entry.
  //
  //(ii)      the Destination Sequence Number in the RREP is greater than
  //          the node's copy of the destination sequence number and the
  //          known value is valid, or
	//
  //(iii)     the sequence numbers are the same, and the New Hop Count is
  //          smaller than the hop count in route table entry.
	else{
		if((m_routing_table[indx].validSeqNumberFlag == 0)
			|| (rrep->dstSequenceNum > m_routing_table[indx].dstSeqNumber && m_routing_table[indx].validSeqNumberFlag == 1)
			|| (rrep->dstSequenceNum == m_routing_table[indx].dstSeqNumber && m_routing_table[indx].hopCount > rrep->hopCount)){
			m_routing_table[indx].validSeqNumberFlag = 1;
			m_routing_table[indx].nextHop = peer_addr;
			m_routing_table[indx].hopCount = rrep->hopCount;
			m_routing_table[indx].dstSeqNumber = rrep->dstSequenceNum;
		}
	}
	
	/********************************************************************************/
	//RREP FORWARDING
	//this is the destination -> no need to forward 
	if(rrep->dstAddress == m_ble_address){
		//only print if we haven't already ( we might receive the same RREP multiple times)
		if(is_device_present(rrep->srcAddress) == FALSE){
			m_found_devices[m_next_found_device_index] = rrep->srcAddress;
			m_next_found_device_index++;
			SEGGER_RTT_printf(0, "%sDEVICE [%u] IS PRESENT\n", RTT_CTRL_BG_BRIGHT_GREEN, rrep->srcAddress, RTT_CTRL_RESET);
			//calculate time taken to discover
			app_timer_cnt_get(&end_ticks);
			float elapsed_time = elapsed_time_from_ticks(start_ticks, end_ticks);
			m_time_of_node_discovery[m_next_discovered_node_index] = elapsed_time;
			m_next_discovered_node_index++;
		}
		if(m_next_found_device_index == NUM_DEVICES){
			SEGGER_RTT_printf(0, "%sALL DEVICES FOUND\n", RTT_CTRL_BG_BRIGHT_GREEN, RTT_CTRL_RESET);
			//print the statistics of the root node
			stats_packet stats;
			generate_stats_packet(&stats, m_rreqs_sent, m_rreps_sent, m_rreqs_processed, m_rreps_processed, 0, 0);
			print_stats_packet(&stats);
			//If all devices are found, check the current tick and caluclate the elapsed time.
			app_timer_cnt_get(&end_ticks);
			float elapsed_time = elapsed_time_from_ticks(start_ticks, end_ticks);
			char my_string[100];
			sprintf (my_string, "TIME: %f\r\n", elapsed_time);
			SEGGER_RTT_WriteString(0, my_string);
		}
	}
	//this is not the destination -> forward to the next hop in table entry for the destination
	else{
		SEGGER_RTT_printf(0, "%s[AODV_MODULE] Forwarding RREP\n", RTT_CTRL_BG_GREEN, RTT_CTRL_RESET);
		indx = routing_table_lookup(m_routing_table, rrep->dstAddress);
		rrep->nextHop = m_routing_table[indx].nextHop;
		QueuePut(NULL, rrep, NULL, m_queue, &m_queueIn, &m_queueOut);
	}
	return NRF_SUCCESS;
}

uint32_t process_stats(stats_packet * stats, uint32_t peer_addr){
	uint32_t err_code = NRF_SUCCESS;

	if(stats->next_hop != m_ble_address){
		return NRF_SUCCESS;
	}
	/********************************************************************************/
	//DUPLICATE STATS
	//determine whether it has received this STATS before
	int indx = stats_lookup(m_received_stats, stats->src_address);
	//add to history if previously unreceived
	if(indx == UNRECEIVED_STATS){
		add_stats_to_previously_received(m_received_stats, stats, &m_next_stats_indx);
	}
	//silently discards the newly received STATS if it has already been received
	else{
		return NRF_SUCCESS;
	}
	
	/***********************************************************************************/
	//FORWARD STATS
	//forward the stats packet if this isn't the root
	if(m_ble_address != ROOT_BLE_ADDRESS){
		int indx = routing_table_lookup(m_routing_table, ROOT_BLE_ADDRESS);
		if(indx == ENTRY_NOT_PRESENT){
			//this shouldn't happen
			SEGGER_RTT_printf(0, "%s[AODV_MODULE] Next hop for stats packet not found\n", RTT_CTRL_BG_GREEN, RTT_CTRL_RESET);
			err_code = NRF_ERROR_INVALID_STATE;
		}
		else
		{
			SEGGER_RTT_printf(0, "%s[AODV_MODULE] STATS forwarded\n", RTT_CTRL_BG_GREEN, RTT_CTRL_RESET);
			stats->next_hop = m_routing_table[indx].nextHop;
			QueuePut(NULL, NULL, stats, m_queue, &m_queueIn, &m_queueOut);
		}
	}
	//this is the root, print the packet
	else{
		SEGGER_RTT_printf(0, "%s[AODV_MODULE] STATS received at root\n", RTT_CTRL_BG_GREEN, RTT_CTRL_RESET);
		print_stats_packet(stats);
	}
	return err_code;
}

int is_device_present(uint32_t device_addr){
	
	for(int i = 0; i < NUM_DEVICES; i++){
		if(m_found_devices[i] == device_addr){
			return TRUE;
		}
	}
	return FALSE;
}

/**
 * @brief Processes advertising reports dispatched by the BLE stack event handler.
 */
uint32_t process_adv_report(const ble_gap_evt_adv_report_t * p_adv_report){
		uint32_t err_code = NRF_SUCCESS;
		
		uint8_t * adv_data = (uint8_t *)p_adv_report->data;
		uint8_t * peer_address_bytes = (uint8_t *)p_adv_report->peer_addr.addr;
		uint32_t peer_address = convert_bytes_to_uint32(peer_address_bytes);
		uint8_t * aodv_data = &adv_data[7];
			
		rreq_packet rreq;
		rrep_packet rrep;
		stats_packet stats;

		uint8_t packet_type = aodv_data[0];
		
		switch(packet_type)
		{
			case(BLE_AODV_RREQ):
				memset(&rreq, 0, sizeof(rreq));
				create_rreq_from_advertising(&rreq, aodv_data);
				err_code = process_rreq(&rreq, peer_address);
				break;
			
			case(BLE_AODV_RREP):
				memset(&rrep, 0, sizeof(rrep));
				create_rrep_from_advertising(&rrep, aodv_data);
				err_code = process_rrep(&rrep, peer_address);
				break;
			
			case(BLE_AODV_STATS):
				memset(&stats, 0, sizeof(stats));
				create_stats_from_advertising(&stats, aodv_data);
				err_code = process_stats(&stats, peer_address);
				break;
			
			default:
				err_code = NRF_ERROR_INVALID_DATA;
				break;
		}
		return err_code;
}

void initialse_roll_call_rreqs(){
	rreq_packet rreq;
	
	for(int i = 0; i < NUM_DEVICES; i++){
		memset(&rreq, 0, sizeof(rreq_packet));
		generate_rreq_packet(&rreq, &m_broadcast_id, m_devices[i], m_ble_address, m_sequence_num);
		m_broadcast_id++;
		QueuePut(&rreq, NULL, NULL, m_queue, &m_queueIn, &m_queueOut);
	}
}

void initialse_route_to_zero_rreqs(){
	rreq_packet rreq;
	memset(&rreq, 0, sizeof(rreq_packet));
	generate_rreq_packet(&rreq, &m_broadcast_id, m_ble_address, m_ble_address, m_sequence_num);
	m_broadcast_id++;
	QueuePut(&rreq, NULL, NULL, m_queue, &m_queueIn, &m_queueOut);
}

void print_node_discovery_times(){
	SEGGER_RTT_printf(0, "%s[AODV_MODULE] Printing node discovery times\n", RTT_CTRL_BG_GREEN, RTT_CTRL_RESET);
  char my_string[5];
	SEGGER_RTT_WriteString(0, my_string);
	for(int i = 0; i < NUM_DEVICES; i++){
			memset(&my_string,0, 5);
			sprintf (my_string, "node %d: %f\r\n", i, m_time_of_node_discovery[i]);
			SEGGER_RTT_WriteString(0, my_string);
	}
}

void stats_timer_handler(void * p_context){
	uint32_t next_hop = 0;
	//find the next hop to the root node
	int indx = routing_table_lookup(m_routing_table, ROOT_BLE_ADDRESS);
	if(indx != ENTRY_NOT_PRESENT){
		next_hop = m_routing_table[indx].nextHop;
	}
	
	//create the packet using global data and the next hop
	stats_packet packet;
	generate_stats_packet(&packet, m_rreqs_sent, m_rreps_sent, m_rreqs_processed, m_rreps_processed, next_hop, m_ble_address);
	SEGGER_RTT_printf(0, "%s[AODV_MODULE] Adding stats to buffer\n", RTT_CTRL_BG_BRIGHT_MAGENTA, RTT_CTRL_RESET);
	QueuePut(NULL, NULL, &packet, m_queue, &m_queueIn, &m_queueOut);
}


void buffer_timer_handler(void * p_context){
	sd_ble_gap_adv_stop();
	
	queue_element next_packet;
	uint32_t err_code = QueueGet(&next_packet, m_queue, &m_queueIn, &m_queueOut);
	
	if(err_code == 0){
		switch(next_packet.type){
			case BLE_AODV_RREQ:
				SEGGER_RTT_printf(0, "%s[AODV_MODULE] RREQ in buffer\n", RTT_CTRL_BG_BRIGHT_MAGENTA, RTT_CTRL_RESET);
				send_rreq(&next_packet.rreq);
				break;
			
			case BLE_AODV_RREP:
				SEGGER_RTT_printf(0, "%s[AODV_MODULE] RREP in buffer\n", RTT_CTRL_BG_BRIGHT_MAGENTA, RTT_CTRL_RESET);
				send_rrep(&next_packet.rrep);
				break;
			
			case BLE_AODV_STATS:
				SEGGER_RTT_printf(0, "%s[AODV_MODULE] STATS in buffer\n", RTT_CTRL_BG_BRIGHT_MAGENTA, RTT_CTRL_RESET);
				send_stats(&next_packet.stats);
				break;
					
			default:
				SEGGER_RTT_printf(0, "%s[AODV_MODULE] invalid packet type in buffer\n", RTT_CTRL_BG_BRIGHT_MAGENTA, RTT_CTRL_RESET);
		}
	}
	else{
		//SEGGER_RTT_printf(0, "%s[AODV_MODULE] Nothing to send\n", RTT_CTRL_BG_BRIGHT_MAGENTA, RTT_CTRL_RESET);
	}
	
}

