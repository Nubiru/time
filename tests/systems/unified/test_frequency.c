/* test_frequency.c -- Frequency Mapper tests
 * TDD RED phase: tests written before implementation.
 * Tests conversion of time cycles to frequencies, octave transposition
 * to audible range, musical note mapping (12-TET, A4=440Hz),
 * interval detection, and planetary orbital frequencies.
 *
 * Planet table (10 entries):
 *   0=Earth day, 1=Mercury, 2=Venus, 3=Earth year, 4=Mars,
 *   5=Jupiter, 6=Saturn, 7=Uranus, 8=Neptune, 9=Moon (synodic) */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/frequency.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== freq_from_period ===== */

void test_from_period_one_second(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 1.0, freq_from_period(1.0));
}

void test_from_period_half_second(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 2.0, freq_from_period(0.5));
}

void test_from_period_440hz(void)
{
    double period = 1.0 / 440.0;
    TEST_ASSERT_FLOAT_WITHIN(1e-6, 440.0, freq_from_period(period));
}

void test_from_period_zero_returns_zero(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 0.0, freq_from_period(0.0));
}

void test_from_period_negative_returns_zero(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 0.0, freq_from_period(-5.0));
}

void test_from_period_earth_day(void)
{
    /* 86400 seconds -> ~1.1574e-5 Hz */
    double expected = 1.0 / 86400.0;
    TEST_ASSERT_FLOAT_WITHIN(1e-15, expected, freq_from_period(86400.0));
}

/* ===== freq_to_audible ===== */

void test_to_audible_already_audible(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-6, 440.0, freq_to_audible(440.0));
}

void test_to_audible_very_low(void)
{
    /* 1 Hz doubled: 2, 4, 8, 16, 32 -> 32 Hz (5 octaves) */
    double result = freq_to_audible(1.0);
    TEST_ASSERT_TRUE(result >= 20.0);
    TEST_ASSERT_TRUE(result <= 20000.0);
    TEST_ASSERT_FLOAT_WITHIN(1e-6, 32.0, result);
}

void test_to_audible_very_high(void)
{
    /* 40000 Hz halved to 20000 */
    double result = freq_to_audible(40000.0);
    TEST_ASSERT_TRUE(result >= 20.0);
    TEST_ASSERT_TRUE(result <= 20000.0);
    TEST_ASSERT_FLOAT_WITHIN(1e-6, 20000.0, result);
}

void test_to_audible_zero_returns_zero(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 0.0, freq_to_audible(0.0));
}

void test_to_audible_negative_returns_zero(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 0.0, freq_to_audible(-100.0));
}

void test_to_audible_boundary_low(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-6, 20.0, freq_to_audible(20.0));
}

void test_to_audible_boundary_high(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-6, 20000.0, freq_to_audible(20000.0));
}

void test_to_audible_earth_day(void)
{
    /* Earth day 1/86400 Hz -> lowest audible octave ~24.273 Hz
     * (Cousto's 194.18 Hz is 3 octaves higher; note is still G) */
    double hz = freq_from_period(86400.0);
    double audible = freq_to_audible(hz);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 24.27259, audible);
}

void test_to_audible_earth_year(void)
{
    /* Earth year 1/31558150 Hz -> lowest audible octave ~34.024 Hz
     * (Cousto's 136.1 Hz "Om" is 2 octaves higher; note is still C#) */
    double hz = freq_from_period(31558150.0);
    double audible = freq_to_audible(hz);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 34.02423, audible);
}

void test_to_audible_synodic_month(void)
{
    /* Moon synodic 1/2551443 Hz -> lowest audible octave ~26.302 Hz
     * (Cousto's 210.42 Hz is 3 octaves higher; note is still G#) */
    double hz = freq_from_period(2551443.0);
    double audible = freq_to_audible(hz);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 26.30232, audible);
}

/* ===== freq_to_note ===== */

