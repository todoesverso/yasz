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


#ifndef PLUGINS_YASZ_LIB_YASZ_H_
#define PLUGINS_YASZ_LIB_YASZ_H_
#include <stdint.h>
#include "lib/utils.h"
#include "lib/voice.h"

typedef struct t_yasz {
  // yasz should have:
  //   - a selected waveform enum
  //   - a global envelop configuration
  //   - an array of voices with adsr

  VOICE *voice[VOICE_MAX_VOICES];
  uint8_t noteState[128];  // this should be the voices?

  /* does this make any sense? */
  double left;
  double right;
} YASZ;


YASZ* yasz_new(uint32_t srate);
void yasz_update_srate(YASZ *p_yasz);
void yasz_render_rt(YASZ *p_yasz);
void yasz_proc_midi(YASZ *p_yasz,
    const uint8_t size,
    const uint8_t* const data);

#endif  // PLUGINS_YASZ_LIB_YASZ_H_
