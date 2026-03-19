/* test_audio_culture.c -- Tests for cultural audio timbres.
 *
 * Validates harmonic partial arrays, base frequencies, amplitude ranges,
 * and per-system lookups for the audio_culture pure data module. */

#include "../unity/unity.h"
#include "../../src/ui/audio_culture.h"
#include <string.h>
#include <math.h>

void setUp(void) {}
void tearDown(void) {}

/* ---- Get tests ---- */

void test_get_hebrew(void) {
    audio_culture_t c = audio_culture_get(CD_SYS_HEBREW);
    TEST_ASSERT_EQUAL_INT(CD_SYS_HEBREW, c.system);
    TEST_ASSERT_EQUAL_STRING("Hebrew", c.name);
}

void test_get_islamic(void) {
    audio_culture_t c = audio_culture_get(CD_SYS_ISLAMIC);
    TEST_ASSERT_EQUAL_STRING("Islamic", c.name);
    TEST_ASSERT_EQUAL_STRING("oud strings", c.timbre_desc);
}

void test_get_buddhist(void) {
    audio_culture_t c = audio_culture_get(CD_SYS_BUDDHIST);
    TEST_ASSERT_EQUAL_STRING("Buddhist", c.name);
    TEST_ASSERT_EQUAL_STRING("singing bowl", c.timbre_desc);
}

void test_get_hindu(void) {
    audio_culture_t c = audio_culture_get(CD_SYS_HINDU);
    TEST_ASSERT_EQUAL_STRING("Hindu", c.name);
}

void test_get_chinese(void) {
    audio_culture_t c = audio_culture_get(CD_SYS_CHINESE);
    TEST_ASSERT_EQUAL_STRING("Chinese", c.name);
}

void test_get_tzolkin(void) {
    audio_culture_t c = audio_culture_get(CD_SYS_TZOLKIN);
    TEST_ASSERT_EQUAL_STRING("Tzolkin", c.name);
    TEST_ASSERT_EQUAL_STRING("wooden flute", c.timbre_desc);
}

void test_get_persian(void) {
    audio_culture_t c = audio_culture_get(CD_SYS_PERSIAN);
    TEST_ASSERT_EQUAL_STRING("Persian", c.name);
    TEST_ASSERT_EQUAL_STRING("tar strings", c.timbre_desc);
}

void test_get_celtic(void) {
    audio_culture_t c = audio_culture_get(CD_SYS_CELTIC);
    TEST_ASSERT_EQUAL_STRING("Celtic", c.name);
    TEST_ASSERT_EQUAL_STRING("harp shimmer", c.timbre_desc);
}

void test_get_japanese(void) {
    audio_culture_t c = audio_culture_get(CD_SYS_JAPANESE);
    TEST_ASSERT_EQUAL_STRING("Japanese", c.name);
    TEST_ASSERT_EQUAL_STRING("koto pluck", c.timbre_desc);
}

void test_get_astronomy(void) {
    audio_culture_t c = audio_culture_get(CD_SYS_ASTRONOMY);
    TEST_ASSERT_EQUAL_STRING("Astronomy", c.name);
    TEST_ASSERT_EQUAL_STRING("pure tone", c.timbre_desc);
}

void test_get_unsupported(void) {
    audio_culture_t c = audio_culture_get(CD_SYS_BAHAI);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, c.base_amplitude);
    TEST_ASSERT_EQUAL_INT(0, c.partial_count);
    TEST_ASSERT_EQUAL_STRING("?", c.name);
}

/* ---- Partial validation ---- */

void test_partials_fundamental_is_one(void) {
    /* All supported timbres have fundamental = 1.0 */
    for (int i = 0; i < (int)CD_SYS_COUNT; i++) {
        if (audio_culture_has_timbre((cd_system_t)i)) {
            audio_culture_t c = audio_culture_get((cd_system_t)i);
            TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, c.partials[0]);
        }
    }
}

void test_partials_in_range(void) {
    /* All partials should be in [0, 1] */
    for (int s = 0; s < (int)CD_SYS_COUNT; s++) {
        audio_culture_t c = audio_culture_get((cd_system_t)s);
        for (int p = 0; p < AC_MAX_PARTIALS; p++) {
            TEST_ASSERT_TRUE(c.partials[p] >= 0.0f);
            TEST_ASSERT_TRUE(c.partials[p] <= 1.0f);
        }
    }
}

