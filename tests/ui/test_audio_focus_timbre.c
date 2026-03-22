/* test_audio_focus_timbre.c -- Tests for per-focus-mode sonic profiles.
 *
 * Validates that each focus mode (A/K/I/C/D/T) has a distinct sonic
 * identity with appropriate frequency, brightness, attack, and partials. */

#include "../unity/unity.h"
#include "../../src/ui/audio_focus_timbre.h"
#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- Retrieval tests ---- */

void test_get_astrology(void) {
    audio_focus_timbre_t t = audio_focus_timbre_get(1);
    TEST_ASSERT_EQUAL_INT(1, t.focus_mode);
    TEST_ASSERT_EQUAL_STRING("Astrology", t.name);
    TEST_ASSERT_EQUAL_STRING("crystalline glass", t.timbre_desc);
}

void test_get_kin(void) {
    audio_focus_timbre_t t = audio_focus_timbre_get(2);
    TEST_ASSERT_EQUAL_INT(2, t.focus_mode);
    TEST_ASSERT_EQUAL_STRING("Kin Maya", t.name);
    TEST_ASSERT_EQUAL_STRING("warm wooden flute", t.timbre_desc);
}

void test_get_iching(void) {
    audio_focus_timbre_t t = audio_focus_timbre_get(3);
    TEST_ASSERT_EQUAL_STRING("I Ching", t.name);
    TEST_ASSERT_EQUAL_STRING("deep bronze bell", t.timbre_desc);
}

void test_get_chinese(void) {
    audio_focus_timbre_t t = audio_focus_timbre_get(4);
    TEST_ASSERT_EQUAL_STRING("Chinese", t.name);
    TEST_ASSERT_EQUAL_STRING("guzheng pluck", t.timbre_desc);
}

void test_get_hd(void) {
    audio_focus_timbre_t t = audio_focus_timbre_get(5);
    TEST_ASSERT_EQUAL_INT(5, t.focus_mode);
    TEST_ASSERT_EQUAL_STRING("Human Design", t.name);
    TEST_ASSERT_EQUAL_STRING("electronic pulse", t.timbre_desc);
}

void test_get_kabbalah(void) {
    audio_focus_timbre_t t = audio_focus_timbre_get(6);
    TEST_ASSERT_EQUAL_INT(6, t.focus_mode);
    TEST_ASSERT_EQUAL_STRING("Kabbalah", t.name);
    TEST_ASSERT_EQUAL_STRING("sustained harmonic chord", t.timbre_desc);
}

void test_get_overview_returns_null(void) {
    audio_focus_timbre_t t = audio_focus_timbre_get(0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, t.base_amplitude);
    TEST_ASSERT_EQUAL_STRING("Overview", t.name);
}

void test_get_invalid_returns_null(void) {
    audio_focus_timbre_t t = audio_focus_timbre_get(99);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, t.base_amplitude);
}

/* ---- has() tests ---- */

void test_has_all_six(void) {
    for (int i = 1; i <= 6; i++) {
        TEST_ASSERT_TRUE_MESSAGE(audio_focus_timbre_has(i),
                                 "Expected timbre for focus mode");
    }
}

void test_has_not_overview(void) {
    TEST_ASSERT_FALSE(audio_focus_timbre_has(0));
}

void test_has_not_invalid(void) {
    TEST_ASSERT_FALSE(audio_focus_timbre_has(-1));
    TEST_ASSERT_FALSE(audio_focus_timbre_has(7));
}

/* ---- name() tests ---- */

void test_name_modes(void) {
    TEST_ASSERT_EQUAL_STRING("Overview", audio_focus_timbre_name(0));
    TEST_ASSERT_EQUAL_STRING("Astrology", audio_focus_timbre_name(1));
    TEST_ASSERT_EQUAL_STRING("Kin Maya", audio_focus_timbre_name(2));
    TEST_ASSERT_EQUAL_STRING("I Ching", audio_focus_timbre_name(3));
    TEST_ASSERT_EQUAL_STRING("Chinese", audio_focus_timbre_name(4));
    TEST_ASSERT_EQUAL_STRING("Human Design", audio_focus_timbre_name(5));
    TEST_ASSERT_EQUAL_STRING("Kabbalah", audio_focus_timbre_name(6));
}

