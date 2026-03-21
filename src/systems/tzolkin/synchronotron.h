#ifndef TIME_SYNCHRONOTRON_H
#define TIME_SYNCHRONOTRON_H

/* Synchronotron — daily frequency computation.
 *
 * The Synchronotron practice produces 4 frequency numbers by combining
 * the day of the heptad (Heptad Gate) with the daily Kin (seal + tone).
 * It ties together the Hunab Ku 21 archetype positions and the Lenses
 * of Time.
 *
 * Self-contained module: does NOT #include other tzolkin headers.
 * All lookup data is internal. Receives seal codes and tones as
 * parameters to stay pure and decoupled.
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Source: Arguelles, "Como Entrar a Hunab Ku 21"
 *         (Boletin Intergalactico No 7).
 */

typedef struct {
    int day_of_week;      /* 1-7 */
    int seal_code;        /* 1-20 (the destiny seal on HK21 tree) */
    int tone;             /* 1-13 */
    int tonal_value;      /* tone x lens_value */
    int lens_number;      /* 1-4 */
    int lens_value;       /* 108, 144, 216, or 288 */
    int heptad_gate_hk;   /* HK number of the day's heptad gate */
    int destiny_hk;       /* HK number of the destiny seal on HK21 tree */
} sync_daily_t;

/* Compute daily synchronotron frequencies.
 * day_of_week: 1-7, seal_code: 1-20, tone: 1-13.
 * Returns zeroed struct for invalid inputs. */
sync_daily_t sync_compute(int day_of_week, int seal_code, int tone);

/* Tonal value: tone x lens_value. Returns 0 for invalid tone. */
int sync_tonal_value(int tone);

/* Lens number for a tone (1-4). Returns 0 for invalid tone. */
int sync_lens_for_tone(int tone);

/* Lens value for a tone (108/144/216/288). Returns 0 for invalid tone. */
int sync_lens_value_for_tone(int tone);

/* Heptad gate HK number for a day (1-7). Returns 0 for invalid. */
int sync_heptad_gate_hk(int day_of_week);

/* HK21 number for a seal (1-20), 441 for seal 21. Returns 0 for invalid. */
int sync_seal_hk(int seal_code);

#endif /* TIME_SYNCHRONOTRON_H */
