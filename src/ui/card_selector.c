/* card_selector.c — Zoom-aware card system selection.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "card_selector.h"
#include <stddef.h>

/* System name lookup table — indexed by system_id (ts_system_t values).
 * Must match the enum order in today_summary.h. */
static const char *s_system_names[] = {
    "Gregorian",     /* 0  TS_SYS_GREGORIAN */
    "Tzolkin",       /* 1  TS_SYS_TZOLKIN */
    "Haab",          /* 2  TS_SYS_HAAB */
    "Chinese",       /* 3  TS_SYS_CHINESE */
    "Hebrew",        /* 4  TS_SYS_HEBREW */
    "Islamic",       /* 5  TS_SYS_ISLAMIC */
    "Buddhist",      /* 6  TS_SYS_BUDDHIST */
    "Hindu",         /* 7  TS_SYS_HINDU */
    "I Ching",       /* 8  TS_SYS_ICHING */
    "Astrology",     /* 9  TS_SYS_ASTROLOGY */
    "Human Design",  /* 10 TS_SYS_HUMAN_DESIGN */
    "Kabbalah",      /* 11 TS_SYS_KABBALAH */
    "Coptic",        /* 12 TS_SYS_COPTIC */
    "Ethiopian",     /* 13 TS_SYS_ETHIOPIAN */
    "Persian",       /* 14 TS_SYS_PERSIAN */
    "Japanese",      /* 15 TS_SYS_JAPANESE */
    "Korean",        /* 16 TS_SYS_KOREAN */
    "Thai",          /* 17 TS_SYS_THAI */
    "Geological",    /* 18 TS_SYS_GEOLOGICAL */
    "Cosmic"         /* 19 TS_SYS_COSMIC */
};

#define CS_NAME_COUNT ((int)(sizeof(s_system_names) / sizeof(s_system_names[0])))

/* Composite relevance score: weighted blend of opacity and scale.
 * Opacity weighted higher — it most directly reflects proximity
 * to the focal depth. */
static float cs_score(const cd_card_state_t *card)
{
    return card->opacity * 0.6f + card->scale * 0.4f;
}

cs_selection_t cs_select_from_frame(const cd_frame_t *frame)
{
    cs_selection_t sel;

    /* Initialize all slots to empty */
    for (int i = 0; i < CS_SLOT_COUNT; i++) {
        sel.slots[i].system_id = -1;
        sel.slots[i].relevance = 0.0f;
        sel.slots[i].opacity = 0.0f;
        sel.slots[i].scale = 0.0f;
    }
    sel.filled_count = 0;
    sel.zoom_level = 0.0f;

    if (!frame || frame->count <= 0) {
        return sel;
    }

    sel.zoom_level = frame->focal_depth;

    /* Collect indices of visible cards with their scores.
     * Using parallel arrays to avoid structs (selection sort below). */
    int indices[CD_MAX_CARDS];
    float scores[CD_MAX_CARDS];
    int visible_count = 0;

    for (int i = 0; i < frame->count && i < CD_MAX_CARDS; i++) {
        if (frame->cards[i].visible) {
            indices[visible_count] = i;
            scores[visible_count] = cs_score(&frame->cards[i]);
            visible_count++;
        }
    }

    /* Selection sort: pick top CS_SLOT_COUNT by score (descending).
     * Only 20 items max, so O(n*k) is trivial. */
    int pick_count = visible_count < CS_SLOT_COUNT
                     ? visible_count : CS_SLOT_COUNT;

    for (int slot = 0; slot < pick_count; slot++) {
        /* Find the maximum score among remaining candidates */
        int best = slot;
        for (int j = slot + 1; j < visible_count; j++) {
            if (scores[j] > scores[best]) {
                best = j;
            }
        }

        /* Swap best into current slot position */
        if (best != slot) {
            int tmp_idx = indices[slot];
            indices[slot] = indices[best];
            indices[best] = tmp_idx;

            float tmp_score = scores[slot];
            scores[slot] = scores[best];
            scores[best] = tmp_score;
        }

        /* Fill the output slot */
        const cd_card_state_t *card = &frame->cards[indices[slot]];
        sel.slots[slot].system_id = card->system_id;
        sel.slots[slot].relevance = scores[slot];
        sel.slots[slot].opacity = card->opacity;
        sel.slots[slot].scale = card->scale;
    }

    sel.filled_count = pick_count;
    return sel;
}

cs_selection_t cs_select(float zoom_level, float aspect_ratio)
{
    cd_plane_t planes[CD_MAX_CARDS];
    int count = cd_default_planes(planes, CD_MAX_CARDS);
    cd_frame_t frame = cd_compute(zoom_level, planes, count, aspect_ratio);
    return cs_select_from_frame(&frame);
}

int cs_find_system(const cs_selection_t *sel, int system_id)
{
    if (!sel) {
        return -1;
    }

    for (int i = 0; i < sel->filled_count; i++) {
        if (sel->slots[i].system_id == system_id) {
            return i;
        }
    }
    return -1;
}

const char *cs_slot_name(const cs_slot_t *slot)
{
    if (!slot || slot->system_id < 0 || slot->system_id >= CS_NAME_COUNT) {
        return NULL;
    }
    return s_system_names[slot->system_id];
}
