/* cros_ec.c - Chrome OS EC top-level file */

/*
 * Copyright 2019 Google LLC
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdarg.h>

#include <sys/printk.h>

#include <cros_ec/common.h>

timestamp_t get_time(void)
{
	timestamp_t t;

	t.val = k_uptime_get();

	return t;
}

uint32_t __hw_clock_event_get(void)
{
	return 0;
}

int cprintf(enum console_channel channel, const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	vprintk(format, ap);
	va_end(ap);

	return 0;
}

int cprints(enum console_channel channel, const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	vprintk(format, ap);
	va_end(ap);

	return 0;
}

void cflush(void)
{
}
