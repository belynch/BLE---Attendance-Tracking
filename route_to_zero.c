#include "ble_custom_advertising.h"
#include "ble_custom_scanning.h"
#include "aodv_module/aodv_module.h"
#include "route_to_zero.h"
#include "SEGGER_RTT.h"

uint32_t root_start_route_to_zero(){
	uint32_t err_code = NRF_SUCCESS;
	SEGGER_RTT_printf(0, "%s[APPL] Starting root route-to-zero\n", RTT_CTRL_BG_MAGENTA, RTT_CTRL_RESET);
	uint32_t this_address = get_ble_address();
	
	err_code = ble_scanning_start();
	APP_ERROR_CHECK(err_code);
	SEGGER_RTT_printf(0, "%s[APPL] Scanning in process\n", RTT_CTRL_BG_BRIGHT_MAGENTA, RTT_CTRL_RESET);

	initialse_route_to_zero_rreqs();
	
	return err_code;
}

uint32_t node_start_route_to_zero(){
	uint32_t err_code = NRF_SUCCESS;
	
	SEGGER_RTT_printf(0, "%s[APPL] Starting node route-to-zero\n", RTT_CTRL_BG_MAGENTA, RTT_CTRL_RESET);

	err_code = ble_scanning_start();
	APP_ERROR_CHECK(err_code);
	SEGGER_RTT_printf(0, "%s[APPL] Scanning in process\n", RTT_CTRL_BG_BRIGHT_MAGENTA, RTT_CTRL_RESET);
	
	return err_code;
}


