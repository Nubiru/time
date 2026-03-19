/* test_personal_resonance.c — Tests for the Personal Resonance module.
 * Validates pr_detect, pr_default_input, pr_type_name, pr_type_count
 * across all resonance detection paths: kin match, seal match, tone match,
 * oracle matches, wavespell, hexagram, hebrew month, chinese, zodiac.
 * Also tests unknown-data skipping, multi-match sorting, and edge cases. */

#include "unity.h"
#include "systems/unified/personal_resonance.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* === Helper: build input with no matches === */
static pr_input_t make_no_match_input(void)
{
    pr_input_t in = pr_default_input();
    /* Birth data */
    in.birth_kin         = 1;
    in.birth_seal        = 0;
    in.birth_tone        = 1;
    in.birth_guide_kin   = 2;
    in.birth_analog_kin  = 3;
    in.birth_antipode_kin = 4;
    in.birth_occult_kin  = 5;
    in.birth_wavespell   = 1;
    in.birth_hexagram    = 1;
    in.birth_hebrew_month = 1;
    in.birth_chinese_animal = 0;
    in.birth_chinese_element = 0;
    in.birth_sun_sign    = 0;
    /* Today: all different */
    in.today_kin         = 100;
    in.today_seal        = 10;
    in.today_tone        = 7;
    in.today_wavespell   = 10;
    in.today_hexagram    = 32;
    in.today_hebrew_month = 7;
    in.today_chinese_animal = 6;
    in.today_chinese_element = 3;
    in.today_sun_sign    = 6;
    return in;
}

/* === pr_default_input tests === */

static void test_default_input_birth_fields_zero(void)
{
    pr_input_t in = pr_default_input();
    TEST_ASSERT_EQUAL_INT(0, in.birth_kin);
    TEST_ASSERT_EQUAL_INT(-1, in.birth_seal);
    TEST_ASSERT_EQUAL_INT(0, in.birth_tone);
    TEST_ASSERT_EQUAL_INT(0, in.birth_guide_kin);
    TEST_ASSERT_EQUAL_INT(0, in.birth_analog_kin);
    TEST_ASSERT_EQUAL_INT(0, in.birth_antipode_kin);
    TEST_ASSERT_EQUAL_INT(0, in.birth_occult_kin);
    TEST_ASSERT_EQUAL_INT(0, in.birth_wavespell);
    TEST_ASSERT_EQUAL_INT(0, in.birth_hexagram);
    TEST_ASSERT_EQUAL_INT(0, in.birth_hebrew_month);
    TEST_ASSERT_EQUAL_INT(-1, in.birth_chinese_animal);
    TEST_ASSERT_EQUAL_INT(-1, in.birth_chinese_element);
    TEST_ASSERT_EQUAL_INT(-1, in.birth_sun_sign);
}

static void test_default_input_today_fields_zero(void)
{
    pr_input_t in = pr_default_input();
    TEST_ASSERT_EQUAL_INT(0, in.today_kin);
    TEST_ASSERT_EQUAL_INT(0, in.today_seal);
    TEST_ASSERT_EQUAL_INT(0, in.today_tone);
    TEST_ASSERT_EQUAL_INT(0, in.today_wavespell);
    TEST_ASSERT_EQUAL_INT(0, in.today_hexagram);
    TEST_ASSERT_EQUAL_INT(0, in.today_hebrew_month);
    TEST_ASSERT_EQUAL_INT(0, in.today_chinese_animal);
    TEST_ASSERT_EQUAL_INT(0, in.today_chinese_element);
    TEST_ASSERT_EQUAL_INT(0, in.today_sun_sign);
}

/* === pr_detect with NULL === */

static void test_detect_null_returns_empty(void)
{
    pr_result_t r = pr_detect(NULL);
    TEST_ASSERT_EQUAL_INT(0, r.count);
    TEST_ASSERT_EQUAL_INT(0, r.strongest);
}

/* === pr_detect with no matches === */

static void test_detect_no_matches(void)
{
    pr_input_t in = make_no_match_input();
    pr_result_t r = pr_detect(&in);
    TEST_ASSERT_EQUAL_INT(0, r.count);
    TEST_ASSERT_EQUAL_INT(0, r.strongest);
}

/* === Kin match (strength 5) === */

