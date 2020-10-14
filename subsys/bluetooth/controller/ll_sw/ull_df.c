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
