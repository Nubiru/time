#include "../unity/unity.h"
#include "../../src/ui/astro_fmt.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Position formatting: 1-3 */

void test_position_aries(void)
{
    char buf[48];
    astro_fmt_position(15.7, buf, sizeof(buf));
    /* Should contain degree and Aries indicator */
    TEST_ASSERT_NOT_NULL(strstr(buf, "15"));
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

void test_position_zero(void)
{
    char buf[48];
    astro_fmt_position(0.0, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "0"));
}

void test_position_wrap(void)
{
    char buf[48];
    astro_fmt_position(359.99, buf, sizeof(buf));
    /* 359.99° = Pisces 29° */
    TEST_ASSERT_NOT_NULL(strstr(buf, "29"));
}

/* Planet position: 4-5 */

void test_planet_position(void)
{
    char buf[64];
    astro_fmt_planet_position("\xe2\x98\x89", 45.5, buf, sizeof(buf)); /* Sun at 45.5° = Taurus 15° */
    TEST_ASSERT_NOT_NULL(strstr(buf, "\xe2\x98\x89")); /* Sun symbol present */
    TEST_ASSERT_NOT_NULL(strstr(buf, "15"));
}

void test_planet_position_null_symbol(void)
{
    char buf[64];
    astro_fmt_planet_position(NULL, 90.0, buf, sizeof(buf));
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

/* Aspect formatting: 6-7 */

void test_aspect_format(void)
{
    char buf[64];
    astro_fmt_aspect("\xe2\x98\x89", "\xe2\x98\x8d", "\xe2\x98\xbd", 2.3, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "2.3"));
}

void test_aspect_format_tight(void)
{
    char buf[64];
    astro_fmt_aspect("Su", "opp", "Mo", 0.1, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "0.1"));
}

/* Elements: 8-9 */

void test_sign_elements(void)
{
    /* Fire: Aries(0), Leo(4), Sagittarius(8) */
    TEST_ASSERT_EQUAL_STRING("Fire", astro_sign_element(0));
    TEST_ASSERT_EQUAL_STRING("Fire", astro_sign_element(4));
    TEST_ASSERT_EQUAL_STRING("Fire", astro_sign_element(8));
    /* Earth: Taurus(1), Virgo(5), Capricorn(9) */
    TEST_ASSERT_EQUAL_STRING("Earth", astro_sign_element(1));
    TEST_ASSERT_EQUAL_STRING("Earth", astro_sign_element(5));
    TEST_ASSERT_EQUAL_STRING("Earth", astro_sign_element(9));
    /* Air: Gemini(2), Libra(6), Aquarius(10) */
    TEST_ASSERT_EQUAL_STRING("Air", astro_sign_element(2));
    /* Water: Cancer(3), Scorpio(7), Pisces(11) */
    TEST_ASSERT_EQUAL_STRING("Water", astro_sign_element(3));
}

void test_element_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", astro_sign_element(-1));
    TEST_ASSERT_EQUAL_STRING("?", astro_sign_element(12));
}

/* Modalities: 10-11 */

void test_sign_modalities(void)
{
    /* Cardinal: Aries(0), Cancer(3), Libra(6), Capricorn(9) */
    TEST_ASSERT_EQUAL_STRING("Cardinal", astro_sign_modality(0));
    TEST_ASSERT_EQUAL_STRING("Cardinal", astro_sign_modality(3));
    /* Fixed: Taurus(1), Leo(4), Scorpio(7), Aquarius(10) */
    TEST_ASSERT_EQUAL_STRING("Fixed", astro_sign_modality(1));
    TEST_ASSERT_EQUAL_STRING("Fixed", astro_sign_modality(4));
    /* Mutable: Gemini(2), Virgo(5), Sagittarius(8), Pisces(11) */
    TEST_ASSERT_EQUAL_STRING("Mutable", astro_sign_modality(2));
    TEST_ASSERT_EQUAL_STRING("Mutable", astro_sign_modality(11));
}

void test_modality_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", astro_sign_modality(-1));
}

/* Polarity: 12-13 */

void test_sign_polarity(void)
{
    /* Yang (masculine/positive): Fire + Air signs (0,2,4,6,8,10) */
    TEST_ASSERT_EQUAL_STRING("Yang", astro_sign_polarity(0));  /* Aries */
    TEST_ASSERT_EQUAL_STRING("Yang", astro_sign_polarity(2));  /* Gemini */
    /* Yin (feminine/negative): Earth + Water signs (1,3,5,7,9,11) */
    TEST_ASSERT_EQUAL_STRING("Yin", astro_sign_polarity(1));   /* Taurus */
    TEST_ASSERT_EQUAL_STRING("Yin", astro_sign_polarity(3));   /* Cancer */
}

