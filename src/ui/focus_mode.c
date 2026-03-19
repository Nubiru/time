#include "focus_mode.h"

/* Transition duration: phi^-2 * 1.0 = ~0.382s (medium pace) */
static const float TRANSITION_DUR = 0.382f;

/* DIM level for background layers when a system is focused */
static const float DIM_LEVEL = 0.25f;

/* --- Preset definitions --- */

static focus_preset_t make_overview(void)
{
    focus_preset_t p;
    p.name = "Overview";
    p.key = 27; /* Esc */
    p.card_mask = 0;
    p.zoom_target = 1.0f;
    for (int i = 0; i < LAYER_COUNT; i++) {
        p.layer_opacities[i] = 1.0f;
    }
    return p;
}

static focus_preset_t make_astrology(void)
{
    focus_preset_t p;
    p.name = "Astrology";
    p.key = 'A';
    p.card_mask = (1 << CARD_ASTROLOGY);
    p.zoom_target = 2.0f; /* zoom toward ecliptic */
    /* Highlight zodiac ring + planets, dim others */
    p.layer_opacities[LAYER_STARS]       = DIM_LEVEL;
    p.layer_opacities[LAYER_GALAXY]      = 0.0f;
    p.layer_opacities[LAYER_ORBITS]      = 0.6f;
    p.layer_opacities[LAYER_PLANETS]     = 1.0f;
    p.layer_opacities[LAYER_ZODIAC_RING] = 1.0f;
    p.layer_opacities[LAYER_LABELS]      = 1.0f;
    p.layer_opacities[LAYER_CARDS]       = 1.0f;
    p.layer_opacities[LAYER_HUD]         = 1.0f;
    return p;
}

static focus_preset_t make_kin(void)
{
    focus_preset_t p;
    p.name = "Kin";
    p.key = 'K';
    p.card_mask = (1 << CARD_TZOLKIN);
    p.zoom_target = 1.5f;
    /* Dim 3D scene, focus on card */
    p.layer_opacities[LAYER_STARS]       = DIM_LEVEL;
    p.layer_opacities[LAYER_GALAXY]      = 0.0f;
    p.layer_opacities[LAYER_ORBITS]      = DIM_LEVEL;
    p.layer_opacities[LAYER_PLANETS]     = DIM_LEVEL;
    p.layer_opacities[LAYER_ZODIAC_RING] = DIM_LEVEL;
    p.layer_opacities[LAYER_LABELS]      = 0.0f;
    p.layer_opacities[LAYER_CARDS]       = 1.0f;
    p.layer_opacities[LAYER_HUD]         = 1.0f;
    return p;
}

static focus_preset_t make_iching(void)
{
    focus_preset_t p;
    p.name = "I Ching";
    p.key = 'I';
    p.card_mask = (1 << CARD_ICHING);
    p.zoom_target = 1.5f;
    p.layer_opacities[LAYER_STARS]       = DIM_LEVEL;
    p.layer_opacities[LAYER_GALAXY]      = 0.0f;
    p.layer_opacities[LAYER_ORBITS]      = DIM_LEVEL;
    p.layer_opacities[LAYER_PLANETS]     = DIM_LEVEL;
    p.layer_opacities[LAYER_ZODIAC_RING] = DIM_LEVEL;
    p.layer_opacities[LAYER_LABELS]      = 0.0f;
    p.layer_opacities[LAYER_CARDS]       = 1.0f;
    p.layer_opacities[LAYER_HUD]         = 1.0f;
    return p;
}

static focus_preset_t make_chinese(void)
{
    focus_preset_t p;
    p.name = "Chinese";
    p.key = 'C';
    p.card_mask = (1 << CARD_CHINESE);
    p.zoom_target = 1.5f;
    p.layer_opacities[LAYER_STARS]       = DIM_LEVEL;
    p.layer_opacities[LAYER_GALAXY]      = 0.0f;
    p.layer_opacities[LAYER_ORBITS]      = DIM_LEVEL;
    p.layer_opacities[LAYER_PLANETS]     = 0.5f;
    p.layer_opacities[LAYER_ZODIAC_RING] = DIM_LEVEL;
    p.layer_opacities[LAYER_LABELS]      = 0.0f;
    p.layer_opacities[LAYER_CARDS]       = 1.0f;
    p.layer_opacities[LAYER_HUD]         = 1.0f;
    return p;
}

