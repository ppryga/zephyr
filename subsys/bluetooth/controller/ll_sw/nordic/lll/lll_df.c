/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include "hal/radio_df.h"
#include "lll_df.h"

/* @brief Function performs common steps for initialization and reset
 * of Direction Finding LLL module.
 *
 * @return	Zero in case of success, other value in case of failure.
 */
static int init_reset(void);

int lll_df_init(void)
{
	int err;

	err = init_reset();
	if (err) {
		return err;
	}

	return 0;
}

int lll_df_reset(void)
{
	int err;

	err = init_reset();
	if (err) {
		return err;
	}

	return 0;
}

static int init_reset(void)
{
	int err;

	err = radio_df_ant_configure();
	if (err) {
		return err;
	}

	return 0;
}

uint8_t lll_df_ant_num_get(void)
{
	return radio_df_ant_num_get();
}
