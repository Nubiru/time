/* pawukon.h -- Balinese Pawukon 210-day concurrent cycle calendar
 *
 * The Pawukon is a unique 210-day ceremonial calendar that runs 10 different
 * week-cycles simultaneously (1-day through 10-day weeks). The 210-day cycle
 * has no astronomical basis -- it is purely cultural and used daily in Bali
 * for determining auspicious days, ceremonies, and temple festivals.
 *
 * 30 named 7-day wuku periods (30 x 7 = 210).
 * 10 concurrent weeks: Eka Wara (1) through Dasa Wara (10).
 * Urip (spiritual weight) computed from Panca Wara + Sapta Wara values.
 *
 * Epoch: JD 2379344.5 (15 July 1802 CE) = Wuku Sinta, day 0 of cycle.
 *
 * Sources: Eiseman, "Bali: Sekala & Niskala" (1990).
 *          Darling, "The Painted Alphabet" (1992).
 *          Dershowitz & Reingold, "Calendrical Calculations" (2018). */

#ifndef TIME_PAWUKON_H
#define TIME_PAWUKON_H

#define PAWUKON_CYCLE_DAYS      210
#define PAWUKON_WUKU_COUNT      30
#define PAWUKON_CONCURRENT_WEEKS 10

typedef struct {
    int wuku;              /* Current wuku (0-29), named 7-day week */
    int wuku_day;          /* Day within current wuku (0-6) */
    int day_in_cycle;      /* Day within 210-day cycle (0-209) */
    int eka_wara;          /* 1-day cycle (always 0) */
    int dwi_wara;          /* 2-day cycle (0-1) */
    int tri_wara;          /* 3-day cycle (0-2) */
    int catur_wara;        /* 4-day cycle (0-3) */
    int panca_wara;        /* 5-day cycle (0-4) */
    int sad_wara;          /* 6-day cycle (0-5) */
    int sapta_wara;        /* 7-day cycle (0-6) */
    int asta_wara;         /* 8-day cycle (0-7) */
    int sanga_wara;        /* 9-day cycle (0-8) */
    int dasa_wara;         /* 10-day cycle (0-9) */
} pawukon_t;

typedef struct {
    const char *name;
    const char *description;
    int wuku;          /* -1 if not wuku-specific */
    int panca_wara;    /* -1 if any */
    int sapta_wara;    /* -1 if any */
} pawukon_ceremony_t;

/* Convert Julian Day to Pawukon date (all 10 concurrent weeks). */
pawukon_t pawukon_from_jd(double jd);

/* Wuku name (0-29). Returns "?" for invalid. */
const char *pawukon_wuku_name(int wuku);

/* Day name for a specific week length (1-10) and day index. Returns "?" for invalid. */
const char *pawukon_day_name(int week_length, int day_index);

/* Compute urip (spiritual weight) = panca_wara_urip + sapta_wara_urip. */
int pawukon_urip(pawukon_t p);

/* Check if the given Pawukon date matches a ceremony. Returns name or NULL. */
const char *pawukon_ceremony(pawukon_t p);

/* Number of defined ceremonies. */
int pawukon_ceremony_count(void);

/* Get ceremony info by index. Returns struct with name=NULL for invalid. */
pawukon_ceremony_t pawukon_ceremony_by_index(int index);

/* Day of cycle (0-209) from JD. */
int pawukon_day_of_cycle(double jd);

/* Days until a specific wuku (0-29) from current JD. Returns -1 for invalid. */
int pawukon_days_until_wuku(double jd, int target_wuku);

/* Is this a Tumpek day? (panca_wara=Kliwon AND sapta_wara=Saniscara) */
int pawukon_is_tumpek(pawukon_t p);

/* Is this a Kajeng Kliwon day? (tri_wara=Kajeng AND panca_wara=Kliwon) */
int pawukon_is_kajeng_kliwon(pawukon_t p);

#endif /* TIME_PAWUKON_H */
