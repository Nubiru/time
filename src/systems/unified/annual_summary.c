/* annual_summary.c — "Your Year in Time" data backbone.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "annual_summary.h"

#include <stdio.h>
#include <string.h>

/* ---- display names ---- */

static const char *CAT_NAMES[AS_CAT_COUNT] = {
    "Birthday", "Cycle", "Coincidence", "Milestone", "Journey"
};

static const char *SIG_NAMES[] = {
    "Routine", "Notable", "Rare", "Extraordinary"
};
#define SIG_NAME_COUNT (int)(sizeof(SIG_NAMES) / sizeof(SIG_NAMES[0]))

/* ---- milestone definitions ---- */

typedef struct {
    int age;
    as_significance_t sig;
    const char *label;
} milestone_def_t;

static const milestone_def_t MILESTONES[] = {
    {18,  AS_SIG_NOTABLE,       "Coming of age"},
    {21,  AS_SIG_NOTABLE,       "Full majority"},
    {25,  AS_SIG_NOTABLE,       "Quarter century"},
    {30,  AS_SIG_NOTABLE,       "Three decades"},
    {33,  AS_SIG_NOTABLE,       "The Christ year"},
    {40,  AS_SIG_NOTABLE,       "Four decades"},
    {50,  AS_SIG_NOTABLE,       "Half century"},
    {52,  AS_SIG_RARE,          "Calendar Round complete"},
    {60,  AS_SIG_RARE,          "Sexagenary cycle complete"},
    {65,  AS_SIG_NOTABLE,       "65 orbits"},
    {70,  AS_SIG_NOTABLE,       "Seven decades"},
    {75,  AS_SIG_NOTABLE,       "Three quarters"},
    {80,  AS_SIG_NOTABLE,       "Eight decades"},
    {90,  AS_SIG_NOTABLE,       "Nine decades"},
    {100, AS_SIG_EXTRAORDINARY, "A century of time"},
};
#define MILESTONE_COUNT (int)(sizeof(MILESTONES) / sizeof(MILESTONES[0]))

/* ---- helpers ---- */

