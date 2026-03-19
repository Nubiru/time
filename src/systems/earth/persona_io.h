/* persona_io.h -- Load/save persona data via storage bridge.
 *
 * Higher-level integration: serializes persona structs via persona_persist,
 * then reads/writes through storage_bridge. This is what main.c calls
 * on startup and on data changes.
 *
 * NOTE: This module IS stateful -- it calls storage_bridge which has
 * module-static state. This is the imperative shell wrapping pure cores. */

#ifndef TIME_PERSONA_IO_H
#define TIME_PERSONA_IO_H

#include "user_prefs.h"
#include "interest_profile.h"
#include "personal_space.h"
#include "privacy_framework.h"

#define PIO_SCHEMA_VERSION 1
#define PIO_EXPORT_BUF_SIZE 16384

/* Load all persona data from storage into C structs.
 * Returns 1 if returning user (birth data found), 0 if new visitor. */
int pio_load_all(up_prefs_t *prefs, ip_profile_t *interests,
                 ps_space_t *space, pf_settings_t *privacy,
                 int *birth_year, int *birth_month, int *birth_day,
                 int *birth_has_loc, double *birth_lat, double *birth_lon);

/* Save preferences to storage. */
void pio_save_prefs(const up_prefs_t *prefs);

/* Save birth data to storage. */
void pio_save_birth(int year, int month, int day,
                    int has_location, double lat, double lon);

/* Save interest profile to storage. */
void pio_save_interests(const ip_profile_t *profile);

/* Save personal space to storage. */
void pio_save_space(const ps_space_t *space);

/* Save privacy settings to storage. */
void pio_save_privacy(const pf_settings_t *settings);

/* Export all persona data as JSON and trigger file download. */
void pio_export(void);

/* Delete all persona data from storage. */
void pio_delete_all(void);

/* Check schema version and run migration if needed. */
void pio_check_migration(void);

#endif /* TIME_PERSONA_IO_H */
