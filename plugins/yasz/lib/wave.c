/*
 * YASZ (Yet Another Simple synthesiZer)
 * Copyright (C) 2020 Victor Rosales <todoesverso@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


/** @file wave.c */

#include <stdint.h>
#include <math.h>
#include "wave.h"
#include "utils.h"

uint8_t tables_initialized = 0;

double yasz_square_t[HARMONICS][TLEN + 1];
double yasz_triangle_t[HARMONICS][TLEN + 1];
double yasz_saw_t[HARMONICS][TLEN + 1];

static void
normalize_rt(double* t) {
    double val, maxamp = 0.0f;
    uint16_t i;

    for (i = 0; i < TLEN; i++) {
        val = fabs(t[i]);
        if (maxamp < val) {
            maxamp = val;
        }
    }

    maxamp = 1.0 / maxamp;

    for (i = 0; i < TLEN; i++) {
        t[i] *= maxamp;
    }
    t[i] = t[0];
}

void
lookup_init_rt() {
    if (tables_initialized == 1) {
        return;
    }

    uint16_t harm_odd = 1, harm = 1;
    double ampsq = 0.0f, amptr = 0.0f, ampsaw = 0.0f;
    double tmp, tmp_saw;
    double twopioversize = 2 * M_PI / TLEN;

    for (uint16_t i = 0; i < HARMONICS; i++) {
        ampsaw = 1.0 / harm;
        ampsq = 1.0 / harm_odd;
        amptr = 1.0 / (harm_odd * harm_odd);
        for (uint16_t j = 0; j < TLEN + 1; j++) {
            tmp = j * harm_odd * twopioversize;
            tmp_saw = j * harm * twopioversize;
            yasz_square_t[i][j] = ampsq * sin(tmp);
            yasz_saw_t[i][j] = ampsaw * sin(tmp_saw);
            yasz_triangle_t[i][j] = amptr * cos(tmp);
            if (i > 0) {
                yasz_saw_t[i][j] += yasz_saw_t[i - 1][j];
                yasz_square_t[i][j] += yasz_square_t[i - 1][j];
                yasz_triangle_t[i][j] += yasz_triangle_t[i - 1][j];
            }
        }
        harm++;
        harm_odd += 2;
    }

    for (uint16_t i = 0; i < HARMONICS; i++) {
        normalize_rt(yasz_saw_t[i]);
        normalize_rt(yasz_square_t[i]);
        normalize_rt(yasz_triangle_t[i]);
    }

    tables_initialized = 1;
}
