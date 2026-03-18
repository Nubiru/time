/* view_adapt.c -- Responsive View Adaptation
 *
 * Static const table of 28 configs (7 views x 4 tiers).
 * Pure functions only. */

#include "view_adapt.h"

#include <stdint.h>

/* View count (matches VIEW_COUNT from view_registry.h, but self-contained) */
#define VA_VIEW_COUNT 7

/* View IDs used as plain ints — mirrors view_registry.h */
#define VA_VIEW_SPACE     0
#define VA_VIEW_EARTH     1
#define VA_VIEW_GALAXY    2
#define VA_VIEW_PERSONAL  3
#define VA_VIEW_DEEP_TIME 4
#define VA_VIEW_CITY      5
#define VA_VIEW_ROOM      6

/* ================================================================
 * STATIC CONFIG TABLE — 7 views x 4 tiers = 28 entries
 *
 * Indexed as: s_configs[view_id][tier]
 * ================================================================ */

static const va_adapt_t s_configs[VA_VIEW_COUNT][VA_TIER_COUNT] = {
    /* ---- VIEW_SPACE (0) ---- */
    {
        /* Desktop */
        { 0, VA_DESKTOP,
          VA_LAYOUT_CARD, VA_LAYOUT_CARD, VA_LAYOUT_CARD, VA_LAYOUT_CARD,
          1, 0, 1, 1,
          0, 34,
          1.0f, 1.0f, 1.0f },
        /* Tablet */
        { 0, VA_TABLET,
          VA_LAYOUT_SHEET, VA_LAYOUT_SHEET, VA_LAYOUT_SHEET, VA_LAYOUT_SHEET,
          1, 1, 1, 1,
          1, 44,
          1.0f, 1.0f, 1.0f },
        /* Phone Landscape */
        { 0, VA_PHONE_LAND,
          VA_LAYOUT_SHEET, VA_LAYOUT_SHEET, VA_LAYOUT_SHEET, VA_LAYOUT_SHEET,
          0, 1, 1, 0,
          2, 44,
          0.9f, 0.85f, 0.85f },
        /* Phone Portrait */
        { 0, VA_PHONE_PORT,
          VA_LAYOUT_FULLSCREEN, VA_LAYOUT_SHEET, VA_LAYOUT_FULLSCREEN, VA_LAYOUT_FULLSCREEN,
          0, 1, 1, 0,
          2, 48,
          0.85f, 0.75f, 0.75f },
    },

    /* ---- VIEW_EARTH (1) ---- */
    {
        /* Desktop */
        { 1, VA_DESKTOP,
          VA_LAYOUT_CARD, VA_LAYOUT_CARD, VA_LAYOUT_CARD, VA_LAYOUT_CARD,
          1, 0, 1, 1,
          0, 34,
          1.0f, 1.0f, 1.0f },
        /* Tablet */
        { 1, VA_TABLET,
          VA_LAYOUT_SHEET, VA_LAYOUT_SHEET, VA_LAYOUT_SHEET, VA_LAYOUT_SHEET,
          1, 1, 1, 1,
          1, 44,
          1.0f, 1.0f, 1.0f },
        /* Phone Landscape */
        { 1, VA_PHONE_LAND,
          VA_LAYOUT_SHEET, VA_LAYOUT_SHEET, VA_LAYOUT_SHEET, VA_LAYOUT_SHEET,
          0, 1, 1, 0,
          2, 44,
          0.9f, 0.85f, 0.85f },
        /* Phone Portrait — Earth: card fullscreen (Space/Galaxy/Earth exception) */
        { 1, VA_PHONE_PORT,
          VA_LAYOUT_FULLSCREEN, VA_LAYOUT_SHEET, VA_LAYOUT_FULLSCREEN, VA_LAYOUT_FULLSCREEN,
          0, 1, 1, 0,
          2, 48,
          0.85f, 0.75f, 0.75f },
    },

    /* ---- VIEW_GALAXY (2) ---- */
    {
        /* Desktop */
        { 2, VA_DESKTOP,
          VA_LAYOUT_CARD, VA_LAYOUT_CARD, VA_LAYOUT_CARD, VA_LAYOUT_CARD,
          1, 0, 1, 1,
          0, 34,
          1.0f, 1.0f, 1.0f },
        /* Tablet */
        { 2, VA_TABLET,
          VA_LAYOUT_SHEET, VA_LAYOUT_SHEET, VA_LAYOUT_SHEET, VA_LAYOUT_SHEET,
          1, 1, 1, 1,
          1, 44,
          1.0f, 1.0f, 1.0f },
        /* Phone Landscape */
        { 2, VA_PHONE_LAND,
          VA_LAYOUT_SHEET, VA_LAYOUT_SHEET, VA_LAYOUT_SHEET, VA_LAYOUT_SHEET,
          0, 1, 1, 0,
          2, 44,
          0.9f, 0.85f, 0.85f },
        /* Phone Portrait — Galaxy: card fullscreen (Space/Galaxy/Earth exception) */
        { 2, VA_PHONE_PORT,
          VA_LAYOUT_FULLSCREEN, VA_LAYOUT_SHEET, VA_LAYOUT_FULLSCREEN, VA_LAYOUT_FULLSCREEN,
          0, 1, 1, 0,
          2, 48,
          0.85f, 0.75f, 0.75f },
    },

    /* ---- VIEW_PERSONAL (3) ---- */
    {
        /* Desktop */
        { 3, VA_DESKTOP,
          VA_LAYOUT_CARD, VA_LAYOUT_CARD, VA_LAYOUT_CARD, VA_LAYOUT_CARD,
          1, 0, 1, 1,
          0, 34,
          1.0f, 1.0f, 1.0f },
        /* Tablet — Personal: card sheet (card-dominant exception) */
        { 3, VA_TABLET,
          VA_LAYOUT_SHEET, VA_LAYOUT_SHEET, VA_LAYOUT_SHEET, VA_LAYOUT_SHEET,
          1, 1, 1, 1,
          1, 44,
          1.0f, 1.0f, 1.0f },
        /* Phone Landscape — Personal: card fullscreen */
        { 3, VA_PHONE_LAND,
          VA_LAYOUT_SHEET, VA_LAYOUT_SHEET, VA_LAYOUT_FULLSCREEN, VA_LAYOUT_SHEET,
          0, 1, 1, 0,
          2, 44,
          0.9f, 0.85f, 0.85f },
        /* Phone Portrait */
        { 3, VA_PHONE_PORT,
          VA_LAYOUT_FULLSCREEN, VA_LAYOUT_SHEET, VA_LAYOUT_FULLSCREEN, VA_LAYOUT_FULLSCREEN,
          0, 1, 1, 0,
          2, 48,
          0.85f, 0.75f, 0.75f },
    },

    /* ---- VIEW_DEEP_TIME (4) ---- */
    {
        /* Desktop */
        { 4, VA_DESKTOP,
          VA_LAYOUT_CARD, VA_LAYOUT_CARD, VA_LAYOUT_CARD, VA_LAYOUT_CARD,
          1, 0, 1, 1,
          0, 34,
          1.0f, 1.0f, 1.0f },
        /* Tablet */
        { 4, VA_TABLET,
          VA_LAYOUT_SHEET, VA_LAYOUT_SHEET, VA_LAYOUT_SHEET, VA_LAYOUT_SHEET,
          1, 1, 1, 1,
          1, 44,
          1.0f, 1.0f, 1.0f },
        /* Phone Landscape */
        { 4, VA_PHONE_LAND,
          VA_LAYOUT_SHEET, VA_LAYOUT_SHEET, VA_LAYOUT_SHEET, VA_LAYOUT_SHEET,
          0, 1, 1, 0,
          2, 44,
          0.9f, 0.85f, 0.85f },
        /* Phone Portrait */
        { 4, VA_PHONE_PORT,
          VA_LAYOUT_FULLSCREEN, VA_LAYOUT_SHEET, VA_LAYOUT_FULLSCREEN, VA_LAYOUT_FULLSCREEN,
          0, 1, 1, 0,
          2, 48,
          0.85f, 0.75f, 0.75f },
    },

    /* ---- VIEW_CITY (5) ---- */
    {
        /* Desktop */
        { 5, VA_DESKTOP,
          VA_LAYOUT_CARD, VA_LAYOUT_CARD, VA_LAYOUT_CARD, VA_LAYOUT_CARD,
          1, 0, 1, 1,
          0, 34,
          1.0f, 1.0f, 1.0f },
        /* Tablet */
        { 5, VA_TABLET,
          VA_LAYOUT_SHEET, VA_LAYOUT_SHEET, VA_LAYOUT_SHEET, VA_LAYOUT_SHEET,
          1, 1, 1, 1,
          1, 44,
          1.0f, 1.0f, 1.0f },
        /* Phone Landscape */
        { 5, VA_PHONE_LAND,
          VA_LAYOUT_SHEET, VA_LAYOUT_SHEET, VA_LAYOUT_SHEET, VA_LAYOUT_SHEET,
          0, 1, 1, 0,
          2, 44,
          0.9f, 0.85f, 0.85f },
        /* Phone Portrait */
        { 5, VA_PHONE_PORT,
          VA_LAYOUT_FULLSCREEN, VA_LAYOUT_SHEET, VA_LAYOUT_FULLSCREEN, VA_LAYOUT_FULLSCREEN,
          0, 1, 1, 0,
          2, 48,
          0.85f, 0.75f, 0.75f },
    },

    /* ---- VIEW_ROOM (6) ---- */
    {
        /* Desktop */
        { 6, VA_DESKTOP,
          VA_LAYOUT_CARD, VA_LAYOUT_CARD, VA_LAYOUT_CARD, VA_LAYOUT_CARD,
          1, 0, 1, 1,
          0, 34,
          1.0f, 1.0f, 1.0f },
        /* Tablet — Room: card sheet (card-dominant exception) */
        { 6, VA_TABLET,
          VA_LAYOUT_SHEET, VA_LAYOUT_SHEET, VA_LAYOUT_SHEET, VA_LAYOUT_SHEET,
          1, 1, 1, 1,
          1, 44,
          1.0f, 1.0f, 1.0f },
        /* Phone Landscape — Room: card fullscreen */
        { 6, VA_PHONE_LAND,
          VA_LAYOUT_SHEET, VA_LAYOUT_SHEET, VA_LAYOUT_FULLSCREEN, VA_LAYOUT_SHEET,
          0, 1, 1, 0,
          2, 44,
          0.9f, 0.85f, 0.85f },
        /* Phone Portrait */
        { 6, VA_PHONE_PORT,
          VA_LAYOUT_FULLSCREEN, VA_LAYOUT_SHEET, VA_LAYOUT_FULLSCREEN, VA_LAYOUT_FULLSCREEN,
          0, 1, 1, 0,
          2, 48,
          0.85f, 0.75f, 0.75f },
    },
};

