/* Time control state — data backend for the bottom-edge time control bar.
 * Pure module: no GL, no malloc, no globals, no side effects.
 * Manages play/pause, speed presets, direction, date display formatting. */

#ifndef TIME_TIME_CONTROL_H
#define TIME_TIME_CONTROL_H

#define TC_MAX_SPEEDS    10
#define TC_DATE_BUF_SIZE 64

/* Time flow direction */
typedef enum {
    TC_DIR_FORWARD =  1,
    TC_DIR_PAUSED  =  0,
    TC_DIR_REVERSE = -1
} tc_direction_t;

/* Speed preset definition */
typedef struct {
    const char *name;       /* Display name: "Real-time", "Day/sec" */
    const char *shortcut;   /* Keyboard shortcut: "1", "4" */
    double multiplier;      /* Speed multiplier: 1.0, 86400.0 */
    int key_index;          /* Which number key (0-5) */
} tc_speed_t;

/* Complete time control state */
typedef struct {
    double jd;              /* Current Julian Day */
    double speed;           /* Current speed multiplier */
    tc_direction_t direction; /* Forward, paused, or reverse */
    int speed_index;        /* Index into speed presets (-1 if custom) */
    int is_live;            /* 1 = tracking real time ("Now" mode) */
    double saved_speed;     /* Speed to restore on unpause */
    tc_direction_t saved_direction; /* Direction to restore on unpause */
} tc_state_t;

/* Formatted time display data */
typedef struct {
    char date[TC_DATE_BUF_SIZE];     /* "2000-01-01" */
    char time[TC_DATE_BUF_SIZE];     /* "12:00:00" */
    char day_name[16];               /* "Saturday" */
    char speed_display[32];          /* "3600x", "PAUSED", "Real-time" */
    char direction_icon[4];          /* ">", "||", "<" */
} tc_display_t;

/* --- Speed Presets --- */

/* Get number of speed presets. */
int tc_speed_count(void);

/* Get a speed preset by index (0-based). */
tc_speed_t tc_speed_get(int index);

/* Find speed preset by key index (0-5). Returns index or -1. */
int tc_speed_find_by_key(int key_index);

/* Get the speed preset name for a multiplier. Returns "Custom" if no match. */
const char *tc_speed_name(double multiplier);

/* --- State Management --- */

/* Create initial time control state (real-time, forward, live). */
tc_state_t tc_state_init(double current_jd);

/* Set speed by preset index. */
tc_state_t tc_set_speed(tc_state_t state, int preset_index);

/* Set speed by key press (0-5). */
tc_state_t tc_set_speed_by_key(tc_state_t state, int key_index);

/* Toggle pause/resume. When resuming, restores previous speed. */
tc_state_t tc_toggle_pause(tc_state_t state);

/* Reverse time direction. */
tc_state_t tc_reverse(tc_state_t state);

/* Jump to "Now" (set live mode, real-time speed, forward). */
tc_state_t tc_go_live(tc_state_t state, double current_jd);

/* Step time by a fixed amount (for arrow key navigation).
 * days: positive = forward, negative = backward. */
tc_state_t tc_step(tc_state_t state, double days);

/* Advance time by dt seconds at current speed. */
tc_state_t tc_advance(tc_state_t state, double dt_seconds);

/* --- Display Formatting --- */

/* Format the current state for display. */
tc_display_t tc_format(tc_state_t state);

/* Format speed as display string.
 * Examples: "PAUSED", "1x", "3600x", "-86400x"
 * Returns number of characters written (excluding null). */
int tc_format_speed(tc_state_t state, char *buf, int buf_size);

/* Format direction as icon string. */
const char *tc_direction_icon(tc_direction_t dir);

/* --- Query --- */

/* Is time currently paused? */
int tc_is_paused(tc_state_t state);

/* Is time running in reverse? */
int tc_is_reverse(tc_state_t state);

/* Is tracking real time? */
int tc_is_live(tc_state_t state);

#endif /* TIME_TIME_CONTROL_H */
