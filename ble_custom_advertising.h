/**
 * This file contains wrapper functions for the nrF5 ble advertising stack.
 *
 * @author  Ben Lynch
 */

#ifndef __BLE_CUSTOM_ADVERTISING_H__
#define __BLE_CUSTOM_ADVERTISING_H__

#include <stdint.h>
#include <stdbool.h>
#include <string.h> /* memset */
#include "ble.h"
#include "nrf_error.h"
#include "ble_advertising.h"

#include "SEGGER_RTT.h"

#define NULL 														0
#define APP_ADV_INTERVAL								0x00A0                                     /**< The advertising interval (in units of 0.625 ms. This value corresponds to 100 ms). */
#define APP_ADV_TIMEOUT_IN_SECONDS			0                                        	 /**< The advertising timeout in units of seconds. (If it is set to 0 no timeout will be used) */

typedef void (*ble_advertising_error_handler_t) (uint32_t nrf_error);

/**
 * Uses all 31 bytes of the BLE advertising packet
 *
 * @param	adv_data	 a pointer to an array of bytes containing the advertising data
 * @return 1 if successful
 */
uint32_t set_custom_adv_data(uint8_t * adv_data);

/**
 * Uses the custom 24 bytes of BLE advertising packets of type manufacturer data.
 *
 * @param	adv_data		a pointer to an array of bytes containing the advertising data
 * @return 1 if successful
 */
uint32_t set_manufacturer_adv_data(uint8_t * adv_data);

/**
 * Sets pre-determined BLE advertising paramaters
 */
void set_adv_params(void);

/**
 * Begins BLE advertising. Advertising paramaters must be set.
 *
 * @return 1 if successful
 */
uint32_t custom_advertising_start(void);

#endif //__BLE_CUSTOM_ADVERTISING_H__
