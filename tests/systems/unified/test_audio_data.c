/* test_audio_data.c -- Web Audio Data module tests
 * TDD RED phase: tests written before implementation.
 * Tests harmonic series generation per planet, aspect sonification
 * mappings, moon phase amplitude envelopes, interval lookups,
 * and planet audio profiles.
 *
 * Planet table (10 entries):
 *   0=Earth day, 1=Mercury, 2=Venus, 3=Earth year, 4=Mars,
 *   5=Jupiter, 6=Saturn, 7=Uranus, 8=Neptune, 9=Moon (synodic) */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/audio_data.h"
#include <math.h>
#include <string.h>

#define PI 3.14159265358979323846

void setUp(void) {}
void tearDown(void) {}

/* ===== audio_planet_profile ===== */

void test_profile_earth_day_index(void)
{
    audio_planet_profile_t p = audio_planet_profile(0);
    TEST_ASSERT_EQUAL_INT(0, p.planet_index);
}

void test_profile_earth_day_harmonics_count(void)
{
    audio_planet_profile_t p = audio_planet_profile(0);
    TEST_ASSERT_EQUAL_INT(2, p.harmonic_count);
}

void test_profile_earth_day_waveform_sine(void)
{
    audio_planet_profile_t p = audio_planet_profile(0);
    TEST_ASSERT_EQUAL_INT(AUDIO_WAVE_SINE, p.waveform);
}

void test_profile_earth_day_amplitude(void)
{
    audio_planet_profile_t p = audio_planet_profile(0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.3f, (float)p.base_amplitude);
}

void test_profile_mercury_harmonics_count(void)
{
    audio_planet_profile_t p = audio_planet_profile(1);
    TEST_ASSERT_EQUAL_INT(6, p.harmonic_count);
}

void test_profile_mercury_waveform_sawtooth(void)
{
    audio_planet_profile_t p = audio_planet_profile(1);
    TEST_ASSERT_EQUAL_INT(AUDIO_WAVE_SAWTOOTH, p.waveform);
}

void test_profile_mercury_amplitude(void)
{
    audio_planet_profile_t p = audio_planet_profile(1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.15f, (float)p.base_amplitude);
}

void test_profile_venus_harmonics_count(void)
{
    audio_planet_profile_t p = audio_planet_profile(2);
    TEST_ASSERT_EQUAL_INT(2, p.harmonic_count);
}

void test_profile_venus_waveform_sine(void)
{
    audio_planet_profile_t p = audio_planet_profile(2);
    TEST_ASSERT_EQUAL_INT(AUDIO_WAVE_SINE, p.waveform);
}

void test_profile_venus_amplitude(void)
{
    audio_planet_profile_t p = audio_planet_profile(2);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.25f, (float)p.base_amplitude);
}

void test_profile_earth_year_harmonics_count(void)
{
    audio_planet_profile_t p = audio_planet_profile(3);
    TEST_ASSERT_EQUAL_INT(3, p.harmonic_count);
}

void test_profile_earth_year_waveform_sine(void)
{
    audio_planet_profile_t p = audio_planet_profile(3);
    TEST_ASSERT_EQUAL_INT(AUDIO_WAVE_SINE, p.waveform);
}

void test_profile_earth_year_amplitude(void)
{
    audio_planet_profile_t p = audio_planet_profile(3);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.2f, (float)p.base_amplitude);
}

void test_profile_mars_harmonics_count(void)
{
    audio_planet_profile_t p = audio_planet_profile(4);
    TEST_ASSERT_EQUAL_INT(5, p.harmonic_count);
}

void test_profile_mars_waveform_triangle(void)
{
    audio_planet_profile_t p = audio_planet_profile(4);
    TEST_ASSERT_EQUAL_INT(AUDIO_WAVE_TRIANGLE, p.waveform);
}

void test_profile_mars_amplitude(void)
{
    audio_planet_profile_t p = audio_planet_profile(4);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.2f, (float)p.base_amplitude);
}

void test_profile_jupiter_harmonics_count(void)
{
    audio_planet_profile_t p = audio_planet_profile(5);
    TEST_ASSERT_EQUAL_INT(4, p.harmonic_count);
}

void test_profile_jupiter_waveform_triangle(void)
{
    audio_planet_profile_t p = audio_planet_profile(5);
    TEST_ASSERT_EQUAL_INT(AUDIO_WAVE_TRIANGLE, p.waveform);
}

void test_profile_jupiter_amplitude(void)
{
    audio_planet_profile_t p = audio_planet_profile(5);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.3f, (float)p.base_amplitude);
}

void test_profile_saturn_harmonics_count(void)
{
    audio_planet_profile_t p = audio_planet_profile(6);
    TEST_ASSERT_EQUAL_INT(4, p.harmonic_count);
}

void test_profile_saturn_waveform_triangle(void)
{
    audio_planet_profile_t p = audio_planet_profile(6);
    TEST_ASSERT_EQUAL_INT(AUDIO_WAVE_TRIANGLE, p.waveform);
}

void test_profile_saturn_amplitude(void)
{
    audio_planet_profile_t p = audio_planet_profile(6);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.25f, (float)p.base_amplitude);
}

void test_profile_uranus_harmonics_count(void)
{
    audio_planet_profile_t p = audio_planet_profile(7);
    TEST_ASSERT_EQUAL_INT(3, p.harmonic_count);
}

