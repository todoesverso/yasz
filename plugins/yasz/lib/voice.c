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

#include "voice.h"
#include "adsr.h"
#include "common.h"
#include "ks.h"
#include "midi.h"
#include "osct.h"
#include <stdlib.h>

extern double adsr_process_rt(ADSR *p);

void voice_attack_rate(VOICE *p, double rate) {
  adsr_set_attack_rate_rt(&p->adsr, rate * p->srate);
}

void voice_decay_rate(VOICE *p, double rate) {
  adsr_set_decay_rate_rt(&p->adsr, rate * p->srate);
}

void voice_release_rate(VOICE *p, double rate) {
  adsr_set_release_rate_rt(&p->adsr, rate * p->srate);
}

void voice_sustain_level(VOICE *p, double level) {
  adsr_set_sustain_level_rt(&p->adsr, level);
}

/**
 * Mixer of OSC (?)
 **/
static inline double voice_osct_render_rt(VOICE *p) {
  return osct_get_out_rt(&p->osc1) * p->osc1_gain +
         osct_get_out_rt(&p->osc2) * p->osc2_gain +
         osct_get_out_rt(&p->sub) * p->sub_gain;
}

static inline void voice_ks_render_rt(VOICE *p, double env) {
  double base_ks = ks_render_rt(&p->ks) * p->ks_gain * env;
  // p->left = base_ks;
  p->left += (base_ks + ks_render_rt(&p->ks1)) * p->ks_gain * env;
  // p->right = base_ks;
  p->right += (base_ks + ks_render_rt(&p->ks2)) * p->ks_gain * env;
}

VOICE
voice_new(uint32_t const srate) {
  VOICE voice = {
      .left = 0.0f,
      .right = 0.0f,
      .srate = srate,
      .sub_gain = 0.6f,
      .osc1_gain = 0.4f,
      .osc2_gain = 0.3f,
      .ks_gain = 0.5f,
      .sub = osct_new(srate),
      .osc1 = osct_new(srate),
      .osc2 = osct_new(srate),

      .ks = ks_new(srate),
      .ks1 = ks_new(srate),
      .ks2 = ks_new(srate),

      .adsr = adsr_new(),
      .midi = midi_new(),
  };

  voice_sub_wavetype_rt(&voice, YASZ_SINE_T);
  voice_osct1_wavetype_rt(&voice, YASZ_SINE_T);
  voice_osct2_wavetype_rt(&voice, YASZ_SINE_T);

  adsr_set_attack_rate_rt(&voice.adsr, 0.1f * srate);
  adsr_set_decay_rate_rt(&voice.adsr, 0.3f * srate);
  adsr_set_release_rate_rt(&voice.adsr, 2.0f * srate);
  adsr_set_sustain_level_rt(&voice.adsr, 0.8f);

  return voice;
}

void voice_render_rt(VOICE *p) {
  double env = adsr_process_rt(&p->adsr);
  p->left = voice_osct_render_rt(p) * env;
  p->right = voice_osct_render_rt(p) * env;
  voice_ks_render_rt(p, env); // TODO: env = 1.0 sound better
  voice_ks_render_rt(p, 1.0); // TODO: env = 1.0 sound better
}

void voice_sub_gain_rt(VOICE *p, float gain) { p->sub_gain = gain; }

void voice_osct1_gain_rt(VOICE *p, float gain) { p->osc1_gain = gain; }

void voice_osct2_gain_rt(VOICE *p, float gain) { p->osc2_gain = gain; }

void voice_ks_gain_rt(VOICE *p, float gain) { p->ks_gain = gain; }

void voice_freq_rt(VOICE *p, double freq) {
  osct_freq_rt(&p->sub, freq * 0.25f);
  osct_freq_rt(&p->osc1, freq);
  osct_freq_rt(&p->osc2, freq);
  ks_freq_rt(&p->ks, freq);
  ks_freq_rt(&p->ks2, freq + 1.f);
  ks_freq_rt(&p->ks1, freq - 1.f);
}

void voice_sub_wavetype_rt(VOICE *p, uint8_t wavetype) {
  osct_wavetype_rt(&p->sub, wavetype);
}

void voice_osct1_wavetype_rt(VOICE *p, uint8_t wavetype) {
  osct_wavetype_rt(&p->osc1, wavetype);
}

void voice_osct2_wavetype_rt(VOICE *p, uint8_t wavetype) {
  osct_wavetype_rt(&p->osc2, wavetype);
}
