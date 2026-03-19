/* test_audio_a11y.c -- Tests for audio accessibility narrative module. */

#include "unity.h"
#include "ui/audio_a11y.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* --- aa_tension_level tests --- */

void test_aa_tension_minimal(void)
{
    TEST_ASSERT_EQUAL_STRING("minimal", aa_tension_level(0.05f));
}

void test_aa_tension_low(void)
{
    TEST_ASSERT_EQUAL_STRING("low", aa_tension_level(0.2f));
}

void test_aa_tension_moderate(void)
{
    TEST_ASSERT_EQUAL_STRING("moderate", aa_tension_level(0.45f));
}

void test_aa_tension_high(void)
{
    TEST_ASSERT_EQUAL_STRING("high", aa_tension_level(0.7f));
}

void test_aa_tension_extreme(void)
{
    TEST_ASSERT_EQUAL_STRING("extreme", aa_tension_level(0.95f));
}

/* --- aa_consonance_level tests --- */

void test_aa_consonance_dissonant(void)
{
    TEST_ASSERT_EQUAL_STRING("dissonant", aa_consonance_level(0.1f));
}

void test_aa_consonance_neutral(void)
{
    TEST_ASSERT_EQUAL_STRING("neutral", aa_consonance_level(0.45f));
}

void test_aa_consonance_resolved(void)
{
    TEST_ASSERT_EQUAL_STRING("resolved", aa_consonance_level(0.9f));
}

/* --- aa_moon_phase_word tests --- */

void test_aa_moon_new(void)
{
    TEST_ASSERT_EQUAL_STRING("new", aa_moon_phase_word(0.1f));
}

void test_aa_moon_full(void)
{
    TEST_ASSERT_EQUAL_STRING("full", aa_moon_phase_word(0.95f));
}

void test_aa_moon_half(void)
{
    TEST_ASSERT_EQUAL_STRING("half", aa_moon_phase_word(0.5f));
}

/* --- aa_chord_desc tests --- */

void test_aa_chord_desc_basic(void)
{
    ps_score_t score;
    memset(&score, 0, sizeof(score));
    score.note_count = 2;
    strncpy(score.notes[0].name, "A4", PS_NOTE_BUF);
    score.notes[0].planet = "Mercury";
    strncpy(score.notes[1].name, "C5", PS_NOTE_BUF);
    score.notes[1].planet = "Venus";

    char buf[AA_MAX_LEN];
    int n = aa_chord_desc(&score, buf, AA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "A4"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Mercury"));
}

void test_aa_chord_desc_null(void)
{
    char buf[AA_MAX_LEN];
    int n = aa_chord_desc(NULL, buf, AA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Silence"));
}

/* --- aa_mood_desc tests --- */

void test_aa_mood_desc_basic(void)
{
    ps_score_t score;
    memset(&score, 0, sizeof(score));
    score.mood = "Earth Breath";
    score.dynamics = "mf";
    score.tempo_mark = "Andante";
    score.tempo_bpm = 80.0f;

    char buf[AA_MAX_LEN];
    int n = aa_mood_desc(&score, buf, AA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Earth Breath"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "mf"));
}

/* --- aa_tension_desc tests --- */

void test_aa_tension_desc_low_consonant(void)
{
    char buf[AA_MAX_LEN];
    int n = aa_tension_desc(0.1f, 0.9f, buf, AA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "resolved"));
}

void test_aa_tension_desc_high_dissonant(void)
{
    char buf[AA_MAX_LEN];
    int n = aa_tension_desc(0.7f, 0.1f, buf, AA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "high"));
}

/* --- aa_moon_desc tests --- */

void test_aa_moon_desc_full(void)
{
    char buf[AA_MAX_LEN];
    int n = aa_moon_desc(0.95f, buf, AA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "full"));
}

void test_aa_moon_desc_new(void)
{
    char buf[AA_MAX_LEN];
    int n = aa_moon_desc(0.1f, buf, AA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "new"));
}

/* --- aa_meditation_desc tests --- */

void test_aa_meditation_active(void)
{
    char buf[AA_MAX_LEN];
    int n = aa_meditation_desc(1, 1, 4.236f, buf, AA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "active"));
}

void test_aa_meditation_inactive(void)
{
    char buf[AA_MAX_LEN];
    int n = aa_meditation_desc(0, 0, 0.0f, buf, AA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "inactive"));
}

/* --- aa_change_desc tests --- */

void test_aa_change_desc_null_prev(void)
{
    audio_params_t curr;
    memset(&curr, 0, sizeof(curr));
    curr.tension = 0.3f;
    curr.consonance = 0.7f;

    char buf[AA_MAX_LEN];
    int n = aa_change_desc(NULL, &curr, buf, AA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
}

void test_aa_change_desc_tension_increase(void)
{
    audio_params_t prev, curr;
    memset(&prev, 0, sizeof(prev));
    memset(&curr, 0, sizeof(curr));
    prev.tension = 0.2f;
    curr.tension = 0.7f;

    char buf[AA_MAX_LEN];
    int n = aa_change_desc(&prev, &curr, buf, AA_MAX_LEN);
    TEST_ASSERT_TRUE(n > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "increasing"));
}

int main(void)
{
    UNITY_BEGIN();

    /* aa_tension_level */
    RUN_TEST(test_aa_tension_minimal);
    RUN_TEST(test_aa_tension_low);
    RUN_TEST(test_aa_tension_moderate);
    RUN_TEST(test_aa_tension_high);
    RUN_TEST(test_aa_tension_extreme);

    /* aa_consonance_level */
    RUN_TEST(test_aa_consonance_dissonant);
    RUN_TEST(test_aa_consonance_neutral);
    RUN_TEST(test_aa_consonance_resolved);

    /* aa_moon_phase_word */
    RUN_TEST(test_aa_moon_new);
    RUN_TEST(test_aa_moon_full);
    RUN_TEST(test_aa_moon_half);

    /* aa_chord_desc */
    RUN_TEST(test_aa_chord_desc_basic);
    RUN_TEST(test_aa_chord_desc_null);

    /* aa_mood_desc */
    RUN_TEST(test_aa_mood_desc_basic);

    /* aa_tension_desc */
    RUN_TEST(test_aa_tension_desc_low_consonant);
    RUN_TEST(test_aa_tension_desc_high_dissonant);

    /* aa_moon_desc */
    RUN_TEST(test_aa_moon_desc_full);
    RUN_TEST(test_aa_moon_desc_new);

    /* aa_meditation_desc */
    RUN_TEST(test_aa_meditation_active);
    RUN_TEST(test_aa_meditation_inactive);

    /* aa_change_desc */
    RUN_TEST(test_aa_change_desc_null_prev);
    RUN_TEST(test_aa_change_desc_tension_increase);

    return UNITY_END();
}