void test_note_a4(void)
{
    freq_note_t note = freq_to_note(440.0);
    TEST_ASSERT_EQUAL_STRING("A", note.name);
    TEST_ASSERT_EQUAL_INT(4, note.octave);
    TEST_ASSERT_FLOAT_WITHIN(0.5, 0.0, note.cents_off);
    TEST_ASSERT_FLOAT_WITHIN(1e-6, 440.0, note.hz);
}

void test_note_c4(void)
{
    freq_note_t note = freq_to_note(261.626);
    TEST_ASSERT_EQUAL_STRING("C", note.name);
    TEST_ASSERT_EQUAL_INT(4, note.octave);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 0.0, note.cents_off);
}

void test_note_a3(void)
{
    freq_note_t note = freq_to_note(220.0);
    TEST_ASSERT_EQUAL_STRING("A", note.name);
    TEST_ASSERT_EQUAL_INT(3, note.octave);
}

void test_note_c5(void)
{
    freq_note_t note = freq_to_note(523.251);
    TEST_ASSERT_EQUAL_STRING("C", note.name);
    TEST_ASSERT_EQUAL_INT(5, note.octave);
}

void test_note_b4(void)
{
    freq_note_t note = freq_to_note(493.883);
    TEST_ASSERT_EQUAL_STRING("B", note.name);
    TEST_ASSERT_EQUAL_INT(4, note.octave);
}

void test_note_f_sharp_4(void)
{
    double fsharp4 = 440.0 * pow(2.0, (66.0 - 69.0) / 12.0);
    freq_note_t note = freq_to_note(fsharp4);
    TEST_ASSERT_EQUAL_STRING("F#", note.name);
    TEST_ASSERT_EQUAL_INT(4, note.octave);
}

void test_note_cents_off_between_notes(void)
{
    /* Frequency exactly between A4 and A#4 should have ~50 cents off */
    double a4 = 440.0;
    double asharp4 = 440.0 * pow(2.0, 1.0 / 12.0);
    double midway = sqrt(a4 * asharp4);
    freq_note_t note = freq_to_note(midway);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 50.0, fabs(note.cents_off));
}

void test_note_hz_preserved(void)
{
    freq_note_t note = freq_to_note(432.0);
    TEST_ASSERT_FLOAT_WITHIN(1e-6, 432.0, note.hz);
}

void test_note_zero_returns_empty(void)
{
    freq_note_t note = freq_to_note(0.0);
    TEST_ASSERT_EQUAL_INT(0, note.octave);
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 0.0, note.cents_off);
}

/* ===== freq_planet ===== */

void test_planet_count_is_10(void)
{
    TEST_ASSERT_EQUAL_INT(FREQ_MAX_PLANETS, freq_planet_count());
}

void test_planet_earth_day(void)
{
    freq_planet_t p = freq_planet(0);
    TEST_ASSERT_EQUAL_STRING("Earth day", p.name);
    TEST_ASSERT_FLOAT_WITHIN(0.5, 86400.0, p.orbital_period_s);
    TEST_ASSERT_TRUE(p.base_hz > 0.0);
    TEST_ASSERT_TRUE(p.audible_hz >= 20.0);
    TEST_ASSERT_TRUE(p.audible_hz <= 20000.0);
}

void test_planet_earth_day_note_g(void)
{
    /* Earth day octave-transposed -> ~194.18 Hz -> G3 */
    freq_planet_t p = freq_planet(0);
    TEST_ASSERT_EQUAL_STRING("G", p.note.name);
}

void test_planet_mercury(void)
{
    freq_planet_t p = freq_planet(1);
    TEST_ASSERT_EQUAL_STRING("Mercury", p.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 7600537.0, p.orbital_period_s);
}

void test_planet_earth_year(void)
{
    freq_planet_t p = freq_planet(3);
    TEST_ASSERT_EQUAL_STRING("Earth year", p.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 31558150.0, p.orbital_period_s);
}

void test_planet_earth_year_note_csharp(void)
{
    /* Earth year -> ~136.1 Hz -> C#3 (the "Om" frequency) */
    freq_planet_t p = freq_planet(3);
    TEST_ASSERT_EQUAL_STRING("C#", p.note.name);
}

