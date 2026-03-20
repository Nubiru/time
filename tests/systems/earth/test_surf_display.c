/* test_surf_display.c — Tests for surf display formatter module.
 * TDD RED phase: all tests written before implementation.
 * Verifies display-ready surf forecast formatting, spot cards,
 * quality labels, tide descriptions, and coordinate formatting. */

#include "unity.h"
#include "systems/earth/surf_display.h"
#include "systems/earth/surf_spots.h"
#include "systems/earth/tide_predict.h"

#include <string.h>
#include <math.h>

/* Test JD: 2460000.5 (arbitrary known date) */
static const double TEST_JD = 2460000.5;

void setUp(void) {}
void tearDown(void) {}

/* ===== sd_quality_label — boundary values ===== */

void test_quality_label_zero(void) {
    const char *label = sd_quality_label(0.0);
    TEST_ASSERT_EQUAL_STRING("Poor", label);
}

void test_quality_label_just_below_02(void) {
    const char *label = sd_quality_label(0.19);
    TEST_ASSERT_EQUAL_STRING("Poor", label);
}

void test_quality_label_at_02(void) {
    const char *label = sd_quality_label(0.2);
    TEST_ASSERT_EQUAL_STRING("Fair", label);
}

void test_quality_label_just_below_04(void) {
    const char *label = sd_quality_label(0.39);
    TEST_ASSERT_EQUAL_STRING("Fair", label);
}

void test_quality_label_at_04(void) {
    const char *label = sd_quality_label(0.4);
    TEST_ASSERT_EQUAL_STRING("Good", label);
}

void test_quality_label_at_06(void) {
    const char *label = sd_quality_label(0.6);
    TEST_ASSERT_EQUAL_STRING("Excellent", label);
}

void test_quality_label_just_below_08(void) {
    const char *label = sd_quality_label(0.79);
    TEST_ASSERT_EQUAL_STRING("Excellent", label);
}

void test_quality_label_at_08(void) {
    const char *label = sd_quality_label(0.8);
    TEST_ASSERT_EQUAL_STRING("Epic", label);
}

void test_quality_label_at_10(void) {
    const char *label = sd_quality_label(1.0);
    TEST_ASSERT_EQUAL_STRING("Epic", label);
}

void test_quality_label_negative_clamps(void) {
    const char *label = sd_quality_label(-0.5);
    TEST_ASSERT_EQUAL_STRING("Poor", label);
}

void test_quality_label_above_one(void) {
    const char *label = sd_quality_label(1.5);
    TEST_ASSERT_EQUAL_STRING("Epic", label);
}

/* ===== sd_format_tide — tide line formatting ===== */

void test_format_tide_rising_spring(void) {
    char buf[SD_LINE_MAX];
    int len = sd_format_tide(1.5, 1, 1, 0, 0, 3.2, 6.1, buf, SD_LINE_MAX);
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Rising"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "high"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Spring"));
}

void test_format_tide_falling_neap(void) {
    char buf[SD_LINE_MAX];
    int len = sd_format_tide(-0.5, 0, 0, 1, 0, 8.0, 2.5, buf, SD_LINE_MAX);
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Falling"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "low"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Neap"));
}

void test_format_tide_king(void) {
    char buf[SD_LINE_MAX];
    int len = sd_format_tide(2.0, 1, 1, 0, 1, 1.0, 7.0, buf, SD_LINE_MAX);
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "King"));
}

void test_format_tide_normal(void) {
    char buf[SD_LINE_MAX];
    int len = sd_format_tide(0.5, 1, 0, 0, 0, 4.0, 8.0, buf, SD_LINE_MAX);
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Normal"));
}

void test_format_tide_small_buffer(void) {
    char buf[10];
    int len = sd_format_tide(0.5, 1, 0, 0, 0, 4.0, 8.0, buf, 10);
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_TRUE(len <= 9);
    TEST_ASSERT_TRUE(strlen(buf) < 10);
}

void test_format_tide_zero_buffer(void) {
    char buf[1] = {'X'};
    int len = sd_format_tide(0.5, 1, 0, 0, 0, 4.0, 8.0, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, len);
}

/* ===== sd_forecast — valid spot ===== */

void test_forecast_valid_spot_name(void) {
    sd_forecast_t f = sd_forecast(0, TEST_JD);
    TEST_ASSERT_EQUAL_STRING("Pipeline", f.spot_name);
}