void test_profile_uranus_waveform_sine(void)
{
    audio_planet_profile_t p = audio_planet_profile(7);
    TEST_ASSERT_EQUAL_INT(AUDIO_WAVE_SINE, p.waveform);
}

void test_profile_uranus_amplitude(void)
{
    audio_planet_profile_t p = audio_planet_profile(7);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.1f, (float)p.base_amplitude);
}

void test_profile_neptune_harmonics_count(void)
{
    audio_planet_profile_t p = audio_planet_profile(8);
    TEST_ASSERT_EQUAL_INT(2, p.harmonic_count);
}

void test_profile_neptune_waveform_sine(void)
{
    audio_planet_profile_t p = audio_planet_profile(8);
    TEST_ASSERT_EQUAL_INT(AUDIO_WAVE_SINE, p.waveform);
}

void test_profile_neptune_amplitude(void)
{
    audio_planet_profile_t p = audio_planet_profile(8);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.08f, (float)p.base_amplitude);
}

void test_profile_moon_harmonics_count(void)
{
    audio_planet_profile_t p = audio_planet_profile(9);
    TEST_ASSERT_EQUAL_INT(4, p.harmonic_count);
}

void test_profile_moon_waveform_triangle(void)
{
    audio_planet_profile_t p = audio_planet_profile(9);
    TEST_ASSERT_EQUAL_INT(AUDIO_WAVE_TRIANGLE, p.waveform);
}

void test_profile_moon_amplitude(void)
{
    audio_planet_profile_t p = audio_planet_profile(9);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.35f, (float)p.base_amplitude);
}

void test_profile_invalid_negative(void)
{
    audio_planet_profile_t p = audio_planet_profile(-1);
    TEST_ASSERT_EQUAL_INT(-1, p.planet_index);
    TEST_ASSERT_EQUAL_INT(0, p.harmonic_count);
}

void test_profile_invalid_too_high(void)
{
    audio_planet_profile_t p = audio_planet_profile(10);
    TEST_ASSERT_EQUAL_INT(-1, p.planet_index);
    TEST_ASSERT_EQUAL_INT(0, p.harmonic_count);
}

void test_profile_fundamental_positive(void)
{
    /* All valid planets should have a positive fundamental frequency */
    for (int i = 0; i < AUDIO_MAX_PLANETS; i++) {
        audio_planet_profile_t p = audio_planet_profile(i);
        TEST_ASSERT_TRUE(p.fundamental_hz > 0.0);
    }
}

void test_profile_fundamental_matches_freq_planet(void)
{
    /* fundamental_hz should come from freq_planet audible_hz */
    for (int i = 0; i < AUDIO_MAX_PLANETS; i++) {
        audio_planet_profile_t p = audio_planet_profile(i);
        double expected = audio_planet_freq(i);
        TEST_ASSERT_FLOAT_WITHIN(0.01f, (float)expected, (float)p.fundamental_hz);
    }
}

/* ===== audio_planet_freq ===== */

void test_planet_freq_all_positive(void)
{
    for (int i = 0; i < AUDIO_MAX_PLANETS; i++) {
        double hz = audio_planet_freq(i);
        TEST_ASSERT_TRUE(hz > 0.0);
    }
}

void test_planet_freq_audible_range(void)
{
    for (int i = 0; i < AUDIO_MAX_PLANETS; i++) {
        double hz = audio_planet_freq(i);
        TEST_ASSERT_TRUE(hz >= 20.0);
        TEST_ASSERT_TRUE(hz <= 20000.0);
    }
}

void test_planet_freq_invalid_returns_zero(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)audio_planet_freq(-1));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)audio_planet_freq(10));
}

/* ===== audio_planet_harmonics ===== */

void test_harmonics_mercury_six(void)
{
    audio_harmonic_t out[AUDIO_MAX_HARMONICS];
    int count = audio_planet_harmonics(1, out, AUDIO_MAX_HARMONICS);
    TEST_ASSERT_EQUAL_INT(6, count);
}

void test_harmonics_fundamental_is_one(void)
{
    /* First harmonic always has harmonic_number=1, freq_ratio=1.0 */
    audio_harmonic_t out[AUDIO_MAX_HARMONICS];
    audio_planet_harmonics(0, out, AUDIO_MAX_HARMONICS);
    TEST_ASSERT_EQUAL_INT(1, out[0].harmonic_number);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, (float)out[0].freq_ratio);
}

void test_harmonics_fundamental_amplitude_is_one(void)
{
    /* Fundamental (n=1) should have amplitude 1.0 from the formula */
    /* h[1] = 1.0 / (1 * (1 + (1-ecc)*1)) = 1.0 / (2 - ecc) */
    /* Actually for n=1: h[1] = 1/(1*(1+(1-e)*1)) = 1/(2-e) */
    /* But we normalize so fundamental = 1.0 */
    audio_harmonic_t out[AUDIO_MAX_HARMONICS];
    audio_planet_harmonics(0, out, AUDIO_MAX_HARMONICS);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, (float)out[0].amplitude);
}

void test_harmonics_overtone_freq_ratio(void)
{
    /* Second harmonic should have freq_ratio = 2.0 */
    audio_harmonic_t out[AUDIO_MAX_HARMONICS];
    audio_planet_harmonics(0, out, AUDIO_MAX_HARMONICS);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 2.0f, (float)out[1].freq_ratio);
}

