/* french_republican_interpret.c — French Republican calendar interpretation
 *
 * Pure data module: static archetype tables + composition.
 * No globals, no malloc, no side effects. */

#include "french_republican_interpret.h"
#include <stdio.h>
#include <string.h>

static const fri_month_t MONTHS[12] = {
    {  1, "Vendemiaire", "Grape Harvest", "Autumn begins",
       "Grapes heavy on the vine, wine-making, the harvest of intoxication",
       "Month of the grape harvest \xe2\x80\x94 the Revolution began at vintage time" },
    {  2, "Brumaire",          "Fog",           "Autumn deepens",
       "Morning mists, the world hidden and revealed, the breath of cooling earth",
       "Month of fog \xe2\x80\x94 Napoleon seized power on 18 Brumaire" },
    {  3, "Frimaire",          "Frost",         "Late autumn",
       "First frost, fields whitening at dawn, the earth hardening",
       "Month of frost \xe2\x80\x94 the cold that ends the growing season and begins the waiting" },
    {  4, "Nivose",    "Snow",          "Early winter",
       "Snow covering the land, silence, the white blanket of rest",
       "Month of snow \xe2\x80\x94 the world gone white and still" },
    {  5, "Pluviose",  "Rain",          "Midwinter",
       "Winter rains, the persistence of water, melting at the margins",
       "Month of rain \xe2\x80\x94 the waters that dissolve winter and feed spring" },
    {  6, "Ventose",   "Wind",          "Late winter",
       "March winds, the last storms before spring, the world stirring",
       "Month of wind \xe2\x80\x94 the breath that sweeps away winter and clears the air" },
    {  7, "Germinal",          "Germination",   "Early spring",
       "Seeds sprouting, first green shoots, the earth opening",
       "Month of germination \xe2\x80\x94 the name Zola chose for revolution rising from the earth" },
    {  8, "Floreal",   "Flower",        "Mid-spring",
       "Flowers everywhere, orchards in blossom, the extravagance of spring",
       "Month of flowers \xe2\x80\x94 nature's most lavish display of color and scent" },
    {  9, "Prairial",          "Meadow",        "Late spring",
       "Meadow grasses tall, haymaking, the green abundance",
       "Month of meadows \xe2\x80\x94 the fields at their greenest, ready for the scythe" },
    { 10, "Messidor",          "Harvest",       "Early summer",
       "Grain harvest, golden fields, the reward of the year's labor",
       "Month of harvest \xe2\x80\x94 the golden reward of patience and labor" },
    { 11, "Thermidor",         "Heat",          "Midsummer",
       "Peak heat, parching sun, the coup that ended the Terror",
       "Month of heat \xe2\x80\x94 9 Thermidor ended Robespierre and the Reign of Terror" },
    { 12, "Fructidor",         "Fruit",         "Late summer",
       "Fruit ripening, orchards heavy, the sweet conclusion of summer",
       "Month of fruit \xe2\x80\x94 the year's sweetest gifts before the cycle turns again" }
};

static const fri_sans_t SANS[6] = {
    { 1, "La Fete de la Vertu",       "Virtue",
      "Day of Virtue \xe2\x80\x94 moral excellence, the foundation of the Republic" },
    { 2, "La Fete du Genie",   "Genius",
      "Day of Genius \xe2\x80\x94 creative brilliance, reason's triumph over superstition" },
    { 3, "La Fete du Travail",         "Labor",
      "Day of Labor \xe2\x80\x94 the dignity of work, the hands that build the nation" },
    { 4, "La Fete de l'Opinion",       "Opinion",
      "Day of Opinion \xe2\x80\x94 free speech, the right to think and speak without fear" },
    { 5, "La Fete des Recompenses", "Rewards",
      "Day of Rewards \xe2\x80\x94 honoring those who served the Republic with distinction" },
    { 6, "La Fete de la Revolution", "Revolution",
      "Day of Revolution \xe2\x80\x94 the leap year day, celebrating the act that changed the world" }
};

static const fri_month_t INVALID_MONTH = { -1, "?", "?", "?", "?", "?" };
static const fri_sans_t INVALID_SANS = { -1, "?", "?", "?" };

fri_month_t fri_month_data(int month)
{
    if (month < 1 || month > 12) return INVALID_MONTH;
    return MONTHS[month - 1];
}

fri_sans_t fri_sans_data(int day)
{
    if (day < 1 || day > 6) return INVALID_SANS;
    return SANS[day - 1];
}

french_republican_interp_t fri_interpret(int rep_year, int month, int day)
{
    french_republican_interp_t r;
    memset(&r, 0, sizeof(r));

    /* Sansculottides (month 0) */
    if (month == 0) {
        fri_sans_t s = fri_sans_data(day);
        if (s.day < 0) {
            snprintf(r.glyph, sizeof(r.glyph), "?");
            snprintf(r.glance, sizeof(r.glance), "?");
            snprintf(r.detail, sizeof(r.detail), "?");
        } else {
            snprintf(r.glyph, sizeof(r.glyph), "San");
            snprintf(r.glance, sizeof(r.glance),
                     "An %d \xe2\x80\x94 %s (%s)",
                     rep_year, s.french, s.virtue);
            snprintf(r.detail, sizeof(r.detail),
                     "An %d, Sansculottide %d. %s. %s.",
                     rep_year, day, s.french, s.brief);
        }
        return r;
    }

    fri_month_t m = fri_month_data(month);
    if (m.month < 0) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    /* Glyph: first 3 chars of French name */
    size_t len = strlen(m.french);
    size_t copy = len < 3 ? len : 3;
    memcpy(r.glyph, m.french, copy);
    r.glyph[copy] = '\0';

    snprintf(r.glance, sizeof(r.glance),
             "An %d %s %d \xe2\x80\x94 %s",
             rep_year, m.french, day, m.english);

    snprintf(r.detail, sizeof(r.detail),
             "An %d, %s (Month of %s), day %d. %s. Nature: %s.",
             rep_year, m.french, m.english, day, m.season, m.nature);

    return r;
}

int fri_month_count(void) { return 12; }
int fri_sans_count(void) { return 6; }
