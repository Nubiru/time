#include "view_registry.h"

#include <ctype.h>
#include <stddef.h>
#include <string.h>

/* Static view definitions. Order must match view_id_t enum. */
static const view_config_t s_views[VIEW_COUNT] = {
    {
        .id           = VIEW_SPACE,
        .name         = "Space View",
        .short_name   = "Space",
        .description  = "Solar system viewed from outside, planets and orbits visible",
        .shortcut     = "Shift+3",
        .scale_min    = 2,
        .scale_max    = 4,
        .default_scale = 3,
        .layers_enabled = 0xFD,  /* Stars+Orbits+Planets+Zodiac+Labels+Cards+HUD */
        .implemented  = 1
    },
    {
        .id           = VIEW_EARTH,
        .name         = "Earth View",
        .short_name   = "Earth",
        .description  = "Globe surface with sky, horizon, and local phenomena",
        .shortcut     = "E",
        .scale_min    = 0,
        .scale_max    = 1,
        .default_scale = 1,
        .layers_enabled = 0xA9,  /* Stars+Planets+Labels+HUD */
        .implemented  = 1
    },
    {
        .id           = VIEW_GALAXY,
        .name         = "Galaxy View",
        .short_name   = "Galaxy",
        .description  = "Milky Way spiral structure at galactic scale",
        .shortcut     = "Shift+5",
        .scale_min    = 5,
        .scale_max    = 6,
        .default_scale = 5,
        .layers_enabled = 0x83,  /* Stars+Galaxy+HUD */
        .implemented  = 1
    },
    {
        .id           = VIEW_PERSONAL,
        .name         = "Personal View",
        .short_name   = "Personal",
        .description  = "Birth chart focus with zodiac wheel and personal gates",
        .shortcut     = "P",
        .scale_min    = 0,
        .scale_max    = 0,
        .default_scale = 0,
        .layers_enabled = 0xF1,  /* Stars+Zodiac+Labels+Cards+HUD */
        .implemented  = 0
    },
    {
        .id           = VIEW_DEEP_TIME,
        .name         = "Deep Time View",
        .short_name   = "Deep Time",
        .description  = "Geological timeline spanning eons, eras, and epochs",
        .shortcut     = "G",
        .scale_min    = 6,
        .scale_max    = 6,
        .default_scale = 6,
        .layers_enabled = 0x83,  /* Stars+Galaxy+HUD */
        .implemented  = 0
    },
    {
        .id           = VIEW_CITY,
        .name         = "City View",
        .short_name   = "City",
        .description  = "Local horizon and sky from a specific location on Earth",
        .shortcut     = "C",
        .scale_min    = 0,
        .scale_max    = 0,
        .default_scale = 0,
        .layers_enabled = 0xA9,  /* Stars+Planets+Labels+HUD */
        .implemented  = 0
    },
    {
        .id           = VIEW_ROOM,
        .name         = "Room View",
        .short_name   = "Room",
        .description  = "Personal 3D space with cards and time data around you",
        .shortcut     = "\xe2\x80\x94",  /* UTF-8 em dash */
        .scale_min    = -1,
        .scale_max    = -1,
        .default_scale = -1,
        .layers_enabled = 0xC0,  /* Cards+HUD */
        .implemented  = 0
    }
};

/* Sentinel returned for invalid IDs. */
static const view_config_t s_unknown = {
    .id           = (view_id_t)-1,
    .name         = "Unknown",
    .short_name   = "Unknown",
    .description  = "Invalid view",
    .shortcut     = "",
    .scale_min    = 0,
    .scale_max    = 0,
    .default_scale = 0,
    .layers_enabled = 0,
    .implemented  = 0
};

/* Case-insensitive string comparison (portable, no POSIX dependency). */
static int ci_streq(const char *a, const char *b)
{
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
            return 0;
        a++;
        b++;
    }
    return *a == *b;
}

view_config_t view_get(view_id_t id)
{
    if ((int)id < 0 || (int)id >= VIEW_COUNT)
        return s_unknown;
    return s_views[id];
}

const char *view_name(view_id_t id)
{
    if ((int)id < 0 || (int)id >= VIEW_COUNT)
        return "Unknown";
    return s_views[id].name;
}

int view_count(void)
{
    return VIEW_COUNT;
}

int view_implemented_count(void)
{
    int count = 0;
    for (int i = 0; i < VIEW_COUNT; i++) {
        if (s_views[i].implemented)
            count++;
    }
    return count;
}

view_id_t view_for_scale(int scale_level)
{
    /* Search implemented views first for best match. */
    for (int i = 0; i < VIEW_COUNT; i++) {
        if (s_views[i].implemented &&
            scale_level >= s_views[i].scale_min &&
            scale_level <= s_views[i].scale_max) {
            return s_views[i].id;
        }
    }
    /* Fallback: search all views (including unimplemented). */
    for (int i = 0; i < VIEW_COUNT; i++) {
        if (scale_level >= s_views[i].scale_min &&
            scale_level <= s_views[i].scale_max) {
            return s_views[i].id;
        }
    }
    /* Nothing matched at all -> default to Space. */
    return VIEW_SPACE;
}

view_id_t view_from_name(const char *name)
{
    if (name == NULL || name[0] == '\0')
        return (view_id_t)-1;

    for (int i = 0; i < VIEW_COUNT; i++) {
        if (ci_streq(name, s_views[i].name) ||
            ci_streq(name, s_views[i].short_name)) {
            return s_views[i].id;
        }
    }
    return (view_id_t)-1;
}