void test_harmonics_amplitude_decreasing(void)
{
    /* Amplitudes should generally decrease with harmonic number */
    audio_harmonic_t out[AUDIO_MAX_HARMONICS];
    int count = audio_planet_harmonics(1, out, AUDIO_MAX_HARMONICS);
    for (int i = 1; i < count; i++) {
        TEST_ASSERT_TRUE(out[i].amplitude < out[i - 1].amplitude);
    }
}

void test_harmonics_limited_by_max(void)
{
    /* If we pass max_harmonics = 2, should only get 2 even if planet has more */
    audio_harmonic_t out[2];
    int count = audio_planet_harmonics(1, out, 2);
    TEST_ASSERT_EQUAL_INT(2, count);
}

void test_harmonics_invalid_planet(void)
{
    audio_harmonic_t out[AUDIO_MAX_HARMONICS];
    int count = audio_planet_harmonics(-1, out, AUDIO_MAX_HARMONICS);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_harmonics_null_output(void)
{
    int count = audio_planet_harmonics(0, NULL, AUDIO_MAX_HARMONICS);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_harmonics_mercury_eccentricity_formula(void)
{
    /* Mercury eccentricity = 0.2056
     * h[1] = 1.0 / (1 * (1 + 0.7944*1)) = 1.0 / 1.7944 = 0.5573
     * normalized: h[1] = 1.0
     * h[2] = 1.0 / (2 * (1 + 0.7944*2)) = 1/(2*2.5888) = 1/5.1776 = 0.19314
     * normalized: h[2] = 0.19314 / 0.5573 = 0.3465 */
    audio_harmonic_t out[AUDIO_MAX_HARMONICS];
    audio_planet_harmonics(1, out, AUDIO_MAX_HARMONICS);
    /* raw h[2] / raw h[1] */
    double ecc = 0.2056;
    double h1 = 1.0 / (1.0 * (1.0 + (1.0 - ecc) * 1.0));
    double h2 = 1.0 / (2.0 * (1.0 + (1.0 - ecc) * 2.0));
    double expected_ratio = h2 / h1;
    TEST_ASSERT_FLOAT_WITHIN(0.01f, (float)expected_ratio, (float)out[1].amplitude);
}

/* ===== audio_aspect_map ===== */

void test_aspect_conjunction_angle(void)
{
    audio_aspect_map_t m = audio_aspect_map(AUDIO_ASPECT_CONJUNCTION);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)m.angle_deg);
}

void test_aspect_conjunction_consonance(void)
{
    audio_aspect_map_t m = audio_aspect_map(AUDIO_ASPECT_CONJUNCTION);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, (float)m.consonance);
}

void test_aspect_conjunction_volume(void)
{
    audio_aspect_map_t m = audio_aspect_map(AUDIO_ASPECT_CONJUNCTION);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.5f, (float)m.volume_factor);
}

void test_aspect_conjunction_interval(void)
{
    audio_aspect_map_t m = audio_aspect_map(AUDIO_ASPECT_CONJUNCTION);
    TEST_ASSERT_EQUAL_STRING("unison", m.musical_interval);
}

void test_aspect_conjunction_name(void)
{
    audio_aspect_map_t m = audio_aspect_map(AUDIO_ASPECT_CONJUNCTION);
    TEST_ASSERT_EQUAL_STRING("conjunction", m.name);
}

void test_aspect_sextile_angle(void)
{
    audio_aspect_map_t m = audio_aspect_map(AUDIO_ASPECT_SEXTILE);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 60.0f, (float)m.angle_deg);
}

void test_aspect_sextile_consonance(void)
{
    audio_aspect_map_t m = audio_aspect_map(AUDIO_ASPECT_SEXTILE);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.7f, (float)m.consonance);
}

void test_aspect_sextile_volume(void)
{
    audio_aspect_map_t m = audio_aspect_map(AUDIO_ASPECT_SEXTILE);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.1f, (float)m.volume_factor);
}

void test_aspect_square_angle(void)
{
    audio_aspect_map_t m = audio_aspect_map(AUDIO_ASPECT_SQUARE);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 90.0f, (float)m.angle_deg);
}

void test_aspect_square_consonance(void)
{
    audio_aspect_map_t m = audio_aspect_map(AUDIO_ASPECT_SQUARE);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, (float)m.consonance);
}

void test_aspect_square_volume(void)
{
    audio_aspect_map_t m = audio_aspect_map(AUDIO_ASPECT_SQUARE);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.8f, (float)m.volume_factor);
}

void test_aspect_trine_angle(void)
{
    audio_aspect_map_t m = audio_aspect_map(AUDIO_ASPECT_TRINE);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 120.0f, (float)m.angle_deg);
}

void test_aspect_trine_consonance(void)
{
    audio_aspect_map_t m = audio_aspect_map(AUDIO_ASPECT_TRINE);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.9f, (float)m.consonance);
}

void test_aspect_trine_volume(void)
{
    audio_aspect_map_t m = audio_aspect_map(AUDIO_ASPECT_TRINE);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.3f, (float)m.volume_factor);
}

void test_aspect_opposition_angle(void)
{
    audio_aspect_map_t m = audio_aspect_map(AUDIO_ASPECT_OPPOSITION);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 180.0f, (float)m.angle_deg);
}

