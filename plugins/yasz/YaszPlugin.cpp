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

#include "DistrhoPlugin.hpp"
extern "C" {
#include "lib/yasz.h"
}
START_NAMESPACE_DISTRHO

/**
  Plugin to show how to get some basic information sent to the UI.
 */
class YaszPlugin : public Plugin {
 public:
    YaszPlugin()
        : Plugin(kParameterCount, 0, 0) {
          uint32_t srate = (uint32_t)getSampleRate();
          yasz = yasz_new(srate);
        }

 protected:
   /* --------------------------------------------------------------------------------------------------------
    * Information */

   /**
      Get the plugin label.
      This label is a short restricted name consisting of only _, a-z, A-Z and 0-9 characters.
    */
    const char* getLabel() const override {
        return DISTRHO_PLUGIN_NAME;
    }

   /**
      Get an extensive comment/description about the plugin.
    */
    const char* getDescription() const override {
        return "Plugin to load and use YASZ";
    }

   /**
      Get the plugin author/maker.
    */
    const char* getMaker() const override {
        return DISTRHO_PLUGIN_BRAND;
    }

   /**
      Get the plugin homepage.
    */
    const char* getHomePage() const override {
        return DISTRHO_PLUGIN_URI;
    }

   /**
      Get the plugin license name (a single line of text).
      For commercial plugins this should return some short copyright information.
    */
    const char* getLicense() const override {
        return "GPL-3";
    }

   /**
      Get the plugin version, in hexadecimal.
    */
    uint32_t getVersion() const override {
        return d_version(0, 0, 1);
    }

   /**
      Get the plugin unique Id.
      This value is used by LADSPA, DSSI and VST plugin formats.
    */
    int64_t getUniqueId() const override {
        return d_cconst('Y', 'A', 'S', 'Z');
    }

   /* --------------------------------------------------------------------------------------------------------
    * Init */

   /**
      Initialize the parameter @a index.
      This function will be called once, shortly after the plugin is created.
    */
    void initParameter(uint32_t , Parameter&) override {}
   /* --------------------------------------------------------------------------------------------------------
    * Internal data */

   /**
      Get the current value of a parameter.
      The host may call this function from any context, including realtime processing.
    */
    float getParameterValue(uint32_t) const override {}

   /**
      Change a parameter value.
      The host may call this function from any context, including realtime processing.
      When a parameter is marked as automable, you must ensure no non-realtime operations are performed.
      @note This function will only be called for parameter inputs.
    */
    void setParameterValue(uint32_t, float) override {
        // this is only called for input parameters, which we have none of.
    }

   /* --------------------------------------------------------------------------------------------------------
    * Audio/MIDI Processing */

    void activate() {
    }

   /**
      Run/process function for plugins without MIDI input.
      @note Some parameters might be null if there are no audio inputs or outputs.
    */
    void run(const float**, float** outputs, uint32_t frames,
        const MidiEvent* midiEvents, uint32_t midiEventCount) override {
          float* const outL = outputs[0];
          float* const outR = outputs[1];

          uint32_t fDone = 0;
          uint32_t curEventIndex = 0;
          uint32_t fToProcess = 0;

          while (fDone < frames) {
            /* process any ready midi event */
            /* start midi process */
            while (curEventIndex < midiEventCount &&
                fDone == midiEvents[curEventIndex].frame) {
              if (midiEvents[curEventIndex].size > MidiEvent::kDataSize)
                continue;

              yasz_proc_midi(yasz,
                  midiEvents[curEventIndex].size,
                  midiEvents[curEventIndex].data);
              curEventIndex++;
            }
            /* stop midi process */

            if (curEventIndex < midiEventCount &&
                midiEvents[curEventIndex].frame < frames)
              fToProcess = midiEvents[curEventIndex].frame - fDone;
            else
              fToProcess = frames - fDone;

            for (uint32_t i = fDone; i < fDone + fToProcess; ++i) {
              yasz_render_rt(yasz);
              outL[i] = 0.5f * static_cast<float>(yasz->left);
              outR[i] = 0.5f * static_cast<float>(yasz->right);
            }

            fDone += fToProcess;
          }
    }

   /* --------------------------------------------------------------------------------------------------------
    * Callbacks (optional) */
    // -------------------------------------------------------------------------------------------------------

 private:
    // Parameters
    YASZ *yasz;

   /**
      Set our plugin class as non-copyable and add a leak detector just in case.
    */
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(YaszPlugin)
};

/* ------------------------------------------------------------------------------------------------------------
 * Plugin entry point, called by DPF to create a new plugin instance. */

Plugin* createPlugin() {
    return new YaszPlugin();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
