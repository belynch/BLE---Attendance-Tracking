#include "bsp.h"
#include "bsp_btn_ble.h"
#include "ble_custom_scanning.h"
#include "SEGGER_RTT.h"

/**
 * @brief Parameters used when scanning.
 */
static const ble_gap_scan_params_t m_scan_params = 
  {
    .active      = SCAN_ACTIVE,
    .selective   = SCAN_SELECTIVE,
    .p_whitelist = NULL,
    .interval    = SCAN_INTERVAL,
    .window      = SCAN_WINDOW,
    .timeout     = SCAN_TIMEOUT
  };

	
/**
 * @brief Function to start scanning.
 */
uint32_t ble_scanning_start(void)
{
		uint32_t err_code;   
    err_code = bsp_indication_set(BSP_INDICATE_SCANNING);
    APP_ERROR_CHECK(err_code);
	
	  return sd_ble_gap_scan_start(&m_scan_params);
}
