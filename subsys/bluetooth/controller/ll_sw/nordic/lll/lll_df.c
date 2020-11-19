/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include "hal/radio_df.h"
#include "lll_df.h"

uint8_t lll_df_ant_num_get(void)
{
	return radio_df_ant_num_get();
}
