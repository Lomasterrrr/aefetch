/*
 * Copyright (c) 2026, Ae-foundation. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * software name includes “ae”.
 *
 * THE SOFTWARE IS PROVIDED “AS IS” AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE
 * FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "../include/include.h"

/*
 *	B Y T E S F M T
 *
 * convert <n> bytes to formatted
 * string presintation.
 */
inline static const char *
bytesfmt(size_t n)
{
	const char *units[] = { "B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB" };
	double c = (double)n;
	static char fmt[32];

	for (n = 0; c >= 1024 && n < 6; n++)
		c /= 1024;

	snprintf(fmt, sizeof(fmt), "%.2f %s", c, units[n]);

	return fmt;
}

/*
 * 	_ _ M E M O R Y
 *
 * get RAM in string with fmt
 * <used / total>; dynamic size
 */
int
__memory(char *memory, size_t n)
{
	assert(n);
	assert(memory);

#ifdef __FreeBSD__
	long free = 0, inactive = 0, cached = 0;
	u_long total = 0, used = 0;
	char line[256];
	size_t len;

	len = sizeof(total);
	if (sysctlbyname("hw.physmem", &total, &len, NULL, 0) != 0)
		return __ERR;

	len = sizeof(free);
	if (sysctlbyname("vm.stats.vm.v_free_count", &free, &len, NULL, 0) != 0)
		return __ERR;

	len = sizeof(inactive);
	if (sysctlbyname("vm.stats.vm.v_inactive_count", &inactive, &len, NULL,
		0) != 0)
		return __ERR;

	len = sizeof(cached);
	if (sysctlbyname("vm.stats.vm.v_cache_count", &cached, &len, NULL, 0) !=
	    0)
		cached = 0;

	used = total -
	    ((u_long)(free + inactive + cached) * (u_long)getpagesize());
	snprintf(line, sizeof(line), "%s", bytesfmt(used));
	snprintf(memory, n, "%s / %s", line, bytesfmt(total));

	return __OK;
#else
	FILE *fp;
	char line[256];
	u_long tot, free, buff, used, cached;

	if (!(fp = fopen("/proc/meminfo", "r")))
		return __ERR;
	while (fgets(line, sizeof(line), fp)) {
		if (sscanf(line, "MemTotal: %lu kB", &tot) == 1)
			continue;
		if (sscanf(line, "MemFree: %lu kB", &free) == 1)
			continue;
		if (sscanf(line, "Buffers: %lu kB", &buff) == 1)
			continue;
		if (sscanf(line, "Cached: %lu kB", &cached) == 1)
			continue;
	}
	fclose(fp);

	used = tot - free - buff - cached;

	/* to bytes */
	used *= 1024UL;
	tot *= 1024UL;

	snprintf(line, sizeof(line), "%s", bytesfmt(used));
	snprintf(memory, n, "%s / %s", line, bytesfmt(tot));

	return __OK;
	;
#endif
}
