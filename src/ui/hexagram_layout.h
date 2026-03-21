/* hexagram_layout.h — I Ching hexagram display layout
 *
 * Computes a visual layout for displaying an I Ching hexagram:
 *   - 6 horizontal lines (solid yang or broken yin)
 *   - Upper and lower trigram names and symbols
 *   - Hexagram name, number, judgment, image, keywords
 *   - All positions in normalized 0-1 coordinate space
 *
 * Line convention: index 0 = bottom, index 5 = top (King Wen).
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_HEXAGRAM_LAYOUT_H
#define TIME_HEXAGRAM_LAYOUT_H

/* Single line in hexagram display */
typedef struct {
    float x, y;           /* left edge of line, normalized 0-1 */
    float w, h;           /* width and height of line */
    int is_yang;          /* 1=solid (yang), 0=broken (yin) */
    int line_index;       /* 0=bottom, 5=top (King Wen convention) */
} hex_line_t;

/* Full hexagram display layout */
typedef struct {
    /* The 6 lines — bottom to top (index 0 at bottom) */
    hex_line_t lines[6];

    /* Hexagram identity */
    int king_wen;                /* 1-64 */
    const char *name;            /* e.g., "The Creative" */
    const char *judgment;        /* brief judgment text */
    const char *image;           /* image text */
    const char *keywords;        /* comma-separated keywords */

    /* Trigrams */
    int upper_trigram;           /* 0-7 */
    int lower_trigram;           /* 0-7 */
    const char *upper_name;      /* e.g., "Heaven" */
    const char *lower_name;      /* e.g., "Heaven" */
    const char *upper_symbol;    /* Unicode trigram symbol */
    const char *lower_symbol;    /* Unicode trigram symbol */

    /* Layout positions for text labels */
    float name_x, name_y;       /* hexagram name position */
    float number_x, number_y;   /* hexagram number position */
    float upper_label_x, upper_label_y;  /* upper trigram label */
    float lower_label_x, lower_label_y;  /* lower trigram label */
    float judgment_x, judgment_y;         /* judgment text position */

    /* Overall bounds */
    float lines_x, lines_y;     /* top-left of lines block */
    float lines_w, lines_h;     /* total width/height of lines block */
} hexagram_layout_t;

/* Compute the hexagram display layout by King Wen number (1-64).
 * Returns zeroed struct with king_wen=0 for invalid input. */
hexagram_layout_t hexagram_layout_compute(int king_wen);

/* Get a single line from the layout by index (0=bottom, 5=top).
 * Returns zeroed hex_line_t with line_index=-1 for invalid. */
hex_line_t hexagram_layout_line(const hexagram_layout_t *layout, int index);

/* Get trigram name from the layout.
 * upper=1 for upper trigram, upper=0 for lower trigram.
 * Returns "?" for NULL layout. */
const char *hexagram_layout_trigram_name(const hexagram_layout_t *layout,
                                         int upper);

#endif /* TIME_HEXAGRAM_LAYOUT_H */
