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
 * 	_ _ U P T I M E
 *
 * get uptime and return this in
 * double.
 */
int
__uptime(char *uptime, size_t n)
{
	assert(uptime);
	assert(n);

#ifdef __FreeBSD__
	struct timeval t;
	size_t siz;
	int mib[2];
	double res;
	time_t t1;

	siz = sizeof(t);
	mib[0] = CTL_KERN;
	mib[1] = KERN_BOOTTIME;

	if (sysctl(mib, 2, &t, &siz, NULL, 0) != 0)
		return __ERR;

	t1 = time(NULL);
	res = difftime(t1, t.tv_sec);

	snprintf(uptime, n, "%d hours, %.1f minutes", (int)res / 3600,
	    res / 60.0);

	return __OK;
#else
	FILE *fp;
	double t;

	/* not sysinfo() */
	if (!(fp = fopen("/proc/uptime", "r")))
		return __ERR;
	if (fscanf(fp, "%lf", &t) != 1) {
		fclose(fp);
		return __ERR;
	}
	fclose(fp);

	snprintf(uptime, n, "%.0f hours, %.1f minutes", t / 3600,
	    fmod(t, 3600) / 60.0);

	return __OK;
#endif
}
