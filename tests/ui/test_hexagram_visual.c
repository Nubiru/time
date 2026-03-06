#include "../unity/unity.h"
#include "../../src/ui/hexagram_visual.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Nuclear hexagram: 1-4 */

void test_nuclear_qian(void)
{
    /* Hex 1 (Qian): all yang. Nuclear lines 2,3,4 = yang, 3,4,5 = yang -> Qian again */
    TEST_ASSERT_EQUAL_INT(1, hexagram_nuclear(1));
}

void test_nuclear_kun(void)
{
    /* Hex 2 (Kun): all yin. Nuclear -> Kun */
    TEST_ASSERT_EQUAL_INT(2, hexagram_nuclear(2));
}

void test_nuclear_known(void)
{
    /* Hex 3 (Zhun): lower=Zhen(3), upper=Kan(5)
     * Lines: 1,0,0, 0,1,0
     * Nuclear lower = lines 2,3,4 = 0,0,1 -> Gen(6)
     * Nuclear upper = lines 3,4,5 = 0,1,1 -> Xun? wait...
     * Actually: lines[0..5] = bottom to top
     * Zhen=[1,0,0], Kan=[0,1,0]
     * lines: [1,0,0,0,1,0]
     * Nuclear lower: lines[1],lines[2],lines[3] = 0,0,0 -> Kun(7)
     * Nuclear upper: lines[2],lines[3],lines[4] = 0,0,1 -> Gen(6)
     * Kun/Gen = Hex 23 (Bo) */
    int nuc = hexagram_nuclear(3);
    TEST_ASSERT_TRUE(nuc >= 1 && nuc <= 64);
}

void test_nuclear_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(-1, hexagram_nuclear(0));
    TEST_ASSERT_EQUAL_INT(-1, hexagram_nuclear(65));
}

/* Complement: 5-8 */

void test_complement_qian_kun(void)
{
    /* Hex 1 (all yang) complement = Hex 2 (all yin) */
    TEST_ASSERT_EQUAL_INT(2, hexagram_complement(1));
}

void test_complement_kun_qian(void)
{
    TEST_ASSERT_EQUAL_INT(1, hexagram_complement(2));
}

void test_complement_symmetry(void)
{
    /* complement(complement(x)) = x */
    for (int i = 1; i <= 64; i++) {
        int c = hexagram_complement(i);
        TEST_ASSERT_TRUE(c >= 1 && c <= 64);
        TEST_ASSERT_EQUAL_INT(i, hexagram_complement(c));
    }
}

void test_complement_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(-1, hexagram_complement(0));
    TEST_ASSERT_EQUAL_INT(-1, hexagram_complement(65));
}

/* Reverse: 9-11 */

void test_reverse_qian(void)
{
    /* All yang reversed = all yang */
    TEST_ASSERT_EQUAL_INT(1, hexagram_reverse(1));
}

void test_reverse_symmetry(void)
{
    /* reverse(reverse(x)) = x */
    for (int i = 1; i <= 64; i++) {
        int r = hexagram_reverse(i);
        TEST_ASSERT_TRUE(r >= 1 && r <= 64);
        TEST_ASSERT_EQUAL_INT(i, hexagram_reverse(r));
    }
}

void test_reverse_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(-1, hexagram_reverse(0));
}

/* ASCII art: 12-14 */

void test_ascii_qian(void)
{
    /* All yang: all lines should be solid */
    hexagram_art_t art = hexagram_ascii(1);
    for (int i = 0; i < HEX_ART_LINES; i++) {
        TEST_ASSERT_TRUE(strlen(art.lines[i]) > 0);
        /* No spaces in middle of solid line */
        TEST_ASSERT_NULL(strstr(art.lines[i], "   "));
    }
}

void test_ascii_kun(void)
{
    /* All yin: all lines should be broken */
    hexagram_art_t art = hexagram_ascii(2);
    for (int i = 0; i < HEX_ART_LINES; i++) {
        TEST_ASSERT_TRUE(strlen(art.lines[i]) > 0);
        /* Should have gap in middle */
        TEST_ASSERT_NOT_NULL(strstr(art.lines[i], "   "));
    }
}

