/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * @addtogroup t_i2c_basic
 * @{
 * @defgroup t_i2c_read_write test_i2c_read_write
 * @brief TestPurpose: verify I2C master can read and write
 * @}
 */

#include <drivers/i2c.h>
#include <zephyr.h>
#include <ztest.h>

#if DT_NODE_HAS_STATUS(DT_ALIAS(i2c_0), okay)
#define I2C_DEV_NAME	DT_LABEL(DT_ALIAS(i2c_0))
#elif DT_NODE_HAS_STATUS(DT_ALIAS(i2c_1), okay)
#define I2C_DEV_NAME	DT_LABEL(DT_ALIAS(i2c_1))
#elif DT_NODE_HAS_STATUS(DT_ALIAS(i2c_2), okay)
#define I2C_DEV_NAME	DT_LABEL(DT_ALIAS(i2c_2))
#else
#error "Please set the correct I2C device"
#endif

uint32_t i2c_cfg = I2C_SPEED_SET(I2C_SPEED_STANDARD) | I2C_MODE_MASTER;
uint16_t addr = 0x70;
uint16_t test_cnt;

const uint8_t reg_data[] = {
	0x16, 0x04, 0x01, 0xc3, 0x01, 0x00, 0x03, 0x01,
	0x12, 0x30, 0x11, 0x20, 0x00, 0x00, 0x00, 0x00,
	0x02, 0x02, 0xff, 0x7f, 0xff, 0xff, 0x01, 0x07,
	0x60, 0x00, 0x4a, 0x00, 0x60, 0x20, 0x08, 0x80,
	0x01, 0x00, 0x00, 0x00, 0xdf, 0x6e, 0xc3, 0x07,
	0x13, 0xd5, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00,
	0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static int test_gy271(void)
{
	unsigned char datas[10];
	const struct device *i2c_dev = device_get_binding(I2C_DEV_NAME);
	int offset = 0;

	if (test_cnt % 3 == 0)
		i2c_cfg = I2C_SPEED_SET(I2C_SPEED_STANDARD) | I2C_MODE_MASTER;
	else if (test_cnt % 3 == 1)
		i2c_cfg = I2C_SPEED_SET(I2C_SPEED_FAST) | I2C_MODE_MASTER;
	else if (test_cnt % 3 == 2)
		i2c_cfg = I2C_SPEED_SET(I2C_SPEED_FAST_PLUS) | I2C_MODE_MASTER;

	TC_PRINT("Test %d, configure is 0x%x\n", test_cnt, i2c_cfg);
	test_cnt++;
	if (!i2c_dev) {
		TC_PRINT("Cannot get I2C device\n");
		return TC_FAIL;
	}

	/* 1. Verify i2c_configure() */
	if (i2c_configure(i2c_dev, i2c_cfg)) {
		TC_PRINT("I2C config failed\n");
		return TC_FAIL;
	}

	/* 2. Verify i2c_write_read() */
	offset = 0x00;
	datas[0] = offset;
	if (i2c_write_read(i2c_dev, addr,
			      &datas[0], 1,
			      &datas[1], 2)) {
		TC_PRINT("Fail to test\n");
		return TC_FAIL;
	}

	TC_PRINT("reg %02x: %02x%02x \n", datas[0], datas[2], datas[1]);
	if (memcmp(reg_data + offset, datas + 1, 2) != 0) {
		TC_PRINT("Wrong data in '2. Verify i2c_write_read()'\n");
		return TC_FAIL;
	}

	offset = 0x02;
	datas[0] = offset;
	if (i2c_write_read(i2c_dev, addr,
			      &datas[0], 1,
			      &datas[1], 2)) {
		TC_PRINT("Fail to test\n");
		return TC_FAIL;
	}

	TC_PRINT("reg %02x: %02x%02x \n", datas[0], datas[2], datas[1]);
	if (memcmp(reg_data + offset, datas + 1, 2) != 0) {
		TC_PRINT("Wrong data in '2. Verify i2c_write_read()'\n");
		return TC_FAIL;
	}


	/* 3. verify i2c_write() */
	datas[0] = 0x12;
	if (i2c_write_read(i2c_dev, addr,
			      &datas[0], 1,
			      &datas[1], 2)) {
		TC_PRINT("Fail to test\n");
		return TC_FAIL;
	}
	TC_PRINT("reg %02x: %02x%02x \n", datas[0], datas[2], datas[1]);

	offset = 0x12;
	datas[0] = offset;
	datas[1] = 0x5A;
	datas[2] = 0xA5;
	if (i2c_write(i2c_dev, datas, 3, addr)) {
		TC_PRINT("Fail to write ALERT_MASK reg\n");
		return TC_FAIL;
	}

	datas[0] = 0x12;
	if (i2c_write_read(i2c_dev, addr,
			      &datas[0], 1,
			      &datas[1], 2)) {
		TC_PRINT("Fail to test\n");
		return TC_FAIL;
	}
	TC_PRINT("reg %02x: %02x%02x \n", datas[0], datas[2], datas[1]);
	if (datas[2] != 0xA5 || datas[1] != 0x5A) {
		TC_PRINT("Wrong ALERT_MASK data\n");
		return TC_FAIL;
	}

	/* Write data back */
	datas[0] = 0x12;
	datas[1] = 0xFF;
	datas[2] = 0x7F;
	if (i2c_write(i2c_dev, datas, 3, addr)) {
		TC_PRINT("Fail to write ALERT_MASK reg\n");
		return TC_FAIL;
	}

	/* 4. verify i2c_read() */
	/* Back to offset 0x00 */
	offset = 0x00;
	datas[0] = offset;
	if (i2c_write(i2c_dev, datas, 1, addr)) {
		TC_PRINT("Fail to write offset reg\n");
		return TC_FAIL;
	}

	if (i2c_read(i2c_dev, datas, 2, addr)) {
		TC_PRINT("Fail to fetch sample from NCT3801\n");
		return TC_FAIL;
	}
	TC_PRINT("0ffset: 0x%02x: %02x %02x\n", offset, datas[0], datas[1]);
	if (memcmp(reg_data + offset, datas, 2) != 0) {
		TC_PRINT("Wrong data in 4. verify i2c_read()\n");
		return TC_FAIL;
	}

	/* 5. verify i2c_read suspend */
	{
		struct i2c_msg msg[2];

		msg[0].buf = &datas[0];
		msg[0].len = 2U;
		msg[0].flags = I2C_MSG_READ;

		msg[1].buf = &datas[2];
		msg[1].len = 2;
		msg[1].flags = I2C_MSG_READ | I2C_MSG_STOP;

		i2c_transfer(i2c_dev, msg, 2, addr);
		TC_PRINT("reg 02: %02x %02x %02x %02x\n", datas[0], datas[1],
							datas[2], datas[3]);

		if (memcmp(reg_data + 2, datas, 4) != 0) {
			TC_PRINT("Wrong data in i2c_read suspend\n");
			return TC_FAIL;
		}
	}

	return TC_PASS;
}

static int test_burst_gy271(void)
{
	unsigned char datas[64];
	const struct device *i2c_dev = device_get_binding(I2C_DEV_NAME);
	int i;
	int offset;

	if (!i2c_dev) {
		TC_PRINT("Cannot get I2C device\n");
		return TC_FAIL;
	}

	/* 1. verify i2c_configure() */
	if (i2c_configure(i2c_dev, i2c_cfg)) {
		TC_PRINT("I2C config failed\n");
		return TC_FAIL;
	}

	/* 6. verify i2c_burst_read() */
	offset = 0x00;
	if (i2c_burst_read(i2c_dev, addr, offset, datas, 64)) {
		TC_PRINT("Fail to fetch sample from sensor GY271\n");
		return TC_FAIL;
	}

	for (i = 0; i< 64; i++) {
		if (i % 8 == 0)
			TC_PRINT("\n ");
		TC_PRINT("0x%02x, ", datas[i]);
	}
	TC_PRINT("\n");

	if (memcmp(reg_data + offset, datas, 64) != 0) {
		TC_PRINT("Wrong data in 6. verify i2c_burst_read()\n");
		return TC_FAIL;
	}

	/* 7. verify i2c_burst_write() */
	/* Write data */
	offset = 0x12; /* alert mask */
	datas[0] = 0x03;
	datas[1] = 0x30;
	if (i2c_burst_write(i2c_dev, addr, offset, datas, 2)) {
		TC_PRINT("Fail to write to sensor GY271\n");
		return TC_FAIL;
	}

	if (i2c_burst_read(i2c_dev, addr, offset, datas+2, 2)) {
		TC_PRINT("Fail to fetch sample from sensor GY271\n");
		return TC_FAIL;
	}

	TC_PRINT("reg %02x: %02x%02x -> %02x%02x\n", offset, datas[0], datas[1],
							datas[2], datas[3]);
	if (datas[0] != datas[2] && datas[1] != datas[3]) {
		TC_PRINT("Wrong data in 7. verify i2c_burst_write()\n");
		return TC_FAIL;
	}

	/* Write data back */
	offset = 0x12; /* alert mask */
	datas[0] = 0xFF;
	datas[1] = 0x7F;
	if (i2c_burst_write(i2c_dev, addr, offset, datas, 2)) {
		TC_PRINT("Fail to write to sensor GY271\n");
		return TC_FAIL;
	}


	k_sleep(K_MSEC(1));

	(void)memset(datas, 0, sizeof(datas));

	return TC_PASS;
}

void test_i2c_gy271(void)
{
	zassert_true(test_gy271() == TC_PASS, NULL);
}

void test_i2c_burst_gy271(void)
{
	zassert_true(test_burst_gy271() == TC_PASS, NULL);
}
