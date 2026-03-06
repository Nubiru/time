/* bodygraph.h — Human Design bodygraph geometry: centers, channels, gates.
 * Pure data module. No GL, no malloc, no globals, no side effects. */

#ifndef TIME_BODYGRAPH_H
#define TIME_BODYGRAPH_H

#define BODYGRAPH_CENTER_COUNT  9
#define BODYGRAPH_CHANNEL_COUNT 36
#define BODYGRAPH_GATE_COUNT    64

/* 2D position */
typedef struct {
    float x, y;
} bg_point_t;

/* Center identity */
typedef enum {
    BG_HEAD = 0,
    BG_AJNA,
    BG_THROAT,
    BG_G_CENTER,
    BG_HEART,
    BG_SPLEEN,
    BG_SACRAL,
    BG_SOLAR_PLEXUS,
    BG_ROOT,
    BG_CENTER_COUNT
} bg_center_id_t;

/* Center data */
typedef struct {
    bg_center_id_t id;
    const char *name;
    bg_point_t position;     /* normalized 0-1 space */
    int is_motor;            /* 1 if motor center (Heart, Sacral, Root, Solar Plexus) */
    int is_awareness;        /* 1 if awareness center (Ajna, Spleen, Solar Plexus) */
    int is_pressure;         /* 1 if pressure center (Head, Root) */
} bg_center_t;

/* Channel data (connects two centers) */
typedef struct {
    int gate_a;              /* gate number on center_a side */
    int gate_b;              /* gate number on center_b side */
    bg_center_id_t center_a; /* first center */
    bg_center_id_t center_b; /* second center */
} bg_channel_t;

/* Gate position on the bodygraph */
typedef struct {
    int gate_number;         /* 1-64 */
    bg_center_id_t center;   /* which center this gate belongs to */
    bg_point_t position;     /* position along channel line */
} bg_gate_pos_t;

/* Get center data by ID. */
bg_center_t bodygraph_center(bg_center_id_t id);

/* Get center name string. */
const char *bodygraph_center_name(bg_center_id_t id);

/* Get center position. */
bg_point_t bodygraph_center_position(bg_center_id_t id);

/* Get channel by index (0-35). */
bg_channel_t bodygraph_channel(int index);

/* Get total channel count. */
int bodygraph_channel_count(void);

/* Find which center a gate belongs to. Returns BG_CENTER_COUNT if invalid. */
bg_center_id_t bodygraph_gate_center(int gate_number);

/* Get gate position on bodygraph. */
bg_gate_pos_t bodygraph_gate_position(int gate_number);

/* Check if a channel is defined (activated) given two gate numbers.
 * Returns channel index (0-35) or -1 if not a valid channel. */
int bodygraph_channel_for_gates(int gate_a, int gate_b);

/* Given an array of activated gates (1=active, 0=inactive, length 65
 * using indices 1-64), determine which centers are defined (have at
 * least one complete channel).
 * defined_centers: output array of BODYGRAPH_CENTER_COUNT booleans. */
void bodygraph_defined_centers(const int *active_gates, int *defined_centers);

/* Count activated channels given active gates array. */
int bodygraph_active_channel_count(const int *active_gates);

#endif /* TIME_BODYGRAPH_H */
