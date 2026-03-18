/* audio_engine.c -- WebAudio bridge for planetary sonification.
 *
 * STATEFUL module (S1): uses EM_ASM, static state, Emscripten APIs.
 * Consumes audio_params_t from audio_score.h and drives WebAudio
 * oscillators in the browser.
 *
 * Behind #ifdef __EMSCRIPTEN__ — native builds get no-op stubs. */

#include "audio_engine.h"

#ifdef __EMSCRIPTEN__

#include <emscripten.h>

/* ------------------------------------------------------------------ */
/* Module-static state (S1)                                            */
/* ------------------------------------------------------------------ */

static int s_muted = 0;
static int s_initialized = 0;
static int s_frame_counter = 0;

/* ------------------------------------------------------------------ */
/* audio_engine_init                                                    */
/* ------------------------------------------------------------------ */

void audio_engine_init(void) {
    if (s_initialized) return;
    EM_ASM({
        if (window._timeAudio) return;
        var ctx = new (window.AudioContext || window.webkitAudioContext)();
        var master = ctx.createGain();
        master.gain.value = 0.3;
        master.connect(ctx.destination);

        var oscs = [];
        var gains = [];
        for (var i = 0; i < 9; i++) {
            var osc = ctx.createOscillator();
            var gain = ctx.createGain();
            osc.type = 'sine';
            osc.frequency.value = 0;
            gain.gain.value = 0;
            osc.connect(gain);
            gain.connect(master);
            osc.start();
            oscs.push(osc);
            gains.push(gain);
        }

        var ta = {};
        ta.ctx = ctx;
        ta.master = master;
        ta.oscs = oscs;
        ta.gains = gains;
        ta.prevVol = 0.3;
        window._timeAudio = ta;
    });
    s_initialized = 1;
    s_muted = 0;
}

/* ------------------------------------------------------------------ */
/* audio_engine_update                                                  */
/* ------------------------------------------------------------------ */

void audio_engine_update(const audio_params_t *params) {
    if (!s_initialized || s_muted) return;

    /* Only update every 10 frames to avoid excessive JS calls */
    s_frame_counter++;
    if (s_frame_counter < 10) return;
    s_frame_counter = 0;

    if (!params) return;

    /* Pass frequencies and amplitudes to JS for active planets */
    for (int i = 0; i < AS_MAX_PLANETS && i < params->planet_count; i++) {
        float freq = params->frequencies[i];
        float amp = params->amplitudes[i] * params->master_volume;
        EM_ASM({
            var ta = window._timeAudio;
            if (!ta) return;
            var idx = $0;
            var now = ta.ctx.currentTime;
            ta.oscs[idx].frequency.linearRampToValueAtTime($1, now + 0.1);
            ta.gains[idx].gain.linearRampToValueAtTime($2, now + 0.1);
        }, i, (double)freq, (double)amp);
    }

    /* Silence unused oscillators */
    for (int i = params->planet_count; i < AS_MAX_PLANETS; i++) {
        EM_ASM({
            var ta = window._timeAudio;
            if (!ta) return;
            var now = ta.ctx.currentTime;
            ta.gains[$0].gain.linearRampToValueAtTime(0, now + 0.1);
        }, i);
    }
}

/* ------------------------------------------------------------------ */
/* audio_engine_set_volume                                              */
/* ------------------------------------------------------------------ */

void audio_engine_set_volume(float volume) {
    if (!s_initialized) return;
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;
    EM_ASM({
        var ta = window._timeAudio;
        if (!ta) return;
        var now = ta.ctx.currentTime;
        ta.master.gain.linearRampToValueAtTime($0, now + 0.05);
        ta.prevVol = $0;
    }, (double)volume);
}

/* ------------------------------------------------------------------ */
/* audio_engine_mute                                                    */
/* ------------------------------------------------------------------ */

void audio_engine_mute(void) {
    if (!s_initialized) return;
    s_muted = 1;
    EM_ASM({
        var ta = window._timeAudio;
        if (!ta) return;
        var now = ta.ctx.currentTime;
        ta.master.gain.linearRampToValueAtTime(0, now + 0.05);
    });
}

/* ------------------------------------------------------------------ */
/* audio_engine_unmute                                                  */
/* ------------------------------------------------------------------ */

void audio_engine_unmute(void) {
    if (!s_initialized) return;
    s_muted = 0;
    EM_ASM({
        var ta = window._timeAudio;
        if (!ta) return;
        var now = ta.ctx.currentTime;
        ta.master.gain.linearRampToValueAtTime(ta.prevVol, now + 0.05);
    });
}

/* ------------------------------------------------------------------ */
/* audio_engine_is_muted                                                */
/* ------------------------------------------------------------------ */

int audio_engine_is_muted(void) {
    return s_muted;
}

/* ------------------------------------------------------------------ */
/* audio_engine_destroy                                                 */
/* ------------------------------------------------------------------ */

void audio_engine_destroy(void) {
    if (!s_initialized) return;
    EM_ASM({
        var ta = window._timeAudio;
        if (!ta) return;
        for (var i = 0; i < ta.oscs.length; i++) {
            ta.oscs[i].stop();
            ta.oscs[i].disconnect();
            ta.gains[i].disconnect();
        }
        ta.master.disconnect();
        ta.ctx.close();
        window._timeAudio = null;
    });
    s_initialized = 0;
    s_muted = 0;
}

#else
/* ------------------------------------------------------------------ */
/* Native stubs — no-op when not targeting Emscripten                  */
/* ------------------------------------------------------------------ */

void audio_engine_init(void) { }
void audio_engine_update(const audio_params_t *params) { (void)params; }
void audio_engine_set_volume(float volume) { (void)volume; }
void audio_engine_mute(void) { }
void audio_engine_unmute(void) { }
int audio_engine_is_muted(void) { return 0; }
void audio_engine_destroy(void) { }

#endif /* __EMSCRIPTEN__ */
