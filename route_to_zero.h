/**
 * This file contains functions to start the Route-to-Zero protocol. The Route-to-Zero
 * protocol creates and sends a single RREQ packet.
 *
 * @author  Ben Lynch
 */


#ifndef __ROUTE_TO_ZERO_H__
#define __ROUTE_TO_ZERO_H__

#include <stdint.h>

/**
 * Creates a single RREQ packet and adds it to the packet buffer.
 *
 * @return	 1 if successful
 */
uint32_t root_start_route_to_zero(void);

/**
 * Begins BLE scanning
 *
 * @return	 1 if successful
 */
uint32_t node_start_route_to_zero(void);

#endif
