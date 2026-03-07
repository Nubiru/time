/* catalog_ingest.c — Pure CSV/TSV star catalog parser implementation. */

#include "catalog_ingest.h"

#include <stdlib.h>
#include <string.h>

/* --- Validation --- */

int catalog_validate_ra(double ra_hours)
{
    return (ra_hours >= 0.0 && ra_hours < 24.0) ? 1 : 0;
}

int catalog_validate_dec(double dec_degrees)
{
    return (dec_degrees >= -90.0 && dec_degrees <= 90.0) ? 1 : 0;
}

int catalog_validate_mag(double v_magnitude)
{
    return (v_magnitude >= -2.0 && v_magnitude <= 20.0) ? 1 : 0;
}

int catalog_validate(const catalog_star_t *entry)
{
    if (!entry) {
        return 0;
    }
    if (!catalog_validate_ra(entry->ra_hours)) {
        return 0;
    }
    if (!catalog_validate_dec(entry->dec_degrees)) {
        return 0;
    }
    if (!catalog_validate_mag(entry->v_magnitude)) {
        return 0;
    }
    if (entry->bv_color < -0.5 || entry->bv_color > 3.0) {
        return 0;
    }
    return 1;
}

/* --- Internal helpers --- */

/* Copy at most dst_size-1 chars from src into dst, null-terminate.
 * src_len is the number of chars to consider from src. */
static void safe_copy(char *dst, size_t dst_size, const char *src, size_t src_len)
{
    size_t copy_len = src_len;
    if (copy_len >= dst_size) {
        copy_len = dst_size - 1;
    }
    memcpy(dst, src, copy_len);
    dst[copy_len] = '\0';
}

/* Split a line on delimiter, return pointers to field starts and field lengths.
 * Returns the number of fields found. */
static int split_fields(const char *line, char delimiter,
                        const char *starts[], size_t lengths[], int max_fields)
{
    int count = 0;
    const char *p = line;

    while (count < max_fields) {
        starts[count] = p;
        const char *delim = p;
        while (*delim != '\0' && *delim != delimiter && *delim != '\n' && *delim != '\r') {
            delim++;
        }
        lengths[count] = (size_t)(delim - p);
        count++;

        if (*delim == delimiter) {
            p = delim + 1;
        } else {
            break;
        }
    }
    return count;
}

/* --- Parsing --- */

catalog_parse_result_t catalog_parse_line(const char *line, char delimiter,
                                          catalog_star_t *out)
{
    catalog_parse_result_t result = { .success = 0, .field_count = 0,
                                      .validation_error = 0 };

    if (!line || !out || line[0] == '\0') {
        return result;
    }

    const char *starts[7];
    size_t lengths[7];
    int field_count = split_fields(line, delimiter, starts, lengths, 7);
    result.field_count = field_count;

    if (field_count < 7) {
        return result;
    }

    /* Parse numeric fields using strtod with length-limited copies */
    char buf[64];

    /* ra_hours */
    safe_copy(buf, sizeof(buf), starts[0], lengths[0]);
    out->ra_hours = strtod(buf, NULL);

    /* dec_degrees */
    safe_copy(buf, sizeof(buf), starts[1], lengths[1]);
    out->dec_degrees = strtod(buf, NULL);

    /* v_magnitude */
    safe_copy(buf, sizeof(buf), starts[2], lengths[2]);
    out->v_magnitude = strtod(buf, NULL);

    /* bv_color */
    safe_copy(buf, sizeof(buf), starts[3], lengths[3]);
    out->bv_color = strtod(buf, NULL);

    /* String fields */
    safe_copy(out->name, CATALOG_NAME_MAX, starts[4], lengths[4]);
    safe_copy(out->spectral, CATALOG_SPECTRAL_MAX, starts[5], lengths[5]);
    safe_copy(out->constellation, CATALOG_CONSTELLATION_MAX, starts[6], lengths[6]);

    /* Validate */
    if (!catalog_validate(out)) {
        result.validation_error = 1;
        return result;
    }

    result.success = 1;
    return result;
}