void test_planet_moon(void)
{
    freq_planet_t p = freq_planet(9);
    TEST_ASSERT_EQUAL_STRING("Moon", p.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 2551443.0, p.orbital_period_s);
}

void test_planet_moon_note_gsharp(void)
{
    /* Synodic month -> ~210.42 Hz -> G#3 */
    freq_planet_t p = freq_planet(9);
    TEST_ASSERT_EQUAL_STRING("G#", p.note.name);
}

void test_planet_neptune(void)
{
    freq_planet_t p = freq_planet(8);
    TEST_ASSERT_EQUAL_STRING("Neptune", p.name);
    TEST_ASSERT_TRUE(p.audible_hz >= 20.0);
}

void test_planet_invalid_negative(void)
{
    freq_planet_t p = freq_planet(-1);
    TEST_ASSERT_EQUAL_STRING("?", p.name);
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 0.0, p.base_hz);
}

void test_planet_invalid_high(void)
{
    freq_planet_t p = freq_planet(10);
    TEST_ASSERT_EQUAL_STRING("?", p.name);
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 0.0, p.base_hz);
}

void test_planet_octaves_shifted_positive(void)
{
    /* All orbital frequencies are sub-audible, so octaves_shifted > 0 */
    freq_planet_t p = freq_planet(3); /* Earth year */
    TEST_ASSERT_TRUE(p.octaves_shifted > 0);
}

/* ===== freq_interval ===== */

void test_interval_unison(void)
{
    freq_interval_t iv = freq_interval(440.0, 440.0);
    TEST_ASSERT_EQUAL_STRING("unison", iv.name);
    TEST_ASSERT_EQUAL_INT(0, iv.semitones);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 0.0, iv.cents_off);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1.0, iv.ratio);
}

void test_interval_octave(void)
{
    freq_interval_t iv = freq_interval(440.0, 880.0);
    TEST_ASSERT_EQUAL_STRING("octave", iv.name);
    TEST_ASSERT_EQUAL_INT(12, iv.semitones);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 2.0, iv.ratio);
}

void test_interval_perfect_fifth(void)
{
    freq_interval_t iv = freq_interval(440.0, 660.0);
    TEST_ASSERT_EQUAL_STRING("perfect fifth", iv.name);
    TEST_ASSERT_EQUAL_INT(7, iv.semitones);
}

void test_interval_perfect_fourth(void)
{
    double hz_b = 440.0 * 4.0 / 3.0;
    freq_interval_t iv = freq_interval(440.0, hz_b);
    TEST_ASSERT_EQUAL_STRING("perfect fourth", iv.name);
    TEST_ASSERT_EQUAL_INT(5, iv.semitones);
}

void test_interval_major_third(void)
{
    double hz_b = 440.0 * 5.0 / 4.0;
    freq_interval_t iv = freq_interval(440.0, hz_b);
    TEST_ASSERT_EQUAL_STRING("major third", iv.name);
    TEST_ASSERT_EQUAL_INT(4, iv.semitones);
}

void test_interval_minor_third(void)
{
    double hz_b = 440.0 * 6.0 / 5.0;
    freq_interval_t iv = freq_interval(440.0, hz_b);
    TEST_ASSERT_EQUAL_STRING("minor third", iv.name);
    TEST_ASSERT_EQUAL_INT(3, iv.semitones);
}

void test_interval_ratio_preserved(void)
{
    freq_interval_t iv = freq_interval(200.0, 300.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1.5, iv.ratio);
}

void test_interval_zero_freq_returns_invalid(void)
{
    freq_interval_t iv = freq_interval(0.0, 440.0);
    TEST_ASSERT_EQUAL_INT(0, iv.semitones);
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 0.0, iv.ratio);
}

/* ===== freq_planet_interval ===== */

