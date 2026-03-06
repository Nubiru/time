#ifndef TIME_LOCATION_PRESETS_H
#define TIME_LOCATION_PRESETS_H

#include <stddef.h>

typedef struct {
    const char *name;       /* "Greenwich", "New York", etc. */
    const char *country;    /* "UK", "USA", etc. */
    double latitude;        /* degrees, positive north */
    double longitude;       /* degrees, positive east */
    double utc_offset;      /* hours from UTC (e.g., -5.0 for EST) */
} location_preset_t;

/* Total number of preset locations. */
int location_preset_count(void);

/* Get a preset by index (0 to count-1). Returns NULL name for invalid index. */
location_preset_t location_preset_get(int index);

/* Find a preset by name (case-insensitive substring match).
 * Returns index, or -1 if not found. */
int location_preset_find(const char *query);

/* Get the default location index (Greenwich). */
int location_preset_default(void);

/* Format coordinates as string: "51.48°N, 0.00°W". */
void location_format_coords(double latitude, double longitude, char *buf, size_t sz);

/* Format location as full string: "Greenwich, UK (51.48°N, 0.00°W)". */
void location_format_full(location_preset_t loc, char *buf, size_t sz);

#endif
