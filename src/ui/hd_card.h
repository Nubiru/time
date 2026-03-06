#ifndef TIME_HD_CARD_H
#define TIME_HD_CARD_H

#include <stddef.h>

/* Human Design display data — all strings ready for rendering. */
typedef struct {
    char sun_gate[48];        /* "Gate 17.4 · Opinions · Following" */
    char earth_gate[48];      /* "Gate 18.2 · Correction · Work on..." */
    char cross_line[64];      /* "Sun 17.4 / Earth 18.2" */
    char sun_keyword[32];     /* "Following" */
    char earth_keyword[32];   /* "Work on What Has Been Spoiled" */
    char summary[64];         /* "Gate 17.4 Opinions · Gate 18.2 Correction" */
} hd_display_t;

/* Compute all display strings from Sun ecliptic longitude. */
hd_display_t hd_display_from_longitude(double sun_longitude);

/* Format gate: "Gate 17.4 · Opinions · Following" */
void hd_fmt_gate(int gate, int line, char *buf, size_t sz);

/* Format gate short: "17.4" */
void hd_fmt_gate_short(int gate, int line, char *buf, size_t sz);

/* Format incarnation cross: "Sun 17.4 / Earth 18.2" */
void hd_fmt_cross(int sun_gate, int sun_line,
                  int earth_gate, int earth_line,
                  char *buf, size_t sz);

/* Format keyword: copies gate keyword to buf */
void hd_fmt_keyword(int gate, char *buf, size_t sz);

#endif
