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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "lib/osc.h"
#include "lib/midi.h"
#include "lib/yasz.h"
#include "lib/adsr.h"

static YASZ* yasz_malloc();
static void yasz_init(YASZ *p, double const srate);

static YASZ* yasz_malloc() {
  YASZ* p;
  p = (YASZ*) malloc(sizeof(YASZ));  // NOLINT(readability/casting)
  return p;
}

static void yasz_init(YASZ *p, double const srate) {
  p->p_osc = new_osc(srate);
  p->p_adsr = adsr_new();
  p->left = 0.0f;
  p->right = 0.0f;

  adsr_set_attack_rate(p->p_adsr, 0.1f * srate);
  adsr_set_decay_rate(p->p_adsr, 0.3f * srate);
  adsr_set_release_rate(p->p_adsr, 2.0f * srate);
  adsr_set_sustain_level(p->p_adsr, 0.8f);

  for (uint32_t i = 0; i <= MIDI_NOTES; i++)
    p->noteState[i] = NOTE_OFF;
}

YASZ* yasz_new(uint32_t srate) {
  YASZ* p_yasz = yasz_malloc();
  if (p_yasz == NULL)
    return NULL;
  yasz_init(p_yasz, srate);
  return p_yasz;
}

void yasz_render_rt(YASZ *p_yasz) {
  double env = adsr_process(p_yasz->p_adsr);
  p_yasz->left = osc_get_out_rt(p_yasz->p_osc) * env;
  p_yasz->right = osc_get_out_rt(p_yasz->p_osc) * env;
}

void yasz_proc_midi(YASZ *p_yasz,
    const uint8_t size,
    const uint8_t* const data) {
  if (size > 3)
    return;

  const uint8_t status = get_midi_status_rt(data);
  const uint8_t note = get_midi_note_rt(data);
  const uint8_t velo = get_midi_note_vel_rt(data);

  switch (status) {
  case MIDI_NOTE_ON:
    if (p_yasz->noteState[note] && velo == 0) {
        p_yasz->noteState[note] = NOTE_OFF;
        adsr_gate(p_yasz->p_adsr, 0);
    }

    if (velo > 0) {
      p_yasz->noteState[note] = NOTE_ON;
      update_freq_from_midi_note(p_yasz->p_osc, note);
      adsr_gate(p_yasz->p_adsr, 1);
    }
    break;
  case MIDI_NOTE_OFF:
    if (p_yasz->noteState[note]) {
      p_yasz->noteState[note] = NOTE_OFF;
      adsr_gate(p_yasz->p_adsr, 0);
    }
    break;
  }

  return;
}
