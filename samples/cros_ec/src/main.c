/*
 * Copyright 2019 Google LLC
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <sys/printk.h>

void main(void)
{
	printk("Chrome OS EC %s\n", CONFIG_BOARD);
}
