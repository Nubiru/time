#include "haab_vinal.h"
#include <string.h>

/* Haab Vinales — 18 named 20-day periods + 5-day Uayeb.
 *
 * The wisdom meanings form a progressive journey:
 * From knowing (Pop) through silence, integration, wisdom,
 * ecstasy, and divination, ending with the nameless Uayeb days.
 *
 * Source: Arguelles, "Telektonon: El Juego de la Profecia" (1995),
 *         Arguelles, "The Mayan Factor" (1987).
 */

static const haab_vinal_t VINALES[HV_TOTAL_PERIODS] = {
    {   /* Vinal 1 */
        .number  = 1,
        .name    = "Pop",
        .meaning = "The one who knows",
        .days    = 20
    },
    {   /* Vinal 2 */
        .number  = 2,
        .name    = "Uo",
        .meaning = "Listen in silence",
        .days    = 20
    },
    {   /* Vinal 3 */
        .number  = 3,
        .name    = "Zip",
        .meaning = "To integrate the universe",
        .days    = 20
    },
    {   /* Vinal 4 */
        .number  = 4,
        .name    = "Zotz",
        .meaning = "Based on knowledge",
        .days    = 20
    },
    {   /* Vinal 5 */
        .number  = 5,
        .name    = "Tzec",
        .meaning = "Reaching the foundations",
        .days    = 20
    },
    {   /* Vinal 6 */
        .number  = 6,
        .name    = "Xul",
        .meaning = "With great wisdom a seed is sown",
        .days    = 20
    },
    {   /* Vinal 7 */
        .number  = 7,
        .name    = "Yaxkin",
        .meaning = "A small ray of the hidden sun",
        .days    = 20
    },
    {   /* Vinal 8 */
        .number  = 8,
        .name    = "Mol",
        .meaning = "Which unifies all parts",
        .days    = 20
    },
    {   /* Vinal 9 */
        .number  = 9,
        .name    = "Chen",
        .meaning = "To enter the inner wisdom source",
        .days    = 20
    },
    {   /* Vinal 10 */
        .number  = 10,
        .name    = "Yax",
        .meaning = "Where the apprentice clarifies the mind",
        .days    = 20
    },
    {   /* Vinal 11 */
        .number  = 11,
        .name    = "Sac",
        .meaning = "Dissipating doubt clouds, elevating oneself",
        .days    = 20
    },
    {   /* Vinal 12 */
        .number  = 12,
        .name    = "Ceh",
        .meaning = "Breaking habitual caution, reaching white light",
        .days    = 20
    },
    {   /* Vinal 13 */
        .number  = 13,
        .name    = "Mac",
        .meaning = "Closing the wrong place, entering ecstasy",
        .days    = 20
    },
    {   /* Vinal 14 */
        .number  = 14,
        .name    = "Kankin",
        .meaning = "Receiving the light of the one who knows",
        .days    = 20
    },
    {   /* Vinal 15 */
        .number  = 15,
        .name    = "Moan",
        .meaning = "To see in the darkness",
        .days    = 20
    },
    {   /* Vinal 16 */
        .number  = 16,
        .name    = "Pax",
        .meaning = "Playing a music of the future",
        .days    = 20
    },
    {   /* Vinal 17 */
        .number  = 17,
        .name    = "Kayab",
        .meaning = "With the song and the rhythm",
        .days    = 20
    },
    {   /* Vinal 18 */
        .number  = 18,
        .name    = "Cumhu",
        .meaning = "At the right place where divination food is obtained",
        .days    = 20
    },
    {   /* Vinal 19 — Uayeb */
        .number  = 19,
        .name    = "Uayeb",
        .meaning = "5-day closing period",
        .days    = 5
    }
};

int hv_vinal_count(void)
{
    return HV_TOTAL_PERIODS;
}

haab_vinal_t hv_vinal_get(int num)
{
    if (num < 1 || num > HV_TOTAL_PERIODS) {
        haab_vinal_t empty;
        memset(&empty, 0, sizeof(empty));
        return empty;
    }
    return VINALES[num - 1];
}

const char *hv_vinal_name(int num)
{
    if (num < 1 || num > HV_TOTAL_PERIODS)
        return NULL;
    return VINALES[num - 1].name;
}

const char *hv_vinal_meaning(int num)
{
    if (num < 1 || num > HV_TOTAL_PERIODS)
        return NULL;
    return VINALES[num - 1].meaning;
}

int hv_is_uayeb(int vinal_num)
{
    return (vinal_num == HV_TOTAL_PERIODS) ? 1 : 0;
}

int hv_haab_days(void)
{
    return HV_HAAB_DAYS;
}

int hv_vinal_for_day_of_year(int day)
{
    if (day < 0 || day >= HV_HAAB_DAYS)
        return 0;
    if (day < HV_VINAL_COUNT * HV_VINAL_DAYS)
        return (day / HV_VINAL_DAYS) + 1;
    return HV_TOTAL_PERIODS; /* Uayeb */
}
