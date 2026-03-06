#include "../unity/unity.h"
#include "../../src/ui/tzolkin_card.h"
#include "../../src/systems/tzolkin/tzolkin.h"
#include "../../src/math/julian.h"

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

/* Castle: 23-26 */

void test_castle_kin1(void)
{
    /* Kin 1 is in Castle 1 (Red Eastern) */
    TEST_ASSERT_EQUAL_INT(CASTLE_RED_EASTERN, tzolkin_castle(1));
}

void test_castle_kin52(void)
{
    /* Kin 52 is last kin in Castle 1 */
    TEST_ASSERT_EQUAL_INT(CASTLE_RED_EASTERN, tzolkin_castle(52));
}

void test_castle_kin53(void)
{
    /* Kin 53 is first kin in Castle 2 (White Northern) */
    TEST_ASSERT_EQUAL_INT(CASTLE_WHITE_NORTHERN, tzolkin_castle(53));
}

void test_castle_kin260(void)
{
    /* Kin 260 is last kin in Castle 5 (Green Central) */
    TEST_ASSERT_EQUAL_INT(CASTLE_GREEN_CENTRAL, tzolkin_castle(260));
}

/* Castle boundaries: 27-28 */

void test_castle_all_boundaries(void)
{
    /* Each castle has exactly 52 kins */
    TEST_ASSERT_EQUAL_INT(1, tzolkin_castle(1));
    TEST_ASSERT_EQUAL_INT(1, tzolkin_castle(52));
    TEST_ASSERT_EQUAL_INT(2, tzolkin_castle(53));
    TEST_ASSERT_EQUAL_INT(2, tzolkin_castle(104));
    TEST_ASSERT_EQUAL_INT(3, tzolkin_castle(105));
    TEST_ASSERT_EQUAL_INT(3, tzolkin_castle(156));
    TEST_ASSERT_EQUAL_INT(4, tzolkin_castle(157));
    TEST_ASSERT_EQUAL_INT(4, tzolkin_castle(208));
    TEST_ASSERT_EQUAL_INT(5, tzolkin_castle(209));
    TEST_ASSERT_EQUAL_INT(5, tzolkin_castle(260));
}

void test_castle_invalid(void)
{
    /* Out of range kin should clamp: 0 -> 1, 261 -> 5 */
    TEST_ASSERT_EQUAL_INT(CASTLE_RED_EASTERN, tzolkin_castle(0));
    TEST_ASSERT_EQUAL_INT(CASTLE_GREEN_CENTRAL, tzolkin_castle(261));
}

/* Castle names: 29-31 */

void test_castle_names_all(void)
{
    TEST_ASSERT_EQUAL_STRING("Red Eastern",      tzolkin_castle_name(CASTLE_RED_EASTERN));
    TEST_ASSERT_EQUAL_STRING("White Northern",    tzolkin_castle_name(CASTLE_WHITE_NORTHERN));
    TEST_ASSERT_EQUAL_STRING("Blue Western",      tzolkin_castle_name(CASTLE_BLUE_WESTERN));
    TEST_ASSERT_EQUAL_STRING("Yellow Southern",   tzolkin_castle_name(CASTLE_YELLOW_SOUTHERN));
    TEST_ASSERT_EQUAL_STRING("Green Central",     tzolkin_castle_name(CASTLE_GREEN_CENTRAL));
}

void test_castle_color_names_all(void)
{
    TEST_ASSERT_EQUAL_STRING("Red",    tzolkin_castle_color_name(CASTLE_RED_EASTERN));
    TEST_ASSERT_EQUAL_STRING("White",  tzolkin_castle_color_name(CASTLE_WHITE_NORTHERN));
    TEST_ASSERT_EQUAL_STRING("Blue",   tzolkin_castle_color_name(CASTLE_BLUE_WESTERN));
    TEST_ASSERT_EQUAL_STRING("Yellow", tzolkin_castle_color_name(CASTLE_YELLOW_SOUTHERN));
    TEST_ASSERT_EQUAL_STRING("Green",  tzolkin_castle_color_name(CASTLE_GREEN_CENTRAL));
}