void test_aspect_opposition_consonance(void)
{
    audio_aspect_map_t m = audio_aspect_map(AUDIO_ASPECT_OPPOSITION);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.8f, (float)m.consonance);
}

void test_aspect_opposition_volume(void)
{
    audio_aspect_map_t m = audio_aspect_map(AUDIO_ASPECT_OPPOSITION);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.2f, (float)m.volume_factor);
}

void test_aspect_opposition_interval(void)
{
    audio_aspect_map_t m = audio_aspect_map(AUDIO_ASPECT_OPPOSITION);
    TEST_ASSERT_EQUAL_STRING("octave", m.musical_interval);
}

/* ===== audio_aspect_harmony ===== */

void test_aspect_harmony_conjunction(void)
{
    const char *h = audio_aspect_harmony(AUDIO_ASPECT_CONJUNCTION);
    TEST_ASSERT_EQUAL_STRING("unison", h);
}

void test_aspect_harmony_sextile(void)
{
    const char *h = audio_aspect_harmony(AUDIO_ASPECT_SEXTILE);
    TEST_ASSERT_EQUAL_STRING("minor third", h);
}

void test_aspect_harmony_square(void)
{
    const char *h = audio_aspect_harmony(AUDIO_ASPECT_SQUARE);
    TEST_ASSERT_EQUAL_STRING("fourth", h);
}

void test_aspect_harmony_trine(void)
{
    const char *h = audio_aspect_harmony(AUDIO_ASPECT_TRINE);
    TEST_ASSERT_EQUAL_STRING("fifth", h);
}

void test_aspect_harmony_opposition(void)
{
    const char *h = audio_aspect_harmony(AUDIO_ASPECT_OPPOSITION);
    TEST_ASSERT_EQUAL_STRING("octave", h);
}

/* ===== audio_aspect_consonance ===== */

void test_consonance_conjunction(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, (float)audio_aspect_consonance(AUDIO_ASPECT_CONJUNCTION));
}

void test_consonance_square_lowest(void)
{
    /* Square has the lowest consonance: 0.5 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, (float)audio_aspect_consonance(AUDIO_ASPECT_SQUARE));
}

void test_consonance_invalid_returns_zero(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)audio_aspect_consonance(AUDIO_ASPECT_COUNT));
}

/* ===== audio_moon_envelope ===== */

void test_moon_envelope_new_moon(void)
{
    /* phase 0.0 -> amplitude 0.2 */
    double amp = audio_moon_envelope(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.2f, (float)amp);
}

void test_moon_envelope_full_moon(void)
{
    /* phase 0.5 -> amplitude 1.0 */
    double amp = audio_moon_envelope(0.5);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, (float)amp);
}

void test_moon_envelope_next_new(void)
{
    /* phase 1.0 -> amplitude 0.2 */
    double amp = audio_moon_envelope(1.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.2f, (float)amp);
}

void test_moon_envelope_first_quarter(void)
{
    /* phase 0.25 -> amplitude ~0.6 */
    double amp = audio_moon_envelope(0.25);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.6f, (float)amp);
}

void test_moon_envelope_last_quarter(void)
{
    /* phase 0.75 -> amplitude ~0.6 */
    double amp = audio_moon_envelope(0.75);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.6f, (float)amp);
}

void test_moon_envelope_waxing_crescent(void)
{
    /* phase 0.125 -> ~0.4 */
    double expected = 0.2 + 0.8 * (0.5 + 0.5 * cos(2.0 * PI * (0.125 - 0.5)));
    double amp = audio_moon_envelope(0.125);
    TEST_ASSERT_FLOAT_WITHIN(0.02f, (float)expected, (float)amp);
}

void test_moon_envelope_waxing_gibbous(void)
{
    /* phase 0.375 -> ~0.8 */
    double expected = 0.2 + 0.8 * (0.5 + 0.5 * cos(2.0 * PI * (0.375 - 0.5)));
    double amp = audio_moon_envelope(0.375);
    TEST_ASSERT_FLOAT_WITHIN(0.02f, (float)expected, (float)amp);
}

void test_moon_envelope_waning_gibbous(void)
{
    /* phase 0.625 -> ~0.8 */
    double expected = 0.2 + 0.8 * (0.5 + 0.5 * cos(2.0 * PI * (0.625 - 0.5)));
    double amp = audio_moon_envelope(0.625);
    TEST_ASSERT_FLOAT_WITHIN(0.02f, (float)expected, (float)amp);
}

void test_moon_envelope_waning_crescent(void)
{
    /* phase 0.875 -> ~0.4 */
    double expected = 0.2 + 0.8 * (0.5 + 0.5 * cos(2.0 * PI * (0.875 - 0.5)));
    double amp = audio_moon_envelope(0.875);
    TEST_ASSERT_FLOAT_WITHIN(0.02f, (float)expected, (float)amp);
}

void test_moon_envelope_symmetric(void)
{
    /* Envelope is symmetric around 0.5 */
    double amp_a = audio_moon_envelope(0.3);
    double amp_b = audio_moon_envelope(0.7);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)amp_a, (float)amp_b);
}

void test_moon_envelope_range(void)
{
    /* All values should be in [0.2, 1.0] */
    for (int i = 0; i <= 100; i++) {
        double phase = (double)i / 100.0;
        double amp = audio_moon_envelope(phase);
        TEST_ASSERT_TRUE(amp >= 0.19);
        TEST_ASSERT_TRUE(amp <= 1.01);
    }
}

