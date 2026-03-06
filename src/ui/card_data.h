#ifndef TIME_CARD_DATA_H
#define TIME_CARD_DATA_H

typedef struct {
    char title[64];
    char line1[128];
    char line2[128];
    char line3[128];
    char detail[256];
} card_content_t;

/* Format Tzolkin kin data for card display.
 * seal: 0-19, tone: 1-13, kin: 1-260, seal_name: e.g. "Hand" */
card_content_t card_format_tzolkin(int seal, int tone, int kin,
                                   const char *seal_name);

/* Format I Ching hexagram for card display.
 * number: 1-64, name: hexagram name,
 * upper_tri/lower_tri: trigram names,
 * lines: 6-element array of 0(yin) or 1(yang), bottom-first */
card_content_t card_format_iching(int number, const char *name,
                                  const char *upper_tri, const char *lower_tri,
                                  const int lines[6]);

/* Format Chinese year for card display. */
card_content_t card_format_chinese(const char *animal_symbol,
                                   const char *element_name,
                                   const char *animal_name,
                                   const char *polarity_name,
                                   int stem, int branch);

/* Format Human Design gate for card display. */
card_content_t card_format_human_design(int sun_gate, int sun_line,
                                        int earth_gate, int earth_line,
                                        const char *gate_name,
                                        const char *gate_keyword);

/* Format astrology summary for card display.
 * sign indices 0-11, degrees within sign. */
card_content_t card_format_astrology(int sun_sign, double sun_deg,
                                     int moon_sign, double moon_deg,
                                     int asc_sign, double asc_deg);

#endif
