/*
 * YASZ (Yet Another Simple synthesiZer)
 * Copyright (C) 2021 Victor Rosales <todoesverso@gmail.com>
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

#include "dly.h"
#include <stdint.h>
#include <stdio.h>

DLY
dly_new(double samplerate, uint32_t delay_ms, double feedback) {
    DLY dly = {
        .samplerate = samplerate,
        .delay_samples = (uint32_t)(samplerate* delay_ms / 1000),
        .delay_ms = delay_ms,
        .feedback = feedback,
    };

    return dly;
}

void
dly_delay_ms(DLY* p_dly, uint32_t delay_ms) {
    p_dly->delay_samples = (uint32_t)(p_dly->samplerate * delay_ms / 1000);
}

void
dly_feedback(DLY* p_dly, double feedback) {
    p_dly->feedback = feedback;
}

double
dly_out(DLY* p_dly) {
    return p_dly->buffer[p_dly->r_index] * p_dly->feedback;
}

void
dly_tick(DLY* p_dly, double sample) {
    p_dly->buffer[p_dly->w_index] = sample;

    int32_t r_index_tmp = p_dly->w_index - p_dly->delay_samples;

    if (r_index_tmp < 0) {
        p_dly->r_index = r_index_tmp + MAXDLY;
    }

    p_dly->r_index = (p_dly->r_index + 1) % MAXDLY;
    p_dly->w_index = (p_dly->w_index + 1) % MAXDLY;
}
