/*
 * hud.h — Time HUD overlay (DOM-based)
 *
 * Extracted from main.c. Pushes formatted time data to HTML DOM.
 * Stateful module (S1): calls EM_ASM to update DOM elements.
 */

#ifndef TIME_HUD_H
#define TIME_HUD_H

#ifdef __EMSCRIPTEN__

/* Update the HUD overlay with current simulation state.
 * Computes Gregorian date, sidereal time, zodiac, aspects, etc.
 * Pushes HTML string to #time-hud element via EM_ASM. */
void hud_update(double jd, double time_speed,
                double obs_lat, double obs_lon,
                float log_zoom);

#endif
#endif /* TIME_HUD_H */
