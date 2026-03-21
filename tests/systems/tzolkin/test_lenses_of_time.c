#include "../../unity/unity.h"
#include "../../../src/systems/tzolkin/lenses_of_time.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* --- Count --- */

static void test_lens_count_is_4(void)
{
    TEST_ASSERT_EQUAL_INT(4, lens_count());
}

/* --- Lens 1: Cosmic Standard / 108 / Stellar Mind / Time Pulsar / 4th --- */

static void test_lens1_number(void)
{
    lens_of_time_t l = lens_by_number(1);
    TEST_ASSERT_EQUAL_INT(1, l.number);
}

static void test_lens1_value(void)
{
    lens_of_time_t l = lens_by_number(1);
    TEST_ASSERT_EQUAL_INT(108, l.value);
}

static void test_lens1_name(void)
{
    lens_of_time_t l = lens_by_number(1);
    TEST_ASSERT_EQUAL_STRING("Cosmic Standard", l.name);
}

static void test_lens1_template(void)
{
    lens_of_time_t l = lens_by_number(1);
    TEST_ASSERT_EQUAL_STRING("Stellar Mind", l.template_name);
}

static void test_lens1_pulsar(void)
{
    lens_of_time_t l = lens_by_number(1);
    TEST_ASSERT_EQUAL_STRING("Time Pulsar", l.pulsar);
}

static void test_lens1_dimension(void)
{
    lens_of_time_t l = lens_by_number(1);
    TEST_ASSERT_EQUAL_INT(4, l.dimension);
}

/* --- Lens 2: Cosmic Harmonic / 144 / Foundation of Tollan / Life Pulsar / 1st --- */

static void test_lens2_number(void)
{
    lens_of_time_t l = lens_by_number(2);
    TEST_ASSERT_EQUAL_INT(2, l.number);
}

static void test_lens2_value(void)
{
    lens_of_time_t l = lens_by_number(2);
    TEST_ASSERT_EQUAL_INT(144, l.value);
}

static void test_lens2_name(void)
{
    lens_of_time_t l = lens_by_number(2);
    TEST_ASSERT_EQUAL_STRING("Cosmic Harmonic", l.name);
}

static void test_lens2_template(void)
{
    lens_of_time_t l = lens_by_number(2);
    TEST_ASSERT_EQUAL_STRING("Foundation of Tollan", l.template_name);
}

static void test_lens2_pulsar(void)
{
    lens_of_time_t l = lens_by_number(2);
    TEST_ASSERT_EQUAL_STRING("Life Pulsar", l.pulsar);
}

static void test_lens2_dimension(void)
{
    lens_of_time_t l = lens_by_number(2);
    TEST_ASSERT_EQUAL_INT(1, l.dimension);
}

/* --- Lens 3: Cosmic Cube / 216 / Primary Cube / Sense Pulsar / 2nd --- */

static void test_lens3_number(void)
{
    lens_of_time_t l = lens_by_number(3);
    TEST_ASSERT_EQUAL_INT(3, l.number);
}

static void test_lens3_value(void)
{
    lens_of_time_t l = lens_by_number(3);
    TEST_ASSERT_EQUAL_INT(216, l.value);
}

static void test_lens3_name(void)
{
    lens_of_time_t l = lens_by_number(3);
    TEST_ASSERT_EQUAL_STRING("Cosmic Cube", l.name);
}

static void test_lens3_template(void)
{
    lens_of_time_t l = lens_by_number(3);
    TEST_ASSERT_EQUAL_STRING("Primary Cube", l.template_name);
}

static void test_lens3_pulsar(void)
{
    lens_of_time_t l = lens_by_number(3);
    TEST_ASSERT_EQUAL_STRING("Sense Pulsar", l.pulsar);
}

static void test_lens3_dimension(void)
{
    lens_of_time_t l = lens_by_number(3);
    TEST_ASSERT_EQUAL_INT(2, l.dimension);
}

/* --- Lens 4: Ultimate Sphere / 288 / Polar Light Harmonic / Mind Pulsar / 3rd --- */

static void test_lens4_number(void)
{
    lens_of_time_t l = lens_by_number(4);
    TEST_ASSERT_EQUAL_INT(4, l.number);
}

