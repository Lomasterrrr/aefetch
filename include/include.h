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

#ifndef __INCLUDE_H
#define __INCLUDE_H

#include <sys/time.h>
#include <sys/utsname.h>

#include <assert.h>
#include <ctype.h>
#include <getopt.h>
#include <locale.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

#ifdef __FreeBSD__
#include <sys/sysctl.h>
#endif

#define ANSI_RESET "\x1b[0m"

typedef enum {
	ANSI_S_BOLD = 1u << 0,
	ANSI_S_DIM = 1u << 1,
	ANSI_S_ITALIC = 1u << 2,
	ANSI_S_UNDERLINE = 1u << 3,
	ANSI_S_STRIKE = 1u << 4,
	ANSI_S_INVERSE = 1u << 5,
	ANSI_S_OVERLINE = 1u << 6
} ansi_style_t;

typedef struct __FETCHSTYLE {
	const char *fg, *bg;
	u_int style;
} FETCHSTYLE;

typedef struct __FETCHLINE {
	const char *desc;	      /* description */
	int space;		      /* space after desc */
	FETCHSTYLE style;	      /* style for desc */
	int (*value)(char *, size_t); /* function for get value */
	const char *resval;	      /* reserved value */
	FETCHSTYLE styleval;	      /* style for value */
	int spacel;		      /* space from logo */
	int skip;		      /* skip n lines before this */
} FETCHLINE;

typedef struct __FETCHDATA {
	size_t linesn;
	const char **logo;
	size_t logon;
} FETCHDATA;

typedef struct __FETCH {
	FETCHDATA data;
	FETCHLINE *lines;
} FETCH;

int hex2ansi(const char *hex, bool fg, bool truecolor, bool base16, char *buf,
    size_t n);
bool setstyleansi(char *buf, int n, u_int style);
size_t unsetstyleansi(char *out, size_t outsz, u_int flags);

size_t ansistrlen(const char *s);

#define __OK  (int)0
#define __ERR (int)-1

int __hostname(char *name, size_t n);
int __uptime(char *uptime, size_t n);
int __osinfo(char *osinfo, size_t n);
int __kernel(char *kernel, size_t n);
int __terminal(char *terminal, size_t n);
int __memory(char *memory, size_t n);
int __cpu(char *cpu, size_t n);

#endif
