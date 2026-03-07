#ifndef TIME_STAR_CATALOG_EXT_H
#define TIME_STAR_CATALOG_EXT_H

#define EXT_TIER_COUNT 4
#define EXT_TIER_0_MAG 1.5f   /* ~21 brightest */
#define EXT_TIER_1_MAG 3.0f   /* ~170 stars */
#define EXT_TIER_2_MAG 4.5f   /* ~500+ stars */
#define EXT_TIER_3_MAG 6.5f   /* ~9000 future */

/* Extended star entry */
typedef struct {
    float ra_hours;   /* Right Ascension 0-24h */
    float dec_deg;    /* Declination -90 to +90 */
    float v_mag;      /* Visual magnitude */
    float bv;         /* B-V color index */
} ext_star_t;

/* Total number of stars in the extended catalog. */
int ext_star_count(void);

/* Get star by index (sorted by magnitude, brightest first). */
ext_star_t ext_star_get(int index);

/* Get star name by index. Returns NULL if unnamed. */
const char *ext_star_name(int index);

/* Get number of stars in a specific tier (0-3). */
int ext_star_tier_count(int tier);

/* Get starting index for a tier in the sorted array. */
int ext_star_tier_start(int tier);

/* Get tier index (0-3) for a given magnitude. */
int ext_star_tier_for_mag(float v_mag);

/* Count of named stars in the catalog. */
int ext_star_named_count(void);

/* Find star index by name (case-insensitive). Returns -1 if not found. */
int ext_star_find(const char *name);

/* Get stars within a rectangular RA/Dec region.
 * Writes matching indices to out[], returns count.
 * ra_min/ra_max in hours (handles wrap at 0/24).
 * dec_min/dec_max in degrees. */
int ext_star_in_region(float ra_min, float ra_max,
                       float dec_min, float dec_max,
                       int *out, int max_out);

#endif /* TIME_STAR_CATALOG_EXT_H */