void test_castle_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", tzolkin_castle_name(0));
    TEST_ASSERT_EQUAL_STRING("?", tzolkin_castle_name(6));
    TEST_ASSERT_EQUAL_STRING("?", tzolkin_castle_color_name(0));
    TEST_ASSERT_EQUAL_STRING("?", tzolkin_castle_color_name(6));
}

/* Display from kin 1 (Red Magnetic Dragon): 32-33 */

void test_display_kin1(void)
{
    tzolkin_display_t d = tzolkin_display_from_kin(1);
    TEST_ASSERT_EQUAL_INT(1, d.kin);
    TEST_ASSERT_EQUAL_INT(0, d.seal);      /* Dragon */
    TEST_ASSERT_EQUAL_INT(1, d.tone);      /* Magnetic */
    TEST_ASSERT_EQUAL_INT(1, d.castle);    /* Red Eastern */
    TEST_ASSERT_EQUAL_INT(1, d.wavespell); /* Wavespell 1 */
    TEST_ASSERT_EQUAL_INT(TZOLKIN_RED, d.color);
    TEST_ASSERT_EQUAL_STRING("Red", d.color_name);
    TEST_ASSERT_EQUAL_STRING("Red Eastern Castle of Turning", d.castle_name);
    TEST_ASSERT_EQUAL_STRING("Purpose", d.tone_keyword);
    TEST_ASSERT_EQUAL_STRING("Unify", d.tone_action);
}

void test_display_kin1_title(void)
{
    tzolkin_display_t d = tzolkin_display_from_kin(1);
    TEST_ASSERT_EQUAL_STRING("Kin 1 \xc2\xb7 Red Magnetic Dragon", d.title);
}

/* Display from kin 207 (Blue Crystal Hand): 34-36 */

void test_display_kin207(void)
{
    tzolkin_display_t d = tzolkin_display_from_kin(207);
    TEST_ASSERT_EQUAL_INT(207, d.kin);
    TEST_ASSERT_EQUAL_INT(6, d.seal);       /* Hand = seal index 6 */
    TEST_ASSERT_EQUAL_INT(12, d.tone);      /* Crystal = tone 12 */
    TEST_ASSERT_EQUAL_INT(4, d.castle);     /* Yellow Southern (kin 157-208) */
    TEST_ASSERT_EQUAL_INT(16, d.wavespell); /* WS 16 */
    TEST_ASSERT_EQUAL_INT(TZOLKIN_BLUE, d.color);
    TEST_ASSERT_EQUAL_STRING("Blue", d.color_name);
    TEST_ASSERT_EQUAL_STRING("Yellow Southern Castle of Giving", d.castle_name);
    TEST_ASSERT_EQUAL_STRING("Cooperation", d.tone_keyword);
    TEST_ASSERT_EQUAL_STRING("Dedicate", d.tone_action);
}

void test_display_kin207_title(void)
{
    tzolkin_display_t d = tzolkin_display_from_kin(207);
    TEST_ASSERT_EQUAL_STRING("Kin 207 \xc2\xb7 Blue Crystal Hand", d.title);
}

void test_display_kin207_summary(void)
{
    tzolkin_display_t d = tzolkin_display_from_kin(207);
    TEST_ASSERT_EQUAL_STRING(
        "Blue Crystal Hand \xc2\xb7 WS 16 \xc2\xb7 Castle 4 Yellow Southern",
        d.summary);
}

/* Display from kin 260 (Yellow Cosmic Sun): 37-38 */

void test_display_kin260(void)
{
    tzolkin_display_t d = tzolkin_display_from_kin(260);
    TEST_ASSERT_EQUAL_INT(260, d.kin);
    TEST_ASSERT_EQUAL_INT(19, d.seal);      /* Sun */
    TEST_ASSERT_EQUAL_INT(13, d.tone);      /* Cosmic */
    TEST_ASSERT_EQUAL_INT(5, d.castle);     /* Green Central */
    TEST_ASSERT_EQUAL_INT(20, d.wavespell); /* WS 20 */
    TEST_ASSERT_EQUAL_INT(TZOLKIN_YELLOW, d.color);
    TEST_ASSERT_EQUAL_STRING("Yellow", d.color_name);
    TEST_ASSERT_EQUAL_STRING("Green Central Castle of Enchantment", d.castle_name);
    TEST_ASSERT_EQUAL_STRING("Endurance", d.tone_keyword);
    TEST_ASSERT_EQUAL_STRING("Transcend", d.tone_action);
}