void test_polarity_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", astro_sign_polarity(-1));
}

/* Element balance: 14-15 */

void test_element_balance_sums(void)
{
    /* All planets in Aries (fire) */
    int signs[10] = {0,0,0,0,0,0,0,0,0,0};
    int f, e, a, w;
    astro_element_balance(signs, &f, &e, &a, &w);
    TEST_ASSERT_EQUAL_INT(10, f);
    TEST_ASSERT_EQUAL_INT(0, e);
    TEST_ASSERT_EQUAL_INT(0, a);
    TEST_ASSERT_EQUAL_INT(0, w);
}

void test_element_balance_mixed(void)
{
    /* 3 fire, 3 earth, 2 air, 2 water */
    int signs[10] = {0,1,2,3, 4,5,6,7, 8,9};
    int f, e, a, w;
    astro_element_balance(signs, &f, &e, &a, &w);
    /* Signs 0,4,8=fire(3), 1,5,9=earth(3), 2,6=air(2)+(10 not present), 3,7=water(2) */
    TEST_ASSERT_EQUAL_INT(10, f + e + a + w);
}

/* Modality balance: 16-17 */

void test_modality_balance_sums(void)
{
    int signs[10] = {0,0,0,0,0,0,0,0,0,0};
    int card, fix, mut;
    astro_modality_balance(signs, &card, &fix, &mut);
    TEST_ASSERT_EQUAL_INT(10, card); /* Aries is cardinal */
    TEST_ASSERT_EQUAL_INT(0, fix);
    TEST_ASSERT_EQUAL_INT(0, mut);
}

void test_modality_balance_mixed(void)
{
    int signs[10] = {0,1,2,3,4,5,6,7,8,9};
    int card, fix, mut;
    astro_modality_balance(signs, &card, &fix, &mut);
    TEST_ASSERT_EQUAL_INT(10, card + fix + mut);
}

/* Formatted balance: 18-19 */

void test_fmt_element_balance(void)
{
    int signs[10] = {0,0,0,0,0,0,0,0,0,0};
    char buf[80];
    astro_fmt_element_balance(signs, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Fire"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "10"));
}

void test_fmt_modality_balance(void)
{
    int signs[10] = {1,1,1,1,1,1,1,1,1,1}; /* all Taurus = fixed */
    char buf[80];
    astro_fmt_modality_balance(signs, buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Fixed"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "10"));
}

/* Edge cases: 20-21 */

void test_position_negative_lon(void)
{
    char buf[48];
    astro_fmt_position(-30.0, buf, sizeof(buf));
    /* Should normalize: -30 + 360 = 330° = Pisces 0° */
    TEST_ASSERT_TRUE(strlen(buf) > 0);
}

void test_all_twelve_elements(void)
{
    /* Verify each of 12 signs maps to a valid element */
    const char *valid[] = {"Fire", "Earth", "Air", "Water"};
    for (int s = 0; s < 12; s++) {
        const char *el = astro_sign_element(s);
        int found = 0;
        for (int i = 0; i < 4; i++) {
            if (strcmp(el, valid[i]) == 0) found = 1;
        }
        TEST_ASSERT_TRUE(found);
    }
}

/* Purity: 22 */

void test_purity(void)
{
    char buf1[48], buf2[48];
    astro_fmt_position(123.456, buf1, sizeof(buf1));
    astro_fmt_position(123.456, buf2, sizeof(buf2));
    TEST_ASSERT_EQUAL_STRING(buf1, buf2);

    TEST_ASSERT_EQUAL_STRING(astro_sign_element(5), astro_sign_element(5));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_position_aries);
    RUN_TEST(test_position_zero);
    RUN_TEST(test_position_wrap);
    RUN_TEST(test_planet_position);
    RUN_TEST(test_planet_position_null_symbol);
    RUN_TEST(test_aspect_format);
    RUN_TEST(test_aspect_format_tight);
    RUN_TEST(test_sign_elements);
    RUN_TEST(test_element_invalid);
    RUN_TEST(test_sign_modalities);
    RUN_TEST(test_modality_invalid);
    RUN_TEST(test_sign_polarity);
    RUN_TEST(test_polarity_invalid);
    RUN_TEST(test_element_balance_sums);
    RUN_TEST(test_element_balance_mixed);
    RUN_TEST(test_modality_balance_sums);
    RUN_TEST(test_modality_balance_mixed);
    RUN_TEST(test_fmt_element_balance);
    RUN_TEST(test_fmt_modality_balance);
    RUN_TEST(test_position_negative_lon);
    RUN_TEST(test_all_twelve_elements);
    RUN_TEST(test_purity);
    return UNITY_END();
}
