#ifndef TIME_ANIMATION_H
#define TIME_ANIMATION_H

typedef enum {
    ANIM_IDLE,
    ANIM_RUNNING,
    ANIM_PAUSED,
    ANIM_COMPLETE
} anim_state_e;

typedef enum {
    EASE_TYPE_LINEAR,
    EASE_TYPE_IN_QUAD,
    EASE_TYPE_OUT_QUAD,
    EASE_TYPE_INOUT_QUAD,
    EASE_TYPE_IN_CUBIC,
    EASE_TYPE_OUT_CUBIC,
    EASE_TYPE_INOUT_CUBIC,
    EASE_TYPE_IN_SINE,
    EASE_TYPE_OUT_SINE,
    EASE_TYPE_INOUT_SINE,
    EASE_TYPE_IN_EXPO,
    EASE_TYPE_OUT_EXPO,
    EASE_TYPE_COUNT
} ease_type_e;

/* Renamed from anim_t to avoid conflict with easing.h's anim_t */
typedef struct {
    float from;
    float to;
    float current;
    float duration;
    float elapsed;
    float progress;
    float eased;
    anim_state_e state;
    ease_type_e easing;
    float delay;
    int loop;          /* 0=none, 1=loop, 2=ping-pong */
    int iteration;
} animation_t;

typedef struct {
    animation_t anims[16];
    int count;
    int current_index;
} anim_sequence_t;

/* Create */
animation_t anim_create(float from, float to, float duration, ease_type_e easing);
animation_t anim_create_delayed(float from, float to, float duration,
                                 ease_type_e easing, float delay);

/* State transitions */
animation_t anim_start(animation_t a);
animation_t anim_pause(animation_t a);
animation_t anim_reset(animation_t a);
animation_t anim_restart(animation_t a);

/* Advance time */
animation_t anim_tick(animation_t a, float dt);

/* Query */
int   anim_is_running(animation_t a);
int   anim_is_complete(animation_t a);
float animation_value(animation_t a);

/* Loop mode */
animation_t anim_set_loop(animation_t a, int loop_mode);

/* Apply easing by type */
float anim_apply_easing(ease_type_e type, float t);

/* Sequence */
anim_sequence_t anim_seq_create(void);
anim_sequence_t anim_seq_add(anim_sequence_t seq, animation_t a);
anim_sequence_t anim_seq_tick(anim_sequence_t seq, float dt);
int             anim_seq_is_complete(anim_sequence_t seq);
float           anim_seq_value(anim_sequence_t seq);

#endif
