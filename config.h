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

#ifndef __CONFIG_H
#define __CONFIG_H

#include "include/include.h"

static const char *LOGO[] = {
	"\x1b[1;33;44m               \x1b[1;37;44m__\x1b[1;35;44m.....\x1b[1;37;44m__\x1b[1;33;44m      \x1b[0m",
	"\x1b[1;33;44m           \x1b[1;36;44m.-\x1b[1;35;44m''\x1b[1;33;44m         \x1b[1;35;44m'.\x1b[1;33;44m    \x1b[0m",
	"\x1b[1;33;44m          \x1b[1;32;44m/\x1b[1;33;44m     \x1b[1;36;44m.-\x1b[1;35;44m''\x1b[1;37;44m\"\x1b[1;35;44m'\x1b[1;36;44m-.\x1b[1;33;44m  \x1b[1;37;44m`\x1b[1;35;44m.\x1b[1;33;44m  \x1b[0m",
	"\x1b[1;33;44m    \x1b[1;37;44m__\x1b[1;33;44m   \x1b[1;32;44m/\x1b[1;33;44m     \x1b[1;32;44m/\x1b[1;37;44m________\x1b[1;32;44m\\\x1b[1;33;44m   \x1b[1;32;44m\\ \x1b[0m",
	"\x1b[1;33;44m \x1b[1;35;44m:\x1b[1;31;44m--\x1b[1;35;44m'.\x1b[1;33;44m \x1b[1;32;44m|\x1b[1;33;44m                  \x1b[1;32;44m   |\x1b[0m",
	"\x1b[1;30;46m/ |   \\ |\\    .-------------' \x1b[0m",
	"\x1b[1;33;44m`\"\x1b[1;33;44m \x1b[1;37;44m__\x1b[1;33;44m \x1b[1;32;44m|\x1b[1;33;44m \x1b[1;32;44m|\x1b[1;33;44m \\\\    \x1b[1;31;44m'-\x1b[1;37;44m____\x1b[1;35;44m...\x1b[1;31;44m---\x1b[1;35;44m. \x1b[0m",
	"\x1b[1;33;44m .\x1b[1;35;44m'.''\x1b[1;32;44m|\x1b[1;33;44m \x1b[1;32;44m|\x1b[1;33;44m  \x1b[1;37;44m`\x1b[1;35;44m.\x1b[1;33;44m             \x1b[1;35;44m.'\x1b[1;33;44m  \x1b[0m",
	"\x1b[1;32;44m/\x1b[1;33;44m \x1b[1;32;44m/\x1b[1;33;44m   \x1b[1;32;44m|\x1b[1;33;44m \x1b[1;32;44m|_\x1b[1;33;44m   \x1b[1;37;44m`\x1b[1;35;44m''\x1b[1;31;44m-\x1b[1;35;44m......\x1b[1;33;44m \x1b[1;31;44m-'\x1b[1;33;44m    \x1b[0m",
	"\x1b[1;33;44m\\ \x1b[1;32;44m\\\x1b[1;35;44m._,\x1b[1;32;44m\\\x1b[1;33;44m \x1b[1;35;44m'/\x1b[1;33;44m                    \x1b[0m",
	"\x1b[1;33;44m \x1b[1;35;44m`\"\x1b[1;33;44m--\x1b[1;35;44m'\x1b[1;33;44m                        \x1b[0m"
};

static FETCHLINE LINES[] = {
	{ "OS:", 1, { "#FFFF55", "#0000AA", ANSI_S_BOLD }, __osinfo, "Linux",
	    { "#55FFFF", "#000000", ANSI_S_UNDERLINE | ANSI_S_ITALIC }, 4, 2},

	{ "Hostname:", 1, { "#FFFF55", "#0000AA", ANSI_S_BOLD }, __hostname, "",
	    { "#55FF55", "#000000", ANSI_S_UNDERLINE | ANSI_S_ITALIC }, 4, 0 },

	{ "Kernel:", 1, { "#FFFF55", "#0000AA", ANSI_S_BOLD }, __kernel, "",
	    { "#AAAAAA", "#000000", ANSI_S_UNDERLINE | ANSI_S_ITALIC }, 4, 0 },

	{ "Uptime:", 1, { "#FFFF55", "#0000AA", ANSI_S_BOLD }, __uptime, "",
	    { "#55FF55", "#000000", ANSI_S_UNDERLINE | ANSI_S_ITALIC }, 4, 0 },

	{ "Terminal:", 1, { "#FFFF55", "#0000AA", ANSI_S_BOLD }, __terminal, "",
	    { "#FF55FF", "#000000", ANSI_S_UNDERLINE | ANSI_S_ITALIC }, 4, 0 },

	{ "CPU:", 1, { "#FFFF55", "#0000AA", ANSI_S_BOLD }, __cpu, "",
	    { "#FF5555", "#000000", ANSI_S_UNDERLINE | ANSI_S_ITALIC }, 4, 0 },

	{ "Memory:", 1, { "#FFFF55", "#0000AA", ANSI_S_BOLD }, __memory, "",
	    { "#55FF55", "#000000", ANSI_S_UNDERLINE | ANSI_S_ITALIC }, 4, 0 },
};

#endif
