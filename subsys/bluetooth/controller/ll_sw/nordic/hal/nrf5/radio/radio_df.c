/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include <errno.h>
#include <devicetree.h>
#include <sys/util_macro.h>
#include <nrfx/hal/nrf_radio.h>

#include "radio_df.h"

void radio_df_mode_set(uint8_t mode)
{
	NRF_RADIO->DFEMODE &= ~RADIO_DFEMODE_DFEOPMODE_Msk;
	NRF_RADIO->DFEMODE |= ((mode << RADIO_DFEMODE_DFEOPMODE_Pos)
			       & RADIO_DFEMODE_DFEOPMODE_Msk);
}

void radio_df_cte_inline_set(uint8_t enable)
{
	NRF_RADIO->CTEINLINECONF &= ~RADIO_CTEINLINECONF_CTEINLINECTRLEN_Msk;
	NRF_RADIO->CTEINLINECONF |= ((enable <<
				      RADIO_CTEINLINECONF_CTEINLINECTRLEN_Pos)
				     & RADIO_CTEINLINECONF_CTEINLINECTRLEN_Msk);
}

void radio_df_cte_length_set(uint8_t value)
{
	NRF_RADIO->DFECTRL1 &= ~RADIO_DFECTRL1_NUMBEROF8US_Msk;
	NRF_RADIO->DFECTRL1 |= ((value << RADIO_DFECTRL1_NUMBEROF8US_Pos)
				& RADIO_DFECTRL1_NUMBEROF8US_Msk);
}

void radio_df_ant_switch_pattern_clear(void)
{
	NRF_RADIO->CLEARPATTERN = RADIO_CLEARPATTERN_CLEARPATTERN_Clear;
}

void radio_df_ant_switch_spacing_set(uint8_t spacing)
{
	NRF_RADIO->DFECTRL1 &= ~RADIO_DFECTRL1_TSWITCHSPACING_Msk;
	NRF_RADIO->DFECTRL1 |= ((spacing << RADIO_DFECTRL1_TSWITCHSPACING_Pos)
				& RADIO_DFECTRL1_TSWITCHSPACING_Msk);
}

void radio_df_ant_switch_pattern_set(uint8_t pattern)
{
	NRF_RADIO->SWITCHPATTERN = pattern;
}

void radio_df_reset(void)
{
	radio_df_mode_set(RADIO_DFEMODE_DFEOPMODE_Disabled);
	radio_df_cte_inline_set(RADIO_CTEINLINECONF_CTEINLINECTRLEN_Disabled);
	radio_df_ant_switch_pattern_clear();
}
