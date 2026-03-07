/* catalog_ingest.h — Pure CSV/TSV star catalog parser.
 *
 * Parses delimited text data into catalog_star_t entries with
 * validation, sorting, filtering, and tier classification.
 * No I/O, no heap allocation, no globals. */

#ifndef TIME_CATALOG_INGEST_H
#define TIME_CATALOG_INGEST_H

#include <stddef.h>

#define CATALOG_NAME_MAX 32
#define CATALOG_SPECTRAL_MAX 8
#define CATALOG_CONSTELLATION_MAX 4

#define CATALOG_TIER_0_MAG 1.5
#define CATALOG_TIER_1_MAG 3.0
#define CATALOG_TIER_2_MAG 4.5
#define CATALOG_TIER_3_MAG 6.5
#define CATALOG_TIER_COUNT 4

/* A single star entry parsed from catalog data. */
typedef struct {
    double ra_hours;
    double dec_degrees;
    double v_magnitude;
    double bv_color;
    char name[CATALOG_NAME_MAX];
    char spectral[CATALOG_SPECTRAL_MAX];
    char constellation[CATALOG_CONSTELLATION_MAX];
} catalog_star_t;

/* Result of parsing a single line. */
typedef struct {
    int success;
    int field_count;
    int validation_error;
} catalog_parse_result_t;

/* Summary of parsing an entire buffer. */
typedef struct {
    int total_lines;
    int parsed_ok;
    int validation_errors;
    int parse_errors;
    int skipped_header;
} catalog_parse_summary_t;

/* Per-tier star counts. */
typedef struct {
    int tier_counts[CATALOG_TIER_COUNT];
    int total;
} catalog_tier_stats_t;

/* Parse a single delimited line into a star entry.
 * Field order: ra_hours, dec_degrees, v_magnitude, bv_color, name, spectral, constellation.
 * Returns result with success=1 if parsing and validation both pass. */
catalog_parse_result_t catalog_parse_line(const char *line, char delimiter,
                                          catalog_star_t *out);

/* Validate all fields of a star entry. Returns 1 if valid, 0 otherwise. */
int catalog_validate(const catalog_star_t *entry);

/* Validate right ascension: [0, 24). Returns 1 if valid. */
int catalog_validate_ra(double ra_hours);

/* Validate declination: [-90, 90]. Returns 1 if valid. */
int catalog_validate_dec(double dec_degrees);

/* Validate visual magnitude: [-2, 20]. Returns 1 if valid. */
int catalog_validate_mag(double v_magnitude);

/* Parse a full text buffer of delimited lines into an array of star entries.
 * Returns the number of successfully parsed entries.
 * If has_header is nonzero, the first line is skipped.
 * summary is filled with parse statistics (may be NULL). */
int catalog_parse_buffer(const char *data, char delimiter, int has_header,
                         catalog_star_t *out, int max_count,
                         catalog_parse_summary_t *summary);

/* Sort stars by visual magnitude (brightest first). Insertion sort. */
void catalog_sort_by_magnitude(catalog_star_t *stars, int count);

/* Copy stars with v_magnitude <= max_mag from src to dst.
 * Returns number of stars copied. Stops at dst_max. */
int catalog_filter_by_magnitude(const catalog_star_t *src, int src_count,
                                double max_mag, catalog_star_t *dst,
                                int dst_max);

/* Return brightness tier index (0-3) for a given magnitude.
 * Tier 0: < 1.5, Tier 1: < 3.0, Tier 2: < 4.5, Tier 3: >= 4.5. */
int catalog_brightness_tier(double v_magnitude);

/* Compute per-tier star counts for an array. */
catalog_tier_stats_t catalog_tier_stats(const catalog_star_t *stars, int count);

/* Returns 1 if the star has no name (name[0] == '\0'). */
int catalog_is_unnamed(const catalog_star_t *entry);

/* Count how many stars in the array have a non-empty name. */
int catalog_count_named(const catalog_star_t *stars, int count);

#endif /* TIME_CATALOG_INGEST_H */
