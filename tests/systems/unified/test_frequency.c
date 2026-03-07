/* test_frequency.c — Frequency Mapper tests
 * TDD RED phase: tests written before implementation.
 * Tests conversion of time cycles to frequencies, octave transposition
 * to audible range, musical note mapping (12-TET, A4=440Hz),
 * interval detection, and planetary orbital frequencies. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/frequency.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== frequency_from_period ===== */

void test_from_period_one_second(void)
{
    /* 1-second period = 1 Hz */
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 1.0, frequency_from_period(1.0));
}

void test_from_period_half_second(void)
{
    /* 0.5-second period = 2 Hz */
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 2.0, frequency_from_period(0.5));
}

void test_from_period_440hz(void)
{
    /* period of A4 = 1/440 */
    double period = 1.0 / 440.0;
    TEST_ASSERT_FLOAT_WITHIN(1e-6, 440.0, frequency_from_period(period));
}

void test_from_period_zero(void)
{
    /* zero period = 0 Hz (error case) */
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 0.0, frequency_from_period(0.0));
}

void test_from_period_negative(void)
{
    /* negative period = 0 Hz (error case) */
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 0.0, frequency_from_period(-5.0));
}

/* ===== frequency_to_period ===== */

void test_to_period_one_hz(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 1.0, frequency_to_period(1.0));
}

void test_to_period_440hz(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-8, 1.0 / 440.0, frequency_to_period(440.0));
}

void test_to_period_zero(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 0.0, frequency_to_period(0.0));
}

void test_to_period_negative(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 0.0, frequency_to_period(-100.0));
}

/* ===== frequency_from_days / frequency_from_years ===== */

void test_from_days_one(void)
{
    /* 1 day = 86400 seconds, freq = 1/86400 */
    double expected = 1.0 / 86400.0;
    TEST_ASSERT_FLOAT_WITHIN(1e-15, expected, frequency_from_days(1.0));
}

void test_from_days_earth_year(void)
{
    /* Earth orbital period = 365.256 days */
    double expected = 1.0 / (365.256 * 86400.0);
    TEST_ASSERT_FLOAT_WITHIN(1e-20, expected, frequency_from_days(365.256));
}

void test_from_days_zero(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 0.0, frequency_from_days(0.0));
}

void test_from_years_one(void)
{
    /* 1 Julian year = 365.25 * 86400 = 31557600 seconds */
    double expected = 1.0 / SECONDS_PER_YEAR;
    TEST_ASSERT_FLOAT_WITHIN(1e-20, expected, frequency_from_years(1.0));
}

void test_from_years_zero(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 0.0, frequency_from_years(0.0));
}

/* ===== frequency_to_audible ===== */

void test_to_audible_already_audible(void)
{
    /* 440 Hz is already audible */
    TEST_ASSERT_FLOAT_WITHIN(1e-6, 440.0, frequency_to_audible(440.0));
}

void test_to_audible_very_low(void)
{
    /* 1 Hz should be doubled repeatedly until >= 20 Hz */
    double result = frequency_to_audible(1.0);
    TEST_ASSERT_TRUE(result >= FREQUENCY_AUDIBLE_MIN);
    TEST_ASSERT_TRUE(result <= FREQUENCY_AUDIBLE_MAX);
    /* 1 * 2^5 = 32 Hz (first octave above 20) */
    TEST_ASSERT_FLOAT_WITHIN(1e-6, 32.0, result);
}

void test_to_audible_very_high(void)
{
    /* 40000 Hz should be halved to get into range */
    double result = frequency_to_audible(40000.0);
    TEST_ASSERT_TRUE(result >= FREQUENCY_AUDIBLE_MIN);
    TEST_ASSERT_TRUE(result <= FREQUENCY_AUDIBLE_MAX);
    /* 40000 / 2 = 20000 exactly at max boundary */
    TEST_ASSERT_FLOAT_WITHIN(1e-6, 20000.0, result);
}

