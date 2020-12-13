/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file Common soc.h include for Nordic nRF5 SoCs.
 */

#ifndef _ZEPHYR_SOC_ARM_NORDIC_NRF_SOC_NRF_COMMON_H_
#define _ZEPHYR_SOC_ARM_NORDIC_NRF_SOC_NRF_COMMON_H_

#ifndef _ASMLANGUAGE
#include <nrfx.h>
#include <devicetree.h>
#include <toolchain.h>

/**
 * @brief Get a PSEL value out of a foo-gpios or foo-pin property
 *
 * Many Nordic drivers use 'foo-pin' properties to specify a pin
 * configuration as a PSEL value directly instead of using a 'foo-gpios'
 * <&gpioX Y flags> style controller phandle + GPIO specifier.
 *
 * To fit with devicetree convention better and avoid forcing users to
 * mentally decode the 'raw' PSEL register values, it would be better
 * to use 'foo-gpios' properties instead. This type of property is
 * also easier to parse into a "pin mux report" without special cases
 * for nRF SoCs, since the phandles are available.
 *
 * To allow for a smoother deprecation period to 'foo-gpios'
 * everywhere, this helper macro can be used to get a PSEL value out
 * of the devicetree using whichever one of 'foo-gpios' or 'foo-pin'
 * is in the DTS.
 *
 * Note that you can also use:
 *
 *   - NRF_DT_CHECK_PSEL() to check the property configuration at build time
 *   - NRF_DT_GPIOS_TO_PSEL() if you only have a 'foo-gpios'
 *
 * @param node_id node identifier
 * @param psel_prop lowercase-and-underscores old-style 'foo-pin' property
 * @param gpios_prop new-style 'foo-gpios' property
 * @return PSEL register value taken from psel_prop or gpios_prop, whichever
 *         is present in the DTS. If gpios_prop is present, it is converted
 *         to a PSEL register value first.
 */
#define NRF_DT_TO_PSEL(node_id, psel_prop, gpios_prop)			\
	COND_CODE_1(DT_NODE_HAS_PROP(node_id, psel_prop),		\
		    (DT_PROP(node_id, psel_prop)),			\
		    (COND_CODE_1(					\
			    DT_NODE_HAS_PROP(node_id, gpios_prop),	\
			    (NRF_DT_GPIOS_TO_PSEL(node_id,		\
						  gpios_prop)),		\
			    (~0U))))	/* invalid configuration */

/**
 * @brief Convert a devicetree GPIO phandle+specifier to PSEL value
 *
 * Various peripherals in nRF SoCs have pin select registers, which
 * usually have PSEL in their names. The low bits of these registers
 * generally look like this in the register map description:
 *
 *     Bit number     5 4 3 2 1 0
 *     ID             B A A A A A
 *
 *     ID  Field  Value    Description
 *     A   PIN    [0..31]  Pin number
 *     B   PORT   [0..1]   Port number
 *
 * Examples:
 *
 * - pin P0.4 has "PSEL value" 4 (B=0 and A=4)
 * - pin P1.5 has "PSEL value" 37 (B=1 and A=5)
 *
 * This macro converts a devicetree GPIO phandle array value
 * "<&<gpioX> <pin> ...>" to a "PSEL value".
 *
 * Note: in Nordic SoC devicetrees, "gpio0" means P0, and "gpio1"
 * means P1.
 *
 * Examples:
 *
 *     foo: my-node {
 *             tx-gpios = <&gpio0 4 ...>;
 *             rx-gpios = <&gpio1 5 ...>;
 *     };
 *
 *     NRF_DT_GPIOS_TO_PSEL(DT_NODELABEL(foo), tx_gpios) // 0 + 4 = 4
 *     NRF_DT_GPIOS_TO_PSEL(DT_NODELABEL(foo), rx_gpios) // 32 + 5 = 37
 */
