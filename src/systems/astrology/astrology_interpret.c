/* astrology_interpret.c — Zodiac sign interpretation data
 *
 * Static data for 12 zodiac archetypes, 4 elements, 3 modalities.
 * Tiered composition for Sun/Moon/Ascendant configuration.
 * Jungian archetypal framework. */

#include "astrology_interpret.h"
#include "../../ui/content_i18n.h"
#include <stdio.h>
#include <string.h>

/* ===== Element index from name ===== */

static int element_index(const char *name)
{
    if (strcmp(name, "Fire")  == 0) return 0;
    if (strcmp(name, "Earth") == 0) return 1;
    if (strcmp(name, "Air")   == 0) return 2;
    if (strcmp(name, "Water") == 0) return 3;
    return -1;
}

/* ===== Modality index from name ===== */

static int modality_index(const char *name)
{
    if (strcmp(name, "Cardinal") == 0) return 0;
    if (strcmp(name, "Fixed")    == 0) return 1;
    if (strcmp(name, "Mutable")  == 0) return 2;
    return -1;
}

/* ===== Static sign data ===== */

static const ai_sign_t SIGNS[12] = {
    { 0, "Aries", "Fire", "Cardinal", "Mars", "The Warrior",
      "Courage, initiative, independence",
      "Impatience, aggression, selfishness",
      "The spark that begins \xe2\x80\x94 raw courage to exist" },
    { 1, "Taurus", "Earth", "Fixed", "Venus", "The Builder",
      "Stability, sensuality, persistence",
      "Stubbornness, possessiveness, inertia",
      "The earth that endures \xe2\x80\x94 patient cultivation of beauty" },
    { 2, "Gemini", "Air", "Mutable", "Mercury", "The Messenger",
      "Communication, curiosity, adaptability",
      "Superficiality, restlessness, duplicity",
      "The breath between \xe2\x80\x94 connecting through language" },
    { 3, "Cancer", "Water", "Cardinal", "Moon", "The Mother",
      "Nurture, emotion, protection",
      "Clinginess, moodiness, manipulation",
      "The shell that protects \xe2\x80\x94 feeling as a form of knowing" },
    { 4, "Leo", "Fire", "Fixed", "Sun", "The King",
      "Creativity, generosity, self-expression",
      "Pride, drama, dominance",
      "The fire that sustains \xe2\x80\x94 the courage to shine" },
    { 5, "Virgo", "Earth", "Mutable", "Mercury", "The Healer",
      "Analysis, service, refinement",
      "Criticism, anxiety, perfectionism",
      "The hand that mends \xe2\x80\x94 wholeness through discernment" },
    { 6, "Libra", "Air", "Cardinal", "Venus", "The Diplomat",
      "Harmony, partnership, justice",
      "Indecision, people-pleasing, avoidance",
      "The scales that balance \xe2\x80\x94 beauty as equilibrium" },
    { 7, "Scorpio", "Water", "Fixed", "Pluto", "The Alchemist",
      "Transformation, intensity, depth",
      "Obsession, jealousy, destruction",
      "The depth that transforms \xe2\x80\x94 death as doorway" },
    { 8, "Sagittarius", "Fire", "Mutable", "Jupiter", "The Explorer",
      "Philosophy, adventure, truth-seeking",
      "Excess, dogmatism, restlessness",
      "The arrow that seeks \xe2\x80\x94 meaning beyond the horizon" },
    { 9, "Capricorn", "Earth", "Cardinal", "Saturn", "The Elder",
      "Ambition, discipline, mastery",
      "Coldness, rigidity, ruthlessness",
      "The mountain that stands \xe2\x80\x94 authority earned through time" },
    { 10, "Aquarius", "Air", "Fixed", "Uranus", "The Visionary",
      "Innovation, humanity, independence",
      "Detachment, rebellion, eccentricity",
      "The wave that carries \xe2\x80\x94 freedom in service of all" },
    { 11, "Pisces", "Water", "Mutable", "Neptune", "The Mystic",
      "Compassion, imagination, transcendence",
      "Escapism, confusion, martyrdom",
      "The ocean that dissolves \xe2\x80\x94 unity beyond separation" }
};

/* ===== Static element data ===== */

