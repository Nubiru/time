/* bahai_interpret.c — Baha'i calendar interpretation data
 *
 * Pure data module: static archetype tables + composition.
 * No globals, no malloc, no side effects. */

#include "bahai_interpret.h"
#include <stdio.h>
#include <string.h>

static const bhi_month_t MONTHS[20] = {
    {  0, "Ayyam-i-Ha", "Days of Ha (intercalary)",
       "Days outside the calendar devoted to hospitality, gift-giving, and charity",
       "The intercalary days \xe2\x80\x94 4 or 5 days of generosity before the Fast" },
    {  1, "Baha", "Splendour",
       "The radiance of God's revelation \xe2\x80\x94 the first attribute, the new year's dawn",
       "Month of Splendour \xe2\x80\x94 Naw-Ruz begins the Baha'i year with light" },
    {  2, "Jalal", "Glory",
       "The majesty and grandeur of God's creation",
       "Month of Glory \xe2\x80\x94 the magnificence that shines through all created things" },
    {  3, "Jamal", "Beauty",
       "The aesthetic perfection manifest in creation and human character",
       "Month of Beauty \xe2\x80\x94 beauty as a reflection of the divine" },
    {  4, "Azamat", "Grandeur",
       "The greatness beyond measure \xe2\x80\x94 awe before the infinite",
       "Month of Grandeur \xe2\x80\x94 the immensity of God's creation beyond comprehension" },
    {  5, "Nur", "Light",
       "Spiritual illumination, the light of knowledge and guidance",
       "Month of Light \xe2\x80\x94 the light that dispels ignorance and reveals truth" },
    {  6, "Rahmat", "Mercy",
       "Divine compassion and forgiveness \xe2\x80\x94 love in action",
       "Month of Mercy \xe2\x80\x94 the tender care of God for every creature" },
    {  7, "Kalimat", "Words",
       "The creative Word of God \xe2\x80\x94 revelation as language, the power of speech",
       "Month of Words \xe2\x80\x94 the Word through which all things were created" },
    {  8, "Kamal", "Perfection",
       "The pursuit of excellence in all things \xe2\x80\x94 completeness without flaw",
       "Month of Perfection \xe2\x80\x94 the divine standard toward which humanity strives" },
    {  9, "Asma", "Names",
       "The names and attributes of God reflected in creation",
       "Month of Names \xe2\x80\x94 every divine attribute has a name, every name a world" },
    { 10, "Izzat", "Might",
       "Spiritual power and strength \xe2\x80\x94 the courage of conviction",
       "Month of Might \xe2\x80\x94 the strength that comes from faith, not force" },
    { 11, "Mashiyyat", "Will",
       "Divine purpose \xe2\x80\x94 surrender to God's plan and active participation in it",
       "Month of Will \xe2\x80\x94 the intersection of human choice and divine purpose" },
    { 12, "Ilm", "Knowledge",
       "The pursuit of truth through science and revelation alike",
       "Month of Knowledge \xe2\x80\x94 the harmony of science and religion, twin wings of one bird" },
    { 13, "Qudrat", "Power",
       "Creative force \xe2\x80\x94 the power to transform self and society",
       "Month of Power \xe2\x80\x94 the divine force that enables transformation" },
    { 14, "Qawl", "Speech",
       "Truthful utterance \xe2\x80\x94 the responsibility and sacredness of words",
       "Month of Speech \xe2\x80\x94 words as vessels of truth or instruments of harm" },
    { 15, "Masa'il", "Questions",
       "The spirit of inquiry \xe2\x80\x94 independent investigation of truth",
       "Month of Questions \xe2\x80\x94 the seeker's duty to question and discover for oneself" },
    { 16, "Sharaf", "Honour",
       "Dignity, nobility, the honor of serving humanity",
       "Month of Honour \xe2\x80\x94 true nobility measured by service, not station" },
    { 17, "Sultan", "Sovereignty",
       "God's supreme authority \xe2\x80\x94 the recognition of a power beyond all powers",
       "Month of Sovereignty \xe2\x80\x94 the king of kings to whom all sovereignty belongs" },
    { 18, "Mulk", "Dominion",
       "God's rule over creation \xe2\x80\x94 the final month before the Fast",
       "Month of Dominion \xe2\x80\x94 the dominion that encompasses all worlds" },
    { 19, "Ala", "Loftiness",
       "The highest aspiration \xe2\x80\x94 the month of fasting, spiritual ascent",
       "Month of Loftiness \xe2\x80\x94 the Fast month, 19 days of sunrise-to-sunset abstinence" }
};

