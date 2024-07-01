/*
 * YASS (Yet Another Simple Synthesizer)
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

#ifndef PLUGINS_YASZ_LIB_UTILS_H_
#define PLUGINS_YASZ_LIB_UTILS_H_

#include <math.h>
#include <stdint.h>

#ifndef M_PI
#define M_PI (3.141592653589793)
#endif

#ifndef TWO_PI
#define TWO_PI (6.283185307179586)
#endif

#ifndef PI_O_2
#define PI_O_2 (1.5707963267948966)
#endif

#ifndef SQRT2_O_2
#define SQRT2_O_2 (0.7071067811865476)
#endif

#ifndef MIN
#define MIN(n1, n2) ((n1) > (n2) ? (n2) : (n1))
#endif

#ifndef MAX
#define MAX(n1, n2) ((n1) > (n2) ? (n1) : (n2))
#endif
#endif // PLUGINS_YASZ_LIB_UTILS_H_
