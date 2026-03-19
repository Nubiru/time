/* welcome_text.c -- Welcome Journey text content generator.
 *
 * Generates personalized onboarding text for each screen of the
 * welcome journey, using birth_profile_t data.
 *
 * Pure module: no globals, no malloc, no GL.
 * Uses file-scope static buffers for formatted output. */

#include "welcome_text.h"

#include <stdio.h>
#include <string.h>

/* ---- static buffers ---- */

static char s_reveal_buf[WT_BUF_SIZE];
static char s_summary_buf[WT_BUF_SIZE * 4];

/* ---- screen titles (static const) ---- */

static const char *SCREEN_TITLES[WT_SCREEN_COUNT] = {
    "When did you arrive on this planet?",     /* INTRO */
    "Where on Earth were you standing?",       /* LOCATION */
    "The Dreamspell",                          /* REVEAL_TZOLKIN */
    "The Chinese Calendar",                    /* REVEAL_CHINESE */
    "The Hebrew Calendar",                     /* REVEAL_HEBREW */
    "The Islamic Calendar",                    /* REVEAL_ISLAMIC */
    "The Buddhist Era",                        /* REVEAL_BUDDHIST */
    "The I Ching",                             /* REVEAL_ICHING */
    "The Zodiac",                              /* REVEAL_ASTROLOGY */
    "Welcome to Time"                          /* SUMMARY */
};

/* ---- screen subtitles (static const) ---- */

static const char *SCREEN_SUBTITLES[WT_SCREEN_COUNT] = {
    "Your birth date is the key that unlocks every time system.",
    "Optional — your location adds solar and sidereal precision.",
    "The 260-day sacred calendar of the Maya",
    "Sixty-year cycles of animal and element",
    "The oldest lunisolar calendar still in use",
    "A pure lunar calendar marking sacred months",
    "Counting years from the Parinirvana",
    "The Book of Changes, 64 hexagrams of wisdom",
    "Where the planets stood when you arrived",
    "You exist across every tradition of measuring time."
};

/* ---- screen names for debug/display ---- */

static const char *SCREEN_NAMES[WT_SCREEN_COUNT] = {
    "Intro", "Location",
    "Tzolkin", "Chinese", "Hebrew", "Islamic",
    "Buddhist", "I Ching", "Astrology",
    "Summary"
};

/* ---- castle name lookup ---- */

static const char *castle_name(int castle)
{
    switch (castle) {
    case 1: return "Red Turning";
    case 2: return "White Crossing";
    case 3: return "Blue Burning";
    case 4: return "Yellow Giving";
    case 5: return "Green Enchantment";
    default: return "Unknown";
    }
}

/* ---- reveal text generators ---- */

static const char *reveal_tzolkin(const birth_profile_t *p)
{
    snprintf(s_reveal_buf, sizeof(s_reveal_buf),
             "You are Kin %d: %s %s. Your wavespell is %d, in the %s Castle.",
             p->tzolkin.kin,
             p->tzolkin.tone_name,
             p->tzolkin.seal_name,
             p->tzolkin.wavespell,
             castle_name(p->tzolkin.castle));
    return s_reveal_buf;
}

static const char *reveal_chinese(const birth_profile_t *p)
{
    const char *pol = p->chinese.polarity == 0 ? "Yang" : "Yin";
    snprintf(s_reveal_buf, sizeof(s_reveal_buf),
             "You were born in the Year of the %s %s — %s polarity.",
             p->chinese.element_name,
             p->chinese.animal_name,
             pol);
    return s_reveal_buf;
}

static const char *reveal_hebrew(const birth_profile_t *p)
{
    snprintf(s_reveal_buf, sizeof(s_reveal_buf),
             "Your birth falls on %d %s, in Hebrew year %d.",
             p->hebrew.day,
             p->hebrew.month_name,
             p->hebrew.year);
    return s_reveal_buf;
}

