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
 * 	_ _ C P U
 *
 * get CPU INFO in string and
 * write to buffer.
 */
int
__cpu(char *cpu, size_t n)
{
	assert(n);
	assert(cpu);

#ifdef __FreeBSD__
	int mib[2];
	char model[256];
	int cores;
	size_t siz;

	siz = sizeof(model);
	mib[0] = CTL_HW;
	mib[1] = HW_MODEL;

	if (sysctl(mib, 2, model, &siz, NULL, 0) == -1)
		return __ERR;
	siz = strlen(model);
	while (siz > 0 &&
	    (model[siz - 1] == ' ' || model[siz - 1] == '\t' ||
		model[siz - 1] == '\n'))
		model[--siz] = '\0';

	siz = sizeof(cores);
	mib[0] = CTL_HW;
	mib[1] = HW_NCPU;

	if (sysctl(mib, 2, &cores, &siz, NULL, 0) == -1)
		return __ERR;

	snprintf(cpu, n, "%s (%d)", model, cores);
	return __OK;
#else
	FILE *fp;
	char line[256];
	char mbuf[256];
	char sbuf[256];

	*mbuf = *sbuf = 0;
	if (!(fp = fopen("/proc/cpuinfo", "r")))
		return __ERR;
	while (fgets(line, sizeof(line), fp)) {
		if (!strncmp(line,
			"model name\t: ", strlen("model name\t: "))) {
			snprintf(mbuf, sizeof(mbuf), "%s",
			    line + strlen("model name\t: "));
			mbuf[strlen(mbuf) - 1] = '\0';
		}
		if (!strncmp(line, "siblings\t: ", strlen("siblings\t: "))) {
			snprintf(sbuf, sizeof(sbuf), "%s",
			    line + strlen("siblings\t: "));
			sbuf[strlen(sbuf) - 1] = '\0';
		}
		if (*mbuf && *sbuf)
			break;
	}

	fclose(fp);

	snprintf(cpu, n, "%s (%s)", mbuf, sbuf);
	return __OK;
#endif
}
