/*
 * YASS (Yet Another Simple Synthesizer)
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


/** @file osct.h */
#ifndef PLUGINS_YASZ_LIB_OSCT_H_
#define PLUGINS_YASZ_LIB_OSCT_H_

#include <math.h>
#include <stdint.h>
#include "utils.h"
#include "wave.h"


/**
 * @brief YASZ Oscilator Structure
 *
 * This is a basic lookup table oscilator structure. This same file
 * provides several functions to interact with this structure.
 */
typedef struct t_osct {
    double freq;                /**< frequency of the oscilator in Hz */
    double phase;               /**< phase of the oscilator in radians */
    double phaseinc;            /**< phase increments */
    double (*ttable)[TLEN + 1]; /**< ppointer to table lookup */
    double* table;              /**< pointer to table lookup */
    double tlenoversr;          /**< constant (table length)/samplerate */
    uint16_t tlen;              /**< lookup table length */
    uint16_t harmonics;         /**< numbers of harmonics to render */
    uint32_t srate;             /**< sample rate in Hz */
    uint8_t wavetype;           /**< wave type see wave_types */
} OSCT;

/**
 * Creates a new oscilator
 * @param[in] srate The sample rate in Hz
 * @return t_osc An allocated oscilator or NULL if no memory
 */
OSCT* osct_new(uint32_t srate);

/**
 * Resets an oscilator
 * @param[in] p_osc Pointer to an oscilator
 * @param[in] srate The sample rate in Hz
 */
void osct_init_rt(OSCT* p, uint32_t srate);

/**
 * Modifies the frequency of a t_osc struct
 * @param[in] p_osc Pointer to an oscilator
 * @param[in] newfreq The new frequency of the oscilator
 */
void osct_freq_rt(OSCT* p, double newfreq);

/**
 * Modifies the phase of a t_osc struct
 * @param[in] p_osc Pointer to a oscilator
 * @param[in] phase The new phase of the oscilator
 */
void osct_phase_rt(OSCT* p, double phase);

/**
 * Modifies the sample rate of a t_osc struct
 * @param[in] p_osc Pointer to an oscilator
 * @param[in] srate The new sample rate of the oscilator
 */
void osct_srate_rt(OSCT* p, uint32_t srate);

/**
 * Modifies the wavetype to a sine
 * @param[in] p_osc Pointer to an oscilator
 */
void osct_wavetype_sine_rt(OSCT* p);

/**
 * Modifies the wavetype to a square
 * @param[in] p_osc Pointer to an oscilator
 */
void osct_wavetype_square_rt(OSCT* p);

/**
 * Modifies the wavetype to a triangle
 * @param[in] p_osc Pointer to an oscilator
 */
void osct_wavetype_triangle_rt(OSCT* p);

/**
 * Modifies the wavetype to a saw
 * @param[in] p_osc Pointer to an oscilator
 */
void osct_wavetype_saw_rt(OSCT* p);

/**
 * Modifies the wavetype
 * @param[in] p  Pointer to an oscilator
 * @param[in] wt Wavetype
 */
void osct_wavetype_rt(OSCT* p, uint8_t wt);

/**
 * Generates the output of the oscilator and increases the phase
 * accordingly
 *
 * @param[in] p_osc Pointer to an oscilator
 * @return double the current value of the oscilator
 */
double osct_get_out_rt(OSCT* p);
#endif  // PLUGINS_YASZ_LIB_OSCT_H_
