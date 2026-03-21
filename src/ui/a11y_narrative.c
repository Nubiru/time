/* a11y_narrative.c — Rich accessibility narratives for screen readers.
 * Produces human-readable narrative descriptions from astronomical and
 * calendar data. Pure module: no GL, no malloc, no globals. */

#include "a11y_narrative.h"
#include "content_i18n.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* ===== Sky position descriptions ===== */

static const char *const POSITION_DESCS[SKY_POS_COUNT] = {
    "rising in the east",
    "low in the east",
    "high in the sky",
    "nearly overhead",
    "low in the west",
    "setting in the west",
    "below the horizon"
};

/* ===== Brightness descriptions (verb phrases) ===== */

static const char *const BRIGHTNESS_DESCS[SKY_BRIGHT_COUNT] = {
    "shines brilliantly",
    "is clearly visible",
    "is visible",
    "is faintly visible",
    "is not visible to the naked eye"
};

/* ===== Transition names ===== */

static const char *const TRANSITION_NAMES[A11Y_TRANS_COUNT] = {
    "Zoom In",
    "Zoom Out",
    "Space View",
    "Earth View",
    "System Enter",
    "System Exit",
    "Time Forward",
    "Time Backward"
};

/* ===== Transition narrative templates ===== */

static const char *const TRANSITION_TEMPLATES[A11Y_TRANS_COUNT] = {
    "Zooming closer to %s.",
    "Zooming out to see %s.",
    "Switching to space view, looking outward from Earth.",
    "Switching to Earth view, looking at the surface.",
    "Entering the %s knowledge system.",
    "Leaving the %s knowledge system.",
    "Moving forward in time by %s.",
    "Moving backward in time by %s."
};

/* Fallback templates (no detail provided) */
static const char *const TRANSITION_FALLBACKS[A11Y_TRANS_COUNT] = {
    "Zooming closer.",
    "Zooming out.",
    "Switching to space view, looking outward from Earth.",
    "Switching to Earth view, looking at the surface.",
    "Entering a knowledge system.",
    "Leaving the current knowledge system.",
    "Moving forward in time.",
    "Moving backward in time."
};

/* ===== Helper: safe snprintf that returns clamped length ===== */

static int safe_snprintf(char *buf, int max_len, const char *fmt, ...)
    __attribute__((format(printf, 3, 4)));

static int safe_snprintf(char *buf, int max_len, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int n = vsnprintf(buf, (size_t)max_len, fmt, args);
    va_end(args);
    if (n < 0) return 0;
    if (n >= max_len) return max_len - 1;
    return n;
}

/* ===== Public API ===== */

const char *a11y_sky_position_desc(sky_position_t pos)
{
    if ((int)pos < 0 || (int)pos >= SKY_POS_COUNT) {
        return "?";
    }
    return POSITION_DESCS[(int)pos];
}

const char *a11y_brightness_desc(sky_brightness_t bright)
{
    if ((int)bright < 0 || (int)bright >= SKY_BRIGHT_COUNT) {
        return "?";
    }
    return BRIGHTNESS_DESCS[(int)bright];
}

const char *a11y_transition_name(a11y_transition_t type)
{
    if ((int)type < 0 || (int)type >= A11Y_TRANS_COUNT) {
        return "?";
    }
    return TRANSITION_NAMES[(int)type];
}

int a11y_sky_position_count(void)
{
    return SKY_POS_COUNT;
}

int a11y_brightness_count(void)
{
    return SKY_BRIGHT_COUNT;
}

int a11y_transition_type_count(void)
{
    return A11Y_TRANS_COUNT;
}

/* ===== Locale-aware getters ===== */

const char *a11y_sky_position_desc_locale(sky_position_t pos,
                                          i18n_locale_t locale)
{
    if ((int)pos < 0 || (int)pos >= SKY_POS_COUNT) return "?";
    char key[32];
    snprintf(key, sizeof(key), "a11y.sky.pos.%d", (int)pos);
    return content_get(key, locale);
}

const char *a11y_brightness_desc_locale(sky_brightness_t bright,
                                        i18n_locale_t locale)
{
    if ((int)bright < 0 || (int)bright >= SKY_BRIGHT_COUNT) return "?";
    char key[32];
    snprintf(key, sizeof(key), "a11y.sky.bright.%d", (int)bright);
    return content_get(key, locale);
}

const char *a11y_transition_name_locale(a11y_transition_t type,
                                        i18n_locale_t locale)
{
    if ((int)type < 0 || (int)type >= A11Y_TRANS_COUNT) return "?";
    char key[32];
    snprintf(key, sizeof(key), "a11y.trans.%d.name", (int)type);
    return content_get(key, locale);
}

