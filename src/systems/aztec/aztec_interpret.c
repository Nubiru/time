/* aztec_interpret.c — Aztec calendar interpretation data
 *
 * Pure data module: static archetype tables + composition.
 * No globals, no malloc, no side effects. */

#include "aztec_interpret.h"
#include <stdio.h>
#include <string.h>

/* ================================================================
 * Static day sign data (20 signs of the Tonalpohualli)
 * ================================================================ */

static const azi_sign_t SIGNS[20] = {
    {  0, "Cipactli",    "Crocodile",    "Tonacatecuhtli",  "East",
       "Beginnings, primal energy, the creative surge",
       "The crocodile \xe2\x80\x94 first sign, the earth monster from whose body the world was made" },
    {  1, "Ehecatl",     "Wind",         "Quetzalcoatl",    "North",
       "Change, breath, spiritual communication",
       "The wind \xe2\x80\x94 the feathered serpent's breath that sweeps across creation" },
    {  2, "Calli",       "House",        "Tepeyollotl",     "West",
       "Shelter, family, rest, the inner world",
       "The house \xe2\x80\x94 the heart of the mountain, protection and introspection" },
    {  3, "Cuetzpalin",  "Lizard",       "Huehuecoyotl",    "South",
       "Agility, survival, adaptability, trickster energy",
       "The lizard \xe2\x80\x94 quick, cunning, surviving by wits in the sun" },
    {  4, "Coatl",       "Serpent",      "Chalchiuhtlicue", "East",
       "Power, sensuality, wisdom coiled in the body",
       "The serpent \xe2\x80\x94 jade-skirted goddess of flowing water and hidden knowledge" },
    {  5, "Miquiztli",   "Death",        "Tecciztecatl",    "North",
       "Transformation, release, the gateway between worlds",
       "Death \xe2\x80\x94 the moon god who sacrificed himself and was reborn as light" },
    {  6, "Mazatl",      "Deer",         "Tlaloc",          "West",
       "Grace, gentleness, the forest's gift, sensitivity",
       "The deer \xe2\x80\x94 gentle creature of the rain god, grace in the wild" },
    {  7, "Tochtli",     "Rabbit",       "Mayahuel",        "South",
       "Fertility, abundance, intoxication, the lunar hare",
       "The rabbit \xe2\x80\x94 the maguey goddess, four hundred rabbits of the moon" },
    {  8, "Atl",         "Water",        "Xiuhtecuhtli",    "East",
       "Purification, emotion, the destructive and nurturing flood",
       "Water \xe2\x80\x94 the turquoise lord of fire who rules through the paradox of opposites" },
    {  9, "Itzcuintli",  "Dog",          "Mictlantecuhtli", "North",
       "Loyalty, guidance to the underworld, the soul's companion",
       "The dog \xe2\x80\x94 guide of the dead across the nine rivers of Mictlan" },
    { 10, "Ozomatli",    "Monkey",       "Xochipilli",      "West",
       "Play, art, dance, the creative spirit unchained",
       "The monkey \xe2\x80\x94 the flower prince, patron of art, music, and sacred games" },
    { 11, "Malinalli",   "Grass",        "Patecatl",        "South",
       "Tenacity, endurance, the medicine that heals and harms",
       "The grass \xe2\x80\x94 twisted fiber that binds and heals, the herbalist's sign" },
    { 12, "Acatl",       "Reed",         "Tezcatlipoca",    "East",
       "Authority, knowledge, the arrow of truth, hollow vessel for spirit",
       "The reed \xe2\x80\x94 the smoking mirror, knowledge that cuts like an obsidian blade" },
    { 13, "Ocelotl",     "Jaguar",       "Tlazolteotl",     "North",
       "Courage, the warrior's heart, night vision, hidden power",
       "The jaguar \xe2\x80\x94 the earth goddess who devours sin and births warriors" },
    { 14, "Cuauhtli",    "Eagle",        "Xipe Totec",      "West",
       "Vision, freedom, the sun warrior, ascent to heaven",
       "The eagle \xe2\x80\x94 the flayed god of spring, the sun's warrior who rises reborn" },
    { 15, "Cozcacuauhtli","Vulture",     "Itzpapalotl",     "South",
       "Wisdom of age, patience, purification through decay",
       "The vulture \xe2\x80\x94 the obsidian butterfly, elder wisdom that transforms death into life" },
    { 16, "Ollin",       "Earthquake",   "Xolotl",          "East",
       "Movement, change, the heartbeat of the Fifth Sun",
       "Earthquake \xe2\x80\x94 the dog-headed twin who carried the sun through the underworld" },
    { 17, "Tecpatl",     "Flint Knife",  "Chalchiuhtotolin", "North",
       "Sacrifice, clarity, the cut that separates truth from illusion",
       "The flint knife \xe2\x80\x94 the jeweled fowl, obsidian that reveals by dividing" },
    { 18, "Quiahuitl",   "Rain",         "Chantico",        "West",
       "Nourishment, cleansing, the storm that feeds and destroys",
       "Rain \xe2\x80\x94 she who dwells in the house, the hearth fire and the storm" },
    { 19, "Xochitl",     "Flower",       "Xochiquetzal",    "South",
       "Beauty, pleasure, art, the flowering of consciousness",
       "The flower \xe2\x80\x94 the quetzal-feathered goddess of love, art, and sacred beauty" }
};

