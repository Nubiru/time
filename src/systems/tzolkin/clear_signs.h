#ifndef TIME_CLEAR_SIGNS_H
#define TIME_CLEAR_SIGNS_H

/* Clear Signs of Pacal Votan — 13 special Kins on the sarcophagus lid.
 *
 * 13 Kins inscribed on the edges of Pacal Votan's sarcophagus lid at
 * Palenque, arranged in 6 groups around the borders (South, East, North,
 * West, and two borderless groups). They form 6 paired relationships
 * plus 1 unpaired Kin — Pacal Votan's own galactic signature (Kin 60,
 * Yellow Galactic Sun).
 *
 * Pure module: no GL, no malloc, no globals, no side effects.
 *
 * Source: Arguelles, "Telektonon: El Juego de la Profecia" (1995).
 */

#define CLEAR_SIGN_COUNT 13

typedef struct {
    int kin_number;         /* 1-260 */
    int seal_code;          /* 0-19 */
    const char *seal_name;  /* "Sun", "Worldbridger", etc. */
    int tone;               /* 1-13 */
    int group;              /* 1-6 */
    const char *border;     /* "South", "East", "North", "West", or "" */
} clear_sign_t;

/* Number of clear signs (always 13). */
int clear_sign_count(void);

/* Get clear sign by index (0-12).
 * Returns zeroed struct for invalid index. */
clear_sign_t clear_sign_get(int index);

/* Check if a kin number is a clear sign.
 * Returns 1 if yes, 0 if no. */
int clear_sign_is_clear_sign(int kin_number);

/* Grand total of all 13 tones (always 78 = 6 x 13). */
int clear_sign_total_tone_sum(void);

/* Tone sum for a group (1-6). Returns 0 for invalid group. */
int clear_sign_group_tone_sum(int group);

/* Paired kin number for a clear sign.
 * Returns paired kin, or 0 if unpaired or not a clear sign. */
int clear_sign_pair(int kin_number);

/* Pacal Votan's galactic signature (always Kin 60, Yellow Galactic Sun). */
int clear_sign_pacal_votan_kin(void);

#endif /* TIME_CLEAR_SIGNS_H */
