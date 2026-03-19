/* zoroastrian_interpret.c — Zoroastrian calendar interpretation data
 *
 * Pure data module: static archetype tables + composition.
 * No globals, no malloc, no side effects. */

#include "zoroastrian_interpret.h"
#include "zoroastrian.h"
#include <stdio.h>
#include <string.h>

/* ================================================================
 * Static Amesha Spenta data (7 Holy Immortals)
 * ================================================================ */

static const zi_amesha_t AMESHAS[7] = {
    { 0, "Ahura Mazda (Hormozd)",
      "Wise Lord",
      "The whole of creation \xe2\x80\x94 the supreme creator and source of all good",
      "Light and fire",
      "Wisdom (Mazda)",
      "The uncreated creator \xe2\x80\x94 pure consciousness from which all Amesha Spentas emanate" },

    { 1, "Vohu Manah (Bahman)",
      "Good Mind",
      "Animals and the capacity for moral choice",
      "Animals and living creatures",
      "Righteous thinking",
      "The good mind that discerns truth from lie \xe2\x80\x94 the first thought of creation" },

    { 2, "Asha Vahishta (Ordibehesht)",
      "Best Truth / Best Righteousness",
      "Fire and the cosmic order of truth",
      "Fire",
      "Truth and righteousness (Asha)",
      "The cosmic order that sustains reality \xe2\x80\x94 truth as the foundation of existence" },

    { 3, "Khshathra Vairya (Shahrivar)",
      "Desirable Dominion",
      "Metals, minerals, and the sky",
      "Metals and sky",
      "Righteous power",
      "The just sovereignty that protects the weak \xe2\x80\x94 power in service of truth" },

    { 4, "Spenta Armaiti (Sepandarmaz)",
      "Holy Devotion",
      "Earth, piety, and loving devotion",
      "Earth",
      "Devotion and faith",
      "The holy earth mother \xe2\x80\x94 devotion that grounds the spirit in sacred matter" },

    { 5, "Haurvatat (Khordad)",
      "Wholeness / Perfection",
      "Water and physical well-being",
      "Water",
      "Health and completeness",
      "The waters of wholeness \xe2\x80\x94 perfection as the natural state of uncorrupted creation" },

    { 6, "Ameretat (Amordad)",
      "Immortality",
      "Plants and the deathless spirit",
      "Plants and vegetation",
      "Immortality",
      "The immortality inherent in all living things \xe2\x80\x94 death is the Lie, not the truth" }
};

/* ================================================================
 * Static Gatha day data (5 epagomenal days)
 * ================================================================ */

static const zi_gatha_t GATHAS[5] = {
    { 1, "Ahunavad",
      "The will of the Lord \xe2\x80\x94 the Ahunavar prayer, creation's first utterance",
      "Recitation of the Ahunavar (Yatha Ahu Vairyo), the most sacred prayer",
      "The prayer that set creation in motion \xe2\x80\x94 Ahura Mazda's will made word" },

    { 2, "Ushtavad",
      "Happiness and bliss through Asha \xe2\x80\x94 the joy of righteous living",
      "Meditation on the happiness that comes from living in accordance with truth",
      "The bliss of Asha \xe2\x80\x94 happiness as the natural result of alignment with cosmic truth" },

    { 3, "Spentamad",
      "Holy wisdom and devotion \xe2\x80\x94 the bountiful spirit of Mazda",
      "Devotional practice honoring the bountiful (Spenta) quality of the divine",
      "The bountiful spirit \xe2\x80\x94 Mazda's generosity overflowing into creation" },

    { 4, "Vohukhshathra",
      "Good dominion \xe2\x80\x94 righteous power and the kingdom of truth",
      "Reflection on the ideal governance of truth over lie",
      "The good kingdom \xe2\x80\x94 when truth rules, the world becomes paradise (Frashokereti)" },

    { 5, "Vahishtoisht",
      "The highest prayer \xe2\x80\x94 the final Gatha, pointing toward renovation",
      "The culminating devotion of the year, anticipating the world's renewal",
      "The highest wish \xe2\x80\x94 the year ends with a prayer for the renovation of all existence" }
};

/* ================================================================
 * Static festival data (9 festivals, indexed 1-9)
 * ================================================================ */