static int is_leap(int y)
{
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

static int year_days(int y)
{
    return is_leap(y) ? 366 : 365;
}

static void add_highlight(as_summary_t *s, const char *headline,
                           const char *body, as_category_t cat,
                           as_significance_t sig, int month)
{
    if (s->highlight_count >= AS_MAX_HIGHLIGHTS) return;
    as_highlight_t *h = &s->highlights[s->highlight_count];
    snprintf(h->headline, AS_HEADLINE_MAX, "%s", headline);
    snprintf(h->body, AS_BODY_MAX, "%s", body);
    h->category = cat;
    h->significance = sig;
    h->month = month;
    s->highlight_count++;
}

/* ---- public API ---- */

as_engagement_t as_default_engagement(void)
{
    as_engagement_t e;
    memset(&e, 0, sizeof(e));
    return e;
}

as_summary_t as_compute(const birth_profile_t *profile, int year,
                         const as_engagement_t *engagement)
{
    as_summary_t s;
    memset(&s, 0, sizeof(s));

    if (!profile) return s;

    int age = year - profile->year;
    int dy = year_days(year);

    /* Birthday profile for this year */
    birth_profile_t bday = bp_compute(year, profile->month, profile->day);

    /* Jan 1 profile for galactic return computation */
    birth_profile_t jan1 = bp_compute(year, 1, 1);

    /* ---- Stats ---- */
    s.stats.year = year;
    s.stats.age = age;
    s.stats.is_leap = is_leap(year);
    s.stats.birthday_kin = bday.tzolkin.kin;
    s.stats.birthday_seal = bday.tzolkin.seal;
    s.stats.birthday_tone = bday.tzolkin.tone;
    s.stats.birthday_hexagram = bday.iching.king_wen;
    s.stats.chinese_animal = bday.chinese.animal;
    s.stats.chinese_element = bday.chinese.element;
    s.stats.wavespells = dy / 13;

    /* Galactic returns: how many times birth kin appears in this year */
    int birth_kin = profile->tzolkin.kin;
    int offset = ((birth_kin - jan1.tzolkin.kin) % 260 + 260) % 260;
    s.stats.galactic_returns = (offset + 260 < dy) ? 2 : 1;

    /* ---- Highlights ---- */

    /* 1. Birthday */
    if (age >= 0) {
        char hl[AS_HEADLINE_MAX];
        char bd[AS_BODY_MAX];
        snprintf(hl, sizeof(hl), "You turned %d", age);
        snprintf(bd, sizeof(bd), "In the Year of the %s, %s energy.",
                 bday.chinese.animal_name, bday.chinese.element_name);
        add_highlight(&s, hl, bd, AS_CAT_BIRTHDAY, AS_SIG_ROUTINE,
                      profile->month);
    }

    /* 2. Birthday Kin */
    {
        char hl[AS_HEADLINE_MAX];
        char bd[AS_BODY_MAX];
        as_significance_t sig =
            (bday.tzolkin.wavespell == profile->tzolkin.wavespell)
                ? AS_SIG_NOTABLE : AS_SIG_ROUTINE;
        snprintf(hl, sizeof(hl), "Kin %d: %s %s on your birthday",
                 bday.tzolkin.kin, bday.tzolkin.tone_name,
                 bday.tzolkin.seal_name);
        snprintf(bd, sizeof(bd),
                 "Your birthday in %d carried this Tzolkin energy.", year);
        add_highlight(&s, hl, bd, AS_CAT_BIRTHDAY, sig, profile->month);
    }

    /* 3. Chinese Year */
    {
        char hl[AS_HEADLINE_MAX];
        char bd[AS_BODY_MAX];
        snprintf(hl, sizeof(hl), "Year of the %s", bday.chinese.animal_name);
        snprintf(bd, sizeof(bd), "%s %s year. %d wavespells traversed.",
                 bday.chinese.element_name, bday.chinese.animal_name,
                 s.stats.wavespells);
        add_highlight(&s, hl, bd, AS_CAT_CYCLE, AS_SIG_ROUTINE, 0);
    }

    /* 4. Galactic Return */
    {
        char hl[AS_HEADLINE_MAX];
        char bd[AS_BODY_MAX];
        if (s.stats.galactic_returns == 2) {
            snprintf(hl, sizeof(hl), "Double Galactic Return");
            snprintf(bd, sizeof(bd),
                     "Your birth Kin %d appeared twice this year.", birth_kin);
            add_highlight(&s, hl, bd, AS_CAT_CYCLE, AS_SIG_NOTABLE, 0);
        } else {
            snprintf(hl, sizeof(hl), "Galactic Return");
            snprintf(bd, sizeof(bd),
                     "Your birth Kin %d echoed through the year.", birth_kin);
            add_highlight(&s, hl, bd, AS_CAT_CYCLE, AS_SIG_ROUTINE, 0);
        }
    }

    /* 5. Birthday seal match (1/20 chance per year) */
    if (bday.tzolkin.seal == profile->tzolkin.seal) {
        char bd[AS_BODY_MAX];
        snprintf(bd, sizeof(bd),
                 "Your birthday carried %s, the same seal as your birth.",
                 bday.tzolkin.seal_name);
        add_highlight(&s, "Birthday Seal Echo", bd,
                      AS_CAT_COINCIDENCE, AS_SIG_NOTABLE, profile->month);
    }

    /* 6. Birthday hexagram match */
    if (bday.iching.king_wen == profile->iching.king_wen &&
        profile->iching.king_wen > 0) {
        char bd[AS_BODY_MAX];
        snprintf(bd, sizeof(bd),
                 "Hexagram %d: %s appeared on your birthday, same as birth.",
                 bday.iching.king_wen, bday.iching.name);
        add_highlight(&s, "Hexagram Return", bd,
                      AS_CAT_COINCIDENCE, AS_SIG_RARE, profile->month);
    }

    /* 7. Chinese animal return (every 12 years) */
    if (bday.chinese.animal == profile->chinese.animal) {
        char bd[AS_BODY_MAX];
        snprintf(bd, sizeof(bd),
                 "The %s returned, your birth animal. A 12-year homecoming.",
                 bday.chinese.animal_name);
        add_highlight(&s, "Birth Animal Return", bd,
                      AS_CAT_COINCIDENCE, AS_SIG_NOTABLE, 0);
    }

    /* 8. Age milestones */
    for (int i = 0; i < MILESTONE_COUNT; i++) {
        if (age == MILESTONES[i].age) {
            char hl[AS_HEADLINE_MAX];
            char bd[AS_BODY_MAX];
            snprintf(hl, sizeof(hl), "%s: age %d",
                     MILESTONES[i].label, age);
            if (age == 52) {
                snprintf(bd, sizeof(bd),
                         "52 years: the Tzolkin Calendar Round. "
                         "Your birth day-sign and Haab position align again.");
            } else if (age == 60) {
                snprintf(bd, sizeof(bd),
                         "60 years: the Chinese sexagenary cycle completes. "
                         "Heavenly Stem and Earthly Branch return together.");
            } else {
                snprintf(bd, sizeof(bd),
                         "%d years of orbits around the Sun.", age);
            }
            add_highlight(&s, hl, bd, AS_CAT_MILESTONE, MILESTONES[i].sig,
                          profile->month);
            break;
        }
    }

    /* 9. Journey (from engagement data) */
    if (engagement && engagement->total_visits > 0) {
        char hl[AS_HEADLINE_MAX];
        char bd[AS_BODY_MAX];
        as_significance_t sig = (engagement->longest_streak >= 30)
                                 ? AS_SIG_NOTABLE : AS_SIG_ROUTINE;
        snprintf(hl, sizeof(hl), "%d systems explored",
                 engagement->systems_explored);
        snprintf(bd, sizeof(bd),
                 "%d visits, %.0f hours with Time. Longest streak: %d days.",
                 engagement->total_visits, engagement->total_hours,
                 engagement->longest_streak);
        add_highlight(&s, hl, bd, AS_CAT_JOURNEY, sig, 0);
    }

    /* ---- Narratives ---- */
    snprintf(s.opening, AS_BODY_MAX,
             "In %d, you completed %d orbits around the Sun.", year, age);
    snprintf(s.closing, AS_BODY_MAX,
             "Time witnessed your year. Every moment was unique.");

    return s;
}

int as_top_highlights(const as_summary_t *summary,
                      as_highlight_t *out, int max)
{
    if (!summary || !out || max <= 0) return 0;

    int n = summary->highlight_count;
    if (n > max) n = max;
    memcpy(out, summary->highlights, (size_t)n * sizeof(as_highlight_t));

    /* Insertion sort descending by significance */
    for (int i = 1; i < n; i++) {
        as_highlight_t tmp = out[i];
        int j = i - 1;
        while (j >= 0 && out[j].significance < tmp.significance) {
            out[j + 1] = out[j];
            j--;
        }
        out[j + 1] = tmp;
    }

    return n;
}

int as_by_category(const as_summary_t *summary,
                   as_category_t category,
                   as_highlight_t *out, int max)
{
    if (!summary || !out || max <= 0) return 0;
    int n = 0;
    for (int i = 0; i < summary->highlight_count && n < max; i++) {
        if (summary->highlights[i].category == category) {
            out[n++] = summary->highlights[i];
        }
    }
    return n;
}

int as_count_significant(const as_summary_t *summary,
                          as_significance_t min_sig)
{
    if (!summary) return 0;
    int n = 0;
    for (int i = 0; i < summary->highlight_count; i++) {
        if (summary->highlights[i].significance >= min_sig) n++;
    }
    return n;
}

const char *as_category_name(as_category_t cat)
{
    if ((int)cat < 0 || cat >= AS_CAT_COUNT) return "?";
    return CAT_NAMES[cat];
}

const char *as_significance_name(as_significance_t sig)
{
    if ((int)sig < 0 || (int)sig >= SIG_NAME_COUNT) return "?";
    return SIG_NAMES[sig];
}

int as_format_summary(const as_summary_t *summary, char *buf, int buf_size)
{
    if (!summary || !buf || buf_size <= 0) return 0;

    int written = 0;
    int rem = buf_size;

    /* Opening */
    int n = snprintf(buf + written, (size_t)rem, "%s\n\n", summary->opening);
    if (n >= rem) { buf[buf_size - 1] = '\0'; return buf_size - 1; }
    written += n;
    rem -= n;

    /* Stats line */
    n = snprintf(buf + written, (size_t)rem,
                 "Year: %d | Age: %d | Kin: %d | Highlights: %d\n\n",
                 summary->stats.year, summary->stats.age,
                 summary->stats.birthday_kin, summary->highlight_count);
    if (n >= rem) { buf[buf_size - 1] = '\0'; return written; }
    written += n;
    rem -= n;

    /* Highlights */
    for (int i = 0; i < summary->highlight_count && rem > 1; i++) {
        const as_highlight_t *h = &summary->highlights[i];
        n = snprintf(buf + written, (size_t)rem, "- %s: %s\n",
                     h->headline, h->body);
        if (n >= rem) { buf[buf_size - 1] = '\0'; return written; }
        written += n;
        rem -= n;
    }

    /* Closing */
    if (rem > 1) {
        n = snprintf(buf + written, (size_t)rem, "\n%s\n", summary->closing);
        if (n >= rem) { buf[buf_size - 1] = '\0'; return written; }
        written += n;
    }

    return written;
}
