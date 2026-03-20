#include "card_data.h"

#include <stdio.h>
#include <string.h>

static const char *safe_str(const char *s)
{
    return s ? s : "?";
}

static const char *ZODIAC_NAMES[12] = {
    "Aries", "Taurus", "Gemini", "Cancer",
    "Leo", "Virgo", "Libra", "Scorpio",
    "Sagittarius", "Capricorn", "Aquarius", "Pisces"
};

static const char *zodiac_name(int sign)
{
    if (sign < 0 || sign > 11) return "?";
    return ZODIAC_NAMES[sign];
}

card_content_t card_format_tzolkin(int seal, int tone, int kin,
                                   const char *seal_name)
{
    card_content_t c;
    memset(&c, 0, sizeof(c));

    snprintf(c.title, sizeof(c.title), "Tzolkin");
    snprintf(c.line1, sizeof(c.line1), "Kin %d", kin);
    snprintf(c.line2, sizeof(c.line2), "Tone %d - Seal %d", tone, seal);
    snprintf(c.line3, sizeof(c.line3), "%s", safe_str(seal_name));
    snprintf(c.detail, sizeof(c.detail),
             "Kin %d | Tone %d | Seal %d (%s)",
             kin, tone, seal, safe_str(seal_name));

    return c;
}

card_content_t card_format_iching(int number, const char *name,
                                  const char *upper_tri, const char *lower_tri,
                                  const int lines[6])
{
    card_content_t c;
    memset(&c, 0, sizeof(c));

    snprintf(c.title, sizeof(c.title), "I Ching");
    snprintf(c.line1, sizeof(c.line1), "Hexagram %d", number);
    snprintf(c.line2, sizeof(c.line2), "%s", safe_str(name));

    if (lines) {
        /* Render lines top-down: lines[5] is top, lines[0] is bottom */
        char line_str[16];
        for (int i = 5; i >= 0; i--) {
            line_str[5 - i] = lines[i] ? '-' : ' ';
        }
        line_str[6] = '\0';
        snprintf(c.line3, sizeof(c.line3), "%s / %s  [%s]",
                 safe_str(upper_tri), safe_str(lower_tri), line_str);
    } else {
        snprintf(c.line3, sizeof(c.line3), "%s / %s",
                 safe_str(upper_tri), safe_str(lower_tri));
    }

    snprintf(c.detail, sizeof(c.detail),
             "Hex %d: %s | Upper: %s | Lower: %s",
             number, safe_str(name),
             safe_str(upper_tri), safe_str(lower_tri));

    return c;
}

card_content_t card_format_chinese(const char *animal_symbol,
                                   const char *element_name,
                                   const char *animal_name,
                                   const char *polarity_name,
                                   int stem, int branch)
{
    card_content_t c;
    memset(&c, 0, sizeof(c));

    snprintf(c.title, sizeof(c.title), "Chinese Year");
    snprintf(c.line1, sizeof(c.line1), "%s %s",
             safe_str(animal_symbol), safe_str(animal_name));
    snprintf(c.line2, sizeof(c.line2), "%s %s",
             safe_str(element_name), safe_str(polarity_name));
    snprintf(c.line3, sizeof(c.line3), "Stem %d Branch %d", stem, branch);
    snprintf(c.detail, sizeof(c.detail),
             "%s %s %s (%s) | Stem %d Branch %d",
             safe_str(element_name), safe_str(polarity_name),
             safe_str(animal_name), safe_str(animal_symbol),
             stem, branch);

    return c;
}

card_content_t card_format_human_design(int sun_gate, int sun_line,
                                        int earth_gate, int earth_line,
                                        const char *gate_name,
                                        const char *gate_keyword)
{
    card_content_t c;
    memset(&c, 0, sizeof(c));

    snprintf(c.title, sizeof(c.title), "Human Design");
    snprintf(c.line1, sizeof(c.line1), "Sun Gate %d.%d", sun_gate, sun_line);
    snprintf(c.line2, sizeof(c.line2), "Earth Gate %d.%d", earth_gate, earth_line);
    snprintf(c.line3, sizeof(c.line3), "%s - %s",
             safe_str(gate_name), safe_str(gate_keyword));
    snprintf(c.detail, sizeof(c.detail),
             "Sun %d.%d | Earth %d.%d | %s (%s)",
             sun_gate, sun_line, earth_gate, earth_line,
             safe_str(gate_name), safe_str(gate_keyword));

    return c;
}