static const char *reveal_islamic(const birth_profile_t *p)
{
    snprintf(s_reveal_buf, sizeof(s_reveal_buf),
             "In the Islamic calendar, you arrived on %d %s, year %d AH.",
             p->islamic.day,
             p->islamic.month_name,
             p->islamic.year);
    return s_reveal_buf;
}

static const char *reveal_buddhist(const birth_profile_t *p)
{
    snprintf(s_reveal_buf, sizeof(s_reveal_buf),
             "In the Buddhist Era, your birth year is %d BE"
             " — %d years after the Parinirvana.",
             p->buddhist.era_year,
             p->buddhist.era_year);
    return s_reveal_buf;
}

static const char *reveal_iching(const birth_profile_t *p)
{
    snprintf(s_reveal_buf, sizeof(s_reveal_buf),
             "Your birth hexagram is %d: %s.",
             p->iching.king_wen,
             p->iching.name);
    return s_reveal_buf;
}

static const char *reveal_astrology(const birth_profile_t *p)
{
    snprintf(s_reveal_buf, sizeof(s_reveal_buf),
             "The Sun stood at %.1f degrees in %s when you arrived.",
             p->astrology.sun_longitude,
             p->astrology.sun_sign_name);
    return s_reveal_buf;
}

/* ---- public API ---- */

const char *wt_screen_title(wt_screen_t screen)
{
    if (screen < 0 || screen >= WT_SCREEN_COUNT) return "Unknown";
    return SCREEN_TITLES[screen];
}

const char *wt_screen_subtitle(wt_screen_t screen)
{
    if (screen < 0 || screen >= WT_SCREEN_COUNT) return "";
    return SCREEN_SUBTITLES[screen];
}

const char *wt_reveal_text(wt_screen_t screen, const birth_profile_t *profile)
{
    if (!profile) return "";
    switch (screen) {
    case WT_SCREEN_REVEAL_TZOLKIN:   return reveal_tzolkin(profile);
    case WT_SCREEN_REVEAL_CHINESE:   return reveal_chinese(profile);
    case WT_SCREEN_REVEAL_HEBREW:    return reveal_hebrew(profile);
    case WT_SCREEN_REVEAL_ISLAMIC:   return reveal_islamic(profile);
    case WT_SCREEN_REVEAL_BUDDHIST:  return reveal_buddhist(profile);
    case WT_SCREEN_REVEAL_ICHING:    return reveal_iching(profile);
    case WT_SCREEN_REVEAL_ASTROLOGY: return reveal_astrology(profile);
    default: return "";
    }
}

const char *wt_summary_text(const birth_profile_t *profile)
{
    if (!profile) return "";

    static const char *systems[] = {
        "tzolkin", "chinese", "hebrew", "islamic",
        "buddhist", "iching", "astrology"
    };
    static const int nsystems = 7;

    int pos = 0;
    pos += snprintf(s_summary_buf + pos, sizeof(s_summary_buf) - (size_t)pos,
                    "Your cosmic identity spans seven traditions:\n\n");

    for (int i = 0; i < nsystems && pos < (int)sizeof(s_summary_buf) - 1; i++) {
        const char *line = bp_summary(profile, systems[i]);
        if (line) {
            pos += snprintf(s_summary_buf + pos,
                            sizeof(s_summary_buf) - (size_t)pos,
                            "%s\n", line);
        }
    }

    if (pos < (int)sizeof(s_summary_buf) - 1) {
        snprintf(s_summary_buf + pos, sizeof(s_summary_buf) - (size_t)pos,
                 "\nEvery calendar, every cycle, every tradition"
                 " — a different lens on the same moment: your arrival.");
    }

    return s_summary_buf;
}

int wt_reveal_screen_count(void)
{
    return 7;
}

int wt_is_reveal_screen(wt_screen_t screen)
{
    return screen >= WT_SCREEN_REVEAL_TZOLKIN
        && screen <= WT_SCREEN_REVEAL_ASTROLOGY;
}

const char *wt_screen_name(wt_screen_t screen)
{
    if (screen < 0 || screen >= WT_SCREEN_COUNT) return "Unknown";
    return SCREEN_NAMES[screen];
}
