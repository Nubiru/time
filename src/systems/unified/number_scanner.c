/* number_scanner.c — cross-system number scanner implementation.
 * Pure data + lookup. All data is static const. No malloc, no globals. */

#include "number_scanner.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* ===== Static cycle registry (21 entries) ===== */

static const ns_cycle_t CYCLES[] = {
    { "Tzolkin",         "Sacred calendar",     260.0,           "days"  },
    { "Haab",            "Solar calendar",      365.0,           "days"  },
    { "Calendar Round",  "Tzolkin x Haab",      18980.0,         "days"  },
    { "Lunar",           "Synodic month",       29.53059,        "days"  },
    { "Metonic",         "19-year cycle",       6939.688,        "days"  },
    { "Saros",           "Eclipse cycle",       6585.32,         "days"  },
    { "Precession",      "Great Year",          25772.0,         "years" },
    { "Hebrew",          "Sabbatical",          7.0,             "years" },
    { "Hebrew",          "Jubilee",             50.0,            "years" },
    { "Islamic",         "30-year cycle",       10631.0,         "days"  },
    { "I Ching",         "Hexagram count",      64.0,            "units" },
    { "Human Design",    "Gate count",          64.0,            "units" },
    { "Chinese",         "Sexagenary cycle",    60.0,            "years" },
    { "Buddhist",        "Kalpa",               4320000000.0,    "years" },
    { "Hindu",           "Maha-Yuga",           4320000.0,       "years" },
    { "Hindu",           "Kali Yuga",           432000.0,        "years" },
    { "Zodiac",          "Signs",               12.0,            "units" },
    { "Solar",           "Tropical year",       365.2422,        "days"  },
    { "Week",            "Week",                7.0,             "days"  },
    { "Kabbalah",        "Sefirot",             10.0,            "units" },
    { "Kabbalah",        "Paths",               22.0,            "units" }
};

static const int CYCLE_COUNT = (int)(sizeof(CYCLES) / sizeof(CYCLES[0]));

/* ===== Static sacred number registry (11 entries) ===== */

static const ns_sacred_t SACRED[] = {
    {     7, "Sabbatical",        "Days of week, planets, sefirot below Abyss"     },
    {    12, "Zodiacal",          "Signs, months, tribes"                           },
    {    22, "Kabbalistic",       "Hebrew letters, paths, Major Arcana"             },
    {    36, "Decanic",           "Decanates of zodiac"                             },
    {    52, "Calendar Round",    "52 years, 52 weeks"                              },
    {    64, "Binary complete",   "Hexagrams, codons, HD gates"                     },
    {    72, "Precessional",      "Years per degree of precession"                  },
    {   108, "Sacred Hindu",      "Nakshatra padas, prayer beads"                   },
    {   360, "Circle",            "Degrees, approximate year"                       },
    {   432, "Cosmic",            "Kali Yuga base, 432 Hz tuning"                   },
    { 25920, "Precession",        "Full precession cycle (approx)"                  }
};

static const int SACRED_COUNT = (int)(sizeof(SACRED) / sizeof(SACRED[0]));

/* ===== Helper: count unique system names in match array ===== */

static int count_unique_systems(const ns_match_t *matches, int count)
{
    const char *seen[NS_MAX_MATCHES];
    int seen_count = 0;

    for (int i = 0; i < count; i++) {
        int already = 0;
        for (int j = 0; j < seen_count; j++) {
            if (strcmp(seen[j], matches[i].system_name) == 0) {
                already = 1;
                break;
            }
        }
        if (!already) {
            seen[seen_count] = matches[i].system_name;
            seen_count++;
        }
    }
    return seen_count;
}

/* ===== Helper: scale check count ===== */

static int scale_check_internal(int target)
{
    if (target <= 0) {
        return 0;
    }

    int count = 0;
    double scaled = (double)target;

    for (int n = 1; n <= 9; n++) {
        scaled *= 10.0;
        int matched_this_scale = 0;

        for (int c = 0; c < CYCLE_COUNT; c++) {
            long cycle_int = (long)CYCLES[c].length;
            if (cycle_int <= 0) {
                continue;
            }
            double diff = fabs(scaled - (double)cycle_int);
            double tolerance = 0.01 * (double)cycle_int;
            if (diff <= tolerance) {
                matched_this_scale = 1;
                break;
            }
        }

        if (matched_this_scale) {
            count++;
        }
    }

    return count;
}

/* ===== Public API ===== */

int ns_cycle_count(void)
{
    return CYCLE_COUNT;
}

ns_cycle_t ns_cycle_get(int index)
{
    if (index < 0 || index >= CYCLE_COUNT) {
        ns_cycle_t empty = { NULL, NULL, 0.0, NULL };
        return empty;
    }
    return CYCLES[index];
}

int ns_sacred_count(void)
{
    return SACRED_COUNT;
}

ns_sacred_t ns_sacred_get(int index)
{
    if (index < 0 || index >= SACRED_COUNT) {
        ns_sacred_t empty = { -1, NULL, NULL };
        return empty;
    }
    return SACRED[index];
}

