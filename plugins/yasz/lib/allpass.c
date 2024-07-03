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

#include "allpass.h"
#include <stdint.h>

#define undenormalize(n)                                                       \
  {                                                                            \
    if (xabs(n) < 1e-37) {                                                     \
      (n) = 0;                                                                 \
    }                                                                          \
  }
static inline double xabs(double n) { return n < 0 ? -n : n; }

ALLPASS
allpass_new(uint32_t samplerate, uint32_t buffsize) {
  (void)samplerate;
  ALLPASS allpass = {
      .buffsize = buffsize,
  };

  return allpass;
}

double allpass_out(ALLPASS *p) { return p->buffer[p->buffindex]; }

void allpass_feedback(ALLPASS *p, float feedback) { p->feedback = feedback; }

double allpass_tick(ALLPASS *p, double input) {
  double bufout = p->buffer[p->buffindex];
  undenormalize(bufout);
  double out = -input + bufout;

  p->buffer[p->buffindex] = input + (bufout * p->feedback);

  if (++p->buffindex >= p->buffsize) {
    p->buffindex = 0;
  }

  return out;
}
