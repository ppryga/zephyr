/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include "hal/radio_df.h"
#include <bluetooth/hci.h>
#include "lll_df.h"

/* @brief Function performs common steps for initialization and reset
 * of Direction Finding LLL module.
 *
 * @return	Zero in case of success, other value in case of failure.
 */
static int init_reset(void);

/* @brief Function performs Direction Finding initialization
 *
 * @return	Zero in case of success, other value in case of failure.
 */
int lll_df_init(void)
{
	int err;

	err = radio_df_ant_configure();
	if (err) {
		return err;
	}

	err = init_reset();
	if (err) {
		return err;
	}

	return 0;
}


/* @brief Function performs Direction Finding reset
 *
 * @return	Zero in case of success, other value in case of failure.
 */
int lll_df_reset(void)
{
	int err;

	err = init_reset();
	if (err) {
		return err;
	}

	return 0;
}

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

/* @brief Function initializes transmission of Constant Tone Extension.
 *
 * @param[in] type      Type of CTE: AoA, AoD 1us, AoD 2us
 * @param[in] length	Duration of CTE in 8us units
 * @param[in] ant_num	Number of antennas in switch pattern
 * @param[in] ant_ids	Antenna identifiers that create switch pattern.
 *
 * @Note Pay attention that first two antenna identifiers in a switch
 * pattern has special purpose. First one is used in guard period, second
 * in reference period. Acutal switching is processed from third antenna.
 *
 * In case of AoA amode nt_num and ant_ids parameters are not used.
 */
void lll_df_conf_cte_tx_enable(uint8_t type, uint8_t length,
			       uint8_t ant_num, uint8_t *ant_ids)
{
	if (type == BT_HCI_LE_AOA_CTE) {
		radio_df_mode_set(RADIO_DFEMODE_DFEOPMODE_AoA);
	} else {
		radio_df_mode_set(RADIO_DFEMODE_DFEOPMODE_AoD);

		if (type == BT_HCI_LE_AOD_CTE_1US) {
			radio_df_ant_switch_spacing_set(RADIO_DFECTRL1_TSWITCHSPACING_2us);
		} else {
			radio_df_ant_switch_spacing_set(RADIO_DFECTRL1_TSWITCHSPACING_4us);
		}

		radio_df_ant_switch_pattern_clear();
		/* First antenna is used for guard period, second for reference period
		* third and following are used for rest of the CTE.
		*/
		for (uint8_t idx = 0; idx < ant_num; ++idx) {
			radio_df_ant_switch_spacing_set(ant_ids[idx]);
		}
	}

	radio_df_cte_length_set(length);
}

void lll_df_conf_cte_tx_disable(void)
{
	radio_df_reset();
}

static int init_reset(void)
{
	return 0;
}
