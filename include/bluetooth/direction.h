/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_BLUETOOTH_DF_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_DF_H_

/** @brief Constant Tone Extension parameters for connectionless
 * transmission.
 *
 * The structure holds information required to setup CTE transmission
 * in periodic advertising.
 */
struct bt_le_df_adv_cte_tx_params {
	/* Length of CTE in 8us units */
	uint8_t  cte_len;
	/* CTE Type: AoA, AoD 1us slots, AoD 2us slots */
	uint8_t  cte_type;
	/* Number of CTE to transmit in each periodic adv interval */
	uint8_t  cte_count;
	/* Number of Antenna IDs in the switch pattern */
	uint8_t  num_ant_ids;
	/* List of antenna IDs in the pattern */
	uint8_t  *ant_ids;
};

/**
 * @brief Set or update the Constant Tone Extension parameters for periodic
 * advertising set.
 *
 * The Constant Tone Extension parameters can only be set or updated on an
 * extended advertisment set which has already set periodic adverising
 * parameters. Update of Constant Tone Extension parameters may not be attempted
 * while Constnat Tone Extension is enabled on particular extended advertisement
 * set.
 *
 * @param[in] adv           Advertising set object.
 * @param[in] params         Constant Tone Extension parameters.
 *
 * @return Zero on success or (negative) error code otherwise.
 */
int bt_le_df_set_adv_cte_tx_params(struct bt_le_ext_adv *adv,
			       const struct bt_le_df_adv_cte_tx_params *params);

int bt_le_df_adv_cte_tx_enable(struct bt_le_ext_adv *adv);
int bt_le_df_adv_cte_tx_disable(struct bt_le_ext_adv *adv);

#endif /* ZEPHYR_INCLUDE_BLUETOOTH_DF_H_ */
