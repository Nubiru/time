/* daily_narrative.c — Personalized daily digest composer.
 *
 * Composes headline, personal resonance, systems overview, and wisdom
 * sections from pre-computed calendar/convergence/birth data.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "daily_narrative.h"
#include <string.h>

/* Tzolkin seal names indexed 0-19 for seal matching */
static const char *const SEAL_NAMES[20] = {
    "Dragon", "Wind", "Night", "Seed", "Serpent",
    "Worldbridger", "Hand", "Star", "Moon", "Dog",
    "Monkey", "Human", "Skywalker", "Wizard", "Eagle",
    "Warrior", "Earth", "Mirror", "Storm", "Sun"
};

/* Month names for headline formatting */
static const char *const MONTH_NAMES[12] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};

/* Internal headline type marker stored in unused bytes of headline.
 * We use the convergence_score + a simple tag approach instead. */
#define DN_HEADLINE_TAG_DATE  "[D]"
#define DN_HEADLINE_TAG_SIG   "[S]"
#define DN_HEADLINE_TAG_CONV  "[C]"
#define DN_TAG_LEN 3

/* --- dn_default_input --- */

dn_input_t dn_default_input(void)
{
    dn_input_t in;
    memset(&in, 0, sizeof(in));
    return in;
}

/* --- Internal: check if birth seal matches today's seal --- */

static int seal_matches(const char *birth_seal, int today_seal)
{
    if (!birth_seal || today_seal < 0 || today_seal > 19) {
        return 0;
    }
    return (strcmp(birth_seal, SEAL_NAMES[today_seal]) == 0);
}

/* --- Internal: get month name --- */

static const char *month_name(int month)
{
    if (month < 1 || month > 12) {
        return "Unknown";
    }
    return MONTH_NAMES[month - 1];
}

/* --- Internal: compose headline --- */

typedef enum {
    HL_DATE = 0,
    HL_SIGNIFICANCE = 1,
    HL_CONVERGENCE = 2
} headline_kind_t;

static headline_kind_t compose_headline(const dn_input_t *input,
                                         char *buf, int buf_size)
{
    /* Priority 1: convergence (score >= 3 and desc present) */
    if (input->convergence_score >= 3 && input->convergence_desc) {
        snprintf(buf, (size_t)buf_size, "%s%s",
                 DN_HEADLINE_TAG_CONV, input->convergence_desc);
        return HL_CONVERGENCE;
    }

    /* Priority 2: first insight with significance >= 2 */
    int count = input->insight_count;
    if (count > DN_MAX_SYSTEMS) {
        count = DN_MAX_SYSTEMS;
    }
    for (int i = 0; i < count; i++) {
        if (input->insights[i].significance >= 2) {
            snprintf(buf, (size_t)buf_size, "%s%s",
                     DN_HEADLINE_TAG_SIG, input->insights[i].summary);
            return HL_SIGNIFICANCE;
        }
    }

    /* Priority 3: date-based */
    if (input->weekday) {
        snprintf(buf, (size_t)buf_size, "%s%s, %s %d — %d systems speak today",
                 DN_HEADLINE_TAG_DATE,
                 input->weekday,
                 month_name(input->gregorian_month),
                 input->gregorian_day,
                 count);
    } else {
        snprintf(buf, (size_t)buf_size, "%s%s %d — %d systems speak today",
                 DN_HEADLINE_TAG_DATE,
                 month_name(input->gregorian_month),
                 input->gregorian_day,
                 count);
    }
    return HL_DATE;
}

/* --- Internal: compose systems section --- */

static int compose_systems(const dn_input_t *input, char *buf, int buf_size)
{
    int count = input->insight_count;
    if (count > DN_MAX_SYSTEMS) {
        count = DN_MAX_SYSTEMS;
    }
    if (count <= 0) {
        buf[0] = '\0';
        return 0;
    }

    int pos = 0;
    for (int i = 0; i < count && pos < buf_size - 1; i++) {
        const dn_insight_t *ins = &input->insights[i];
        int written;
        if (i > 0) {
            written = snprintf(buf + pos, (size_t)(buf_size - pos), ". ");
            if (written > 0) {
                pos += written;
            }
        }
        written = snprintf(buf + pos, (size_t)(buf_size - pos),
                           "%s: %s",
                           ins->system_name ? ins->system_name : "Unknown",
                           ins->summary);
        if (written > 0) {
            pos += written;
        }
    }
    return count;
}

/* --- Internal: compose personal section --- */

