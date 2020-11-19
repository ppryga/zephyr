/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* @brief Function performs antenna configuration.
 *
 * @return	Zero in case of success, other value in case of failure.
 */
int radio_df_ant_configure(void);

/* @brief Function provides number of available antennae for Direction Finding.
 *
 * The number of antenna is design dependent. It is provided via device tree.
 *
 * @return	Number of available antennae.
 */
uint8_t radio_df_ant_num_get(void);
