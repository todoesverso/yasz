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
#include "midi.h"
#include "yasz.h"
#include "adsr.h"
#include "voice.h"

static YASZ* yasz_malloc();
static void yasz_init(YASZ *p, uint32_t const srate);

static YASZ* yasz_malloc() {
  YASZ* p;
  p = (YASZ*) malloc(sizeof(YASZ));  // NOLINT(readability/casting)
  return p;
}

static void yasz_init(YASZ *p, uint32_t const srate) {
  p->left = 0.0f;
  p->right = 0.0f;

  for (uint8_t i = 0; i < VOICE_MAX_VOICES; i++)
    p->voice[i] = voice_new(srate);
}

YASZ* yasz_new(uint32_t srate) {
  YASZ* p_yasz = yasz_malloc();
  if (p_yasz == NULL)
    return NULL;
  yasz_init(p_yasz, srate);
  return p_yasz;
}

void yazs_mixer_rt(YASZ* p) {
  double left = 0.0f;
  double right = 0.0f;
  for (uint8_t i = 0; i < VOICE_MAX_VOICES; i++) {
    voice_render_rt(p->voice[i]);
    left += p->voice[i]->left;
    right += p->voice[i]->right;
  }
  p->left = left;
  p->right = right;
}

void yasz_render_rt(YASZ *p) {
  yazs_mixer_rt(p);
}

VOICE *yasz_get_free_voice(YASZ *p) {
  for (uint8_t i = 0; i < VOICE_MAX_VOICES; i++)
    if (voice_is_free(p->voice[i]))
      return p->voice[i];

  return NULL;
}

VOICE *yasz_get_on_voice(YASZ *p, uint8_t note) {
  for (uint8_t i = 0; i < VOICE_MAX_VOICES; i++)
    if (p->voice[i]->midi->midinote == note)
      return p->voice[i];

  return NULL;
}

void yasz_proc_midi(YASZ *p_yasz,
    const uint8_t size,
    const uint8_t* const data) {
  if (size > 3)
    return;

  VOICE *p_free_voice = yasz_get_free_voice(p_yasz);
  const uint8_t status = midi_get_status_rt(data);
  const uint8_t note = midi_get_note_rt(data);
  const uint8_t velo = midi_get_note_vel_rt(data);
  VOICE *p_voice_on = yasz_get_on_voice(p_yasz, note);

  switch (status) {
  case MIDI_NOTE_ON:
    if (p_voice_on && velo == 0) {
        p_voice_on->midi->notestate = NOTE_OFF;
        p_voice_on->midi->midinote = NOTE_OFF;
        adsr_gate_off_rt(p_voice_on->adsr);
    }

    if (velo > 0 && p_free_voice) {
      p_free_voice->midi->notestate = NOTE_ON;
      p_free_voice->midi->midinote = note;
      double freq = midi_to_freq[note];
      voice_freq_rt(p_free_voice, freq);
      adsr_gate_on_rt(p_free_voice->adsr);
    }
    break;
  case MIDI_NOTE_OFF:
    if (p_voice_on) {
      p_voice_on->midi->notestate = NOTE_OFF;
      p_voice_on->midi->midinote = NOTE_OFF;
      adsr_gate_off_rt(p_voice_on->adsr);
    }
    break;
  }

  return;
}
