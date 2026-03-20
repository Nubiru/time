/* islamic_interpret.c -- Islamic month & prayer interpretation data
 *
 * All functions are pure (no globals, no malloc, no side effects).
 * Month data from classical Islamic tradition. */

#include "islamic_interpret.h"
#include "../../ui/content_i18n.h"
#include <stdio.h>
#include <string.h>

#define MONTH_COUNT 12
#define PRAYER_COUNT 5

static const isi_month_t MONTHS[MONTH_COUNT] = {
    {  1, "Muharram",        1,
       "First month, month of Allah",
       "Ashura (10th) -- Moses delivered from Pharaoh",
       "Sacred month of mourning and remembrance" },
    {  2, "Safar",           0,
       "Month of journeying",
       "",
       "Month of travel -- traditionally considered difficult" },
    {  3, "Rabi al-Awwal",   0,
       "Month of the Prophet's birth",
       "Mawlid (12th) -- birthday of Prophet Muhammad",
       "Month of joy and prophetic celebration" },
    {  4, "Rabi al-Thani",   0,
       "Month of spring's continuation",
       "",
       "Quiet month of reflection and growth" },
    {  5, "Jumada al-Ula",   0,
       "Month of frozen water",
       "",
       "Month of endurance through hardship" },
    {  6, "Jumada al-Thani", 0,
       "Month of summer's approach",
       "",
       "Month of preparation and anticipation" },
    {  7, "Rajab",           1,
       "Month of respect",
       "Isra and Mi'raj (27th) -- Night Journey",
       "Sacred month of spiritual ascent" },
    {  8, "Sha'ban",         0,
       "Month of division/separation",
       "Laylat al-Bara'ah (15th) -- Night of Fortune",
       "Month of preparation for Ramadan" },
    {  9, "Ramadan",         0,
       "Month of burning heat/fasting",
       "Entire month: Sawm (fasting), Laylat al-Qadr (27th)",
       "Month of fasting, revelation, and spiritual renewal" },
    { 10, "Shawwal",         0,
       "Month of upliftment",
       "Eid al-Fitr (1st) -- Festival of Breaking Fast",
       "Month of celebration and voluntary fasting" },
    { 11, "Dhul Qa'dah",     1,
       "Month of rest",
       "",
       "Sacred month of truce and preparation for Hajj" },
    { 12, "Dhul Hijjah",     1,
       "Month of pilgrimage",
       "Hajj (8th-12th), Eid al-Adha (10th) -- Festival of Sacrifice",
       "Sacred month of pilgrimage and sacrifice" }
};

static const isi_prayer_t PRAYERS[PRAYER_COUNT] = {
    { 0, "Fajr",    "al-Fajr",    "Pre-dawn, first light",
      "Awakening -- beginning before the world stirs" },
    { 1, "Dhuhr",   "al-Zuhr",    "Midday, after zenith",
      "Remembrance -- pausing at the day's peak" },
    { 2, "Asr",     "al-'Asr",    "Afternoon, declining sun",
      "Reflection -- awareness as time passes" },
    { 3, "Maghrib", "al-Maghrib",  "Just after sunset",
      "Gratitude -- marking the day's completion" },
    { 4, "Isha",    "al-'Isha",    "Night, after twilight fades",
      "Surrender -- trust in the darkness" }
};

isi_month_t isi_month_data(int month)
{
    if (month < 1 || month > MONTH_COUNT) {
        isi_month_t invalid = { -1, "?", 0, "?", "?", "?" };
        return invalid;
    }
    return MONTHS[month - 1];
}

isi_prayer_t isi_prayer_data(int index)
{
    if (index < 0 || index >= PRAYER_COUNT) {
        isi_prayer_t invalid = { -1, "?", "?", "?", "?" };
        return invalid;
    }
    return PRAYERS[index];
}

int isi_month_count(void)
{
    return MONTH_COUNT;
}

int isi_prayer_count(void)
{
    return PRAYER_COUNT;
}

islamic_interp_t isi_interpret(hijri_date_t date)
{
    islamic_interp_t result;
    memset(&result, 0, sizeof(result));

    isi_month_t m = isi_month_data(date.month);

    /* T0: glyph -- month number as string */
    snprintf(result.glyph, sizeof(result.glyph), "%d", date.month);

    /* Get month name from hijri.h for the glance line */
    const char *month_name = hijri_month_name(date.month);

    /* T1: glance -- "{year} {month_name} {day} -- {significance}" */
    snprintf(result.glance, sizeof(result.glance),
             "%d %s %d -- %s",
             date.year, month_name, date.day, m.significance);

    /* T3: detail -- full narrative paragraph */
    int written = snprintf(result.detail, sizeof(result.detail),
             "%s (%s).",
             month_name, m.brief);

    /* Append sacred status if applicable */
    if (m.is_sacred && written > 0 && (size_t)written < sizeof(result.detail)) {
        int added = snprintf(result.detail + written,
                 sizeof(result.detail) - (size_t)written,
                 " This is one of the four sacred months.");
        if (added > 0) {
            written += added;
        }
    }

    /* Append event if present */
    if (strlen(m.event) > 0 && written > 0 &&
        (size_t)written < sizeof(result.detail)) {
        snprintf(result.detail + written,
                 sizeof(result.detail) - (size_t)written,
                 " %s.", m.event);
    }

    return result;
}

/* ================================================================
 * Locale-aware interpretation
 * ================================================================ */

islamic_interp_t isi_interpret_locale(hijri_date_t date,
                                      i18n_locale_t locale)
{
    /* English fast path */
    if (locale == I18N_LOCALE_EN) {
        return isi_interpret(date);
    }

    islamic_interp_t result;
    memset(&result, 0, sizeof(result));

    char key[64];

    /* Month name (Arabic names preserved across locales) */
    snprintf(key, sizeof(key), "islamic.month.%d.name", date.month);
    const char *month_name = content_get(key, locale);

    snprintf(key, sizeof(key), "islamic.month.%d.significance", date.month);
    const char *significance = content_get(key, locale);

    snprintf(key, sizeof(key), "islamic.month.%d.brief", date.month);
    const char *brief = content_get(key, locale);

    snprintf(key, sizeof(key), "islamic.month.%d.event", date.month);
    const char *event = content_get(key, locale);

    /* T0: glyph -- month number as string */
    snprintf(result.glyph, sizeof(result.glyph), "%d", date.month);

    /* T1: glance */
    const char *tpl_glance = content_get("islamic.tpl.glance", locale);
    snprintf(result.glance, sizeof(result.glance), tpl_glance,
             date.year, month_name, date.day, significance);

    /* T3: detail */
    const char *tpl_detail = content_get("islamic.tpl.detail", locale);
    int written = snprintf(result.detail, sizeof(result.detail),
                           tpl_detail, month_name, brief);

    /* Append sacred status if applicable */
    isi_month_t m = isi_month_data(date.month);
    if (m.is_sacred && written > 0 && (size_t)written < sizeof(result.detail)) {
        const char *tpl_sacred = content_get("islamic.tpl.detail_sacred",
                                             locale);
        int added = snprintf(result.detail + written,
                             sizeof(result.detail) - (size_t)written,
                             "%s", tpl_sacred);
        if (added > 0) {
            written += added;
        }
    }

    /* Append event if present */
    if (event != NULL && event[0] != '\0' &&
        written > 0 && (size_t)written < sizeof(result.detail)) {
        const char *tpl_event = content_get("islamic.tpl.detail_event",
                                            locale);
        snprintf(result.detail + written,
                 sizeof(result.detail) - (size_t)written,
                 tpl_event, event);
    }

    return result;
}
