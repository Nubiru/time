/* buddhist_interpret.c -- Buddhist time interpretation data
 *
 * All functions are pure (no globals, no malloc, no side effects).
 * Uposatha, kalpa, and month data from Theravada Buddhist tradition. */

#include "buddhist_interpret.h"
#include "../../ui/content_i18n.h"
#include <stdio.h>
#include <string.h>

#define UPOSATHA_COUNT 5
#define KALPA_COUNT    4
#define MONTH_COUNT    12

static const bi_uposatha_t UPOSATHAS[UPOSATHA_COUNT] = {
    { UPOSATHA_NONE, "No Observance",
      "Regular day",
      "An ordinary day in the cycle" },
    { UPOSATHA_NEW_MOON, "New Moon Observance",
      "Monastics recite Patimokkha; laity observe 8 precepts",
      "Darkness as potential — the seed moment before renewal" },
    { UPOSATHA_FULL_MOON, "Full Moon Observance",
      "Most important — full Patimokkha recital, intensive practice",
      "Illumination — the Buddha's birth, enlightenment, and parinibbana all on full moons" },
    { UPOSATHA_FIRST_QUARTER, "Waxing Half-Moon",
      "Lighter observance, meditation and reflection",
      "Growing light — effort and aspiration building" },
    { UPOSATHA_LAST_QUARTER, "Waning Half-Moon",
      "Lighter observance, letting go practice",
      "Releasing — wisdom of impermanence, letting go" }
};

static const bi_kalpa_t KALPAS[KALPA_COUNT] = {
    { KALPA_PHASE_FORMATION, "Formation", "Vivatta",
      "Worlds arise from emptiness — matter condenses, beings take form",
      "The cosmic breath inward — creation as inevitable arising" },
    { KALPA_PHASE_EXISTENCE, "Existence", "Vivattathaayi",
      "Beings live, die, and are reborn — the wheel turns",
      "The present era — sentient experience in all its suffering and beauty" },
    { KALPA_PHASE_DESTRUCTION, "Destruction", "Samvatta",
      "Worlds dissolve through fire, water, or wind",
      "The cosmic breath outward — all conditioned things are impermanent" },
    { KALPA_PHASE_EMPTINESS, "Emptiness", "Samvattathaayi",
      "Vast darkness between worlds — nothing remains",
      "The great rest — potential without form, like dreamless sleep" }
};

static const bi_month_t MONTHS[MONTH_COUNT] = {
    {  1, "Citta",      "Hot",   "Songkran/New Year in some traditions",
       "Month of awakening — nature stirs" },
    {  2, "Vesakha",    "Hot",   "Vesak — Buddha's birth, enlightenment, parinibbana",
       "The most sacred month — triple anniversary" },
    {  3, "Jettha",     "Hot",   "",
       "Month of endurance through heat" },
    {  4, "Asalha",     "Rainy", "Asalha Puja — First Sermon, Dhammacakkappavattana",
       "Month of the first teaching — the Wheel turns" },
    {  5, "Savana",     "Rainy", "Vassa begins — Rain Retreat starts",
       "Month of withdrawal — monastics enter retreat" },
    {  6, "Bhaddapada", "Rainy", "Vassa continues",
       "Month of devoted practice" },
    {  7, "Assayuja",   "Rainy", "Vassa ends — Pavarana ceremony",
       "Month of invitation — community reconciliation" },
    {  8, "Kattika",    "Cool",  "Kathina — robe offering to monastics",
       "Month of generosity — dana and gratitude" },
    {  9, "Maggasira",  "Cool",  "",
       "Month of the path — quiet walking" },
    { 10, "Phussa",     "Cool",  "",
       "Month of nourishment" },
    { 11, "Magha",      "Cool",  "Magha Puja — Sangha Day, 1250 monks gathered",
       "Month of the Sangha — community of awakening" },
    { 12, "Phalguna",   "Hot",   "",
       "Month of completion — cycle prepares to renew" }
};

bi_uposatha_t bi_uposatha_data(uposatha_type_t type)
{
    if ((int)type < 0 || (int)type >= UPOSATHA_COUNT) {
        return UPOSATHAS[0]; /* NONE */
    }
    return UPOSATHAS[(int)type];
}

bi_kalpa_t bi_kalpa_data(kalpa_phase_t phase)
{
    if ((int)phase < 0 || (int)phase >= KALPA_COUNT) {
        bi_kalpa_t invalid = { phase, "?", "?", "?", "?" };
        return invalid;
    }
    return KALPAS[(int)phase];
}

bi_month_t bi_month_data(int month)
{
    if (month < 1 || month > MONTH_COUNT) {
        bi_month_t invalid = { -1, "?", "?", "?", "?" };
        return invalid;
    }
    return MONTHS[month - 1];
}

int bi_uposatha_count(void)
{
    return UPOSATHA_COUNT;
}

int bi_kalpa_phase_count(void)
{
    return KALPA_COUNT;
}

int bi_month_count(void)
{
    return MONTH_COUNT;
}

