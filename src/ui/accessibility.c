#include "accessibility.h"

#include <stdio.h>
#include <string.h>

/* Month names for datetime formatting */
static const char *const MONTH_NAMES[12] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};

/* Event type display names */
static const char *const EVENT_NAMES[] = {
    "None",
    "Scale Change",
    "System Focus",
    "Festival",
    "Aspect",
    "Time Jump",
    "View Change"
};

int aria_planet_desc(const aria_planet_t *planet, char *out, int max_len)
{
    if (!planet || !out || max_len <= 0) return 0;

    const char *name = planet->name ? planet->name : "unknown";
    const char *sign = planet->sign ? planet->sign : "unknown";

    int n;
    if (planet->is_retrograde) {
        n = snprintf(out, (size_t)max_len,
                     "%s in %s at %.1f degrees (retrograde)",
                     name, sign, planet->degree);
    } else {
        n = snprintf(out, (size_t)max_len,
                     "%s in %s at %.1f degrees",
                     name, sign, planet->degree);
    }

    /* snprintf returns what it would have written; clamp to actual */
    if (n < 0) return 0;
    if (n >= max_len) n = max_len - 1;
    return n;
}

int aria_sky_desc(const aria_planet_t *planets, int count,
                  char *out, int max_len)
{
    if (!planets || !out || max_len <= 0 || count <= 0) return 0;

    int total = 0;
    for (int i = 0; i < count; i++) {
        if (i > 0 && total < max_len - 1) {
            int sep = snprintf(out + total, (size_t)(max_len - total), ", ");
            if (sep > 0) {
                if (total + sep >= max_len) {
                    total = max_len - 1;
                    break;
                }
                total += sep;
            }
        }

        if (total >= max_len - 1) break;

        int remaining = max_len - total;
        const char *name = planets[i].name ? planets[i].name : "unknown";
        const char *sign = planets[i].sign ? planets[i].sign : "unknown";

        int n;
        if (planets[i].is_retrograde) {
            n = snprintf(out + total, (size_t)remaining,
                         "%s in %s at %.1f degrees (retrograde)",
                         name, sign, planets[i].degree);
        } else {
            n = snprintf(out + total, (size_t)remaining,
                         "%s in %s at %.1f degrees",
                         name, sign, planets[i].degree);
        }

        if (n < 0) break;
        if (total + n >= max_len) {
            total = max_len - 1;
            break;
        }
        total += n;
    }

    return total;
}

int aria_datetime_desc(int year, int month, int day, int hour, int minute,
                       char *out, int max_len)
{
    if (!out || max_len <= 0) return 0;

    const char *month_name;
    if (month >= 1 && month <= 12) {
        month_name = MONTH_NAMES[month - 1];
    } else {
        month_name = "Unknown";
    }

    int n = snprintf(out, (size_t)max_len,
                     "%s %d, %d at %d:%02d UTC",
                     month_name, day, year, hour, minute);

    if (n < 0) return 0;
    if (n >= max_len) n = max_len - 1;
    return n;
}

int aria_calendar_desc(const char *system_name, const char *date_str,
                       char *out, int max_len)
{
    if (!out || max_len <= 0) return 0;

    const char *sys = system_name ? system_name : "unknown";
    const char *date = date_str ? date_str : "unknown";

    int n = snprintf(out, (size_t)max_len,
                     "%s calendar: %s", sys, date);

    if (n < 0) return 0;
    if (n >= max_len) n = max_len - 1;
    return n;
}

int aria_festival_desc(const char *system_name, const char *festival_name,
                       char *out, int max_len)
{
    if (!out || max_len <= 0) return 0;

    const char *sys = system_name ? system_name : "unknown";
    const char *fest = festival_name ? festival_name : "unknown";

    int n = snprintf(out, (size_t)max_len,
                     "Festival alert: %s (%s)", fest, sys);

    if (n < 0) return 0;
    if (n >= max_len) n = max_len - 1;
    return n;
}

int aria_scale_desc(const char *scale_name, char *out, int max_len)
{
    if (!out || max_len <= 0) return 0;

    const char *name = scale_name ? scale_name : "unknown";

    int n = snprintf(out, (size_t)max_len,
                     "Zoom level: %s scale", name);

    if (n < 0) return 0;
    if (n >= max_len) n = max_len - 1;
    return n;
}

int aria_view_desc(const char *view_name, char *out, int max_len)
{
    if (!out || max_len <= 0) return 0;

    const char *name = view_name ? view_name : "unknown";

    int n = snprintf(out, (size_t)max_len,
                     "View changed to %s view", name);

    if (n < 0) return 0;
    if (n >= max_len) n = max_len - 1;
    return n;
}

int aria_event_desc(aria_event_t event, const char *detail,
                    char *out, int max_len)
{
    if (!out || max_len <= 0) return 0;
    if (event == ARIA_EVENT_NONE || event >= ARIA_EVENT_COUNT) return 0;

    const char *name = EVENT_NAMES[event];
    const char *det = detail ? detail : "";

    int n;
    if (detail && strlen(detail) > 0) {
        n = snprintf(out, (size_t)max_len, "%s: %s", name, det);
    } else {
        n = snprintf(out, (size_t)max_len, "%s", name);
    }

    if (n < 0) return 0;
    if (n >= max_len) n = max_len - 1;
    return n;
}

const char *aria_event_name(aria_event_t event)
{
    if (event >= 0 && event < ARIA_EVENT_COUNT) {
        return EVENT_NAMES[event];
    }
    return "Unknown";
}

int aria_retrograde_desc(const char *planet_name, bool is_retrograde,
                         char *out, int max_len)
{
    if (!out || max_len <= 0) return 0;

    const char *name = planet_name ? planet_name : "unknown";
    const char *status = is_retrograde ? "retrograde" : "direct";

    int n = snprintf(out, (size_t)max_len, "%s is %s", name, status);

    if (n < 0) return 0;
    if (n >= max_len) n = max_len - 1;
    return n;
}

int aria_aspect_desc(const char *planet_a, const char *planet_b,
                     const char *aspect_name, double separation_deg,
                     char *out, int max_len)
{
    if (!out || max_len <= 0) return 0;

    const char *a = planet_a ? planet_a : "unknown";
    const char *b = planet_b ? planet_b : "unknown";
    const char *aspect = aspect_name ? aspect_name : "unknown aspect";

    int n = snprintf(out, (size_t)max_len,
                     "%s and %s in %s at %.1f degrees separation",
                     a, b, aspect, separation_deg);

    if (n < 0) return 0;
    if (n >= max_len) n = max_len - 1;
    return n;
}

int aria_event_count(void)
{
    return ARIA_EVENT_COUNT;
}