static void test_detect_kin_match(void)
{
    pr_input_t in = make_no_match_input();
    in.birth_kin = 42;
    in.today_kin = 42;
    /* Seal/tone will also match if kin matches, but let's set them different
     * to isolate kin. Actually if kin matches, seal and tone likely match too.
     * Set seal/tone to match as they would in real life. */
    in.birth_seal = 1;
    in.today_seal = 1;
    in.birth_tone = 3;
    in.today_tone = 3;
    in.birth_wavespell = 4;
    in.today_wavespell = 4;

    pr_result_t r = pr_detect(&in);
    /* Should have kin match + seal match + tone match + wavespell match */
    TEST_ASSERT_GREATER_OR_EQUAL(1, r.count);
    TEST_ASSERT_EQUAL_INT(5, r.strongest);

    /* First echo (sorted by strength desc) should be kin match */
    TEST_ASSERT_EQUAL_INT(PR_TYPE_KIN_MATCH, r.echoes[0].type);
    TEST_ASSERT_EQUAL_INT(5, r.echoes[0].strength);
    TEST_ASSERT_NOT_NULL(strstr(r.echoes[0].description, "galactic birthday"));
    TEST_ASSERT_EQUAL_STRING("Tzolkin", r.echoes[0].system);
}

/* === Seal match (strength 2) === */

