/* myanmar_interpret.c — Myanmar calendar interpretation data
 *
 * Pure data module: static archetype tables + composition.
 * No globals, no malloc, no side effects. */

#include "myanmar_interpret.h"
#include <stdio.h>
#include <string.h>

static const char *YEAR_TYPE_NAMES[3] = {
    "Common (354 days)", "Small Watat (384 days)", "Big Watat (385 days)"
};

static const mmi_month_t MONTHS[12] = {
    {  0, "Tagu",       "Hot season, Myanmar New Year (mid-April)",
       "Thingyan water festival, Myanmar New Year celebrations",
       "First month \xe2\x80\x94 the great water festival washes away the old year" },
    {  1, "Kason",      "Hottest month, pre-monsoon",
       "Watering the Bodhi tree (Kason full moon, Buddha's enlightenment)",
       "Month of the Bodhi tree \xe2\x80\x94 water poured on sacred trees on the full moon" },
    {  2, "Nayon",      "Early monsoon, first rains",
       "Buddhist examinations, Dhamma study intensifies",
       "Month of study \xe2\x80\x94 the rains begin and monks deepen their learning" },
    {  3, "Waso",       "Monsoon, Buddhist Lent begins",
       "Waso full moon: beginning of Vassa (Lent), robe offering to monks",
       "Month of Lent \xe2\x80\x94 Vassa begins, monks retreat for three months of rain" },
    {  4, "Wagaung",    "Heavy monsoon rains",
       "Drawing lots for robes, spirit propitiation, nat festivals",
       "Month of lots \xe2\x80\x94 communities draw lots to offer robes during the rains" },
    {  5, "Tawthalin",  "Late monsoon, floods receding",
       "Boat racing festivals on rivers and lakes",
       "Month of boat races \xe2\x80\x94 the rivers are full and boats race on the floodwaters" },
    {  6, "Thadingyut", "Post-monsoon, end of Lent",
       "Festival of Lights (Thadingyut full moon), end of Vassa, lighting festivals",
       "Month of lights \xe2\x80\x94 the Buddha descends from heaven, the world lights up to welcome Him" },
    {  7, "Tazaungmon", "Cool season begins",
       "Tazaungdaing festival of lights, robe-weaving competitions",
       "Month of weaving \xe2\x80\x94 all-night robe-weaving contests in the light of the full moon" },
    {  8, "Nadaw",      "Cool season, pleasant weather",
       "Nat spirit festivals, traditional music and dance",
       "Month of nat spirits \xe2\x80\x94 the spirits are honored with music and offerings" },
    {  9, "Pyatho",     "Coolest month",
       "Harvest festival, village feasts, equestrian events",
       "Month of harvest \xe2\x80\x94 the rice is gathered and the villages celebrate with games" },
    { 10, "Tabodwe",    "Cool to warm transition",
       "Htamane (sticky rice) festival, communal cooking",
       "Month of sticky rice \xe2\x80\x94 communities gather to cook htamane together in great pots" },
    { 11, "Tabaung",    "Pre-summer, warming",
       "Sand pagoda building, Shwedagon pagoda festival",
       "Month of pagodas \xe2\x80\x94 sand stupas built on riverbanks, the great Shwedagon festival" }
};

static const mmi_month_t INVALID_MONTH = { -1, "?", "?", "?", "?" };

mmi_month_t mmi_month_data(int month)
{
    if (month < 0 || month > 11) return INVALID_MONTH;
    return MONTHS[month];
}

myanmar_interp_t mmi_interpret(int month, int day, int is_thingyan,
                               int year_type)
{
    myanmar_interp_t r;
    memset(&r, 0, sizeof(r));

    mmi_month_t m = mmi_month_data(month);
    if (m.month < 0) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    size_t len = strlen(m.name);
    size_t copy = len < 3 ? len : 3;
    memcpy(r.glyph, m.name, copy);
    r.glyph[copy] = '\0';

    if (is_thingyan) {
        snprintf(r.glance, sizeof(r.glance),
                 "%s %d \xe2\x80\x94 Thingyan (Water Festival)",
                 m.name, day);
    } else {
        snprintf(r.glance, sizeof(r.glance),
                 "%s %d \xe2\x80\x94 %s",
                 m.name, day, m.season);
    }

    const char *yt = (year_type >= 0 && year_type <= 2)
                     ? YEAR_TYPE_NAMES[year_type] : "?";

    int pos = 0;
    pos += snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                    "Month: %s. %s. Festival: %s. Year type: %s.",
                    m.name, m.season, m.festival, yt);

    if (is_thingyan) {
        snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                 " THINGYAN: the great water festival \xe2\x80\x94"
                 " 4-5 days of ritual cleansing, water throwing,"
                 " merit-making, and joyful renewal.");
    }

    return r;
}

int mmi_month_count(void) { return 12; }