static const zi_festival_t FESTIVALS[9] = {
    { 1, "Nowruz",
      "New Day \xe2\x80\x94 rebirth of creation, triumph of light over darkness",
      "Setting the Haft Sin table, jumping over fire, visiting family, wearing new clothes",
      "The new day \xe2\x80\x94 creation begins again with the spring equinox" },

    { 2, "Maidyozarem",
      "Mid-spring \xe2\x80\x94 the creation of the sky, gratitude for growth",
      "Seasonal gathering honoring the sky's creation and the greening earth",
      "First Gahanbar \xe2\x80\x94 the sky was created, and the world turns green beneath it" },

    { 3, "Maidyoshem",
      "Midsummer \xe2\x80\x94 the creation of water, abundance of the longest days",
      "Honoring water and its life-giving power at the height of summer",
      "Second Gahanbar \xe2\x80\x94 water was created, and the world drinks from its abundance" },

    { 4, "Paitishem",
      "Harvest home \xe2\x80\x94 the creation of earth, gratitude for the grain",
      "Thanksgiving feast for the harvest, honoring the earth's generosity",
      "Third Gahanbar \xe2\x80\x94 earth was created, and it gives back everything we planted" },

    { 5, "Ayathrem",
      "Return of the herds \xe2\x80\x94 the creation of plants, preparation for winter",
      "Celebrating the return of livestock from pasture, honoring vegetation",
      "Fourth Gahanbar \xe2\x80\x94 plants were created, and the herds return fed and fattened" },

    { 6, "Maidyarem",
      "Midwinter \xe2\x80\x94 the creation of animals, endurance through the dark",
      "Honoring animal creation in the deepest cold, community warmth",
      "Fifth Gahanbar \xe2\x80\x94 animals were created, and their warmth helps us endure the cold" },

    { 7, "Hamaspathmaidyem",
      "All Souls \xe2\x80\x94 the creation of humanity, honoring the Fravashis (guardian spirits)",
      "The Gatha days: remembering the dead, welcoming ancestor spirits, year-end devotion",
      "Sixth Gahanbar \xe2\x80\x94 humanity was created last and best, and the ancestors return to bless us" },

    { 8, "Yalda",
      "Birth of light \xe2\x80\x94 the longest night yields to the returning sun",
      "All-night vigil with pomegranates, poetry (Hafez), nuts, and family gathering",
      "The night of Mithra's birth \xe2\x80\x94 the longest darkness before the invincible light returns" },

    { 9, "Sadeh",
      "The hundredth day \xe2\x80\x94 fire against the cold, midwinter warmth",
      "Lighting great bonfires, celebrating fire as the weapon against winter's darkness",
      "A hundred days before Nowruz \xe2\x80\x94 the bonfire that defies winter and promises spring" }
};

/* ================================================================
 * Invalid sentinels
 * ================================================================ */

static const zi_amesha_t INVALID_AMESHA = {
    -1, "?", "?", "?", "?", "?", "?"
};

static const zi_gatha_t INVALID_GATHA = {
    -1, "?", "?", "?", "?"
};

static const zi_festival_t INVALID_FESTIVAL = {
    -1, "?", "?", "?", "?"
};

/* ================================================================
 * Public API
 * ================================================================ */

zi_amesha_t zi_amesha_data(int index)
{
    if (index < 0 || index > 6) return INVALID_AMESHA;
    return AMESHAS[index];
}

zi_gatha_t zi_gatha_data(int day)
{
    if (day < 1 || day > 5) return INVALID_GATHA;
    return GATHAS[day - 1];
}

zi_festival_t zi_festival_data(int festival)
{
    if (festival < 1 || festival > 9) return INVALID_FESTIVAL;
    return FESTIVALS[festival - 1];
}

zoroastrian_interp_t zi_interpret(int month, int day, int festival)
{
    zoroastrian_interp_t r;
    memset(&r, 0, sizeof(r));

    /* Gatha days (month 13) */
    if (month == 13) {
        zi_gatha_t g = zi_gatha_data(day);
        if (g.day < 0) {
            snprintf(r.glyph, sizeof(r.glyph), "?");
            snprintf(r.glance, sizeof(r.glance), "?");
            snprintf(r.detail, sizeof(r.detail), "?");
        } else {
            snprintf(r.glyph, sizeof(r.glyph), "Gth");
            snprintf(r.glance, sizeof(r.glance),
                     "Gatha day %d (%s) \xe2\x80\x94 %s",
                     day, g.name, g.theme);
            int pos = 0;
            pos += snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                            "Gatha: %s. %s. Practice: %s.",
                            g.name, g.brief, g.practice);
            if (festival >= 1 && festival <= 9) {
                zi_festival_t f = zi_festival_data(festival);
                snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                         " Festival: %s \xe2\x80\x94 %s. %s",
                         f.name, f.theme, f.practice);
            }
        }
        return r;
    }

    /* Regular months (1-12) */
    if (month < 1 || month > 12) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    const char *month_name = zoro_month_name(month);
    const char *day_name = zoro_day_name(day);

    /* Glyph: first 3 chars of month name */
    size_t len = strlen(month_name);
    size_t copy = len < 3 ? len : 3;
    memcpy(r.glyph, month_name, copy);
    r.glyph[copy] = '\0';

    /* Glance */
    if (festival >= 1 && festival <= 9) {
        zi_festival_t f = zi_festival_data(festival);
        snprintf(r.glance, sizeof(r.glance),
                 "%s day %d (%s) \xe2\x80\x94 %s",
                 month_name, day, day_name, f.name);
    } else {
        snprintf(r.glance, sizeof(r.glance),
                 "%s day %d (%s)",
                 month_name, day, day_name);
    }

    /* Detail */
    int pos = 0;
    pos += snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                    "Month: %s. Day: %s (day %d of 30).",
                    month_name, day_name, day);

    /* First 7 days correspond to Amesha Spentas */
    if (day >= 1 && day <= 7) {
        zi_amesha_t a = zi_amesha_data(day - 1);
        pos += snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                        " Amesha Spenta: %s (%s). Domain: %s. "
                        "Element: %s. Virtue: %s.",
                        a.avestan, a.meaning, a.domain,
                        a.element, a.virtue);
    }

    if (festival >= 1 && festival <= 9) {
        zi_festival_t f = zi_festival_data(festival);
        snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                 " Festival: %s \xe2\x80\x94 %s. %s",
                 f.name, f.theme, f.practice);
    }

    return r;
}

int zi_amesha_count(void)
{
    return 7;
}

int zi_gatha_count(void)
{
    return 5;
}

int zi_festival_count(void)
{
    return 9;
}
