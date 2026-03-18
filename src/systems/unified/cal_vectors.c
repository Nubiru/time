/* cal_vectors.c — Calendar Test Vector database implementation
 * Static database of historically verified reference dates.
 *
 * Sources:
 *   - Reingold & Dershowitz, "Calendrical Calculations" (4th ed.)
 *   - Meeus, "Astronomical Algorithms" (2nd ed.)
 *   - JPL Horizons ephemeris service
 *   - US Naval Observatory circular 179
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "cal_vectors.h"

/* ========== Static vector database ========== */

static const cv_vector_t s_vectors[] = {
    /* Vector 0: J2000.0 Epoch */
    {
        .jd = 2451545.0,
        .name = "J2000.0 Epoch",
        .description = "Standard astronomical epoch, noon UT 2000 Jan 1",
        .entry_count = 5,
        .entries = {
            { CV_CAL_GREGORIAN, 2000,  1,  1, 0 },
            { CV_CAL_HEBREW,    5760, 10, 23, 0 },  /* 23 Tevet 5760 */
            { CV_CAL_ISLAMIC,   1420,  9, 25, 0 },  /* 25 Ramadan 1420 */
            { CV_CAL_PERSIAN,   1378, 10, 11, 0 },  /* 11 Dey 1378 */
            { CV_CAL_COPTIC,    1716,  4, 22, 0 },  /* 22 Koiak 1716 */
        }
    },
    /* Vector 1: Gregorian Reform */
    {
        .jd = 2299161.0,
        .name = "Gregorian Reform",
        .description = "First day of the Gregorian calendar, promulgated by Pope Gregory XIII",
        .entry_count = 1,
        .entries = {
            { CV_CAL_GREGORIAN, 1582, 10, 15, 0 },
        }
    },
    /* Vector 2: French Revolution Calendar Start */
    {
        .jd = 2375840.0,
        .name = "French Republican Calendar Start",
        .description = "Autumnal equinox 1792, epoch of the French Republican calendar",
        .entry_count = 2,
        .entries = {
            { CV_CAL_GREGORIAN,         1792, 9, 22, 0 },
            { CV_CAL_FRENCH_REPUBLICAN,    1, 1,  1, 0 },
        }
    },
    /* Vector 3: Islamic Epoch */
    {
        .jd = 1948439.5,
        .name = "Islamic Epoch",
        .description = "Hijra — Muhammad's migration from Mecca to Medina, 1 Muharram AH 1",
        .entry_count = 2,
        .entries = {
            { CV_CAL_GREGORIAN, 622, 7, 19, 0 },
            { CV_CAL_ISLAMIC,     1, 1,  1, 0 },
        }
    },
    /* Vector 4: Unix Epoch */
    {
        .jd = 2440587.5,
        .name = "Unix Epoch",
        .description = "Midnight beginning 1970 Jan 1, origin of Unix time",
        .entry_count = 1,
        .entries = {
            { CV_CAL_GREGORIAN, 1970, 1, 1, 0 },
        }
    },
    /* Vector 5: Year 2000 Leap Day */
    {
        .jd = 2451604.5,
        .name = "Year 2000 Leap Day",
        .description = "Feb 29 in a century leap year (divisible by 400)",
        .entry_count = 1,
        .entries = {
            { CV_CAL_GREGORIAN, 2000, 2, 29, 0 },
        }
    },
    /* Vector 6: Coptic Epoch */
    {
        .jd = 1825029.5,
        .name = "Coptic Epoch",
        .description = "Era of Diocletian / Era of Martyrs, 1 Thout AM 1",
        .entry_count = 2,
        .entries = {
            { CV_CAL_GREGORIAN, 284, 8, 29, 0 },
            { CV_CAL_COPTIC,      1, 1,  1, 0 },
        }
    },
    /* Vector 7: Mayan Long Count 13.0.0.0.0 */
    {
        .jd = 2456282.5,
        .name = "Mayan Long Count End",
        .description = "Completion of 13th b'ak'tun (13.0.0.0.0), GMT correlation 584283",
        .entry_count = 1,
        .entries = {
            { CV_CAL_GREGORIAN, 2012, 12, 21, 0 },
        }
    },
    /* Vector 8: Bahai Epoch */
    {
        .jd = 2394647.0,
        .name = "Bahai Epoch",
        .description = "Declaration of the Bab, Naw-Ruz 1 BE (Bahai Era)",
        .entry_count = 2,
        .entries = {
            { CV_CAL_GREGORIAN, 1844, 3, 21, 0 },
            { CV_CAL_BAHAI,        1, 1,  1, 0 },
        }
    },
    /* Vector 9: Persian Epoch (Nowruz) */
    {
        .jd = 1952063.0,
        .name = "Persian Epoch",
        .description = "Solar Hijri epoch, Nowruz of the year of the Hijra",
        .entry_count = 2,
        .entries = {
            { CV_CAL_GREGORIAN, 622, 3, 22, 0 },
            { CV_CAL_PERSIAN,     1, 1,  1, 0 },
        }
    },
};

#define CV_VECTOR_COUNT ((int)(sizeof(s_vectors) / sizeof(s_vectors[0])))

/* ========== API implementation ========== */

int cv_count(void)
{
    return CV_VECTOR_COUNT;
}

cv_vector_t cv_get(int index)
{
    if (index < 0 || index >= CV_VECTOR_COUNT) {
        cv_vector_t sentinel = {
            .jd = 0.0,
            .name = (void *)0,
            .description = (void *)0,
            .entry_count = 0,
            .entries = {{ 0 }}
        };
        return sentinel;
    }
    return s_vectors[index];
}

double cv_jd(int index)
{
    if (index < 0 || index >= CV_VECTOR_COUNT) {
        return 0.0;
    }
    return s_vectors[index].jd;
}

const char *cv_name(int index)
{
    if (index < 0 || index >= CV_VECTOR_COUNT) {
        return "Unknown";
    }
    return s_vectors[index].name;
}

cv_entry_t cv_find_entry(cv_vector_t vec, int system)
{
    for (int i = 0; i < vec.entry_count; i++) {
        if (vec.entries[i].system == system) {
            return vec.entries[i];
        }
    }
    cv_entry_t not_found = { .system = -1, .year = 0, .month = 0, .day = 0, .extra = 0 };
    return not_found;
}

int cv_has_system(cv_vector_t vec, int system)
{
    for (int i = 0; i < vec.entry_count; i++) {
        if (vec.entries[i].system == system) {
            return 1;
        }
    }
    return 0;
}
