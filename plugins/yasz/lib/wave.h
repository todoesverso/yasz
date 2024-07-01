/*
 * YASZ (Yet Another Simple synthesiZer)
 * Copyright (C) 2020 Victor Rosales <todoesverso@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/** @file wave.h */
#ifndef PLUGINS_YASZ_LIB_WAVE_H_
#define PLUGINS_YASZ_LIB_WAVE_H_

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#define HARMONICS (128)
#define TLEN (2048)
#define MAX_FREQ (8000)

extern bool tables_initialized;

extern double yasz_square_t[HARMONICS][TLEN + 1];
extern double yasz_triangle_t[HARMONICS][TLEN + 1];
extern double yasz_saw_t[HARMONICS][TLEN + 1];
#define yasz_sine_t (yasz_square_t)

void lookup_init_rt();
#endif // PLUGINS_YASZ_LIB_WAVE_H_