static void test_lens4_value(void)
{
    lens_of_time_t l = lens_by_number(4);
    TEST_ASSERT_EQUAL_INT(288, l.value);
}

static void test_lens4_name(void)
{
    lens_of_time_t l = lens_by_number(4);
    TEST_ASSERT_EQUAL_STRING("Ultimate Sphere", l.name);
}

static void test_lens4_template(void)
{
    lens_of_time_t l = lens_by_number(4);
    TEST_ASSERT_EQUAL_STRING("Polar Light Harmonic", l.template_name);
}

static void test_lens4_pulsar(void)
{
    lens_of_time_t l = lens_by_number(4);
    TEST_ASSERT_EQUAL_STRING("Mind Pulsar", l.pulsar);
}

static void test_lens4_dimension(void)
{
    lens_of_time_t l = lens_by_number(4);
    TEST_ASSERT_EQUAL_INT(3, l.dimension);
}

/* --- Tone-to-Lens mapping: all 13 tones --- */

static void test_tone1_maps_to_lens1(void)
{
    lens_of_time_t l = lens_for_tone(1);
    TEST_ASSERT_EQUAL_INT(1, l.number);
    TEST_ASSERT_EQUAL_INT(108, l.value);
}

static void test_tone2_maps_to_lens2(void)
{
    lens_of_time_t l = lens_for_tone(2);
    TEST_ASSERT_EQUAL_INT(2, l.number);
    TEST_ASSERT_EQUAL_INT(144, l.value);
}

static void test_tone3_maps_to_lens3(void)
{
    lens_of_time_t l = lens_for_tone(3);
    TEST_ASSERT_EQUAL_INT(3, l.number);
    TEST_ASSERT_EQUAL_INT(216, l.value);
}

static void test_tone4_maps_to_lens4(void)
{
    lens_of_time_t l = lens_for_tone(4);
    TEST_ASSERT_EQUAL_INT(4, l.number);
    TEST_ASSERT_EQUAL_INT(288, l.value);
}

static void test_tone5_maps_to_lens1(void)
{
    lens_of_time_t l = lens_for_tone(5);
    TEST_ASSERT_EQUAL_INT(1, l.number);
    TEST_ASSERT_EQUAL_INT(108, l.value);
}

static void test_tone6_maps_to_lens2(void)
{
    lens_of_time_t l = lens_for_tone(6);
    TEST_ASSERT_EQUAL_INT(2, l.number);
    TEST_ASSERT_EQUAL_INT(144, l.value);
}

static void test_tone7_maps_to_lens3(void)
{
    lens_of_time_t l = lens_for_tone(7);
    TEST_ASSERT_EQUAL_INT(3, l.number);
    TEST_ASSERT_EQUAL_INT(216, l.value);
}

static void test_tone8_maps_to_lens4(void)
{
    lens_of_time_t l = lens_for_tone(8);
    TEST_ASSERT_EQUAL_INT(4, l.number);
    TEST_ASSERT_EQUAL_INT(288, l.value);
}

static void test_tone9_maps_to_lens1(void)
{
    lens_of_time_t l = lens_for_tone(9);
    TEST_ASSERT_EQUAL_INT(1, l.number);
    TEST_ASSERT_EQUAL_INT(108, l.value);
}

static void test_tone10_maps_to_lens2(void)
{
    lens_of_time_t l = lens_for_tone(10);
    TEST_ASSERT_EQUAL_INT(2, l.number);
    TEST_ASSERT_EQUAL_INT(144, l.value);
}

static void test_tone11_maps_to_lens3(void)
{
    lens_of_time_t l = lens_for_tone(11);
    TEST_ASSERT_EQUAL_INT(3, l.number);
    TEST_ASSERT_EQUAL_INT(216, l.value);
}

static void test_tone12_maps_to_lens4(void)
{
    lens_of_time_t l = lens_for_tone(12);
    TEST_ASSERT_EQUAL_INT(4, l.number);
    TEST_ASSERT_EQUAL_INT(288, l.value);
}

static void test_tone13_maps_to_lens1(void)
{
    lens_of_time_t l = lens_for_tone(13);
    TEST_ASSERT_EQUAL_INT(1, l.number);
    TEST_ASSERT_EQUAL_INT(108, l.value);
}

/* --- Tonal values --- */