static const bhi_holy_day_t HOLY_DAYS[8] = {
    { 1, "Naw-Ruz",
      "New Year \xe2\x80\x94 1 Baha, the vernal equinox, rebirth and renewal",
      "The Baha'i New Year \xe2\x80\x94 the day the Sun enters Aries, ending the Fast" },
    { 2, "First Day of Ridvan",
      "Declaration of Baha'u'llah in the Garden of Ridvan \xe2\x80\x94 the Most Great Festival",
      "The holiest day \xe2\x80\x94 when Baha'u'llah announced His mission in the garden of paradise" },
    { 3, "Ninth Day of Ridvan",
      "Family of Baha'u'llah joins Him in the garden",
      "The ninth day of Ridvan \xe2\x80\x94 reunion in the garden of declaration" },
    { 4, "Twelfth Day of Ridvan",
      "Departure from the garden \xe2\x80\x94 conclusion of the Most Great Festival",
      "The twelfth and final day of Ridvan \xe2\x80\x94 the 12-day festival concludes" },
    { 5, "Declaration of the Bab",
      "The Bab declares His mission as the Herald of a new age",
      "The Bab's declaration \xe2\x80\x94 the gate opens, the new dispensation begins" },
    { 6, "Ascension of Baha'u'llah",
      "Passing of the Founder \xe2\x80\x94 solemn remembrance at 3 AM",
      "Baha'u'llah's ascension \xe2\x80\x94 the Manifestation of God departs this world" },
    { 7, "Martyrdom of the Bab",
      "Execution of the Bab by firing squad in Tabriz",
      "The Bab's martyrdom \xe2\x80\x94 the Herald gave His life for the promise of a new world" },
    { 8, "Birth of the Bab / Birth of Baha'u'llah",
      "Twin Holy Days celebrating the births of the Twin Manifestations",
      "The Twin Birthdays \xe2\x80\x94 two consecutive days of joy for two consecutive Revelations" }
};

static const bhi_month_t INVALID_MONTH = { -1, "?", "?", "?", "?" };
static const bhi_holy_day_t INVALID_HOLY_DAY = { -1, "?", "?", "?" };

bhi_month_t bhi_month_data(int month)
{
    if (month < 0 || month > 19) return INVALID_MONTH;
    return MONTHS[month];
}

bhi_holy_day_t bhi_holy_day_data(int holy_day)
{
    if (holy_day < 1 || holy_day > 8) return INVALID_HOLY_DAY;
    return HOLY_DAYS[holy_day - 1];
}

bahai_interp_t bhi_interpret(int month, int day, int holy_day)
{
    bahai_interp_t r;
    memset(&r, 0, sizeof(r));

    bhi_month_t m = bhi_month_data(month);
    if (m.month < 0) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    size_t len = strlen(m.arabic);
    size_t copy = len < 3 ? len : 3;
    memcpy(r.glyph, m.arabic, copy);
    r.glyph[copy] = '\0';

    if (holy_day >= 1 && holy_day <= 8) {
        bhi_holy_day_t h = bhi_holy_day_data(holy_day);
        snprintf(r.glance, sizeof(r.glance),
                 "%s (%s) day %d \xe2\x80\x94 %s",
                 m.arabic, m.english, day, h.name);
    } else {
        snprintf(r.glance, sizeof(r.glance),
                 "%s (%s) day %d",
                 m.arabic, m.english, day);
    }

    int pos = 0;
    pos += snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                    "Month: %s (%s). %s.",
                    m.arabic, m.english, m.quality);

    if (holy_day >= 1 && holy_day <= 8) {
        bhi_holy_day_t h = bhi_holy_day_data(holy_day);
        snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                 " Holy Day: %s \xe2\x80\x94 %s.",
                 h.name, h.theme);
    }

    return r;
}

int bhi_month_count(void) { return 20; }
int bhi_holy_day_count(void) { return 8; }
