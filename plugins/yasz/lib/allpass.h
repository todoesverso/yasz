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

#ifndef PLUGINS_YASZ_LIB_ALLPASS_H_
#define PLUGINS_YASZ_LIB_ALLPASS_H_
#include <stdint.h>

typedef struct t_allpass {
  uint32_t samplerate;
  float feedback;
  double buffer[2048];
  uint32_t buffsize;
  uint32_t buffindex;
} ALLPASS;

ALLPASS allpass_new(uint32_t samplerate, uint32_t buffsize);
double allpass_out(ALLPASS *p);
double allpass_tick(ALLPASS *p, double sample);
void allpass_feedback(ALLPASS *p, float feedback);
#endif // PLUGINS_YASZ_LIB_ALLPASS_H_
