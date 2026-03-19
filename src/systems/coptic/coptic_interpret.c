/* coptic_interpret.c — Coptic calendar interpretation data
 *
 * Pure data module: static archetype tables + composition.
 * No globals, no malloc, no side effects. */

#include "coptic_interpret.h"
#include <stdio.h>
#include <string.h>

/* ================================================================
 * Static month data (13 months)
 * ================================================================ */

static const ci_coptic_month_t MONTHS[13] = {
    {  1, "Thout",     "From Egyptian Thoth (Djehuty), god of wisdom and writing",
       "Start of the Coptic year, commemorating the martyrs",
       "Month of Thoth \xe2\x80\x94 the ancient scribe god opens the new year of the martyrs" },
    {  2, "Paopi",     "From Egyptian Ipet (Opet festival), sacred barque procession",
       "Season of planting, parish celebrations",
       "Month of Paopi \xe2\x80\x94 the sacred festival that honored the river's gift" },
    {  3, "Hathor",    "From Egyptian Hathor, goddess of love, joy, and the sky",
       "Advent preparation begins in the Coptic tradition",
       "Month of Hathor \xe2\x80\x94 the cow-goddess of joy gives way to Advent expectation" },
    {  4, "Koiak",     "From Egyptian Ka-her-Ka (soul upon soul), Osiris mysteries",
       "Coptic Advent fast, preparation for Christmas (29 Koiak)",
       "Month of Koiak \xe2\x80\x94 the ancient Osiris rites become the fast before Christ's nativity" },
    {  5, "Tobi",      "From Egyptian Djebaty, associated with purification",
       "Theophany/Epiphany season, baptismal celebrations",
       "Month of Tobi \xe2\x80\x94 the waters of purification and the baptism of Christ" },
    {  6, "Meshir",    "From Egyptian Mekhir, month of the burning sun",
       "Ordinary time, parish festivals of local saints",
       "Month of Meshir \xe2\x80\x94 the burning sun that once parched Egypt now warms the faithful" },
    {  7, "Paremhat",  "From Egyptian Pa-en-Amenhotep (Imhotep), architect-healer",
       "The Annunciation (29 Paremhat), feast of the Theotokos",
       "Month of Paremhat \xe2\x80\x94 the builder-healer's month crowned by the angel's greeting" },
    {  8, "Parmouti",  "From Egyptian Pa-en-Renenutet, harvest goddess",
       "Great Lent deepens, approaching Holy Week",
       "Month of Parmouti \xe2\x80\x94 the harvest goddess meets the Lenten fast" },
    {  9, "Pashons",   "From Egyptian Pa-en-Khonsu, moon god of passage",
       "Paschal season, Easter and its afterglow",
       "Month of Pashons \xe2\x80\x94 the moon god's month receives the light of resurrection" },
    { 10, "Paoni",     "From Egyptian Pa-en-Inet (valley festival), honoring the dead",
       "Apostles' Fast begins, honoring Peter and Paul",
       "Month of Paoni \xe2\x80\x94 the ancient valley of the dead becomes the apostolic fast" },
    { 11, "Epip",      "From Egyptian Ipip, associated with the great heat",
       "Summer height, commemoration of saints and martyrs",
       "Month of Epip \xe2\x80\x94 the fierce heat that tested both ancient Egypt and early Christians" },
    { 12, "Mesori",    "From Egyptian Wep-Renpet (birth of Ra), the sun reborn",
       "Assumption of the Virgin Mary fast and feast",
       "Month of Mesori \xe2\x80\x94 the rebirth of Ra becomes the Assumption of the Theotokos" },
    { 13, "Nasie",     "Epagomenal days (Greek epagomenai), outside the regular months",
       "Days of prayer and reflection between years, no fasting",
       "The little month \xe2\x80\x94 five or six days suspended between years, sacred pause" }
};

/* ================================================================
 * Static feast data (6 feasts)
 * ================================================================ */

