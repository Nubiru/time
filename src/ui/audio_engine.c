/* audio_engine.c -- WebAudio bridge for planetary sonification.
 *
 * STATEFUL module (S1): uses EM_ASM, static state, Emscripten APIs.
 * Consumes audio_params_t from audio_score.h and drives WebAudio
 * oscillators in the browser.
 *
 * L0.4: PeriodicWave from harmonic partials per planet
 * L0.5: ConvolverNode reverb with procedural impulse response
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

        /* Master gain */
        var master = ctx.createGain();
        master.gain.value = 0.3;
        master.connect(ctx.destination);

        /* Dry/wet routing for reverb */
        var dryGain = ctx.createGain();
        dryGain.gain.value = 0.7;
        dryGain.connect(master);

        var wetGain = ctx.createGain();
        wetGain.gain.value = 0.3;
        wetGain.connect(master);

        /* Procedural impulse response for convolution reverb */
        var irLen = Math.floor(ctx.sampleRate * 2.0); /* 2 second tail */
        var irBuf = ctx.createBuffer(2, irLen, ctx.sampleRate);
        for (var ch = 0; ch < 2; ch++) {
            var data = irBuf.getChannelData(ch);
            for (var s = 0; s < irLen; s++) {
                /* Exponential decay with random noise */
                var t = s / ctx.sampleRate;
                data[s] = (Math.random() * 2 - 1) * Math.exp(-3.0 * t);
            }
        }

        var convolver = ctx.createConvolver();
        convolver.buffer = irBuf;
        convolver.connect(wetGain);

        /* Create 9 oscillators with per-planet gains and stereo panners */
        var oscs = [];
        var gains = [];
        var panners = [];
        var waveTypes = []; /* track current waveform to avoid redundant updates */
        for (var i = 0; i < 9; i++) {
            var osc = ctx.createOscillator();
            var gain = ctx.createGain();
            var panner = ctx.createStereoPanner();
            osc.type = 'sine';
            osc.frequency.value = 0;
            gain.gain.value = 0;
            panner.pan.value = 0;
            osc.connect(gain);
            gain.connect(panner);
            /* Route each panner output to both dry and wet paths */
            panner.connect(dryGain);
            panner.connect(convolver);
            osc.start();
            oscs.push(osc);
            gains.push(gain);
            panners.push(panner);
            waveTypes.push('sine');
        }

        var ta = {};
        ta.ctx = ctx;
        ta.master = master;
        ta.dryGain = dryGain;
        ta.wetGain = wetGain;
        ta.convolver = convolver;
        ta.oscs = oscs;
        ta.gains = gains;
        ta.panners = panners;
        ta.waveTypes = waveTypes;
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

    /* Update each active planet oscillator */
    for (int i = 0; i < AS_MAX_PLANETS && i < params->planet_count; i++) {
        float freq = params->frequencies[i];
        float amp = params->amplitudes[i] * params->master_volume;
        int waveform = params->waveform_types[i];
        int hcount = params->harmonic_counts[i];
        float pan = params->pan_positions[i];

        /* Pass harmonic amplitudes for PeriodicWave creation */
        float h0 = (hcount > 0) ? params->harmonic_amps[i][0] : 0.0f;
        float h1 = (hcount > 1) ? params->harmonic_amps[i][1] : 0.0f;
        float h2 = (hcount > 2) ? params->harmonic_amps[i][2] : 0.0f;
        float h3 = (hcount > 3) ? params->harmonic_amps[i][3] : 0.0f;
        float h4 = (hcount > 4) ? params->harmonic_amps[i][4] : 0.0f;
        float h5 = (hcount > 5) ? params->harmonic_amps[i][5] : 0.0f;
        float h6 = (hcount > 6) ? params->harmonic_amps[i][6] : 0.0f;
        float h7 = (hcount > 7) ? params->harmonic_amps[i][7] : 0.0f;

        EM_ASM({
            var ta = window._timeAudio;
            if (!ta) return;
            var idx = $0;
            var now = ta.ctx.currentTime;
            var hcount = $3;
            var waveform = $4;

            /* Ramp frequency and amplitude */
            ta.oscs[idx].frequency.linearRampToValueAtTime($1, now + 0.1);
            ta.gains[idx].gain.linearRampToValueAtTime($2, now + 0.1);

            /* Ramp stereo pan position (L1.2) */
            ta.panners[idx].pan.linearRampToValueAtTime($13, now + 0.1);

            /* Set waveform: use PeriodicWave for rich harmonics, preset for simple */
            if (hcount > 2) {
                /* Build PeriodicWave from harmonic amplitudes */
                var real = new Float32Array(hcount + 1);
                var imag = new Float32Array(hcount + 1);
                real[0] = 0; /* DC offset = 0 */
                real[1] = $5;  /* fundamental */
                if (hcount > 1) real[2] = $6;
                if (hcount > 2) real[3] = $7;
                if (hcount > 3) real[4] = $8;
                if (hcount > 4) real[5] = $9;
                if (hcount > 5) real[6] = $10;
                if (hcount > 6) real[7] = $11;
                if (hcount > 7) real[8] = $12;
                var wave = ta.ctx.createPeriodicWave(real, imag,
                    {disableNormalization: false});
                ta.oscs[idx].setPeriodicWave(wave);
                ta.waveTypes[idx] = 'periodic';
            } else {
                /* Use preset waveform type */
                var types = ['sine', 'triangle', 'sawtooth'];
                var newType = types[waveform] || 'sine';
                if (ta.waveTypes[idx] !== newType) {
                    ta.oscs[idx].type = newType;
                    ta.waveTypes[idx] = newType;
                }
            }
        }, i, (double)freq, (double)amp, hcount, waveform,
           (double)h0, (double)h1, (double)h2, (double)h3,
           (double)h4, (double)h5, (double)h6, (double)h7,
           (double)pan);
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

    /* Update reverb wet/dry mix */
    {
        float wet = params->reverb_wet;
        float dry = 1.0f - wet;
        EM_ASM({
            var ta = window._timeAudio;
            if (!ta) return;
            var now = ta.ctx.currentTime;
            ta.dryGain.gain.linearRampToValueAtTime($0, now + 0.5);
            ta.wetGain.gain.linearRampToValueAtTime($1, now + 0.5);
        }, (double)dry, (double)wet);
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
        /* Resume audio context in case browser suspended it */
        if (ta.ctx.state === 'suspended') ta.ctx.resume();
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
            ta.panners[i].disconnect();
        }
        ta.dryGain.disconnect();
        ta.wetGain.disconnect();
        ta.convolver.disconnect();
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