void test_to_audible_earth_year(void)
{
    /* Earth year: 365.256 days = 31558118.4 seconds
     * frequency = 1/31558118.4 ≈ 3.1687e-8 Hz
     * Lowest audible octave: 30 doublings -> ~34.024 Hz
     * (Cousto's 136.1 Hz is the 32nd octave, 2 octaves higher) */
    double hz = frequency_from_days(365.256);
    double audible = frequency_to_audible(hz);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 34.024, audible);
}

void test_to_audible_extreme_low(void)
{
    /* Extremely low frequency */
    double result = frequency_to_audible(1e-10);
    TEST_ASSERT_TRUE(result >= FREQUENCY_AUDIBLE_MIN);
    TEST_ASSERT_TRUE(result <= FREQUENCY_AUDIBLE_MAX);
}

void test_to_audible_boundary_low(void)
{
    /* 20 Hz is exactly at boundary — should stay */
    TEST_ASSERT_FLOAT_WITHIN(1e-6, 20.0, frequency_to_audible(20.0));
}

void test_to_audible_boundary_high(void)
{
    /* 20000 Hz is exactly at boundary — should stay */
    TEST_ASSERT_FLOAT_WITHIN(1e-6, 20000.0, frequency_to_audible(20000.0));
}

/* ===== frequency_octaves_to_audible ===== */

void test_octaves_to_audible_already_in_range(void)
{
    TEST_ASSERT_EQUAL_INT(0, frequency_octaves_to_audible(440.0));
}

void test_octaves_to_audible_1hz(void)
{
    /* 1 Hz needs 5 octaves up to get to 32 Hz */
    TEST_ASSERT_EQUAL_INT(5, frequency_octaves_to_audible(1.0));
}

void test_octaves_to_audible_40000hz(void)
{
    /* 40000 Hz needs 1 octave down */
    TEST_ASSERT_EQUAL_INT(-1, frequency_octaves_to_audible(40000.0));
}

/* ===== frequency_to_note ===== */

void test_note_a4(void)
{
    frequency_note_t note = frequency_to_note(440.0);
    TEST_ASSERT_EQUAL_STRING("A", note.name);
    TEST_ASSERT_EQUAL_INT(4, note.octave);
    TEST_ASSERT_FLOAT_WITHIN(0.5, 0.0, note.cents_off);
    TEST_ASSERT_EQUAL_INT(69, note.midi_number);
}

void test_note_c4(void)
{
    frequency_note_t note = frequency_to_note(261.626);
    TEST_ASSERT_EQUAL_STRING("C", note.name);
    TEST_ASSERT_EQUAL_INT(4, note.octave);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 0.0, note.cents_off);
    TEST_ASSERT_EQUAL_INT(60, note.midi_number);
}

void test_note_a3(void)
{
    /* A3 = 220 Hz */
    frequency_note_t note = frequency_to_note(220.0);
    TEST_ASSERT_EQUAL_STRING("A", note.name);
    TEST_ASSERT_EQUAL_INT(3, note.octave);
    TEST_ASSERT_EQUAL_INT(57, note.midi_number);
}

void test_note_c5(void)
{
    /* C5 = 523.251 Hz */
    frequency_note_t note = frequency_to_note(523.251);
    TEST_ASSERT_EQUAL_STRING("C", note.name);
    TEST_ASSERT_EQUAL_INT(5, note.octave);
    TEST_ASSERT_EQUAL_INT(72, note.midi_number);
}

void test_note_midi_60_is_c4(void)
{
    /* MIDI 60 = C4 = 261.626 Hz */
    double c4_hz = 440.0 * pow(2.0, (60.0 - 69.0) / 12.0);
    frequency_note_t note = frequency_to_note(c4_hz);
    TEST_ASSERT_EQUAL_STRING("C", note.name);
    TEST_ASSERT_EQUAL_INT(4, note.octave);
    TEST_ASSERT_EQUAL_INT(60, note.midi_number);
}

