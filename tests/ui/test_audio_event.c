/* test_audio_event.c -- Tests for audio event detection module.
 *
 * Validates aspect consonance scoring, convergence density,
 * combined event intensity, and full scan summaries. */

#include "../unity/unity.h"
#include "../../src/ui/audio_event.h"

void setUp(void) {}
void tearDown(void) {}

static const double JD_J2000 = 2451545.0;

/* ---- consonance tests ---- */

void test_consonance_in_range(void)
{
    float c = audio_event_consonance(JD_J2000, 8.0);
    TEST_ASSERT_TRUE(c >= 0.0f);
    TEST_ASSERT_TRUE(c <= 1.0f);
}

void test_consonance_zero_orb_neutral(void)
{
    float c = audio_event_consonance(JD_J2000, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.5f, c);
}

void test_consonance_negative_orb_neutral(void)
{
    float c = audio_event_consonance(JD_J2000, -5.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.5f, c);
}

void test_consonance_different_dates(void)
{
    float c1 = audio_event_consonance(JD_J2000, 8.0);
    float c2 = audio_event_consonance(JD_J2000 + 180.0, 8.0);
    TEST_ASSERT_TRUE(c1 >= 0.0f && c1 <= 1.0f);
    TEST_ASSERT_TRUE(c2 >= 0.0f && c2 <= 1.0f);
}

/* ---- density tests ---- */

void test_density_in_range(void)
{
    float d = audio_event_density(JD_J2000);
    TEST_ASSERT_TRUE(d >= 0.0f);
    TEST_ASSERT_TRUE(d <= 1.0f);
}

void test_density_different_dates(void)
{
    float d1 = audio_event_density(JD_J2000);
    float d2 = audio_event_density(JD_J2000 + 30.0);
    TEST_ASSERT_TRUE(d1 >= 0.0f && d1 <= 1.0f);
    TEST_ASSERT_TRUE(d2 >= 0.0f && d2 <= 1.0f);
}

/* ---- intensity tests ---- */

void test_intensity_in_range(void)
{
    float i = audio_event_intensity(JD_J2000, 8.0);
    TEST_ASSERT_TRUE(i >= 0.0f);
    TEST_ASSERT_TRUE(i <= 1.0f);
}

void test_intensity_blends_consonance_and_density(void)
{
    float c = audio_event_consonance(JD_J2000, 8.0);
    float d = audio_event_density(JD_J2000);
    float expected = 0.6f * c + 0.4f * d;
    float actual = audio_event_intensity(JD_J2000, 8.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, expected, actual);
}

void test_intensity_zero_orb_uses_neutral_consonance(void)
{
    float d = audio_event_density(JD_J2000);
    float expected = 0.6f * 0.5f + 0.4f * d;
    float actual = audio_event_intensity(JD_J2000, 0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, expected, actual);
}

/* ---- scan tests ---- */

void test_scan_summary_valid(void)
{
    audio_event_summary_t s = audio_event_scan(JD_J2000, 8.0);
    TEST_ASSERT_TRUE(s.aspect_count >= 0);
    TEST_ASSERT_TRUE(s.aspect_count <= AE_MAX_ACTIVE_ASPECTS);
    TEST_ASSERT_TRUE(s.consonance >= 0.0f && s.consonance <= 1.0f);
    TEST_ASSERT_TRUE(s.convergence_density >= 0.0f
                     && s.convergence_density <= 1.0f);
    TEST_ASSERT_TRUE(s.event_intensity >= 0.0f && s.event_intensity <= 1.0f);
    TEST_ASSERT_TRUE(s.convergence_count >= 0);
}

void test_scan_tightest_orb_valid(void)
{
    audio_event_summary_t s = audio_event_scan(JD_J2000, 8.0);
    if (s.aspect_count > 0) {
        TEST_ASSERT_TRUE(s.tightest_orb >= 0.0f);
        TEST_ASSERT_TRUE(s.tightest_orb <= 8.0f);
    } else {
        TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, s.tightest_orb);
    }
}

void test_scan_intensity_matches_standalone(void)
{
    audio_event_summary_t s = audio_event_scan(JD_J2000, 8.0);
    float standalone = audio_event_intensity(JD_J2000, 8.0);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, standalone, s.event_intensity);
}

void test_scan_consonance_matches_standalone(void)
{
    audio_event_summary_t s = audio_event_scan(JD_J2000, 8.0);
    float standalone = audio_event_consonance(JD_J2000, 8.0);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, standalone, s.consonance);
}

void test_scan_density_matches_standalone(void)
{
    audio_event_summary_t s = audio_event_scan(JD_J2000, 8.0);
    float standalone = audio_event_density(JD_J2000);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, standalone, s.convergence_density);
}

/* ---- wide date range test ---- */

void test_scan_across_dates(void)
{
    for (int i = 0; i < 10; i++) {
        double jd = JD_J2000 + i * 36.5;
        audio_event_summary_t s = audio_event_scan(jd, 8.0);
        TEST_ASSERT_TRUE(s.consonance >= 0.0f && s.consonance <= 1.0f);
        TEST_ASSERT_TRUE(s.event_intensity >= 0.0f
                         && s.event_intensity <= 1.0f);
    }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_consonance_in_range);
    RUN_TEST(test_consonance_zero_orb_neutral);
    RUN_TEST(test_consonance_negative_orb_neutral);
    RUN_TEST(test_consonance_different_dates);
    RUN_TEST(test_density_in_range);
    RUN_TEST(test_density_different_dates);
    RUN_TEST(test_intensity_in_range);
    RUN_TEST(test_intensity_blends_consonance_and_density);
    RUN_TEST(test_intensity_zero_orb_uses_neutral_consonance);
    RUN_TEST(test_scan_summary_valid);
    RUN_TEST(test_scan_tightest_orb_valid);
    RUN_TEST(test_scan_intensity_matches_standalone);
    RUN_TEST(test_scan_consonance_matches_standalone);
    RUN_TEST(test_scan_density_matches_standalone);
    RUN_TEST(test_scan_across_dates);
    return UNITY_END();
}
