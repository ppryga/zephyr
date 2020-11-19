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

/* @brief Max supported CTE length in 8us units */
#define RADIO_DF_MAX_CTE_LEN 20
/* @brief Min supported CTE length in 8us units */
#define RADIO_DF_MIN_CTE_LEN 2

/* @brief Max supported length of antenna switching pattern */
#define RADIO_DF_MAX_ANT_PATTERN_LEN 40
/* @brief Min supported length of antenna switching pattern */
#define RADIO_DF_MIN_ANT_PATTERN_LEN 3

/* @brief Minimum antennae number required if antennae switching is enabled */
#define DF_ANT_NUM_MIN 2
/* @brief Value to set antenna GPIO pin as not connected */
#define DF_GPIO_PIN_NOT_SET 0xFF
/* @brief Number of PSEL_DFEGPIO registers to in Radio peripheral */
#define DF_PSEL_GPIO_NUM 8

/* @brief Direction Finding antenna matrix configuration */
struct df_ant_cfg {
	uint8_t ant_num;
	/* Selection of GPIOs to be used for atenna switching by Radio */
	uint8_t dfe_gpio[DF_PSEL_GPIO_NUM];
};

#define DFE_ANT DT_NODELABEL(dfe_ant)
#define DFE_GPIO_PIN(idx)						\
	UTIL_OR(UTIL_AND(DT_NODE_HAS_PROP(DFE_ANT, dfegpio##idx##_pin),	\
				DT_PROP(DFE_ANT, dfegpio##idx##_pin)),	\
		DF_GPIO_PIN_NOT_SET)

#define DFE_GPIO_PIN_DISCONNECT (RADIO_PSEL_DFEGPIO_CONNECT_Disconnected << \
				 RADIO_PSEL_DFEGPIO_CONNECT_Pos)

#if IS_ENABLED(CONFIG_BT_CTLR_DF_ANT_SWITCHING_TX) || \
	IS_ENABLED(CONFIG_BT_CTRL_DF_ANT_SWITCHING_RX)

#if DT_NODE_HAS_STATUS(DFE_ANT, okay)
const static struct df_ant_cfg ant_cfg = {
	.ant_num = DT_PROP(DFE_ANT, ant_num),
	.dfe_gpio = {
		DFE_GPIO_PIN(0),
		DFE_GPIO_PIN(1),
		DFE_GPIO_PIN(2),
		DFE_GPIO_PIN(3),
		DFE_GPIO_PIN(4),
		DFE_GPIO_PIN(5),
		DFE_GPIO_PIN(6),
		DFE_GPIO_PIN(7),
	}
};
#else
#error "DF antenna switching feature requires dfe_ant to be enabled in DTS"
#endif

#endif /* CONFIG_BT_CTLR_DF_ANT_SWITCHING_TX ||
	* CONFIG_BT_CTRL_DF_ANT_SWITCHING_RX
	*/

int radio_df_ant_configure(void)
{
#if IS_ENABLED(CONFIG_BT_CTLR_DF_ANT_SWITCHING_TX) || \
	IS_ENABLED(CONFIG_BT_CTRL_DF_ANT_SWITCHING_RX)
	uint8_t pins_num = 0;

	if (ant_cfg.ant_num == 0) {
		return -ENOTSUP;
	}

	for (uint8_t idx = 0; idx < DF_PSEL_GPIO_NUM; ++idx) {
		if (ant_cfg.dfe_gpio[idx] != DF_GPIO_PIN_NOT_SET) {
			++pins_num;
		}
	}

	/* Check if there is enough pins cnofigured to represent each pattern
	 * for given antennane number or antennae number is lower than
	 * required minimum.
	 */
	if (((0x1 << pins_num) - 1) < ant_cfg.ant_num ||
	    ant_cfg.ant_num < DF_ANT_NUM_MIN) {
		return -ENOTSUP;
	}

	for (uint8_t idx = 0; idx < DF_PSEL_GPIO_NUM; ++idx) {
		if (ant_cfg.dfe_gpio[idx] != DF_GPIO_PIN_NOT_SET) {
			NRF_RADIO->PSEL.DFEGPIO[idx] = ant_cfg.dfe_gpio[idx];
			++pins_num;
		} else {
			NRF_RADIO->PSEL.DFEGPIO[idx] = DFE_GPIO_PIN_DISCONNECT;
		}
	}
#endif /* CONFIG_BT_CTLR_DF_ANT_SWITCHING_TX ||
	* CONFIG_BT_CTRL_DF_ANT_SWITCHING_RX
	*/
	return 0;
}

uint8_t radio_df_ant_num_get(void)
{
#if IS_ENABLED(CONFIG_BT_CTLR_DF_ANT_SWITCHING_TX) || \
	IS_ENABLED(CONFIG_BT_CTRL_DF_ANT_SWITCHING_RX)
	return ant_cfg.ant_num;
#else
	return 0;
#endif
}