void test_note_b4(void)
{
    /* B4 = 493.883 Hz, MIDI 71 */
    frequency_note_t note = frequency_to_note(493.883);
    TEST_ASSERT_EQUAL_STRING("B", note.name);
    TEST_ASSERT_EQUAL_INT(4, note.octave);
    TEST_ASSERT_EQUAL_INT(71, note.midi_number);
}

void test_note_f_sharp_4(void)
{
    /* F#4 = 369.994 Hz, MIDI 66 */
    double fsharp4 = 440.0 * pow(2.0, (66.0 - 69.0) / 12.0);
    frequency_note_t note = frequency_to_note(fsharp4);
    TEST_ASSERT_EQUAL_STRING("F#", note.name);
    TEST_ASSERT_EQUAL_INT(4, note.octave);
    TEST_ASSERT_EQUAL_INT(66, note.midi_number);
}

void test_note_cents_off_between_notes(void)
{
    /* Frequency exactly between A4 and A#4 should have ~50 cents off */
    double a4 = 440.0;
    double asharp4 = 440.0 * pow(2.0, 1.0 / 12.0);
    double midway = sqrt(a4 * asharp4); /* geometric midpoint */
    frequency_note_t note = frequency_to_note(midway);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 50.0, fabs(note.cents_off));
}

void test_note_exact_hz_preserved(void)
{
    frequency_note_t note = frequency_to_note(432.0);
    TEST_ASSERT_FLOAT_WITHIN(1e-6, 432.0, note.exact_hz);
}

/* ===== frequency_note_hz ===== */

void test_note_hz_a4(void)
{
    /* note_index 9 = A, octave 4 -> 440 Hz */
    TEST_ASSERT_FLOAT_WITHIN(0.01, 440.0, frequency_note_hz(9, 4));
}

void test_note_hz_c4(void)
{
    /* note_index 0 = C, octave 4 -> 261.626 Hz */
    TEST_ASSERT_FLOAT_WITHIN(0.01, 261.626, frequency_note_hz(0, 4));
}

void test_note_hz_a5(void)
{
    /* A5 = 880 Hz */
    TEST_ASSERT_FLOAT_WITHIN(0.01, 880.0, frequency_note_hz(9, 5));
}

void test_note_hz_c0(void)
{
    /* C0 = very low */
    double c0 = 440.0 * pow(2.0, (0.0 - 69.0) / 12.0);
    /* MIDI 0 is C-1 in some conventions, but we use C0 = MIDI 12 */
    /* note_index=0, octave=0 -> MIDI = 12 */
    double expected = 440.0 * pow(2.0, (12.0 - 69.0) / 12.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01, expected, frequency_note_hz(0, 0));
    (void)c0;
}

/* ===== frequency_cents ===== */

void test_cents_octave(void)
{
    /* 440 to 880 = 1200 cents */
    TEST_ASSERT_FLOAT_WITHIN(0.01, 1200.0, frequency_cents(440.0, 880.0));
}

void test_cents_perfect_fifth(void)
{
    /* 440 to 660 = ~701.96 cents */
    TEST_ASSERT_FLOAT_WITHIN(0.1, 701.96, frequency_cents(440.0, 660.0));
}

void test_cents_unison(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0, frequency_cents(440.0, 440.0));
}

void test_cents_negative_interval(void)
{
    /* 880 to 440 = -1200 cents */
    TEST_ASSERT_FLOAT_WITHIN(0.01, -1200.0, frequency_cents(880.0, 440.0));
}

void test_cents_semitone(void)
{
    /* One semitone = 100 cents */
    double a4 = 440.0;
    double asharp4 = a4 * pow(2.0, 1.0 / 12.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 100.0, frequency_cents(a4, asharp4));
}

/* ===== frequency_interval ===== */

void test_interval_unison(void)
{
    frequency_interval_t iv = frequency_interval(440.0, 440.0);
    TEST_ASSERT_EQUAL_STRING("Unison", iv.name);
    TEST_ASSERT_EQUAL_INT(0, iv.semitones);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 0.0, iv.cents);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1.0, iv.ratio);
}

