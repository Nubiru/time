/* kabbalah_today.c — Kabbalah "Today" page implementation
 *
 * Composes a complete daily Kabbalah reading from sefirot data, four worlds,
 * tree paths, and interpretation text.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "kabbalah_today.h"
#include "sefirot.h"
#include "four_worlds.h"
#include "tree_geometry.h"
#include "kabbalah_interpret.h"
#include <stdio.h>
#include <string.h>

/* ===== Planet name lookup ===== */

static const char *PLANET_NAMES[8] = {
    "Mercury", "Venus", "Earth", "Mars",
    "Jupiter", "Saturn", "Uranus", "Neptune"
};

static const char *planet_name_for(int planet_index)
{
    if (planet_index < 0 || planet_index >= 8) {
        return "None";
    }
    return PLANET_NAMES[planet_index];
}

/* ===== Internal helpers ===== */

static void build_page_title(kt_page_t *page, const sefirah_t *sef)
{
    snprintf(page->page_title, KT_TITLE_MAX,
             "Kabbalah \xe2\x80\x94 %s (%s)",
             sef->name, sef->meaning);
}

static void build_sefirah_section(kt_page_t *page, const sefirah_t *sef)
{
    snprintf(page->sefirah_section, KT_SECTION_MAX,
             "Sefirah: %s \xe2\x80\x94 %s\n"
             "Pillar: %s\n"
             "Triad: %s\n"
             "Planet: %s",
             sef->name, sef->meaning,
             sefirot_pillar_name(sef->pillar),
             sefirot_triad_name(sef->triad),
             planet_name_for(sef->planet_index));
}

static void build_world_section(kt_page_t *page, int sefirah_index)
{
    int world_index = four_worlds_for_sefirah(sefirah_index);
    kabbalistic_world_t w = four_worlds_get(world_index);

    snprintf(page->world_section, KT_SECTION_MAX,
             "World: %s \xe2\x80\x94 %s\n"
             "%s",
             w.name, w.meaning, w.description);
}

static void build_path_section(kt_page_t *page, int path_index)
{
    tree_path_t path = tree_path_get(path_index);

    snprintf(page->path_section, KT_SECTION_MAX,
             "Path of the Day: %d\n"
             "From: %s \xe2\x86\x92 To: %s\n"
             "Hebrew Letter: %s (%s) \xe2\x80\x94 Value: %d\n"
             "Tarot: %s (#%d)\n"
             "Attribution: %s",
             path.id + 1,
             sefirot_name(path.from_sefirah),
             sefirot_name(path.to_sefirah),
             path.hebrew_letter, path.hebrew_char, path.letter_value,
             path.tarot, path.tarot_number,
             path.attribution);
}

static void build_meditation_section(kt_page_t *page, int sefirah_index)
{
    ki_sefirah_t ki = ki_sefirah_data(sefirah_index);

    snprintf(page->meditation_section, KT_SECTION_MAX,
             "Meditation: %s\n"
             "Question: %s\n"
             "Essence: %s",
             ki.meditation, ki.question, ki.brief);
}

static void build_tree_section(kt_page_t *page, const sefirah_t *sef,
                                int middle)
{
    int pos = 0;
    pos += snprintf(page->tree_section + pos,
                    KT_SECTION_MAX - (unsigned)pos,
                    "Middle Pillar: %s\n"
                    "Position: (%.2f, %.2f)",
                    middle ? "Yes" : "No",
                    (double)sef->pos_x, (double)sef->pos_y);

    if (middle && pos < KT_SECTION_MAX - 1) {
        snprintf(page->tree_section + pos,
                 KT_SECTION_MAX - (unsigned)pos,
                 "\nThe central axis of balance connects "
                 "Keter through Tiferet and Yesod to Malkuth.");
    }
}

/* ===== Public API ===== */

kt_page_t kt_today(double jd)
{
    kt_page_t page;
    memset(&page, 0, sizeof(page));

    int jd_int = (int)jd;
    int sefirah_index = jd_int % 10;
    int path_index = jd_int % 22;

    if (sefirah_index < 0 || sefirah_index >= 10) {
        return page;
    }

    sefirah_t sef = sefirot_get(sefirah_index);
    int middle = sefirot_is_middle_pillar(sefirah_index) ? 1 : 0;

    page.sefirah_index = sefirah_index;
    page.path_index = path_index;
    page.is_middle_pillar = middle;

    build_page_title(&page, &sef);
    build_sefirah_section(&page, &sef);
    build_world_section(&page, sefirah_index);
    build_path_section(&page, path_index);
    build_meditation_section(&page, sefirah_index);
    build_tree_section(&page, &sef, middle);

    page.section_count = 5;
    page.has_data = 1;

    return page;
}

int kt_format(const kt_page_t *page, char *buf, int buf_size)
{
    if (!page || !buf || buf_size <= 0) {
        return 0;
    }

    if (!page->has_data) {
        return 0;
    }

    int n = snprintf(buf, (unsigned)buf_size,
                     "%s\n\n"
                     "%s\n\n"
                     "%s\n\n"
                     "%s\n\n"
                     "%s\n\n"
                     "%s",
                     page->page_title,
                     page->sefirah_section,
                     page->world_section,
                     page->path_section,
                     page->meditation_section,
                     page->tree_section);

    if (n < 0) {
        return 0;
    }

    /* snprintf may return value > buf_size if truncated */
    if (n >= buf_size) {
        return buf_size - 1;
    }

    return n;
}
