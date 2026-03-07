/*
 * input.h — Emscripten input event handlers
 *
 * Extracted from main.c. All handlers mutate g_state via pointer.
 * Stateful module (S1): touches app_state_t, registers callbacks.
 */

#ifndef TIME_INPUT_H
#define TIME_INPUT_H

#ifdef __EMSCRIPTEN__
#include "app_state.h"

/* Register all input callbacks (mouse, keyboard, wheel).
 * Must be called after GL init and state creation. */
void input_register(app_state_t *state);

#endif
#endif /* TIME_INPUT_H */
