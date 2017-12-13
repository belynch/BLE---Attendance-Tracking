#include "ble_custom_advertising.h"
#include "bsp.h"
#include "bsp_btn_ble.h"
#include "SEGGER_RTT.h"

static ble_gap_adv_params_t	m_adv_params;

uint32_t set_custom_adv_data(uint8_t * adv_data){
		uint32_t err_code;

		err_code = sd_ble_gap_adv_data_set(adv_data, BLE_GAP_ADV_MAX_SIZE, NULL, 0);
		if(err_code != NRF_SUCCESS){
			SEGGER_RTT_printf(0, "%sError setting advertisement data: %u\n", RTT_CTRL_BG_BRIGHT_RED, err_code, RTT_CTRL_RESET);
		}
    APP_ERROR_CHECK(err_code);

		set_adv_params();

		return err_code;
}

uint32_t set_manufacturer_adv_data(uint8_t * adv_data){
		uint32_t err_code;

		ble_advdata_t advdata;

		ble_advdata_manuf_data_t adv_manuf_data;
    uint8_array_t            adv_manuf_data_array;

		adv_manuf_data_array.p_data = adv_data;
    adv_manuf_data_array.size = 24;

		adv_manuf_data.company_identifier = 0xFF00;
		adv_manuf_data.data = adv_manuf_data_array;

	  memset(&advdata, 0, sizeof(advdata));
		advdata.flags = 0x04;
		advdata.p_manuf_specific_data = &adv_manuf_data;

		err_code = ble_advdata_set(&advdata, NULL);
		if(err_code != NRF_SUCCESS){
			SEGGER_RTT_printf(0, "%sError setting advertisement data: %u\n", RTT_CTRL_BG_BRIGHT_RED, err_code, RTT_CTRL_RESET);
		}
    APP_ERROR_CHECK(err_code);

		set_adv_params();

		return err_code;
}

void set_adv_params(){
	  memset(&m_adv_params, 0, sizeof(m_adv_params));

		m_adv_params.type					= BLE_GAP_ADV_TYPE_ADV_NONCONN_IND;
	  m_adv_params.p_peer_addr 	= NULL;                           	// Undirected advertisement
		m_adv_params.fp						= BLE_GAP_ADV_FP_ANY;
    m_adv_params.interval			= APP_ADV_INTERVAL;
    m_adv_params.timeout			= APP_ADV_TIMEOUT_IN_SECONDS;
}

uint32_t custom_advertising_start(){
		uint32_t err_code;
    err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING);
		APP_ERROR_CHECK(err_code);

    return sd_ble_gap_adv_start(&m_adv_params);
}