void test_hebrew_brass_odd_harmonics(void) {
    /* Shofar (brass): odd harmonics stronger than even */
    audio_culture_t c = audio_culture_get(CD_SYS_HEBREW);
    /* partial[0]=1.0 (1st), [1]=0.0 (2nd even), [2]=0.7 (3rd odd) */
    TEST_ASSERT_TRUE(c.partials[2] > c.partials[1]); /* 3rd > 2nd */
}

void test_tzolkin_flute_few_partials(void) {
    /* Wooden flute: very few partials (mostly fundamental) */
    audio_culture_t c = audio_culture_get(CD_SYS_TZOLKIN);
    TEST_ASSERT_EQUAL_INT(3, c.partial_count);
    TEST_ASSERT_TRUE(c.partials[1] < 0.2f); /* weak 2nd harmonic */
}

void test_hindu_sitar_rich_harmonics(void) {
    /* Sitar: many harmonics, slow decay */
    audio_culture_t c = audio_culture_get(CD_SYS_HINDU);
    TEST_ASSERT_EQUAL_INT(8, c.partial_count);
    TEST_ASSERT_TRUE(c.partials[7] > 0.2f); /* still audible at 8th */
}

/* ---- Base frequency tests ---- */

void test_base_freq_positive(void) {
    for (int s = 0; s < (int)CD_SYS_COUNT; s++) {
        if (audio_culture_has_timbre((cd_system_t)s)) {
            audio_culture_t c = audio_culture_get((cd_system_t)s);
            TEST_ASSERT_TRUE(c.base_freq_hz > 0.0f);
        }
    }
}

void test_base_amplitude_range(void) {
    for (int s = 0; s < (int)CD_SYS_COUNT; s++) {
        if (audio_culture_has_timbre((cd_system_t)s)) {
            audio_culture_t c = audio_culture_get((cd_system_t)s);
            TEST_ASSERT_TRUE(c.base_amplitude > 0.0f);
            TEST_ASSERT_TRUE(c.base_amplitude <= 1.0f);
        }
    }
}

void test_unsupported_base_freq_zero(void) {
    audio_culture_t c = audio_culture_get(CD_SYS_BAHAI);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, c.base_freq_hz);
}

/* ---- Name/desc tests ---- */

void test_name_hebrew(void) {
    TEST_ASSERT_EQUAL_STRING("Hebrew", audio_culture_name(CD_SYS_HEBREW));
}

void test_name_unsupported(void) {
    TEST_ASSERT_EQUAL_STRING("?", audio_culture_name(CD_SYS_BAHAI));
}

void test_desc_buddhist(void) {
    TEST_ASSERT_EQUAL_STRING("singing bowl", audio_culture_desc(CD_SYS_BUDDHIST));
}

void test_desc_unsupported(void) {
    TEST_ASSERT_EQUAL_STRING("?", audio_culture_desc(CD_SYS_BAHAI));
}

/* ---- Count and has_timbre ---- */

void test_count(void) {
    TEST_ASSERT_EQUAL_INT(10, audio_culture_count());
}

void test_has_timbre_hebrew(void) {
    TEST_ASSERT_EQUAL_INT(1, audio_culture_has_timbre(CD_SYS_HEBREW));
}

void test_has_timbre_unsupported(void) {
    TEST_ASSERT_EQUAL_INT(0, audio_culture_has_timbre(CD_SYS_BAHAI));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_get_hebrew);
    RUN_TEST(test_get_islamic);
    RUN_TEST(test_get_buddhist);
    RUN_TEST(test_get_hindu);
    RUN_TEST(test_get_chinese);
    RUN_TEST(test_get_tzolkin);
    RUN_TEST(test_get_persian);
    RUN_TEST(test_get_celtic);
    RUN_TEST(test_get_japanese);
    RUN_TEST(test_get_astronomy);
    RUN_TEST(test_get_unsupported);
    RUN_TEST(test_partials_fundamental_is_one);
    RUN_TEST(test_partials_in_range);
    RUN_TEST(test_hebrew_brass_odd_harmonics);
    RUN_TEST(test_tzolkin_flute_few_partials);
    RUN_TEST(test_hindu_sitar_rich_harmonics);
    RUN_TEST(test_base_freq_positive);
    RUN_TEST(test_base_amplitude_range);
    RUN_TEST(test_unsupported_base_freq_zero);
    RUN_TEST(test_name_hebrew);
    RUN_TEST(test_name_unsupported);
    RUN_TEST(test_desc_buddhist);
    RUN_TEST(test_desc_unsupported);
    RUN_TEST(test_count);
    RUN_TEST(test_has_timbre_hebrew);
    RUN_TEST(test_has_timbre_unsupported);
    return UNITY_END();
}