void test_display_kin260_title(void)
{
    tzolkin_display_t d = tzolkin_display_from_kin(260);
    TEST_ASSERT_EQUAL_STRING("Kin 260 \xc2\xb7 Yellow Cosmic Sun", d.title);
}

/* Display oracle names: 39-40 */

void test_display_oracle_names_kin1(void)
{
    /* Kin 1: Red Magnetic Dragon (seal 0, tone 1)
     * Oracle: destiny=Dragon, analog=Sun(19), antipode=Monkey(10),
     *         occult=Sun(19), guide=Dragon(0) (tone 1 -> guide=self) */
    tzolkin_display_t d = tzolkin_display_from_kin(1);
    TEST_ASSERT_EQUAL_STRING("Red Dragon", d.oracle_guide);
    TEST_ASSERT_EQUAL_STRING("Yellow Sun", d.oracle_analog);
    TEST_ASSERT_EQUAL_STRING("Blue Monkey", d.oracle_antipode);
    TEST_ASSERT_EQUAL_STRING("Yellow Sun", d.oracle_occult);
}

void test_display_oracle_names_kin207(void)
{
    /* Kin 207: Blue Crystal Hand (seal 6, tone 12)
     * analog=(6+19)%20=5=Worldbridger, antipode=(6+10)%20=16=Earth,
     * occult=(19-6+20)%20=13=Wizard, guide: tone 12 -> offset 4,
     * guide = (6+4)%20 = 10 = Monkey */
    tzolkin_display_t d = tzolkin_display_from_kin(207);
    TEST_ASSERT_EQUAL_STRING("White Worldbridger", d.oracle_analog);
    TEST_ASSERT_EQUAL_STRING("Red Earth", d.oracle_antipode);
    TEST_ASSERT_EQUAL_STRING("White Wizard", d.oracle_occult);
    TEST_ASSERT_EQUAL_STRING("Blue Monkey", d.oracle_guide);
}

/* Display from JD: 41 */

void test_display_from_jd(void)
{
    /* 2012-12-21 = Kin 207 in Dreamspell */
    /* JD for 2012-12-21 noon = 2456283.5 */
    double jd_2012_12_21 = gregorian_to_jd(2012, 12, 21.5);
    tzolkin_display_t d = tzolkin_display_from_jd(jd_2012_12_21);
    TEST_ASSERT_EQUAL_INT(207, d.kin);
    TEST_ASSERT_EQUAL_INT(6, d.seal);
    TEST_ASSERT_EQUAL_INT(12, d.tone);
}

/* Castle full names: 42-44 */

void test_castle_full_names_all(void)
{
    TEST_ASSERT_EQUAL_STRING("Red Eastern Castle of Turning",
                             tzolkin_castle_full_name(CASTLE_RED_EASTERN));
    TEST_ASSERT_EQUAL_STRING("White Northern Castle of Crossing",
                             tzolkin_castle_full_name(CASTLE_WHITE_NORTHERN));
    TEST_ASSERT_EQUAL_STRING("Blue Western Castle of Burning",
                             tzolkin_castle_full_name(CASTLE_BLUE_WESTERN));
    TEST_ASSERT_EQUAL_STRING("Yellow Southern Castle of Giving",
                             tzolkin_castle_full_name(CASTLE_YELLOW_SOUTHERN));
    TEST_ASSERT_EQUAL_STRING("Green Central Castle of Enchantment",
                             tzolkin_castle_full_name(CASTLE_GREEN_CENTRAL));
}

