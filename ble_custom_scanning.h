/**
 * This file contains wrapper functions for the nrF5 ble scanning stack.
 *
 * @author  Ben Lynch
 */

#ifndef __BLE_CUSTOM_SCANNING_H__
#define __BLE_CUSTOM_SCANNING_H__

#include <stdint.h>
#include "ble.h"

#define SCAN_INTERVAL           0x0030                          /**< Determines scan interval in units of 0.625 millisecond. 0x50 = 0.05s */
#define SCAN_WINDOW             0x0020                          /**< Determines scan window in units of 0.625 millisecond. 0x8 = 0.01s */
#define SCAN_ACTIVE             0                               /**< If 1, performe active scanning (scan requests). */
#define SCAN_SELECTIVE          0                               /**< If 1, ignore unknown devices (non whitelisted). */
#define SCAN_TIMEOUT            0x0000                          /**< Timout when scanning. 0x0000 disables timeout. */

/**
 * Begins BLE scanning.
 *
 * @return 1 if successfull
 */
uint32_t ble_scanning_start(void);

#endif //__BLE_CUSTOM_SCANNING_H__