void test_moon_envelope_clamp_negative(void)
{
    /* Negative phase should be clamped/wrapped */
    double amp = audio_moon_envelope(-0.5);
    TEST_ASSERT_TRUE(amp >= 0.0);
    TEST_ASSERT_TRUE(amp <= 1.1);
}

void test_moon_envelope_clamp_above_one(void)
{
    /* Phase > 1.0 should be clamped/wrapped */
    double amp = audio_moon_envelope(1.5);
    TEST_ASSERT_TRUE(amp >= 0.0);
    TEST_ASSERT_TRUE(amp <= 1.1);
}

/* ===== audio_interval_name ===== */

void test_interval_name_same_planet(void)
{
    const char *name = audio_interval_name(0, 0);
    TEST_ASSERT_EQUAL_STRING("unison", name);
}

void test_interval_name_returns_string(void)
{
    const char *name = audio_interval_name(0, 1);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(strlen(name) > 0);
}

void test_interval_name_invalid_returns_unknown(void)
{
    const char *name = audio_interval_name(-1, 0);
    TEST_ASSERT_EQUAL_STRING("unknown", name);
}

void test_interval_name_invalid_b_returns_unknown(void)
{
    const char *name = audio_interval_name(0, 10);
    TEST_ASSERT_EQUAL_STRING("unknown", name);
}

/* ===== audio_planet_amplitude ===== */

void test_amplitude_sun_day(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.3f, (float)audio_planet_amplitude(0));
}

void test_amplitude_mercury(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.15f, (float)audio_planet_amplitude(1));
}

void test_amplitude_venus(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.25f, (float)audio_planet_amplitude(2));
}

void test_amplitude_earth_year(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.2f, (float)audio_planet_amplitude(3));
}

void test_amplitude_mars(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.2f, (float)audio_planet_amplitude(4));
}

void test_amplitude_jupiter(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.3f, (float)audio_planet_amplitude(5));
}

void test_amplitude_saturn(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.25f, (float)audio_planet_amplitude(6));
}

void test_amplitude_uranus(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.1f, (float)audio_planet_amplitude(7));
}

void test_amplitude_neptune(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.08f, (float)audio_planet_amplitude(8));
}

void test_amplitude_moon(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.35f, (float)audio_planet_amplitude(9));
}

void test_amplitude_invalid_returns_zero(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)audio_planet_amplitude(-1));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)audio_planet_amplitude(10));
}

/* ===== audio_aspect_from_angle ===== */

void test_aspect_from_angle_conjunction(void)
{
    TEST_ASSERT_EQUAL_INT(AUDIO_ASPECT_CONJUNCTION, audio_aspect_from_angle(0.0, 8.0));
}

void test_aspect_from_angle_sextile(void)
{
    TEST_ASSERT_EQUAL_INT(AUDIO_ASPECT_SEXTILE, audio_aspect_from_angle(60.0, 6.0));
}

void test_aspect_from_angle_square(void)
{
    TEST_ASSERT_EQUAL_INT(AUDIO_ASPECT_SQUARE, audio_aspect_from_angle(90.0, 8.0));
}

void test_aspect_from_angle_trine(void)
{
    TEST_ASSERT_EQUAL_INT(AUDIO_ASPECT_TRINE, audio_aspect_from_angle(120.0, 8.0));
}

void test_aspect_from_angle_opposition(void)
{
    TEST_ASSERT_EQUAL_INT(AUDIO_ASPECT_OPPOSITION, audio_aspect_from_angle(180.0, 8.0));
}

void test_aspect_from_angle_within_orb(void)
{
    /* 62 degrees with orb 6 -> sextile (60 +/- 6) */
    TEST_ASSERT_EQUAL_INT(AUDIO_ASPECT_SEXTILE, audio_aspect_from_angle(62.0, 6.0));
}

void test_aspect_from_angle_outside_orb(void)
{
    /* 70 degrees with orb 6 -> no aspect */
    TEST_ASSERT_EQUAL_INT(-1, audio_aspect_from_angle(70.0, 6.0));
}

void test_aspect_from_angle_no_match(void)
{
    /* 45 degrees -> no major aspect */
    TEST_ASSERT_EQUAL_INT(-1, audio_aspect_from_angle(45.0, 5.0));
}

void test_aspect_from_angle_near_conjunction_orb(void)
{
    /* 7 degrees with orb 8 -> conjunction */
    TEST_ASSERT_EQUAL_INT(AUDIO_ASPECT_CONJUNCTION, audio_aspect_from_angle(7.0, 8.0));
}

void test_aspect_from_angle_exact_orb_boundary(void)
{
    /* Exactly at orb boundary: 68.0 with orb 8.0 for sextile (60 + 8 = 68) */
    TEST_ASSERT_EQUAL_INT(AUDIO_ASPECT_SEXTILE, audio_aspect_from_angle(68.0, 8.0));
}

/* ===== audio_aspect_count ===== */

void test_aspect_count(void)
{
    TEST_ASSERT_EQUAL_INT(AUDIO_ASPECT_COUNT, audio_aspect_count());
}

/* ===== audio_waveform_name ===== */

