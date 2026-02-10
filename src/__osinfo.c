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
 * 	_ _ O S I N F O
 *
 * get distr name (PRETTY) from
 * os-release and write this in
 * osinfo with len n.
 */
int
__osinfo(char *osinfo, size_t n)
{
	FILE *fp;
	char line[256];
	char *np;

	assert(n);
	assert(osinfo);

	if (!(fp = fopen("/etc/os-release", "r")))
		return __ERR;
	while (fgets(line, sizeof(line), fp)) {
		if (strncmp(line, "PRETTY_NAME=", 12) != 0)
			continue;
		np = strchr(line, '"') + 1;
		np[strcspn(np, "\n") - 1] = '\0';
		snprintf(osinfo, n, "%s", np);
		break;
	}

	fclose(fp);
	return __OK;
	;
}