static void test_tonal_value_tone1(void)
{
    TEST_ASSERT_EQUAL_INT(108, lens_tonal_value(1));  /* 1 x 108 */
}

static void test_tonal_value_tone5(void)
{
    TEST_ASSERT_EQUAL_INT(540, lens_tonal_value(5));  /* 5 x 108 */
}

static void test_tonal_value_tone13(void)
{
    TEST_ASSERT_EQUAL_INT(1404, lens_tonal_value(13)); /* 13 x 108 */
}

static void test_tonal_value_tone8(void)
{
    TEST_ASSERT_EQUAL_INT(2304, lens_tonal_value(8));  /* 8 x 288 */
}

static void test_tonal_value_tone12(void)
{
    TEST_ASSERT_EQUAL_INT(3456, lens_tonal_value(12)); /* 12 x 288 */
}

static void test_tonal_value_tone2(void)
{
    TEST_ASSERT_EQUAL_INT(288, lens_tonal_value(2));   /* 2 x 144 */
}

static void test_tonal_value_tone3(void)
{
    TEST_ASSERT_EQUAL_INT(648, lens_tonal_value(3));   /* 3 x 216 */
}

static void test_tonal_value_tone9(void)
{
    TEST_ASSERT_EQUAL_INT(972, lens_tonal_value(9));   /* 9 x 108 */
}

static void test_tonal_value_tone10(void)
{
    TEST_ASSERT_EQUAL_INT(1440, lens_tonal_value(10)); /* 10 x 144 */
}

static void test_tonal_value_tone4(void)
{
    TEST_ASSERT_EQUAL_INT(1152, lens_tonal_value(4));  /* 4 x 288 */
}

static void test_tonal_value_tone6(void)
{
    TEST_ASSERT_EQUAL_INT(864, lens_tonal_value(6));   /* 6 x 144 */
}

static void test_tonal_value_tone7(void)
{
    TEST_ASSERT_EQUAL_INT(1512, lens_tonal_value(7));  /* 7 x 216 */
}

static void test_tonal_value_tone11(void)
{
    TEST_ASSERT_EQUAL_INT(2376, lens_tonal_value(11)); /* 11 x 216 */
}

/* --- lens_value_for_tone --- */

static void test_lens_value_tone1(void)
{
    TEST_ASSERT_EQUAL_INT(108, lens_value_for_tone(1));
}

static void test_lens_value_tone4(void)
{
    TEST_ASSERT_EQUAL_INT(288, lens_value_for_tone(4));
}

static void test_lens_value_tone7(void)
{
    TEST_ASSERT_EQUAL_INT(216, lens_value_for_tone(7));
}

static void test_lens_value_tone10(void)
{
    TEST_ASSERT_EQUAL_INT(144, lens_value_for_tone(10));
}

static void test_lens_value_tone13(void)
{
    TEST_ASSERT_EQUAL_INT(108, lens_value_for_tone(13));
}

/* --- Boundary: invalid tones --- */

static void test_tone0_returns_zeroed(void)
{
    lens_of_time_t l = lens_for_tone(0);
    TEST_ASSERT_EQUAL_INT(0, l.number);
    TEST_ASSERT_EQUAL_INT(0, l.value);
    TEST_ASSERT_NULL(l.name);
    TEST_ASSERT_NULL(l.template_name);
    TEST_ASSERT_NULL(l.pulsar);
    TEST_ASSERT_EQUAL_INT(0, l.dimension);
}

static void test_tone14_returns_zeroed(void)
{
    lens_of_time_t l = lens_for_tone(14);
    TEST_ASSERT_EQUAL_INT(0, l.number);
    TEST_ASSERT_EQUAL_INT(0, l.value);
    TEST_ASSERT_NULL(l.name);
    TEST_ASSERT_NULL(l.template_name);
    TEST_ASSERT_NULL(l.pulsar);
    TEST_ASSERT_EQUAL_INT(0, l.dimension);
}

static void test_negative_tone_returns_zeroed(void)
{
    lens_of_time_t l = lens_for_tone(-1);
    TEST_ASSERT_EQUAL_INT(0, l.number);
    TEST_ASSERT_EQUAL_INT(0, l.value);
    TEST_ASSERT_NULL(l.name);
    TEST_ASSERT_EQUAL_INT(0, l.dimension);
}

