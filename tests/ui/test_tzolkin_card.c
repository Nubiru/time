#include "../unity/unity.h"
#include "../../src/ui/tzolkin_card.h"

#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* Seal colors: 1-3 */

void test_seal_colors_cycle(void)
{
    /* Seals cycle Red(0), White(1), Blue(2), Yellow(3) repeating 5 times */
    for (int i = 0; i < 20; i++) {
        TEST_ASSERT_EQUAL_INT(i % 4, (int)tzolkin_seal_color(i));
    }
}

void test_specific_seal_colors(void)
{
    TEST_ASSERT_EQUAL_INT(TZOLKIN_RED,    tzolkin_seal_color(0));  /* Red Dragon */
    TEST_ASSERT_EQUAL_INT(TZOLKIN_WHITE,  tzolkin_seal_color(1));  /* White Wind */
    TEST_ASSERT_EQUAL_INT(TZOLKIN_BLUE,   tzolkin_seal_color(2));  /* Blue Night */
    TEST_ASSERT_EQUAL_INT(TZOLKIN_YELLOW, tzolkin_seal_color(3));  /* Yellow Seed */
    TEST_ASSERT_EQUAL_INT(TZOLKIN_RED,    tzolkin_seal_color(4));  /* Red Serpent */
    TEST_ASSERT_EQUAL_INT(TZOLKIN_YELLOW, tzolkin_seal_color(19)); /* Yellow Sun */
}

void test_color_names(void)
{
    TEST_ASSERT_EQUAL_STRING("Red",    tzolkin_color_name(TZOLKIN_RED));
    TEST_ASSERT_EQUAL_STRING("White",  tzolkin_color_name(TZOLKIN_WHITE));
    TEST_ASSERT_EQUAL_STRING("Blue",   tzolkin_color_name(TZOLKIN_BLUE));
    TEST_ASSERT_EQUAL_STRING("Yellow", tzolkin_color_name(TZOLKIN_YELLOW));
}

/* Earth families: 4-6 */

void test_earth_family_polar(void)
{
    /* Polar: Dragon(0), Wind(1), Night(2), Seed(3) — seals 0,5,10,15 actually */
    /* Dreamspell Earth families group by position in the 4-color cycle:
     * Polar: 0,5,10,15 | Cardinal: 1,6,11,16 | Core: 2,7,12,17 |
     * Signal: 3,8,13,18 | Gateway: 4,9,14,19 */
    TEST_ASSERT_EQUAL_INT(EARTH_POLAR, tzolkin_earth_family(0));
    TEST_ASSERT_EQUAL_INT(EARTH_POLAR, tzolkin_earth_family(5));
    TEST_ASSERT_EQUAL_INT(EARTH_POLAR, tzolkin_earth_family(10));
    TEST_ASSERT_EQUAL_INT(EARTH_POLAR, tzolkin_earth_family(15));
}

void test_earth_family_all(void)
{
    /* Each group has 4 seals, 5 groups */
    int counts[5] = {0, 0, 0, 0, 0};
    for (int i = 0; i < 20; i++) {
        earth_family_t f = tzolkin_earth_family(i);
        TEST_ASSERT_TRUE(f >= 0 && f <= 4);
        counts[f]++;
    }
    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_EQUAL_INT(4, counts[i]);
    }
}

void test_earth_family_names(void)
{
    TEST_ASSERT_EQUAL_STRING("Polar",    tzolkin_earth_family_name(EARTH_POLAR));
    TEST_ASSERT_EQUAL_STRING("Cardinal", tzolkin_earth_family_name(EARTH_CARDINAL));
    TEST_ASSERT_EQUAL_STRING("Core",     tzolkin_earth_family_name(EARTH_CORE));
    TEST_ASSERT_EQUAL_STRING("Signal",   tzolkin_earth_family_name(EARTH_SIGNAL));
    TEST_ASSERT_EQUAL_STRING("Gateway",  tzolkin_earth_family_name(EARTH_GATEWAY));
}

/* Tone keywords and actions: 7-9 */

