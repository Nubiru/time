/* human_design_interpret.c — Human Design interpretation data
 *
 * Pure data module: static archetype tables + composition.
 * No globals, no malloc, no side effects. */

#include "human_design_interpret.h"
#include "human_design.h"
#include <stdio.h>
#include <string.h>

/* ================================================================
 * Static type data (5 types)
 * ================================================================ */

static const hdi_type_t TYPES[5] = {
    { 0, "Manifestor",
      "To Inform",
      "Peace",
      "Anger",
      "Closed and repelling \xe2\x80\x94 pushes energy outward to initiate",
      "The initiator who sets things in motion \xe2\x80\x94 designed to act independently and inform others" },

    { 1, "Generator",
      "To Respond",
      "Satisfaction",
      "Frustration",
      "Open and enveloping \xe2\x80\x94 draws life toward it magnetically",
      "The life force of the planet \xe2\x80\x94 designed to find the right work through gut response" },

    { 2, "Manifesting Generator",
      "To Respond, Then Inform",
      "Satisfaction",
      "Frustration",
      "Open and enveloping \xe2\x80\x94 draws life while accelerating toward action",
      "The multi-passionate builder \xe2\x80\x94 responds first, then initiates with sustainable energy" },

    { 3, "Projector",
      "To Wait for the Invitation",
      "Success",
      "Bitterness",
      "Focused and absorbing \xe2\x80\x94 penetrates into the other's aura to see deeply",
      "The guide who sees others clearly \xe2\x80\x94 designed to direct energy, not generate it" },

    { 4, "Reflector",
      "To Wait a Lunar Cycle",
      "Surprise",
      "Disappointment",
      "Resistant and sampling \xe2\x80\x94 reflects the health of the community",
      "The mirror of the community \xe2\x80\x94 designed to evaluate by reflecting the world through a full lunar cycle" }
};

/* Type abbreviations for glyph */
static const char *TYPE_ABBR[5] = {
    "Mft", "Gen", "MaG", "Prj", "Ref"
};

/* ================================================================
 * Static authority data (7 authorities)
 * ================================================================ */

static const hdi_authority_t AUTHORITIES[7] = {
    { 0, "Emotional",
      "Ride the emotional wave \xe2\x80\x94 clarity comes over time, never in the moment",
      "Wait for emotional clarity across highs and lows before deciding",
      "Decisions emerge from emotional patience \xe2\x80\x94 there is no truth in the now" },

    { 1, "Sacral",
      "Listen to the gut response \xe2\x80\x94 an immediate uh-huh or uhn-uhn",
      "A visceral, in-the-body yes or no that bypasses the mind",
      "The body knows before the mind \xe2\x80\x94 respond to life through sacral sounds" },

    { 2, "Splenic",
      "Trust the spontaneous knowing \xe2\x80\x94 the spleen speaks once and softly",
      "A quiet, instantaneous hit of intuition that does not repeat",
      "Survival intelligence speaks in the moment \xe2\x80\x94 trust the first instinct" },

    { 3, "Ego",
      "Honor what the heart truly wants \xe2\x80\x94 willpower aligned with desire",
      "A sense of what you have the will and heart to commit to",
      "The heart center drives commitment \xe2\x80\x94 only promise what the ego can sustain" },

    { 4, "Self-Projected",
      "Hear yourself speak your truth \xe2\x80\x94 the voice reveals direction",
      "Listen to what comes out of your mouth when you speak about the decision",
      "Identity speaks through the voice \xe2\x80\x94 talk it through with trusted others" },

    { 5, "Mental",
      "Use the mind as a sounding board \xe2\x80\x94 no inner authority, rely on environment",
      "Discuss with others and notice what feels right in different environments",
      "No defined inner authority \xe2\x80\x94 decisions emerge through dialogue and environment" },

    { 6, "Lunar",
      "Wait through a full 28-day lunar cycle \xe2\x80\x94 sample all transits before deciding",
      "After a full moon cycle, notice which option consistently felt right",
      "The moon's transit through all 64 gates reveals clarity \xe2\x80\x94 patience is everything" }
};

/* ================================================================
 * Static profile data (12 valid combinations)
 * ================================================================ */

