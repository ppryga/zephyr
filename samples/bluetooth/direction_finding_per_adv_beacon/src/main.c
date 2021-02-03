/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <errno.h>
#include <zephyr.h>
#include <sys/printk.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/direction.h>
#include <sys/byteorder.h>
#include <sys/util.h>

/* Length of CTE in unit of 8[us] */
#define CTE_LEN (0x14U)

static void adv_sent_cb(struct bt_le_ext_adv *adv,
			struct bt_le_ext_adv_sent_info *info);

static struct bt_le_ext_adv_cb adv_callbacks = {
	.sent = adv_sent_cb,
};

static struct bt_le_ext_adv *adv_set;

static struct bt_le_adv_param param =
		BT_LE_ADV_PARAM_INIT(BT_LE_ADV_OPT_EXT_ADV |
				     BT_LE_ADV_OPT_USE_NAME,
				     BT_GAP_ADV_FAST_INT_MIN_2,
				     BT_GAP_ADV_FAST_INT_MAX_2,
				     NULL);

static struct bt_le_ext_adv_start_param ext_adv_start_param = {
	.timeout = 0,
	.num_events = 0,
};

static struct bt_le_per_adv_param per_adv_param = {
	.interval_min = BT_GAP_ADV_SLOW_INT_MIN,
	.interval_max = BT_GAP_ADV_SLOW_INT_MAX,
	.options = BT_LE_ADV_OPT_USE_TX_POWER,
};

#if IS_ENABLED(CONFIG_BT_CTLR_DF_ANT_SWITCH_TX)
static uint8_t ant_patterns[] = {0x2, 0x0, 0x5, 0x6, 0x1, 0x4, 0xC, 0x9, 0xE,
				 0xD, 0x8, 0xA};
#endif /* CONFIG_BT_CTLR_DF_ANT_SWITCH_TX */

struct bt_df_adv_cte_tx_param cte_params = {
	.cte_len = CTE_LEN,
	.cte_type = BT_HCI_LE_AOD_CTE_2US,
	.cte_count = 1,
#if IS_ENABLED(CONFIG_BT_CTLR_DF_ANT_SWITCH_TX)
	.num_ant_ids = ARRAY_SIZE(ant_patterns),
	.ant_ids = ant_patterns
#endif /* CONFIG_BT_CTLR_DF_ANT_SWITCH_TX */
	};

static void adv_sent_cb(struct bt_le_ext_adv *adv,
			struct bt_le_ext_adv_sent_info *info)
{
	printk("Advertiser[%d] %p sent %d\n", bt_le_ext_adv_get_index(adv),
	       adv, info->num_sent);
}

static struct bt_le_ext_adv *advertising_set_create(void)
{
	struct bt_le_ext_adv *adv;
	int err;

	printk("Advertising set create...");
	err = bt_le_ext_adv_create(&param, &adv_callbacks, &adv);
	if (err) {
		printk("failed (err %d)\n", err);
		return NULL;
	}
	printk("success\n");
	return adv;
}

static int periodic_advertising_params_set(struct bt_le_ext_adv *adv)
{
	int err;

	printk("Periodic advertising params set...");
	err = bt_le_per_adv_set_param(adv, &per_adv_param);
	if (err) {
		printk("failed (err %d)\n", err);
		return err;
	}
	printk("success\n");

	return 0;
}

static int periodic_advertising_enable(struct bt_le_ext_adv *adv)
{
	int err;

	printk("Periodic advertising enable...");
	err = bt_le_per_adv_start(adv);
	if (err) {
		printk("failed (err %d)\n", err);
		return err;
	}
	printk("success\n");

	printk("Extended advertising enable...");
	err = bt_le_ext_adv_start(adv, &ext_adv_start_param);
	if (err) {
		printk("failed (err %d)\n", err);
		return err;
	}
	printk("success\n");

	char addr_s[BT_ADDR_LE_STR_LEN];
	bt_addr_le_t addr = {0};
	size_t count = 1;

	bt_id_get(&addr, &count);
	bt_addr_le_to_str(&addr, addr_s, sizeof(addr_s));

	printk("Started extended advertising as %s\n", addr_s);

	return 0;
}

static int cte_params_set(struct bt_le_ext_adv *adv,
			  struct bt_df_adv_cte_tx_param *params)
{
	int err;

	printk("Update CTE params...");
	err = bt_df_set_adv_cte_tx_param(adv, params);
	if (err) {
		printk("failed (err %d)\n", err);
		return err;
	}
	printk("success\n");

	return 0;
}

static int cte_enable(struct bt_le_ext_adv *adv)
{
	int err;

	printk("Enable CTE...");
	err = bt_df_adv_cte_tx_enable(adv);
	if (err) {
		printk("failed (err %d)\n", err);
		return err;
	}
	printk("success\n");

	return 0;
}

static void bt_ready(int err)
{
	printk("Bluetooth initialization...\n");
	if (err) {
		printk("failed (err %d)\n", err);
		return;
	}
	printk("success\n");

	adv_set = advertising_set_create();
	if (!adv_set) {
		return;
	}

	err = cte_params_set(adv_set, &cte_params);
	if (err) {
		return;
	}

	err = periodic_advertising_params_set(adv_set);
	if (err) {
		return;
	}

	err = cte_enable(adv_set);
	if (err) {
		return;
	}

	err = periodic_advertising_enable(adv_set);
	if (err) {
		return;
	}
}

void main(void)
{
	printk("Starting Direction Finding Connectionelss Beacon Demo\n");

	/* Initialize the Bluetooth Subsystem */
	bt_enable(bt_ready);
}
