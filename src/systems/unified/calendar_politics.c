/* calendar_politics.c -- Pure data module documenting key historical
 * moments where political power shaped global timekeeping.
 *
 * 10 decisions sorted chronologically from 46 BCE to 1930 CE.
 * All functions are pure: no globals, no malloc, no side effects. */

#include "calendar_politics.h"

#include <ctype.h>
#include <stddef.h>
#include <string.h>

#define CP_NUM_DECISIONS 10

static const cp_decision_t decisions[CP_NUM_DECISIONS] = {
    {
        .title = "Julian Calendar Reform",
        .year = -46,
        .who = "Julius Caesar (advised by Sosigenes)",
        .what_changed = "Replaced chaotic Roman calendar with 365.25-day "
                        "solar year, fixed month lengths",
        .what_lost = "Lunar-based timekeeping, natural month=lunation "
                     "alignment",
        .who_benefited = "Roman Empire (administrative uniformity)",
        .category = CP_CAT_IMPERIAL
    },
    {
        .title = "Constantine's Sabbath Shift",
        .year = 321,
        .who = "Emperor Constantine",
        .what_changed = "Mandated Sunday as official rest day empire-wide "
                        "(dies Solis)",
        .what_lost = "Saturn's day (Saturday/Sabbath) as universal rest day",
        .who_benefited = "Roman state + Christian church "
                         "(religious-political alignment)",
        .category = CP_CAT_RELIGIOUS
    },
    {
        .title = "Council of Nicaea",
        .year = 325,
        .who = "Emperor Constantine + 318 bishops",
        .what_changed = "Standardized Easter computation, 7-day week "
                        "mandated empire-wide",
        .what_lost = "Regional calendar diversity, local seasonal reckoning",
        .who_benefited = "Church hierarchy (unified religious calendar)",
        .category = CP_CAT_RELIGIOUS
    },
    {
        .title = "Anno Domini Epoch",
        .year = 525,
        .who = "Dionysius Exiguus",
        .what_changed = "Created AD/BC dating system centered on Jesus's "
                        "birth",
        .what_lost = "Older era systems (Roman AUC, Olympic cycles), "
                     "accurate birth date (4-7 BCE)",
        .who_benefited = "Christian world (time itself recentered on faith)",
        .category = CP_CAT_RELIGIOUS
    },
    {
        .title = "Gregorian Calendar Reform",
        .year = 1582,
        .who = "Pope Gregory XIII (advised by Aloysius Lilius, "
               "Christopher Clavius)",
        .what_changed = "Deleted 10 days, refined leap year rules "
                        "(century rule)",
        .what_lost = "Julian continuity, Protestant nations refused for "
                     "170+ years",
        .who_benefited = "Catholic church (Easter accuracy), eventually "
                         "global commerce",
        .category = CP_CAT_SCIENTIFIC
    },
    {
        .title = "British Calendar Act",
        .year = 1752,
        .who = "British Parliament",
        .what_changed = "Adopted Gregorian calendar, deleted 11 days "
                        "(Sep 2 to 14)",
        .what_lost = "11 days of civic/economic life, public protests",
        .who_benefited = "British Empire (alignment with European trade)",
        .category = CP_CAT_IMPERIAL
    },
    {
        .title = "Railroad Standard Time",
        .year = 1883,
        .who = "US and Canadian railroad companies",
        .what_changed = "Replaced ~300 local solar times with 4 standard "
                        "time zones",
        .what_lost = "Local solar noon, natural connection to sun position",
        .who_benefited = "Railroad industry (scheduling), commerce",
        .category = CP_CAT_COMMERCIAL
    },
    {
        .title = "International Meridian Conference",
        .year = 1884,
        .who = "25 nations at Washington DC",
        .what_changed = "Established Greenwich as Prime Meridian, 24 time "
                        "zones, International Date Line",
        .what_lost = "Multiple competing meridians, local time sovereignty",
        .who_benefited = "British Empire (Greenwich), international shipping",
        .category = CP_CAT_COMMERCIAL
    },
    {
        .title = "Standard Time Act",
        .year = 1918,
        .who = "US Congress",
        .what_changed = "Codified time zones into federal law, introduced "
                        "Daylight Saving Time",
        .what_lost = "Natural sunrise-based schedules, farmers' rhythms",
        .who_benefited = "Industrial economy, war effort (fuel savings)",
        .category = CP_CAT_COMMERCIAL
    },
    {
        .title = "Soviet Revolutionary Calendar",
        .year = 1930,
        .who = "Joseph Stalin",
        .what_changed = "Replaced 7-day week with 5-day nepreryvka "
                        "(continuous work week)",
        .what_lost = "Shared rest day, religious Sabbath, family "
                     "synchronization",
        .who_benefited = "Soviet state (continuous industrial production)",
        .category = CP_CAT_IMPERIAL
    }
};

