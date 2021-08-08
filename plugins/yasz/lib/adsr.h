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


#ifndef PLUGINS_YASZ_LIB_ADSR_H_
#define PLUGINS_YASZ_LIB_ADSR_H_
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"
#include "osct.h"

enum envstate {
    IDLE,
    ATTACK,
    DECAY,
    SUSTAIN,
    RELEASE
};

typedef struct t_adsr {
    /* attack */
    double arate;
    double acoef;
    double abase;
    /* decay */
    double drate;
    double dcoef;
    double dbase;
    /* sustain */
    double slevel;
    /* release */
    double rrate;
    double rcoef;
    double rbase;

    double targetratioa;
    double targetratiodr;

    uint8_t state;
    double out;
} ADSR;

ADSR adsr_new();
void adsr_set_attack_rate_rt(ADSR* p_adsr, double rate);
void adsr_set_decay_rate_rt(ADSR* p_adsr, double rate);
void adsr_set_release_rate_rt(ADSR* p_adsr, double rate);
void adsr_set_target_ratio_a_rt(ADSR* p_adsr, double rate);
void adsr_set_target_ratio_dr_rt(ADSR* p_adsr, double rate);
void adsr_set_sustain_level_rt(ADSR* p_adsr, double level);

#define DBMIN180 (0.000000001)  // -180dB
#define adsr_calc_coef_rt(r, tr) ((r <= 0) ? 0.0 : exp(-log((1.0 + tr) / tr) / r))  // NOLINT(whitespace/line_length)
#define min_ratio_db_rt(r) ((r < DBMIN180) ? DBMIN180 : r)

inline double
adsr_process_rt(ADSR* p) {
    switch (p->state) {
        case IDLE:
            break;
        case ATTACK:
            p->out = p->abase + p->out * p->acoef;
            if (p->out >= 1.0) {
                p->out = 1.0;
                p->state = DECAY;
            }
            break;
        case DECAY:
            p->out = p->dbase + p->out * p->dcoef;
            if (p->out <= p->slevel) {
                p->out = p->slevel;
                p->state = SUSTAIN;
            }
            break;
        case SUSTAIN:
            break;
        case RELEASE:
            p->out = p->rbase + p->out * p->rcoef;
            if (p->out <= 0.0) {
                p->out = 0.0;
                p->state = IDLE;
            }
            break;
    }
    return p->out;
}

static inline void
adsr_gate_rt(ADSR* p, uint8_t gate) {
    if (gate) {
        p->state = ATTACK;
    } else if (p->state != IDLE) {
        p->state = RELEASE;
    }
}


#define adsr_gate_on_rt(p) (adsr_gate_rt(p, 1))
#define adsr_gate_off_rt(p) (adsr_gate_rt(p, 0))
#endif  // PLUGINS_YASZ_LIB_ADSR_H_
