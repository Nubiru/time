#ifndef TIME_CONSTELLATION_H
#define TIME_CONSTELLATION_H

#define CONSTELLATION_MAX_LINES 12

/* A line segment connecting two stars by catalog index. */
typedef struct {
    int star_a;
    int star_b;
} cline_t;

/* A constellation: named set of line segments with IAU abbreviation. */
typedef struct {
    const char *name;
    const char *abbr;
    cline_t lines[CONSTELLATION_MAX_LINES];
    int line_count;
} constellation_t;

/* Number of constellations in the database. */
int constellation_count(void);

/* Get constellation by index. Returns empty (name=NULL, line_count=0) if invalid. */
constellation_t constellation_get(int index);

/* Find constellation by name. Returns index or -1 if not found. */
int constellation_find(const char *name);

/* Get IAU abbreviation by index. Returns NULL if invalid. */
const char *constellation_abbr(int index);

/* Total number of line segments across all constellations. */
int constellation_total_lines(void);

/* Map ecliptic longitude (degrees) to zodiac sign index (0=Aries..11=Pisces).
 * Handles wrapping and negative values. */
int constellation_zodiac_from_longitude(float longitude_deg);

#endif
