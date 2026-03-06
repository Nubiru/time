#ifndef TIME_HUMAN_DESIGN_H
#define TIME_HUMAN_DESIGN_H

typedef struct {
    int gate;          /* 1-64 gate number */
    int line;          /* 1-6 line within the gate */
    double longitude;  /* exact ecliptic longitude that produced this */
} hd_gate_t;

typedef struct {
    hd_gate_t sun;     /* Sun gate.line */
    hd_gate_t earth;   /* Earth gate.line (Sun + 180) */
} hd_incarnation_t;

/* Get gate and line from ecliptic longitude. */
hd_gate_t        hd_gate_from_longitude(double ecliptic_longitude);

/* Get Sun and Earth gates for an incarnation cross. */
hd_incarnation_t hd_incarnation(double sun_longitude);

/* Human Design gate name (1-64). Returns "?" for invalid. */
const char      *hd_gate_name(int gate);

/* Human Design gate keyword (1-64). Returns "?" for invalid. */
const char      *hd_gate_keyword(int gate);

/* Gate number at ecliptic longitude (1-64). */
int              hd_gate_at_degree(double ecliptic_longitude);

#endif