static void test_tonal_value_tone0_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, lens_tonal_value(0));
}

static void test_tonal_value_tone14_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, lens_tonal_value(14));
}

static void test_tonal_value_negative_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, lens_tonal_value(-1));
}

static void test_lens_value_tone0_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, lens_value_for_tone(0));
}

static void test_lens_value_tone14_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, lens_value_for_tone(14));
}

static void test_lens_value_negative_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, lens_value_for_tone(-1));
}

/* --- Boundary: invalid lens_by_number --- */

static void test_lens0_returns_zeroed(void)
{
    lens_of_time_t l = lens_by_number(0);
    TEST_ASSERT_EQUAL_INT(0, l.number);
    TEST_ASSERT_EQUAL_INT(0, l.value);
    TEST_ASSERT_NULL(l.name);
}

static void test_lens5_returns_zeroed(void)
{
    lens_of_time_t l = lens_by_number(5);
    TEST_ASSERT_EQUAL_INT(0, l.number);
    TEST_ASSERT_EQUAL_INT(0, l.value);
    TEST_ASSERT_NULL(l.name);
}

static void test_lens_negative_returns_zeroed(void)
{
    lens_of_time_t l = lens_by_number(-1);
    TEST_ASSERT_EQUAL_INT(0, l.number);
    TEST_ASSERT_EQUAL_INT(0, l.value);
    TEST_ASSERT_NULL(l.name);
}

/* --- Purity: same input gives same output --- */

static void test_purity_same_result(void)
{
    lens_of_time_t a = lens_for_tone(7);
    lens_of_time_t b = lens_for_tone(7);
    TEST_ASSERT_EQUAL_INT(a.number, b.number);
    TEST_ASSERT_EQUAL_INT(a.value, b.value);
    TEST_ASSERT_EQUAL_INT(a.dimension, b.dimension);
    TEST_ASSERT_EQUAL_STRING(a.name, b.name);
    TEST_ASSERT_EQUAL_STRING(a.template_name, b.template_name);
    TEST_ASSERT_EQUAL_STRING(a.pulsar, b.pulsar);
}

/* --- Sweep: all 4 lenses have non-null strings --- */

static void test_all_lenses_have_valid_data(void)
{
    for (int n = 1; n <= 4; n++) {
        lens_of_time_t l = lens_by_number(n);
        TEST_ASSERT_EQUAL_INT(n, l.number);
        TEST_ASSERT_TRUE(l.value > 0);
        TEST_ASSERT_NOT_NULL(l.name);
        TEST_ASSERT_NOT_NULL(l.template_name);
        TEST_ASSERT_NOT_NULL(l.pulsar);
        TEST_ASSERT_TRUE(l.dimension >= 1 && l.dimension <= 4);
        TEST_ASSERT_TRUE(strlen(l.name) > 0);
        TEST_ASSERT_TRUE(strlen(l.template_name) > 0);
        TEST_ASSERT_TRUE(strlen(l.pulsar) > 0);
    }
}

/* --- Lens values are unique --- */

static void test_lens_values_unique(void)
{
    int vals[4];
    for (int n = 1; n <= 4; n++) {
        vals[n - 1] = lens_by_number(n).value;
    }
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            TEST_ASSERT_NOT_EQUAL(vals[i], vals[j]);
        }
    }
}

/* --- Lens values are multiples of 36 --- */

static void test_lens_values_multiples_of_36(void)
{
    for (int n = 1; n <= 4; n++) {
        int v = lens_by_number(n).value;
        TEST_ASSERT_EQUAL_INT(0, v % 36);
    }
}

/* --- 108 = sacred number (used by Heptad Gate day 1 too) --- */

static void test_108_is_lens1_value(void)
{
    TEST_ASSERT_EQUAL_INT(108, lens_by_number(1).value);
}