static const char *category_names[CP_CAT_COUNT] = {
    "Religious",
    "Imperial",
    "Commercial",
    "Scientific"
};

int cp_decision_count(void)
{
    return CP_NUM_DECISIONS;
}

cp_decision_t cp_decision_get(int index)
{
    if (index < 0 || index >= CP_NUM_DECISIONS) {
        cp_decision_t empty = {
            .title = NULL,
            .year = 0,
            .who = NULL,
            .what_changed = NULL,
            .what_lost = NULL,
            .who_benefited = NULL,
            .category = CP_CAT_RELIGIOUS
        };
        return empty;
    }
    return decisions[index];
}

const char *cp_category_name(cp_category_t category)
{
    if (category < 0 || category >= CP_CAT_COUNT) {
        return "Unknown";
    }
    return category_names[category];
}

int cp_decisions_by_category(cp_category_t category)
{
    if (category < 0 || category >= CP_CAT_COUNT) {
        return 0;
    }
    int count = 0;
    for (int i = 0; i < CP_NUM_DECISIONS; i++) {
        if (decisions[i].category == category) {
            count++;
        }
    }
    return count;
}

/* Case-insensitive substring search */
static int ci_strstr(const char *haystack, const char *needle)
{
    if (!haystack || !needle) {
        return 0;
    }
    size_t hlen = strlen(haystack);
    size_t nlen = strlen(needle);
    if (nlen == 0) {
        return 1;
    }
    if (nlen > hlen) {
        return 0;
    }
    for (size_t i = 0; i <= hlen - nlen; i++) {
        size_t j;
        for (j = 0; j < nlen; j++) {
            if (tolower((unsigned char)haystack[i + j]) !=
                tolower((unsigned char)needle[j])) {
                break;
            }
        }
        if (j == nlen) {
            return 1;
        }
    }
    return 0;
}

int cp_find_by_keyword(const char *keyword)
{
    if (!keyword) {
        return -1;
    }
    for (int i = 0; i < CP_NUM_DECISIONS; i++) {
        if (ci_strstr(decisions[i].title, keyword)) {
            return i;
        }
    }
    return -1;
}

int cp_decisions_in_range(int year_start, int year_end,
                           int *out, int max_out)
{
    if (!out || max_out <= 0) {
        return 0;
    }
    int count = 0;
    for (int i = 0; i < CP_NUM_DECISIONS && count < max_out; i++) {
        if (decisions[i].year >= year_start &&
            decisions[i].year <= year_end) {
            out[count] = i;
            count++;
        }
    }
    return count;
}

int cp_natural_losses_count(void)
{
    int count = 0;
    for (int i = 0; i < CP_NUM_DECISIONS; i++) {
        if (decisions[i].what_lost &&
            strlen(decisions[i].what_lost) > 0) {
            count++;
        }
    }
    return count;
}

const char *cp_arguelles_quote(void)
{
    return "Time is not money. Time is art.";
}
