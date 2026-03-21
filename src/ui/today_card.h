/* today_card.h — Universal card content dispatcher.
 *
 * Given a system_id and Julian Day (+ Sun/Moon longitudes for
 * astronomical systems), computes the system's current state and
 * produces a formatted card_content_t.
 *
 * Bridges the 20 knowledge system computation modules to the
 * unified card_content_t display format.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#ifndef TIME_TODAY_CARD_H
#define TIME_TODAY_CARD_H

#include "card_data.h"                          /* card_content_t */
#include "../systems/unified/today_summary.h"   /* ts_system_t */

/* Compute card content for any knowledge system at a given moment.
 * system_id: ts_system_t value (0-20)
 * jd: Julian Day number
 * sun_lon_deg: Sun ecliptic longitude (degrees, 0-360)
 * moon_lon_deg: Moon ecliptic longitude (degrees, 0-360)
 * observer_lat: observer latitude (degrees, -90 to 90)
 * observer_lon: observer longitude (degrees, -180 to 180)
 * Returns formatted card_content_t. Unknown systems return empty card. */
card_content_t today_card_for_system(int system_id, double jd,
                                      double sun_lon_deg,
                                      double moon_lon_deg,
                                      double observer_lat,
                                      double observer_lon);

/* Check if a system_id has card content support.
 * Returns 1 if supported, 0 if not. */
int today_card_supported(int system_id);

/* Count of supported systems. */
int today_card_supported_count(void);

#endif /* TIME_TODAY_CARD_H */