void test_forecast_valid_spot_country(void) {
    sd_forecast_t f = sd_forecast(0, TEST_JD);
    TEST_ASSERT_EQUAL_STRING("USA", f.country);
}

void test_forecast_valid_spot_quality_range(void) {
    sd_forecast_t f = sd_forecast(0, TEST_JD);
    TEST_ASSERT_TRUE(f.quality >= 0.0 && f.quality <= 1.0);
}

void test_forecast_valid_spot_tide_line_populated(void) {
    sd_forecast_t f = sd_forecast(0, TEST_JD);
    TEST_ASSERT_TRUE(strlen(f.tide_line) > 0);
}

void test_forecast_valid_spot_quality_line_populated(void) {
    sd_forecast_t f = sd_forecast(0, TEST_JD);
    TEST_ASSERT_TRUE(strlen(f.quality_line) > 0);
}

void test_forecast_valid_spot_best_time_populated(void) {
    sd_forecast_t f = sd_forecast(0, TEST_JD);
    TEST_ASSERT_TRUE(strlen(f.best_time_line) > 0);
}

void test_forecast_valid_spot_advice_populated(void) {
    sd_forecast_t f = sd_forecast(0, TEST_JD);
    TEST_ASSERT_TRUE(strlen(f.advice) > 0);
}

void test_forecast_valid_spot_window_count(void) {
    sd_forecast_t f = sd_forecast(0, TEST_JD);
    TEST_ASSERT_TRUE(f.window_count >= 0);
}

void test_forecast_quality_line_contains_label(void) {
    sd_forecast_t f = sd_forecast(0, TEST_JD);
    /* Quality line should contain one of the labels */
    int has_label = (strstr(f.quality_line, "Poor") != NULL ||
                     strstr(f.quality_line, "Fair") != NULL ||
                     strstr(f.quality_line, "Good") != NULL ||
                     strstr(f.quality_line, "Excellent") != NULL ||
                     strstr(f.quality_line, "Epic") != NULL);
    TEST_ASSERT_TRUE(has_label);
}

void test_forecast_quality_line_contains_percent(void) {
    sd_forecast_t f = sd_forecast(0, TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(f.quality_line, "%"));
}

void test_forecast_best_time_contains_colon(void) {
    /* Best time should have HH:MM format with a colon */
    sd_forecast_t f = sd_forecast(0, TEST_JD);
    TEST_ASSERT_NOT_NULL(strstr(f.best_time_line, ":"));
}

/* ===== sd_forecast — invalid spots ===== */

void test_forecast_invalid_negative_empty(void) {
    sd_forecast_t f = sd_forecast(-1, TEST_JD);
    TEST_ASSERT_EQUAL_STRING("", f.spot_name);
    TEST_ASSERT_EQUAL_STRING("", f.country);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)f.quality);
}

void test_forecast_invalid_over_max_empty(void) {
    sd_forecast_t f = sd_forecast(50, TEST_JD);
    TEST_ASSERT_EQUAL_STRING("", f.spot_name);
    TEST_ASSERT_EQUAL_STRING("", f.country);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)f.quality);
}

void test_forecast_invalid_large_index(void) {
    sd_forecast_t f = sd_forecast(999, TEST_JD);
    TEST_ASSERT_EQUAL_STRING("", f.spot_name);
    TEST_ASSERT_EQUAL_INT(0, f.window_count);
}

/* ===== sd_forecast — multiple spots ===== */

void test_forecast_different_spots_different_names(void) {
    sd_forecast_t f0 = sd_forecast(0, TEST_JD);
    sd_forecast_t f5 = sd_forecast(5, TEST_JD);
    /* Pipeline vs Trestles */
    TEST_ASSERT_TRUE(strcmp(f0.spot_name, f5.spot_name) != 0);
}

/* ===== sd_spot_card — valid spot ===== */

void test_spot_card_valid_name(void) {
    sd_spot_card_t card = sd_spot_card(0);
    TEST_ASSERT_EQUAL_STRING("Pipeline", card.name);
}

void test_spot_card_valid_country(void) {
    sd_spot_card_t card = sd_spot_card(0);
    TEST_ASSERT_EQUAL_STRING("USA", card.country);
}

void test_spot_card_valid_wave_type(void) {
    sd_spot_card_t card = sd_spot_card(0);
    /* Pipeline is WAVE_REEF -> "Reef break" */
    TEST_ASSERT_EQUAL_STRING("Reef break", card.wave_type);
}