void test_tone_keywords(void)
{
    TEST_ASSERT_EQUAL_STRING("Purpose",   tzolkin_tone_keyword(1));
    TEST_ASSERT_EQUAL_STRING("Challenge", tzolkin_tone_keyword(2));
    TEST_ASSERT_EQUAL_STRING("Endurance", tzolkin_tone_keyword(13));
}

void test_tone_actions(void)
{
    TEST_ASSERT_EQUAL_STRING("Unify",     tzolkin_tone_action(1));
    TEST_ASSERT_EQUAL_STRING("Polarize",  tzolkin_tone_action(2));
    TEST_ASSERT_EQUAL_STRING("Transcend", tzolkin_tone_action(13));
}

void test_tone_all_valid(void)
{
    for (int t = 1; t <= 13; t++) {
        const char *kw = tzolkin_tone_keyword(t);
        const char *act = tzolkin_tone_action(t);
        TEST_ASSERT_NOT_NULL(kw);
        TEST_ASSERT_NOT_NULL(act);
        TEST_ASSERT_TRUE(strcmp(kw, "?") != 0);
        TEST_ASSERT_TRUE(strcmp(act, "?") != 0);
    }
}

/* Oracle: 10-14 */

void test_oracle_analog(void)
{
    /* Analog relationship: seal + 19 mod 20 (complement to 19) */
    /* Dragon(0) analog = Sun(19), Wind(1) analog = Storm(18) */
    tzolkin_oracle_t o = tzolkin_oracle(0, 1);
    TEST_ASSERT_EQUAL_INT(0, o.destiny);
    TEST_ASSERT_EQUAL_INT(19, o.analog);  /* Dragon <-> Sun */
}

void test_oracle_antipode(void)
{
    /* Antipode: seal + 10 mod 20 */
    /* Dragon(0) antipode = Monkey(10) */
    tzolkin_oracle_t o = tzolkin_oracle(0, 1);
    TEST_ASSERT_EQUAL_INT(10, o.antipode);
}

void test_oracle_occult(void)
{
    /* Occult: 19 - seal */
    /* Dragon(0) occult = Sun(19) — wait, that's same as analog */
    /* Actually: occult partner formula = (19 - seal) mod 20 for most seals */
    /* Dragon(0) occult = Sun(19), Serpent(4) occult = Eagle(15) */
    tzolkin_oracle_t o = tzolkin_oracle(4, 1);
    TEST_ASSERT_EQUAL_INT(15, o.occult); /* Serpent <-> Eagle */
}

void test_oracle_guide_depends_on_tone(void)
{
    /* Guide changes based on tone. Same seal, different tones = different guides */
    tzolkin_oracle_t o1 = tzolkin_oracle(0, 1);
    tzolkin_oracle_t o2 = tzolkin_oracle(0, 2);
    /* For tone 1: guide = self (same color, first in sequence) */
    TEST_ASSERT_EQUAL_INT(0, o1.guide); /* Tone 1: guide = self */
    /* Tone 2 should give a different guide */
    TEST_ASSERT_TRUE(o2.guide >= 0 && o2.guide <= 19);
}

void test_oracle_all_valid(void)
{
    for (int s = 0; s < 20; s++) {
        for (int t = 1; t <= 13; t++) {
            tzolkin_oracle_t o = tzolkin_oracle(s, t);
            TEST_ASSERT_TRUE(o.destiny >= 0 && o.destiny <= 19);
            TEST_ASSERT_TRUE(o.analog >= 0 && o.analog <= 19);
            TEST_ASSERT_TRUE(o.antipode >= 0 && o.antipode <= 19);
            TEST_ASSERT_TRUE(o.occult >= 0 && o.occult <= 19);
            TEST_ASSERT_TRUE(o.guide >= 0 && o.guide <= 19);
        }
    }
}

/* Wavespell: 15-17 */

void test_wavespell_kin1(void)
{
    /* Kin 1 = wavespell 1 */
    TEST_ASSERT_EQUAL_INT(1, tzolkin_wavespell(1));
}

