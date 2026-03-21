#ifndef TIME_KIN_DAILY_CHRONO_H
#define TIME_KIN_DAILY_CHRONO_H

/* Kin Daily Chronometer — Universal daily practice card.
 *
 * Combines oracle cross + plasma + 13-Moon info + heptad gate
 * into a single struct for daily Dreamspell practice. This is
 * THE single view a practitioner uses each day.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Source: Arguelles, "Dreamspell" (1990), "13 Moon Calendar".
 */

#define KIN_CHRONO_MAX_TEXT 64

/* One oracle position with full display data. */
typedef struct {
    int kin;                         /* 1-260 */
    int seal;                        /* 0-19 */
    int tone;                        /* 1-13 */
    int color;                       /* 0-3 */
    char name[KIN_CHRONO_MAX_TEXT];  /* "Red Dragon", "Blue Monkey", etc. */
} kin_chrono_oracle_t;

typedef struct {
    /* Today's identity */
    int kin;
    int seal;
    int tone;
    int color;
    char kin_name[KIN_CHRONO_MAX_TEXT];       /* "Blue Magnetic Monkey" */
    char tone_action[KIN_CHRONO_MAX_TEXT];    /* "Unify" */
    char tone_power[KIN_CHRONO_MAX_TEXT];     /* "Purpose" */
    char color_action[KIN_CHRONO_MAX_TEXT];   /* "Transforms" */

    /* Oracle (5 positions) */
    kin_chrono_oracle_t destiny;   /* center */
    kin_chrono_oracle_t guide;     /* top */
    kin_chrono_oracle_t analog;    /* right */
    kin_chrono_oracle_t antipode;  /* left */
    kin_chrono_oracle_t occult;    /* bottom */

    /* 13-Moon context */
    int moon;                                 /* 1-13 */
    int moon_day;                             /* 1-28 */
    char moon_name[KIN_CHRONO_MAX_TEXT];      /* "Magnetic Moon" */

    /* Plasma / Heptad Gate */
    int plasma;                               /* 0-6 */
    char plasma_name[KIN_CHRONO_MAX_TEXT];    /* "Dali" */
    char plasma_chakra[KIN_CHRONO_MAX_TEXT];  /* "Crown" */
    char plasma_action[KIN_CHRONO_MAX_TEXT];  /* "Target" */
    char mantra[KIN_CHRONO_MAX_TEXT];         /* "OM" */

    /* Wavespell + Castle context */
    int wavespell_position;                   /* 1-13 */
    char wavespell_info[KIN_CHRONO_MAX_TEXT]; /* "WS 11: Blue Monkey" */
    char castle_info[KIN_CHRONO_MAX_TEXT];    /* "Blue Castle of Burning" */

    /* Flags */
    int is_portal;     /* Galactic Activation Portal (always 0, caller checks) */
    int is_power_day;  /* wavespell gate position (1,5,9,13) */
    int is_day_out;    /* Day Out of Time */
} kin_daily_chrono_t;

/* Compute the complete daily chronometer from Julian Day.
 * Combines: Tzolkin kin + oracle + 13-Moon + heptad gate + wavespell/castle.
 * Returns zeroed struct for invalid JD (< 0). */
kin_daily_chrono_t kin_daily_chrono_compute(double jd);

#endif /* TIME_KIN_DAILY_CHRONO_H */
