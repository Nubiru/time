/* settings_panel.c — Settings panel data layer implementation.
 *
 * Builds display-ready settings data from user_prefs.
 * Apply functions map (section, option, value) back to prefs fields.
 *
 * PERSONA stream: Settings screen data model.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects. */

#include "settings_panel.h"

#include <string.h>

/* --- internal helpers --- */

/* Section indices (must match build order) */
static const int SEC_APPEARANCE = 0;
static const int SEC_TIME       = 1;
static const int SEC_SOCIAL     = 2;
static const int SEC_LOCATION   = 3;
static const int SEC_COUNT      = 4;

/* Option indices within Appearance */
static const int APP_THEME           = 0;
static const int APP_FONT_SCALE      = 1;
static const int APP_ANIMATION_SPEED = 2;
static const int APP_OPACITY         = 3;

/* Option indices within Time */
static const int TIM_24H        = 0;
static const int TIM_WEEK_START = 1;

/* Option indices within Social */
static const int SOC_ACTIVITY      = 0;
static const int SOC_NOTIFICATIONS = 1;
static const int SOC_ROULETTE      = 2;

/* Option indices within Location */
static const int LOC_RADIUS = 0;

static float sp_clamp_f(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static void sp_set_label(char *dst, const char *src) {
    strncpy(dst, src, SP_LABEL_MAX - 1);
    dst[SP_LABEL_MAX - 1] = '\0';
}

static void sp_set_section_name(char *dst, const char *src) {
    strncpy(dst, src, SP_SECTION_MAX - 1);
    dst[SP_SECTION_MAX - 1] = '\0';
}

static void sp_add_choice(sp_option_t *opt, const char *name) {
    if (opt->choice_count >= SP_MAX_CHOICES) return;
    strncpy(opt->choices[opt->choice_count], name, SP_CHOICE_MAX - 1);
    opt->choices[opt->choice_count][SP_CHOICE_MAX - 1] = '\0';
    opt->choice_count++;
}

/* --- build helpers --- */

static sp_section_t sp_build_appearance(const up_prefs_t *prefs) {
    sp_section_t sec;
    memset(&sec, 0, sizeof(sec));
    sp_set_section_name(sec.name, "Appearance");

    /* Theme (CHOICE) */
    sp_option_t *theme = &sec.options[APP_THEME];
    sp_set_label(theme->label, "Theme");
    theme->type = SP_OPT_CHOICE;
    sp_add_choice(theme, "Cosmos");
    sp_add_choice(theme, "Dawn");
    theme->choice_selected = (int)prefs->visual.theme;

    /* Font Scale (SLIDER) */
    sp_option_t *font = &sec.options[APP_FONT_SCALE];
    sp_set_label(font->label, "Font Scale");
    font->type = SP_OPT_SLIDER;
    font->slider_min  = 0.5f;
    font->slider_max  = 2.0f;
    font->slider_step = 0.1f;
    font->slider_value = prefs->visual.font_scale;

    /* Animation Speed (SLIDER) */
    sp_option_t *anim = &sec.options[APP_ANIMATION_SPEED];
    sp_set_label(anim->label, "Animation Speed");
    anim->type = SP_OPT_SLIDER;
    anim->slider_min  = 0.5f;
    anim->slider_max  = 2.0f;
    anim->slider_step = 0.1f;
    anim->slider_value = prefs->visual.animation_speed;

    /* Opacity (SLIDER) */
    sp_option_t *opacity = &sec.options[APP_OPACITY];
    sp_set_label(opacity->label, "Opacity");
    opacity->type = SP_OPT_SLIDER;
    opacity->slider_min  = 0.5f;
    opacity->slider_max  = 1.0f;
    opacity->slider_step = 0.05f;
    opacity->slider_value = prefs->visual.opacity_scale;

    sec.option_count = 4;
    return sec;
}

static sp_section_t sp_build_time(const up_prefs_t *prefs) {
    sp_section_t sec;
    memset(&sec, 0, sizeof(sec));
    sp_set_section_name(sec.name, "Time");

    /* 24-Hour Time (TOGGLE) */
    sp_option_t *h24 = &sec.options[TIM_24H];
    sp_set_label(h24->label, "24-Hour Time");
    h24->type = SP_OPT_TOGGLE;
    h24->toggle_value = prefs->temporal.use_24h_time;

    /* Week Starts On (CHOICE) */
    sp_option_t *week = &sec.options[TIM_WEEK_START];
    sp_set_label(week->label, "Week Starts On");
    week->type = SP_OPT_CHOICE;
    sp_add_choice(week, "Sunday");
    sp_add_choice(week, "Monday");
    week->choice_selected = prefs->temporal.week_start_day;

    sec.option_count = 2;
    return sec;
}

static sp_section_t sp_build_social(const up_prefs_t *prefs) {
    sp_section_t sec;
    memset(&sec, 0, sizeof(sec));
    sp_set_section_name(sec.name, "Social");

    /* Activity Level (CHOICE) */
    sp_option_t *act = &sec.options[SOC_ACTIVITY];
    sp_set_label(act->label, "Activity Level");
    act->type = SP_OPT_CHOICE;
    sp_add_choice(act, "Introvert");
    sp_add_choice(act, "Balanced");
    sp_add_choice(act, "Extrovert");
    act->choice_selected = (int)prefs->social.activity_level;

    /* Notifications (CHOICE) */
    sp_option_t *notif = &sec.options[SOC_NOTIFICATIONS];
    sp_set_label(notif->label, "Notifications");
    notif->type = SP_OPT_CHOICE;
    sp_add_choice(notif, "Off");
    sp_add_choice(notif, "Low");
    sp_add_choice(notif, "Medium");
    sp_add_choice(notif, "High");
    notif->choice_selected = prefs->social.notification_frequency;

    /* Neighbor Roulette (TOGGLE) */
    sp_option_t *roul = &sec.options[SOC_ROULETTE];
    sp_set_label(roul->label, "Neighbor Roulette");
    roul->type = SP_OPT_TOGGLE;
    roul->toggle_value = prefs->social.roulette_opt_in;

    sec.option_count = 3;
    return sec;
}

static sp_section_t sp_build_location(const up_prefs_t *prefs) {
    sp_section_t sec;
    memset(&sec, 0, sizeof(sec));
    sp_set_section_name(sec.name, "Location");

    /* Discovery Radius (SLIDER) */
    sp_option_t *rad = &sec.options[LOC_RADIUS];
    sp_set_label(rad->label, "Discovery Radius");
    rad->type = SP_OPT_SLIDER;
    rad->slider_min  = 1.0f;
    rad->slider_max  = 500.0f;
    rad->slider_step = 10.0f;
    rad->slider_value = (float)prefs->location.exploration_radius_km;

    sec.option_count = 1;
    return sec;
}

/* --- public API --- */

sp_panel_t sp_build(const up_prefs_t *prefs) {
    sp_panel_t panel;
    memset(&panel, 0, sizeof(panel));

    panel.sections[SEC_APPEARANCE] = sp_build_appearance(prefs);
    panel.sections[SEC_TIME]       = sp_build_time(prefs);
    panel.sections[SEC_SOCIAL]     = sp_build_social(prefs);
    panel.sections[SEC_LOCATION]   = sp_build_location(prefs);
    panel.section_count = SEC_COUNT;

    return panel;
}

up_prefs_t sp_apply_toggle(const up_prefs_t *prefs, int section, int option,
                           int value) {
    up_prefs_t out = *prefs;

    if (section == SEC_TIME && option == TIM_24H) {
        out.temporal.use_24h_time = value ? 1 : 0;
    } else if (section == SEC_SOCIAL && option == SOC_ROULETTE) {
        out.social.roulette_opt_in = value ? 1 : 0;
    }
    /* Invalid section/option: return unchanged copy */

    return out;
}

up_prefs_t sp_apply_choice(const up_prefs_t *prefs, int section, int option,
                           int choice_index) {
    up_prefs_t out = *prefs;

    if (section == SEC_APPEARANCE && option == APP_THEME) {
        if (choice_index >= 0 && choice_index < THEME_COUNT) {
            out.visual.theme = (theme_id_t)choice_index;
        }
    } else if (section == SEC_TIME && option == TIM_WEEK_START) {
        if (choice_index >= 0 && choice_index <= 1) {
            out.temporal.week_start_day = choice_index;
        }
    } else if (section == SEC_SOCIAL && option == SOC_ACTIVITY) {
        if (choice_index >= 0 && choice_index <= 2) {
            out.social.activity_level = (ip_activity_t)choice_index;
        }
    } else if (section == SEC_SOCIAL && option == SOC_NOTIFICATIONS) {
        if (choice_index >= 0 && choice_index <= 3) {
            out.social.notification_frequency = choice_index;
        }
    }
    /* Invalid section/option: return unchanged copy */

    return out;
}

up_prefs_t sp_apply_slider(const up_prefs_t *prefs, int section, int option,
                           float value) {
    up_prefs_t out = *prefs;

    if (section == SEC_APPEARANCE && option == APP_FONT_SCALE) {
        out.visual.font_scale = sp_clamp_f(value, 0.5f, 2.0f);
    } else if (section == SEC_APPEARANCE && option == APP_ANIMATION_SPEED) {
        out.visual.animation_speed = sp_clamp_f(value, 0.5f, 2.0f);
    } else if (section == SEC_APPEARANCE && option == APP_OPACITY) {
        out.visual.opacity_scale = sp_clamp_f(value, 0.5f, 1.0f);
    } else if (section == SEC_LOCATION && option == LOC_RADIUS) {
        out.location.exploration_radius_km = (double)sp_clamp_f(value, 1.0f, 500.0f);
    }
    /* Invalid section/option: return unchanged copy */

    return out;
}

int sp_section_count(const sp_panel_t *panel) {
    if (!panel) return 0;
    return panel->section_count;
}

int sp_option_count(const sp_panel_t *panel, int section) {
    if (!panel) return 0;
    if (section < 0 || section >= panel->section_count) return 0;
    return panel->sections[section].option_count;
}

const char *sp_option_label(const sp_panel_t *panel, int section, int option) {
    if (!panel) return NULL;
    if (section < 0 || section >= panel->section_count) return NULL;
    if (option < 0 || option >= panel->sections[section].option_count) return NULL;
    return panel->sections[section].options[option].label;
}