void test_interval_octave(void)
{
    frequency_interval_t iv = frequency_interval(440.0, 880.0);
    TEST_ASSERT_EQUAL_STRING("Octave", iv.name);
    TEST_ASSERT_EQUAL_INT(12, iv.semitones);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 1200.0, iv.cents);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 2.0, iv.ratio);
}

void test_interval_perfect_fifth(void)
{
    frequency_interval_t iv = frequency_interval(440.0, 660.0);
    TEST_ASSERT_EQUAL_STRING("Perfect 5th", iv.name);
    TEST_ASSERT_EQUAL_INT(7, iv.semitones);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 702.0, iv.cents);
}

void test_interval_perfect_fourth(void)
{
    /* ratio 4:3 = ~498 cents = 5 semitones */
    double hz_b = 440.0 * 4.0 / 3.0;
    frequency_interval_t iv = frequency_interval(440.0, hz_b);
    TEST_ASSERT_EQUAL_STRING("Perfect 4th", iv.name);
    TEST_ASSERT_EQUAL_INT(5, iv.semitones);
}

void test_interval_major_third(void)
{
    /* ratio 5:4 = ~386 cents = 4 semitones */
    double hz_b = 440.0 * 5.0 / 4.0;
    frequency_interval_t iv = frequency_interval(440.0, hz_b);
    TEST_ASSERT_EQUAL_STRING("Major 3rd", iv.name);
    TEST_ASSERT_EQUAL_INT(4, iv.semitones);
}

void test_interval_minor_third(void)
{
    /* ratio 6:5 = ~316 cents = 3 semitones */
    double hz_b = 440.0 * 6.0 / 5.0;
    frequency_interval_t iv = frequency_interval(440.0, hz_b);
    TEST_ASSERT_EQUAL_STRING("Minor 3rd", iv.name);
    TEST_ASSERT_EQUAL_INT(3, iv.semitones);
}

void test_interval_ratio_preserved(void)
{
    frequency_interval_t iv = frequency_interval(200.0, 300.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1.5, iv.ratio);
}

/* ===== frequency_note_name ===== */

void test_note_name_c(void)
{
    TEST_ASSERT_EQUAL_STRING("C", frequency_note_name(0));
}

void test_note_name_a(void)
{
    TEST_ASSERT_EQUAL_STRING("A", frequency_note_name(9));
}

void test_note_name_b(void)
{
    TEST_ASSERT_EQUAL_STRING("B", frequency_note_name(11));
}

void test_note_name_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("?", frequency_note_name(-1));
}

void test_note_name_invalid_high(void)
{
    TEST_ASSERT_EQUAL_STRING("?", frequency_note_name(12));
}

/* ===== frequency_interval_name ===== */

void test_interval_name_unison(void)
{
    TEST_ASSERT_EQUAL_STRING("Unison", frequency_interval_name(0));
}

void test_interval_name_octave(void)
{
    TEST_ASSERT_EQUAL_STRING("Octave", frequency_interval_name(12));
}

void test_interval_name_perfect_fifth(void)
{
    TEST_ASSERT_EQUAL_STRING("Perfect 5th", frequency_interval_name(7));
}

void test_interval_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", frequency_interval_name(13));
}

void test_interval_name_negative(void)
{
    TEST_ASSERT_EQUAL_STRING("?", frequency_interval_name(-1));
}

/* ===== frequency_planet ===== */

void test_planet_mercury(void)
{
    planet_frequency_t p = frequency_planet(0);
    TEST_ASSERT_EQUAL_STRING("Mercury", p.name);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 87.969, p.orbital_period_days);
    TEST_ASSERT_TRUE(p.orbital_hz > 0.0);
    TEST_ASSERT_TRUE(p.audible_hz >= FREQUENCY_AUDIBLE_MIN);
    TEST_ASSERT_TRUE(p.audible_hz <= FREQUENCY_AUDIBLE_MAX);
}