/* ================================================================
 * PUBLIC API
 * ================================================================ */

va_screen_tier_t va_tier_from_width(int screen_width)
{
    if (screen_width > 1024) return VA_DESKTOP;
    if (screen_width >= 768) return VA_TABLET;
    if (screen_width >= 481) return VA_PHONE_LAND;
    return VA_PHONE_PORT;
}

const char *va_tier_name(va_screen_tier_t tier)
{
    switch (tier) {
    case VA_DESKTOP:    return "Desktop";
    case VA_TABLET:     return "Tablet";
    case VA_PHONE_LAND: return "Phone Landscape";
    case VA_PHONE_PORT: return "Phone Portrait";
    default:            return "?";
    }
}

va_adapt_t va_get(int view_id, va_screen_tier_t tier)
{
    /* Clamp tier to valid range */
    if (tier < VA_DESKTOP || tier >= VA_TIER_COUNT) {
        tier = VA_DESKTOP;
    }

    /* Out-of-range view_id: default to Space with requested tier */
    if (view_id < 0 || view_id >= VA_VIEW_COUNT) {
        va_adapt_t fallback = s_configs[VA_VIEW_SPACE][tier];
        /* Keep the clamped tier in case it was adjusted */
        fallback.tier = tier;
        return fallback;
    }

    return s_configs[view_id][tier];
}

