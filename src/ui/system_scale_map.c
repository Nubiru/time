#include "system_scale_map.h"

#include <math.h>

/* Scale level home centers (log_zoom values from camera_scale):
 * 0 Personal:  -2.303
 * 1 Earth:      0.0
 * 2 Inner:      1.386
 * 3 Solar:      3.401
 * 4 Local:      5.298
 * 5 Galaxy:     7.601
 * 6 Universe:   9.903
 */

/* Fade ramp width in log_zoom units (20% of typical scale range ~2.0) */
#define FADE_RAMP 0.8f

/*
 * Knowledge system scale assignments:
 *
 * Gregorian:  Always visible (universal time reference)
 * Astrology:  Home at Inner Solar (zodiac ring), visible Personal→Solar
 * Tzolkin:    Home at Personal (spiritual/ritual time), visible Personal→Inner
 * I Ching:    Home at Personal (divination), visible Personal→Earth
 * Chinese:    Home at Earth (cultural/civil calendar), visible Personal→Solar
 * HD:         Home at Personal (natal chart), visible Personal→Inner
 * Cosmic:     Home at Galaxy/Universe, visible Local→Universe
 */

static const system_scale_info_t SYSTEM_MAP[KNOWLEDGE_SYSTEM_COUNT] = {
    /* Gregorian — always visible across all scales */
    { SYS_GREGORIAN, "Gregorian",
      3,      /* home: Solar System */
      -5.0f,  /* visible from minimum zoom */
      11.0f,  /* visible to maximum zoom */
      1.0f },

    /* Astrology — zodiac ring, planets in signs */
    { SYS_ASTROLOGY, "Astrology",
      2,      /* home: Inner Solar System (zodiac ring) */
      -4.6f,  /* visible from Personal */
      4.4f,   /* fades out past Solar */
      1.0f },

    /* Tzolkin — personal spiritual cycle */
    { SYS_TZOLKIN, "Tzolkin",
      0,      /* home: Personal Chart */
      -4.6f,  /* visible from Personal */
      2.1f,   /* fades out at Inner */
      1.0f },

    /* I Ching — contemplative/divination */
    { SYS_ICHING, "I Ching",
      0,      /* home: Personal Chart */
      -4.6f,  /* visible from Personal */
      0.7f,   /* fades out at Earth */
      1.0f },

    /* Chinese — cultural calendar */
    { SYS_CHINESE, "Chinese",
      1,      /* home: Earth-Moon */
      -4.6f,  /* visible from Personal */
      4.4f,   /* fades out past Solar */
      1.0f },

    /* Human Design — natal chart system */
    { SYS_HD, "Human Design",
      0,      /* home: Personal Chart */
      -4.6f,  /* visible from Personal */
      2.1f,   /* fades out at Inner */
      1.0f },

    /* Cosmic Time — deep time at galactic+ scales */
    { SYS_COSMIC, "Cosmic Time",
      5,      /* home: Galaxy */
      4.4f,   /* visible from Local neighborhood */
      11.0f,  /* visible to maximum zoom */
      1.0f },
};

system_scale_info_t system_scale_info(knowledge_system_t sys)
{
    if (sys < 0 || sys >= KNOWLEDGE_SYSTEM_COUNT) {
        return SYSTEM_MAP[SYS_GREGORIAN];
    }
    return SYSTEM_MAP[sys];
}

float system_opacity_at_zoom(knowledge_system_t sys, float log_zoom)
{
    if (sys < 0 || sys >= KNOWLEDGE_SYSTEM_COUNT) return 0.0f;

    const system_scale_info_t *info = &SYSTEM_MAP[sys];

    /* Outside visible range entirely */
    if (log_zoom < info->scale_min || log_zoom > info->scale_max) return 0.0f;

    float opacity = info->home_opacity;

    /* Fade in at lower boundary */
    float fade_in_end = info->scale_min + FADE_RAMP;
    if (log_zoom < fade_in_end) {
        float t = (log_zoom - info->scale_min) / FADE_RAMP;
        opacity *= t;
    }

    /* Fade out at upper boundary */
    float fade_out_start = info->scale_max - FADE_RAMP;
    if (log_zoom > fade_out_start) {
        float t = (info->scale_max - log_zoom) / FADE_RAMP;
        opacity *= t;
    }

    /* Clamp */
    if (opacity < 0.0f) opacity = 0.0f;
    if (opacity > 1.0f) opacity = 1.0f;

    return opacity;
}

const char *system_name(knowledge_system_t sys)
{
    if (sys < 0 || sys >= KNOWLEDGE_SYSTEM_COUNT) return "?";
    return SYSTEM_MAP[sys].name;
}

int systems_visible_at_zoom(float log_zoom, knowledge_system_t *out, int max)
{
    int count = 0;
    for (int i = 0; i < KNOWLEDGE_SYSTEM_COUNT && count < max; i++) {
        if (system_opacity_at_zoom((knowledge_system_t)i, log_zoom) > 0.0f) {
            if (out) out[count] = (knowledge_system_t)i;
            count++;
        }
    }
    return count;
}

int system_home_scale(knowledge_system_t sys)
{
    if (sys < 0 || sys >= KNOWLEDGE_SYSTEM_COUNT) return 3; /* default: Solar */
    return SYSTEM_MAP[sys].home_scale;
}

int system_count(void)
{
    return KNOWLEDGE_SYSTEM_COUNT;
}
