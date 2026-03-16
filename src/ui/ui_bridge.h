#ifndef TIME_UI_BRIDGE_H
#define TIME_UI_BRIDGE_H

/* ui_bridge.h -- C-to-DOM bridge for UI panels.
 *
 * STATEFUL module (S1): uses EM_ASM, static state, Emscripten APIs.
 * Wires pure UI data modules (help_overlay, command_palette, layer_panel,
 * ui_html, time_control, toast_message, ui_state) into the browser DOM.
 *
 * Behind #ifdef __EMSCRIPTEN__ — native builds get no-op stubs. */

/* Initialize UI panels with data from pure modules.
 * Populates help panel, layer panel, command palette via DOM injection.
 * Call once after GL init and input registration. */
void ui_bridge_init(void);

/* Update time bar display each frame.
 * jd: current Julian Day. speed: days per real second. */
void ui_bridge_update_time(double jd, double speed);

#endif /* TIME_UI_BRIDGE_H */
