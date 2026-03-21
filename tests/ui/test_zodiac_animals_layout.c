#include "../unity/unity.h"
#include "../../src/ui/zodiac_animals_layout.h"
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void setUp(void) {}
void tearDown(void) {}

/* Test date: JD 2460676.5 = 2025-01-01 */
#define TEST_JD 2460676.5

/* --- Center and radius --- */

void test_center_x(void)
{
    zodiac_animals_layout_t layout = zodiac_animals_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.50f, layout.center_x);
}

void test_center_y(void)
{
    zodiac_animals_layout_t layout = zodiac_animals_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.50f, layout.center_y);
}

void test_radius(void)
{
    zodiac_animals_layout_t layout = zodiac_animals_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.38f, layout.radius);
}

/* --- Animal data populated --- */

void test_animal_indices(void)
{
    zodiac_animals_layout_t layout = zodiac_animals_compute(TEST_JD);
    for (int i = 0; i < ZA_ANIMAL_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(i, layout.animals[i].animal);
    }
}

void test_animal_names_not_null(void)
{
    zodiac_animals_layout_t layout = zodiac_animals_compute(TEST_JD);
    for (int i = 0; i < ZA_ANIMAL_COUNT; i++) {
        TEST_ASSERT_NOT_NULL(layout.animals[i].name);
        TEST_ASSERT_TRUE(strlen(layout.animals[i].name) > 0);
    }
}

void test_animal_symbols_not_null(void)
{
    zodiac_animals_layout_t layout = zodiac_animals_compute(TEST_JD);
    for (int i = 0; i < ZA_ANIMAL_COUNT; i++) {
        TEST_ASSERT_NOT_NULL(layout.animals[i].symbol);
        TEST_ASSERT_TRUE(strlen(layout.animals[i].symbol) > 0);
    }
}

void test_current_animal_in_range(void)
{
    zodiac_animals_layout_t layout = zodiac_animals_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.current_animal >= 0);
    TEST_ASSERT_TRUE(layout.current_animal < ZA_ANIMAL_COUNT);
}

void test_current_element_in_range(void)
{
    zodiac_animals_layout_t layout = zodiac_animals_compute(TEST_JD);
    TEST_ASSERT_TRUE(layout.current_element >= 0);
    TEST_ASSERT_TRUE(layout.current_element <= 4);
}

void test_current_element_name_not_null(void)
{
    zodiac_animals_layout_t layout = zodiac_animals_compute(TEST_JD);
    TEST_ASSERT_NOT_NULL(layout.current_element_name);
    TEST_ASSERT_TRUE(strlen(layout.current_element_name) > 0);
}

/* --- Angles --- */

void test_angles_at_30_degree_intervals(void)
{
    zodiac_animals_layout_t layout = zodiac_animals_compute(TEST_JD);
    for (int i = 0; i < ZA_ANIMAL_COUNT; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.01f, (float)(i * 30),
                                 layout.animals[i].angle_deg);
    }
}

void test_rat_at_top(void)
{
    /* Rat (index 0) should be at top: x=0.50, y=0.12 (center_y - radius) */
    zodiac_animals_layout_t layout = zodiac_animals_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f, layout.animals[0].x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.12f, layout.animals[0].y);
}

void test_horse_at_bottom(void)
{
    /* Horse (index 6) at 180 degrees: x=0.50, y=0.88 (center_y + radius) */
    zodiac_animals_layout_t layout = zodiac_animals_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f, layout.animals[6].x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.88f, layout.animals[6].y);
}

void test_rabbit_at_right(void)
{
    /* Rabbit (index 3) at 90 degrees: x=0.88, y=0.50 */
    zodiac_animals_layout_t layout = zodiac_animals_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.88f, layout.animals[3].x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f, layout.animals[3].y);
}

void test_rooster_at_left(void)
{
    /* Rooster (index 9) at 270 degrees: x=0.12, y=0.50 */
    zodiac_animals_layout_t layout = zodiac_animals_compute(TEST_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.12f, layout.animals[9].x);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.50f, layout.animals[9].y);
}

/* --- Positions in normalized space --- */

void test_all_positions_normalized(void)
{
    zodiac_animals_layout_t layout = zodiac_animals_compute(TEST_JD);
    for (int i = 0; i < ZA_ANIMAL_COUNT; i++) {
        TEST_ASSERT_TRUE(layout.animals[i].x >= 0.0f);
        TEST_ASSERT_TRUE(layout.animals[i].x <= 1.0f);
        TEST_ASSERT_TRUE(layout.animals[i].y >= 0.0f);
        TEST_ASSERT_TRUE(layout.animals[i].y <= 1.0f);
    }
}

/* --- Highlighting --- */