void test_planet_interval_same_planet(void)
{
    freq_interval_t iv = freq_planet_interval(3, 3);
    TEST_ASSERT_EQUAL_STRING("unison", iv.name);
    TEST_ASSERT_EQUAL_INT(0, iv.semitones);
}

void test_planet_interval_earth_day_vs_moon(void)
{
    /* Earth day vs Moon -- should produce a valid interval */
    freq_interval_t iv = freq_planet_interval(0, 9);
    TEST_ASSERT_TRUE(iv.semitones >= 0);
    TEST_ASSERT_TRUE(iv.semitones <= 12);
    TEST_ASSERT_TRUE(iv.ratio > 0.0);
}

/* ===== freq_octave_shift ===== */

void test_octave_shift_up_one(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-6, 880.0, freq_octave_shift(440.0, 1));
}

void test_octave_shift_down_one(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-6, 220.0, freq_octave_shift(440.0, -1));
}

void test_octave_shift_zero(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-6, 440.0, freq_octave_shift(440.0, 0));
}

void test_octave_shift_up_three(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-6, 3520.0, freq_octave_shift(440.0, 3));
}

/* ===== freq_semitone_distance ===== */

void test_semitone_distance_octave(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01, 12.0, freq_semitone_distance(440.0, 880.0));
}

void test_semitone_distance_unison(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0, freq_semitone_distance(440.0, 440.0));
}

void test_semitone_distance_one_semitone(void)
{
    double asharp4 = 440.0 * pow(2.0, 1.0 / 12.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 1.0, freq_semitone_distance(440.0, asharp4));
}

void test_semitone_distance_negative(void)
{
    /* 880 -> 440 = -12 semitones */
    TEST_ASSERT_FLOAT_WITHIN(0.01, -12.0, freq_semitone_distance(880.0, 440.0));
}

/* ===== freq_note_hz ===== */

void test_note_hz_a4(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01, 440.0, freq_note_hz("A", 4));
}

void test_note_hz_c4(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01, 261.63, freq_note_hz("C", 4));
}

void test_note_hz_a5(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01, 880.0, freq_note_hz("A", 5));
}

void test_note_hz_csharp4(void)
{
    double expected = 440.0 * pow(2.0, (61.0 - 69.0) / 12.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01, expected, freq_note_hz("C#", 4));
}

void test_note_hz_invalid_name_returns_zero(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 0.0, freq_note_hz("Z", 4));
}

void test_note_hz_null_returns_zero(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 0.0, freq_note_hz(NULL, 4));
}

/* ===== Roundtrip / integration tests ===== */

void test_roundtrip_period_frequency(void)
{
    double period = 1.0 / 440.0;
    double hz = freq_from_period(period);
    double back = 1.0 / hz;
    TEST_ASSERT_FLOAT_WITHIN(1e-8, period, back);
}

void test_all_planets_have_valid_audible(void)
{
    for (int i = 0; i < freq_planet_count(); i++) {
        freq_planet_t p = freq_planet(i);
        TEST_ASSERT_TRUE(p.audible_hz >= 20.0);
        TEST_ASSERT_TRUE(p.audible_hz <= 20000.0);
        TEST_ASSERT_TRUE(p.base_hz > 0.0);
    }
}

void test_all_planets_have_note_name(void)
{
    for (int i = 0; i < freq_planet_count(); i++) {
        freq_planet_t p = freq_planet(i);
        TEST_ASSERT_TRUE(strlen(p.note.name) > 0);
    }
}

void test_note_roundtrip_all_12(void)
{
    /* For each of 12 notes, get hz then map back -- name should match */
    const char *names[] = {
        "C","C#","D","D#","E","F","F#","G","G#","A","A#","B"
    };
    for (int i = 0; i < 12; i++) {
        double hz = freq_note_hz(names[i], 4);
        freq_note_t note = freq_to_note(hz);
        TEST_ASSERT_EQUAL_STRING(names[i], note.name);
        TEST_ASSERT_EQUAL_INT(4, note.octave);
    }
}

/* ===== main ===== */