int catalog_parse_buffer(const char *data, char delimiter, int has_header,
                         catalog_star_t *out, int max_count,
                         catalog_parse_summary_t *summary)
{
    catalog_parse_summary_t local_summary = {
        .total_lines = 0, .parsed_ok = 0,
        .validation_errors = 0, .parse_errors = 0,
        .skipped_header = 0
    };

    if (!data || !out || max_count <= 0) {
        if (summary) {
            *summary = local_summary;
        }
        return 0;
    }

    int count = 0;
    int first_line = 1;
    const char *p = data;

    while (*p != '\0' && count < max_count) {
        /* Find end of current line */
        const char *eol = p;
        while (*eol != '\0' && *eol != '\n') {
            eol++;
        }

        size_t line_len = (size_t)(eol - p);

        /* Skip empty lines */
        if (line_len == 0) {
            p = (*eol == '\n') ? eol + 1 : eol;
            continue;
        }

        /* Skip header if requested */
        if (first_line && has_header) {
            local_summary.skipped_header = 1;
            first_line = 0;
            p = (*eol == '\n') ? eol + 1 : eol;
            continue;
        }
        first_line = 0;

        local_summary.total_lines++;

        /* Make a temporary copy for parsing (line may not be null-terminated at eol) */
        char line_buf[512];
        if (line_len >= sizeof(line_buf)) {
            line_len = sizeof(line_buf) - 1;
        }
        memcpy(line_buf, p, line_len);
        line_buf[line_len] = '\0';

        catalog_parse_result_t r = catalog_parse_line(line_buf, delimiter, &out[count]);
        if (r.success) {
            local_summary.parsed_ok++;
            count++;
        } else if (r.validation_error) {
            local_summary.validation_errors++;
        } else {
            local_summary.parse_errors++;
        }

        p = (*eol == '\n') ? eol + 1 : eol;
    }

    if (summary) {
        *summary = local_summary;
    }
    return count;
}

/* --- Sorting --- */

void catalog_sort_by_magnitude(catalog_star_t *stars, int count)
{
    if (!stars || count <= 1) {
        return;
    }

    /* Insertion sort — stable, simple, no function pointers */
    for (int i = 1; i < count; i++) {
        catalog_star_t key = stars[i];
        int j = i - 1;
        while (j >= 0 && stars[j].v_magnitude > key.v_magnitude) {
            stars[j + 1] = stars[j];
            j--;
        }
        stars[j + 1] = key;
    }
}

/* --- Filtering --- */

int catalog_filter_by_magnitude(const catalog_star_t *src, int src_count,
                                double max_mag, catalog_star_t *dst,
                                int dst_max)
{
    if (!src || !dst || src_count <= 0 || dst_max <= 0) {
        return 0;
    }

    int count = 0;
    for (int i = 0; i < src_count && count < dst_max; i++) {
        if (src[i].v_magnitude <= max_mag) {
            dst[count] = src[i];
            count++;
        }
    }
    return count;
}

/* --- Tier classification --- */

int catalog_brightness_tier(double v_magnitude)
{
    if (v_magnitude < CATALOG_TIER_0_MAG) {
        return 0;
    }
    if (v_magnitude < CATALOG_TIER_1_MAG) {
        return 1;
    }
    if (v_magnitude < CATALOG_TIER_2_MAG) {
        return 2;
    }
    return 3;
}

catalog_tier_stats_t catalog_tier_stats(const catalog_star_t *stars, int count)
{
    catalog_tier_stats_t stats;
    memset(&stats, 0, sizeof(stats));

    if (!stars || count <= 0) {
        return stats;
    }

    stats.total = count;
    for (int i = 0; i < count; i++) {
        int tier = catalog_brightness_tier(stars[i].v_magnitude);
        stats.tier_counts[tier]++;
    }
    return stats;
}

/* --- Naming --- */

int catalog_is_unnamed(const catalog_star_t *entry)
{
    if (!entry) {
        return 1;
    }
    return (entry->name[0] == '\0') ? 1 : 0;
}

int catalog_count_named(const catalog_star_t *stars, int count)
{
    if (!stars || count <= 0) {
        return 0;
    }

    int named = 0;
    for (int i = 0; i < count; i++) {
        if (!catalog_is_unnamed(&stars[i])) {
            named++;
        }
    }
    return named;
}
