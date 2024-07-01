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

#include "freeverb.h"
#include "allpass.h"
#include "comb.h"
#include <stdint.h>
#include <stdio.h>

void freeverb_update(FREEVERB *p) {
  uint8_t i = 0;

  p->wet1 = p->wet * (p->width * 0.5f + 0.5f);
  p->wet2 = p->wet * ((1 - p->width) * 0.5f);

  p->roomsize1 = p->roomsize;
  p->damp1 = p->damp;
  p->gain = FV_FIXEDGAIN;

  for (i = 0; i < FV_NUMCOMBS; i++) {
    comb_feedback(&p->combL[i], p->roomsize1);
    comb_feedback(&p->combR[i], p->roomsize1);
    comb_setdamp(&p->combL[i], p->damp1);
    comb_setdamp(&p->combR[i], p->damp1);
  }
}

void freeverb_setwet(FREEVERB *p, float wet) {
  p->wet = wet * FV_SCALEWET;
  freeverb_update(p);
}

void freeverb_setdry(FREEVERB *p, float dry) { p->dry = dry * FV_SCALEDRY; }

void freeverb_setwidth(FREEVERB *p, float width) {
  p->width = width;
  freeverb_update(p);
}

void freeverb_setroomsize(FREEVERB *p, float roomsize) {
  p->roomsize = (roomsize * FV_SCALEROOM) + FV_OFFSETROOM;
  freeverb_update(p);
}

void freeverb_setdamp(FREEVERB *p, float damp) {
  p->damp = damp * FV_SCALEDAMP;
  freeverb_update(p);
}

void freeverb_setsr(FREEVERB *p, uint32_t sr) {
  const uint8_t combs[] = {1116, 1188, 1277, 1356, 1422, 1491, 1557, 1617};
  const uint8_t allpasses[] = {556, 441, 341, 225};

  double multiplier = (double)sr / FV_INITIALSR;

  /* init comb buffers */
  for (int i = 0; i < FV_NUMCOMBS; i++) {
    p->combL[i].buffsize = combs[i] * multiplier;
    p->combR[i].buffsize = (combs[i] + FV_STEREOSPREAD) * multiplier;
  }

  /* init allpass buffers */
  for (int i = 0; i < FV_NUMALLPASSES; i++) {
    p->allpassL[i].buffsize = allpasses[i] * multiplier;
    p->allpassR[i].buffsize = (allpasses[i] + FV_STEREOSPREAD) * multiplier;
  }
}

FREEVERB
freeverb_new(uint32_t samplerate) {
  FREEVERB freeverb = {0};

  for (int i = 0; i < FV_NUMALLPASSES; i++) {
    freeverb.allpassL[i].feedback = 0.5f;
    freeverb.allpassR[i].feedback = 0.5f;
  }

  freeverb_setsr(&freeverb, samplerate);
  freeverb_setwet(&freeverb, FV_INITIALWET);
  freeverb_setdamp(&freeverb, FV_INITIALDAMP);
  freeverb_setdry(&freeverb, FV_INITIALDRY);
  freeverb_setroomsize(&freeverb, FV_INITIALROOM);
  freeverb_setwidth(&freeverb, FV_INITIALWIDTH);

  return freeverb;
}

void freeverb_tick(FREEVERB *p, double inputL, double inputR) {
  double outL = 0.0f;
  double outR = 0.0f;
  uint8_t i = 0;

  double input = (inputL + inputR) * p->gain;

  // parallel combs
  for (i = 0; i < FV_NUMCOMBS; i++) {
    outL += comb_tick(&p->combL[i], input);
    outR += comb_tick(&p->combR[i], input);
  }
  // serial allpass
  for (i = 0; i < FV_NUMALLPASSES; i++) {
    outL = allpass_tick(&p->allpassL[i], outL);
    outR = allpass_tick(&p->allpassR[i], outR);
  }
  p->outL = outL * p->wet1 + outR * p->wet2 + inputL * p->dry;
  p->outR = outR * p->wet1 + outL * p->wet2 + inputR * p->dry;
}