va_adapt_t va_get_for_width(int view_id, int screen_width)
{
    return va_get(view_id, va_tier_from_width(screen_width));
}

int va_panel_available(va_adapt_t config, int panel_type)
{
    va_layout_mode_t layout;
    switch (panel_type) {
    case 0: layout = config.help_layout;    break;
    case 1: layout = config.layer_layout;   break;
    case 2: layout = config.card_layout;    break;
    case 3: layout = config.palette_layout; break;
    default: return 0;
    }
    return layout != VA_LAYOUT_HIDDEN ? 1 : 0;
}

int va_tier_count(void)
{
    return VA_TIER_COUNT;
}

int va_tier_breakpoint(va_screen_tier_t tier)
{
    switch (tier) {
    case VA_DESKTOP:    return INT32_MAX;
    case VA_TABLET:     return 1024;
    case VA_PHONE_LAND: return 767;
    case VA_PHONE_PORT: return 480;
    default:            return INT32_MAX;
    }
}

const char *va_layout_name(va_layout_mode_t mode)
{
    switch (mode) {
    case VA_LAYOUT_CARD:       return "Card";
    case VA_LAYOUT_SHEET:      return "Sheet";
    case VA_LAYOUT_FULLSCREEN: return "Fullscreen";
    case VA_LAYOUT_HIDDEN:     return "Hidden";
    default:                   return "?";
    }
}