static const hdi_profile_t PROFILES[12] = {
    { 1, 3, "1/3", "Investigator / Martyr",
      "Investigator \xe2\x80\x94 driven to build a secure foundation of knowledge",
      "Martyr \xe2\x80\x94 learns through trial and error, discovering what doesn't work",
      "The deeply experiential researcher \xe2\x80\x94 builds certainty through investigation and breakdowns" },

    { 1, 4, "1/4", "Investigator / Opportunist",
      "Investigator \xe2\x80\x94 needs a solid knowledge foundation before acting",
      "Opportunist \xe2\x80\x94 opportunities flow through personal networks and relationships",
      "The networked scholar \xe2\x80\x94 deep research shared through close community" },

    { 2, 4, "2/4", "Hermit / Opportunist",
      "Hermit \xe2\x80\x94 possesses natural talent that needs calling out by others",
      "Opportunist \xe2\x80\x94 life moves through relationship networks",
      "The naturally gifted friend \xe2\x80\x94 talent emerges when called by the right people" },

    { 2, 5, "2/5", "Hermit / Heretic",
      "Hermit \xe2\x80\x94 natural gifts that require solitude to develop",
      "Heretic \xe2\x80\x94 universalizing projections attract practical solutions from others",
      "The reluctant savior \xe2\x80\x94 called from isolation to deliver practical solutions" },

    { 3, 5, "3/5", "Martyr / Heretic",
      "Martyr \xe2\x80\x94 learns through bonds made and broken, trial and error",
      "Heretic \xe2\x80\x94 projected upon as having universal answers",
      "The resilient problem-solver \xe2\x80\x94 experience-born wisdom that others seek out" },

    { 3, 6, "3/6", "Martyr / Role Model",
      "Martyr \xe2\x80\x94 discovers through experimentation and broken bonds",
      "Role Model \xe2\x80\x94 three life phases: trial (to 30), observer (30-50), exemplar (50+)",
      "The experiential sage \xe2\x80\x94 hard-won wisdom that eventually becomes living example" },

    { 4, 6, "4/6", "Opportunist / Role Model",
      "Opportunist \xe2\x80\x94 influence flows through close community bonds",
      "Role Model \xe2\x80\x94 three life phases: trial, retreat to the roof, then embodied wisdom",
      "The trusted mentor \xe2\x80\x94 community-rooted guidance that matures into role model leadership" },

    { 4, 1, "4/1", "Opportunist / Investigator",
      "Opportunist \xe2\x80\x94 life direction comes through relationships",
      "Investigator \xe2\x80\x94 unconscious need for deep, authoritative foundation",
      "The fixed foundation networker \xe2\x80\x94 once the foundation is set, it does not change" },

    { 5, 1, "5/1", "Heretic / Investigator",
      "Heretic \xe2\x80\x94 others project universal solutions onto you",
      "Investigator \xe2\x80\x94 unconscious need to deeply research before delivering",
      "The practical general \xe2\x80\x94 deeply researched solutions delivered to meet projections" },

    { 5, 2, "5/2", "Heretic / Hermit",
      "Heretic \xe2\x80\x94 projected upon as having the answer",
      "Hermit \xe2\x80\x94 unconscious natural talent that needs withdrawal to recharge",
      "The called savior \xe2\x80\x94 natural talent projected upon by others, needing retreat" },

    { 6, 2, "6/2", "Role Model / Hermit",
      "Role Model \xe2\x80\x94 three life phases: trial, observation, embodiment",
      "Hermit \xe2\x80\x94 unconscious natural gift that flourishes in solitude",
      "The wise recluse \xe2\x80\x94 three-phase maturation with natural gifts that emerge alone" },

    { 6, 3, "6/3", "Role Model / Martyr",
      "Role Model \xe2\x80\x94 three-phase life journey toward authentic embodiment",
      "Martyr \xe2\x80\x94 unconscious drive to discover through trial and error",
      "The experienced optimist \xe2\x80\x94 double trial process that ultimately models authentic living" }
};

/* ================================================================
 * Static center data (9 centers)
 * ================================================================ */

static const hdi_center_theme_t CENTERS[9] = {
    { 0, "Head",
      "Consistent mental pressure to think, question, and wonder",
      "Amplifies others' inspiration \xe2\x80\x94 overwhelmed by questions that aren't yours",
      "Am I trying to answer everybody else's questions?",
      "Pineal gland" },

    { 1, "Ajna",
      "Fixed way of processing and conceptualizing information",
      "Open to all ways of thinking \xe2\x80\x94 wisdom is mental flexibility",
      "Am I trying to convince everyone I'm certain?",
      "Anterior and posterior pituitary" },

    { 2, "Throat",
      "Consistent voice and capacity to manifest through expression",
      "Adapts communication to the audience \xe2\x80\x94 pressure to speak or act prematurely",
      "Am I trying to attract attention?",
      "Thyroid and parathyroid" },

    { 3, "G Center",
      "Fixed sense of identity, direction, and love",
      "Shape-shifts identity depending on environment and companions",
      "Am I still looking for love and direction?",
      "Liver and blood" },

    { 4, "Heart",
      "Reliable willpower and capacity for material promises",
      "Inconsistent willpower \xe2\x80\x94 wisdom is not needing to prove self-worth",
      "Am I still trying to prove myself?",
      "Heart, stomach, gallbladder, thymus" },

    { 5, "Spleen",
      "Consistent intuition, immune response, and survival awareness",
      "Amplifies health and fear signals \xe2\x80\x94 holds on to what should be released",
      "Am I still holding on to what isn't good for me?",
      "Lymphatic system, spleen, T-cells" },

    { 6, "Sacral",
      "Sustainable life force and work energy \xe2\x80\x94 the Generator's engine",
      "Inconsistent energy \xe2\x80\x94 wisdom is knowing when enough is enough",
      "Do I know when enough is enough?",
      "Ovaries and testes" },

    { 7, "Solar Plexus",
      "Emotional wave that brings depth, passion, and feeling to experience",
      "Empathic amplifier \xe2\x80\x94 absorbs and magnifies the emotions of others",
      "Am I avoiding confrontation and truth?",
      "Kidneys, prostate, pancreas, nervous system" },

    { 8, "Root",
      "Consistent adrenal pressure that drives action and momentum",
      "Amplifies stress \xe2\x80\x94 pressure to hurry and be free of the pressure",
      "Am I in a hurry to be free of the pressure?",
      "Adrenal glands" }
};