ns_scan_result_t number_scan(int target)
{
    ns_scan_result_t result;
    memset(&result, 0, sizeof(result));

    if (target <= 0) {
        return result;
    }

    /* Step 1: Check each cycle for factor/multiple relationships */
    for (int c = 0; c < CYCLE_COUNT && result.match_count < NS_MAX_MATCHES; c++) {
        long cycle_int = (long)CYCLES[c].length;
        if (cycle_int <= 0) {
            continue;
        }

        if (cycle_int % (long)target == 0) {
            /* target divides cycle: FACTOR relationship */
            ns_match_t *m = &result.matches[result.match_count];
            m->system_name = CYCLES[c].system;
            m->cycle_name = CYCLES[c].name;
            m->cycle_length = CYCLES[c].length;
            m->relationship = NS_REL_FACTOR;
            m->ratio = (double)cycle_int / (double)target;
            snprintf(m->description, sizeof(m->description),
                     "Factor of %s %s (%ld/%d)",
                     CYCLES[c].system, CYCLES[c].name, cycle_int, target);
            result.match_count++;
        } else if ((long)target % cycle_int == 0) {
            /* cycle divides target: MULTIPLE relationship */
            ns_match_t *m = &result.matches[result.match_count];
            m->system_name = CYCLES[c].system;
            m->cycle_name = CYCLES[c].name;
            m->cycle_length = CYCLES[c].length;
            m->relationship = NS_REL_MULTIPLE;
            m->ratio = (double)target / (double)cycle_int;
            snprintf(m->description, sizeof(m->description),
                     "Multiple of %s %s (%d/%ld)",
                     CYCLES[c].system, CYCLES[c].name, target, cycle_int);
            result.match_count++;
        }
    }

    /* Step 2: Scale check — target * 10^n within 1% of a cycle */
    double scaled = (double)target;
    for (int n = 1; n <= 9 && result.match_count < NS_MAX_MATCHES; n++) {
        scaled *= 10.0;

        for (int c = 0; c < CYCLE_COUNT && result.match_count < NS_MAX_MATCHES; c++) {
            long cycle_int = (long)CYCLES[c].length;
            if (cycle_int <= 0) {
                continue;
            }
            double diff = fabs(scaled - (double)cycle_int);
            double tolerance = 0.01 * (double)cycle_int;
            if (diff <= tolerance) {
                /* Check we haven't already matched this cycle as factor/multiple */
                int already = 0;
                for (int k = 0; k < result.match_count; k++) {
                    if (strcmp(result.matches[k].cycle_name, CYCLES[c].name) == 0 &&
                        strcmp(result.matches[k].system_name, CYCLES[c].system) == 0 &&
                        result.matches[k].relationship != NS_REL_SCALE) {
                        /* Already have a factor/multiple for this exact cycle;
                         * still add scale if it's a different relationship */
                    }
                    if (strcmp(result.matches[k].cycle_name, CYCLES[c].name) == 0 &&
                        strcmp(result.matches[k].system_name, CYCLES[c].system) == 0 &&
                        result.matches[k].relationship == NS_REL_SCALE) {
                        already = 1;
                        break;
                    }
                }
                if (!already) {
                    ns_match_t *m = &result.matches[result.match_count];
                    m->system_name = CYCLES[c].system;
                    m->cycle_name = CYCLES[c].name;
                    m->cycle_length = CYCLES[c].length;
                    m->relationship = NS_REL_SCALE;
                    m->ratio = scaled / (double)cycle_int;
                    snprintf(m->description, sizeof(m->description),
                             "Scale: %d x10^%d ~ %s %s",
                             target, n, CYCLES[c].system, CYCLES[c].name);
                    result.match_count++;
                }
            }
        }
    }

    /* Step 3: Count unique systems (coincidence_score) */
    result.coincidence_score = count_unique_systems(result.matches, result.match_count);

    /* Step 4: Count scale appearances */
    result.scale_appearances = scale_check_internal(target);

    return result;
}

int factor_scan(long cycle_length, int *out_factors, int out_max)
{
    if (cycle_length <= 0 || out_factors == NULL || out_max <= 0) {
        return 0;
    }

    int count = 0;
    for (int i = 0; i < SACRED_COUNT && count < out_max; i++) {
        if (SACRED[i].number > 0 && cycle_length % (long)SACRED[i].number == 0) {
            out_factors[count] = SACRED[i].number;
            count++;
        }
    }

    return count;
}

int coincidence_score(int number)
{
    ns_scan_result_t r = number_scan(number);
    return r.coincidence_score;
}

int ns_is_factor_of_cycle(int number)
{
    if (number <= 0) {
        return 0;
    }

    for (int c = 0; c < CYCLE_COUNT; c++) {
        long cycle_int = (long)CYCLES[c].length;
        if (cycle_int > 0 && cycle_int % (long)number == 0) {
            return 1;
        }
    }

    return 0;
}

int ns_is_sacred_multiple(int number)
{
    if (number <= 0) {
        return 0;
    }

    for (int i = 0; i < SACRED_COUNT; i++) {
        if (SACRED[i].number > 0 && number % SACRED[i].number == 0) {
            return 1;
        }
    }

    return 0;
}

int ns_scale_check(int number)
{
    return scale_check_internal(number);
}
