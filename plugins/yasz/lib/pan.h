/*
 * YASZ (Yet Another Simple synthesiZer)
 * Copyright (C) 2021 Victor Rosales <todoesverso@gmail.com>
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


/** @file midi.h */
#ifndef PLUGINS_YASZ_LIB_PAN_H_
#define PLUGINS_YASZ_LIB_PAN_H_
#include <stdint.h>
#include <math.h>
#include "utils.h"

/**
 * @brief YASZ Panning data representation
 *
 * Very simple and functional panning representation
 **/
typedef struct pan_t {
    double left;   /**< Left gain */
    double right;  /**< Right gain */
} PANPOS;

/* Extracted from The Audio Programming Book */
inline PANPOS
constantpower_rt(double possition) {
    PANPOS pos;

    double thispos = possition * PI_O_2;
    double angle = thispos * 0.5;

    pos.left = SQRT2_O_2 * (cos(angle) - sin(angle));
    pos.right = SQRT2_O_2 * (cos(angle) + sin(angle));

    return pos;
}

#endif  // PLUGINS_YASZ_LIB_PAN_H_
