#ifndef TIME_TELEKTONON_ORACLE_H
#define TIME_TELEKTONON_ORACLE_H

/* Telektonon Oracle — Seven Generations Oracle system.
 *
 * The Seven Generations Oracle from the Telektonon game consists of
 * 7 Years of Prophecy (1993-2000), each with 3 oracle positions
 * (Crown, Heart, Root). Each position carries a distinct power and
 * focus within the annual oracle reading.
 *
 * The Oracle Victory Day occurs when 8 of the 15 daily oracle
 * positions match positions in the Seven Generations Oracle.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Source: Arguelles, "Telektonon: El Juego de la Profecia" (1995).
 */

#define TKO_PROPHECY_YEAR_COUNT  7
#define TKO_POSITION_COUNT       3

/* --- Enums --- */

typedef enum {
    TKO_CROWN = 0,
    TKO_HEART = 1,
    TKO_ROOT  = 2
} tko_position_t;

/* --- Structs --- */

typedef struct {
    int          year_number;    /* 1-7 */
    int          start_greg_year; /* 1993-1999 */
    const char  *name;           /* "Victory Hits the Target", etc. */
    const char  *victory_seal;   /* "1st Seal" through "7th Seal" */
} tko_prophecy_year_t;

typedef struct {
    tko_position_t  position;
    const char     *name;        /* English name */
    const char     *name_es;     /* Spanish name */
    const char     *power;       /* "Power of Mind", etc. */
    const char     *focus;       /* Oracle focus description */
} tko_oracle_position_t;

/* --- Functions --- */

/* Number of prophecy years (always 7). */
int tko_prophecy_year_count(void);

/* Get prophecy year by number (1-7).
 * Returns zeroed struct for invalid number. */
tko_prophecy_year_t tko_prophecy_year_get(int num);

/* Prophecy year name string (1-7).
 * Returns NULL for invalid number. */
const char *tko_prophecy_year_name(int num);

/* Number of oracle positions (always 3). */
int tko_position_count(void);

/* Get oracle position data by enum.
 * Returns zeroed struct for invalid position. */
tko_oracle_position_t tko_position_get(tko_position_t pos);

/* Oracle position name (English).
 * Returns NULL for invalid position. */
const char *tko_position_name(tko_position_t pos);

#endif /* TIME_TELEKTONON_ORACLE_H */
