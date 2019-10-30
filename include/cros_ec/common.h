/* Copyright 2019 Google LLC
 *
 * SPDX-License-Identifier: bsd-3-clause
 */

#ifndef ZEPHYR_INCLUDE_CROS_EC_H
#define ZEPHYR_INCLUDE_CROS_EC_H

#include <shell/shell.h>

enum {
	EC_SUCCESS	 = 0,
};

enum console_channel {
	CC_COMMAND	= 0,
};

__attribute__((__format__(__printf__, 2, 3)))
int cprints(enum console_channel channel, const char *format, ...);

#define ccprintf(format, args...) cprintf(CC_COMMAND, format, ## args)

/**
 * Print formatted output to the console channel.
 *
 * @param channel	Output chanel
 * @param format	Format string; see printf.h for valid formatting codes
 *
 * @return non-zero if output was truncated.
 */
__attribute__((__format__(__printf__, 2, 3)))
int cprintf(enum console_channel channel, const char *format, ...);

/**
 * Print formatted output with timestamp. This is like:
 *   cprintf(channel, "[%pT " + format + "]\n", PRINTF_TIMESTAMP_NOW, ...)
 *
 * @param channel	Output channel
 * @param format	Format string; see printf.h for valid formatting codes
 *
 * @return non-zero if output was truncated.
 */
__attribute__((__format__(__printf__, 2, 3)))
int cprints(enum console_channel channel, const char *format, ...);

/**
 * Flush the console output for all channels.
 */
void cflush(void);

#define DECLARE_SAFE_CONSOLE_COMMAND(NAME, ROUTINE, ARGDESC, HELP)	\
	SHELL_CMD_REGISTER(NAME, NULL, HELP, ROUTINE)

/* Microsecond timestamp. */
typedef union {
	uint64_t val;
	struct {
		uint32_t lo;
		uint32_t hi;
	} le /* little endian words */;
} timestamp_t;

/**
 * Get the current timestamp from the system timer.
 */
timestamp_t get_time(void);

/* Returns the timestamp of the next programed event */
uint32_t __hw_clock_event_get(void);

#endif /* ZEPHYR_INCLUDE_CROS_EC_H */