void test_spot_card_valid_best_tide(void) {
    sd_spot_card_t card = sd_spot_card(0);
    /* Pipeline is TIDE_LOW -> "Low tide" */
    TEST_ASSERT_EQUAL_STRING("Low tide", card.best_tide);
}

void test_spot_card_valid_best_season(void) {
    sd_spot_card_t card = sd_spot_card(0);
    /* Pipeline is SEASON_WINTER -> "Winter" */
    TEST_ASSERT_EQUAL_STRING("Winter", card.best_season);
}

void test_spot_card_valid_description(void) {
    sd_spot_card_t card = sd_spot_card(0);
    TEST_ASSERT_TRUE(strlen(card.description) > 0);
}

void test_spot_card_coordinates_format(void) {
    sd_spot_card_t card = sd_spot_card(0);
    /* Pipeline: 21.665, -158.053 -> should contain degree symbols */
    TEST_ASSERT_NOT_NULL(strstr(card.coordinates, "\xc2\xb0"));
    TEST_ASSERT_NOT_NULL(strstr(card.coordinates, "N"));
    TEST_ASSERT_NOT_NULL(strstr(card.coordinates, "W"));
}

void test_spot_card_south_east_coordinates(void) {
    /* Uluwatu (index 20): lat -8.815, lon 115.087 -> S, E */
    sd_spot_card_t card = sd_spot_card(20);
    TEST_ASSERT_NOT_NULL(strstr(card.coordinates, "S"));
    TEST_ASSERT_NOT_NULL(strstr(card.coordinates, "E"));
}

/* ===== sd_spot_card — invalid spot ===== */

void test_spot_card_invalid_negative(void) {
    sd_spot_card_t card = sd_spot_card(-1);
    TEST_ASSERT_EQUAL_STRING("", card.name);
    TEST_ASSERT_EQUAL_STRING("", card.country);
}

void test_spot_card_invalid_over_max(void) {
    sd_spot_card_t card = sd_spot_card(50);
    TEST_ASSERT_EQUAL_STRING("", card.name);
    TEST_ASSERT_EQUAL_STRING("", card.country);
}

/* ===== sd_format_forecast — multi-line output ===== */

