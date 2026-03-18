#ifndef TIME_TOUCH_GESTURES_H
#define TIME_TOUCH_GESTURES_H

/* Touch gesture recognition — pure state machine.
 * No global state, no timers, no callbacks.
 * Caller passes time_ms from external clock (emscripten_get_now or similar).
 * Tracks up to 5 simultaneous touch points by ID. */

#define PI 3.14159265358979323846

/* Gesture recognition thresholds */
#define TG_TAP_MAX_MS        300.0   /* Max duration for a tap (ms) */
#define TG_TAP_MAX_MOVEMENT  0.02f   /* Max normalized movement for tap (~10px on 500px) */
#define TG_DOUBLE_TAP_MAX_MS 400.0   /* Max interval between taps for double tap (ms) */
#define TG_LONG_PRESS_MIN_MS 500.0   /* Min hold time for long press (ms) */
#define TG_SWIPE_MIN_DIST    0.1f    /* Min normalized distance for swipe (~50px on 500px) */
#define TG_SWIPE_MAX_ANGLE   0.5236f /* Max angle from axis for swipe (30 degrees in radians) */
#define TG_PINCH_MIN_CHANGE  0.02f   /* Min distance change ratio for pinch detection */
#define TG_ROTATE_MIN_ANGLE  0.1f    /* Min angle change for rotation (radians) */
#define TG_MAX_TOUCH_POINTS  5       /* Max simultaneous touch points */

/* Recognized gesture types */
typedef enum {
    TG_NONE = 0,
    TG_TAP,                /* Single finger, < 300ms, < 10px movement */
    TG_DOUBLE_TAP,         /* Two taps within 400ms */
    TG_LONG_PRESS,         /* Single finger, > 500ms, < 10px movement */
    TG_SWIPE_LEFT,         /* Single finger, > 50px horizontal, < 30 degree angle */
    TG_SWIPE_RIGHT,
    TG_SWIPE_UP,
    TG_SWIPE_DOWN,
    TG_PINCH_IN,           /* Two fingers, distance decreasing */
    TG_PINCH_OUT,          /* Two fingers, distance increasing */
    TG_TWO_FINGER_ROTATE,  /* Two fingers, angle changing */
    TG_THREE_FINGER_TAP    /* Three fingers, < 300ms */
} tg_gesture_t;

/* Gesture result data */
typedef struct {
    float x, y;            /* Position (normalized 0-1) */
    float dx, dy;          /* Delta since touch start */
    float pinch_scale;     /* 1.0 = no change, > 1 = zoom in */
    float rotate_angle;    /* Radians of rotation */
    float velocity;        /* Normalized units per second of swipe */
} tg_gesture_data_t;

/* Internal touch point tracking */
typedef struct {
    int id;                /* Touch point identifier */
    int active;            /* 1 if this slot is in use */
    float start_x;         /* Start position (normalized) */
    float start_y;
    float current_x;       /* Current/last position */
    float current_y;
    double start_time;     /* Time of touch start (ms) */
    int long_press_fired;  /* 1 if long press already reported */
} tg_touch_point_t;

/* Complete touch gesture state */
typedef struct {
    tg_touch_point_t points[TG_MAX_TOUCH_POINTS];
    int active_count;            /* Number of currently active touch points */
    tg_gesture_t last_gesture;   /* Last recognized gesture */
    double last_tap_time;        /* Time of last tap end (for double-tap detection) */
    float last_tap_x;            /* Position of last tap (for double-tap proximity) */
    float last_tap_y;
    int three_finger_started;    /* Count of fingers that were active together */
    int three_finger_ended;      /* Count of ended fingers from a 3-finger group */
} tg_state_t;

/* Initialize a clean gesture state (no touches, no history) */
tg_state_t tg_init(void);

/* Register a new touch point. Returns new state by value (functional style). */
tg_state_t tg_touch_start(tg_state_t state, int id, float x, float y,
                           double time_ms);

/* Update a touch point position. Returns new state by value. */
tg_state_t tg_touch_move(tg_state_t state, int id, float x, float y,
                          double time_ms);

/* End a touch point. Returns gesture type. Modifies state via pointer
 * (because it returns gesture AND updates state). Writes gesture data to out. */
tg_gesture_t tg_touch_end(tg_state_t *state, int id, float x, float y,
                           double time_ms, tg_gesture_data_t *out);

/* Check if a long press has occurred. Call periodically with current time.
 * Returns TG_LONG_PRESS if threshold met, TG_NONE otherwise. */
tg_gesture_t tg_check_long_press(tg_state_t *state, double time_ms,
                                  tg_gesture_data_t *out);

/* Get human-readable name for a gesture type. Returns static string. */
const char *tg_gesture_name(tg_gesture_t gesture);

#endif /* TIME_TOUCH_GESTURES_H */
