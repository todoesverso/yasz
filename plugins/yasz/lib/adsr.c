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

#include "adsr.h"

/**************************
 * Static adsr.c functions
 **************************/
ADSR
adsr_new() {
    ADSR adsr = {
        .state = IDLE
    };

    adsr_set_attack_rate_rt(&adsr, 0);
    adsr_set_decay_rate_rt(&adsr, 0);
    adsr_set_release_rate_rt(&adsr, 0);
    adsr_set_sustain_level_rt(&adsr, 1.0);
    adsr_set_target_ratio_a_rt(&adsr, 0.3);
    adsr_set_target_ratio_dr_rt(&adsr, 0.00001);

    return adsr;
}

void
adsr_set_attack_rate_rt(ADSR* p, double rate) {
    p->arate = rate;
    p->acoef = adsr_calc_coef_rt(rate, p->targetratioa);
    p->abase = (1.0 + p->targetratioa) * (1.0 - p->acoef);
}

void
adsr_set_decay_rate_rt(ADSR* p, double rate) {
    p->drate = rate;
    p->dcoef = adsr_calc_coef_rt(rate, p->targetratiodr);
    p->dbase = (p->slevel - p->targetratiodr) * (1.0 - p->dcoef);
}

void
adsr_set_release_rate_rt(ADSR* p, double rate) {
    p->rrate = rate;
    p->rcoef = adsr_calc_coef_rt(rate, p->targetratiodr);
    p->rbase = -p->targetratiodr * (1.0 - p->rcoef);
}

void
adsr_set_sustain_level_rt(ADSR* p, double level) {
    p->slevel = level;
    p->dbase = (p->slevel - p->targetratiodr) * (1.0 - p->dcoef);
}

void
adsr_set_target_ratio_a_rt(ADSR* p, double ratio) {
    ratio = min_ratio_db_rt(ratio);
    p->targetratioa = ratio;
    p->acoef = adsr_calc_coef_rt(p->arate, p->targetratioa);
    p->abase = (1.0 + p->targetratioa) * (1.0 - p->acoef);
}

void
adsr_set_target_ratio_dr_rt(ADSR* p, double ratio) {
    ratio = min_ratio_db_rt(ratio);
    p->targetratiodr = ratio;
    p->dcoef = adsr_calc_coef_rt(p->drate, p->targetratiodr);
    p->rcoef = adsr_calc_coef_rt(p->rrate, p->targetratiodr);
    p->dbase = (p->slevel - p->targetratiodr) * (1.0 - p->dcoef);
    p->rbase = -p->targetratiodr * (1.0 - p->rcoef);
}
