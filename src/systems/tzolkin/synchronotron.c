#include "synchronotron.h"
#include <string.h>

/* Synchronotron — daily frequency computation.
 *
 * All lookup data is self-contained. No cross-dependencies on other
 * tzolkin headers.
 *
 * Source: Arguelles, "Como Entrar a Hunab Ku 21"
 */

/* --- Heptad Gate HK numbers (day 1-7) ---
 * Day 1: DALI    / Dragon    / 108
 * Day 2: SELI    / Hand      / 291
 * Day 3: GAMMA   / Wind      / 144
 * Day 4: KALI    / Skywalker / 315
 * Day 5: ALFA    / Sun       / 414
 * Day 6: LIMI    / Mirror    / 402
 * Day 7: SILIO   / Hunab Ku  / 441 */
static const int GATE_HK[7] = {
    108, 291, 144, 315, 414, 402, 441
};

/* --- Seal HK21 numbers (seal 1-21) ---
 * Indexed 0-20, where index 0 = seal code 1 (Dragon).
 * Seal 21 = Hunab Ku center (441). */
static const int SEAL_HK[21] = {
    108,  /* 1  Dragon     */
    144,  /* 2  Wind       */
    126,  /* 3  Night      */
     90,  /* 4  Seed       */
    288,  /* 5  Serpent    */
    294,  /* 6  Worldbridger */
    291,  /* 7  Monkey     */
    300,  /* 8  Star       */
    306,  /* 9  Moon       */
    303,  /* 10 Dog        */
    312,  /* 11 Hand       */
    318,  /* 12 Human      */
    315,  /* 13 Skywalker  */
    276,  /* 14 Wizard     */
    282,  /* 15 Eagle      */
    279,  /* 16 Warrior    */
    396,  /* 17 Earth      */
    402,  /* 18 Mirror     */
    408,  /* 19 Storm      */
    414,  /* 20 Sun        */
    441   /* 21 Hunab Ku   */
};

/* --- Lens values (lens 1-4) ---
 * Lens 1: 108 (Cosmic Standard)
 * Lens 2: 144 (Cosmic Harmonic)
 * Lens 3: 216 (Cosmic Cube)
 * Lens 4: 288 (Ultimate Sphere) */
static const int LENS_VALUES[4] = {
    108, 144, 216, 288
};

/* --- Tone-to-Lens mapping ---
 * Tones 1,5,9,13  -> Lens 1 (108)
 * Tones 2,6,10    -> Lens 2 (144)
 * Tones 3,7,11    -> Lens 3 (216)
 * Tones 4,8,12    -> Lens 4 (288) */
static const int TONE_TO_LENS[13] = {
    1, 2, 3, 4,   /* tones  1- 4 */
    1, 2, 3, 4,   /* tones  5- 8 */
    1, 2, 3, 4,   /* tones  9-12 */
    1              /* tone  13    */
};

/* --- Function implementations --- */

int sync_lens_for_tone(int tone)
{
    if (tone < 1 || tone > 13)
        return 0;
    return TONE_TO_LENS[tone - 1];
}

int sync_lens_value_for_tone(int tone)
{
    int lens = sync_lens_for_tone(tone);
    if (lens == 0)
        return 0;
    return LENS_VALUES[lens - 1];
}

int sync_tonal_value(int tone)
{
    int lv = sync_lens_value_for_tone(tone);
    if (lv == 0)
        return 0;
    return tone * lv;
}

int sync_heptad_gate_hk(int day_of_week)
{
    if (day_of_week < 1 || day_of_week > 7)
        return 0;
    return GATE_HK[day_of_week - 1];
}

int sync_seal_hk(int seal_code)
{
    if (seal_code < 1 || seal_code > 21)
        return 0;
    return SEAL_HK[seal_code - 1];
}

sync_daily_t sync_compute(int day_of_week, int seal_code, int tone)
{
    sync_daily_t result;
    memset(&result, 0, sizeof(result));

    if (day_of_week < 1 || day_of_week > 7)
        return result;
    if (seal_code < 1 || seal_code > 20)
        return result;
    if (tone < 1 || tone > 13)
        return result;

    result.day_of_week    = day_of_week;
    result.seal_code      = seal_code;
    result.tone           = tone;
    result.lens_number    = sync_lens_for_tone(tone);
    result.lens_value     = sync_lens_value_for_tone(tone);
    result.tonal_value    = sync_tonal_value(tone);
    result.heptad_gate_hk = sync_heptad_gate_hk(day_of_week);
    result.destiny_hk     = sync_seal_hk(seal_code);

    return result;
}