static focus_preset_t make_hd(void)
{
    focus_preset_t p;
    p.name = "Human Design";
    p.key = 'D';
    p.card_mask = (1 << CARD_HD);
    p.zoom_target = 1.5f;
    /* HD bodygraph is a card-centered experience */
    p.layer_opacities[LAYER_STARS]       = DIM_LEVEL;
    p.layer_opacities[LAYER_GALAXY]      = 0.0f;
    p.layer_opacities[LAYER_ORBITS]      = 0.0f;
    p.layer_opacities[LAYER_PLANETS]     = 0.4f;
    p.layer_opacities[LAYER_ZODIAC_RING] = 0.4f;
    p.layer_opacities[LAYER_LABELS]      = 0.0f;
    p.layer_opacities[LAYER_CARDS]       = 1.0f;
    p.layer_opacities[LAYER_HUD]         = 1.0f;
    return p;
}

static focus_preset_t get_preset(int mode)
{
    switch (mode) {
        case FOCUS_MODE_OVERVIEW:  return make_overview();
        case FOCUS_MODE_ASTROLOGY: return make_astrology();
        case FOCUS_MODE_KIN:       return make_kin();
        case FOCUS_MODE_ICHING:    return make_iching();
        case FOCUS_MODE_CHINESE:   return make_chinese();
        case FOCUS_MODE_HD:        return make_hd();
        default:                   return make_overview();
    }
}

/* --- Public API --- */

focus_modes_t focus_mode_init(void)
{
    focus_modes_t fm;
    fm.active = FOCUS_MODE_OVERVIEW;
    fm.previous = FOCUS_MODE_OVERVIEW;
    fm.transition_t = 1.0f; /* no transition in progress */
    fm.transition_dur = TRANSITION_DUR;
    return fm;
}

focus_modes_t focus_mode_switch(focus_modes_t fm, int mode)
{
    if (mode < 0 || mode >= FOCUS_MODE_COUNT) return fm;

    /* Toggle: pressing same key returns to overview */
    if (mode == (int)fm.active && mode != FOCUS_MODE_OVERVIEW) {
        fm.previous = fm.active;
        fm.active = FOCUS_MODE_OVERVIEW;
    } else {
        fm.previous = fm.active;
        fm.active = (focus_mode_e)mode;
    }
    fm.transition_t = 0.0f;
    return fm;
}

focus_modes_t focus_mode_from_key(focus_modes_t fm, int key)
{
    switch (key) {
        case 'A': case 'a': return focus_mode_switch(fm, FOCUS_MODE_ASTROLOGY);
        case 'K': case 'k': return focus_mode_switch(fm, FOCUS_MODE_KIN);
        case 'I': case 'i': return focus_mode_switch(fm, FOCUS_MODE_ICHING);
        case 'C': case 'c': return focus_mode_switch(fm, FOCUS_MODE_CHINESE);
        case 'D': case 'd': return focus_mode_switch(fm, FOCUS_MODE_HD);
        case 27:            return focus_mode_switch(fm, FOCUS_MODE_OVERVIEW);
        default:            return fm;
    }
}

focus_modes_t focus_mode_tick(focus_modes_t fm, float dt)
{
    if (dt <= 0.0f) return fm;
    if (fm.transition_t >= 1.0f) return fm;

    fm.transition_t += dt / fm.transition_dur;
    if (fm.transition_t > 1.0f) fm.transition_t = 1.0f;
    return fm;
}

focus_mode_e focus_mode_active(focus_modes_t fm)
{
    return fm.active;
}

focus_preset_t focus_mode_preset(focus_modes_t fm, int mode)
{
    (void)fm;
    return get_preset(mode);
}

int focus_mode_is_transitioning(focus_modes_t fm)
{
    return fm.transition_t < 1.0f;
}

float focus_mode_transition_progress(focus_modes_t fm)
{
    return fm.transition_t;
}

const char *focus_mode_name(int mode)
{
    focus_preset_t p = get_preset(mode);
    return p.name;
}
