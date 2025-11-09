#!/usr/bin/env bash
set -euo pipefail

export LV2_PATH=$(pwd)/bin
MIDI_FILE="${1:-test.mid}"
PLUGIN_URI="https://github.com/todoesverso/yasz"
PLUGIN_NAME="YASZ"
TIMEOUT_SEC=10

echo "🎹 Launching LV2 synth..."
# Launch jalv in background
killall -9 jalv || true
jalv -s "$PLUGIN_URI" &
SYNTH_PID=$!

# Give the plugin a few moments to register ports
sleep 1

echo "⏳ Waiting for plugin audio ports..."
count=0
while true; do
    PLUGIN_OUT_L=$(pw-link -o | grep -i "${PLUGIN_NAME}:lv2_audio_out_1" || true)
    PLUGIN_OUT_R=$(pw-link -o | grep -i "${PLUGIN_NAME}:lv2_audio_out_2" || true)
    if [ -n "$PLUGIN_OUT_L" ] && [ -n "$PLUGIN_OUT_R" ]; then
        echo "✅ Plugin audio ports found: $PLUGIN_OUT_L , $PLUGIN_OUT_R"
        break
    fi
    sleep 0.2
    count=$((count+1))
    if [ $count -ge $((TIMEOUT_SEC*5)) ]; then
        echo "❌ Timeout waiting for plugin audio ports"
        kill $SYNTH_PID
        exit 1
    fi
done

# Connect plugin audio to first system sink ports
SYSTEM_OUT_L=$(pw-link -i | grep -i "Speaker__sink:playback_FL" | head -n1)
SYSTEM_OUT_R=$(pw-link -i | grep -i "Speaker__sink:playback_FR" | head -n1)

pw-link "$PLUGIN_OUT_L" "$SYSTEM_OUT_L"
pw-link "$PLUGIN_OUT_R" "$SYSTEM_OUT_R"

# Detect plugin MIDI input
PLUGIN_IN=$(pw-link -i | grep -i "${PLUGIN_NAME}" | grep "lv2_events_in" | head -n1)
MIDI_OUT=$(pw-link -o | grep -i "Midi-Bridge" | head -n1)
# Connect MIDI output to plugin input
pw-link "$MIDI_OUT" "$PLUGIN_IN"

MIDI_OUT=$(aplaymidi  -l | grep Through | head -n1 | cut -f1 -d ':')
# Play MIDI file using aplaymidi (most compatible)
aplaymidi -p "$MIDI_OUT" "$MIDI_FILE"

# Wait for user to terminate
wait $SYNTH_PID
killall -9 jalv || true
