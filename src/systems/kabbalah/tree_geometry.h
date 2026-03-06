/* tree_geometry.h — Kabbalah Tree of Life: 22 paths + 231 gates
 *
 * Pure geometry module providing the 22 paths with Hebrew letters,
 * Tarot Major Arcana, and zodiac/planet/element correspondences.
 * Generates renderable line geometry from Sefirot positions.
 * Also provides the 231 Gates (Sefer Yetzirah two-letter combinations).
 *
 * All functions are pure: no globals, no malloc, no side effects.
 *
 * Sources: Sefer Yetzirah (letter classification),
 *          Golden Dawn / Crowley 777 correspondence tables,
 *          Kircher Tree (path layout). */

#ifndef TIME_TREE_GEOMETRY_H
#define TIME_TREE_GEOMETRY_H

#define TREE_PATH_COUNT 22
#define TREE_GATE_COUNT 231  /* C(22,2) = 231 two-letter combinations */

/* Path types (Hebrew letter classification) */
#define TREE_LETTER_MOTHER  0  /* 3 mother letters: Aleph, Mem, Shin */
#define TREE_LETTER_DOUBLE  1  /* 7 double letters: Bet..Tav */
#define TREE_LETTER_SIMPLE  2  /* 12 simple letters */

/* A path connecting two Sefirot */
typedef struct {
    int id;                    /* 0-21 */
    int from_sefirah;          /* index 0-9 into sefirot */
    int to_sefirah;            /* index 0-9 into sefirot */
    const char *hebrew_letter; /* Hebrew letter name (e.g., "Aleph") */
    const char *hebrew_char;   /* Hebrew character (UTF-8, e.g., "\xD7\x90") */
    int letter_value;          /* Numerical value (1, 2, 3, ..., 400) */
    int letter_type;           /* MOTHER(0), DOUBLE(1), SIMPLE(2) */
    const char *tarot;         /* Major Arcana name (e.g., "The Fool") */
    int tarot_number;          /* Major Arcana number (0-21) */
    const char *attribution;   /* Zodiac sign, planet, or element name */
    int zodiac_sign;           /* zodiac index 0-11, or -1 if planet/element */
    int planet_index;          /* planet index (0=Merc..5=Sat, 8=Sun, 9=Moon), or -1 */
} tree_path_t;

/* Line segment for rendering */
typedef struct {
    float x0, y0;   /* start point (from sefirah position) */
    float x1, y1;   /* end point (to sefirah position) */
} tree_line_t;

/* Full renderable tree layout */
typedef struct {
    tree_line_t lines[TREE_PATH_COUNT];
    float node_x[11];   /* 10 Sefirot + Da'at x positions */
    float node_y[11];   /* 10 Sefirot + Da'at y positions */
    int line_count;
    int node_count;
} tree_layout_t;

/* A Sefer Yetzirah Gate (two-letter combination) */
typedef struct {
    int letter_a;    /* index 0-21 into path array */
    int letter_b;    /* index 0-21 into path array */
} tree_gate_t;

/* Get path by index (0-21). Returns zeroed struct for invalid. */
tree_path_t tree_path_get(int index);

/* Get path for a zodiac sign (0-11). Returns zeroed struct if not found. */
tree_path_t tree_path_for_zodiac(int sign);

/* Get path for a planet (0-9). Returns zeroed struct if not found. */
tree_path_t tree_path_for_planet(int planet);

/* Get path for a Tarot Major Arcana number (0-21). Returns zeroed struct if not found. */
tree_path_t tree_path_for_tarot(int tarot_number);

/* Get path count (always 22). */
int tree_path_count(void);

/* Generate renderable Tree layout at given width and height.
 * Reads Sefirot positions and scales to fit. */
tree_layout_t tree_layout_generate(float width, float height);

/* Get a Gate by index (0-230). Returns zeroed struct for invalid. */
tree_gate_t tree_gate_get(int index);

/* Gate count (always 231). */
int tree_gate_count(void);

/* Count paths of each type. */
int tree_count_mother_paths(void);
int tree_count_double_paths(void);
int tree_count_simple_paths(void);

#endif /* TIME_TREE_GEOMETRY_H */
