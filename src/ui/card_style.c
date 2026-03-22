/* card_style.c — Per-system card visual styling.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "card_style.h"
#include "golden_layout.h"

/* Card background alpha — translucent wash over cosmos (RefUI: needs visible
 * reading surface. 0.22 = grounding plane without obscuring stars). */
#define CS_BG_BASE_ALPHA 0.22f

/* Border alpha — system accent at 20% (RefUI: border reinforces system identity,
 * turning chartjunk into data-ink). */
#define CS_BORDER_ALPHA  0.20f

int card_style_to_ct_system(int ts_system_id)
{
    switch (ts_system_id) {
    case TS_SYS_GREGORIAN:    return CT_SYSTEM_GREGORIAN;
    case TS_SYS_TZOLKIN:      return CT_SYSTEM_TZOLKIN;
    case TS_SYS_HAAB:         return CT_SYSTEM_TZOLKIN;     /* Mayan kin */
    case TS_SYS_CHINESE:      return CT_SYSTEM_CHINESE;
    case TS_SYS_HEBREW:       return CT_SYSTEM_HEBREW;
    case TS_SYS_ISLAMIC:      return CT_SYSTEM_ISLAMIC;
    case TS_SYS_BUDDHIST:     return CT_SYSTEM_BUDDHIST;
    case TS_SYS_HINDU:        return CT_SYSTEM_HINDU;
    case TS_SYS_ICHING:       return CT_SYSTEM_ICHING;
    case TS_SYS_ASTROLOGY:    return CT_SYSTEM_ASTROLOGY;
    case TS_SYS_HUMAN_DESIGN: return CT_SYSTEM_HUMAN_DESIGN;
    case TS_SYS_KABBALAH:     return CT_SYSTEM_KABBALAH;
    case TS_SYS_COPTIC:       return CT_SYSTEM_COPTIC;
    case TS_SYS_ETHIOPIAN:    return CT_SYSTEM_ETHIOPIAN;
    case TS_SYS_PERSIAN:      return CT_SYSTEM_PERSIAN;
    case TS_SYS_JAPANESE:     return CT_SYSTEM_JAPANESE;
    case TS_SYS_KOREAN:       return CT_SYSTEM_KOREAN;
    case TS_SYS_THAI:         return CT_SYSTEM_THAI;
    case TS_SYS_GEOLOGICAL:   return CT_SYSTEM_GEOLOGY;
    case TS_SYS_COSMIC:       return CT_SYSTEM_ASTRONOMY;
    default:                  return -1;
    }
}

card_style_t card_style_default(float opacity, theme_id_t theme_id)
{
    theme_t theme = theme_get(theme_id);
    card_style_t style;

    style.background   = theme.bg_surface;
    style.background.a = CS_BG_BASE_ALPHA * opacity;

    /* Border: structural only — dim grey, not data-encoding (Tufte) */
    style.border   = theme.border;
    style.border.a = CS_BORDER_ALPHA * opacity;

    style.title = theme.brand_primary;
    style.body  = theme.text_primary;
    style.muted = theme.text_secondary;

    return style;
}

card_style_t card_style_for_system(int system_id, float opacity,
                                    theme_id_t theme_id)
{
    int ct = card_style_to_ct_system(system_id);
    if (ct < 0) {
        return card_style_default(opacity, theme_id);
    }

    theme_t theme = theme_get(theme_id);
    card_style_t style;

    /* System-colored translucent wash (Tufte: color IS the data) */
    style.background   = theme_system_accent(&theme, (ct_system_t)ct);
    style.background.a = CS_BG_BASE_ALPHA * opacity;

    /* Border: system accent at low alpha — reinforces identity (RefUI).
     * Turns structural line into data-ink. */
    style.border   = theme_system_accent(&theme, (ct_system_t)ct);
    style.border.a = CS_BORDER_ALPHA * opacity;

    /* System accent mixed toward white — guarantees readability (RefUI).
     * 0.62/0.38 mix: desaturated 8% + luminance boosted 8% (Itten/Albers:
     * small bright area on dark bg looks hyper-intense without desaturation). */
    color_rgba_t accent = theme_system_accent(&theme, (ct_system_t)ct);
    style.title.r = accent.r * 0.62f + 0.38f;
    style.title.g = accent.g * 0.62f + 0.38f;
    style.title.b = accent.b * 0.62f + 0.38f;
    style.title.a = 1.0f;

    /* Theme text colors for body content */
    style.body  = theme.text_primary;
    style.muted = theme.text_secondary;

    return style;
}

int card_style_focus_system(int focus_mode)
{
    switch (focus_mode) {
    case 1: return TS_SYS_ASTROLOGY;     /* FOCUS_MODE_ASTROLOGY */
    case 2: return TS_SYS_TZOLKIN;       /* FOCUS_MODE_KIN */
    case 3: return TS_SYS_ICHING;        /* FOCUS_MODE_ICHING */
    case 4: return TS_SYS_CHINESE;       /* FOCUS_MODE_CHINESE */
    case 5: return TS_SYS_HUMAN_DESIGN;  /* FOCUS_MODE_HD */
    case 6: return TS_SYS_KABBALAH;      /* FOCUS_MODE_KABBALAH */
    default: return -1;
    }
}