/* ================================================================
 * Invalid sentinels
 * ================================================================ */

static const hdi_type_t INVALID_TYPE = {
    -1, "?", "?", "?", "?", "?", "?"
};

static const hdi_authority_t INVALID_AUTHORITY = {
    -1, "?", "?", "?", "?"
};

static const hdi_profile_t INVALID_PROFILE = {
    -1, -1, "?", "?", "?", "?", "?"
};

static const hdi_center_theme_t INVALID_CENTER = {
    -1, "?", "?", "?", "?", "?"
};

/* ================================================================
 * Profile lookup helper
 * ================================================================ */

static int profile_index(int line1, int line2)
{
    for (int i = 0; i < 12; i++) {
        if (PROFILES[i].line1 == line1 && PROFILES[i].line2 == line2)
            return i;
    }
    return -1;
}

/* ================================================================
 * Public API
 * ================================================================ */

hdi_type_t hdi_type_data(int type)
{
    if (type < 0 || type > 4) return INVALID_TYPE;
    return TYPES[type];
}

hdi_authority_t hdi_authority_data(int authority)
{
    if (authority < 0 || authority > 6) return INVALID_AUTHORITY;
    return AUTHORITIES[authority];
}

hdi_profile_t hdi_profile_data(int line1, int line2)
{
    int idx = profile_index(line1, line2);
    if (idx < 0) return INVALID_PROFILE;
    return PROFILES[idx];
}

hdi_center_theme_t hdi_center_data(int center)
{
    if (center < 0 || center > 8) return INVALID_CENTER;
    return CENTERS[center];
}

human_design_interp_t hdi_interpret(int type, int authority,
                                    int line1, int line2,
                                    int sun_gate)
{
    human_design_interp_t r;
    memset(&r, 0, sizeof(r));

    hdi_type_t      t = hdi_type_data(type);
    hdi_authority_t a = hdi_authority_data(authority);
    hdi_profile_t   p = hdi_profile_data(line1, line2);

    const char *gate_name = hd_gate_name(sun_gate);
    const char *gate_kw   = hd_gate_keyword(sun_gate);

    /* Glyph: 3-char type abbreviation */
    if (t.type < 0) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
    } else {
        snprintf(r.glyph, sizeof(r.glyph), "%s", TYPE_ABBR[type]);
    }

    /* Glance: "{Type} {profile} — Gate {n}: {name}" */
    if (t.type < 0) {
        snprintf(r.glance, sizeof(r.glance), "?");
    } else {
        snprintf(r.glance, sizeof(r.glance),
                 "%s %s \xe2\x80\x94 Gate %d: %s",
                 t.name, p.name, sun_gate, gate_name);
    }

    /* Detail: full narrative */
    if (t.type < 0) {
        snprintf(r.detail, sizeof(r.detail), "?");
    } else {
        snprintf(r.detail, sizeof(r.detail),
                 "Type: %s. Strategy: %s. Signature: %s. Not-Self: %s. "
                 "Aura: %s. "
                 "Authority: %s \xe2\x80\x94 %s. "
                 "Profile: %s (%s). %s / %s. "
                 "Sun Gate %d: %s (%s). %s.",
                 t.name, t.strategy, t.signature, t.not_self,
                 t.aura,
                 a.name, a.brief,
                 p.name, p.theme, p.conscious_role, p.unconscious_role,
                 sun_gate, gate_name, gate_kw, t.brief);
    }

    return r;
}

int hdi_type_count(void)
{
    return 5;
}

int hdi_authority_count(void)
{
    return 7;
}

int hdi_center_count(void)
{
    return 9;
}
