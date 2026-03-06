#ifndef TIME_ASPECTS_H
#define TIME_ASPECTS_H

/* Ptolemaic aspect types. */
typedef enum {
    ASPECT_NONE = -1,
    ASPECT_CONJUNCTION = 0,
    ASPECT_SEXTILE = 1,
    ASPECT_SQUARE = 2,
    ASPECT_TRINE = 3,
    ASPECT_OPPOSITION = 4
} aspect_type_t;

/* A single aspect between two planets. */
typedef struct {
    int planet_a;         /* index 0-7 */
    int planet_b;         /* index 0-7 */
    aspect_type_t type;
    double orb;           /* actual deviation from exact angle (degrees) */
} aspect_t;

/* All aspects found among 8 planets (max 28 = 8 choose 2). */
typedef struct {
    aspect_t aspects[28];
    int count;
} aspect_list_t;

/* Shortest arc between two ecliptic longitudes (0-180 degrees). */
double angular_separation(double lon_a, double lon_b);

/* Which aspect (if any) matches the given separation within max_orb. */
aspect_type_t aspect_check(double separation, double max_orb);

/* Deviation from exact aspect angle (degrees). */
double aspect_orb(double separation, aspect_type_t type);

/* Check all 28 planet pairs, return list of aspects found.
 * longitudes: array of 8 ecliptic longitudes (degrees).
 * max_orb: maximum orb to consider (degrees). */
aspect_list_t aspects_find_all(const double longitudes[8], double max_orb);

/* Human-readable aspect name. */
const char *aspect_name(aspect_type_t type);

/* Unicode aspect symbol. */
const char *aspect_symbol(aspect_type_t type);

#endif