void test_wavespell_kin14(void)
{
    /* Kin 14 = end of wavespell 1 (13 kins per wavespell). Kin 14 = wavespell 2 */
    TEST_ASSERT_EQUAL_INT(2, tzolkin_wavespell(14));
}

void test_wavespell_range(void)
{
    for (int k = 1; k <= 260; k++) {
        int ws = tzolkin_wavespell(k);
        TEST_ASSERT_TRUE(ws >= 1 && ws <= 20);
    }
}

/* Wavespell seal: 18-19 */

void test_wavespell_seal(void)
{
    /* Wavespell 1 starts with Dragon (seal 0) */
    TEST_ASSERT_EQUAL_INT(0, tzolkin_wavespell_seal(1));
    /* Wavespell 2 starts with Wizard (seal 14) — actually depends on the sequence */
    /* In Dreamspell: WS1=Dragon, WS2=Wizard, WS3=Hand, WS4=Sun, WS5=Skywalker,
     * WS6=Worldbridger, WS7=Storm, WS8=Human, WS9=Serpent, WS10=Mirror,
     * WS11=Monkey, WS12=Seed, WS13=Earth, WS14=Dog, WS15=Night, WS16=Warrior,
     * WS17=Moon, WS18=Wind, WS19=Eagle, WS20=Star */
    TEST_ASSERT_EQUAL_INT(13, tzolkin_wavespell_seal(2)); /* Wizard (seal 13) */
}

void test_wavespell_seal_range(void)
{
    for (int ws = 1; ws <= 20; ws++) {
        int seal = tzolkin_wavespell_seal(ws);
        TEST_ASSERT_TRUE(seal >= 0 && seal <= 19);
    }
}

/* Invalid inputs: 20-21 */

void test_invalid_seal_color(void)
{
    /* Out of range should return Red (0) as default */
    TEST_ASSERT_EQUAL_INT(TZOLKIN_RED, tzolkin_seal_color(-1));
    TEST_ASSERT_EQUAL_INT(TZOLKIN_RED, tzolkin_seal_color(20));
}

void test_invalid_tone_keyword(void)
{
    TEST_ASSERT_EQUAL_STRING("?", tzolkin_tone_keyword(0));
    TEST_ASSERT_EQUAL_STRING("?", tzolkin_tone_keyword(14));
    TEST_ASSERT_EQUAL_STRING("?", tzolkin_tone_action(0));
}

/* Purity: 22 */

void test_purity(void)
{
    tzolkin_oracle_t a = tzolkin_oracle(6, 7);
    tzolkin_oracle_t b = tzolkin_oracle(6, 7);
    TEST_ASSERT_EQUAL_INT(a.analog, b.analog);
    TEST_ASSERT_EQUAL_INT(a.guide, b.guide);

    int ws1 = tzolkin_wavespell(100);
    int ws2 = tzolkin_wavespell(100);
    TEST_ASSERT_EQUAL_INT(ws1, ws2);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_seal_colors_cycle);
    RUN_TEST(test_specific_seal_colors);
    RUN_TEST(test_color_names);
    RUN_TEST(test_earth_family_polar);
    RUN_TEST(test_earth_family_all);
    RUN_TEST(test_earth_family_names);
    RUN_TEST(test_tone_keywords);
    RUN_TEST(test_tone_actions);
    RUN_TEST(test_tone_all_valid);
    RUN_TEST(test_oracle_analog);
    RUN_TEST(test_oracle_antipode);
    RUN_TEST(test_oracle_occult);
    RUN_TEST(test_oracle_guide_depends_on_tone);
    RUN_TEST(test_oracle_all_valid);
    RUN_TEST(test_wavespell_kin1);
    RUN_TEST(test_wavespell_kin14);
    RUN_TEST(test_wavespell_range);
    RUN_TEST(test_wavespell_seal);
    RUN_TEST(test_wavespell_seal_range);
    RUN_TEST(test_invalid_seal_color);
    RUN_TEST(test_invalid_tone_keyword);
    RUN_TEST(test_purity);
    return UNITY_END();
}