int main(void)
{
    UNITY_BEGIN();

    /* freq_from_period */
    RUN_TEST(test_from_period_one_second);
    RUN_TEST(test_from_period_half_second);
    RUN_TEST(test_from_period_440hz);
    RUN_TEST(test_from_period_zero_returns_zero);
    RUN_TEST(test_from_period_negative_returns_zero);
    RUN_TEST(test_from_period_earth_day);

    /* freq_to_audible */
    RUN_TEST(test_to_audible_already_audible);
    RUN_TEST(test_to_audible_very_low);
    RUN_TEST(test_to_audible_very_high);
    RUN_TEST(test_to_audible_zero_returns_zero);
    RUN_TEST(test_to_audible_negative_returns_zero);
    RUN_TEST(test_to_audible_boundary_low);
    RUN_TEST(test_to_audible_boundary_high);
    RUN_TEST(test_to_audible_earth_day);
    RUN_TEST(test_to_audible_earth_year);
    RUN_TEST(test_to_audible_synodic_month);

    /* freq_to_note */
    RUN_TEST(test_note_a4);
    RUN_TEST(test_note_c4);
    RUN_TEST(test_note_a3);
    RUN_TEST(test_note_c5);
    RUN_TEST(test_note_b4);
    RUN_TEST(test_note_f_sharp_4);
    RUN_TEST(test_note_cents_off_between_notes);
    RUN_TEST(test_note_hz_preserved);
    RUN_TEST(test_note_zero_returns_empty);

    /* freq_planet */
    RUN_TEST(test_planet_count_is_10);
    RUN_TEST(test_planet_earth_day);
    RUN_TEST(test_planet_earth_day_note_g);
    RUN_TEST(test_planet_mercury);
    RUN_TEST(test_planet_earth_year);
    RUN_TEST(test_planet_earth_year_note_csharp);
    RUN_TEST(test_planet_moon);
    RUN_TEST(test_planet_moon_note_gsharp);
    RUN_TEST(test_planet_neptune);
    RUN_TEST(test_planet_invalid_negative);
    RUN_TEST(test_planet_invalid_high);
    RUN_TEST(test_planet_octaves_shifted_positive);

    /* freq_interval */
    RUN_TEST(test_interval_unison);
    RUN_TEST(test_interval_octave);
    RUN_TEST(test_interval_perfect_fifth);
    RUN_TEST(test_interval_perfect_fourth);
    RUN_TEST(test_interval_major_third);
    RUN_TEST(test_interval_minor_third);
    RUN_TEST(test_interval_ratio_preserved);
    RUN_TEST(test_interval_zero_freq_returns_invalid);

    /* freq_planet_interval */
    RUN_TEST(test_planet_interval_same_planet);
    RUN_TEST(test_planet_interval_earth_day_vs_moon);

    /* freq_octave_shift */
    RUN_TEST(test_octave_shift_up_one);
    RUN_TEST(test_octave_shift_down_one);
    RUN_TEST(test_octave_shift_zero);
    RUN_TEST(test_octave_shift_up_three);

    /* freq_semitone_distance */
    RUN_TEST(test_semitone_distance_octave);
    RUN_TEST(test_semitone_distance_unison);
    RUN_TEST(test_semitone_distance_one_semitone);
    RUN_TEST(test_semitone_distance_negative);

    /* freq_note_hz */
    RUN_TEST(test_note_hz_a4);
    RUN_TEST(test_note_hz_c4);
    RUN_TEST(test_note_hz_a5);
    RUN_TEST(test_note_hz_csharp4);
    RUN_TEST(test_note_hz_invalid_name_returns_zero);
    RUN_TEST(test_note_hz_null_returns_zero);

    /* Roundtrip / integration */
    RUN_TEST(test_roundtrip_period_frequency);
    RUN_TEST(test_all_planets_have_valid_audible);
    RUN_TEST(test_all_planets_have_note_name);
    RUN_TEST(test_note_roundtrip_all_12);

    return UNITY_END();
}
