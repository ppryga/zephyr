/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* @brief Max supported CTE length in 8us units */
#define LLL_DF_MAX_CTE_LEN 20
/* @brief Min supported CTE length in 8us units */
#define LLL_DF_MIN_CTE_LEN 2

/* @brief Max supported length of antenna switching pattern */
#define LLL_DF_MAX_ANT_PATTERN_LEN 40
/* @brief Min supported length of antenna switching pattern */
#define LLL_DF_MIN_ANT_PATTERN_LEN 3

/* @brief Function perofrms Direction Finding initialization
 *
 * @return	Zero in case of success, other value in case of failure.
 */
int lll_df_init(void);

/* @brief Function perofrms Direction Finding reset
 *
 * @return	Zero in case of success, other value in case of failure.
 */
int lll_df_reset(void);

/* @brief Function provides number of available antennae.
 *
 * The number of antenna is design dependent and is provided via device tree.
 *
 * @return	Number of available antennae.
 */
uint8_t lll_df_ant_num_get(void);