static void test_detect_seal_match(void)
{
    pr_input_t in = make_no_match_input();
    in.birth_seal = 7;   /* Hand */
    in.today_seal = 7;

    pr_result_t r = pr_detect(&in);
    /* Find the seal match echo */
    int found = 0;
    for (int i = 0; i < r.count; i++) {
        if (r.echoes[i].type == PR_TYPE_SEAL_MATCH) {
            TEST_ASSERT_EQUAL_INT(2, r.echoes[i].strength);
            TEST_ASSERT_EQUAL_STRING("Tzolkin", r.echoes[i].system);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE_MESSAGE(found, "SEAL_MATCH echo not found");
}

/* === Tone match (strength 2) === */

static void test_detect_tone_match(void)
{
    pr_input_t in = make_no_match_input();
    in.birth_tone = 9;
    in.today_tone = 9;

    pr_result_t r = pr_detect(&in);
    int found = 0;
    for (int i = 0; i < r.count; i++) {
        if (r.echoes[i].type == PR_TYPE_TONE_MATCH) {
            TEST_ASSERT_EQUAL_INT(2, r.echoes[i].strength);
            TEST_ASSERT_EQUAL_STRING("Tzolkin", r.echoes[i].system);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE_MESSAGE(found, "TONE_MATCH echo not found");
}

/* === Hexagram match (strength 4) === */

static void test_detect_hexagram_match(void)
{
    pr_input_t in = make_no_match_input();
    in.birth_hexagram = 23;
    in.today_hexagram = 23;

    pr_result_t r = pr_detect(&in);
    int found = 0;
    for (int i = 0; i < r.count; i++) {
        if (r.echoes[i].type == PR_TYPE_HEXAGRAM_MATCH) {
            TEST_ASSERT_EQUAL_INT(4, r.echoes[i].strength);
            TEST_ASSERT_EQUAL_STRING("I Ching", r.echoes[i].system);
            TEST_ASSERT_NOT_NULL(strstr(r.echoes[i].description, "#23"));
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE_MESSAGE(found, "HEXAGRAM_MATCH echo not found");
}

/* === Oracle match: guide (strength 3) === */

static void test_detect_oracle_guide_match(void)
{
    pr_input_t in = make_no_match_input();
    in.birth_guide_kin = 50;
    in.today_kin = 50;

    pr_result_t r = pr_detect(&in);
    int found = 0;
    for (int i = 0; i < r.count; i++) {
        if (r.echoes[i].type == PR_TYPE_ORACLE_MATCH) {
            TEST_ASSERT_EQUAL_INT(3, r.echoes[i].strength);
            TEST_ASSERT_NOT_NULL(strstr(r.echoes[i].description, "guide"));
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE_MESSAGE(found, "ORACLE_MATCH (guide) not found");
}

/* === Oracle match: analog (strength 3) === */

static void test_detect_oracle_analog_match(void)
{
    pr_input_t in = make_no_match_input();
    in.birth_analog_kin = 50;
    in.today_kin = 50;

    pr_result_t r = pr_detect(&in);
    int found = 0;
    for (int i = 0; i < r.count; i++) {
        if (r.echoes[i].type == PR_TYPE_ORACLE_MATCH) {
            TEST_ASSERT_EQUAL_INT(3, r.echoes[i].strength);
            TEST_ASSERT_NOT_NULL(strstr(r.echoes[i].description, "partner"));
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE_MESSAGE(found, "ORACLE_MATCH (analog) not found");
}

/* === Oracle match: antipode (strength 3) === */

static void test_detect_oracle_antipode_match(void)
{
    pr_input_t in = make_no_match_input();
    in.birth_antipode_kin = 50;
    in.today_kin = 50;

    pr_result_t r = pr_detect(&in);
    int found = 0;
    for (int i = 0; i < r.count; i++) {
        if (r.echoes[i].type == PR_TYPE_ORACLE_MATCH) {
            TEST_ASSERT_EQUAL_INT(3, r.echoes[i].strength);
            TEST_ASSERT_NOT_NULL(strstr(r.echoes[i].description, "challenge"));
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE_MESSAGE(found, "ORACLE_MATCH (antipode) not found");
}

/* === Oracle match: occult (strength 3) === */

static void test_detect_oracle_occult_match(void)
{
    pr_input_t in = make_no_match_input();
    in.birth_occult_kin = 50;
    in.today_kin = 50;

    pr_result_t r = pr_detect(&in);
    int found = 0;
    for (int i = 0; i < r.count; i++) {
        if (r.echoes[i].type == PR_TYPE_ORACLE_MATCH) {
            TEST_ASSERT_EQUAL_INT(3, r.echoes[i].strength);
            TEST_ASSERT_NOT_NULL(strstr(r.echoes[i].description, "hidden"));
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE_MESSAGE(found, "ORACLE_MATCH (occult) not found");
}

/* === Wavespell match (strength 2) === */

static void test_detect_wavespell_match(void)
{
    pr_input_t in = make_no_match_input();
    in.birth_wavespell = 5;
    in.today_wavespell = 5;

    pr_result_t r = pr_detect(&in);
    int found = 0;
    for (int i = 0; i < r.count; i++) {
        if (r.echoes[i].type == PR_TYPE_WAVESPELL_MATCH) {
            TEST_ASSERT_EQUAL_INT(2, r.echoes[i].strength);
            TEST_ASSERT_EQUAL_STRING("Tzolkin", r.echoes[i].system);
            TEST_ASSERT_NOT_NULL(strstr(r.echoes[i].description, "wavespell"));
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE_MESSAGE(found, "WAVESPELL_MATCH echo not found");
}

/* === Hebrew month match (strength 2) === */

static void test_detect_hebrew_month_match(void)
{
    pr_input_t in = make_no_match_input();
    in.birth_hebrew_month = 10;
    in.today_hebrew_month = 10;

    pr_result_t r = pr_detect(&in);
    int found = 0;
    for (int i = 0; i < r.count; i++) {
        if (r.echoes[i].type == PR_TYPE_HEBREW_MONTH_MATCH) {
            TEST_ASSERT_EQUAL_INT(2, r.echoes[i].strength);
            TEST_ASSERT_EQUAL_STRING("Hebrew", r.echoes[i].system);
            TEST_ASSERT_NOT_NULL(strstr(r.echoes[i].description, "Hebrew birth month"));
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE_MESSAGE(found, "HEBREW_MONTH_MATCH echo not found");
}

/* === Chinese animal match (strength 1) === */

static void test_detect_chinese_animal_match(void)
{
    pr_input_t in = make_no_match_input();
    in.birth_chinese_animal = 3;
    in.today_chinese_animal = 3;

    pr_result_t r = pr_detect(&in);
    int found = 0;
    for (int i = 0; i < r.count; i++) {
        if (r.echoes[i].type == PR_TYPE_CHINESE_MATCH) {
            TEST_ASSERT_EQUAL_INT(1, r.echoes[i].strength);
            TEST_ASSERT_EQUAL_STRING("Chinese", r.echoes[i].system);
            TEST_ASSERT_NOT_NULL(strstr(r.echoes[i].description, "birth animal"));
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE_MESSAGE(found, "CHINESE_MATCH echo not found");
}

/* === Zodiac match (strength 1) === */

static void test_detect_zodiac_match(void)
{
    pr_input_t in = make_no_match_input();
    in.birth_sun_sign = 4;
    in.today_sun_sign = 4;

    pr_result_t r = pr_detect(&in);
    int found = 0;
    for (int i = 0; i < r.count; i++) {
        if (r.echoes[i].type == PR_TYPE_ZODIAC_MATCH) {
            TEST_ASSERT_EQUAL_INT(1, r.echoes[i].strength);
            TEST_ASSERT_EQUAL_STRING("Astrology", r.echoes[i].system);
            TEST_ASSERT_NOT_NULL(strstr(r.echoes[i].description, "birth sign"));
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE_MESSAGE(found, "ZODIAC_MATCH echo not found");
}

/* === Multiple matches sorted by strength descending === */

static void test_detect_multiple_sorted_by_strength(void)
{
    pr_input_t in = make_no_match_input();
    /* Set up: zodiac(1) + tone(2) + hexagram(4) */
    in.birth_sun_sign = 3;
    in.today_sun_sign = 3;
    in.birth_tone = 5;
    in.today_tone = 5;
    in.birth_hexagram = 11;
    in.today_hexagram = 11;

    pr_result_t r = pr_detect(&in);
    TEST_ASSERT_EQUAL_INT(3, r.count);
    TEST_ASSERT_EQUAL_INT(4, r.strongest);

    /* Should be sorted: hexagram(4), tone(2), zodiac(1) */
    TEST_ASSERT_EQUAL_INT(PR_TYPE_HEXAGRAM_MATCH, r.echoes[0].type);
    TEST_ASSERT_EQUAL_INT(4, r.echoes[0].strength);
    TEST_ASSERT_TRUE(r.echoes[1].strength >= r.echoes[2].strength);
}

/* === Unknown birth data skipping === */

static void test_detect_skips_unknown_kin(void)
{
    pr_input_t in = make_no_match_input();
    in.birth_kin = 0;      /* unknown */
    in.today_kin = 100;

    pr_result_t r = pr_detect(&in);
    for (int i = 0; i < r.count; i++) {
        TEST_ASSERT_NOT_EQUAL(PR_TYPE_KIN_MATCH, r.echoes[i].type);
    }
}

static void test_detect_skips_unknown_seal(void)
{
    pr_input_t in = make_no_match_input();
    in.birth_seal = -1;    /* unknown */
    in.today_seal = 5;

    pr_result_t r = pr_detect(&in);
    for (int i = 0; i < r.count; i++) {
        TEST_ASSERT_NOT_EQUAL(PR_TYPE_SEAL_MATCH, r.echoes[i].type);
    }
}

static void test_detect_skips_unknown_tone(void)
{
    pr_input_t in = make_no_match_input();
    in.birth_tone = 0;     /* unknown */
    in.today_tone = 7;

    pr_result_t r = pr_detect(&in);
    for (int i = 0; i < r.count; i++) {
        TEST_ASSERT_NOT_EQUAL(PR_TYPE_TONE_MATCH, r.echoes[i].type);
    }
}

static void test_detect_skips_unknown_hexagram(void)
{
    pr_input_t in = make_no_match_input();
    in.birth_hexagram = 0;
    in.today_hexagram = 23;

    pr_result_t r = pr_detect(&in);
    for (int i = 0; i < r.count; i++) {
        TEST_ASSERT_NOT_EQUAL(PR_TYPE_HEXAGRAM_MATCH, r.echoes[i].type);
    }
}

static void test_detect_skips_unknown_chinese(void)
{
    pr_input_t in = make_no_match_input();
    in.birth_chinese_animal = -1;  /* unknown */
    in.today_chinese_animal = 3;

    pr_result_t r = pr_detect(&in);
    for (int i = 0; i < r.count; i++) {
        TEST_ASSERT_NOT_EQUAL(PR_TYPE_CHINESE_MATCH, r.echoes[i].type);
    }
}

static void test_detect_skips_unknown_zodiac(void)
{
    pr_input_t in = make_no_match_input();
    in.birth_sun_sign = -1;  /* unknown */
    in.today_sun_sign = 4;

    pr_result_t r = pr_detect(&in);
    for (int i = 0; i < r.count; i++) {
        TEST_ASSERT_NOT_EQUAL(PR_TYPE_ZODIAC_MATCH, r.echoes[i].type);
    }
}

/* === Strongest field correct === */

static void test_detect_strongest_field(void)
{
    pr_input_t in = make_no_match_input();
    /* Seal (2) + hexagram (4) */
    in.birth_seal = 3;
    in.today_seal = 3;
    in.birth_hexagram = 10;
    in.today_hexagram = 10;

    pr_result_t r = pr_detect(&in);
    TEST_ASSERT_EQUAL_INT(4, r.strongest);
}

static void test_detect_strongest_zero_when_empty(void)
{
    pr_input_t in = make_no_match_input();
    pr_result_t r = pr_detect(&in);
    TEST_ASSERT_EQUAL_INT(0, r.strongest);
}

/* === pr_type_name tests === */

static void test_type_name_seal_match(void)
{
    TEST_ASSERT_EQUAL_STRING("Seal Match", pr_type_name(PR_TYPE_SEAL_MATCH));
}

static void test_type_name_tone_match(void)
{
    TEST_ASSERT_EQUAL_STRING("Tone Match", pr_type_name(PR_TYPE_TONE_MATCH));
}

static void test_type_name_kin_match(void)
{
    TEST_ASSERT_EQUAL_STRING("Kin Match", pr_type_name(PR_TYPE_KIN_MATCH));
}

static void test_type_name_oracle_match(void)
{
    TEST_ASSERT_EQUAL_STRING("Oracle Match", pr_type_name(PR_TYPE_ORACLE_MATCH));
}

static void test_type_name_wavespell_match(void)
{
    TEST_ASSERT_EQUAL_STRING("Wavespell Match", pr_type_name(PR_TYPE_WAVESPELL_MATCH));
}

static void test_type_name_hexagram_match(void)
{
    TEST_ASSERT_EQUAL_STRING("Hexagram Match", pr_type_name(PR_TYPE_HEXAGRAM_MATCH));
}

static void test_type_name_hebrew_month_match(void)
{
    TEST_ASSERT_EQUAL_STRING("Hebrew Month Match", pr_type_name(PR_TYPE_HEBREW_MONTH_MATCH));
}

static void test_type_name_chinese_match(void)
{
    TEST_ASSERT_EQUAL_STRING("Chinese Match", pr_type_name(PR_TYPE_CHINESE_MATCH));
}

static void test_type_name_zodiac_match(void)
{
    TEST_ASSERT_EQUAL_STRING("Zodiac Match", pr_type_name(PR_TYPE_ZODIAC_MATCH));
}

static void test_type_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", pr_type_name(PR_TYPE_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", pr_type_name((pr_type_t)99));
    TEST_ASSERT_EQUAL_STRING("?", pr_type_name((pr_type_t)-1));
}

/* === pr_type_count === */

static void test_type_count(void)
{
    TEST_ASSERT_EQUAL_INT(9, pr_type_count());
}

/* === Count does not exceed PR_MAX_ECHOES === */

static void test_detect_max_echoes_respected(void)
{
    pr_input_t in = pr_default_input();
    /* Set everything to match */
    in.birth_kin = 42;
    in.birth_seal = 1;
    in.birth_tone = 3;
    in.birth_guide_kin = 42;   /* same as today_kin -> oracle */
    in.birth_analog_kin = 42;
    in.birth_antipode_kin = 42;
    in.birth_occult_kin = 42;
    in.birth_wavespell = 4;
    in.birth_hexagram = 11;
    in.birth_hebrew_month = 5;
    in.birth_chinese_animal = 2;
    in.birth_chinese_element = 1;
    in.birth_sun_sign = 3;

    in.today_kin = 42;
    in.today_seal = 1;
    in.today_tone = 3;
    in.today_wavespell = 4;
    in.today_hexagram = 11;
    in.today_hebrew_month = 5;
    in.today_chinese_animal = 2;
    in.today_chinese_element = 1;
    in.today_sun_sign = 3;

    pr_result_t r = pr_detect(&in);
    TEST_ASSERT_LESS_OR_EQUAL(PR_MAX_ECHOES, r.count);
}

/* === Oracle skipped when guide_kin is 0 (unknown) === */

static void test_detect_skips_unknown_oracle(void)
{
    pr_input_t in = make_no_match_input();
    in.birth_guide_kin = 0;    /* unknown */
    in.birth_analog_kin = 0;
    in.birth_antipode_kin = 0;
    in.birth_occult_kin = 0;
    in.today_kin = 50;

    pr_result_t r = pr_detect(&in);
    for (int i = 0; i < r.count; i++) {
        TEST_ASSERT_NOT_EQUAL(PR_TYPE_ORACLE_MATCH, r.echoes[i].type);
    }
}

/* === Chinese element match also triggers CHINESE_MATCH === */

static void test_detect_chinese_element_match(void)
{
    pr_input_t in = make_no_match_input();
    in.birth_chinese_animal = 0;  /* different from today */
    in.today_chinese_animal = 6;
    in.birth_chinese_element = 2;
    in.today_chinese_element = 2;

    pr_result_t r = pr_detect(&in);
    int found = 0;
    for (int i = 0; i < r.count; i++) {
        if (r.echoes[i].type == PR_TYPE_CHINESE_MATCH) {
            TEST_ASSERT_EQUAL_INT(1, r.echoes[i].strength);
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE_MESSAGE(found, "CHINESE_MATCH (element) echo not found");
}

/* === All birth unknown => no echoes === */

static void test_detect_all_unknown_no_echoes(void)
{
    pr_input_t in = pr_default_input();
    /* Today fields set but birth all default (unknown) */
    in.today_kin = 100;
    in.today_seal = 5;
    in.today_tone = 7;
    in.today_wavespell = 8;
    in.today_hexagram = 32;
    in.today_hebrew_month = 7;
    in.today_chinese_animal = 6;
    in.today_chinese_element = 3;
    in.today_sun_sign = 6;

    pr_result_t r = pr_detect(&in);
    TEST_ASSERT_EQUAL_INT(0, r.count);
    TEST_ASSERT_EQUAL_INT(0, r.strongest);
}

int main(void)
{
    UNITY_BEGIN();

    /* Default input */
    RUN_TEST(test_default_input_birth_fields_zero);
    RUN_TEST(test_default_input_today_fields_zero);

    /* NULL input */
    RUN_TEST(test_detect_null_returns_empty);

    /* No matches */
    RUN_TEST(test_detect_no_matches);

    /* Individual match types */
    RUN_TEST(test_detect_kin_match);
    RUN_TEST(test_detect_seal_match);
    RUN_TEST(test_detect_tone_match);
    RUN_TEST(test_detect_hexagram_match);
    RUN_TEST(test_detect_oracle_guide_match);
    RUN_TEST(test_detect_oracle_analog_match);
    RUN_TEST(test_detect_oracle_antipode_match);
    RUN_TEST(test_detect_oracle_occult_match);
    RUN_TEST(test_detect_wavespell_match);
    RUN_TEST(test_detect_hebrew_month_match);
    RUN_TEST(test_detect_chinese_animal_match);
    RUN_TEST(test_detect_zodiac_match);

    /* Multiple + sorting */
    RUN_TEST(test_detect_multiple_sorted_by_strength);

    /* Unknown data skipping */
    RUN_TEST(test_detect_skips_unknown_kin);
    RUN_TEST(test_detect_skips_unknown_seal);
    RUN_TEST(test_detect_skips_unknown_tone);
    RUN_TEST(test_detect_skips_unknown_hexagram);
    RUN_TEST(test_detect_skips_unknown_chinese);
    RUN_TEST(test_detect_skips_unknown_zodiac);
    RUN_TEST(test_detect_skips_unknown_oracle);

    /* Strongest field */
    RUN_TEST(test_detect_strongest_field);
    RUN_TEST(test_detect_strongest_zero_when_empty);

    /* Type name */
    RUN_TEST(test_type_name_seal_match);
    RUN_TEST(test_type_name_tone_match);
    RUN_TEST(test_type_name_kin_match);
    RUN_TEST(test_type_name_oracle_match);
    RUN_TEST(test_type_name_wavespell_match);
    RUN_TEST(test_type_name_hexagram_match);
    RUN_TEST(test_type_name_hebrew_month_match);
    RUN_TEST(test_type_name_chinese_match);
    RUN_TEST(test_type_name_zodiac_match);
    RUN_TEST(test_type_name_invalid);

    /* Type count */
    RUN_TEST(test_type_count);

    /* Max echoes */
    RUN_TEST(test_detect_max_echoes_respected);

    /* Chinese element match */
    RUN_TEST(test_detect_chinese_element_match);

    /* All unknown */
    RUN_TEST(test_detect_all_unknown_no_echoes);

    return UNITY_END();
}