void test_exactly_one_highlighted(void)
{
    zodiac_animals_layout_t layout = zodiac_animals_compute(TEST_JD);
    int count = 0;
    for (int i = 0; i < ZA_ANIMAL_COUNT; i++) {
        if (layout.animals[i].highlighted) {
            count++;
        }
    }
    TEST_ASSERT_EQUAL_INT(1, count);
}

void test_highlighted_matches_current(void)
{
    zodiac_animals_layout_t layout = zodiac_animals_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(1,
        layout.animals[layout.current_animal].highlighted);
}

/* --- Accessor --- */

void test_accessor_valid_index(void)
{
    zodiac_animals_layout_t layout = zodiac_animals_compute(TEST_JD);
    za_animal_t a = zodiac_animals_at(&layout, 0);
    TEST_ASSERT_EQUAL_INT(0, a.animal);
    TEST_ASSERT_NOT_NULL(a.name);
}

void test_accessor_invalid_index(void)
{
    zodiac_animals_layout_t layout = zodiac_animals_compute(TEST_JD);
    za_animal_t a = zodiac_animals_at(&layout, 12);
    TEST_ASSERT_EQUAL_INT(-1, a.animal);
    TEST_ASSERT_EQUAL_STRING("?", a.name);
}

void test_accessor_negative_index(void)
{
    zodiac_animals_layout_t layout = zodiac_animals_compute(TEST_JD);
    za_animal_t a = zodiac_animals_at(&layout, -1);
    TEST_ASSERT_EQUAL_INT(-1, a.animal);
}

void test_accessor_null_layout(void)
{
    za_animal_t a = zodiac_animals_at(NULL, 0);
    TEST_ASSERT_EQUAL_INT(-1, a.animal);
    TEST_ASSERT_EQUAL_STRING("?", a.name);
    TEST_ASSERT_EQUAL_STRING("?", a.symbol);
}

/* --- Purity --- */

void test_purity_same_input_same_output(void)
{
    zodiac_animals_layout_t a = zodiac_animals_compute(TEST_JD);
    zodiac_animals_layout_t b = zodiac_animals_compute(TEST_JD);
    TEST_ASSERT_EQUAL_INT(a.current_animal, b.current_animal);
    TEST_ASSERT_EQUAL_INT(a.current_element, b.current_element);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.animals[0].x, b.animals[0].x);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, a.animals[0].y, b.animals[0].y);
}

void test_purity_different_years(void)
{
    /* 2025-01-01 vs 2026-02-01 — different years */
    zodiac_animals_layout_t a = zodiac_animals_compute(2460676.5);
    zodiac_animals_layout_t b = zodiac_animals_compute(2461076.5);
    /* A year apart should differ in current_animal */
    TEST_ASSERT_TRUE(a.current_animal != b.current_animal ||
                     a.current_element != b.current_element);
}

/* --- Known zodiac names --- */

void test_first_animal_is_rat(void)
{
    zodiac_animals_layout_t layout = zodiac_animals_compute(TEST_JD);
    TEST_ASSERT_EQUAL_STRING("Rat", layout.animals[0].name);
}

void test_last_animal_is_pig(void)
{
    zodiac_animals_layout_t layout = zodiac_animals_compute(TEST_JD);
    TEST_ASSERT_EQUAL_STRING("Pig", layout.animals[11].name);
}

int main(void)
{
    UNITY_BEGIN();
    /* Center and radius */
    RUN_TEST(test_center_x);
    RUN_TEST(test_center_y);
    RUN_TEST(test_radius);
    /* Animal data */
    RUN_TEST(test_animal_indices);
    RUN_TEST(test_animal_names_not_null);
    RUN_TEST(test_animal_symbols_not_null);
    RUN_TEST(test_current_animal_in_range);
    RUN_TEST(test_current_element_in_range);
    RUN_TEST(test_current_element_name_not_null);
    /* Angles */
    RUN_TEST(test_angles_at_30_degree_intervals);
    RUN_TEST(test_rat_at_top);
    RUN_TEST(test_horse_at_bottom);
    RUN_TEST(test_rabbit_at_right);
    RUN_TEST(test_rooster_at_left);
    /* Positions */
    RUN_TEST(test_all_positions_normalized);
    /* Highlighting */
    RUN_TEST(test_exactly_one_highlighted);
    RUN_TEST(test_highlighted_matches_current);
    /* Accessor */
    RUN_TEST(test_accessor_valid_index);
    RUN_TEST(test_accessor_invalid_index);
    RUN_TEST(test_accessor_negative_index);
    RUN_TEST(test_accessor_null_layout);
    /* Purity */
    RUN_TEST(test_purity_same_input_same_output);
    RUN_TEST(test_purity_different_years);
    /* Known names */
    RUN_TEST(test_first_animal_is_rat);
    RUN_TEST(test_last_animal_is_pig);
    return UNITY_END();
}
