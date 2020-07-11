/*
 * YASS (Yet Another Simple Synthesizer)
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


#ifndef PLUGINS_YASZ_LIB_OSC_H_
#define PLUGINS_YASZ_LIB_OSC_H_

#include <math.h>
#include <stdint.h>
#ifndef M_PI
#define M_PI (3.141592653589793)
#endif
#ifndef TWO_PI
#define TWO_PI (6.283185307179586)
#endif


typedef struct t_osc {
  double freq;
  double phase;
  double phaseinc;
  double twopioversr;
  uint32_t srate;
} OSC;

OSC* osc_new(uint32_t srate);
double osc_get_out_rt(OSC* p_osc);
void osc_update_freq(OSC* p_osc, double newfreq);
void osc_update_freq_from_midi_note_rt(OSC* p_osc, uint8_t note);
void osc_update_phase(OSC *p_osc, double phase);
void osc_update_srate(OSC *p_osc, uint32_t srate);

#endif  // PLUGINS_YASZ_LIB_OSC_H_
