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


/// @file osc.h
#ifndef PLUGINS_YASZ_LIB_OSC_H_
#define PLUGINS_YASZ_LIB_OSC_H_

#include <math.h>
#include <stdint.h>
#ifndef M_PI
#define M_PI (3.141592653589793)
#endif
#ifndef TWO_PI
#define TWO_PI (6.283185307179586)
#endif

/**
 * @brief YASZ Oscilator Structure
 *
 * This is a basic oscilator structure. This same file
 * provides several functions to interact with this structure.
 */
typedef struct t_osc {
  /*@{*/
  double freq;        /**< frequency of the oscilator in Hz */
  double phase;       /**< phase of the oscilator in radians */
  double phaseinc;    /**< phase incrmenets */
  double twopioversr; /**< constant (2*M_PI)/samplerate */
  uint32_t srate;     /**< sample rate in Hz */
  /*@}*/
} OSC;


/**
 * Creats a new YASZ oscilator
 * @param[in] srate The sample rate in Hz
 * @return t_osc An allocated YASZ oscilator or NULL if no memory
 */
OSC* osc_new(uint32_t srate);

/**
 * Modifies the frequency of a t_osc struct
 * @param[in] p_osc Pointer to a YASZ oscilator
 * @param[in] newfreq The new frequency of the oscilator
 */
void osc_update_freq_rt(OSC* p_osc, double newfreq);

/**
 * Modifies the phase of a t_osc struct
 * @param[in] p_osc Pointer to a YASZ oscilator
 * @param[in] phase The new phase of the oscilator
 */
void osc_update_phase_rt(OSC *p_osc, double phase);

/**
 * Modifies the sample rate of a t_osc struct
 * @param[in] p_osc Pointer to a YASZ oscilator
 * @param[in] srate The new sample rate of the oscilator
 */
void osc_update_srate_rt(OSC *p_osc, uint32_t srate);

double osc_get_out_rt(OSC* p_osc);
#endif  // PLUGINS_YASZ_LIB_OSC_H_