/* ================================================================
 * Static year bearer data (4 year bearers)
 * ================================================================ */

static const azi_bearer_t BEARERS[4] = {
    { 0, "Tochtli", "Rabbit", "South",
      "Abundance, fertility, lunar influence \xe2\x80\x94 a year of growth and celebration",
      "Rabbit years \xe2\x80\x94 the south wind brings abundance and the danger of excess" },
    { 1, "Acatl",   "Reed",   "East",
      "Authority, knowledge, beginnings \xe2\x80\x94 a year of leadership and destiny",
      "Reed years \xe2\x80\x94 the east wind brings vision and the burden of power" },
    { 2, "Tecpatl", "Flint",  "North",
      "Sacrifice, truth, hardship \xe2\x80\x94 a year of testing and purification",
      "Flint years \xe2\x80\x94 the north wind brings clarity through the cold blade of truth" },
    { 3, "Calli",   "House",  "West",
      "Rest, family, introspection \xe2\x80\x94 a year of shelter and inner work",
      "House years \xe2\x80\x94 the west wind brings the comfort of home and the labor of building" }
};

/* ================================================================
 * Invalid sentinels
 * ================================================================ */

static const azi_sign_t INVALID_SIGN = {
    -1, "?", "?", "?", "?", "?", "?"
};

static const azi_bearer_t INVALID_BEARER = {
    -1, "?", "?", "?", "?", "?"
};

/* ================================================================
 * Public API
 * ================================================================ */

azi_sign_t azi_sign_data(int sign)
{
    if (sign < 0 || sign > 19) return INVALID_SIGN;
    return SIGNS[sign];
}

azi_bearer_t azi_bearer_data(int bearer)
{
    if (bearer < 0 || bearer > 3) return INVALID_BEARER;
    return BEARERS[bearer];
}

aztec_interp_t azi_interpret(int day_sign, int day_number,
                             int year_bearer, int year_number)
{
    aztec_interp_t r;
    memset(&r, 0, sizeof(r));

    azi_sign_t s = azi_sign_data(day_sign);
    if (s.sign < 0) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    /* Glyph: first 3 chars of Nahuatl name */
    size_t len = strlen(s.nahuatl);
    size_t copy = len < 3 ? len : 3;
    memcpy(r.glyph, s.nahuatl, copy);
    r.glyph[copy] = '\0';

    /* Glance */
    azi_bearer_t b = azi_bearer_data(year_bearer);
    if (b.bearer >= 0) {
        snprintf(r.glance, sizeof(r.glance),
                 "%d %s \xe2\x80\x94 Year %d %s",
                 day_number, s.english, year_number, b.english);
    } else {
        snprintf(r.glance, sizeof(r.glance),
                 "%d %s \xe2\x80\x94 %s",
                 day_number, s.english, s.quality);
    }

    /* Detail */
    int pos = 0;
    pos += snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                    "Day: %d %s (%s). Patron: %s. Direction: %s. %s.",
                    day_number, s.nahuatl, s.english, s.deity, s.direction,
                    s.quality);

    if (b.bearer >= 0) {
        snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                 " Year: %d %s (%s). %s",
                 year_number, b.nahuatl, b.english, b.quality);
    }

    return r;
}

int azi_sign_count(void) { return 20; }
int azi_bearer_count(void) { return 4; }
