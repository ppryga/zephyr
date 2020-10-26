/*
 * Copyright (c) 2018-2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "hal/debug.h"
#include "ull_df.h"

#define BT_DBG_ENABLED IS_ENABLED(CONFIG_BT_DEBUG_HCI_DRIVER)
#define LOG_MODULE_NAME bt_ctlr_ull_df
#include "common/log.h"

uint8_t ll_df_set_conn_cte_tx_params(uint16_t handle, uint8_t cte_types,
				     uint8_t switching_patterns_len,
				     uint8_t *ant_id)
{
	if (cte_types & BT_HCI_LE_AOD_CTE_RESP_1US ||
	    cte_types & BT_HCI_LE_AOD_CTE_RESP_2US) {

		if (!IS_ENABLED(CONFIG_BT_CTLR_DF_ANT_SWITCHING_TX)) {
			return BT_HCI_ERR_UNSUPP_FEATURE_PARAM_VAL;
		}

		if (switching_patterns_len < BT_HCI_LE_SWITCH_PATTERN_LEN_MIN ||
		    switching_patterns_len > BT_HCI_LE_SWITCH_PATTERN_LEN_MAX ||
		    ant_id == NULL) {
			return BT_HCI_ERR_UNSUPP_FEATURE_PARAM_VAL;
		}
	}

	return BT_HCI_ERR_UNKNOWN_CMD;
}

void ll_df_read_ant_inf(uint8_t *switch_sample_rates,
			uint8_t *num_ant,
			uint8_t *max_switch_pattern_len,
			uint8_t *max_cte_len)
{
	LL_ASSERT(switch_sample_rates);
	LL_ASSERT(num_ant);
	LL_ASSERT(max_switch_pattern_len);
	LL_ASSERT(max_cte_len);

	/* Currently filled with data that inform about
	 * lack of antenna support for Direction Finding
	 */
	*switch_sample_rates = 0;
	*num_ant = 0;
	*max_switch_pattern_len = 0;
	*max_cte_len = 0;
}
