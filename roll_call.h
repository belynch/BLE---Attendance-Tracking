/**
 * This file contains functions to start the Roll Call protocol. The roll call protocol
 * creates and sends a RREQ for each device listed on the roll.
 *
 * @author  Ben Lynch
 */

#ifndef __ROLL_CALL_H__
#define __ROLL_CALL_H__

#include <stdint.h>

/**
 * Creates the RREQs for each device on the roll and adds them to the packet buffer.
 *
 * @return	 1 if successful
 */
uint32_t root_start_roll_call(void);

/**
 * Begins BLE scanning
 *
 * @return	 1 if successful
 */
uint32_t node_start_roll_call(void);

#endif
