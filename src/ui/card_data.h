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

/* Format Hebrew calendar date for card display. */
card_content_t card_format_hebrew(int year, int month, int day,
                                  const char *month_name);

/* Format Islamic (Hijri) date for card display. */
card_content_t card_format_islamic(int year, int month, int day,
                                   const char *month_name);

/* Format Buddhist observance for card display.
 * uposatha_type: 0=none, 1=new_moon, 2=full_moon, 3=first_quarter, 4=last_quarter
 * era_year: Buddhist Era year */
card_content_t card_format_buddhist(int era_year, int uposatha_type,
                                    const char *uposatha_name,
                                    double moon_illumination);

/* Format Hindu/Vedic panchanga for card display.
 * nakshatra: 0-26, tithi: 0-29, vara: 0-6 */
card_content_t card_format_hindu(int nakshatra, const char *nakshatra_name,
                                 int tithi, int vara,
                                 const char *yoga_name);

/* Format Kabbalah Tree of Life for card display.
 * sefirah: 0-9, path: 0-21 (or -1 for none) */
card_content_t card_format_kabbalah(int sefirah, const char *sefirah_name,
                                    const char *sefirah_meaning,
                                    int path, const char *path_letter);

/* Format geological time for card display. */
card_content_t card_format_geology(const char *eon_name, const char *era_name,
                                   const char *period_name, const char *epoch_name);

/* Format Gregorian date for card display.
 * day_of_week: 0=Mon through 6=Sun */
card_content_t card_format_gregorian(int year, int month, int day,
                                     int day_of_week, const char *season);

/* Format Coptic date for card display. */
card_content_t card_format_coptic(int year, int month, int day,
                                  const char *month_name);

/* Format Ethiopian date for card display. */
card_content_t card_format_ethiopian(int year, int month, int day,
                                     const char *month_name);

/* Format Persian (Solar Hijri) date for card display. */
card_content_t card_format_persian(int year, int month, int day,
                                   const char *month_name);

/* Format Thai date for card display.
 * be_year: Buddhist Era year used in Thailand */
card_content_t card_format_thai(int be_year, int month, int day,
                                const char *month_name);

#endif
