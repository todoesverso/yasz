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


/** @file midi.h */
#ifndef PLUGINS_YASZ_LIB_MIDI_H_
#define PLUGINS_YASZ_LIB_MIDI_H_
#include <stdint.h>

#define MIDI_NOTE_OFF         0x80
#define MIDI_NOTE_ON          0x90
#define MIDI_NOTE_PRESSURE    0xa0
#define MIDI_CONTROL          0xb0
#define MIDI_PGM_CHANGE       0xc0
#define MIDI_CHANNEL_PRESSURE 0xd0
#define MIDI_BENDER           0xe0

#define MIDI_NOTES            127
enum midi_note_status {
    NOTE_OFF, /**< Midi note off status */
    NOTE_ON   /**< Midi note on status */
};

enum midi_status_index_from_event {
    MIDI_STATUS_BYTE, /**< Midi's status byte */
    MIDI_NOTE_BYTE,   /**< Midi's note byte */
    MIDI_VEL_BYTE     /**< Midi's velocity byte */
};

/**
 * @brief YASZ Midi data representation
 *
 * Very simple and functional  midi representation
 **/
typedef struct midi_t {
    uint8_t notestate; /**< The state of the midi note (NOTE_OFF|NOTE_ON)*/
    uint8_t midinote;  /**< The actual midi note */
} MIDI;


// MSB, discard channel
#define midi_get_status_rt(x)   (x[MIDI_STATUS_BYTE] & 0xF0)
#define midi_get_note_rt(x)     (x[MIDI_NOTE_BYTE])
#define midi_get_note_vel_rt(x) (x[MIDI_VEL_BYTE])
#define midi_to_freq_rt(x,b)    (b*powf(2.0f, ((x)-69.f)*0.083333333))

MIDI* midi_new();
void midi_init_rt(MIDI* p);

extern const double midi_to_freq[MIDI_NOTES + 1];
#endif  // PLUGINS_YASZ_LIB_MIDI_H_