void test_waveform_name_sine(void)
{
    TEST_ASSERT_EQUAL_STRING("sine", audio_waveform_name(AUDIO_WAVE_SINE));
}

void test_waveform_name_triangle(void)
{
    TEST_ASSERT_EQUAL_STRING("triangle", audio_waveform_name(AUDIO_WAVE_TRIANGLE));
}

void test_waveform_name_sawtooth(void)
{
    TEST_ASSERT_EQUAL_STRING("sawtooth", audio_waveform_name(AUDIO_WAVE_SAWTOOTH));
}

void test_waveform_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("unknown", audio_waveform_name(AUDIO_WAVE_COUNT));
}

/* ===== Kepler voice assignments ===== */

void test_kepler_voice_mercury_soprano(void)
{
    TEST_ASSERT_EQUAL_INT(KEPLER_VOICE_SOPRANO, audio_kepler_voice(1));
}

void test_kepler_voice_venus_alto(void)
{
    TEST_ASSERT_EQUAL_INT(KEPLER_VOICE_ALTO, audio_kepler_voice(2));
}

void test_kepler_voice_earth_alto(void)
{
    TEST_ASSERT_EQUAL_INT(KEPLER_VOICE_ALTO, audio_kepler_voice(3));
}

void test_kepler_voice_mars_tenor(void)
{
    TEST_ASSERT_EQUAL_INT(KEPLER_VOICE_TENOR, audio_kepler_voice(4));
}

void test_kepler_voice_jupiter_bass(void)
{
    TEST_ASSERT_EQUAL_INT(KEPLER_VOICE_BASS, audio_kepler_voice(5));
}

void test_kepler_voice_saturn_bass(void)
{
    TEST_ASSERT_EQUAL_INT(KEPLER_VOICE_BASS, audio_kepler_voice(6));
}

void test_kepler_voice_uranus_unassigned(void)
{
    TEST_ASSERT_EQUAL_INT(KEPLER_VOICE_COUNT, audio_kepler_voice(7));
}

void test_kepler_voice_neptune_unassigned(void)
{
    TEST_ASSERT_EQUAL_INT(KEPLER_VOICE_COUNT, audio_kepler_voice(8));
}

void test_kepler_voice_earth_day_unassigned(void)
{
    TEST_ASSERT_EQUAL_INT(KEPLER_VOICE_COUNT, audio_kepler_voice(0));
}

void test_kepler_voice_moon_unassigned(void)
{
    TEST_ASSERT_EQUAL_INT(KEPLER_VOICE_COUNT, audio_kepler_voice(9));
}

void test_kepler_voice_invalid_negative(void)
{
    TEST_ASSERT_EQUAL_INT(KEPLER_VOICE_COUNT, audio_kepler_voice(-1));
}

void test_kepler_voice_invalid_high(void)
{
    TEST_ASSERT_EQUAL_INT(KEPLER_VOICE_COUNT, audio_kepler_voice(99));
}

void test_kepler_voice_name_bass(void)
{
    TEST_ASSERT_EQUAL_STRING("bass", audio_kepler_voice_name(KEPLER_VOICE_BASS));
}

void test_kepler_voice_name_tenor(void)
{
    TEST_ASSERT_EQUAL_STRING("tenor", audio_kepler_voice_name(KEPLER_VOICE_TENOR));
}

void test_kepler_voice_name_alto(void)
{
    TEST_ASSERT_EQUAL_STRING("alto", audio_kepler_voice_name(KEPLER_VOICE_ALTO));
}

void test_kepler_voice_name_soprano(void)
{
    TEST_ASSERT_EQUAL_STRING("soprano", audio_kepler_voice_name(KEPLER_VOICE_SOPRANO));
}

void test_kepler_voice_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("unassigned", audio_kepler_voice_name(KEPLER_VOICE_COUNT));
}

/* ===== Just intonation ratios ===== */

void test_just_ratio_unison(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.0f, (float)audio_just_ratio(AUDIO_JI_UNISON));
}

void test_just_ratio_octave(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 2.0f, (float)audio_just_ratio(AUDIO_JI_OCTAVE));
}

void test_just_ratio_perfect_fifth(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.5f, (float)audio_just_ratio(AUDIO_JI_PERFECT_FIFTH));
}

void test_just_ratio_perfect_fourth(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)(4.0 / 3.0),
                             (float)audio_just_ratio(AUDIO_JI_PERFECT_FOURTH));
}

void test_just_ratio_major_third(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.25f, (float)audio_just_ratio(AUDIO_JI_MAJOR_THIRD));
}

void test_just_ratio_minor_third(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.2f, (float)audio_just_ratio(AUDIO_JI_MINOR_THIRD));
}

void test_just_ratio_tritone(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)(45.0 / 32.0),
                             (float)audio_just_ratio(AUDIO_JI_TRITONE));
}

void test_just_ratio_minor_second(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)(16.0 / 15.0),
                             (float)audio_just_ratio(AUDIO_JI_MINOR_SECOND));
}

void test_just_ratio_major_second(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.125f, (float)audio_just_ratio(AUDIO_JI_MAJOR_SECOND));
}

void test_just_ratio_invalid(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, (float)audio_just_ratio(AUDIO_JI_COUNT));
}

void test_just_ratio_invalid_negative(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, (float)audio_just_ratio(-1));
}