static const ai_element_t ELEMENTS[4] = {
    { "Fire",  "Will, spirit, creative force",
      "Through action, inspiration, and enthusiasm" },
    { "Earth", "Form, matter, practical wisdom",
      "Through building, sustaining, and manifesting" },
    { "Air",   "Mind, communication, relationship",
      "Through thinking, connecting, and exchanging" },
    { "Water", "Feeling, intuition, the unconscious",
      "Through emotion, empathy, and transformation" }
};

/* ===== Static modality data ===== */

static const ai_modality_t MODALITIES[3] = {
    { "Cardinal", "Initiation, leadership, new beginnings",
      "Begins the season \xe2\x80\x94 the spark that starts" },
    { "Fixed",    "Sustaining, determination, depth",
      "Holds the season \xe2\x80\x94 the strength that endures" },
    { "Mutable",  "Adaptation, flexibility, transition",
      "Ends the season \xe2\x80\x94 the wisdom that releases" }
};

/* ===== 3-letter abbreviations ===== */

static const char *SIGN_ABBR[12] = {
    "Ari", "Tau", "Gem", "Can", "Leo", "Vir",
    "Lib", "Sco", "Sag", "Cap", "Aqu", "Pis"
};

/* ===== Invalid sentinel ===== */

static const ai_sign_t INVALID_SIGN = {
    -1, "?", "?", "?", "?", "?", "?", "?", "?"
};

static const ai_element_t INVALID_ELEMENT = { "?", "?", "?" };
static const ai_modality_t INVALID_MODALITY = { "?", "?", "?" };

/* ===== Public functions ===== */

ai_sign_t ai_sign_data(int sign)
{
    if (sign < 0 || sign > 11) return INVALID_SIGN;
    return SIGNS[sign];
}

ai_element_t ai_element_data(int element)
{
    if (element < 0 || element > 3) return INVALID_ELEMENT;
    return ELEMENTS[element];
}

ai_modality_t ai_modality_data(int modality)
{
    if (modality < 0 || modality > 2) return INVALID_MODALITY;
    return MODALITIES[modality];
}

int ai_sign_count(void)  { return 12; }
int ai_element_count(void) { return 4; }
int ai_modality_count(void) { return 3; }

astrology_interp_t ai_interpret(int sun_sign, int moon_sign, int asc_sign)
{
    astrology_interp_t r;
    memset(&r, 0, sizeof(r));

    /* Validate required inputs */
    if (sun_sign < 0 || sun_sign > 11 || moon_sign < 0 || moon_sign > 11) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    ai_sign_t sun  = SIGNS[sun_sign];
    ai_sign_t moon = SIGNS[moon_sign];
    int ei = element_index(sun.element);
    int mi = modality_index(sun.modality);
    ai_element_t  elem = (ei >= 0) ? ELEMENTS[ei]    : INVALID_ELEMENT;
    ai_modality_t mod  = (mi >= 0) ? MODALITIES[mi]  : INVALID_MODALITY;

    /* T0: glyph — 3-letter abbreviation of sun sign */
    snprintf(r.glyph, sizeof(r.glyph), "%s", SIGN_ABBR[sun_sign]);

    /* T1: glance — "Sun in X · Moon in Y[ · Z Rising]" */
    if (asc_sign >= 0 && asc_sign <= 11) {
        ai_sign_t asc = SIGNS[asc_sign];
        snprintf(r.glance, sizeof(r.glance),
                 "Sun in %s \xC2\xB7 Moon in %s \xC2\xB7 %s Rising",
                 sun.name, moon.name, asc.name);
    } else {
        snprintf(r.glance, sizeof(r.glance),
                 "Sun in %s \xC2\xB7 Moon in %s",
                 sun.name, moon.name);
    }

    /* T3: detail — full narrative */
    int pos = 0;
    pos += snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                    "%s Sun: %s. %s. Keywords: %s. Shadow: %s. "
                    "Element: %s (%s). Modality: %s (%s). "
                    "Moon in %s: emotional nature is %s.",
                    sun.name, sun.archetype, sun.brief,
                    sun.keywords, sun.shadow,
                    sun.element, elem.quality,
                    sun.modality, mod.role,
                    moon.name, moon.archetype);

    if (asc_sign >= 0 && asc_sign <= 11) {
        ai_sign_t asc = SIGNS[asc_sign];
        snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                 " Ascendant in %s: the mask is %s.",
                 asc.name, asc.archetype);
    }

    return r;
}

