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

static inline u_char
hexval(u_char ch)
{
	if (ch >= '0' && ch <= '9')
		return (u_char)(ch - '0');
	if (ch >= 'a' && ch <= 'f')
		return (u_char)(ch - 'a' + 10);
	if (ch >= 'A' && ch <= 'F')
		return (u_char)(ch - 'A' + 10);
	return 255;
}

static inline bool
parsehex(const char *s, u_char *r, u_char *g, u_char *b)
{
	u_char v[3], hi, lo;
	size_t n = 0;

	if (!s || !r || !g || !b)
		return 0;
	if (*s == '#')
		++s;

	n = strlen(s);
	switch (n) {
	case 6:
		for (n = 0; n < 3; n++) {
			hi = hexval(s[n * 2]);
			lo = hexval(s[n * 2 + 1]);
			if (hi > 15 || lo > 15)
				return 0;
			v[n] = (u_char)((hi << 4) | lo);
		}
		break;
	case 3:
		for (n = 0; n < 3; n++) {
			hi = hexval(s[n]);
			if (hi > 15)
				return 0;
			v[n] = (u_char)((hi << 4) | hi);
		}
		break;
	default:
		return 0;
	}

	*r = v[0];
	*g = v[1];
	*b = v[2];

	return 1;
}

static inline double
srgbtolinear(double c)
{
	c /= 255.0;
	return (c <= 0.04045) ? (c / 12.92) : pow((c + 0.055) / 1.055, 2.4);
}

static inline void
rgbtolab(u_char r, u_char g, u_char b, double *L, double *a, double *B)
{
	double fx, fy, fz;
	double X, Y, Z;

	fx = srgbtolinear(r);
	fy = srgbtolinear(g);
	fz = srgbtolinear(b);

	X = 0.4124564 * fx + 0.3575761 * fy + 0.1804375 * fz;
	Y = 0.2126729 * fx + 0.7151522 * fy + 0.0721750 * fz;
	Z = 0.0193339 * fx + 0.1191920 * fy + 0.9503041 * fz;

#define flab(t)                                              \
	(t > ((6.0 / 29.0) * (6.0 / 29.0) * (6.0 / 29.0))) ? \
	    cbrt(t) :                                        \
	    (t / (3 * (6.0 / 29.0) * (6.0 / 29.0)) + 4.0 / 29.0);

	fx = flab(X / 0.95047);
	fy = flab(Y / 1.00000);
	fz = flab(Z / 1.08883);
#undef flab

	*L = 116.0 * fy - 16.0;
	*a = 500.0 * (fx - fy);
	*B = 200.0 * (fy - fz);
}

static inline double
deltaE2Lab(u_char r1, u_char g1, u_char b1, u_char r2, u_char g2, u_char b2)
{
	double L1, a1, b1c, L2, a2, b2c;
	rgbtolab(r1, g1, b1, &L1, &a1, &b1c);
	rgbtolab(r2, g2, b2, &L2, &a2, &b2c);
	double dL = L1 - L2, da = a1 - a2, db = b1c - b2c;
	return dL * dL + da * da + db * db; /* CIE76^2 */
}

static const int L[6] = { 0, 95, 135, 175, 215, 255 };
static const int base16[16][3] = { { 0, 0, 0 }, { 205, 0, 0 }, { 0, 205, 0 },
	{ 205, 205, 0 }, { 0, 0, 238 }, { 205, 0, 205 }, { 0, 205, 205 },
	{ 229, 229, 229 }, { 127, 127, 127 }, { 255, 0, 0 }, { 0, 255, 0 },
	{ 255, 255, 0 }, { 92, 92, 255 }, { 255, 0, 255 }, { 0, 255, 255 },
	{ 255, 255, 255 } };

static inline void
indextorgb256(int idx, u_char *r, u_char *g, u_char *b)
{
	u_char i, k, r6, g6, b6, v;

	if (idx >= 16 && idx <= 231) {
		k = idx - 16;
		r6 = k / 36;
		g6 = (k % 36) / 6;
		b6 = k % 6;
#define level6(i) L[i < 0 ? 0 : i > 5 ? 5 : i]
		*r = level6(r6);
		*g = level6(g6);
		*b = level6(b6);
#undef level6
	} else if (idx >= 232 && idx <= 255) {
		v = 8 + 10 * (idx - 232);
		*r = *g = *b = v;
	} else {
		i = (idx < 0 ? 0 : idx > 15 ? 15 : idx);
		*r = base16[i][0];
		*g = base16[i][1];
		*b = base16[i][2];
	}
}

static inline int
xterm256index(u_char r, u_char g, u_char b, bool base16)
{
	double best = 1e100, d2;
	int best_idx = 16, idx;
	u_char rr, gg, bb;

	for (idx = base16 ? 0 : 16; idx <= 255; ++idx) {
		indextorgb256(idx, &rr, &gg, &bb);
		d2 = deltaE2Lab(r, g, b, rr, gg, bb);
		if (d2 < best) {
			best = d2;
			best_idx = idx;
		}
	}

	return best_idx;
}

static inline bool
truecolorsupport(void)
{
	const char *s;
	s = getenv("COLORTERM");
	if (s && (strstr(s, "truecolor") || strstr(s, "24bit")))
		return 1;
	s = getenv("TERM");
	if (s && strstr(s, "-truecolor"))
		return 1;
	return 0;
}

int
hex2ansi(const char *hex, bool fg, bool truecolor, bool base16, char *buf,
    size_t n)
{
	u_char R, G, B;

	if (!hex || !buf || n < 8)
		return 0;
	if (!parsehex(hex, &R, &G, &B))
		return 0;

	/* true color */
	if (truecolor && truecolorsupport())
		return snprintf(buf, n, "\x1b[%hhu;2;%hhu;%hhu;%hhum",
		    fg ? 38 : 48, R, G, B);

	/* xterm color */
	return snprintf(buf, n, "\x1b[%hhu;5;%hhum", fg ? 38 : 48,
	    xterm256index(R, G, B, base16));
}

static bool
__styleansi(char *buf, int n, u_int style, const char **styles)
{
	bool flag = 0;
	int i;

	if (!buf || n < 5 || !style)
		return 0;

	*buf++ = '\x1b';
	*buf = '[';
	n -= 4; /* and 'm' + 0 */

	for (i = 0; i <= 6; i++) {
		if (style & (1u << i)) {
			if ((n -= strlen(styles[i]) + flag) < 0)
				goto end;
			if (flag)
				*++buf = ';';
			else
				flag = 1;
			while (*styles[i])
				*++buf = *styles[i]++;
		}
	}

end:
	*++buf = 'm';
	*++buf = 0;
	return 1;
}

bool
setstyleansi(char *buf, int n, u_int style)
{
	const char *styles[] = { "1", "2", "3", "4", "9", "7", "53" };
	return __styleansi(buf, n, style, styles);
}

size_t
unsetstyleansi(char *buf, size_t n, u_int style)
{
	const char *styles[] = { "22", "22", "23", "24", "29", "27", "55" };
	return __styleansi(buf, n, style, styles);
}