buddhist_interp_t bi_interpret(int be_year, int month,
                               uposatha_type_t uposatha,
                               kalpa_phase_t kalpa_phase)
{
    buddhist_interp_t result;
    memset(&result, 0, sizeof(result));

    bi_month_t m = bi_month_data(month);
    bi_uposatha_t u = bi_uposatha_data(uposatha);
    bi_kalpa_t k = bi_kalpa_data(kalpa_phase);

    /* T0: glyph — Buddhist Era marker */
    snprintf(result.glyph, sizeof(result.glyph), "BE");

    /* T1: glance — "BE {year} · {month_name} · {uposatha_name or season}" */
    if (uposatha != UPOSATHA_NONE) {
        snprintf(result.glance, sizeof(result.glance),
                 "BE %d · %s · %s",
                 be_year, m.pali_name, u.name);
    } else {
        snprintf(result.glance, sizeof(result.glance),
                 "BE %d · %s · %s",
                 be_year, m.pali_name, m.season);
    }

    /* T3: detail — narrative combining month meaning + uposatha + kalpa */
    int written = snprintf(result.detail, sizeof(result.detail),
                           "%s: %s.",
                           m.pali_name, m.meaning);

    /* Append uposatha practice if observance day */
    if (uposatha != UPOSATHA_NONE && written > 0 &&
        (size_t)written < sizeof(result.detail)) {
        int added = snprintf(result.detail + written,
                             sizeof(result.detail) - (size_t)written,
                             " %s. %s.",
                             u.practice, u.meaning);
        if (added > 0) {
            written += added;
        }
    }

    /* Append kalpa context */
    if (written > 0 && (size_t)written < sizeof(result.detail)) {
        snprintf(result.detail + written,
                 sizeof(result.detail) - (size_t)written,
                 " %s phase (%s): %s.",
                 k.name, k.pali, k.meaning);
    }

    return result;
}

/* ================================================================
 * Locale-aware interpretation
 * ================================================================ */

buddhist_interp_t bi_interpret_locale(int be_year, int month,
                                      uposatha_type_t uposatha,
                                      kalpa_phase_t kalpa_phase,
                                      i18n_locale_t locale)
{
    /* English fast path */
    if (locale == I18N_LOCALE_EN) {
        return bi_interpret(be_year, month, uposatha, kalpa_phase);
    }

    buddhist_interp_t result;
    memset(&result, 0, sizeof(result));

    char key[64];

    /* Month data */
    snprintf(key, sizeof(key), "buddhist.month.%d.pali_name", month);
    const char *pali_name = content_get(key, locale);

    snprintf(key, sizeof(key), "buddhist.month.%d.season", month);
    const char *season = content_get(key, locale);

    snprintf(key, sizeof(key), "buddhist.month.%d.meaning", month);
    const char *meaning = content_get(key, locale);

    /* Uposatha data */
    snprintf(key, sizeof(key), "buddhist.uposatha.%d.name", (int)uposatha);
    const char *u_name = content_get(key, locale);

    snprintf(key, sizeof(key), "buddhist.uposatha.%d.practice", (int)uposatha);
    const char *u_practice = content_get(key, locale);

    snprintf(key, sizeof(key), "buddhist.uposatha.%d.meaning", (int)uposatha);
    const char *u_meaning = content_get(key, locale);

    /* Kalpa data */
    snprintf(key, sizeof(key), "buddhist.kalpa.%d.name", (int)kalpa_phase);
    const char *k_name = content_get(key, locale);

    snprintf(key, sizeof(key), "buddhist.kalpa.%d.pali", (int)kalpa_phase);
    const char *k_pali = content_get(key, locale);

    snprintf(key, sizeof(key), "buddhist.kalpa.%d.meaning", (int)kalpa_phase);
    const char *k_meaning = content_get(key, locale);

    /* T0: glyph — Buddhist Era marker */
    snprintf(result.glyph, sizeof(result.glyph), "BE");

    /* T1: glance */
    if (uposatha != UPOSATHA_NONE) {
        const char *tpl = content_get("buddhist.tpl.glance_uposatha", locale);
        snprintf(result.glance, sizeof(result.glance), tpl,
                 be_year, pali_name, u_name);
    } else {
        const char *tpl = content_get("buddhist.tpl.glance", locale);
        snprintf(result.glance, sizeof(result.glance), tpl,
                 be_year, pali_name, season);
    }

    /* T3: detail — month meaning */
    const char *tpl_detail = content_get("buddhist.tpl.detail", locale);
    int written = snprintf(result.detail, sizeof(result.detail),
                           tpl_detail, pali_name, meaning);

    /* Append uposatha practice if observance day */
    if (uposatha != UPOSATHA_NONE && written > 0 &&
        (size_t)written < sizeof(result.detail)) {
        const char *tpl_upo = content_get("buddhist.tpl.detail_uposatha",
                                          locale);
        int added = snprintf(result.detail + written,
                             sizeof(result.detail) - (size_t)written,
                             tpl_upo, u_practice, u_meaning);
        if (added > 0) {
            written += added;
        }
    }

    /* Append kalpa context */
    if (written > 0 && (size_t)written < sizeof(result.detail)) {
        const char *tpl_kalpa = content_get("buddhist.tpl.detail_kalpa",
                                            locale);
        snprintf(result.detail + written,
                 sizeof(result.detail) - (size_t)written,
                 tpl_kalpa, k_name, k_pali, k_meaning);
    }

    return result;
}