void test_planet_earth(void)
{
    planet_frequency_t p = frequency_planet(2);
    TEST_ASSERT_EQUAL_STRING("Earth", p.name);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 365.256, p.orbital_period_days);
    /* Earth audible: lowest audible octave ~34.024 Hz (C#1) */
    TEST_ASSERT_FLOAT_WITHIN(0.1, 34.024, p.audible_hz);
}

void test_planet_earth_note_is_csharp(void)
{
    planet_frequency_t p = frequency_planet(2);
    TEST_ASSERT_EQUAL_STRING("C#", p.note.name);
}

void test_planet_neptune(void)
{
    planet_frequency_t p = frequency_planet(7);
    TEST_ASSERT_EQUAL_STRING("Neptune", p.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 60182.0, p.orbital_period_days);
    TEST_ASSERT_TRUE(p.audible_hz >= FREQUENCY_AUDIBLE_MIN);
}

void test_planet_invalid_index(void)
{
    planet_frequency_t p = frequency_planet(-1);
    TEST_ASSERT_EQUAL_STRING("?", p.name);
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 0.0, p.orbital_hz);
}

void test_planet_invalid_high(void)
{
    planet_frequency_t p = frequency_planet(8);
    TEST_ASSERT_EQUAL_STRING("?", p.name);
    TEST_ASSERT_FLOAT_WITHIN(1e-10, 0.0, p.orbital_hz);
}

void test_planet_count(void)
{
    TEST_ASSERT_EQUAL_INT(8, frequency_planet_count());
}

/* ===== Roundtrip / integration tests ===== */

void test_roundtrip_period_frequency(void)
{
    /* period -> freq -> period should roundtrip */
    double period = 0.002272727; /* ~440 Hz */
    double hz = frequency_from_period(period);
    double back = frequency_to_period(hz);
    TEST_ASSERT_FLOAT_WITHIN(1e-8, period, back);
}

void test_note_hz_roundtrip(void)
{
    /* note_hz -> to_note -> should give same note index */
    double hz = frequency_note_hz(5, 4); /* F4 */
    frequency_note_t note = frequency_to_note(hz);
    TEST_ASSERT_EQUAL_STRING("F", note.name);
    TEST_ASSERT_EQUAL_INT(4, note.octave);
}

void test_all_12_notes_distinct(void)
{
    /* Every chromatic note in octave 4 should map to a distinct name */
    const char *seen[12];
    for (int i = 0; i < 12; i++) {
        double hz = frequency_note_hz(i, 4);
        frequency_note_t note = frequency_to_note(hz);
        seen[i] = note.name;
        /* Verify the note maps back correctly */
        TEST_ASSERT_EQUAL_STRING(frequency_note_name(i), note.name);
    }
    /* Check all are distinct */
    for (int i = 0; i < 12; i++) {
        for (int j = i + 1; j < 12; j++) {
            TEST_ASSERT_TRUE(strcmp(seen[i], seen[j]) != 0);
        }
    }
}

void test_all_planets_have_valid_audible(void)
{
    for (int i = 0; i < FREQUENCY_PLANET_COUNT; i++) {
        planet_frequency_t p = frequency_planet(i);
        TEST_ASSERT_TRUE(p.audible_hz >= FREQUENCY_AUDIBLE_MIN);
        TEST_ASSERT_TRUE(p.audible_hz <= FREQUENCY_AUDIBLE_MAX);
        TEST_ASSERT_TRUE(p.orbital_hz > 0.0);
    }
}

void test_all_planets_have_note(void)
{
    for (int i = 0; i < FREQUENCY_PLANET_COUNT; i++) {
        planet_frequency_t p = frequency_planet(i);
        TEST_ASSERT_TRUE(strlen(p.note.name) > 0);
        TEST_ASSERT_TRUE(p.note.midi_number >= 0);
    }
}

/* ===== main ===== */