card_content_t card_format_astrology(int sun_sign, double sun_deg,
                                     int moon_sign, double moon_deg,
                                     int asc_sign, double asc_deg)
{
    card_content_t c;
    memset(&c, 0, sizeof(c));

    snprintf(c.title, sizeof(c.title), "Astrology");
    snprintf(c.line1, sizeof(c.line1), "Sun: %.1f %s",
             sun_deg, zodiac_name(sun_sign));
    snprintf(c.line2, sizeof(c.line2), "Moon: %.1f %s",
             moon_deg, zodiac_name(moon_sign));
    snprintf(c.line3, sizeof(c.line3), "ASC: %.1f %s",
             asc_deg, zodiac_name(asc_sign));
    snprintf(c.detail, sizeof(c.detail),
             "Sun %.1f %s | Moon %.1f %s | ASC %.1f %s",
             sun_deg, zodiac_name(sun_sign),
             moon_deg, zodiac_name(moon_sign),
             asc_deg, zodiac_name(asc_sign));

    return c;
}

card_content_t card_format_hebrew(int year, int month, int day,
                                  const char *month_name)
{
    card_content_t c;
    memset(&c, 0, sizeof(c));

    snprintf(c.title, sizeof(c.title), "Hebrew Calendar");
    snprintf(c.line1, sizeof(c.line1), "%d %s", day, safe_str(month_name));
    snprintf(c.line2, sizeof(c.line2), "Year %d", year);
    snprintf(c.line3, sizeof(c.line3), "Month %d of 13", month);
    snprintf(c.detail, sizeof(c.detail),
             "%d %s, %d (Month %d)", day, safe_str(month_name), year, month);

    return c;
}

card_content_t card_format_islamic(int year, int month, int day,
                                   const char *month_name)
{
    card_content_t c;
    memset(&c, 0, sizeof(c));

    snprintf(c.title, sizeof(c.title), "Islamic Calendar");
    snprintf(c.line1, sizeof(c.line1), "%d %s", day, safe_str(month_name));
    snprintf(c.line2, sizeof(c.line2), "Year %d AH", year);
    snprintf(c.line3, sizeof(c.line3), "Month %d of 12", month);
    snprintf(c.detail, sizeof(c.detail),
             "%d %s, %d AH", day, safe_str(month_name), year);

    return c;
}

card_content_t card_format_buddhist(int era_year, int uposatha_type,
                                    const char *uposatha_name,
                                    double moon_illumination)
{
    card_content_t c;
    memset(&c, 0, sizeof(c));

    snprintf(c.title, sizeof(c.title), "Buddhist");
    snprintf(c.line1, sizeof(c.line1), "BE %d", era_year);
    if (uposatha_type > 0) {
        snprintf(c.line2, sizeof(c.line2), "%s", safe_str(uposatha_name));
    } else {
        snprintf(c.line2, sizeof(c.line2), "No observance");
    }
    snprintf(c.line3, sizeof(c.line3), "Moon: %.0f%%",
             moon_illumination * 100.0);
    snprintf(c.detail, sizeof(c.detail),
             "BE %d | %s | Moon %.0f%%",
             era_year,
             uposatha_type > 0 ? safe_str(uposatha_name) : "No observance",
             moon_illumination * 100.0);

    return c;
}

card_content_t card_format_hindu(int nakshatra, const char *nakshatra_name,
                                 int tithi, int vara,
                                 const char *yoga_name)
{
    card_content_t c;
    memset(&c, 0, sizeof(c));

    (void)nakshatra;

    snprintf(c.title, sizeof(c.title), "Hindu Panchanga");
    snprintf(c.line1, sizeof(c.line1), "Nakshatra: %s",
             safe_str(nakshatra_name));
    snprintf(c.line2, sizeof(c.line2), "Tithi %d · Vara %d", tithi, vara);
    snprintf(c.line3, sizeof(c.line3), "Yoga: %s", safe_str(yoga_name));
    snprintf(c.detail, sizeof(c.detail),
             "%s | Tithi %d | Vara %d | %s",
             safe_str(nakshatra_name), tithi, vara, safe_str(yoga_name));

    return c;
}

card_content_t card_format_kabbalah(int sefirah, const char *sefirah_name,
                                    const char *sefirah_meaning,
                                    int path, const char *path_letter)
{
    card_content_t c;
    memset(&c, 0, sizeof(c));

    snprintf(c.title, sizeof(c.title), "Tree of Life");
    snprintf(c.line1, sizeof(c.line1), "%s", safe_str(sefirah_name));
    snprintf(c.line2, sizeof(c.line2), "%s", safe_str(sefirah_meaning));
    if (path >= 0) {
        snprintf(c.line3, sizeof(c.line3), "Path %d (%s)",
                 path, safe_str(path_letter));
    } else {
        c.line3[0] = '\0';
    }
    snprintf(c.detail, sizeof(c.detail),
             "Sefirah %d: %s (%s)", sefirah,
             safe_str(sefirah_name), safe_str(sefirah_meaning));

    return c;
}