static int compose_personal(const dn_input_t *input, char *buf, int buf_size)
{
    if (!input->has_birth) {
        buf[0] = '\0';
        return 0;
    }

    int resonances = 0;
    int pos = 0;

    /* Check seal match */
    if (seal_matches(input->birth_seal, input->today_seal)) {
        int w = snprintf(buf + pos, (size_t)(buf_size - pos),
                         "Your birth seal appears today");
        if (w > 0) { pos += w; }
        resonances++;
    }

    /* Check tone match */
    if (input->birth_tone > 0 && input->birth_tone == input->today_tone) {
        if (resonances > 0 && pos < buf_size - 1) {
            int w = snprintf(buf + pos, (size_t)(buf_size - pos), ". ");
            if (w > 0) { pos += w; }
        }
        int w = snprintf(buf + pos, (size_t)(buf_size - pos),
                         "Your birth tone (%d) resonates today",
                         input->birth_tone);
        if (w > 0) { pos += w; }
        resonances++;
    }

    /* Check hexagram match */
    if (input->birth_hexagram > 0 &&
        input->birth_hexagram == input->today_hexagram) {
        if (resonances > 0 && pos < buf_size - 1) {
            int w = snprintf(buf + pos, (size_t)(buf_size - pos), ". ");
            if (w > 0) { pos += w; }
        }
        int w = snprintf(buf + pos, (size_t)(buf_size - pos),
                         "Your birth hexagram returns today");
        if (w > 0) { pos += w; }
        resonances++;
    }

    /* Check Hebrew month match */
    if (input->birth_hebrew_month > 0 &&
        input->birth_hebrew_month == input->today_hebrew_month) {
        if (resonances > 0 && pos < buf_size - 1) {
            int w = snprintf(buf + pos, (size_t)(buf_size - pos), ". ");
            if (w > 0) { pos += w; }
        }
        int w = snprintf(buf + pos, (size_t)(buf_size - pos),
                         "You are in your birth month");
        if (w > 0) { pos += w; }
        resonances++;
    }

    /* No resonances found */
    if (resonances == 0) {
        snprintf(buf, (size_t)buf_size,
                 "No direct resonances today — the cycles continue their dance");
    }

    return resonances;
}

/* --- Internal: compose wisdom section --- */

static void compose_wisdom(const dn_input_t *input, char *buf, int buf_size)
{
    if (input->wisdom_quote && input->wisdom_author) {
        snprintf(buf, (size_t)buf_size, "'%s' — %s",
                 input->wisdom_quote, input->wisdom_author);
    } else {
        buf[0] = '\0';
    }
}

/* --- dn_compose --- */

daily_narrative_t dn_compose(const dn_input_t *input)
{
    daily_narrative_t n;
    memset(&n, 0, sizeof(n));

    if (!input) {
        return n;
    }

    /* Headline (with internal tag prefix) */
    compose_headline(input, n.headline, (int)sizeof(n.headline));

    /* Systems */
    n.insight_count = compose_systems(input, n.systems,
                                       (int)sizeof(n.systems));

    /* Personal */
    n.resonance_count = compose_personal(input, n.personal,
                                          (int)sizeof(n.personal));

    /* Wisdom */
    compose_wisdom(input, n.wisdom, (int)sizeof(n.wisdom));

    /* Echo convergence score */
    n.convergence_score = input->convergence_score;

    /* Strip the internal tag from headline for display.
     * We keep the tag as the first 3 chars for dn_headline_type(). */
    /* Actually, we store headline WITH tag. dn_headline_type reads
     * the tag, and callers see it via the headline field. We need to
     * shift the visible text to remove the tag. Instead, let's keep
     * the tag in the first 3 bytes and shift the rest forward on return.
     *
     * Better approach: store the tag in the headline and let
     * dn_headline_type parse it. The visible text starts at offset 3. */

    /* Shift headline left to remove tag, but preserve info for type query.
     * We'll embed the type in the last byte of headline instead. */

    /* Revised approach: store headline type in a reserved area.
     * We use headline[255] (last byte of 256 buffer) as type marker:
     * 'D' = date, 'S' = significance, 'C' = convergence. */
    char tag = n.headline[1];  /* [D], [S], [C] — 2nd char is the type */
    /* Shift visible text left by DN_TAG_LEN */
    size_t len = strlen(n.headline);
    if (len >= DN_TAG_LEN) {
        memmove(n.headline, n.headline + DN_TAG_LEN, len - DN_TAG_LEN + 1);
    }
    /* Store type marker at end of buffer (never visible in normal strings) */
    n.headline[sizeof(n.headline) - 1] = tag;

    return n;
}

/* --- dn_headline_type --- */

int dn_headline_type(const daily_narrative_t *narrative)
{
    if (!narrative) {
        return 0;
    }
    char tag = narrative->headline[sizeof(narrative->headline) - 1];
    switch (tag) {
    case 'S': return 1;
    case 'C': return 2;
    default:  return 0;
    }
}
