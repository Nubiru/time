/* pawukon_interpret.c — Balinese Pawukon calendar interpretation data
 *
 * Pure data module: static archetype tables + composition.
 * No globals, no malloc, no side effects. */

#include "pawukon_interpret.h"
#include "../../ui/content_i18n.h"
#include <stdio.h>
#include <string.h>

static const pwi_wuku_t WUKUS[30] = {
    {  0, "Sinta",       "Shiva",       "Love, new beginnings, tender emotion",
       "First wuku \xe2\x80\x94 Shiva's week, named for the faithful wife of Rama" },
    {  1, "Landep",      "Vishnu",      "Sharpness, clarity, intellectual brilliance",
       "The sharp week \xe2\x80\x94 keen perception and mental acuity" },
    {  2, "Ukir",        "Brahma",      "Artistry, craftsmanship, creative skill",
       "The carving week \xe2\x80\x94 the creator's hand shapes form from formlessness" },
    {  3, "Kulantir",    "Vishnu",      "Teaching, wisdom transmission, guidance",
       "The teaching week \xe2\x80\x94 knowledge passed from master to student" },
    {  4, "Taulu",       "Shiva",       "Patience, endurance, steady perseverance",
       "The patience week \xe2\x80\x94 the turtle's wisdom of slow progress" },
    {  5, "Gumbreg",     "Brahma",      "Confidence, inner strength, self-assurance",
       "The confident week \xe2\x80\x94 standing firm in one's purpose" },
    {  6, "Wariga",      "Vishnu",      "Good fortune, agricultural blessing, growth",
       "The auspicious week \xe2\x80\x94 the season of planting and favorable outcomes" },
    {  7, "Warigadean",  "Shiva",       "Delayed fortune, patience rewarded later",
       "The second Wariga \xe2\x80\x94 what was planted in Wariga bears fruit here" },
    {  8, "Julungwangi", "Brahma",      "Beauty, harmony, aesthetic perfection",
       "The beautiful week \xe2\x80\x94 the perfection of form in nature and art" },
    {  9, "Sungsang",    "Vishnu",      "Reversal, inversion, seeing from a new angle",
       "The inverted week \xe2\x80\x94 what was hidden is revealed, what was obvious deceives" },
    { 10, "Dungulan",    "Shiva",       "Protection, warding, spiritual defense",
       "The protective week \xe2\x80\x94 a shield against malevolent forces" },
    { 11, "Kuningan",    "Brahma",      "Ancestors return to heaven, golden light",
       "The golden week \xe2\x80\x94 the ancestors descend at Galungan and ascend at Kuningan" },
    { 12, "Langkir",     "Vishnu",      "Rest after ceremony, integration, reflection",
       "The resting week \xe2\x80\x94 quiet absorption after the ancestor ceremonies" },
    { 13, "Medangsia",   "Shiva",       "Danger, caution, alertness to hidden threats",
       "The cautious week \xe2\x80\x94 a time to be alert and avoid unnecessary risks" },
    { 14, "Pujut",       "Brahma",      "Sadness, contemplation, emotional depth",
       "The melancholy week \xe2\x80\x94 depth of feeling as a form of wisdom" },
    { 15, "Pahang",      "Vishnu",      "Openness, vulnerability, transparent speech",
       "The open week \xe2\x80\x94 truth spoken plainly, hearts laid bare" },
    { 16, "Krulut",      "Shiva",       "Entanglement, complexity, interconnection",
       "The tangled week \xe2\x80\x94 the web of relationships that bind and sustain" },
    { 17, "Merakih",     "Brahma",      "Industry, hard work, physical labor",
       "The working week \xe2\x80\x94 the dignity and necessity of honest labor" },
    { 18, "Tambir",      "Vishnu",      "Purification, cleansing, spiritual hygiene",
       "The cleansing week \xe2\x80\x94 washing away accumulated impurity" },
    { 19, "Medangkungan","Shiva",       "Community, gathering, social bonds",
       "The gathering week \xe2\x80\x94 the village assembles, bonds are renewed" },
    { 20, "Matal",       "Brahma",      "Death awareness, mortality, impermanence",
       "The death week \xe2\x80\x94 remembrance that all things pass" },
    { 21, "Uye",         "Vishnu",      "Ceremony, ritual precision, sacred protocol",
       "The ceremonial week \xe2\x80\x94 every gesture and offering in its proper place" },
    { 22, "Menail",      "Shiva",       "Grief, loss, the process of letting go",
       "The grieving week \xe2\x80\x94 sorrow as a doorway to acceptance" },
    { 23, "Perangbakat", "Brahma",      "Departure, travel, movement toward the new",
       "The departure week \xe2\x80\x94 setting out on the journey with intention" },
    { 24, "Bala",        "Vishnu",      "Strength, martial energy, warrior spirit",
       "The strong week \xe2\x80\x94 the warrior's power in service of dharma" },
    { 25, "Ugu",         "Shiva",       "Healing, medicine, restoration of balance",
       "The healing week \xe2\x80\x94 the body and spirit brought back to equilibrium" },
    { 26, "Wayang",      "Brahma",      "Shadow play, illusion, the drama of existence",
       "The puppet week \xe2\x80\x94 the shadow play that mirrors the cosmos" },
    { 27, "Klawu",       "Vishnu",      "Dissolution, endings, the return to source",
       "The dissolving week \xe2\x80\x94 forms returning to the formless" },
    { 28, "Dukut",       "Shiva",       "Grass, humility, smallness with purpose",
       "The grass week \xe2\x80\x94 the smallest blade pushes through the hardest ground" },
    { 29, "Watugunung",  "Brahma",      "Culmination, the peak before the cycle renews",
       "The last wuku \xe2\x80\x94 Saraswati's day, knowledge honored as the cycle completes" }
};