card_content_t card_format_geology(const char *eon_name, const char *era_name,
                                   const char *period_name, const char *epoch_name)
{
    card_content_t c;
    memset(&c, 0, sizeof(c));

    snprintf(c.title, sizeof(c.title), "Geological Time");
    snprintf(c.line1, sizeof(c.line1), "%s Eon", safe_str(eon_name));
    snprintf(c.line2, sizeof(c.line2), "%s Era", safe_str(era_name));
    snprintf(c.line3, sizeof(c.line3), "%s · %s",
             safe_str(period_name), safe_str(epoch_name));
    snprintf(c.detail, sizeof(c.detail),
             "%s > %s > %s > %s",
             safe_str(eon_name), safe_str(era_name),
             safe_str(period_name), safe_str(epoch_name));

    return c;
}

static const char *WEEKDAY_NAMES[7] = {
    "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
};

static const char *weekday_name(int day_of_week)
{
    if (day_of_week < 0 || day_of_week > 6) return "?";
    return WEEKDAY_NAMES[day_of_week];
}

card_content_t card_format_gregorian(int year, int month, int day,
                                     int day_of_week, const char *season)
{
    card_content_t c;
    memset(&c, 0, sizeof(c));

    snprintf(c.title, sizeof(c.title), "Gregorian");
    snprintf(c.line1, sizeof(c.line1), "%s", weekday_name(day_of_week));
    snprintf(c.line2, sizeof(c.line2), "%d-%02d-%02d", year, month, day);
    if (season) {
        snprintf(c.line3, sizeof(c.line3), "%s", season);
    } else {
        c.line3[0] = '\0';
    }
    if (season) {
        snprintf(c.detail, sizeof(c.detail),
                 "%s, %d-%02d-%02d (%s)",
                 weekday_name(day_of_week), year, month, day, season);
    } else {
        snprintf(c.detail, sizeof(c.detail),
                 "%s, %d-%02d-%02d",
                 weekday_name(day_of_week), year, month, day);
    }

    return c;
}

card_content_t card_format_coptic(int year, int month, int day,
                                  const char *month_name)
{
    card_content_t c;
    memset(&c, 0, sizeof(c));

    snprintf(c.title, sizeof(c.title), "Coptic Calendar");
    snprintf(c.line1, sizeof(c.line1), "%d %s", day, safe_str(month_name));
    snprintf(c.line2, sizeof(c.line2), "AM %d", year);
    snprintf(c.line3, sizeof(c.line3), "Month %d", month);
    snprintf(c.detail, sizeof(c.detail),
             "%d %s, AM %d", day, safe_str(month_name), year);

    return c;
}

card_content_t card_format_ethiopian(int year, int month, int day,
                                     const char *month_name)
{
    card_content_t c;
    memset(&c, 0, sizeof(c));

    snprintf(c.title, sizeof(c.title), "Ethiopian Calendar");
    snprintf(c.line1, sizeof(c.line1), "%d %s", day, safe_str(month_name));
    snprintf(c.line2, sizeof(c.line2), "Year %d", year);
    snprintf(c.line3, sizeof(c.line3), "Month %d", month);
    snprintf(c.detail, sizeof(c.detail),
             "%d %s, %d", day, safe_str(month_name), year);

    return c;
}

card_content_t card_format_persian(int year, int month, int day,
                                   const char *month_name)
{
    card_content_t c;
    memset(&c, 0, sizeof(c));

    snprintf(c.title, sizeof(c.title), "Solar Hijri");
    snprintf(c.line1, sizeof(c.line1), "%d %s", day, safe_str(month_name));
    snprintf(c.line2, sizeof(c.line2), "Year %d SH", year);
    snprintf(c.line3, sizeof(c.line3), "Month %d", month);
    snprintf(c.detail, sizeof(c.detail),
             "%d %s, %d SH", day, safe_str(month_name), year);

    return c;
}

card_content_t card_format_thai(int be_year, int month, int day,
                                const char *month_name)
{
    card_content_t c;
    memset(&c, 0, sizeof(c));

    snprintf(c.title, sizeof(c.title), "Thai Calendar");
    snprintf(c.line1, sizeof(c.line1), "%d %s", day, safe_str(month_name));
    snprintf(c.line2, sizeof(c.line2), "BE %d", be_year);
    snprintf(c.line3, sizeof(c.line3), "Month %d", month);
    snprintf(c.detail, sizeof(c.detail),
             "%d %s, BE %d", day, safe_str(month_name), be_year);

    return c;
}
