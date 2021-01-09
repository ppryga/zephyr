/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Performs steps related with DF antennae configuration. */
int radio_df_ant_configure(void);
/* Provides number of available antennae for Direction Finding. */
uint8_t radio_df_ant_num_get(void);

/* Sets Direction Finding mode. */
void radio_df_mode_set(uint8_t mode);
/* Sets inline configuration enabled or disabled for receive of CTE. */
void radio_df_cte_inline_set(uint8_t eanble);
/* Sets length of the CTE for transmission. */
void radio_df_cte_length_set(uint8_t value);
/* Clears antenna switch pattern. */
void radio_df_ant_switch_pattern_clear(void);
/* Set antenna switch spacing to provided value */
void radio_df_ant_switch_spacing_set(uint8_t spacing);
/* Set antenna switch pattern. Pay attention, paterns are added to
 * Radio internal list. Before start of new patterns clear the list
 * by call to @ref radio_df_ant_switch_pattern_clear.
 */
void radio_df_ant_switch_pattern_set(uint8_t pattern);
/* Resets Direction Finding radio configuration */
void radio_df_reset(void);
