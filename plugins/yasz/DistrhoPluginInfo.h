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

/*
see https://distrho.github.io/DPF/group__PluginMacros.html
*/

#ifndef PLUGINS_YASZ_DISTRHOPLUGININFO_H_
#define PLUGINS_YASZ_DISTRHOPLUGININFO_H_

#define DISTRHO_PLUGIN_BRAND "todoesverso"
#define DISTRHO_PLUGIN_NAME "YASZ"
#define DISTRHO_PLUGIN_URI "https://github.com/todoesverso/yasz"

#define DISTRHO_PLUGIN_HAS_UI 1
#define DISTRHO_PLUGIN_IS_RT_SAFE 1
#define DISTRHO_PLUGIN_NUM_INPUTS 0
#define DISTRHO_PLUGIN_NUM_OUTPUTS 2
#define DISTRHO_PLUGIN_IS_SYNTH 1
#define DISTRHO_UI_USER_RESIZABLE 1
#define DISTRHO_UI_USE_NANOVG 1

#ifdef __MOD_DEVICES__
#define DISTRHO_PLUGIN_USES_MODGUI 1
#endif

enum Parameters { paramPan = 0, kParameterCount };

#endif // PLUGINS_YASZ_DISTRHOPLUGININFO_H_
