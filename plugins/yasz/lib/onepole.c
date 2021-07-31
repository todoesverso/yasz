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

#include "lib/onepole.h"

static ONEPOLE* onepole_malloc();
static void onepole_init(ONEPOLE* p_op, double freq);

static ONEPOLE*
onepole_malloc() {
    ONEPOLE* p;
    p = (ONEPOLE*) malloc(sizeof(ONEPOLE));  // NOLINT(readability/casting)
    return p;
}

static void
onepole_init(ONEPOLE* p, double freq) {
    p->a0 = 0.0f;
    p->b1 = 0.0f;
    p->z1 = 0.0f;
    onepole_set_freq_rt(p, freq);
}

ONEPOLE*
onepole_new(double freq) {
    ONEPOLE* p = onepole_malloc();
    if (p == NULL) {
        return NULL;
    }
    onepole_init(p, freq);
    return p;
}
