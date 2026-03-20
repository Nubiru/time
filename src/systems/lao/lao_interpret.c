/* lao_interpret.c -- Lao calendar interpretation data
 *
 * Pure data module: static archetype tables + composition.
 * No globals, no malloc, no side effects. */

#include "lao_interpret.h"
#include <stdio.h>
#include <string.h>

/* ================================================================
 * Static month data (12 months, Lao romanizations)
 * ================================================================ */

static const li_lao_month_t MONTHS[12] = {
    {  1, "Mangkon",      "Makara (Capricorn)",
       "Cool season peak \xe2\x80\x94 the sea-goat month, Mekong mists, quiet temples",
       "January \xe2\x80\x94 named from Capricorn, the cool season wraps the river valleys" },
    {  2, "Kumphaa",      "Kumbha (Aquarius)",
       "Cool season fading \xe2\x80\x94 the water-bearer month, Boun Pha Vet (Vessantara Jataka)",
       "February \xe2\x80\x94 named from Aquarius, monks chant the tale of the Buddha's last birth" },
    {  3, "Miinaa",       "Mina (Pisces)",
       "Hot season begins \xe2\x80\x94 the fish month, dry heat over rice paddies",
       "March \xe2\x80\x94 named from Pisces, the land waits between harvests" },
    {  4, "Meesaa",       "Mesha (Aries)",
       "Hot season peak \xe2\x80\x94 the ram month, Pi Mai Lao water festival",
       "April \xe2\x80\x94 named from Aries, Nang Sangkhan descends as Laos washes away the old year" },
    {  5, "Pheutsaphaa",  "Vrishabha (Taurus)",
       "Hot season end \xe2\x80\x94 the bull month, Boun Bang Fai rockets call the rain",
       "May \xe2\x80\x94 named from Taurus, villages launch rockets to summon the monsoon" },
    {  6, "Mithunaa",     "Mithuna (Gemini)",
       "Rainy season begins \xe2\x80\x94 the twins month, Visakha Boucha (triple anniversary)",
       "June \xe2\x80\x94 named from Gemini, the Buddha's birth, awakening, and parinibbana honored" },
    {  7, "Kawlakot",     "Karkata (Cancer)",
       "Rainy season \xe2\x80\x94 the crab month, Khao Phansa (Buddhist Lent begins)",
       "July \xe2\x80\x94 named from Cancer, monks enter the three-month rains retreat" },
    {  8, "Singhaa",      "Simha (Leo)",
       "Rainy season peak \xe2\x80\x94 the lion month, rice transplanting, Mekong rises",
       "August \xe2\x80\x94 named from Leo, the monsoon feeds the river and the paddies" },
    {  9, "Kanyaa",       "Kanya (Virgo)",
       "Late rainy season \xe2\x80\x94 the maiden month, Khao Phadap Din (honoring ancestors)",
       "September \xe2\x80\x94 named from Virgo, offerings placed at temples for the departed" },
    { 10, "Tulaa",        "Tula (Libra)",
       "Rainy season ending \xe2\x80\x94 the scales month, Ok Phansa (Lent ends), boat racing",
       "October \xe2\x80\x94 named from Libra, Lai Heua Fai fire boats float down the Mekong" },
    { 11, "Phachik",      "Vrischika (Scorpio)",
       "Cool season begins \xe2\x80\x94 the scorpion month, Boun That Luang at the great stupa",
       "November \xe2\x80\x94 named from Scorpio, the nation gathers at That Luang in Vientiane" },
    { 12, "Thanwaa",      "Dhanus (Sagittarius)",
       "Cool season \xe2\x80\x94 the archer month, National Day (December 2), year closing",
       "December \xe2\x80\x94 named from Sagittarius, Laos celebrates its founding and the cool returns" }
};

/* ================================================================
 * Static festival data (3 festivals)
 * ================================================================ */

static const li_lao_festival_t FESTIVALS[3] = {
    { 1, "Pi Mai Lao",
      "Water, renewal, baci \xe2\x80\x94 the Lao New Year cleansing",
      "Pouring water over Buddha images and elders, baci string-tying ceremony, sand stupas at temples, Nang Sangkhan maiden procession in Luang Prabang",
      "Pi Mai \xe2\x80\x94 three days when Laos washes away the old year: Sangkhan Luang (farewell), Sangkhan Nao (the day between), Sangkhan Khuen (new dawn)" },

    { 2, "National Day",
      "Sovereignty, memory \xe2\x80\x94 founding of the Lao People's Democratic Republic",
      "National ceremonies, public holiday, reflection on independence (December 2, 1975)",
      "National Day \xe2\x80\x94 honoring the establishment of the Lao PDR and the long path to sovereignty" },

    { 3, "Liberation Day",
      "Freedom, history \xe2\x80\x94 liberation from French colonial rule",
      "Ceremonies and remembrance of Lao Issara independence declaration (October 12, 1945)",
      "Liberation Day \xe2\x80\x94 the moment Laos declared its independence from colonial administration" }
};

/* ================================================================
 * Invalid sentinels
 * ================================================================ */

static const li_lao_month_t INVALID_MONTH = {
    -1, "?", "?", "?", "?"
};

static const li_lao_festival_t INVALID_FESTIVAL = {
    -1, "?", "?", "?", "?"
};

/* ================================================================
 * Public API
 * ================================================================ */

li_lao_month_t lai_month_data(int month)
{
    if (month < 1 || month > 12) return INVALID_MONTH;
    return MONTHS[month - 1];
}

li_lao_festival_t lai_festival_data(int festival)
{
    if (festival < 1 || festival > 3) return INVALID_FESTIVAL;
    return FESTIVALS[festival - 1];
}

lao_interp_t lai_interpret(int be_year, int month, int day, int festival)
{
    lao_interp_t r;
    memset(&r, 0, sizeof(r));

    li_lao_month_t m = lai_month_data(month);

    if (m.month < 0) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    /* Glyph: first 3 chars of Lao month name */
    size_t len = strlen(m.lao);
    size_t copy = len < 3 ? len : 3;
    memcpy(r.glyph, m.lao, copy);
    r.glyph[copy] = '\0';

    /* Glance */
    if (festival >= 1 && festival <= 3) {
        li_lao_festival_t f = lai_festival_data(festival);
        snprintf(r.glance, sizeof(r.glance),
                 "BE %d %s %d \xe2\x80\x94 %s",
                 be_year, m.lao, day, f.name);
    } else {
        snprintf(r.glance, sizeof(r.glance),
                 "BE %d %s %d",
                 be_year, m.lao, day);
    }

    /* Detail */
    int pos = 0;
    pos += snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                    "BE %d. Month: %s (from Sanskrit %s). %s.",
                    be_year, m.lao, m.origin, m.season);

    if (festival >= 1 && festival <= 3) {
        li_lao_festival_t f = lai_festival_data(festival);
        snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                 " Festival: %s \xe2\x80\x94 %s. %s",
                 f.name, f.theme, f.practice);
    }

    return r;
}

int lai_month_count(void)
{
    return 12;
}

int lai_festival_count(void)
{
    return 3;
}
