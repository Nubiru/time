/* tarot_visual.h — Visual data for the 22 Major Arcana (Thoth deck)
 *
 * Pure data module providing per-card color schemes (4 color scales),
 * symbolic descriptions, geometric motifs, and Tree of Life path
 * positions for the 22 Major Arcana.
 *
 * Color scales follow the Golden Dawn / Crowley 777 system:
 *   King (Atziluth)   — bright, pure, primary
 *   Queen (Briah)     — rich, deep
 *   Prince (Yetzirah) — muted, complex
 *   Princess (Assiah) — dark, earthen
 *
 * All colors derived from color_theory.h mood system.
 * All proportions derived from golden_layout.h phi constants.
 * No hardcoded RGB values. No hardcoded pixel values.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects.
 *
 * Sources: Aleister Crowley (Book of Thoth, 777),
 *          Lady Frieda Harris (Thoth deck artist). */

#ifndef TIME_TAROT_VISUAL_H
#define TIME_TAROT_VISUAL_H

#include "../math/color.h"

#define TV_MAJOR_COUNT 22
#define TV_COLOR_SCALES 4

/* Color scale types (Golden Dawn / Crowley 777) */
typedef enum {
    TV_SCALE_KING = 0,    /* Atziluth (archetypal world) — bright/pure */
    TV_SCALE_QUEEN,       /* Briah (creative world) — rich/deep */
    TV_SCALE_PRINCE,      /* Yetzirah (formative world) — muted/complex */
    TV_SCALE_PRINCESS,    /* Assiah (material world) — dark/earthen */
    TV_SCALE_COUNT
} tv_scale_t;

/* Geometric motif type for card symbols */
typedef enum {
    TV_MOTIF_CIRCLE = 0,      /* unity, wholeness, spirit */
    TV_MOTIF_TRIANGLE,        /* trinity, fire, ascent */
    TV_MOTIF_SQUARE,          /* stability, earth, matter */
    TV_MOTIF_HEXAGRAM,        /* as above so below */
    TV_MOTIF_PENTAGRAM,       /* five elements, human form */
    TV_MOTIF_SPIRAL,          /* evolution, growth, phi */
    TV_MOTIF_CROSS,           /* four directions, balance */
    TV_MOTIF_LEMNISCATE,      /* infinity, eternal return */
    TV_MOTIF_CRESCENT,        /* moon, receptivity, change */
    TV_MOTIF_LIGHTNING,       /* sudden force, revelation */
    TV_MOTIF_RAYS,            /* radiation, glory, sun */
    TV_MOTIF_WAVE,            /* flow, water, emotion */
    TV_MOTIF_COLUMN,          /* pillar, structure, path */
    TV_MOTIF_EYE,             /* awareness, watchfulness */
    TV_MOTIF_ANKH,            /* life, eternity */
    TV_MOTIF_COUNT
} tv_motif_t;

/* Visual data for one Major Arcanum */
typedef struct {
    int number;                       /* 0-21 (Fool=0, World=21) */
    const char *name;                 /* "The Fool", "The Magician", etc. */
    const char *thoth_name;           /* Crowley's title (e.g., "The Magus") */
    color_rgb_t colors[TV_COLOR_SCALES]; /* King/Queen/Prince/Princess scale colors */
    tv_motif_t primary_motif;         /* dominant geometric symbol */
    tv_motif_t secondary_motif;       /* supporting geometric symbol */
    const char *symbol_desc;          /* brief symbolic description */
    int tree_path_index;              /* index into tree_geometry paths (0-21) */
    const char *attribution;          /* zodiac/planet/element attribution */
} tv_major_t;

/* Card frame geometry (golden ratio proportions) */
typedef struct {
    float width;          /* card width */
    float height;         /* card height = width * phi */
    float border;         /* border thickness */
    float symbol_radius;  /* central symbol circle radius */
    float title_y;        /* title text Y position */
    float number_y;       /* number display Y position */
} tv_card_frame_t;

/* Get visual data for a Major Arcanum by number (0-21).
 * Returns struct with number=-1 for invalid. */
tv_major_t tv_major_get(int number);

/* Get Major Arcanum count (always 22). */
int tv_major_count(void);

/* Get color for a card in a specific scale.
 * Returns black for invalid inputs. */
color_rgb_t tv_card_color(int number, tv_scale_t scale);

/* Get the primary color for a card (King scale). */
color_rgb_t tv_card_primary(int number);

/* Get geometric motif name as string. */
const char *tv_motif_name(tv_motif_t motif);

/* Get color scale name as string. */
const char *tv_scale_name(tv_scale_t scale);

/* Compute card frame geometry for a given width.
 * Uses golden ratio for all proportions. */
tv_card_frame_t tv_card_frame(float width);

/* Get the Tree of Life path position for a card.
 * Returns normalized (x, y) coordinates on the Tree.
 * The position is the midpoint of the path connecting two Sefirot.
 * Writes to out_x and out_y. Returns 1 on success, 0 on invalid. */
int tv_tree_position(int number, float *out_x, float *out_y);

/* Get all 22 Major Arcana tree positions for rendering.
 * Writes up to max_out (x, y) pairs.
 * Returns count written. */
int tv_tree_positions_all(float *out_x, float *out_y, int max_out);

#endif /* TIME_TAROT_VISUAL_H */
