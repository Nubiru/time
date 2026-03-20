/* iching_today.c — I Ching "Today" page implementation
 *
 * Composes a complete daily I Ching reading from hexagram data, trigram
 * breakdown, interpretation text, and ASCII line visualization.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "iching_today.h"
#include "iching.h"
#include "iching_interpret.h"
#include <stdio.h>
#include <string.h>

/* ===== Line art constants ===== */

/* Yang (solid): 9 heavy box-drawing characters */
#define YANG_LINE "\xe2\x94\x81\xe2\x94\x81\xe2\x94\x81\xe2\x94\x81" \
                  "\xe2\x94\x81\xe2\x94\x81\xe2\x94\x81\xe2\x94\x81" \
                  "\xe2\x94\x81"

/* Yin (broken): 3 heavy + space + 3 heavy */
#define YIN_LINE  "\xe2\x94\x81\xe2\x94\x81\xe2\x94\x81 " \
                  "\xe2\x94\x81\xe2\x94\x81\xe2\x94\x81"

/* ===== Internal helpers ===== */

static void build_page_title(ic_page_t *page, int king_wen,
                              const char *name)
{
    snprintf(page->page_title, IC_TITLE_MAX,
             "I Ching \xe2\x80\x94 Hexagram %d \xc2\xb7 %s",
             king_wen, name);
}

static void build_hexagram_section(ic_page_t *page, int king_wen,
                                    const ii_hexagram_t *data)
{
    snprintf(page->hexagram_section, IC_SECTION_MAX,
             "Hexagram: %d \xe2\x80\x94 %s\n"
             "Element: %s\n"
             "Keywords: %s",
             king_wen, data->name, data->element, data->keywords);
}

static void build_trigram_section(ic_page_t *page,
                                   int upper_tri, int lower_tri)
{
    const char *upper_sym  = iching_trigram_symbol(upper_tri);
    const char *upper_name = iching_trigram_name(upper_tri);
    const char *lower_sym  = iching_trigram_symbol(lower_tri);
    const char *lower_name = iching_trigram_name(lower_tri);

    snprintf(page->trigram_section, IC_SECTION_MAX,
             "Upper Trigram: %s %s\n"
             "Lower Trigram: %s %s\n"
             "Reading: %s over %s",
             upper_sym, upper_name,
             lower_sym, lower_name,
             upper_name, lower_name);
}

static void build_judgment_section(ic_page_t *page,
                                    const ii_hexagram_t *data)
{
    snprintf(page->judgment_section, IC_SECTION_MAX,
             "The Judgment:\n%s", data->judgment);
}

static void build_image_section(ic_page_t *page,
                                 const ii_hexagram_t *data)
{
    snprintf(page->image_section, IC_SECTION_MAX,
             "The Image:\n%s", data->image);
}

static void build_lines_section(ic_page_t *page, const int lines[6])
{
    int pos = 0;
    pos += snprintf(page->lines_section + pos,
                    IC_SECTION_MAX - (unsigned)pos,
                    "Lines (top to bottom):");

    /* Display from top (line[5]) to bottom (line[0]) */
    for (int i = 5; i >= 0 && pos < IC_SECTION_MAX - 1; i--) {
        const char *art = lines[i] ? YANG_LINE : YIN_LINE;
        pos += snprintf(page->lines_section + pos,
                        IC_SECTION_MAX - (unsigned)pos,
                        "\n  %s", art);
    }
}

/* ===== Public API ===== */

ic_page_t ic_today(double jd)
{
    ic_page_t page;
    memset(&page, 0, sizeof(page));

    hexagram_t hex = iching_from_jd(jd);
    if (hex.king_wen < 1 || hex.king_wen > 64)
        return page;

    ii_hexagram_t data = ii_hexagram_data(hex.king_wen);
    if (data.king_wen < 1)
        return page;

    page.king_wen = hex.king_wen;

    build_page_title(&page, hex.king_wen, data.name);
    build_hexagram_section(&page, hex.king_wen, &data);
    build_trigram_section(&page, hex.upper_trigram, hex.lower_trigram);
    build_judgment_section(&page, &data);
    build_image_section(&page, &data);
    build_lines_section(&page, hex.lines);

    page.section_count = 5;
    page.has_data = 1;

    return page;
}

int ic_format(const ic_page_t *page, char *buf, int buf_size)
{
    if (!page || !buf || buf_size <= 0)
        return 0;

    if (!page->has_data)
        return 0;

    int n = snprintf(buf, (unsigned)buf_size,
                     "%s\n\n"
                     "%s\n\n"
                     "%s\n\n"
                     "%s\n\n"
                     "%s\n\n"
                     "%s",
                     page->page_title,
                     page->hexagram_section,
                     page->trigram_section,
                     page->judgment_section,
                     page->image_section,
                     page->lines_section);

    if (n < 0)
        return 0;

    /* snprintf may return value > buf_size if truncated */
    if (n >= buf_size)
        return buf_size - 1;

    return n;
}
