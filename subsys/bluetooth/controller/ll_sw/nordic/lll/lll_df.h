/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* @brief Max supported CTE length in 8us units */
#define LLL_DF_MAX_CTE_LEN 20
/* @brief Min supported CTE length in 8us units */
#define LLL_DF_MIN_CTE_LEN 2

/* @brief Min supported length of antenna switching pattern */
#define LLL_DF_MIN_ANT_PATTERN_LEN 3

/* @brief Function provides number of available antennae.
 *
 * The number of antenna is design dependent and is provided via device tree.
 *
 * @return      Number of available antennae.
 */
uint8_t lll_df_ant_num_get(void);