/* ---- count() test ---- */

void test_count(void) {
    TEST_ASSERT_EQUAL_INT(6, audio_focus_timbre_count());
}

/* ---- Frequency range tests ---- */

void test_frequencies_in_audible_range(void) {
    for (int i = 1; i <= 6; i++) {
        audio_focus_timbre_t t = audio_focus_timbre_get(i);
        TEST_ASSERT_TRUE(t.base_freq_hz >= 100.0f);
        TEST_ASSERT_TRUE(t.base_freq_hz <= 500.0f);
    }
}

void test_frequencies_distinct(void) {
    float freqs[6];
    for (int i = 0; i < 6; i++) {
        freqs[i] = audio_focus_timbre_get(i + 1).base_freq_hz;
    }
    for (int i = 0; i < 6; i++) {
        for (int j = i + 1; j < 6; j++) {
            TEST_ASSERT_TRUE_MESSAGE(
                fabsf(freqs[i] - freqs[j]) > 5.0f,
                "Focus timbres should have distinct frequencies");
        }
    }
}

/* ---- Brightness range tests ---- */

void test_brightness_in_range(void) {
    for (int i = 1; i <= 6; i++) {
        audio_focus_timbre_t t = audio_focus_timbre_get(i);
        TEST_ASSERT_TRUE(t.brightness >= 0.0f);
        TEST_ASSERT_TRUE(t.brightness <= 1.0f);
    }
}

void test_astrology_brightest(void) {
    /* Astrology should be the brightest — crystalline, celestial */
    audio_focus_timbre_t astro = audio_focus_timbre_get(1);
    for (int i = 2; i <= 6; i++) {
        audio_focus_timbre_t other = audio_focus_timbre_get(i);
        TEST_ASSERT_TRUE_MESSAGE(
            astro.brightness >= other.brightness,
            "Astrology should be brightest timbre");
    }
}

void test_kin_darkest(void) {
    /* Kin Maya should be the darkest — warm, earthy, wooden flute */
    audio_focus_timbre_t kin = audio_focus_timbre_get(2);
    for (int i = 1; i <= 6; i++) {
        if (i == 2) continue;
        audio_focus_timbre_t other = audio_focus_timbre_get(i);
        TEST_ASSERT_TRUE_MESSAGE(
            kin.brightness <= other.brightness,
            "Kin Maya should be darkest timbre");
    }
}

/* ---- Attack time tests ---- */

void test_attack_in_range(void) {
    for (int i = 1; i <= 6; i++) {
        audio_focus_timbre_t t = audio_focus_timbre_get(i);
        TEST_ASSERT_TRUE(t.attack_s > 0.0f);
        TEST_ASSERT_TRUE(t.attack_s <= 1.0f);
    }
}

void test_bell_fastest_attack(void) {
    /* I Ching bell should have very fast attack (percussive) */
    audio_focus_timbre_t bell = audio_focus_timbre_get(3);
    TEST_ASSERT_TRUE(bell.attack_s <= 0.01f);
}

void test_kabbalah_slowest_attack(void) {
    /* Kabbalah sustained chord should have slowest attack */
    audio_focus_timbre_t kab = audio_focus_timbre_get(6);
    for (int i = 1; i <= 5; i++) {
        audio_focus_timbre_t other = audio_focus_timbre_get(i);
        TEST_ASSERT_TRUE_MESSAGE(
            kab.attack_s >= other.attack_s,
            "Kabbalah should have slowest attack");
    }
}

/* ---- Reverb boost tests ---- */

void test_reverb_boost_in_range(void) {
    for (int i = 1; i <= 6; i++) {
        audio_focus_timbre_t t = audio_focus_timbre_get(i);
        TEST_ASSERT_TRUE(t.reverb_boost >= 0.0f);
        TEST_ASSERT_TRUE(t.reverb_boost <= 0.30f);
    }
}

