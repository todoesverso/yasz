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

#include "osct.h"
#include "common.h"
#include "wave.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>

/************************
 * Static osct.c functions
 ************************/
static double osct_calc_new_phase_rt(OSCT *p) {
  return p->tlenoversr * p->freq;
}

static inline double interpolate(OSCT *p) {
  double integral;
  double frac = modf(p->phase, &integral);
  uint16_t bindex = (uint16_t)integral;
  uint16_t nindex = bindex + 1;
  return fma(frac, p->table[nindex] - p->table[bindex], p->table[bindex]);
}

static double osct_render_rt(OSCT *p) {
  double val = interpolate(p);
  p->phase += p->phaseinc;
  while (p->phase >= p->tlen) {
    p->phase -= p->tlen;
  }
  while (p->phase < 0.0) {
    p->phase += p->tlen;
  }

  return val;
}

/*************************
 * Public osct.c functions
 *************************/
OSCT osct_new(uint32_t srate) {
  lookup_init_rt();
  OSCT osc = {
      .tlen = TLEN,
      .srate = srate,
      .tlenoversr = (double)TLEN / srate,
      .ttable = yasz_saw_t,
      .table = yasz_saw_t[0],
  };

  return osc;
}

void osct_freq_rt(OSCT *p, double newfreq) {
  p->freq = newfreq;
  uint16_t harmonic = MAX_FREQ / p->freq - 1.0;
  if (harmonic >= HARMONICS) {
    harmonic = HARMONICS - 1;
  }
  p->harmonics = harmonic;
  if (p->wavetype == YASZ_SINE_T) {
    harmonic = 0;
  }
  p->table = p->ttable[harmonic];
  p->phaseinc = osct_calc_new_phase_rt(p);
}

void osct_phase_rt(OSCT *p, double phase) { p->phase = phase; }

void osct_srate_rt(OSCT *p, uint32_t srate) {
  p->srate = srate;
  p->tlenoversr = (double)(p->tlen / srate); // NOLINT(readability/casting)
}

double osct_get_out_rt(OSCT *p) { return osct_render_rt(p); }

void osct_wavetype_rt(OSCT *p, uint8_t wavetype) {

  p->wavetype = wavetype;
  switch (p->wavetype) {
  case (YASZ_SQUARE_T):
    p->ttable = yasz_square_t;
    break;
  case (YASZ_SAW_T):
    p->ttable = yasz_saw_t;
    break;
  case (YASZ_TRIANGLE_T):
    p->ttable = yasz_triangle_t;
    break;
  default: // (YASZ_SINE_T)
    p->ttable = yasz_sine_t;
    break;
  }
}

void osct_wavetype_sine_rt(OSCT *p) { osct_wavetype_rt(p, YASZ_SINE_T); }

void osct_wavetype_square_rt(OSCT *p) { osct_wavetype_rt(p, YASZ_SQUARE_T); }

void osct_wavetype_triangle_rt(OSCT *p) {
  osct_wavetype_rt(p, YASZ_TRIANGLE_T);
}

void osct_wavetype_saw_rt(OSCT *p) { osct_wavetype_rt(p, YASZ_SAW_T); }