int main(void)
{
    UNITY_BEGIN();

    /* Count */
    RUN_TEST(test_lens_count_is_4);

    /* Lens 1 — Cosmic Standard / 108 */
    RUN_TEST(test_lens1_number);
    RUN_TEST(test_lens1_value);
    RUN_TEST(test_lens1_name);
    RUN_TEST(test_lens1_template);
    RUN_TEST(test_lens1_pulsar);
    RUN_TEST(test_lens1_dimension);

    /* Lens 2 — Cosmic Harmonic / 144 */
    RUN_TEST(test_lens2_number);
    RUN_TEST(test_lens2_value);
    RUN_TEST(test_lens2_name);
    RUN_TEST(test_lens2_template);
    RUN_TEST(test_lens2_pulsar);
    RUN_TEST(test_lens2_dimension);

    /* Lens 3 — Cosmic Cube / 216 */
    RUN_TEST(test_lens3_number);
    RUN_TEST(test_lens3_value);
    RUN_TEST(test_lens3_name);
    RUN_TEST(test_lens3_template);
    RUN_TEST(test_lens3_pulsar);
    RUN_TEST(test_lens3_dimension);

    /* Lens 4 — Ultimate Sphere / 288 */
    RUN_TEST(test_lens4_number);
    RUN_TEST(test_lens4_value);
    RUN_TEST(test_lens4_name);
    RUN_TEST(test_lens4_template);
    RUN_TEST(test_lens4_pulsar);
    RUN_TEST(test_lens4_dimension);

    /* Tone-to-Lens mapping — all 13 tones */
    RUN_TEST(test_tone1_maps_to_lens1);
    RUN_TEST(test_tone2_maps_to_lens2);
    RUN_TEST(test_tone3_maps_to_lens3);
    RUN_TEST(test_tone4_maps_to_lens4);
    RUN_TEST(test_tone5_maps_to_lens1);
    RUN_TEST(test_tone6_maps_to_lens2);
    RUN_TEST(test_tone7_maps_to_lens3);
    RUN_TEST(test_tone8_maps_to_lens4);
    RUN_TEST(test_tone9_maps_to_lens1);
    RUN_TEST(test_tone10_maps_to_lens2);
    RUN_TEST(test_tone11_maps_to_lens3);
    RUN_TEST(test_tone12_maps_to_lens4);
    RUN_TEST(test_tone13_maps_to_lens1);

    /* Tonal values — all 13 tones */
    RUN_TEST(test_tonal_value_tone1);
    RUN_TEST(test_tonal_value_tone2);
    RUN_TEST(test_tonal_value_tone3);
    RUN_TEST(test_tonal_value_tone4);
    RUN_TEST(test_tonal_value_tone5);
    RUN_TEST(test_tonal_value_tone6);
    RUN_TEST(test_tonal_value_tone7);
    RUN_TEST(test_tonal_value_tone8);
    RUN_TEST(test_tonal_value_tone9);
    RUN_TEST(test_tonal_value_tone10);
    RUN_TEST(test_tonal_value_tone11);
    RUN_TEST(test_tonal_value_tone12);
    RUN_TEST(test_tonal_value_tone13);

    /* lens_value_for_tone */
    RUN_TEST(test_lens_value_tone1);
    RUN_TEST(test_lens_value_tone4);
    RUN_TEST(test_lens_value_tone7);
    RUN_TEST(test_lens_value_tone10);
    RUN_TEST(test_lens_value_tone13);

    /* Boundary — invalid tones */
    RUN_TEST(test_tone0_returns_zeroed);
    RUN_TEST(test_tone14_returns_zeroed);
    RUN_TEST(test_negative_tone_returns_zeroed);
    RUN_TEST(test_tonal_value_tone0_returns_0);
    RUN_TEST(test_tonal_value_tone14_returns_0);
    RUN_TEST(test_tonal_value_negative_returns_0);
    RUN_TEST(test_lens_value_tone0_returns_0);
    RUN_TEST(test_lens_value_tone14_returns_0);
    RUN_TEST(test_lens_value_negative_returns_0);

    /* Boundary — invalid lens_by_number */
    RUN_TEST(test_lens0_returns_zeroed);
    RUN_TEST(test_lens5_returns_zeroed);
    RUN_TEST(test_lens_negative_returns_zeroed);

    /* Purity */
    RUN_TEST(test_purity_same_result);

    /* Sweep */
    RUN_TEST(test_all_lenses_have_valid_data);
    RUN_TEST(test_lens_values_unique);
    RUN_TEST(test_lens_values_multiples_of_36);
    RUN_TEST(test_108_is_lens1_value);

    return UNITY_END();
}