void test_kabbalah_most_reverb(void) {
    /* Kabbalah — stone temple, cavernous, should have most reverb */
    audio_focus_timbre_t kab = audio_focus_timbre_get(6);
    for (int i = 1; i <= 5; i++) {
        audio_focus_timbre_t other = audio_focus_timbre_get(i);
        TEST_ASSERT_TRUE_MESSAGE(
            kab.reverb_boost >= other.reverb_boost,
            "Kabbalah should have most reverb boost");
    }
}

void test_hd_least_reverb(void) {
    /* Human Design — dry, modern, precise */
    audio_focus_timbre_t hd = audio_focus_timbre_get(5);
    for (int i = 1; i <= 6; i++) {
        if (i == 5) continue;
        audio_focus_timbre_t other = audio_focus_timbre_get(i);
        TEST_ASSERT_TRUE_MESSAGE(
            hd.reverb_boost <= other.reverb_boost,
            "Human Design should have least reverb boost");
    }
}

/* ---- Partial count tests ---- */

void test_partial_counts_valid(void) {
    for (int i = 1; i <= 6; i++) {
        audio_focus_timbre_t t = audio_focus_timbre_get(i);
        TEST_ASSERT_TRUE(t.partial_count >= 1);
        TEST_ASSERT_TRUE(t.partial_count <= AFT_MAX_PARTIALS);
    }
}

void test_kabbalah_most_partials(void) {
    /* Full harmonic series — should use all 8 */
    audio_focus_timbre_t kab = audio_focus_timbre_get(6);
    TEST_ASSERT_EQUAL_INT(8, kab.partial_count);
}

void test_kin_fewest_partials(void) {
    /* Flute — almost pure tone */
    audio_focus_timbre_t kin = audio_focus_timbre_get(2);
    for (int i = 1; i <= 6; i++) {
        if (i == 2) continue;
        audio_focus_timbre_t other = audio_focus_timbre_get(i);
        TEST_ASSERT_TRUE_MESSAGE(
            kin.partial_count <= other.partial_count,
            "Kin Maya flute should have fewest partials");
    }
}

void test_fundamental_always_one(void) {
    for (int i = 1; i <= 6; i++) {
        audio_focus_timbre_t t = audio_focus_timbre_get(i);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, t.partials[0]);
    }
}

/* ---- apply() tests ---- */

void test_apply_valid_mode(void) {
    float freqs[9] = {0};
    float amps[9] = {0};
    int waves[9] = {0};
    int hcounts[9] = {0};
    float hamps[9][AFT_MAX_PARTIALS] = {{0}};
    float pans[9] = {0};

    int ok = audio_focus_timbre_apply(1, 8, freqs, amps, waves, hcounts,
                                      hamps, pans);
    TEST_ASSERT_EQUAL_INT(1, ok);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 392.0f, freqs[8]);
    TEST_ASSERT_TRUE(amps[8] > 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, pans[8]);
}

void test_apply_overview_noop(void) {
    float freqs[9] = {0};
    float amps[9] = {0};
    int waves[9] = {0};
    int hcounts[9] = {0};
    float hamps[9][AFT_MAX_PARTIALS] = {{0}};
    float pans[9] = {0};

    int ok = audio_focus_timbre_apply(0, 8, freqs, amps, waves, hcounts,
                                      hamps, pans);
    TEST_ASSERT_EQUAL_INT(0, ok);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, amps[8]);
}

void test_apply_null_safety(void) {
    int ok = audio_focus_timbre_apply(1, 0, NULL, NULL, NULL, NULL,
                                      NULL, NULL);
    TEST_ASSERT_EQUAL_INT(0, ok);
}

void test_apply_hd(void) {
    float freqs[9] = {0};
    float amps[9] = {0};
    int waves[9] = {0};
    int hcounts[9] = {0};
    float hamps[9][AFT_MAX_PARTIALS] = {{0}};
    float pans[9] = {0};

    int ok = audio_focus_timbre_apply(5, 8, freqs, amps, waves, hcounts,
                                      hamps, pans);
    TEST_ASSERT_EQUAL_INT(1, ok);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 220.0f, freqs[8]);
    TEST_ASSERT_EQUAL_INT(2, waves[8]); /* sawtooth */
}

