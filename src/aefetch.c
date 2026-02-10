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

#include "../config.h"

inline static void
getstyleline(FETCHSTYLE *style, size_t n, char *fg, char *bg, char *son,
    char *soff)
{
	setstyleansi(son, n, style->style);
	unsetstyleansi(soff, n, style->style);
	hex2ansi(style->fg, true, true, false, fg, n);
	hex2ansi(style->bg, false, true, false, bg, n);
}

static inline void
fillspace(char *buf, size_t n, size_t max)
{
	n = n >= max ? max - 1 : n;
	memset(buf, ' ', n);
	buf[n] = 0;
}

/*
 * 	C O M P I L E
 *
 * displays fetch along with the logo
 * and all styling; main function
 */
inline static void
compile(FETCH *fetch)
{
	char bg[512], fg[512], son[512], soff[512];
	char desc[2048], skip[512], tval[2048];
	size_t logon = 0, off = 0, n, i;
	const char *lp;

	if (!fetch)
		return;

	for (n = 0; n < fetch->data.logon; n++)
		if ((ansistrlen(fetch->data.logo[n])) > off)
			off = ansistrlen(fetch->data.logo[n]);

	for (n = 0; n < fetch->data.linesn; n++) {
		i = fetch->lines[n].skip;
		while (i--) {
			if (logon < fetch->data.logon)
				puts(fetch->data.logo[logon++]);
			else
				putchar('\n');
		}

		getstyleline(&fetch->lines[n].style, sizeof(son), fg, bg, son,
		    soff);
		fillspace(skip, fetch->lines[n].space, sizeof(skip));
		snprintf(desc, sizeof(desc), "%s%s%s%s%s%s%s", bg, fg, son,
		    (fetch->lines[n].desc) ? fetch->lines[n].desc : "", soff,
		    ANSI_RESET, skip);

		getstyleline(&fetch->lines[n].styleval, sizeof(son), fg, bg,
		    son, soff);

		if (fetch->lines[n].value)
			fetch->lines[n].value(tval, sizeof(tval));
		else if (fetch->lines[n].resval)
			snprintf(tval, sizeof(tval), "%s",
			    fetch->lines[n].resval);
		else
			*tval = 0;

		lp = (logon < fetch->data.logon) ? fetch->data.logo[logon++] :
						   "";
		i = ansistrlen(lp);
		fillspace(skip,
		    ((off > i) ? off - i : 0) +
			fetch->lines[n].spacel,
		    sizeof(skip));

		printf("%s%s%s%s%s%s%s%s%s\n", lp, skip, desc, bg, fg, son,
		    tval, soff, ANSI_RESET);
	}

	while (logon < fetch->data.logon)
		puts(fetch->data.logo[logon++]);
}

/*
 * 	A E F E T C H
 */
int
main(void)
{
	FETCH fetch = { .lines = LINES,
		.data.logo = LOGO,
		.data.linesn = sizeof(LINES) / sizeof(*LINES),
		.data.logon = sizeof(LOGO) / sizeof(*LOGO) };
	setlocale(LC_ALL, "");
	compile(&fetch);
	return 0;
}
