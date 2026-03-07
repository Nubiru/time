# Task: Extended Star Catalog

**Agent**: ALPHA
**Roadmap Reference**: Track 9.1 — "Agent: Extended Star Catalog (Agent A)"
**Date**: 2026-03-06
**Status**: COMPLETE

## Goal

Expanded star catalog with tier-based LOD (Level of Detail) access. Extends the project from 195 stars to 500+ stars covering Tiers 0-2 completely. The module is designed for future expansion to 9110 stars (BSC5 full coverage). Each star has RA, Dec, V magnitude, B-V color index, and optional name. Provides tier-based access for progressive rendering at different zoom levels.

## READ FIRST

- `src/render/star_catalog.h` — existing 195-star catalog (star_entry_t, coordinate conversion)
- `src/render/catalog_ingest.h` — tier thresholds (CATALOG_TIER_0_MAG etc.)

## Files to Create

- `src/render/star_catalog_ext.h`
- `src/render/star_catalog_ext.c`
- `tests/render/test_star_catalog_ext.c`

## API

```c
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
```

## Data Requirements

The catalog should include AT MINIMUM:

### Tier 0 (V < 1.5) — ALL ~21 brightest stars:
Sirius (-1.46), Canopus (-0.74), Alpha Centauri (-0.27), Arcturus (-0.05), Vega (0.03), Capella (0.08), Rigel (0.13), Procyon (0.34), Achernar (0.46), Betelgeuse (0.50), Hadar (0.61), Altair (0.77), Acrux (0.76), Aldebaran (0.85), Antares (0.96), Spica (1.04), Pollux (1.14), Fomalhaut (1.16), Deneb (1.25), Mimosa (1.25), Regulus (1.35)

### Tier 1 (V 1.5-3.0) — Major named stars including:
Adhara, Castor, Gacrux, Shaula, Bellatrix, Elnath, Miaplacidus, Alnilam, Alnitak, Alnair, Alioth, Dubhe, Mirfak, Wezen, Sargas, Kaus Australis, Avior, Alkaid, Menkalinan, Atria, Alhena, Peacock, Alsephina, Mirzam, Alphard, Polaris, Hamal, Algieba, Diphda, Mizar, Nunki, Menkent, Mirach, Alpheratz, Rasalhague, Kochab, Saiph, Denebola, Algol, Tiaki, Muhlifain, Naos, Aspidiske, Suhail, Alphecca, Mintaka, Sadr, Eltanin, Schedar, Caph, Ruchbah, and more.

### Tier 2 (V 3.0-4.5) — Fill out major constellation patterns:
Stars needed to complete constellation stick figures for all 40 constellations already in the constellation module.

### Target: >= 300 stars, ideally 400-500

### Data accuracy:
- RA: to 0.01 hour precision (~0.15 degree)
- Dec: to 0.1 degree precision
- V mag: to 0.01 magnitude
- B-V: to 0.01 index

## DONE WHEN

- [ ] >= 300 stars with correct RA, Dec, magnitude, B-V
- [ ] All 21 Tier 0 stars present with names
- [ ] >= 80 Tier 1 stars with names
- [ ] Tier-based access functions working
- [ ] Stars sorted by magnitude (brightest first)
- [ ] Name lookup (case-insensitive)
- [ ] Regional query (RA/Dec box)
- [ ] >= 30 tests
- [ ] All tests pass with zero warnings
- [ ] Purity: no malloc, no globals, no side effects
- [ ] Compiles: `gcc -Wall -Wextra -Werror -std=c11 -pedantic`

## Constraints

- C11, `-Wall -Wextra -Werror -std=c11 -pedantic`
- No malloc, no globals, no side effects
- `#include <string.h>` for strcmp/strcasecmp
- `#include <stddef.h>` for NULL
- Stars stored as `static const` arrays (compiled into binary)
- Standalone module (does NOT depend on star_catalog.h)
- Data sorted by magnitude at compile time