void test_apply_kabbalah(void) {
    float freqs[9] = {0};
    float amps[9] = {0};
    int waves[9] = {0};
    int hcounts[9] = {0};
    float hamps[9][AFT_MAX_PARTIALS] = {{0}};
    float pans[9] = {0};

    int ok = audio_focus_timbre_apply(6, 8, freqs, amps, waves, hcounts,
                                      hamps, pans);
    TEST_ASSERT_EQUAL_INT(1, ok);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 196.0f, freqs[8]);
    TEST_ASSERT_EQUAL_INT(1, waves[8]); /* triangle */
    TEST_ASSERT_EQUAL_INT(8, hcounts[8]); /* full series */
}

/* ---- Sonic character distinctness ---- */

void test_each_mode_has_unique_character(void) {
    /* Each focus mode should differ in at least 2 of:
     * frequency, brightness, attack, reverb_boost */
    for (int i = 1; i <= 6; i++) {
        for (int j = i + 1; j <= 6; j++) {
            audio_focus_timbre_t a = audio_focus_timbre_get(i);
            audio_focus_timbre_t b = audio_focus_timbre_get(j);
            int diffs = 0;
            if (fabsf(a.base_freq_hz - b.base_freq_hz) > 10.0f) diffs++;
            if (fabsf(a.brightness - b.brightness) > 0.05f) diffs++;
            if (fabsf(a.attack_s - b.attack_s) > 0.002f) diffs++;
            if (fabsf(a.reverb_boost - b.reverb_boost) > 0.01f) diffs++;
            TEST_ASSERT_TRUE_MESSAGE(
                diffs >= 2,
                "Each pair of focus timbres should differ in 2+ dimensions");
        }
    }
}

/* ---- Runner ---- */

int main(void) {
    UNITY_BEGIN();

    /* Retrieval */
    RUN_TEST(test_get_astrology);
    RUN_TEST(test_get_kin);
    RUN_TEST(test_get_iching);
    RUN_TEST(test_get_chinese);
    RUN_TEST(test_get_hd);
    RUN_TEST(test_get_kabbalah);
    RUN_TEST(test_get_overview_returns_null);
    RUN_TEST(test_get_invalid_returns_null);

    /* has() */
    RUN_TEST(test_has_all_six);
    RUN_TEST(test_has_not_overview);
    RUN_TEST(test_has_not_invalid);

    /* name() */
    RUN_TEST(test_name_modes);

    /* count() */
    RUN_TEST(test_count);

    /* Frequency */
    RUN_TEST(test_frequencies_in_audible_range);
    RUN_TEST(test_frequencies_distinct);

    /* Brightness */
    RUN_TEST(test_brightness_in_range);
    RUN_TEST(test_astrology_brightest);
    RUN_TEST(test_kin_darkest);

    /* Attack */
    RUN_TEST(test_attack_in_range);
    RUN_TEST(test_bell_fastest_attack);
    RUN_TEST(test_kabbalah_slowest_attack);

    /* Reverb */
    RUN_TEST(test_reverb_boost_in_range);
    RUN_TEST(test_kabbalah_most_reverb);
    RUN_TEST(test_hd_least_reverb);

    /* Partials */
    RUN_TEST(test_partial_counts_valid);
    RUN_TEST(test_kabbalah_most_partials);
    RUN_TEST(test_kin_fewest_partials);
    RUN_TEST(test_fundamental_always_one);

    /* apply() */
    RUN_TEST(test_apply_valid_mode);
    RUN_TEST(test_apply_overview_noop);
    RUN_TEST(test_apply_null_safety);
    RUN_TEST(test_apply_hd);
    RUN_TEST(test_apply_kabbalah);

    /* Distinctness */
    RUN_TEST(test_each_mode_has_unique_character);

    return UNITY_END();
}