static const ci_coptic_feast_t FEASTS[6] = {
    { 1, "Nayrouz",
      "New Year of the Martyrs \xe2\x80\x94 remembrance of those who died for faith",
      "Red vestments, red dates, lighting candles for the martyrs, visiting churches",
      "The Coptic New Year \xe2\x80\x94 1 Thout, colored red for the blood of the martyrs" },

    { 2, "Feast of the Cross",
      "Discovery of the True Cross by Empress Helena in Jerusalem",
      "Lighting bonfires (demera), singing hymns, outdoor celebration on 17 Thout",
      "The Cross found \xe2\x80\x94 bonfires mirror the flames Helena lit to find the buried wood" },

    { 3, "Christmas",
      "The Nativity of Christ \xe2\x80\x94 culmination of the 43-day Advent fast",
      "Midnight liturgy (29 Koiak / January 7), breaking the fast with festive foods",
      "The Word made flesh \xe2\x80\x94 after 43 days of fasting, the Light enters the world" },

    { 4, "Epiphany",
      "Theophany \xe2\x80\x94 the Baptism of Christ in the Jordan, God revealed",
      "Blessing of waters, baptismal renewal liturgy on 11 Tobi (January 19)",
      "God revealed in water \xe2\x80\x94 the dove descends, the voice speaks, the Trinity appears" },

    { 5, "Annunciation",
      "The angel Gabriel announces to the Virgin Mary \xe2\x80\x94 the Incarnation begins",
      "Joyful liturgy on 29 Paremhat (April 7), special hymns to the Theotokos",
      "The angel's greeting \xe2\x80\x94 the moment when heaven asked earth's permission" },

    { 6, "Easter",
      "The Resurrection of Christ \xe2\x80\x94 the feast of feasts, joy of joys",
      "55-day Great Lent, midnight Paschal liturgy, breaking fast, red eggs, greeting 'Christ is Risen'",
      "The Resurrection \xe2\x80\x94 death defeated, the stone rolled away, the first light of Sunday" }
};

/* ================================================================
 * Invalid sentinels
 * ================================================================ */

static const ci_coptic_month_t INVALID_MONTH = {
    -1, "?", "?", "?", "?"
};

static const ci_coptic_feast_t INVALID_FEAST = {
    -1, "?", "?", "?", "?"
};

/* ================================================================
 * Public API
 * ================================================================ */

ci_coptic_month_t cci_month_data(int month)
{
    if (month < 1 || month > 13) return INVALID_MONTH;
    return MONTHS[month - 1];
}

ci_coptic_feast_t cci_feast_data(int feast)
{
    if (feast < 1 || feast > 6) return INVALID_FEAST;
    return FEASTS[feast - 1];
}

coptic_interp_t cci_interpret(int month, int day, int feast)
{
    coptic_interp_t r;
    memset(&r, 0, sizeof(r));

    ci_coptic_month_t m = cci_month_data(month);

    if (m.month < 0) {
        snprintf(r.glyph, sizeof(r.glyph), "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    /* Glyph: first 3 chars of month name */
    size_t len = strlen(m.name);
    size_t copy = len < 3 ? len : 3;
    memcpy(r.glyph, m.name, copy);
    r.glyph[copy] = '\0';

    /* Glance */
    if (feast >= 1 && feast <= 6) {
        ci_coptic_feast_t f = cci_feast_data(feast);
        snprintf(r.glance, sizeof(r.glance),
                 "%s %d \xe2\x80\x94 %s",
                 m.name, day, f.name);
    } else {
        snprintf(r.glance, sizeof(r.glance),
                 "%s %d \xe2\x80\x94 %s",
                 m.name, day, m.origin);
    }

    /* Detail */
    int pos = 0;
    pos += snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                    "Month: %s. Origin: %s. Liturgy: %s.",
                    m.name, m.origin, m.liturgy);

    if (feast >= 1 && feast <= 6) {
        ci_coptic_feast_t f = cci_feast_data(feast);
        snprintf(r.detail + pos, sizeof(r.detail) - (size_t)pos,
                 " Feast: %s \xe2\x80\x94 %s. %s",
                 f.name, f.theme, f.practice);
    }

    return r;
}

int cci_month_count(void)
{
    return 13;
}

int cci_feast_count(void)
{
    return 6;
}