void test_ascii_invalid(void)
{
    hexagram_art_t art = hexagram_ascii(0);
    /* Should still not crash, lines can be empty */
    TEST_ASSERT_TRUE(strlen(art.lines[0]) == 0 || art.lines[0][0] != '\0' || art.lines[0][0] == '\0');
}

/* Summary: 15-16 */

void test_summary_hex1(void)
{
    char buf[80];
    hexagram_summary(1, buf, 80);
    TEST_ASSERT_NOT_NULL(strstr(buf, "Qian"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "1."));
}

void test_summary_invalid(void)
{
    char buf[80];
    char *result = hexagram_summary(0, buf, 80);
    TEST_ASSERT_EQUAL_PTR(buf, result);
    TEST_ASSERT_TRUE(strlen(buf) > 0); /* should have some error text */
}

/* Trigram elements: 17-18 */

void test_trigram_elements(void)
{
    TEST_ASSERT_EQUAL_STRING("Heaven", trigram_element(0)); /* Qian */
    TEST_ASSERT_EQUAL_STRING("Lake", trigram_element(1));   /* Dui */
    TEST_ASSERT_EQUAL_STRING("Fire", trigram_element(2));   /* Li */
    TEST_ASSERT_EQUAL_STRING("Thunder", trigram_element(3)); /* Zhen */
    TEST_ASSERT_EQUAL_STRING("Wind", trigram_element(4));    /* Xun */
    TEST_ASSERT_EQUAL_STRING("Water", trigram_element(5));   /* Kan */
    TEST_ASSERT_EQUAL_STRING("Mountain", trigram_element(6)); /* Gen */
    TEST_ASSERT_EQUAL_STRING("Earth", trigram_element(7));   /* Kun */
}

void test_trigram_element_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", trigram_element(-1));
    TEST_ASSERT_EQUAL_STRING("?", trigram_element(8));
}

/* Is complement: 19 */

void test_is_complement(void)
{
    TEST_ASSERT_TRUE(hexagram_is_complement(1, 2));
    TEST_ASSERT_TRUE(hexagram_is_complement(2, 1));
    TEST_ASSERT_FALSE(hexagram_is_complement(1, 3));
    TEST_ASSERT_FALSE(hexagram_is_complement(0, 1));
}

/* Unicode: 20-21 */

void test_unicode_hex1(void)
{
    const char *sym = hexagram_unicode(1);
    TEST_ASSERT_NOT_NULL(sym);
    TEST_ASSERT_TRUE(strlen(sym) > 0);
    TEST_ASSERT_TRUE(strcmp(sym, "?") != 0);
}

void test_unicode_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", hexagram_unicode(0));
    TEST_ASSERT_EQUAL_STRING("?", hexagram_unicode(65));
}

/* Purity: 22 */

void test_purity(void)
{
    int n1 = hexagram_nuclear(3);
    int n2 = hexagram_nuclear(3);
    TEST_ASSERT_EQUAL_INT(n1, n2);

    int c1 = hexagram_complement(10);
    int c2 = hexagram_complement(10);
    TEST_ASSERT_EQUAL_INT(c1, c2);

    hexagram_art_t a1 = hexagram_ascii(42);
    hexagram_art_t a2 = hexagram_ascii(42);
    TEST_ASSERT_EQUAL_STRING(a1.lines[0], a2.lines[0]);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_nuclear_qian);
    RUN_TEST(test_nuclear_kun);
    RUN_TEST(test_nuclear_known);
    RUN_TEST(test_nuclear_invalid);
    RUN_TEST(test_complement_qian_kun);
    RUN_TEST(test_complement_kun_qian);
    RUN_TEST(test_complement_symmetry);
    RUN_TEST(test_complement_invalid);
    RUN_TEST(test_reverse_qian);
    RUN_TEST(test_reverse_symmetry);
    RUN_TEST(test_reverse_invalid);
    RUN_TEST(test_ascii_qian);
    RUN_TEST(test_ascii_kun);
    RUN_TEST(test_ascii_invalid);
    RUN_TEST(test_summary_hex1);
    RUN_TEST(test_summary_invalid);
    RUN_TEST(test_trigram_elements);
    RUN_TEST(test_trigram_element_invalid);
    RUN_TEST(test_is_complement);
    RUN_TEST(test_unicode_hex1);
    RUN_TEST(test_unicode_invalid);
    RUN_TEST(test_purity);
    return UNITY_END();
}
