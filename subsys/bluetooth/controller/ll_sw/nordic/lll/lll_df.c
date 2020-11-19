/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include "hal/radio_df.h"
#include "lll_df.h"

/* @brief Function provides number of available antennae.
 *
 * The number of antenna is hardware defined and it is provided via device tree.
 *
 * @return      Number of available antennae.
 */
uint8_t lll_df_ant_num_get(void)
{
	return radio_df_ant_num_get();
}
