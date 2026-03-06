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
