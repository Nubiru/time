/* daily_greeting.c — First text a user sees when entering Time.
 *
 * Pure functions: no globals, no malloc, no side effects.
 * Uses snprintf into caller-provided buffers. */

#include "daily_greeting.h"
#include "content_i18n.h"

#include <stdio.h>
#include <string.h>

/* ---- English fallback greetings ---- */

static const char *const en_greetings[DG_PERIOD_COUNT] = {
    [DG_PERIOD_DAWN]      = "The light returns",
    [DG_PERIOD_MORNING]   = "Good morning",
    [DG_PERIOD_MIDDAY]    = "Good afternoon",
    [DG_PERIOD_AFTERNOON] = "Good afternoon",
    [DG_PERIOD_EVENING]   = "Good evening",
    [DG_PERIOD_NIGHT]     = "Good night",
};

/* Content keys matching the period enum order */
static const char *const greeting_keys[DG_PERIOD_COUNT] = {
    "greeting.dawn",
    "greeting.morning",
    "greeting.midday",
    "greeting.afternoon",
    "greeting.evening",
    "greeting.night",
};

/* ---- Period classification ---- */

dg_period_t dg_period(int hour)
{
    if (hour < 0 || hour > 23) return DG_PERIOD_NIGHT;
    if (hour < 5)  return DG_PERIOD_NIGHT;
    if (hour < 8)  return DG_PERIOD_DAWN;
    if (hour < 12) return DG_PERIOD_MORNING;
    if (hour < 14) return DG_PERIOD_MIDDAY;
    if (hour < 18) return DG_PERIOD_AFTERNOON;
    if (hour < 21) return DG_PERIOD_EVENING;
    return DG_PERIOD_NIGHT;
}

/* ---- Greeting text lookup ---- */

static const char *greeting_text(dg_period_t period, i18n_locale_t locale)
{
    if (period >= DG_PERIOD_COUNT) {
        return en_greetings[DG_PERIOD_NIGHT];
    }

    /* EN locale: use hardcoded English fallbacks directly */
    if (locale == I18N_LOCALE_EN) {
        return en_greetings[period];
    }

    /* Other locales: try content_get, which falls back to EN then key */
    const char *text = content_get(greeting_keys[period], locale);

    /* If content_get returned the key itself (no translation found),
     * fall back to our hardcoded English */
    if (text == greeting_keys[period]) {
        return en_greetings[period];
    }

    return text;
}

/* ---- Summary construction ---- */

static void build_summary(const dg_input_t *input, char *buf, int buf_len)
{
    int pos = 0;
    buf[0] = '\0';

    /* Weekday, date */
    if (input->weekday != NULL && input->date_str != NULL) {
        pos += snprintf(buf + pos, (size_t)(buf_len - pos),
                        "%s, %s.", input->weekday, input->date_str);
    } else if (input->weekday != NULL) {
        pos += snprintf(buf + pos, (size_t)(buf_len - pos),
                        "%s.", input->weekday);
    } else if (input->date_str != NULL) {
        pos += snprintf(buf + pos, (size_t)(buf_len - pos),
                        "%s.", input->date_str);
    }

    /* Kin string */
    if (input->kin_str != NULL && pos < buf_len - 1) {
        if (pos > 0) {
            pos += snprintf(buf + pos, (size_t)(buf_len - pos), " ");
        }
        pos += snprintf(buf + pos, (size_t)(buf_len - pos),
                        "%s.", input->kin_str);
    }

    (void)pos; /* suppress unused warning on final value */
}

/* ---- Teaser construction ---- */

static void build_teaser(const dg_input_t *input, char *buf, int buf_len)
{
    buf[0] = '\0';

    if (input->convergence_count <= 0) {
        return;
    }

    if (input->convergence != NULL) {
        snprintf(buf, (size_t)buf_len, "%s.", input->convergence);
    } else {
        snprintf(buf, (size_t)buf_len, "%d systems converge today.",
                 input->convergence_count);
    }
}

/* ---- Public API ---- */

int dg_compose(const dg_input_t *input, dg_output_t *out)
{
    if (input == NULL || out == NULL) {
        return 0;
    }

    /* Period */
    out->period = dg_period(input->hour);

    /* Greeting */
    const char *g = greeting_text(out->period, input->locale);
    snprintf(out->greeting, sizeof(out->greeting), "%s", g);

    /* Summary */
    build_summary(input, out->summary, (int)sizeof(out->summary));

    /* Teaser */
    build_teaser(input, out->teaser, (int)sizeof(out->teaser));

    return 1;
}
