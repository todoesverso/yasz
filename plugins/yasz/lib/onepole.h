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

#ifndef PLUGINS_YASZ_LIB_ONEPOLE_H_
#define PLUGINS_YASZ_LIB_ONEPOLE_H_
#include "lib/utils.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct t_oenpole {
  double a0;
  double b1;
  double z1;
} ONEPOLE;

inline void onepole_set_freq_rt(ONEPOLE *p, double freq) {
  p->b1 = exp(-TWO_PI * freq);
  p->a0 = 1.0 - p->b1;
}

inline double onepole_process(ONEPOLE *p, double input) {
  return p->z1 = input * p->a0 + p->z1 * p->b1;
}

ONEPOLE *onepole_new(double freq);
#endif // PLUGINS_YASZ_LIB_ONEPOLE_H_
