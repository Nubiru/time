#ifndef TIME_FOCUS_MODE_H
#define TIME_FOCUS_MODE_H

#include "card_layout.h"
#include "../render/render_layers.h"

/* Keyboard-triggered focus modes — each highlights one knowledge system.
 * Pure: no GL, no globals, no I/O. Returns by value. */

typedef enum {
    FOCUS_MODE_OVERVIEW   = 0,  /* All systems visible, no focus */
    FOCUS_MODE_ASTROLOGY  = 1,  /* A key: zoom ecliptic, show astrology card */
    FOCUS_MODE_KIN        = 2,  /* K key: dim 3D, enlarge Kin card */
    FOCUS_MODE_ICHING     = 3,  /* I key: hexagram display */
    FOCUS_MODE_CHINESE    = 4,  /* C key: Chinese calendar card */
    FOCUS_MODE_HD         = 5,  /* D key: Human Design bodygraph */
    FOCUS_MODE_COUNT      = 6
} focus_mode_e;

/* Target visual state for a focus mode */
typedef struct {
    const char *name;                     /* "Astrology", "Kin", etc. */
    int key;                              /* ASCII key code (or 27 for Esc) */
    int card_mask;                        /* which cards to show (bitmask) */
    float zoom_target;                    /* target zoom level */
    float layer_opacities[LAYER_COUNT];   /* per-layer target opacity */
} focus_preset_t;

/* Full focus mode state */
typedef struct {
    focus_mode_e active;    /* current mode */
    focus_mode_e previous;  /* mode before current (for toggle) */
    float transition_t;     /* 0→1 progress of mode transition */
    float transition_dur;   /* duration of mode transition (seconds) */
} focus_modes_t;

/* Initialize: overview mode, no transition. */
focus_modes_t focus_mode_init(void);

/* Switch to a mode. If already in that mode, toggles back to overview. */
focus_modes_t focus_mode_switch(focus_modes_t fm, int mode);

/* Switch based on keypress. Maps A/K/I/C/D/Esc. No-op for other keys. */
focus_modes_t focus_mode_from_key(focus_modes_t fm, int key);

/* Advance transition timer. */
focus_modes_t focus_mode_tick(focus_modes_t fm, float dt);

/* Current active mode. */
focus_mode_e focus_mode_active(focus_modes_t fm);

/* Get the visual preset for a mode. Invalid returns overview. */
focus_preset_t focus_mode_preset(focus_modes_t fm, int mode);

/* Is a mode transition in progress? */
int focus_mode_is_transitioning(focus_modes_t fm);

/* Transition progress (0.0 = just started, 1.0 = complete). */
float focus_mode_transition_progress(focus_modes_t fm);

/* Mode display name. Invalid returns "Overview". */
const char *focus_mode_name(int mode);

#endif