int main(void)
{
    UNITY_BEGIN();

    /* frequency_from_period */
    RUN_TEST(test_from_period_one_second);
    RUN_TEST(test_from_period_half_second);
    RUN_TEST(test_from_period_440hz);
    RUN_TEST(test_from_period_zero);
    RUN_TEST(test_from_period_negative);

    /* frequency_to_period */
    RUN_TEST(test_to_period_one_hz);
    RUN_TEST(test_to_period_440hz);
    RUN_TEST(test_to_period_zero);
    RUN_TEST(test_to_period_negative);

    /* frequency_from_days / frequency_from_years */
    RUN_TEST(test_from_days_one);
    RUN_TEST(test_from_days_earth_year);
    RUN_TEST(test_from_days_zero);
    RUN_TEST(test_from_years_one);
    RUN_TEST(test_from_years_zero);

    /* frequency_to_audible */
    RUN_TEST(test_to_audible_already_audible);
    RUN_TEST(test_to_audible_very_low);
    RUN_TEST(test_to_audible_very_high);
    RUN_TEST(test_to_audible_earth_year);
    RUN_TEST(test_to_audible_extreme_low);
    RUN_TEST(test_to_audible_boundary_low);
    RUN_TEST(test_to_audible_boundary_high);

    /* frequency_octaves_to_audible */
    RUN_TEST(test_octaves_to_audible_already_in_range);
    RUN_TEST(test_octaves_to_audible_1hz);
    RUN_TEST(test_octaves_to_audible_40000hz);

    /* frequency_to_note */
    RUN_TEST(test_note_a4);
    RUN_TEST(test_note_c4);
    RUN_TEST(test_note_a3);
    RUN_TEST(test_note_c5);
    RUN_TEST(test_note_midi_60_is_c4);
    RUN_TEST(test_note_b4);
    RUN_TEST(test_note_f_sharp_4);
    RUN_TEST(test_note_cents_off_between_notes);
    RUN_TEST(test_note_exact_hz_preserved);

    /* frequency_note_hz */
    RUN_TEST(test_note_hz_a4);
    RUN_TEST(test_note_hz_c4);
    RUN_TEST(test_note_hz_a5);
    RUN_TEST(test_note_hz_c0);

    /* frequency_cents */
    RUN_TEST(test_cents_octave);
    RUN_TEST(test_cents_perfect_fifth);
    RUN_TEST(test_cents_unison);
    RUN_TEST(test_cents_negative_interval);
    RUN_TEST(test_cents_semitone);

    /* frequency_interval */
    RUN_TEST(test_interval_unison);
    RUN_TEST(test_interval_octave);
    RUN_TEST(test_interval_perfect_fifth);
    RUN_TEST(test_interval_perfect_fourth);
    RUN_TEST(test_interval_major_third);
    RUN_TEST(test_interval_minor_third);
    RUN_TEST(test_interval_ratio_preserved);

    /* frequency_note_name */
    RUN_TEST(test_note_name_c);
    RUN_TEST(test_note_name_a);
    RUN_TEST(test_note_name_b);
    RUN_TEST(test_note_name_invalid_negative);
    RUN_TEST(test_note_name_invalid_high);

    /* frequency_interval_name */
    RUN_TEST(test_interval_name_unison);
    RUN_TEST(test_interval_name_octave);
    RUN_TEST(test_interval_name_perfect_fifth);
    RUN_TEST(test_interval_name_invalid);
    RUN_TEST(test_interval_name_negative);

    /* frequency_planet */
    RUN_TEST(test_planet_mercury);
    RUN_TEST(test_planet_earth);
    RUN_TEST(test_planet_earth_note_is_csharp);
    RUN_TEST(test_planet_neptune);
    RUN_TEST(test_planet_invalid_index);
    RUN_TEST(test_planet_invalid_high);
    RUN_TEST(test_planet_count);

    /* Roundtrip / integration */
    RUN_TEST(test_roundtrip_period_frequency);
    RUN_TEST(test_note_hz_roundtrip);
    RUN_TEST(test_all_12_notes_distinct);
    RUN_TEST(test_all_planets_have_valid_audible);
    RUN_TEST(test_all_planets_have_note);

    return UNITY_END();
}
