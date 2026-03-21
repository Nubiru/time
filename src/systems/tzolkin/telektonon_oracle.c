#include "telektonon_oracle.h"
#include <string.h>

/* Telektonon Oracle — Seven Generations Oracle data.
 *
 * 7 Years of Prophecy (1993-2000) and 3 oracle positions.
 *
 * Source: Arguelles, "Telektonon: El Juego de la Profecia" (1995).
 */

/* --- Prophecy year data --- */

static const tko_prophecy_year_t YEARS[TKO_PROPHECY_YEAR_COUNT] = {
    {   /* Year 1 */
        .year_number    = 1,
        .start_greg_year = 1993,
        .name           = "Victory Hits the Target",
        .victory_seal   = "1st Seal"
    },
    {   /* Year 2 */
        .year_number    = 2,
        .start_greg_year = 1994,
        .name           = "Victory Flows",
        .victory_seal   = "2nd Seal"
    },
    {   /* Year 3 */
        .year_number    = 3,
        .start_greg_year = 1995,
        .name           = "Victory Pacifies",
        .victory_seal   = "3rd Seal"
    },
    {   /* Year 4 */
        .year_number    = 4,
        .start_greg_year = 1996,
        .name           = "Victory Establishes",
        .victory_seal   = "4th Seal"
    },
    {   /* Year 5 */
        .year_number    = 5,
        .start_greg_year = 1997,
        .name           = "Victory Liberates",
        .victory_seal   = "5th Seal"
    },
    {   /* Year 6 */
        .year_number    = 6,
        .start_greg_year = 1998,
        .name           = "Victory Purifies",
        .victory_seal   = "6th Seal"
    },
    {   /* Year 7 */
        .year_number    = 7,
        .start_greg_year = 1999,
        .name           = "Victory Discharges",
        .victory_seal   = "7th Seal"
    }
};

/* --- Oracle position data --- */

static const tko_oracle_position_t POSITIONS[TKO_POSITION_COUNT] = {
    {
        .position = TKO_CROWN,
        .name     = "Crown",
        .name_es  = "Corona",
        .power    = "Power of Mind",
        .focus    = "Mandate of Heaven, Purpose of Annual Oracle"
    },
    {
        .position = TKO_HEART,
        .name     = "Heart",
        .name_es  = "Corazon",
        .power    = "Power of Love",
        .focus    = "Human Sentiments, Ring Oracle Result"
    },
    {
        .position = TKO_ROOT,
        .name     = "Root",
        .name_es  = "Raiz",
        .power    = "Hidden Power of Earth",
        .focus    = "Lost Generation Oracle"
    }
};

/* --- Public functions --- */

int tko_prophecy_year_count(void)
{
    return TKO_PROPHECY_YEAR_COUNT;
}

tko_prophecy_year_t tko_prophecy_year_get(int num)
{
    if (num < 1 || num > TKO_PROPHECY_YEAR_COUNT) {
        tko_prophecy_year_t empty;
        memset(&empty, 0, sizeof(empty));
        return empty;
    }
    return YEARS[num - 1];
}

const char *tko_prophecy_year_name(int num)
{
    if (num < 1 || num > TKO_PROPHECY_YEAR_COUNT)
        return NULL;
    return YEARS[num - 1].name;
}

int tko_position_count(void)
{
    return TKO_POSITION_COUNT;
}

tko_oracle_position_t tko_position_get(tko_position_t pos)
{
    if ((int)pos < 0 || (int)pos >= TKO_POSITION_COUNT) {
        tko_oracle_position_t empty;
        memset(&empty, 0, sizeof(empty));
        return empty;
    }
    return POSITIONS[(int)pos];
}

const char *tko_position_name(tko_position_t pos)
{
    if ((int)pos < 0 || (int)pos >= TKO_POSITION_COUNT)
        return NULL;
    return POSITIONS[(int)pos].name;
}