void test_just_ratio_ascending_order(void)
{
    /* Each interval ratio should be >= the previous one */
    for (int i = 1; i < AUDIO_JI_COUNT; i++) {
        TEST_ASSERT_TRUE(audio_just_ratio(i) >= audio_just_ratio(i - 1));
    }
}

void test_just_interval_name_unison(void)
{
    TEST_ASSERT_EQUAL_STRING("unison", audio_just_interval_name(AUDIO_JI_UNISON));
}

void test_just_interval_name_fifth(void)
{
    TEST_ASSERT_EQUAL_STRING("perfect fifth", audio_just_interval_name(AUDIO_JI_PERFECT_FIFTH));
}

void test_just_interval_name_octave(void)
{
    TEST_ASSERT_EQUAL_STRING("octave", audio_just_interval_name(AUDIO_JI_OCTAVE));
}

void test_just_interval_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("unknown", audio_just_interval_name(AUDIO_JI_COUNT));
}

void test_just_interval_count(void)
{
    TEST_ASSERT_EQUAL_INT(13, audio_just_interval_count());
}

/* ===== main ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Planet profiles */
    RUN_TEST(test_profile_earth_day_index);
    RUN_TEST(test_profile_earth_day_harmonics_count);
    RUN_TEST(test_profile_earth_day_waveform_sine);
    RUN_TEST(test_profile_earth_day_amplitude);
    RUN_TEST(test_profile_mercury_harmonics_count);
    RUN_TEST(test_profile_mercury_waveform_sawtooth);
    RUN_TEST(test_profile_mercury_amplitude);
    RUN_TEST(test_profile_venus_harmonics_count);
    RUN_TEST(test_profile_venus_waveform_sine);
    RUN_TEST(test_profile_venus_amplitude);
    RUN_TEST(test_profile_earth_year_harmonics_count);
    RUN_TEST(test_profile_earth_year_waveform_sine);
    RUN_TEST(test_profile_earth_year_amplitude);
    RUN_TEST(test_profile_mars_harmonics_count);
    RUN_TEST(test_profile_mars_waveform_triangle);
    RUN_TEST(test_profile_mars_amplitude);
    RUN_TEST(test_profile_jupiter_harmonics_count);
    RUN_TEST(test_profile_jupiter_waveform_triangle);
    RUN_TEST(test_profile_jupiter_amplitude);
    RUN_TEST(test_profile_saturn_harmonics_count);
    RUN_TEST(test_profile_saturn_waveform_triangle);
    RUN_TEST(test_profile_saturn_amplitude);
    RUN_TEST(test_profile_uranus_harmonics_count);
    RUN_TEST(test_profile_uranus_waveform_sine);
    RUN_TEST(test_profile_uranus_amplitude);
    RUN_TEST(test_profile_neptune_harmonics_count);
    RUN_TEST(test_profile_neptune_waveform_sine);
    RUN_TEST(test_profile_neptune_amplitude);
    RUN_TEST(test_profile_moon_harmonics_count);
    RUN_TEST(test_profile_moon_waveform_triangle);
    RUN_TEST(test_profile_moon_amplitude);
    RUN_TEST(test_profile_invalid_negative);
    RUN_TEST(test_profile_invalid_too_high);
    RUN_TEST(test_profile_fundamental_positive);
    RUN_TEST(test_profile_fundamental_matches_freq_planet);

    /* Planet freq */
    RUN_TEST(test_planet_freq_all_positive);
    RUN_TEST(test_planet_freq_audible_range);
    RUN_TEST(test_planet_freq_invalid_returns_zero);

    /* Harmonics */
    RUN_TEST(test_harmonics_mercury_six);
    RUN_TEST(test_harmonics_fundamental_is_one);
    RUN_TEST(test_harmonics_fundamental_amplitude_is_one);
    RUN_TEST(test_harmonics_overtone_freq_ratio);
    RUN_TEST(test_harmonics_amplitude_decreasing);
    RUN_TEST(test_harmonics_limited_by_max);
    RUN_TEST(test_harmonics_invalid_planet);
    RUN_TEST(test_harmonics_null_output);
    RUN_TEST(test_harmonics_mercury_eccentricity_formula);

    /* Aspect map */
    RUN_TEST(test_aspect_conjunction_angle);
    RUN_TEST(test_aspect_conjunction_consonance);
    RUN_TEST(test_aspect_conjunction_volume);
    RUN_TEST(test_aspect_conjunction_interval);
    RUN_TEST(test_aspect_conjunction_name);
    RUN_TEST(test_aspect_sextile_angle);
    RUN_TEST(test_aspect_sextile_consonance);
    RUN_TEST(test_aspect_sextile_volume);
    RUN_TEST(test_aspect_square_angle);
    RUN_TEST(test_aspect_square_consonance);
    RUN_TEST(test_aspect_square_volume);
    RUN_TEST(test_aspect_trine_angle);
    RUN_TEST(test_aspect_trine_consonance);
    RUN_TEST(test_aspect_trine_volume);
    RUN_TEST(test_aspect_opposition_angle);
    RUN_TEST(test_aspect_opposition_consonance);
    RUN_TEST(test_aspect_opposition_volume);
    RUN_TEST(test_aspect_opposition_interval);

    /* Aspect harmony */
    RUN_TEST(test_aspect_harmony_conjunction);
    RUN_TEST(test_aspect_harmony_sextile);
    RUN_TEST(test_aspect_harmony_square);
    RUN_TEST(test_aspect_harmony_trine);
    RUN_TEST(test_aspect_harmony_opposition);

    /* Aspect consonance */
    RUN_TEST(test_consonance_conjunction);
    RUN_TEST(test_consonance_square_lowest);
    RUN_TEST(test_consonance_invalid_returns_zero);

    /* Moon envelope */
    RUN_TEST(test_moon_envelope_new_moon);
    RUN_TEST(test_moon_envelope_full_moon);
    RUN_TEST(test_moon_envelope_next_new);
    RUN_TEST(test_moon_envelope_first_quarter);
    RUN_TEST(test_moon_envelope_last_quarter);
    RUN_TEST(test_moon_envelope_waxing_crescent);
    RUN_TEST(test_moon_envelope_waxing_gibbous);
    RUN_TEST(test_moon_envelope_waning_gibbous);
    RUN_TEST(test_moon_envelope_waning_crescent);
    RUN_TEST(test_moon_envelope_symmetric);
    RUN_TEST(test_moon_envelope_range);
    RUN_TEST(test_moon_envelope_clamp_negative);
    RUN_TEST(test_moon_envelope_clamp_above_one);

    /* Interval name */
    RUN_TEST(test_interval_name_same_planet);
    RUN_TEST(test_interval_name_returns_string);
    RUN_TEST(test_interval_name_invalid_returns_unknown);
    RUN_TEST(test_interval_name_invalid_b_returns_unknown);

    /* Planet amplitude */
    RUN_TEST(test_amplitude_sun_day);
    RUN_TEST(test_amplitude_mercury);
    RUN_TEST(test_amplitude_venus);
    RUN_TEST(test_amplitude_earth_year);
    RUN_TEST(test_amplitude_mars);
    RUN_TEST(test_amplitude_jupiter);
    RUN_TEST(test_amplitude_saturn);
    RUN_TEST(test_amplitude_uranus);
    RUN_TEST(test_amplitude_neptune);
    RUN_TEST(test_amplitude_moon);
    RUN_TEST(test_amplitude_invalid_returns_zero);

    /* Aspect from angle */
    RUN_TEST(test_aspect_from_angle_conjunction);
    RUN_TEST(test_aspect_from_angle_sextile);
    RUN_TEST(test_aspect_from_angle_square);
    RUN_TEST(test_aspect_from_angle_trine);
    RUN_TEST(test_aspect_from_angle_opposition);
    RUN_TEST(test_aspect_from_angle_within_orb);
    RUN_TEST(test_aspect_from_angle_outside_orb);
    RUN_TEST(test_aspect_from_angle_no_match);
    RUN_TEST(test_aspect_from_angle_near_conjunction_orb);
    RUN_TEST(test_aspect_from_angle_exact_orb_boundary);

    /* Aspect count */
    RUN_TEST(test_aspect_count);

    /* Waveform name */
    RUN_TEST(test_waveform_name_sine);
    RUN_TEST(test_waveform_name_triangle);
    RUN_TEST(test_waveform_name_sawtooth);
    RUN_TEST(test_waveform_name_invalid);

    /* Kepler voice assignments */
    RUN_TEST(test_kepler_voice_mercury_soprano);
    RUN_TEST(test_kepler_voice_venus_alto);
    RUN_TEST(test_kepler_voice_earth_alto);
    RUN_TEST(test_kepler_voice_mars_tenor);
    RUN_TEST(test_kepler_voice_jupiter_bass);
    RUN_TEST(test_kepler_voice_saturn_bass);
    RUN_TEST(test_kepler_voice_uranus_unassigned);
    RUN_TEST(test_kepler_voice_neptune_unassigned);
    RUN_TEST(test_kepler_voice_earth_day_unassigned);
    RUN_TEST(test_kepler_voice_moon_unassigned);
    RUN_TEST(test_kepler_voice_invalid_negative);
    RUN_TEST(test_kepler_voice_invalid_high);
    RUN_TEST(test_kepler_voice_name_bass);
    RUN_TEST(test_kepler_voice_name_tenor);
    RUN_TEST(test_kepler_voice_name_alto);
    RUN_TEST(test_kepler_voice_name_soprano);
    RUN_TEST(test_kepler_voice_name_invalid);

    /* Just intonation ratios */
    RUN_TEST(test_just_ratio_unison);
    RUN_TEST(test_just_ratio_octave);
    RUN_TEST(test_just_ratio_perfect_fifth);
    RUN_TEST(test_just_ratio_perfect_fourth);
    RUN_TEST(test_just_ratio_major_third);
    RUN_TEST(test_just_ratio_minor_third);
    RUN_TEST(test_just_ratio_tritone);
    RUN_TEST(test_just_ratio_minor_second);
    RUN_TEST(test_just_ratio_major_second);
    RUN_TEST(test_just_ratio_invalid);
    RUN_TEST(test_just_ratio_invalid_negative);
    RUN_TEST(test_just_ratio_ascending_order);
    RUN_TEST(test_just_interval_name_unison);
    RUN_TEST(test_just_interval_name_fifth);
    RUN_TEST(test_just_interval_name_octave);
    RUN_TEST(test_just_interval_name_invalid);
    RUN_TEST(test_just_interval_count);

    return UNITY_END();
}
