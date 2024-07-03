/*
 * YASS (Yet Another Simple Synthesizer)
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

#include "osc.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/************************
 * Static osc.c functions
 ************************/
static OSC *osc_malloc() {
  OSC *p_osc;
  p_osc = (OSC *)malloc(sizeof(OSC)); // NOLINT(readability/casting)
  return p_osc;
}

static double osc_calc_new_phase_rt(OSC *p_osc) {
  return p_osc->twopioversr * p_osc->freq;
}

/* WAVE types */
static double osc_sine_rt(OSC *p_osc) {
  double val = sin(p_osc->phase);
  UPDATE_PHASE(p_osc);
  return val;
}

static double osc_square_rt(OSC *p_osc) {
  double val = (p_osc->phase <= M_PI) ? 1 : 0;
  UPDATE_PHASE(p_osc);
  return val;
}

static double osc_triangle_rt(OSC *p_osc) {
  double val = (2.0 * (p_osc->phase * (M_1_TWO_PI))) - 1.0;
  if (val < 0.0) {
    val = -val;
  }
  val = 2.0 * (val - 0.5);
  UPDATE_PHASE(p_osc);
  return val;
}

static double osc_saw_down_rt(OSC *p_osc) {
  double val = 1.0 - 2.0 * (p_osc->phase * (M_1_TWO_PI));
  UPDATE_PHASE(p_osc);
  return val;
}

static double osc_saw_up_rt(OSC *p_osc) {
  double val = (2.0 * (p_osc->phase * (M_1_TWO_PI))) - 1.0;
  UPDATE_PHASE(p_osc);
  return val;
}

/*************************
 * Public osc.c functions
 *************************/
void osc_init_rt(OSC *p_osc, uint32_t srate) {
  p_osc->freq = 0.0f;
  p_osc->phase = 0.0f;
  p_osc->phaseinc = 0.0f;
  p_osc->srate = srate;
  p_osc->twopioversr = (double)TWO_PI / srate; // NOLINT(readability/casting)
}

OSC *osc_new(uint32_t srate) {
  OSC *p_osc = osc_malloc();
  if (p_osc == NULL) {
    return NULL;
  }
  osc_init_rt(p_osc, srate);
  return p_osc;
}

void osc_update_freq_rt(OSC *p_osc, double newfreq) {
  p_osc->freq = newfreq;
  p_osc->phaseinc = osc_calc_new_phase_rt(p_osc);
}

void osc_update_phase_rt(OSC *p_osc, double phase) { p_osc->phase = phase; }

void osc_update_wavetype_rt(OSC *p_osc, uint8_t wavetype) {
  p_osc->wavetype = wavetype;
}

void osc_update_srate_rt(OSC *p_osc, uint32_t srate) {
  p_osc->srate = srate;
  p_osc->twopioversr = (double)TWO_PI / srate; // NOLINT(readability/casting)
}

double osc_get_out_rt(OSC *p_osc) {
  double val;

  switch (p_osc->wavetype) {
  case (YASZ_SQUARE):
    val = osc_square_rt(p_osc);
    break;
  case (YASZ_SAW_UP):
    val = osc_saw_up_rt(p_osc);
    break;
  case (YASZ_SAW_DOWN):
    val = osc_saw_down_rt(p_osc);
    break;
  case (YASZ_TRIANGLE):
    val = osc_triangle_rt(p_osc);
    break;
  default:
    val = osc_sine_rt(p_osc);
    break;
  }

  return val;
}