static const pwi_wuku_t INVALID_WUKU = { -1, "?", "?", "?", "?" };

pwi_wuku_t pwi_wuku_data(int wuku)
{
    if (wuku < 0 || wuku > 29) return INVALID_WUKU;
    return WUKUS[wuku];
}

pawukon_interp_t pwi_interpret(int wuku, int wuku_day, int urip,
                               int is_tumpek, int is_kajeng_kliwon)
{
    pawukon_interp_t r;
    memset(&r, 0, sizeof(r));

    pwi_wuku_t w = pwi_wuku_data(wuku);
    if (w.wuku < 0) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    size_t len = strlen(w.name);
    size_t copy = len < 3 ? len : 3;
    memcpy(r.glyph, w.name, copy);
    r.glyph[copy] = '\0';

    if (is_kajeng_kliwon) {
        snprintf(r.glance, sizeof(r.glance),
                 "%s day %d \xe2\x80\x94 Kajeng Kliwon (spirit day)",
                 w.name, wuku_day);
    } else if (is_tumpek) {
        snprintf(r.glance, sizeof(r.glance),
                 "%s day %d \xe2\x80\x94 Tumpek (sacred Saturday)",
                 w.name, wuku_day);
    } else {
        snprintf(r.glance, sizeof(r.glance),
                 "%s day %d \xe2\x80\x94 %s",
                 w.name, wuku_day, w.quality);
    }

    int pos = 0;
    pos += snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                    "Wuku: %s (deity: %s). %s. Urip (spiritual weight): %d.",
                    w.name, w.deity, w.quality, urip);

    if (is_kajeng_kliwon) {
        pos += snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                        " KAJENG KLIWON: the spirit world is close \xe2\x80\x94"
                        " offerings and prayers to maintain balance.");
    }
    if (is_tumpek) {
        snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                 " TUMPEK: sacred Saturday \xe2\x80\x94"
                 " blessings for plants, animals, or crafted objects.");
    }

    return r;
}

int pwi_wuku_count(void) { return 30; }

/* ================================================================
 * Locale-aware interpretation
 * ================================================================ */

pawukon_interp_t pwi_interpret_locale(int wuku, int wuku_day, int urip,
                                      int is_tumpek, int is_kajeng_kliwon,
                                      i18n_locale_t locale)
{
    /* English fast path */
    if (locale == I18N_LOCALE_EN) {
        return pwi_interpret(wuku, wuku_day, urip, is_tumpek, is_kajeng_kliwon);
    }

    pawukon_interp_t r;
    memset(&r, 0, sizeof(r));

    char key[64];

    /* Wuku data */
    snprintf(key, sizeof(key), "pawukon.wuku.%d.name", wuku);
    const char *name = content_get(key, locale);

    snprintf(key, sizeof(key), "pawukon.wuku.%d.deity", wuku);
    const char *deity = content_get(key, locale);

    snprintf(key, sizeof(key), "pawukon.wuku.%d.quality", wuku);
    const char *quality = content_get(key, locale);

    /* Glyph: first 3 chars of wuku name */
    size_t len = strlen(name);
    size_t copy = len < 3 ? len : 3;
    memcpy(r.glyph, name, copy);
    r.glyph[copy] = '\0';

    /* Glance */
    const char *tpl_glance = content_get("pawukon.tpl.glance", locale);
    snprintf(r.glance, sizeof(r.glance), tpl_glance,
             name, wuku_day, quality);

    /* Detail */
    const char *tpl_detail = content_get("pawukon.tpl.detail", locale);
    int written = snprintf(r.detail, sizeof(r.detail), tpl_detail,
                           name, deity, quality, urip);

    if (is_kajeng_kliwon &&
        written > 0 && (size_t)written < sizeof(r.detail)) {
        const char *kk = content_get("pawukon.tpl.kajeng_kliwon", locale);
        int n = snprintf(r.detail + written,
                         sizeof(r.detail) - (size_t)written,
                         " %s", kk);
        if (n > 0) written += n;
    }
    if (is_tumpek &&
        written > 0 && (size_t)written < sizeof(r.detail)) {
        const char *tp = content_get("pawukon.tpl.tumpek", locale);
        snprintf(r.detail + written,
                 sizeof(r.detail) - (size_t)written,
                 " %s", tp);
    }

    return r;
}