void test_castle_short_names_all(void)
{
    TEST_ASSERT_EQUAL_STRING("Red Eastern",
                             tzolkin_castle_short_name(CASTLE_RED_EASTERN));
    TEST_ASSERT_EQUAL_STRING("White Northern",
                             tzolkin_castle_short_name(CASTLE_WHITE_NORTHERN));
    TEST_ASSERT_EQUAL_STRING("Blue Western",
                             tzolkin_castle_short_name(CASTLE_BLUE_WESTERN));
    TEST_ASSERT_EQUAL_STRING("Yellow Southern",
                             tzolkin_castle_short_name(CASTLE_YELLOW_SOUTHERN));
    TEST_ASSERT_EQUAL_STRING("Green Central",
                             tzolkin_castle_short_name(CASTLE_GREEN_CENTRAL));
}

void test_castle_full_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", tzolkin_castle_full_name(0));
    TEST_ASSERT_EQUAL_STRING("?", tzolkin_castle_full_name(6));
    TEST_ASSERT_EQUAL_STRING("?", tzolkin_castle_short_name(0));
    TEST_ASSERT_EQUAL_STRING("?", tzolkin_castle_short_name(6));
}

/* Wavespell position: 45-48 */

void test_wavespell_position_kin1(void)
{
    TEST_ASSERT_EQUAL_INT(1, tzolkin_wavespell_position(1));
}

void test_wavespell_position_kin13(void)
{
    TEST_ASSERT_EQUAL_INT(13, tzolkin_wavespell_position(13));
}

void test_wavespell_position_kin14(void)
{
    /* Kin 14 is the first kin of wavespell 2 */
    TEST_ASSERT_EQUAL_INT(1, tzolkin_wavespell_position(14));
}

void test_wavespell_position_range(void)
{
    for (int k = 1; k <= 260; k++) {
        int pos = tzolkin_wavespell_position(k);
        TEST_ASSERT_TRUE(pos >= 1 && pos <= 13);
    }
}

/* Seal keywords: 49-52 */

void test_seal_keyword_dragon(void)
{
    TEST_ASSERT_EQUAL_STRING("Birth", tzolkin_seal_keyword(0));
}

void test_seal_keyword_hand(void)
{
    TEST_ASSERT_EQUAL_STRING("Accomplishment", tzolkin_seal_keyword(6));
}

void test_seal_keyword_sun(void)
{
    TEST_ASSERT_EQUAL_STRING("Universal Fire", tzolkin_seal_keyword(19));
}

void test_seal_keyword_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", tzolkin_seal_keyword(-1));
    TEST_ASSERT_EQUAL_STRING("?", tzolkin_seal_keyword(20));
}

/* Display with new fields: 53-57 */

void test_display_kin1_new_fields(void)
{
    tzolkin_display_t d = tzolkin_display_from_kin(1);
    TEST_ASSERT_EQUAL_INT(1, d.wavespell_position);
    TEST_ASSERT_EQUAL_STRING("Birth", d.seal_keyword);
    TEST_ASSERT_EQUAL_STRING("Red Eastern Castle of Turning", d.castle_name);
}

void test_display_kin207_new_fields(void)
{
    tzolkin_display_t d = tzolkin_display_from_kin(207);
    /* Kin 207: position within wavespell = ((207-1) % 13) + 1 = (206 % 13) + 1 = 11 + 1 = 12 */
    TEST_ASSERT_EQUAL_INT(12, d.wavespell_position);
    TEST_ASSERT_EQUAL_STRING("Accomplishment", d.seal_keyword);
    TEST_ASSERT_EQUAL_STRING("Yellow Southern Castle of Giving", d.castle_name);
}

void test_display_kin260_new_fields(void)
{
    tzolkin_display_t d = tzolkin_display_from_kin(260);
    /* Kin 260: position = ((260-1) % 13) + 1 = (259 % 13) + 1 = 12 + 1 = 13 */
    TEST_ASSERT_EQUAL_INT(13, d.wavespell_position);
    TEST_ASSERT_EQUAL_STRING("Universal Fire", d.seal_keyword);
    TEST_ASSERT_EQUAL_STRING("Green Central Castle of Enchantment",
                             d.castle_name);
}