void test_format_forecast_non_empty(void) {
    sd_forecast_t f = sd_forecast(0, TEST_JD);
    char buf[SD_FORECAST_MAX];
    int len = sd_format_forecast(&f, buf, SD_FORECAST_MAX);
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

void test_format_forecast_contains_spot_name(void) {
    sd_forecast_t f = sd_forecast(0, TEST_JD);
    char buf[SD_FORECAST_MAX];
    sd_format_forecast(&f, buf, SD_FORECAST_MAX);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Pipeline"));
}

void test_format_forecast_contains_country(void) {
    sd_forecast_t f = sd_forecast(0, TEST_JD);
    char buf[SD_FORECAST_MAX];
    sd_format_forecast(&f, buf, SD_FORECAST_MAX);
    TEST_ASSERT_NOT_NULL(strstr(buf, "USA"));
}

void test_format_forecast_small_buffer(void) {
    sd_forecast_t f = sd_forecast(0, TEST_JD);
    char buf[20];
    int len = sd_format_forecast(&f, buf, 20);
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_TRUE(len <= 19);
}

void test_format_forecast_zero_buffer(void) {
    sd_forecast_t f = sd_forecast(0, TEST_JD);
    char buf[1] = {'X'};
    int len = sd_format_forecast(&f, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, len);
}

/* ===== sd_format_card — multi-line output ===== */

void test_format_card_non_empty(void) {
    sd_spot_card_t card = sd_spot_card(0);
    char buf[SD_CARD_MAX];
    int len = sd_format_card(&card, buf, SD_CARD_MAX);
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

void test_format_card_contains_name(void) {
    sd_spot_card_t card = sd_spot_card(0);
    char buf[SD_CARD_MAX];
    sd_format_card(&card, buf, SD_CARD_MAX);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Pipeline"));
}

void test_format_card_contains_wave_type(void) {
    sd_spot_card_t card = sd_spot_card(0);
    char buf[SD_CARD_MAX];
    sd_format_card(&card, buf, SD_CARD_MAX);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Reef break"));
}

void test_format_card_small_buffer(void) {
    sd_spot_card_t card = sd_spot_card(0);
    char buf[15];
    int len = sd_format_card(&card, buf, 15);
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_TRUE(len <= 14);
}

void test_format_card_zero_buffer(void) {
    sd_spot_card_t card = sd_spot_card(0);
    char buf[1] = {'X'};
    int len = sd_format_card(&card, buf, 0);
    TEST_ASSERT_EQUAL_INT(0, len);
}

/* ===== sd_forecast — spot 49 (last valid) ===== */

void test_forecast_last_valid_spot(void) {
    sd_forecast_t f = sd_forecast(49, TEST_JD);
    TEST_ASSERT_EQUAL_STRING("Canggu", f.spot_name);
    TEST_ASSERT_EQUAL_STRING("Indonesia", f.country);
    TEST_ASSERT_TRUE(f.quality >= 0.0 && f.quality <= 1.0);
}

/* ===== main ===== */

int main(void) {
    UNITY_BEGIN();

    /* sd_quality_label — boundaries */
    RUN_TEST(test_quality_label_zero);
    RUN_TEST(test_quality_label_just_below_02);
    RUN_TEST(test_quality_label_at_02);
    RUN_TEST(test_quality_label_just_below_04);
    RUN_TEST(test_quality_label_at_04);
    RUN_TEST(test_quality_label_at_06);
    RUN_TEST(test_quality_label_just_below_08);
    RUN_TEST(test_quality_label_at_08);
    RUN_TEST(test_quality_label_at_10);
    RUN_TEST(test_quality_label_negative_clamps);
    RUN_TEST(test_quality_label_above_one);

    /* sd_format_tide */
    RUN_TEST(test_format_tide_rising_spring);
    RUN_TEST(test_format_tide_falling_neap);
    RUN_TEST(test_format_tide_king);
    RUN_TEST(test_format_tide_normal);
    RUN_TEST(test_format_tide_small_buffer);
    RUN_TEST(test_format_tide_zero_buffer);

    /* sd_forecast — valid spot */
    RUN_TEST(test_forecast_valid_spot_name);
    RUN_TEST(test_forecast_valid_spot_country);
    RUN_TEST(test_forecast_valid_spot_quality_range);
    RUN_TEST(test_forecast_valid_spot_tide_line_populated);
    RUN_TEST(test_forecast_valid_spot_quality_line_populated);
    RUN_TEST(test_forecast_valid_spot_best_time_populated);
    RUN_TEST(test_forecast_valid_spot_advice_populated);
    RUN_TEST(test_forecast_valid_spot_window_count);
    RUN_TEST(test_forecast_quality_line_contains_label);
    RUN_TEST(test_forecast_quality_line_contains_percent);
    RUN_TEST(test_forecast_best_time_contains_colon);

    /* sd_forecast — invalid spots */
    RUN_TEST(test_forecast_invalid_negative_empty);
    RUN_TEST(test_forecast_invalid_over_max_empty);
    RUN_TEST(test_forecast_invalid_large_index);

    /* sd_forecast — multiple spots */
    RUN_TEST(test_forecast_different_spots_different_names);

    /* sd_spot_card — valid */
    RUN_TEST(test_spot_card_valid_name);
    RUN_TEST(test_spot_card_valid_country);
    RUN_TEST(test_spot_card_valid_wave_type);
    RUN_TEST(test_spot_card_valid_best_tide);
    RUN_TEST(test_spot_card_valid_best_season);
    RUN_TEST(test_spot_card_valid_description);
    RUN_TEST(test_spot_card_coordinates_format);
    RUN_TEST(test_spot_card_south_east_coordinates);

    /* sd_spot_card — invalid */
    RUN_TEST(test_spot_card_invalid_negative);
    RUN_TEST(test_spot_card_invalid_over_max);

    /* sd_format_forecast */
    RUN_TEST(test_format_forecast_non_empty);
    RUN_TEST(test_format_forecast_contains_spot_name);
    RUN_TEST(test_format_forecast_contains_country);
    RUN_TEST(test_format_forecast_small_buffer);
    RUN_TEST(test_format_forecast_zero_buffer);

    /* sd_format_card */
    RUN_TEST(test_format_card_non_empty);
    RUN_TEST(test_format_card_contains_name);
    RUN_TEST(test_format_card_contains_wave_type);
    RUN_TEST(test_format_card_small_buffer);
    RUN_TEST(test_format_card_zero_buffer);

    /* sd_forecast — edge */
    RUN_TEST(test_forecast_last_valid_spot);

    return UNITY_END();
}
