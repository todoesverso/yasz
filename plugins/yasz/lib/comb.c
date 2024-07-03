/*
 * YASZ (Yet Another Simple synthesiZer)
 * Copyright (C) 2021 Victor Rosales <todoesverso@gmail.com>
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

#include "comb.h"
#include <stdint.h>

#define undenormalize(n)                                                       \
  {                                                                            \
    if (xabs(n) < 1e-37) {                                                     \
      (n) = 0.0f;                                                              \
    }                                                                          \
  }
static inline double xabs(double n) { return n < 0 ? -n : n; }

#define undenormalise(sample)                                                  \
  if (((*(unsigned int *)&sample) & 0x7f800000) == 0)                          \
  sample = 0.0f

COMB comb_new(uint32_t samplerate, uint32_t buffsize) {
  (void)samplerate;
  COMB comb = {
      .buffsize = buffsize,
  };

  return comb;
}

double comb_out(COMB *p) { return p->buffer[p->buffindex]; }

void comb_setdamp(COMB *p, float damp) {
  p->damp1 = damp;
  p->damp2 = 1 - damp;
}

void comb_feedback(COMB *p, float feedback) { p->feedback = feedback; }

double comb_tick(COMB *p, double input) {
  double out = p->buffer[p->buffindex];
  undenormalize(out);
  p->filterstore = (out * p->damp2) + (p->filterstore * p->damp1);
  undenormalize(p->filterstore);
  p->buffer[p->buffindex] = input + (p->filterstore * p->feedback);
  if (++p->buffindex >= p->buffsize) {
    p->buffindex = 0;
  }

  return out;
}