/* ================================================================
 * Locale-aware interpretation
 * ================================================================ */

astrology_interp_t ai_interpret_locale(int sun_sign, int moon_sign,
                                       int asc_sign, i18n_locale_t locale)
{
    /* English fast path */
    if (locale == I18N_LOCALE_EN) {
        return ai_interpret(sun_sign, moon_sign, asc_sign);
    }

    astrology_interp_t r;
    memset(&r, 0, sizeof(r));

    if (sun_sign < 0 || sun_sign > 11 ||
        moon_sign < 0 || moon_sign > 11) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    char key[64];

    /* Sun sign data */
    snprintf(key, sizeof(key), "astrology.sign.%d.name", sun_sign);
    const char *sun_name = content_get(key, locale);

    snprintf(key, sizeof(key), "astrology.sign.%d.abbr", sun_sign);
    const char *sun_abbr = content_get(key, locale);

    snprintf(key, sizeof(key), "astrology.sign.%d.archetype", sun_sign);
    const char *sun_arch = content_get(key, locale);

    snprintf(key, sizeof(key), "astrology.sign.%d.brief", sun_sign);
    const char *sun_brief = content_get(key, locale);

    snprintf(key, sizeof(key), "astrology.sign.%d.keywords", sun_sign);
    const char *sun_kw = content_get(key, locale);

    snprintf(key, sizeof(key), "astrology.sign.%d.shadow", sun_sign);
    const char *sun_shadow = content_get(key, locale);

    snprintf(key, sizeof(key), "astrology.sign.%d.element", sun_sign);
    const char *sun_elem_name = content_get(key, locale);

    snprintf(key, sizeof(key), "astrology.sign.%d.modality", sun_sign);
    const char *sun_mod_name = content_get(key, locale);

    /* Element quality from sun sign's element index */
    int ei = element_index(SIGNS[sun_sign].element);
    const char *elem_quality = "?";
    if (ei >= 0) {
        snprintf(key, sizeof(key), "astrology.element.%d.quality", ei);
        elem_quality = content_get(key, locale);
    }

    /* Modality role from sun sign's modality index */
    int mi = modality_index(SIGNS[sun_sign].modality);
    const char *mod_role = "?";
    if (mi >= 0) {
        snprintf(key, sizeof(key), "astrology.modality.%d.role", mi);
        mod_role = content_get(key, locale);
    }

    /* Moon sign data */
    snprintf(key, sizeof(key), "astrology.sign.%d.name", moon_sign);
    const char *moon_name = content_get(key, locale);

    snprintf(key, sizeof(key), "astrology.sign.%d.archetype", moon_sign);
    const char *moon_arch = content_get(key, locale);

    /* T0: glyph — abbreviation of sun sign */
    snprintf(r.glyph, sizeof(r.glyph), "%s", sun_abbr);

    /* T1: glance */
    if (asc_sign >= 0 && asc_sign <= 11) {
        snprintf(key, sizeof(key), "astrology.sign.%d.name", asc_sign);
        const char *asc_name = content_get(key, locale);
        const char *tpl = content_get("astrology.tpl.glance_asc", locale);
        snprintf(r.glance, sizeof(r.glance), tpl,
                 sun_name, moon_name, asc_name);
    } else {
        const char *tpl = content_get("astrology.tpl.glance", locale);
        snprintf(r.glance, sizeof(r.glance), tpl, sun_name, moon_name);
    }

    /* T3: detail */
    const char *tpl_detail = content_get("astrology.tpl.detail", locale);
    int pos = snprintf(r.detail, sizeof(r.detail), tpl_detail,
                       sun_name, sun_arch, sun_brief, sun_kw, sun_shadow,
                       sun_elem_name, elem_quality,
                       sun_mod_name, mod_role,
                       moon_name, moon_arch);

    if (asc_sign >= 0 && asc_sign <= 11 &&
        pos > 0 && (size_t)pos < sizeof(r.detail)) {
        snprintf(key, sizeof(key), "astrology.sign.%d.name", asc_sign);
        const char *asc_name = content_get(key, locale);
        snprintf(key, sizeof(key), "astrology.sign.%d.archetype", asc_sign);
        const char *asc_arch = content_get(key, locale);
        const char *tpl_asc = content_get("astrology.tpl.detail_asc", locale);
        snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                 tpl_asc, asc_name, asc_arch);
    }

    return r;
}