int a11y_narr_planet(const a11y_planet_t *planet, char *out, int max_len)
{
    if (!planet || !out || max_len <= 0) return 0;

    const char *name = planet->name ? planet->name : "unknown";
    const char *sign = planet->sign ? planet->sign : "unknown";
    const char *pos_desc = a11y_sky_position_desc(planet->position);
    const char *bright_desc = a11y_brightness_desc(planet->brightness);

    int total = 0;
    int remaining = max_len;

    /* Planet name + brightness + position */
    if (planet->position == SKY_POS_BELOW) {
        total = safe_snprintf(out, remaining,
            "%s is below the horizon in %s", name, sign);
    } else {
        total = safe_snprintf(out, remaining,
            "%s %s, %s, in %s", name, bright_desc, pos_desc, sign);
    }

    remaining = max_len - total;
    if (remaining <= 1) goto finish;

    /* Near object */
    if (planet->near_object && planet->position != SKY_POS_BELOW) {
        int n = safe_snprintf(out + total, remaining,
            " near %s", planet->near_object);
        total += n;
        remaining -= n;
        if (remaining <= 1) goto finish;
    }

    /* Retrograde */
    if (planet->is_retrograde) {
        int n = safe_snprintf(out + total, remaining,
            ", moving retrograde");
        total += n;
        remaining -= n;
        if (remaining <= 1) goto finish;
    }

    /* Period */
    if (total < max_len - 1) {
        int n = safe_snprintf(out + total, remaining, ".");
        total += n;
    }

finish:
    return total;
}

int a11y_narr_sky(const a11y_planet_t *planets, int count,
                  char *out, int max_len)
{
    if (!planets || !out || max_len <= 0 || count <= 0) return 0;

    int total = 0;
    int remaining = max_len;

    /* Opening line */
    total = safe_snprintf(out, remaining, "The sky.");
    remaining = max_len - total;

    /* Each planet as a sentence */
    for (int i = 0; i < count && remaining > 1; i++) {
        /* Add space separator */
        int n = safe_snprintf(out + total, remaining, " ");
        total += n;
        remaining -= n;
        if (remaining <= 1) break;

        /* Planet narrative */
        n = a11y_narr_planet(&planets[i], out + total, remaining);
        total += n;
        remaining -= n;
    }

    return total;
}

int a11y_narr_calendar(const char *system_name, const char *date_str,
                       const char *meaning, char *out, int max_len)
{
    if (!out || max_len <= 0) return 0;

    const char *sys = system_name ? system_name : "Calendar";
    const char *date = date_str ? date_str : "today";

    int total;
    if (meaning && strlen(meaning) > 0) {
        total = safe_snprintf(out, max_len,
            "In the %s calendar, today is %s. %s.", sys, date, meaning);
    } else {
        total = safe_snprintf(out, max_len,
            "In the %s calendar, today is %s.", sys, date);
    }

    return total;
}

int a11y_narr_transition(a11y_transition_t type, const char *detail,
                         char *out, int max_len)
{
    if (!out || max_len <= 0) return 0;
    if ((int)type < 0 || (int)type >= A11Y_TRANS_COUNT) return 0;

    int total;
    if (detail && strlen(detail) > 0) {
        total = safe_snprintf(out, max_len,
            TRANSITION_TEMPLATES[(int)type], detail);
    } else {
        total = safe_snprintf(out, max_len,
            "%s", TRANSITION_FALLBACKS[(int)type]);
    }

    return total;
}

int a11y_narr_convergence(const char *const *systems, int count,
                          const char *strength_adj,
                          char *out, int max_len)
{
    if (!systems || !out || max_len <= 0 || count <= 0) return 0;

    const char *adj = strength_adj ? strength_adj : "notable";

    int total = 0;
    int remaining = max_len;

    /* Opening with count and adjective */
    if (count == 1) {
        total = safe_snprintf(out, remaining,
            "A %s moment in %s.",
            adj, systems[0] ? systems[0] : "unknown");
    } else {
        total = safe_snprintf(out, remaining,
            "A %s convergence across %d systems: ",
            adj, count);
        remaining = max_len - total;

        /* List system names */
        for (int i = 0; i < count && remaining > 1; i++) {
            const char *name = systems[i] ? systems[i] : "unknown";
            int n;
            if (i == 0) {
                n = safe_snprintf(out + total, remaining, "%s", name);
            } else if (i == count - 1) {
                n = safe_snprintf(out + total, remaining, " and %s", name);
            } else {
                n = safe_snprintf(out + total, remaining, ", %s", name);
            }
            total += n;
            remaining -= n;
        }

        /* Close */
        if (remaining > 1) {
            int n = safe_snprintf(out + total, remaining, ".");
            total += n;
        }
    }

    return total;
}