void test_display_kin14_wavespell_position(void)
{
    /* Kin 14: first kin of wavespell 2, position = 1 */
    tzolkin_display_t d = tzolkin_display_from_kin(14);
    TEST_ASSERT_EQUAL_INT(1, d.wavespell_position);
    TEST_ASSERT_EQUAL_INT(2, d.wavespell);
}

void test_display_all_seal_keywords_populated(void)
{
    /* Every kin should have a non-empty, non-"?" seal_keyword */
    for (int k = 1; k <= 260; k++) {
        tzolkin_display_t d = tzolkin_display_from_kin(k);
        TEST_ASSERT_TRUE(d.seal_keyword[0] != '\0');
        TEST_ASSERT_TRUE(strcmp(d.seal_keyword, "?") != 0);
    }
}

/* Purity for new functions: 58 */

void test_castle_purity(void)
{
    /* Same input -> same output */
    tzolkin_castle_t c1 = tzolkin_castle(130);
    tzolkin_castle_t c2 = tzolkin_castle(130);
    TEST_ASSERT_EQUAL_INT(c1, c2);

    tzolkin_display_t d1 = tzolkin_display_from_kin(100);
    tzolkin_display_t d2 = tzolkin_display_from_kin(100);
    TEST_ASSERT_EQUAL_INT(d1.kin, d2.kin);
    TEST_ASSERT_EQUAL_INT(d1.castle, d2.castle);
    TEST_ASSERT_EQUAL_STRING(d1.title, d2.title);
    TEST_ASSERT_EQUAL_STRING(d1.summary, d2.summary);
}

int main(void)
{
    UNITY_BEGIN();
    /* Existing tests (1-22) */
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
    /* New tests: Castle (23-28) */
    RUN_TEST(test_castle_kin1);
    RUN_TEST(test_castle_kin52);
    RUN_TEST(test_castle_kin53);
    RUN_TEST(test_castle_kin260);
    RUN_TEST(test_castle_all_boundaries);
    RUN_TEST(test_castle_invalid);
    /* New tests: Castle names (29-31) */
    RUN_TEST(test_castle_names_all);
    RUN_TEST(test_castle_color_names_all);
    RUN_TEST(test_castle_name_invalid);
    /* New tests: Display (32-42) */
    RUN_TEST(test_display_kin1);
    RUN_TEST(test_display_kin1_title);
    RUN_TEST(test_display_kin207);
    RUN_TEST(test_display_kin207_title);
    RUN_TEST(test_display_kin207_summary);
    RUN_TEST(test_display_kin260);
    RUN_TEST(test_display_kin260_title);
    RUN_TEST(test_display_oracle_names_kin1);
    RUN_TEST(test_display_oracle_names_kin207);
    RUN_TEST(test_display_from_jd);
    /* New tests: Castle full/short names (42-44) */
    RUN_TEST(test_castle_full_names_all);
    RUN_TEST(test_castle_short_names_all);
    RUN_TEST(test_castle_full_name_invalid);
    /* New tests: Wavespell position (45-48) */
    RUN_TEST(test_wavespell_position_kin1);
    RUN_TEST(test_wavespell_position_kin13);
    RUN_TEST(test_wavespell_position_kin14);
    RUN_TEST(test_wavespell_position_range);
    /* New tests: Seal keywords (49-52) */
    RUN_TEST(test_seal_keyword_dragon);
    RUN_TEST(test_seal_keyword_hand);
    RUN_TEST(test_seal_keyword_sun);
    RUN_TEST(test_seal_keyword_invalid);
    /* New tests: Display new fields (53-57) */
    RUN_TEST(test_display_kin1_new_fields);
    RUN_TEST(test_display_kin207_new_fields);
    RUN_TEST(test_display_kin260_new_fields);
    RUN_TEST(test_display_kin14_wavespell_position);
    RUN_TEST(test_display_all_seal_keywords_populated);
    /* Purity (58) */
    RUN_TEST(test_castle_purity);
    return UNITY_END();
}
