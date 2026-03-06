#ifndef TIME_DEEP_SKY_H
#define TIME_DEEP_SKY_H

/* Deep sky object type classification. */
typedef enum {
    DSO_OPEN_CLUSTER = 0,
    DSO_GLOBULAR_CLUSTER,
    DSO_EMISSION_NEBULA,
    DSO_REFLECTION_NEBULA,
    DSO_PLANETARY_NEBULA,
    DSO_DARK_NEBULA,
    DSO_SUPERNOVA_REMNANT,
    DSO_SPIRAL_GALAXY,
    DSO_ELLIPTICAL_GALAXY,
    DSO_IRREGULAR_GALAXY,
    DSO_TYPE_COUNT
} dso_type_t;

/* RGB color hint for rendering. */
typedef struct {
    float r, g, b;
} dso_color_t;

/* Deep sky object catalog entry. */
typedef struct {
    const char *designation;    /* "M1", "M31", etc. */
    const char *name;           /* Common name or NULL */
    double ra_hours;            /* Right ascension (hours, 0-24) */
    double dec_deg;             /* Declination (degrees, -90 to +90) */
    dso_type_t type;
    double apparent_mag;        /* Integrated apparent magnitude */
    double angular_size_arcmin; /* Angular diameter in arcminutes */
    dso_color_t color;          /* Visual color hint for rendering */
} dso_entry_t;

/* Number of objects in the catalog. */
int dso_catalog_count(void);

/* Get entry by index (0-based). Returns sentinel with null designation if invalid. */
dso_entry_t dso_catalog_get(int index);

/* Human-readable name for a DSO type. Returns "Unknown" if invalid. */
const char *dso_type_name(dso_type_t type);

/* Default rendering color for a DSO type. Returns black if invalid. */
dso_color_t dso_type_color(dso_type_t type);

/* Count entries of a given type. */
int dso_count_by_type(dso_type_t type);

/* Find a Messier object by number (1-110). Returns index or -1. */
int dso_find_messier(int messier_number);

/* Find by common name (case-insensitive substring). Returns first match index or -1. */
int dso_find_by_name(const char *name);

#endif
