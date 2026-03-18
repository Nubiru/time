/* audio_engine.h -- WebAudio bridge for planetary sonification.
 *
 * STATEFUL module (S1): uses EM_ASM, static state, Emscripten APIs.
 * Consumes audio_params_t from audio_score.h and drives WebAudio
 * oscillators in the browser.
 *
 * Behind #ifdef __EMSCRIPTEN__ — native builds get no-op stubs. */

#ifndef TIME_AUDIO_ENGINE_H
#define TIME_AUDIO_ENGINE_H

#include "audio_score.h"

/* Initialize WebAudio context.
 * Must be called after user gesture (e.g., "Enter Time" click).
 * Creates AudioContext, master gain, and 9 planet oscillators. */
void audio_engine_init(void);

/* Update sound output from current score parameters.
 * Call each frame or at reduced rate (every 10 frames).
 * Smoothly ramps frequencies and amplitudes to avoid clicks. */
void audio_engine_update(const audio_params_t *params);

/* Set master volume (0.0 = silent, 1.0 = full). */
void audio_engine_set_volume(float volume);

/* Mute all audio output (remembers previous volume). */
void audio_engine_mute(void);

/* Unmute (restores previous volume). */
void audio_engine_unmute(void);

/* Returns 1 if currently muted, 0 otherwise. */
int audio_engine_is_muted(void);

/* Destroy WebAudio context and release resources. */
void audio_engine_destroy(void);

#endif /* TIME_AUDIO_ENGINE_H */
