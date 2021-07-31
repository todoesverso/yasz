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


#ifndef PLUGINS_YASZ_LIB_VOICE_H_
#define PLUGINS_YASZ_LIB_VOICE_H_

#include <stdint.h>
#include "adsr.h"
#include "osct.h"
#include "midi.h"
#include "ks.h"

#define VOICE_MAX_VOICES 10

typedef struct voice_t {
    ADSR* adsr;
    OSCT* sub;
    OSCT* osct1;
    OSCT* osct2;
    MIDI* midi;
    KS*   ks;
    KS*   ks1;
    KS*   ks2;

    float sub_gain;
    float osct1_gain;
    float osct2_gain;
    float ks_gain;
    double left;
    double right;
    double srate;
} VOICE;

VOICE* voice_new(uint32_t const srate);
void voice_render_rt(VOICE* p);
void voice_freq_rt(VOICE* p, double freq);
void voice_osct1_gain_rt(VOICE* p, float gain);
void voice_osct2_gain_rt(VOICE* p, float gain);
void voice_sub_gain_rt(VOICE* p, float gain);
void voice_ks_gain_rt(VOICE* p, float gain);
void voice_sub_wavetype_rt(VOICE* p, uint8_t wt);
void voice_osct1_wavetype_rt(VOICE* p, uint8_t wt);
void voice_osct2_wavetype_rt(VOICE* p, uint8_t wt);
#define voice_is_free(v) ((v->midi->notestate == NOTE_OFF) ? 1 : 0)

#endif  // PLUGINS_YASZ_LIB_VOICE_H_
