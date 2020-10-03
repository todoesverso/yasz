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


static VOICE* voice_malloc();
static void voice_init(VOICE* p, uint32_t const srate);

static VOICE* voice_malloc() {
  VOICE* p;
  p = (VOICE*) malloc(sizeof(VOICE));  // NOLINT(readability/casting)
  return p;
}

static void voice_init(VOICE* p, uint32_t const srate) {
  p->osct = osct_new(srate);
  osct_wavetype_saw_rt(p->osct);
  p->adsr = adsr_new();
  p->midi = midi_new();
  p->left = 0.0f;
  p->right = 0.0f;

  adsr_set_attack_rate_rt(p->adsr, 0.1f * srate);
  adsr_set_decay_rate_rt(p->adsr, 0.3f * srate);
  adsr_set_release_rate_rt(p->adsr, 2.0f * srate);
  adsr_set_sustain_level_rt(p->adsr, 0.8f);
}

VOICE* voice_new(uint32_t const srate) {
  VOICE* p = voice_malloc();
  if (p == NULL)
    return NULL;
  voice_init(p, srate);
  return p;
}

void voice_render_rt(VOICE *p) {
  double env = adsr_process_rt(p->adsr);
  p->left = osct_get_out_rt(p->osct) * env;
  p->right = osct_get_out_rt(p->osct) * env;
}

void voice_freq_rt(VOICE *p, double freq) {
  osct_freq_rt(p->osct, freq);
}
