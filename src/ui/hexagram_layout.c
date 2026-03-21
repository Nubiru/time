/* hexagram_layout.c — I Ching hexagram display layout
 *
 * Computes visual layout for a hexagram in normalized 0-1 coordinates.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "hexagram_layout.h"
#include "../systems/iching/iching.h"
#include "../systems/iching/iching_interpret.h"
#include <string.h>

/* Lines block geometry (normalized 0-1) */
#define LINES_X       0.25f
#define LINES_W       0.50f
#define LINE_H        0.05f
#define LINE_GAP      0.03f
#define LINE_STEP     (LINE_H + LINE_GAP)  /* 0.08 */
/* Bottom line (index 0) top-left y */
#define LINES_BOT_Y   0.60f
/* Top line (index 5) top-left y = 0.60 - 5*0.08 = 0.20 */
#define LINES_TOP_Y   (LINES_BOT_Y - 5.0f * LINE_STEP)
/* Total block height: from top line top to bottom line bottom */
/* = LINES_BOT_Y + LINE_H - LINES_TOP_Y = 0.65 - 0.20 = 0.45 */
#define LINES_TOTAL_H (LINES_BOT_Y + LINE_H - LINES_TOP_Y)

/* Text label positions */
#define NAME_X        0.80f
#define NAME_Y        0.25f
#define NUMBER_X      0.80f
#define NUMBER_Y      0.18f
#define UPPER_LABEL_X 0.80f
#define UPPER_LABEL_Y 0.35f
#define LOWER_LABEL_X 0.80f
#define LOWER_LABEL_Y 0.52f
#define JUDGMENT_X    0.10f
#define JUDGMENT_Y    0.72f

hexagram_layout_t hexagram_layout_compute(int king_wen)
{
    hexagram_layout_t layout;
    memset(&layout, 0, sizeof(layout));

    if (king_wen < 1 || king_wen > 64) {
        return layout;
    }

    /* Fetch hexagram data */
    hexagram_t hex = iching_hexagram(king_wen);
    ii_hexagram_t data = ii_hexagram_data(king_wen);

    /* Identity */
    layout.king_wen = king_wen;
    layout.name     = data.name;
    layout.judgment  = data.judgment;
    layout.image     = data.image;
    layout.keywords  = data.keywords;

    /* Trigrams */
    layout.upper_trigram = hex.upper_trigram;
    layout.lower_trigram = hex.lower_trigram;
    layout.upper_name   = iching_trigram_name(hex.upper_trigram);
    layout.lower_name   = iching_trigram_name(hex.lower_trigram);
    layout.upper_symbol = iching_trigram_symbol(hex.upper_trigram);
    layout.lower_symbol = iching_trigram_symbol(hex.lower_trigram);

    /* Compute 6 line positions — bottom (index 0) to top (index 5).
     * In screen space, index 0 is at the lowest y (largest y value)
     * and index 5 is at the highest y (smallest y value). */
    for (int i = 0; i < 6; i++) {
        layout.lines[i].x          = LINES_X;
        layout.lines[i].w          = LINES_W;
        layout.lines[i].h          = LINE_H;
        layout.lines[i].is_yang    = hex.lines[i];
        layout.lines[i].line_index = i;
        /* Bottom line (i=0) at LINES_BOT_Y, each subsequent line steps up */
        layout.lines[i].y = LINES_BOT_Y - (float)i * LINE_STEP;
    }

    /* Text label positions */
    layout.name_x        = NAME_X;
    layout.name_y        = NAME_Y;
    layout.number_x      = NUMBER_X;
    layout.number_y      = NUMBER_Y;
    layout.upper_label_x = UPPER_LABEL_X;
    layout.upper_label_y = UPPER_LABEL_Y;
    layout.lower_label_x = LOWER_LABEL_X;
    layout.lower_label_y = LOWER_LABEL_Y;
    layout.judgment_x    = JUDGMENT_X;
    layout.judgment_y    = JUDGMENT_Y;

    /* Overall lines block bounds */
    layout.lines_x = LINES_X;
    layout.lines_y = LINES_TOP_Y;
    layout.lines_w = LINES_W;
    layout.lines_h = LINES_TOTAL_H;

    return layout;
}

hex_line_t hexagram_layout_line(const hexagram_layout_t *layout, int index)
{
    hex_line_t line;
    memset(&line, 0, sizeof(line));
    line.line_index = -1;

    if (!layout || index < 0 || index > 5) {
        return line;
    }

    return layout->lines[index];
}

const char *hexagram_layout_trigram_name(const hexagram_layout_t *layout,
                                         int upper)
{
    if (!layout) {
        return "?";
    }
    return upper ? layout->upper_name : layout->lower_name;
}