#define NRF_DT_GPIOS_TO_PSEL(node_id, prop)				\
	(DT_GPIO_PIN(node_id, prop) +					\
	 NRF_PSEL_OFFSET_HELPER(node_id, prop))

/**
 * Error out the build if the devicetree node with identifier
 * 'node_id' has both a legacy psel-style property and a gpios
 * property.
 *
 * Otherwise, do nothing.
 *
 * @param node_id node identifier
 * @param psel_prop lowercase-and-underscores PSEL style property
 * @param psel_prop_name human-readable string name of psel_prop
 * @param gpios_prop lowercase-and-underscores foo-gpios style property
 * @param gpio_prop_name human-readable string name of gpios_prop
 */
#define NRF_DT_CHECK_PSEL(node_id, psel_prop, psel_prop_name,		\
			  gpios_prop, gpios_prop_name)			\
	BUILD_ASSERT(							\
		(DT_NODE_HAS_PROP(node_id, psel_prop) ^			\
		 DT_NODE_HAS_PROP(node_id, gpios_prop)),		\
		"Devicetree node " DT_NODE_PATH(node_id)		\
		" has neither or both of the " psel_prop_name		\
		" and " gpios_prop_name					\
		" properties set; you must set exactly one. "		\
		"Note: use /delete-property/ to delete properties.")

/**
 * Error out the build if 'prop' is set on node 'node_id' and
 * DT_GPIO_CTLR(node_id, prop) is not an SoC GPIO controller.
 *
 * Otherwise, do nothing.
 *
 * @param node_id node identifier
 * @param psel_prop lowercase-and-underscores PSEL style property
 * @param psel_prop_name human-readable string name of psel_prop
 * @param gpios_prop lowercase-and-underscores foo-gpios style property
 * @param gpio_prop_name human-readable string name of gpios_prop
 */
#define NRF_DT_CHECK_GPIO_CTLR_IS_SOC(node_id, prop, prop_name)		\
	BUILD_ASSERT(!DT_NODE_HAS_PROP(node_id, prop) ||		\
		     NRF_DT_GPIO_CTLR_IS_SOC(node_id, prop),		\
		     "Devicetree node " DT_NODE_PATH(node_id)		\
		     " property " prop_name				\
		     " GPIO controller must be &gpio0 "			\
		     "or &gpio1 (if available on the SoC), but got "	\
		     DT_NODE_PATH(DT_GPIO_CTLR(node_id, prop))		\
		     ", which is neither of these")

/*
 * Returns true if DT_GPIO_CTLR(node_id, prop) is an SoC GPIO
 * controller (i.e. same node as DT_NODELABEL(gpio0) or
 * DT_NODELABEL(gpio1)). Otherwise, returns false.
 */
#define NRF_DT_GPIO_CTLR_IS_SOC(node_id, prop)				\
	(NRF_DT_GPIO_CTLR_IS_GPIOX(node_id, prop, 0) ||			\
	 UTIL_AND(DT_NODE_EXISTS(DT_NODELABEL(gpio1)),			\
		  NRF_DT_GPIO_CTLR_IS_GPIOX(node_id, prop, 1)))

/* Is DT_GPIO_CTLR(node_id, prop) the same node as DT_NODELABEL(gpio<idx>)? */
#define NRF_DT_GPIO_CTLR_IS_GPIOX(node_id, prop, idx)			\
	DT_SAME_NODE(DT_GPIO_CTLR(node_id, prop),			\
		     DT_NODELABEL(gpio ## idx))

/*
 * If DT_GPIO_CTLR(node_id, prop) is gpio0, return 0.
 * Otherwise, assume it's gpio1 and return 32.
 * (The assumption is checked using NRF_IS_SOC_GPIO_OR_COMPILE_ERROR.)
 */
#define NRF_PSEL_OFFSET_HELPER(node_id, prop)		\
	(NRF_DT_GPIO_CTLR_IS_GPIOX(node_id, prop, 0) ? 0 : 32)

#endif /* !_ASMLANGUAGE */

#endif
