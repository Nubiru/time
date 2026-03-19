/* input_dispatch.h — Keyboard input to action mapping.
 *
 * Maps key codes to application actions. The render loop calls
 * id_dispatch(key, shift) to get an action, then applies it to
 * view_state, camera, time speed, etc.
 *
 * Key mappings (from USER_CONTROLS.md):
 *   E        -> Switch to Earth View
 *   Space    -> Toggle pause
 *   1-5      -> Time speed presets
 *   0        -> Pause
 *   Shift+0-6-> Scale jumps
 *   A/K/I/C/D-> Focus modes
 *   T        -> Toggle orbit trails
 *   H        -> Toggle HUD
 *   S        -> Toggle star labels
 *   L        -> Toggle labels
 *   G        -> Toggle grid
 *   Esc      -> Return to overview / cancel
 *   -        -> Reverse time
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_INPUT_DISPATCH_H
#define TIME_INPUT_DISPATCH_H

/* Action types returned by dispatch */
typedef enum {
    ID_ACTION_NONE = 0,
    ID_ACTION_VIEW_SPACE,       /* Switch to Space View */
    ID_ACTION_VIEW_EARTH,       /* Switch to Earth View (E key) */
    ID_ACTION_VIEW_GALAXY,      /* Switch to Galaxy View */
    ID_ACTION_FOCUS_ASTROLOGY,  /* A key */
    ID_ACTION_FOCUS_KIN,        /* K key */
    ID_ACTION_FOCUS_ICHING,     /* I key */
    ID_ACTION_FOCUS_CHINESE,    /* C key */
    ID_ACTION_FOCUS_HD,         /* D key */
    ID_ACTION_FOCUS_OVERVIEW,   /* Esc - return to overview */
    ID_ACTION_TOGGLE_TRAILS,    /* T key */
    ID_ACTION_TOGGLE_HUD,       /* H key */
    ID_ACTION_TOGGLE_STARS,     /* S key */
    ID_ACTION_TOGGLE_LABELS,    /* L key */
    ID_ACTION_TOGGLE_GRID,      /* G key */
    ID_ACTION_TOGGLE_PAUSE,     /* Space */
    ID_ACTION_TIME_REALTIME,    /* 1 key */
    ID_ACTION_TIME_FAST,        /* 2 key */
    ID_ACTION_TIME_FASTER,      /* 3 key */
    ID_ACTION_TIME_DAY_SEC,     /* 4 key */
    ID_ACTION_TIME_10DAY_SEC,   /* 5 key */
    ID_ACTION_TIME_PAUSE,       /* 0 key */
    ID_ACTION_TIME_REVERSE,     /* - key */
    ID_ACTION_SCALE_JUMP,       /* Shift+0-6 */
    ID_ACTION_COUNT
} id_action_t;

/* Dispatch result */
typedef struct {
    id_action_t action;
    int parameter;          /* extra data: scale level for SCALE_JUMP, etc. */
} id_result_t;

/* Dispatch a key press to an action.
 * key: ASCII key code (or special: 27=Esc, 32=Space).
 * shift: 1 if shift is held.
 * Returns action + optional parameter. */
id_result_t id_dispatch(int key, int shift);

/* Get display name for an action. */
const char *id_action_name(id_action_t action);

/* Get the key label for an action (e.g., "E", "Space", "Shift+3"). */
const char *id_key_label(id_action_t action);

/* Is this a focus mode action? */
int id_is_focus_action(id_action_t action);

/* Is this a time control action? */
int id_is_time_action(id_action_t action);

/* Is this a toggle action? */
int id_is_toggle_action(id_action_t action);

/* Get time speed multiplier for a time action.
 * Returns: 1 (realtime), 60, 3600, 86400, 864000, 0 (pause), -1 (reverse).
 * Returns 0 for non-time actions. */
int id_time_speed(id_action_t action);

#endif /* TIME_INPUT_DISPATCH_H */
