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

#include "ks.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>

KS ks_new(uint32_t const srate) {
  KS ks = {
      .srate = srate,
      .size = 1,
  };

  return ks;
}

double ks_render_rt(KS *p) {
  p->sample = p->table[p->index];
  if (p->index == 0) {
    p->sample += p->table[p->size - 1];
  } else {
    p->sample += p->table[p->index - 1];
  }

  p->sample = p->sample * 0.5 * 0.994;
  p->table[p->index] = p->sample;
  p->index++;
  p->index %= p->size;

  return p->sample;
}

void ks_freq_rt(KS *p, double freq) {
  p->size = 1 + (uint32_t)p->srate / (freq * 0.5f); // 1 octave up!?
  p->index = 0;
  // TODO: Review this rand() slow
  for (uint32_t i = 0; i < p->size; i++) {
    p->table[i] = ((double)rand() / (double)RAND_MAX) * 2.0f - 1.0f;
  }
}
