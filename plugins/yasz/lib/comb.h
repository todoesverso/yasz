/*
 * YASZ (Yet Another Simple synthesiZer)
 * Copyright (C) 2021 Victor Rosales <todoesverso@gmail.com>
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


#ifndef PLUGINS_YASZ_LIB_COMB_H_
#define PLUGINS_YASZ_LIB_COMB_H_
#include <stdint.h>

typedef struct t_comb {
    uint32_t  samplerate;
    float     feedback;
    float     filterstore;
    float     damp1;
    float     damp2;
    double    buffer[4096];
    uint32_t  buffsize;
    uint32_t  buffindex;
} COMB;


COMB comb_new(uint32_t samplerate, uint32_t buffsize);
double comb_out(COMB* p);
double comb_tick(COMB* p, double sample);
void comb_setdamp(COMB* p, float damp);
void comb_feedback(COMB* p, float feedback);
#endif  // PLUGINS_YASZ_LIB_COMB_H_
