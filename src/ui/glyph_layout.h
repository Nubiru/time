#ifndef TIME_GLYPH_LAYOUT_H
#define TIME_GLYPH_LAYOUT_H

typedef struct {
    float x, y;          /* top-left corner */
    float width, height; /* bounding box size */
} glyph_rect_t;

typedef enum {
    ALIGN_LEFT   = 0,
    ALIGN_CENTER = 1,
    ALIGN_RIGHT  = 2
} glyph_align_t;

typedef struct {
    float x, y;           /* origin position */
    float char_width;     /* width per character (monospace) */
    float char_height;    /* height per character */
    float line_spacing;   /* vertical gap between lines */
    float padding;        /* padding inside bounding box */
    glyph_align_t align;  /* horizontal alignment */
} glyph_style_t;

/* Compute bounding rect for a string of N characters. */
glyph_rect_t glyph_text_rect(glyph_style_t style, int num_chars);

/* Compute bounding rect for a multi-line block. */
glyph_rect_t glyph_block_rect(glyph_style_t style, int num_lines, int max_chars);

/* Compute x position of character at index within a line, respecting alignment. */
float glyph_char_x(glyph_style_t style, int char_index, int line_length);

/* Compute y position of line at index. */
float glyph_line_y(glyph_style_t style, int line_index);

/* Position a label centered above a point. */
glyph_rect_t glyph_label_above(float px, float py, float offset_y,
                                glyph_style_t style, int num_chars);

/* Position a label centered below a point. */
glyph_rect_t glyph_label_below(float px, float py, float offset_y,
                                glyph_style_t style, int num_chars);

/* Check if two rects overlap (strict, touching does not count). */
int glyph_rects_overlap(glyph_rect_t a, glyph_rect_t b);

/* Clamp a rect to stay within screen bounds (0,0)-(screen_w,screen_h). */
glyph_rect_t glyph_clamp_to_screen(glyph_rect_t r, float screen_w, float screen_h);

/* Default style for HUD text. */
glyph_style_t glyph_style_hud(void);

/* Default style for card text. */
glyph_style_t glyph_style_card(void);

#endif
