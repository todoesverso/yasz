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

#include <stdlib.h>
#include "midi.h"
#include "osct.h"
#include "adsr.h"
#include "voice.h"
#include "ks.h"
#include "common.h"


static VOICE*
voice_malloc() {
    VOICE* p;
    p = (VOICE*) malloc(sizeof(VOICE));  // NOLINT(readability/casting)
    return p;
}

static void
voice_init(VOICE* p, uint32_t const srate) {
    p->srate = srate;
    p->sub = osct_new(srate);
    p->osct1 = osct_new(srate);
    p->osct2 = osct_new(srate);
    p->ks = ks_new(srate);
    p->ks1 = ks_new(srate);
    p->ks2 = ks_new(srate);
    voice_sub_wavetype_rt(p, YASZ_SINE_T);
    voice_osct1_wavetype_rt(p, YASZ_SAW_T);
    voice_osct2_wavetype_rt(p, YASZ_TRIANGLE_T);
    p->adsr = adsr_new();
    p->midi = midi_new();
    p->sub_gain = 0.6f;
    p->osct1_gain = 0.4f;
    p->osct2_gain = 0.3f;
    p->ks_gain = 0.2f;
    p->left = 0.0f;
    p->right = 0.0f;

    adsr_set_attack_rate_rt(p->adsr, 0.1f * srate);
    adsr_set_decay_rate_rt(p->adsr, 0.3f * srate);
    adsr_set_release_rate_rt(p->adsr, 2.0f * srate);
    adsr_set_sustain_level_rt(p->adsr, 0.8f);
}

void
voice_attack_rate(VOICE* p, double rate) {
    adsr_set_attack_rate_rt(p->adsr, rate * p->srate);
}

void
voice_decay_rate(VOICE* p, double rate) {
    adsr_set_decay_rate_rt(p->adsr, rate * p->srate);
}

void
voice_release_rate(VOICE* p, double rate) {
    adsr_set_release_rate_rt(p->adsr, rate * p->srate);
}

void
voice_sustain_level(VOICE* p, double level) {
    adsr_set_sustain_level_rt(p->adsr, level);
}

/**
 * Mixer of OSC (?)
 **/
static inline double
voice_osct_render_rt(VOICE* p) {
    return osct_get_out_rt(p->osct1) * p->osct1_gain +
           osct_get_out_rt(p->osct2) * p->osct2_gain +
           osct_get_out_rt(p->sub) * p->sub_gain;
}

static inline void
voice_ks_render_rt(VOICE* p) {
    double base_ks = ks_render_rt(p->ks);
    p->left += (base_ks +
                ks_render_rt(p->ks1)) * p->ks_gain;
    p->right += (base_ks +
                 ks_render_rt(p->ks2)) * p->ks_gain;
}

VOICE*
voice_new(uint32_t const srate) {
    VOICE* p = voice_malloc();
    if (p == NULL)
    { return NULL; }
    voice_init(p, srate);
    return p;
}

void
voice_render_rt(VOICE* p) {
    double env = adsr_process_rt(p->adsr);
    p->left = voice_osct_render_rt(p) * env;
    p->right = voice_osct_render_rt(p) * env;
    voice_ks_render_rt(p);
}

void
voice_sub_gain_rt(VOICE* p, float gain) {
    p->sub_gain = gain;
}

void
voice_osct1_gain_rt(VOICE* p, float gain) {
    p->osct1_gain = gain;
}

void
voice_osct2_gain_rt(VOICE* p, float gain) {
    p->osct2_gain = gain;
}

void
voice_ks_gain_rt(VOICE* p, float gain) {
    p->ks_gain = gain;
}

void
voice_freq_rt(VOICE* p, double freq) {
    osct_freq_rt(p->sub, freq * 0.25f);
    osct_freq_rt(p->osct1, freq);
    osct_freq_rt(p->osct2, freq);
    ks_freq_rt(p->ks, freq);
    ks_freq_rt(p->ks2, freq + 1.f);
    ks_freq_rt(p->ks1, freq - 1.f);
}

void
voice_sub_wavetype_rt(VOICE* p, uint8_t wavetype) {
    osct_wavetype_rt(p->sub, wavetype);
}

void
voice_osct1_wavetype_rt(VOICE* p, uint8_t wavetype) {
    osct_wavetype_rt(p->osct1, wavetype);
}

void
voice_osct2_wavetype_rt(VOICE* p, uint8_t wavetype) {
    osct_wavetype_rt(p->osct2, wavetype);
}


