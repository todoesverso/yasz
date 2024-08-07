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

#include "yasz.h"
#include "adsr.h"
#include "dly.h"
#include "freeverb.h"
#include "midi.h"
#include "pan.h"
#include "voice.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

extern inline PANPOS constantpower_rt(double possition);
static YASZ *yasz_malloc();
static void yasz_init(YASZ *p, uint32_t const srate);

static YASZ *yasz_malloc() {
  YASZ *p;
  p = (YASZ *)malloc(sizeof(YASZ)); // NOLINT(readability/casting)
  return p;
}

static void yasz_init(YASZ *p, uint32_t const srate) {
  p->dly = dly_new(srate, 500, 0.6);
  p->freeverb = freeverb_new(srate);

  p->pan_value = 0.0f;
  p->pan_position = constantpower_rt(0.0f);

  p->left = 0.0f;
  p->right = 0.0f;

  for (uint8_t i = 0; i < VOICE_MAX_VOICES; i++) {
    p->voice[i] = voice_new(srate);
  }
}

YASZ *yasz_new(uint32_t srate) {
  YASZ *p_yasz = yasz_malloc();
  if (p_yasz == NULL) {
    return NULL;
  }
  yasz_init(p_yasz, srate);
  return p_yasz;
}

void yasz_pan_rt(YASZ *p, double pan_value) {
  p->pan_value = pan_value;
  p->pan_position = constantpower_rt(pan_value);
}

void yazs_mixer_rt(YASZ *p) {
  double left = 0.0f;
  double right = 0.0f;

  for (uint8_t i = 0; i < VOICE_MAX_VOICES; i++) {
    voice_render_rt(&p->voice[i]);
    left += p->voice[i].left;
    right += p->voice[i].right;
  }
  // left += dly_out(&p->dly);
  // right += dly_out(&p->dly);

  p->left = left * p->pan_position.left * 0.3;
  p->right = right * p->pan_position.right * 0.3;
}

void yasz_render_rt(YASZ *p) {
  // dly_tick(&p->dly, p->left);
  yazs_mixer_rt(p);
  freeverb_tick(&p->freeverb, p->left, p->right);
  p->left = p->freeverb.outL;
  p->right = p->freeverb.outR;
}

VOICE *yasz_get_free_voice(YASZ *p) {
  for (uint8_t i = 0; i < VOICE_MAX_VOICES; i++)
    if (voice_is_free(p->voice[i])) {
      return &p->voice[i];
    }

  return NULL;
}

VOICE *yasz_get_on_voice(YASZ *p, uint8_t note) {
  for (uint8_t i = 0; i < VOICE_MAX_VOICES; i++)
    if (p->voice[i].midi.midinote == note) {
      return &p->voice[i];
    }

  return NULL;
}

void yasz_proc_midi(YASZ *p_yasz, const uint8_t size,
                    const uint8_t *const data) {
  if (size > 3) {
    return;
  }

  VOICE *p_free_voice = yasz_get_free_voice(p_yasz);
  const uint8_t status = midi_get_status_rt(data);
  const uint8_t note = midi_get_note_rt(data);
  const uint8_t velo = midi_get_note_vel_rt(data);
  VOICE *p_voice_on = yasz_get_on_voice(p_yasz, note);

  switch (status) {
  case MIDI_NOTE_ON:
    if (p_voice_on && velo == 0) {
      p_voice_on->midi.notestate = NOTE_OFF;
      p_voice_on->midi.midinote = NOTE_OFF;
      adsr_gate_off_rt(&p_voice_on->adsr);
    }

    if (velo > 0 && p_free_voice) {
      p_free_voice->midi.notestate = NOTE_ON;
      p_free_voice->midi.midinote = note;
      double freq = midi_to_freq[note];
      // double freq = midi_to_freq_rt(note, 440);
      voice_freq_rt(p_free_voice, freq);
      adsr_gate_on_rt(&p_free_voice->adsr);
    }
    break;
  case MIDI_NOTE_OFF:
    if (p_voice_on) {
      p_voice_on->midi.notestate = NOTE_OFF;
      p_voice_on->midi.midinote = NOTE_OFF;
      adsr_gate_off_rt(&p_voice_on->adsr);
    }
    break;
  }

  return;
}
