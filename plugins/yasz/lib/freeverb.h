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


#ifndef PLUGINS_YASZ_LIB_FREEVERB_H_
#define PLUGINS_YASZ_LIB_FREEVERB_H_
#include <stdint.h>

#include "comb.h"
#include "allpass.h"

#define FV_FIXEDGAIN    0.015f
#define FV_SCALEWET     3.0f
#define FV_SCALEDRY     2.0f
#define FV_SCALEDAMP    0.4f
#define FV_SCALEROOM    0.28f
#define FV_OFFSETROOM   0.7f
#define FV_STEREOSPREAD 23
#define FV_NUMCOMBS     8
#define FV_NUMALLPASSES 4

#define FV_INITIALSR    44100
#define FV_INITIALROOM  0.5
#define FV_INITIALDAMP  0.5
#define FV_INITIALWET   (0.0 / FV_SCALEWET)
#define FV_INITIALDRY   0.9
#define FV_INITIALWIDTH 1.0

typedef struct t_freeverb {
    uint32_t  samplerate;

    double    outL;
    double    outR;

    float     gain;
    float     roomsize, roomsize1;
    float     damp, damp1;
    float     wet, wet1, wet2;
    float     dry;
    float     width;

    // Comb filters
    COMB    combL[FV_NUMCOMBS];
    COMB    combR[FV_NUMCOMBS];

    // Allpass filters
    ALLPASS allpassL[FV_NUMALLPASSES];
    ALLPASS allpassR[FV_NUMALLPASSES];

} FREEVERB;

FREEVERB freeverb_new(uint32_t samplerate);
double freeverb_out(FREEVERB* p);
void freeverb_tick(FREEVERB* p, double inputL, double inputR);

#endif  // PLUGINS_YASZ_LIB_FREEVERB_H_
