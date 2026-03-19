/* text_lod.c — Text Level of Detail (LANGUAGE L1.2)
 *
 * Maps depth tier + system data to appropriately verbose text.
 * Provides phi-timed transitions between depth levels. */

#include "text_lod.h"

/* ===================================================================
 * text_lod_for_system
 * =================================================================== */

text_lod_t text_lod_for_system(const ts_entry_t *entry,
                                const depth_interp_t *interp,
                                depth_tier_t tier)
{
    text_lod_t lod;
    memset(&lod, 0, sizeof(lod));
    lod.tier = tier;

    if (!entry || !entry->active) {
        return lod;
    }

    lod.system_id = (int)entry->system;

    switch (tier) {
    case DEPTH_TIER_GLYPH:
        /* T0: just the glyph symbol, or first few chars of date */
        if (interp && interp->has_data && interp->glyph[0]) {
            snprintf(lod.text, TEXT_LOD_MAX, "%s", interp->glyph);
        } else if (entry->date_str[0]) {
            /* Extract first word as mini-label */
            const char *space = strchr(entry->date_str, ' ');
            if (space) {
                int len = (int)(space - entry->date_str);
                if (len >= TEXT_LOD_MAX) len = TEXT_LOD_MAX - 1;
                memcpy(lod.text, entry->date_str, (size_t)len);
                lod.text[len] = '\0';
            } else {
                snprintf(lod.text, TEXT_LOD_MAX, "%s", entry->date_str);
            }
        }
        break;

    case DEPTH_TIER_GLANCE:
        /* T1: one-line summary */
        if (interp && interp->has_data && interp->glance[0]) {
            snprintf(lod.text, TEXT_LOD_MAX, "%s", interp->glance);
        } else {
            snprintf(lod.text, TEXT_LOD_MAX, "%s", entry->date_str);
        }
        break;

    case DEPTH_TIER_CARD:
        /* T2: date + note context */
        if (interp && interp->has_data && interp->glance[0]) {
            if (entry->note[0]) {
                snprintf(lod.text, TEXT_LOD_MAX, "%s. %s",
                         interp->glance, entry->note);
            } else {
                snprintf(lod.text, TEXT_LOD_MAX, "%s", interp->glance);
            }
        } else {
            if (entry->note[0]) {
                snprintf(lod.text, TEXT_LOD_MAX, "%s — %s",
                         entry->date_str, entry->note);
            } else {
                snprintf(lod.text, TEXT_LOD_MAX, "%s", entry->date_str);
            }
        }
        break;

    case DEPTH_TIER_PANEL:
        /* T3: full detail paragraph */
        if (interp && interp->has_data && interp->detail[0]) {
            int pos = snprintf(lod.text, TEXT_LOD_MAX, "%s", interp->detail);
            if (pos > 0 && entry->note[0] && pos < TEXT_LOD_MAX - 1) {
                snprintf(lod.text + pos, (size_t)(TEXT_LOD_MAX - pos),
                         ". %s", entry->note);
            }
        } else if (interp && interp->has_data && interp->glance[0]) {
            snprintf(lod.text, TEXT_LOD_MAX, "%s", interp->glance);
        } else {
            snprintf(lod.text, TEXT_LOD_MAX, "%s", entry->date_str);
        }
        break;

    case DEPTH_TIER_BOARD:
        /* T4: everything combined */
        {
            int pos = 0;
            if (interp && interp->has_data) {
                if (interp->glance[0]) {
                    int w = snprintf(lod.text + pos, (size_t)(TEXT_LOD_MAX - pos),
                                     "%s", interp->glance);
                    if (w > 0) pos += w;
                }
                if (interp->detail[0] && pos < TEXT_LOD_MAX - 1) {
                    int w = snprintf(lod.text + pos, (size_t)(TEXT_LOD_MAX - pos),
                                     ". %s", interp->detail);
                    if (w > 0) pos += w;
                }
            } else {
                int w = snprintf(lod.text + pos, (size_t)(TEXT_LOD_MAX - pos),
                                 "%s", entry->date_str);
                if (w > 0) pos += w;
            }
            if (entry->note[0] && pos < TEXT_LOD_MAX - 1) {
                snprintf(lod.text + pos, (size_t)(TEXT_LOD_MAX - pos),
                         ". %s", entry->note);
            }
        }
        break;

    default:
        /* Invalid tier: treat as glance */
        snprintf(lod.text, TEXT_LOD_MAX, "%s", entry->date_str);
        break;
    }

    return lod;
}

/* ===================================================================
 * text_lod_transition_init
 * =================================================================== */

void text_lod_transition_init(text_lod_transition_t *tr,
                               depth_tier_t initial_tier)
{
    if (!tr) return;
    memset(tr, 0, sizeof(*tr));
    tr->current_tier = initial_tier;
    tr->target_tier = initial_tier;
    tr->opacity = 1.0;
    tr->transitioning = 0;
    tr->phase = 0;
}

/* ===================================================================
 * text_lod_transition_set_target
 * =================================================================== */

void text_lod_transition_set_target(text_lod_transition_t *tr,
                                     depth_tier_t target)
{
    if (!tr) return;
    if (target == tr->current_tier && !tr->transitioning) return;

    tr->target_tier = target;
    tr->elapsed = 0.0;
    tr->transitioning = 1;
    tr->phase = 0; /* start with fade-out */
}

/* ===================================================================
 * text_lod_transition_update
 * =================================================================== */

void text_lod_transition_update(text_lod_transition_t *tr, double dt)
{
    if (!tr || !tr->transitioning) return;

    tr->elapsed += dt;
    double half = TEXT_LOD_FADE_DURATION * 0.5;

    if (tr->phase == 0) {
        /* Phase 0: fading out current tier */
        double t = tr->elapsed / half;
        if (t >= 1.0) {
            /* Midpoint: switch to new tier, start fading in */
            tr->current_tier = tr->target_tier;
            tr->phase = 1;
            tr->elapsed = tr->elapsed - half; /* carry over excess */
            t = tr->elapsed / half;
            if (t >= 1.0) t = 1.0;
            tr->opacity = t;
        } else {
            tr->opacity = 1.0 - t;
        }
    } else {
        /* Phase 1: fading in new tier */
        double t = tr->elapsed / half;
        if (t >= 1.0) {
            tr->opacity = 1.0;
            tr->transitioning = 0;
            tr->phase = 0;
            tr->elapsed = 0.0;
        } else {
            tr->opacity = t;
        }
    }
}
