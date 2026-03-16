/* personal_space.c -- Personal Space Data implementation.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects.
 * All modifier functions take struct by value, return new struct. */

#include "personal_space.h"

#include <string.h>

/* ----- helpers ----- */

static void safe_copy_string(char *dst, const char *src, int max_len) {
    if (!src) {
        dst[0] = '\0';
        return;
    }
    int i = 0;
    while (i < max_len - 1 && src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

/* Zodiac sign -> element index (0=fire, 1=earth, 2=air, 3=water).
 * Fire: Aries(0), Leo(4), Sag(8). Earth: Taurus(1), Virgo(5), Cap(9).
 * Air: Gemini(2), Libra(6), Aquarius(10). Water: Cancer(3), Scorp(7), Pisces(11). */
static int zodiac_element(int sign) {
    if (sign < 0 || sign > 11) return -1;
    static const int elements[12] = {
        0, 1, 2, 3,  /* Ari, Tau, Gem, Can */
        0, 1, 2, 3,  /* Leo, Vir, Lib, Sco */
        0, 1, 2, 3   /* Sag, Cap, Aqu, Pis */
    };
    return elements[sign];
}

/* Chinese animal -> element index (0=wood, 1=fire, 2=earth, 3=metal, 4=water).
 * Traditional fixed-element mapping by animal. */
static int chinese_element(int animal) {
    if (animal < 0 || animal > 11) return -1;
    /* Rat=water(4), Ox=earth(2), Tiger=wood(0), Rabbit=wood(0),
     * Dragon=earth(2), Snake=fire(1), Horse=fire(1), Goat=earth(2),
     * Monkey=metal(3), Rooster=metal(3), Dog=earth(2), Pig=water(4) */
    static const int elements[12] = {
        4, 2, 0, 0,  /* Rat, Ox, Tiger, Rabbit */
        2, 1, 1, 2,  /* Dragon, Snake, Horse, Goat */
        3, 3, 2, 4   /* Monkey, Rooster, Dog, Pig */
    };
    return elements[animal];
}

/* Kin tone (1-13) -> personality trait weight (0-100 range contribution).
 * Magnetic(1)=high, Lunar(2)=low, Electric(3)=mid, etc.
 * Simple mapping: tone itself normalized to a 0-40 range. */
static int tone_trait(int tone) {
    if (tone < 1 || tone > 13) return 0;
    /* Scale: tone 1->3, tone 7->20, tone 13->40. Linear interpolation. */
    return 3 + ((tone - 1) * 37) / 12;
}

/* ----- public API ----- */

ps_space_t ps_create(void) {
    ps_space_t s;
    memset(&s, 0, sizeof(s));
    s.favorite_count = 0;
    s.cycle_count = 0;
    s.bookmark_count = 0;
    s.explored_count = 0;
    s.favorite_systems[0] = -1;
    s.archetype.kin_seal = -1;
    s.archetype.kin_tone = -1;
    s.archetype.zodiac_sign = -1;
    s.archetype.chinese_animal = -1;
    s.archetype.hexagram = 0;
    s.archetype.archetype_score = 0;
    return s;
}

ps_space_t ps_add_favorite(ps_space_t space, int system_id) {
    /* Check duplicate */
    for (int i = 0; i < space.favorite_count; i++) {
        if (space.favorite_systems[i] == system_id) {
            return space;
        }
    }
    /* Check overflow */
    if (space.favorite_count >= PS_MAX_FAVORITES) {
        return space;
    }
    space.favorite_systems[space.favorite_count] = system_id;
    space.favorite_count++;
    /* Maintain -1 terminator if room */
    if (space.favorite_count < PS_MAX_FAVORITES) {
        space.favorite_systems[space.favorite_count] = -1;
    }
    return space;
}

ps_space_t ps_remove_favorite(ps_space_t space, int system_id) {
    int found = -1;
    for (int i = 0; i < space.favorite_count; i++) {
        if (space.favorite_systems[i] == system_id) {
            found = i;
            break;
        }
    }
    if (found < 0) return space;
    /* Shift remaining */
    for (int i = found; i < space.favorite_count - 1; i++) {
        space.favorite_systems[i] = space.favorite_systems[i + 1];
    }
    space.favorite_count--;
    if (space.favorite_count < PS_MAX_FAVORITES) {
        space.favorite_systems[space.favorite_count] = -1;
    }
    return space;
}

int ps_is_favorite(const ps_space_t *space, int system_id) {
    for (int i = 0; i < space->favorite_count; i++) {
        if (space->favorite_systems[i] == system_id) {
            return 1;
        }
    }
    return 0;
}

ps_space_t ps_add_cycle(ps_space_t space, const char *name, int system_id,
                         double start_jd, double next_event_jd) {
    if (space.cycle_count >= PS_MAX_CYCLES) return space;

    ps_cycle_t *c = &space.cycles[space.cycle_count];
    safe_copy_string(c->name, name, PS_CYCLE_NAME_MAX);
    c->system_id = system_id;
    c->start_jd = start_jd;
    c->end_jd = 0.0;
    c->next_event_jd = next_event_jd;
    c->active = 1;
    space.cycle_count++;
    return space;
}

ps_space_t ps_remove_cycle(ps_space_t space, int index) {
    if (index < 0 || index >= space.cycle_count) return space;
    for (int i = index; i < space.cycle_count - 1; i++) {
        space.cycles[i] = space.cycles[i + 1];
    }
    space.cycle_count--;
    return space;
}

ps_space_t ps_toggle_cycle(ps_space_t space, int index) {
    if (index < 0 || index >= space.cycle_count) return space;
    space.cycles[index].active = space.cycles[index].active ? 0 : 1;
    return space;
}

ps_space_t ps_bookmark(ps_space_t space, double jd, const char *note, int system_id) {
    if (space.bookmark_count >= PS_MAX_BOOKMARKS) return space;

    ps_bookmark_t *b = &space.bookmarks[space.bookmark_count];
    b->jd = jd;
    safe_copy_string(b->note, note, PS_NOTE_MAX);
    b->system_id = system_id;
    space.bookmark_count++;
    return space;
}

ps_space_t ps_remove_bookmark(ps_space_t space, int index) {
    if (index < 0 || index >= space.bookmark_count) return space;
    for (int i = index; i < space.bookmark_count - 1; i++) {
        space.bookmarks[i] = space.bookmarks[i + 1];
    }
    space.bookmark_count--;
    return space;
}

ps_space_t ps_mark_explored(ps_space_t space, int system_id) {
    /* Check duplicate */
    for (int i = 0; i < space.explored_count; i++) {
        if (space.explored_systems[i] == system_id) {
            return space;
        }
    }
    if (space.explored_count >= PS_MAX_EXPLORED) return space;
    space.explored_systems[space.explored_count] = system_id;
    space.explored_count++;
    return space;
}

int ps_is_explored(const ps_space_t *space, int system_id) {
    for (int i = 0; i < space->explored_count; i++) {
        if (space->explored_systems[i] == system_id) {
            return 1;
        }
    }
    return 0;
}

int ps_explored_count(const ps_space_t *space) {
    return space->explored_count;
}

double ps_exploration_progress(const ps_space_t *space, int total_systems) {
    if (total_systems <= 0) return 0.0;
    double ratio = (double)space->explored_count / (double)total_systems;
    if (ratio > 1.0) ratio = 1.0;
    return ratio;
}

ps_archetype_t ps_compute_archetype(int kin_seal, int kin_tone,
                                     int zodiac_sign, int chinese_animal,
                                     int hexagram) {
    ps_archetype_t a;
    a.kin_seal = kin_seal;
    a.kin_tone = kin_tone;
    a.zodiac_sign = zodiac_sign;
    a.chinese_animal = chinese_animal;
    a.hexagram = hexagram;

    /* No birth data -> score 0 */
    if (kin_tone < 0 && zodiac_sign < 0 && chinese_animal < 0) {
        a.archetype_score = 0;
        return a;
    }

    /* Composite score from three weighted components:
     * - Tone trait: 0-40 (weight ~40%)
     * - Zodiac element: 0-30 (weight ~30%)
     * - Chinese element: 0-30 (weight ~30%) */
    int score = 0;

    /* Tone contribution (0-40) */
    score += tone_trait(kin_tone);

    /* Zodiac element contribution (0-30) */
    int z_elem = zodiac_element(zodiac_sign);
    if (z_elem >= 0) {
        /* fire=30, air=22, water=15, earth=8 — energy spectrum */
        static const int z_scores[4] = { 30, 8, 22, 15 };
        score += z_scores[z_elem];
    }

    /* Chinese element contribution (0-30) */
    int c_elem = chinese_element(chinese_animal);
    if (c_elem >= 0) {
        /* wood=24, fire=30, earth=12, metal=18, water=6 — energy spectrum */
        static const int c_scores[5] = { 24, 30, 12, 18, 6 };
        score += c_scores[c_elem];
    }

    /* Clamp to 0-100 */
    if (score < 0) score = 0;
    if (score > 100) score = 100;

    a.archetype_score = score;
    return a;
}

int ps_active_cycles(const ps_space_t *space, ps_cycle_t *out, int max) {
    int count = 0;
    for (int i = 0; i < space->cycle_count && count < max; i++) {
        if (space->cycles[i].active) {
            out[count] = space->cycles[i];
            count++;
        }
    }
    return count;
}

int ps_upcoming_bookmarks(const ps_space_t *space, double jd_start,
                           double jd_end, ps_bookmark_t *out, int max) {
    /* Collect matching bookmarks */
    ps_bookmark_t temp[PS_MAX_BOOKMARKS];
    int count = 0;
    for (int i = 0; i < space->bookmark_count; i++) {
        if (space->bookmarks[i].jd >= jd_start &&
            space->bookmarks[i].jd <= jd_end) {
            temp[count] = space->bookmarks[i];
            count++;
        }
    }

    /* Simple insertion sort by jd */
    for (int i = 1; i < count; i++) {
        ps_bookmark_t key = temp[i];
        int j = i - 1;
        while (j >= 0 && temp[j].jd > key.jd) {
            temp[j + 1] = temp[j];
            j--;
        }
        temp[j + 1] = key;
    }

    /* Copy up to max */
    int out_count = count < max ? count : max;
    for (int i = 0; i < out_count; i++) {
        out[i] = temp[i];
    }
    return out_count;
}

int ps_summary(const ps_space_t *space, char *buf, int buf_size) {
    if (buf_size <= 0) return 0;

    /* Count active cycles */
    int active = 0;
    for (int i = 0; i < space->cycle_count; i++) {
        if (space->cycles[i].active) active++;
    }

    int written = snprintf(buf, (size_t)buf_size,
        "%d favorites, %d active cycles, %d bookmarks, %d systems explored",
        space->favorite_count, active,
        space->bookmark_count, space->explored_count);

    if (written < 0) {
        buf[0] = '\0';
        return 0;
    }
    if (written >= buf_size) {
        return buf_size - 1;
    }
    return written;
}
