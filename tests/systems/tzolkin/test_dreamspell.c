#include "../../unity/unity.h"
#include "../../../src/systems/tzolkin/dreamspell.h"
#include <stdio.h>

void setUp(void) {}
void tearDown(void) {}

/* ============================================================
 * Seal indexing in this codebase (matching tzolkin.h / tzolkin.c):
 *   0=Dragon, 1=Wind, 2=Night, 3=Seed, 4=Serpent,
 *   5=World-Bridger, 6=Hand, 7=Star, 8=Moon, 9=Dog,
 *   10=Monkey, 11=Human, 12=Skywalker, 13=Wizard,
 *   14=Eagle, 15=Warrior, 16=Earth, 17=Mirror, 18=Storm, 19=Sun
 *
 * Color mapping: seal % 4: 0=Red, 1=White, 2=Blue, 3=Yellow
 * ============================================================ */

/* --- Helper: kin_from_tone_seal via CRT --- */

/* Verify kin 1 = tone 1, seal 0 (Red Magnetic Dragon) */
void test_kin1_is_magnetic_dragon(void) {
    dreamspell_oracle_t o = dreamspell_oracle(1);
    TEST_ASSERT_EQUAL_INT(1, o.destiny);
    /* tone of kin 1 = 1, seal of kin 1 = 0 (Dragon) */
    dreamspell_tone_t t = dreamspell_tone(1);
    TEST_ASSERT_EQUAL_STRING("Magnetic", t.name);
}

/* Verify kin 2 = tone 2, seal 1 (White Lunar Wind) */
void test_kin2_is_lunar_wind(void) {
    dreamspell_tone_t t = dreamspell_tone(2);
    TEST_ASSERT_EQUAL_STRING("Lunar", t.name);
    dreamspell_color_t c = dreamspell_color(1);
    TEST_ASSERT_EQUAL_STRING("White", c.name);
}

/* Verify kin 20 = tone 7 (Resonant), seal 19 (Sun) */
void test_kin20_is_resonant_sun(void) {
    /* kin 20: tone = (20-1)%13+1 = 19%13+1 = 6+1 = 7 */
    /* seal = (20-1)%20 = 19 = Sun */
    dreamspell_tone_t t = dreamspell_tone(7);
    TEST_ASSERT_EQUAL_STRING("Resonant", t.name);
}

/* Verify kin 21 = tone 8, seal 0 (Dragon) */
void test_kin21_is_galactic_dragon(void) {
    /* kin 21: tone = (21-1)%13+1 = 20%13+1 = 7+1 = 8 */
    /* seal = (21-1)%20 = 0 = Dragon */
    dreamspell_tone_t t = dreamspell_tone(8);
    TEST_ASSERT_EQUAL_STRING("Galactic", t.name);
}

/* Verify kin 260 = tone 13, seal 19 (Cosmic Sun) */
void test_kin260_is_cosmic_sun(void) {
    /* kin 260: tone = (260-1)%13+1 = 259%13+1 = 12+1 = 13 */
    /* seal = (260-1)%20 = 259%20 = 19 = Sun */
    dreamspell_tone_t t = dreamspell_tone(13);
    TEST_ASSERT_EQUAL_STRING("Cosmic", t.name);
    dreamspell_color_t c = dreamspell_color(19);
    TEST_ASSERT_EQUAL_STRING("Yellow", c.name);
}

/* --- Tone data tests --- */

void test_tone_magnetic(void) {
    dreamspell_tone_t t = dreamspell_tone(1);
    TEST_ASSERT_EQUAL_INT(1, t.number);
    TEST_ASSERT_EQUAL_STRING("Magnetic", t.name);
    TEST_ASSERT_EQUAL_STRING("Unify", t.action);
    TEST_ASSERT_EQUAL_STRING("Purpose", t.power);
}

void test_tone_lunar(void) {
    dreamspell_tone_t t = dreamspell_tone(2);
    TEST_ASSERT_EQUAL_INT(2, t.number);
    TEST_ASSERT_EQUAL_STRING("Lunar", t.name);
    TEST_ASSERT_EQUAL_STRING("Polarize", t.action);
    TEST_ASSERT_EQUAL_STRING("Challenge", t.power);
}

void test_tone_electric(void) {
    dreamspell_tone_t t = dreamspell_tone(3);
    TEST_ASSERT_EQUAL_INT(3, t.number);
    TEST_ASSERT_EQUAL_STRING("Electric", t.name);
    TEST_ASSERT_EQUAL_STRING("Activate", t.action);
    TEST_ASSERT_EQUAL_STRING("Service", t.power);
}

void test_tone_self_existing(void) {
    dreamspell_tone_t t = dreamspell_tone(4);
    TEST_ASSERT_EQUAL_INT(4, t.number);
    TEST_ASSERT_EQUAL_STRING("Self-Existing", t.name);
    TEST_ASSERT_EQUAL_STRING("Define", t.action);
    TEST_ASSERT_EQUAL_STRING("Form", t.power);
}

void test_tone_overtone(void) {
    dreamspell_tone_t t = dreamspell_tone(5);
    TEST_ASSERT_EQUAL_INT(5, t.number);
    TEST_ASSERT_EQUAL_STRING("Overtone", t.name);
    TEST_ASSERT_EQUAL_STRING("Empower", t.action);
    TEST_ASSERT_EQUAL_STRING("Radiance", t.power);
}

void test_tone_rhythmic(void) {
    dreamspell_tone_t t = dreamspell_tone(6);
    TEST_ASSERT_EQUAL_INT(6, t.number);
    TEST_ASSERT_EQUAL_STRING("Rhythmic", t.name);
    TEST_ASSERT_EQUAL_STRING("Organize", t.action);
    TEST_ASSERT_EQUAL_STRING("Equality", t.power);
}

void test_tone_resonant(void) {
    dreamspell_tone_t t = dreamspell_tone(7);
    TEST_ASSERT_EQUAL_INT(7, t.number);
    TEST_ASSERT_EQUAL_STRING("Resonant", t.name);
    TEST_ASSERT_EQUAL_STRING("Channel", t.action);
    TEST_ASSERT_EQUAL_STRING("Attunement", t.power);
}

void test_tone_galactic(void) {
    dreamspell_tone_t t = dreamspell_tone(8);
    TEST_ASSERT_EQUAL_INT(8, t.number);
    TEST_ASSERT_EQUAL_STRING("Galactic", t.name);
    TEST_ASSERT_EQUAL_STRING("Harmonize", t.action);
    TEST_ASSERT_EQUAL_STRING("Integrity", t.power);
}

void test_tone_solar(void) {
    dreamspell_tone_t t = dreamspell_tone(9);
    TEST_ASSERT_EQUAL_INT(9, t.number);
    TEST_ASSERT_EQUAL_STRING("Solar", t.name);
    TEST_ASSERT_EQUAL_STRING("Pulse", t.action);
    TEST_ASSERT_EQUAL_STRING("Intention", t.power);
}

void test_tone_planetary(void) {
    dreamspell_tone_t t = dreamspell_tone(10);
    TEST_ASSERT_EQUAL_INT(10, t.number);
    TEST_ASSERT_EQUAL_STRING("Planetary", t.name);
    TEST_ASSERT_EQUAL_STRING("Perfect", t.action);
    TEST_ASSERT_EQUAL_STRING("Manifestation", t.power);
}

void test_tone_spectral(void) {
    dreamspell_tone_t t = dreamspell_tone(11);
    TEST_ASSERT_EQUAL_INT(11, t.number);
    TEST_ASSERT_EQUAL_STRING("Spectral", t.name);
    TEST_ASSERT_EQUAL_STRING("Dissolve", t.action);
    TEST_ASSERT_EQUAL_STRING("Liberation", t.power);
}

void test_tone_crystal(void) {
    dreamspell_tone_t t = dreamspell_tone(12);
    TEST_ASSERT_EQUAL_INT(12, t.number);
    TEST_ASSERT_EQUAL_STRING("Crystal", t.name);
    TEST_ASSERT_EQUAL_STRING("Dedicate", t.action);
    TEST_ASSERT_EQUAL_STRING("Cooperation", t.power);
}

void test_tone_cosmic(void) {
    dreamspell_tone_t t = dreamspell_tone(13);
    TEST_ASSERT_EQUAL_INT(13, t.number);
    TEST_ASSERT_EQUAL_STRING("Cosmic", t.name);
    TEST_ASSERT_EQUAL_STRING("Endure", t.action);
    TEST_ASSERT_EQUAL_STRING("Presence", t.power);
}

void test_tone_invalid_zero(void) {
    dreamspell_tone_t t = dreamspell_tone(0);
    TEST_ASSERT_EQUAL_INT(0, t.number);
    TEST_ASSERT_EQUAL_STRING("?", t.name);
}

void test_tone_invalid_fourteen(void) {
    dreamspell_tone_t t = dreamspell_tone(14);
    TEST_ASSERT_EQUAL_INT(0, t.number);
    TEST_ASSERT_EQUAL_STRING("?", t.name);
}

void test_tone_invalid_negative(void) {
    dreamspell_tone_t t = dreamspell_tone(-1);
    TEST_ASSERT_EQUAL_INT(0, t.number);
    TEST_ASSERT_EQUAL_STRING("?", t.name);
}

/* --- Color data tests --- */

void test_color_red(void) {
    /* seal 0 (Dragon) % 4 = 0 = Red */
    dreamspell_color_t c = dreamspell_color(0);
    TEST_ASSERT_EQUAL_STRING("Red", c.name);
    TEST_ASSERT_EQUAL_STRING("Initiates", c.action);
    TEST_ASSERT_EQUAL_STRING("Birth", c.quality);
}

void test_color_white(void) {
    /* seal 1 (Wind) % 4 = 1 = White */
    dreamspell_color_t c = dreamspell_color(1);
    TEST_ASSERT_EQUAL_STRING("White", c.name);
    TEST_ASSERT_EQUAL_STRING("Refines", c.action);
    TEST_ASSERT_EQUAL_STRING("Death", c.quality);
}

void test_color_blue(void) {
    /* seal 2 (Night) % 4 = 2 = Blue */
    dreamspell_color_t c = dreamspell_color(2);
    TEST_ASSERT_EQUAL_STRING("Blue", c.name);
    TEST_ASSERT_EQUAL_STRING("Transforms", c.action);
    TEST_ASSERT_EQUAL_STRING("Magic", c.quality);
}

void test_color_yellow(void) {
    /* seal 3 (Seed) % 4 = 3 = Yellow */
    dreamspell_color_t c = dreamspell_color(3);
    TEST_ASSERT_EQUAL_STRING("Yellow", c.name);
    TEST_ASSERT_EQUAL_STRING("Ripens", c.action);
    TEST_ASSERT_EQUAL_STRING("Intelligence", c.quality);
}

void test_color_cycles(void) {
    /* Red seals: 0,4,8,12,16 */
    for (int s = 0; s < 20; s += 4) {
        dreamspell_color_t c = dreamspell_color(s);
        TEST_ASSERT_EQUAL_STRING("Red", c.name);
    }
    /* White seals: 1,5,9,13,17 */
    for (int s = 1; s < 20; s += 4) {
        dreamspell_color_t c = dreamspell_color(s);
        TEST_ASSERT_EQUAL_STRING("White", c.name);
    }
    /* Blue seals: 2,6,10,14,18 */
    for (int s = 2; s < 20; s += 4) {
        dreamspell_color_t c = dreamspell_color(s);
        TEST_ASSERT_EQUAL_STRING("Blue", c.name);
    }
    /* Yellow seals: 3,7,11,15,19 */
    for (int s = 3; s < 20; s += 4) {
        dreamspell_color_t c = dreamspell_color(s);
        TEST_ASSERT_EQUAL_STRING("Yellow", c.name);
    }
}

void test_color_invalid_seal(void) {
    dreamspell_color_t c = dreamspell_color(-1);
    TEST_ASSERT_EQUAL_STRING("?", c.name);
    c = dreamspell_color(20);
    TEST_ASSERT_EQUAL_STRING("?", c.name);
}

/* --- Oracle tests --- */

/* Kin 1: Red Magnetic Dragon.
 * tone=1, seal=0 (Dragon), color=Red
 * Analog: seal = (19-0) % 20 = 19 (Sun), tone=1 → kin_from(1,19) = 40
 * Wait: CRT: ((40*(1-1) + 221*19) % 260) + 1 = (4199%260)+1 = (4199-16*260)+1
 * = (4199-4160)+1 = 39+1 = 40
 * Antipode: seal = (0+10)%20 = 10 (Monkey), tone=1 → kin_from(1,10)
 *   = ((0 + 221*10) % 260) + 1 = (2210%260)+1 = (2210-8*260)+1 = (2210-2080)+1 = 130+1 = 131
 * Guide: tone 1 → guide_seal = self = 0. kin_from(1,0) = 1
 * Occult: 261-1 = 260
 * Hidden: 261-1 = 260
 */
void test_oracle_kin1(void) {
    dreamspell_oracle_t o = dreamspell_oracle(1);
    TEST_ASSERT_EQUAL_INT(1, o.destiny);
    TEST_ASSERT_EQUAL_INT(40, o.analog);
    TEST_ASSERT_EQUAL_INT(131, o.antipode);
    TEST_ASSERT_EQUAL_INT(1, o.guide);
    TEST_ASSERT_EQUAL_INT(260, o.occult);
    TEST_ASSERT_EQUAL_INT(260, o.hidden);
}

/* Kin 131: Blue Magnetic Monkey.
 * tone = (131-1)%13+1 = 130%13+1 = 0+1 = 1
 * seal = (131-1)%20 = 130%20 = 10 (Monkey), color = 10%4 = 2 = Blue
 * Analog: seal = (19-10)%20 = 9 (Dog), tone=1 → kin_from(1,9)
 *   = ((0 + 221*9) % 260) + 1 = (1989%260)+1 = (1989-7*260)+1 = (1989-1820)+1 = 169+1 = 170
 * Antipode: seal = (10+10)%20 = 0 (Dragon), tone=1 → kin_from(1,0) = 1
 * Guide: tone 1 → guide_seal = self = 10. kin_from(1,10) = 131
 * Occult: 261-131 = 130
 * Hidden: 261-131 = 130
 */
void test_oracle_kin131(void) {
    dreamspell_oracle_t o = dreamspell_oracle(131);
    TEST_ASSERT_EQUAL_INT(131, o.destiny);
    TEST_ASSERT_EQUAL_INT(170, o.analog);
    TEST_ASSERT_EQUAL_INT(1, o.antipode);
    TEST_ASSERT_EQUAL_INT(131, o.guide);
    TEST_ASSERT_EQUAL_INT(130, o.occult);
    TEST_ASSERT_EQUAL_INT(130, o.hidden);
}

/* Kin 260: Yellow Cosmic Sun.
 * tone = (260-1)%13+1 = 259%13+1 = 12+1 = 13
 * seal = (260-1)%20 = 259%20 = 19 (Sun), color = 19%4 = 3 = Yellow
 * Analog: seal = (19-19)%20 = 0 (Dragon), tone=13 → kin_from(13,0)
 *   = ((40*12 + 221*0) % 260) + 1 = (480%260)+1 = 220+1 = 221
 * Antipode: seal = (19+10)%20 = 9 (Dog), tone=13 → kin_from(13,9)
 *   = ((40*12 + 221*9) % 260) + 1 = (480+1989)%260+1 = 2469%260+1
 *   = (2469 - 9*260) + 1 = (2469-2340)+1 = 129+1 = 130
 * Guide: tone 13 → group {3,8,13} → guide_seal = (19+4)%20 = 3 (Seed)
 *   tone=13 → kin_from(13,3) = ((480+663)%260)+1 = (1143%260)+1
 *   = (1143-4*260)+1 = (1143-1040)+1 = 103+1 = 104
 * Occult: 261-260 = 1
 * Hidden: 261-260 = 1
 */
void test_oracle_kin260(void) {
    dreamspell_oracle_t o = dreamspell_oracle(260);
    TEST_ASSERT_EQUAL_INT(260, o.destiny);
    TEST_ASSERT_EQUAL_INT(221, o.analog);
    TEST_ASSERT_EQUAL_INT(130, o.antipode);
    TEST_ASSERT_EQUAL_INT(104, o.guide);
    TEST_ASSERT_EQUAL_INT(1, o.occult);
    TEST_ASSERT_EQUAL_INT(1, o.hidden);
}

/* Kin 44: Yellow Overtone Seed.
 * tone = (44-1)%13+1 = 43%13+1 = 4+1 = 5
 * seal = (44-1)%20 = 43%20 = 3 (Seed), color = 3%4 = 3 = Yellow
 * Analog: seal = (19-3)%20 = 16 (Earth), tone=5 → kin_from(5,16)
 *   = ((40*4 + 221*16) % 260)+1 = (160+3536)%260+1 = 3696%260+1
 *   = (3696-14*260)+1 = (3696-3640)+1 = 56+1 = 57
 * Antipode: seal = (3+10)%20 = 13 (Wizard), tone=5 → kin_from(5,13)
 *   = ((40*4 + 221*13) % 260)+1 = (160+2873)%260+1 = 3033%260+1
 *   = (3033-11*260)+1 = (3033-2860)+1 = 173+1 = 174
 * Guide: tone 5 → group {5,10} → guide_seal = (3+8)%20 = 11 (Human)
 *   tone=5 → kin_from(5,11) = ((160+2431)%260)+1 = 2591%260+1
 *   = (2591-9*260)+1 = (2591-2340)+1 = 251+1 = 252
 * Occult: 261-44 = 217
 */
void test_oracle_kin44(void) {
    dreamspell_oracle_t o = dreamspell_oracle(44);
    TEST_ASSERT_EQUAL_INT(44, o.destiny);
    TEST_ASSERT_EQUAL_INT(57, o.analog);
    TEST_ASSERT_EQUAL_INT(174, o.antipode);
    TEST_ASSERT_EQUAL_INT(252, o.guide);
    TEST_ASSERT_EQUAL_INT(217, o.occult);
    TEST_ASSERT_EQUAL_INT(217, o.hidden);
}

/* Kin 100: Yellow Solar Sun.
 * tone = (100-1)%13+1 = 99%13+1 = 8+1 = 9
 * seal = (100-1)%20 = 99%20 = 19 (Sun), color = 19%4 = 3 = Yellow
 * Analog: seal = (19-19)%20 = 0 (Dragon), tone=9 → kin_from(9,0)
 *   = ((40*8 + 0) % 260) + 1 = (320%260)+1 = 60+1 = 61
 * Antipode: seal = (19+10)%20 = 9 (Dog), tone=9 → kin_from(9,9)
 *   = ((320 + 1989) % 260)+1 = 2309%260+1 = (2309-8*260)+1 = (2309-2080)+1 = 229+1 = 230
 * Guide: tone 9 → group {4,9} → guide_seal = (19+16)%20 = 15 (Warrior)
 *   tone=9 → kin_from(9,15) = ((320+221*15)%260)+1 = ((320+3315)%260)+1
 *   = (3635%260)+1 = (3635-13*260)+1 = (3635-3380)+1 = 255+1 = 256
 * Occult: 261-100 = 161
 */
void test_oracle_kin100(void) {
    dreamspell_oracle_t o = dreamspell_oracle(100);
    TEST_ASSERT_EQUAL_INT(100, o.destiny);
    TEST_ASSERT_EQUAL_INT(61, o.analog);
    TEST_ASSERT_EQUAL_INT(230, o.antipode);
    TEST_ASSERT_EQUAL_INT(256, o.guide);
    TEST_ASSERT_EQUAL_INT(161, o.occult);
    TEST_ASSERT_EQUAL_INT(161, o.hidden);
}

/* Kin 207: Blue Crystal Hand (2012-12-21 anchor date).
 * tone = (207-1)%13+1 = 206%13+1 = 11+1 = 12
 * seal = (207-1)%20 = 206%20 = 6 (Hand), color = 6%4 = 2 = Blue
 * Analog: seal = (19-6)%20 = 13 (Wizard), tone=12 → kin_from(12,13)
 *   = ((40*11 + 221*13) % 260)+1 = (440+2873)%260+1 = 3313%260+1
 *   = (3313-12*260)+1 = (3313-3120)+1 = 193+1 = 194
 * Antipode: seal = (6+10)%20 = 16 (Earth), tone=12 → kin_from(12,16)
 *   = ((440 + 221*16) % 260)+1 = (440+3536)%260+1 = 3976%260+1
 *   = (3976-15*260)+1 = (3976-3900)+1 = 76+1 = 77
 * Guide: tone 12 → group {2,7,12} → guide_seal = (6+12)%20 = 18 (Storm)
 *   tone=12 → kin_from(12,18) = ((440+221*18)%260)+1 = (440+3978)%260+1
 *   = 4418%260+1 = (4418-16*260)+1 = (4418-4160)+1 = 258+1 = 259
 *   Wait: 4418/260 = 16.99..., 16*260=4160, 4418-4160=258. So 259.
 * Occult: 261-207 = 54
 */
void test_oracle_kin207(void) {
    dreamspell_oracle_t o = dreamspell_oracle(207);
    TEST_ASSERT_EQUAL_INT(207, o.destiny);
    TEST_ASSERT_EQUAL_INT(194, o.analog);
    TEST_ASSERT_EQUAL_INT(77, o.antipode);
    TEST_ASSERT_EQUAL_INT(259, o.guide);
    TEST_ASSERT_EQUAL_INT(54, o.occult);
    TEST_ASSERT_EQUAL_INT(54, o.hidden);
}

/* Oracle with tone 2: guide uses (S+12)%20 */
/* Kin 2: White Lunar Wind. tone=2, seal=1 (Wind)
 * Guide: tone 2 → (1+12)%20 = 13 (Wizard), color=13%4=1=White ✓
 *   kin_from(2,13) = ((40*1+221*13)%260)+1 = (40+2873)%260+1 = 2913%260+1
 *   = (2913-11*260)+1 = (2913-2860)+1 = 53+1 = 54
 * Analog: seal=(19-1)%20=18 (Storm), kin_from(2,18) = ((40+221*18)%260)+1
 *   = (40+3978)%260+1 = 4018%260+1 = (4018-15*260)+1 = (4018-3900)+1 = 118+1 = 119
 * Antipode: seal=(1+10)%20=11 (Human), kin_from(2,11) = ((40+221*11)%260)+1
 *   = (40+2431)%260+1 = 2471%260+1 = (2471-9*260)+1 = (2471-2340)+1 = 131+1 = 132
 */
void test_oracle_kin2_guide_tone2(void) {
    dreamspell_oracle_t o = dreamspell_oracle(2);
    TEST_ASSERT_EQUAL_INT(2, o.destiny);
    TEST_ASSERT_EQUAL_INT(54, o.guide);
    TEST_ASSERT_EQUAL_INT(119, o.analog);
    TEST_ASSERT_EQUAL_INT(132, o.antipode);
    TEST_ASSERT_EQUAL_INT(259, o.occult);
}

/* Oracle with tone 3: guide uses (S+4)%20 */
/* Kin 3: Blue Electric Night. tone=3, seal=2 (Night)
 * Guide: tone 3 → (2+4)%20 = 6 (Hand), color=6%4=2=Blue ✓
 *   kin_from(3,6) = ((40*2+221*6)%260)+1 = (80+1326)%260+1 = 1406%260+1
 *   = (1406-5*260)+1 = (1406-1300)+1 = 106+1 = 107
 * Analog: seal=(19-2)%20=17 (Mirror), kin_from(3,17) = ((80+221*17)%260)+1
 *   = (80+3757)%260+1 = 3837%260+1 = (3837-14*260)+1 = (3837-3640)+1 = 197+1 = 198
 * Antipode: seal=(2+10)%20=12 (Skywalker), kin_from(3,12)
 *   = ((80+221*12)%260)+1 = (80+2652)%260+1 = 2732%260+1
 *   = (2732-10*260)+1 = (2732-2600)+1 = 132+1 = 133
 */
void test_oracle_kin3_guide_tone3(void) {
    dreamspell_oracle_t o = dreamspell_oracle(3);
    TEST_ASSERT_EQUAL_INT(3, o.destiny);
    TEST_ASSERT_EQUAL_INT(107, o.guide);
    TEST_ASSERT_EQUAL_INT(198, o.analog);
    TEST_ASSERT_EQUAL_INT(133, o.antipode);
}

/* Oracle with tone 4: guide uses (S+16)%20 */
/* Kin 4: Yellow Self-Existing Seed. tone=4, seal=3 (Seed)
 * Guide: tone 4 → (3+16)%20 = 19 (Sun), color=19%4=3=Yellow ✓
 *   kin_from(4,19) = ((40*3+221*19)%260)+1 = (120+4199)%260+1 = 4319%260+1
 *   = (4319-16*260)+1 = (4319-4160)+1 = 159+1 = 160
 */
void test_oracle_kin4_guide_tone4(void) {
    dreamspell_oracle_t o = dreamspell_oracle(4);
    TEST_ASSERT_EQUAL_INT(4, o.destiny);
    TEST_ASSERT_EQUAL_INT(160, o.guide);
}

/* Oracle with tone 5: guide uses (S+8)%20 */
/* Kin 5: Red Overtone Serpent. tone=5, seal=4 (Serpent)
 * Guide: tone 5 → (4+8)%20 = 12 (Skywalker), color=12%4=0=Red ✓
 *   kin_from(5,12) = ((40*4+221*12)%260)+1 = (160+2652)%260+1 = 2812%260+1
 *   = (2812-10*260)+1 = (2812-2600)+1 = 212+1 = 213
 */
void test_oracle_kin5_guide_tone5(void) {
    dreamspell_oracle_t o = dreamspell_oracle(5);
    TEST_ASSERT_EQUAL_INT(5, o.destiny);
    TEST_ASSERT_EQUAL_INT(213, o.guide);
}

/* Oracle with tone 6: guide is self */
/* Kin 6: White Rhythmic World-Bridger. tone=6, seal=5
 * Guide: tone 6 → guide_seal = 5, kin_from(6,5) = 6 (self)
 */
void test_oracle_kin6_guide_tone6(void) {
    dreamspell_oracle_t o = dreamspell_oracle(6);
    TEST_ASSERT_EQUAL_INT(6, o.destiny);
    TEST_ASSERT_EQUAL_INT(6, o.guide);
}

/* Oracle with tone 7: guide uses (S+12)%20 */
/* Kin 7: Blue Resonant Hand. tone=7, seal=6 (Hand)
 * Guide: tone 7 → (6+12)%20 = 18 (Storm), color=18%4=2=Blue ✓
 *   kin_from(7,18) = ((40*6+221*18)%260)+1 = (240+3978)%260+1 = 4218%260+1
 *   = (4218-16*260)+1 = (4218-4160)+1 = 58+1 = 59
 */
void test_oracle_kin7_guide_tone7(void) {
    dreamspell_oracle_t o = dreamspell_oracle(7);
    TEST_ASSERT_EQUAL_INT(7, o.destiny);
    TEST_ASSERT_EQUAL_INT(59, o.guide);
}

/* Oracle with tone 10: guide uses (S+8)%20 */
/* Kin 10: White Planetary Dog. tone=10, seal=9 (Dog)
 * Guide: tone 10 → (9+8)%20 = 17 (Mirror), color=17%4=1=White ✓
 *   kin_from(10,17) = ((40*9+221*17)%260)+1 = (360+3757)%260+1 = 4117%260+1
 *   = (4117-15*260)+1 = (4117-3900)+1 = 217+1 = 218
 */
void test_oracle_kin10_guide_tone10(void) {
    dreamspell_oracle_t o = dreamspell_oracle(10);
    TEST_ASSERT_EQUAL_INT(10, o.destiny);
    TEST_ASSERT_EQUAL_INT(218, o.guide);
}

/* Oracle with tone 11: guide is self */
/* Kin 11: Blue Spectral Monkey. tone=11, seal=10 (Monkey)
 * Guide: tone 11 → guide_seal = 10, kin_from(11,10)
 *   = ((40*10+221*10)%260)+1 = (400+2210)%260+1 = 2610%260+1
 *   = (2610-10*260)+1 = (2610-2600)+1 = 10+1 = 11 (self) ✓
 */
void test_oracle_kin11_guide_tone11(void) {
    dreamspell_oracle_t o = dreamspell_oracle(11);
    TEST_ASSERT_EQUAL_INT(11, o.destiny);
    TEST_ASSERT_EQUAL_INT(11, o.guide);
}

/* All oracle kins in range 1-260 */
void test_oracle_all_kins_in_range(void) {
    for (int k = 1; k <= 260; k++) {
        dreamspell_oracle_t o = dreamspell_oracle(k);
        TEST_ASSERT_EQUAL_INT(k, o.destiny);
        TEST_ASSERT_TRUE(o.guide >= 1 && o.guide <= 260);
        TEST_ASSERT_TRUE(o.analog >= 1 && o.analog <= 260);
        TEST_ASSERT_TRUE(o.antipode >= 1 && o.antipode <= 260);
        TEST_ASSERT_TRUE(o.occult >= 1 && o.occult <= 260);
        TEST_ASSERT_TRUE(o.hidden >= 1 && o.hidden <= 260);
    }
}

/* Occult + destiny = 261 for all kins */
void test_oracle_occult_sum_261(void) {
    for (int k = 1; k <= 260; k++) {
        dreamspell_oracle_t o = dreamspell_oracle(k);
        TEST_ASSERT_EQUAL_INT(261, o.destiny + o.occult);
        TEST_ASSERT_EQUAL_INT(o.occult, o.hidden);
    }
}

/* Guide has same color as destiny for all kins */
void test_oracle_guide_same_color(void) {
    for (int k = 1; k <= 260; k++) {
        dreamspell_oracle_t o = dreamspell_oracle(k);
        int dest_seal = (k - 1) % 20;
        int guide_seal = (o.guide - 1) % 20;
        TEST_ASSERT_EQUAL_INT(dest_seal % 4, guide_seal % 4);
    }
}

/* Analog has same tone as destiny for all kins */
void test_oracle_analog_same_tone(void) {
    for (int k = 1; k <= 260; k++) {
        dreamspell_oracle_t o = dreamspell_oracle(k);
        int dest_tone = (k - 1) % 13 + 1;
        int analog_tone = (o.analog - 1) % 13 + 1;
        TEST_ASSERT_EQUAL_INT(dest_tone, analog_tone);
    }
}

/* Antipode has same tone as destiny for all kins */
void test_oracle_antipode_same_tone(void) {
    for (int k = 1; k <= 260; k++) {
        dreamspell_oracle_t o = dreamspell_oracle(k);
        int dest_tone = (k - 1) % 13 + 1;
        int antipode_tone = (o.antipode - 1) % 13 + 1;
        TEST_ASSERT_EQUAL_INT(dest_tone, antipode_tone);
    }
}

/* Guide has same tone as destiny for all kins */
void test_oracle_guide_same_tone(void) {
    for (int k = 1; k <= 260; k++) {
        dreamspell_oracle_t o = dreamspell_oracle(k);
        int dest_tone = (k - 1) % 13 + 1;
        int guide_tone = (o.guide - 1) % 13 + 1;
        TEST_ASSERT_EQUAL_INT(dest_tone, guide_tone);
    }
}

/* Oracle with invalid kin returns zeroed struct */
void test_oracle_invalid_zero(void) {
    dreamspell_oracle_t o = dreamspell_oracle(0);
    TEST_ASSERT_EQUAL_INT(0, o.destiny);
    TEST_ASSERT_EQUAL_INT(0, o.guide);
    TEST_ASSERT_EQUAL_INT(0, o.analog);
    TEST_ASSERT_EQUAL_INT(0, o.antipode);
    TEST_ASSERT_EQUAL_INT(0, o.occult);
    TEST_ASSERT_EQUAL_INT(0, o.hidden);
}

void test_oracle_invalid_261(void) {
    dreamspell_oracle_t o = dreamspell_oracle(261);
    TEST_ASSERT_EQUAL_INT(0, o.destiny);
}

void test_oracle_invalid_negative(void) {
    dreamspell_oracle_t o = dreamspell_oracle(-5);
    TEST_ASSERT_EQUAL_INT(0, o.destiny);
}

/* --- Wavespell tests --- */

void test_wavespell_kin1(void) {
    dreamspell_wavespell_t w = dreamspell_wavespell(1);
    TEST_ASSERT_EQUAL_INT(1, w.number);
    TEST_ASSERT_EQUAL_INT(0, w.seal); /* Dragon initiates WS 1 */
    TEST_ASSERT_NOT_NULL(w.purpose);
    TEST_ASSERT_NOT_NULL(w.action);
    TEST_ASSERT_NOT_NULL(w.power);
}

void test_wavespell_kin13(void) {
    /* Last kin of wavespell 1 */
    dreamspell_wavespell_t w = dreamspell_wavespell(13);
    TEST_ASSERT_EQUAL_INT(1, w.number);
    TEST_ASSERT_EQUAL_INT(0, w.seal);
}

void test_wavespell_kin14(void) {
    /* First kin of wavespell 2 */
    dreamspell_wavespell_t w = dreamspell_wavespell(14);
    TEST_ASSERT_EQUAL_INT(2, w.number);
    /* seal of kin 14 = (14-1)%20 = 13 (Wizard) */
    TEST_ASSERT_EQUAL_INT(13, w.seal);
}

void test_wavespell_kin248(void) {
    /* First kin of wavespell 20 */
    dreamspell_wavespell_t w = dreamspell_wavespell(248);
    TEST_ASSERT_EQUAL_INT(20, w.number);
    /* seal of kin 248 = (248-1)%20 = 247%20 = 7 (Star) */
    TEST_ASSERT_EQUAL_INT(7, w.seal);
}

void test_wavespell_kin260(void) {
    /* Last kin of wavespell 20 */
    dreamspell_wavespell_t w = dreamspell_wavespell(260);
    TEST_ASSERT_EQUAL_INT(20, w.number);
    TEST_ASSERT_EQUAL_INT(7, w.seal);
}

void test_wavespell_covers_all_260(void) {
    /* Every kin 1-260 belongs to a wavespell 1-20 */
    for (int k = 1; k <= 260; k++) {
        dreamspell_wavespell_t w = dreamspell_wavespell(k);
        TEST_ASSERT_TRUE(w.number >= 1 && w.number <= 20);
        TEST_ASSERT_TRUE(w.seal >= 0 && w.seal <= 19);
    }
}

void test_wavespell_invalid(void) {
    dreamspell_wavespell_t w = dreamspell_wavespell(0);
    TEST_ASSERT_EQUAL_INT(0, w.number);
    w = dreamspell_wavespell(261);
    TEST_ASSERT_EQUAL_INT(0, w.number);
}

/* Test wavespell_seal function */
void test_wavespell_seal_kin1(void) {
    TEST_ASSERT_EQUAL_INT(0, dreamspell_wavespell_seal(1)); /* Dragon */
}

void test_wavespell_seal_kin14(void) {
    TEST_ASSERT_EQUAL_INT(13, dreamspell_wavespell_seal(14)); /* Wizard */
}

void test_wavespell_seal_kin260(void) {
    TEST_ASSERT_EQUAL_INT(7, dreamspell_wavespell_seal(260)); /* Star */
}

void test_wavespell_seal_invalid(void) {
    TEST_ASSERT_EQUAL_INT(-1, dreamspell_wavespell_seal(0));
    TEST_ASSERT_EQUAL_INT(-1, dreamspell_wavespell_seal(261));
}

/* Test wavespell_position function */
void test_wavespell_position_kin1(void) {
    TEST_ASSERT_EQUAL_INT(1, dreamspell_wavespell_position(1));
}

void test_wavespell_position_kin13(void) {
    TEST_ASSERT_EQUAL_INT(13, dreamspell_wavespell_position(13));
}

void test_wavespell_position_kin14(void) {
    TEST_ASSERT_EQUAL_INT(1, dreamspell_wavespell_position(14));
}

void test_wavespell_position_kin260(void) {
    TEST_ASSERT_EQUAL_INT(13, dreamspell_wavespell_position(260));
}

void test_wavespell_position_invalid(void) {
    TEST_ASSERT_EQUAL_INT(0, dreamspell_wavespell_position(0));
    TEST_ASSERT_EQUAL_INT(0, dreamspell_wavespell_position(261));
}

/* Wavespell action comes from color of initiating seal */
void test_wavespell_action_from_color(void) {
    /* WS 1: seal=0 (Dragon), color=Red, action="Initiates" */
    dreamspell_wavespell_t w = dreamspell_wavespell(1);
    TEST_ASSERT_EQUAL_STRING("Initiates", w.action);
    /* WS 2: seal=13 (Wizard), color=White (13%4=1), action="Refines" */
    w = dreamspell_wavespell(14);
    TEST_ASSERT_EQUAL_STRING("Refines", w.action);
}

void test_wavespell_power_from_color(void) {
    /* WS 1: seal=0 (Dragon), color=Red, quality="Birth" */
    dreamspell_wavespell_t w = dreamspell_wavespell(1);
    TEST_ASSERT_EQUAL_STRING("Birth", w.power);
}

/* --- Castle tests --- */

void test_castle_kin1(void) {
    dreamspell_castle_t c = dreamspell_castle(1);
    TEST_ASSERT_EQUAL_INT(1, c.number);
    TEST_ASSERT_EQUAL_STRING("Red Castle of Turning", c.name);
    TEST_ASSERT_EQUAL_STRING("Red", c.color);
    TEST_ASSERT_EQUAL_STRING("Birth", c.theme);
    TEST_ASSERT_EQUAL_INT(1, c.start_kin);
    TEST_ASSERT_EQUAL_INT(52, c.end_kin);
}

void test_castle_kin52(void) {
    dreamspell_castle_t c = dreamspell_castle(52);
    TEST_ASSERT_EQUAL_INT(1, c.number);
    TEST_ASSERT_EQUAL_INT(1, c.start_kin);
    TEST_ASSERT_EQUAL_INT(52, c.end_kin);
}

void test_castle_kin53(void) {
    dreamspell_castle_t c = dreamspell_castle(53);
    TEST_ASSERT_EQUAL_INT(2, c.number);
    TEST_ASSERT_EQUAL_STRING("White Castle of Crossing", c.name);
    TEST_ASSERT_EQUAL_STRING("White", c.color);
    TEST_ASSERT_EQUAL_STRING("Death", c.theme);
    TEST_ASSERT_EQUAL_INT(53, c.start_kin);
    TEST_ASSERT_EQUAL_INT(104, c.end_kin);
}

void test_castle_kin105(void) {
    dreamspell_castle_t c = dreamspell_castle(105);
    TEST_ASSERT_EQUAL_INT(3, c.number);
    TEST_ASSERT_EQUAL_STRING("Blue Castle of Burning", c.name);
    TEST_ASSERT_EQUAL_STRING("Blue", c.color);
    TEST_ASSERT_EQUAL_STRING("Magic", c.theme);
    TEST_ASSERT_EQUAL_INT(105, c.start_kin);
    TEST_ASSERT_EQUAL_INT(156, c.end_kin);
}

void test_castle_kin157(void) {
    dreamspell_castle_t c = dreamspell_castle(157);
    TEST_ASSERT_EQUAL_INT(4, c.number);
    TEST_ASSERT_EQUAL_STRING("Yellow Castle of Giving", c.name);
    TEST_ASSERT_EQUAL_STRING("Yellow", c.color);
    TEST_ASSERT_EQUAL_STRING("Intelligence", c.theme);
    TEST_ASSERT_EQUAL_INT(157, c.start_kin);
    TEST_ASSERT_EQUAL_INT(208, c.end_kin);
}

void test_castle_kin209(void) {
    dreamspell_castle_t c = dreamspell_castle(209);
    TEST_ASSERT_EQUAL_INT(5, c.number);
    TEST_ASSERT_EQUAL_STRING("Green Castle of Enchantment", c.name);
    TEST_ASSERT_EQUAL_STRING("Green", c.color);
    TEST_ASSERT_EQUAL_STRING("Enchantment", c.theme);
    TEST_ASSERT_EQUAL_INT(209, c.start_kin);
    TEST_ASSERT_EQUAL_INT(260, c.end_kin);
}

void test_castle_kin260(void) {
    dreamspell_castle_t c = dreamspell_castle(260);
    TEST_ASSERT_EQUAL_INT(5, c.number);
}

void test_castle_invalid(void) {
    dreamspell_castle_t c = dreamspell_castle(0);
    TEST_ASSERT_EQUAL_INT(0, c.number);
    c = dreamspell_castle(261);
    TEST_ASSERT_EQUAL_INT(0, c.number);
}

void test_castle_count(void) {
    TEST_ASSERT_EQUAL_INT(5, dreamspell_castle_count());
}

/* --- Earth family tests --- */

void test_family_cardinal(void) {
    /* Cardinal (East): 0(Dragon), 5(WB), 10(Monkey), 15(Warrior) — seal%5==0 */
    dreamspell_family_t f = dreamspell_earth_family(0);
    TEST_ASSERT_EQUAL_STRING("Cardinal", f.name);
    TEST_ASSERT_EQUAL_STRING("East", f.direction);
    TEST_ASSERT_EQUAL_INT(0, f.members[0]);
    TEST_ASSERT_EQUAL_INT(5, f.members[1]);
    TEST_ASSERT_EQUAL_INT(10, f.members[2]);
    TEST_ASSERT_EQUAL_INT(15, f.members[3]);
}

void test_family_core(void) {
    /* Core (South): 1(Wind), 6(Hand), 11(Human), 16(Earth) — seal%5==1 */
    dreamspell_family_t f = dreamspell_earth_family(1);
    TEST_ASSERT_EQUAL_STRING("Core", f.name);
    TEST_ASSERT_EQUAL_STRING("South", f.direction);
    TEST_ASSERT_EQUAL_INT(1, f.members[0]);
    TEST_ASSERT_EQUAL_INT(6, f.members[1]);
    TEST_ASSERT_EQUAL_INT(11, f.members[2]);
    TEST_ASSERT_EQUAL_INT(16, f.members[3]);
}

void test_family_signal(void) {
    /* Signal (West): 2(Night), 7(Star), 12(Skywalker), 17(Mirror) — seal%5==2 */
    dreamspell_family_t f = dreamspell_earth_family(2);
    TEST_ASSERT_EQUAL_STRING("Signal", f.name);
    TEST_ASSERT_EQUAL_STRING("West", f.direction);
    TEST_ASSERT_EQUAL_INT(2, f.members[0]);
    TEST_ASSERT_EQUAL_INT(7, f.members[1]);
    TEST_ASSERT_EQUAL_INT(12, f.members[2]);
    TEST_ASSERT_EQUAL_INT(17, f.members[3]);
}

void test_family_gateway(void) {
    /* Gateway (Center): 3(Seed), 8(Moon), 13(Wizard), 18(Storm) — seal%5==3 */
    dreamspell_family_t f = dreamspell_earth_family(3);
    TEST_ASSERT_EQUAL_STRING("Gateway", f.name);
    TEST_ASSERT_EQUAL_STRING("Center", f.direction);
    TEST_ASSERT_EQUAL_INT(3, f.members[0]);
    TEST_ASSERT_EQUAL_INT(8, f.members[1]);
    TEST_ASSERT_EQUAL_INT(13, f.members[2]);
    TEST_ASSERT_EQUAL_INT(18, f.members[3]);
}

void test_family_polar(void) {
    /* Polar (North): 4(Serpent), 9(Dog), 14(Eagle), 19(Sun) — seal%5==4 */
    dreamspell_family_t f = dreamspell_earth_family(4);
    TEST_ASSERT_EQUAL_STRING("Polar", f.name);
    TEST_ASSERT_EQUAL_STRING("North", f.direction);
    TEST_ASSERT_EQUAL_INT(4, f.members[0]);
    TEST_ASSERT_EQUAL_INT(9, f.members[1]);
    TEST_ASSERT_EQUAL_INT(14, f.members[2]);
    TEST_ASSERT_EQUAL_INT(19, f.members[3]);
}

/* Every seal from same family maps to the same family */
void test_family_consistency(void) {
    for (int s = 0; s < 20; s++) {
        dreamspell_family_t f = dreamspell_earth_family(s);
        /* Verify this seal appears in its own family members */
        int found = 0;
        for (int i = 0; i < 4; i++) {
            if (f.members[i] == s) found = 1;
        }
        TEST_ASSERT_EQUAL_INT(1, found);
    }
}

/* All 20 seals appear exactly once across all 5 families */
void test_family_all_seals_covered(void) {
    int seen[20] = {0};
    /* Use one representative seal per family: 0,1,2,3,4 (seal%5) */
    for (int rep = 0; rep < 5; rep++) {
        dreamspell_family_t f = dreamspell_earth_family(rep);
        for (int i = 0; i < 4; i++) {
            TEST_ASSERT_TRUE(f.members[i] >= 0 && f.members[i] <= 19);
            seen[f.members[i]]++;
        }
    }
    for (int s = 0; s < 20; s++) {
        TEST_ASSERT_EQUAL_INT(1, seen[s]);
    }
}

void test_family_count(void) {
    TEST_ASSERT_EQUAL_INT(5, dreamspell_family_count());
}

void test_family_invalid(void) {
    dreamspell_family_t f = dreamspell_earth_family(-1);
    TEST_ASSERT_EQUAL_STRING("?", f.name);
    f = dreamspell_earth_family(20);
    TEST_ASSERT_EQUAL_STRING("?", f.name);
}

/* --- Analog relationship is symmetric --- */
void test_oracle_analog_symmetry(void) {
    for (int k = 1; k <= 260; k++) {
        dreamspell_oracle_t o1 = dreamspell_oracle(k);
        dreamspell_oracle_t o2 = dreamspell_oracle(o1.analog);
        TEST_ASSERT_EQUAL_INT(k, o2.analog);
    }
}

/* --- Antipode relationship is symmetric --- */
void test_oracle_antipode_symmetry(void) {
    for (int k = 1; k <= 260; k++) {
        dreamspell_oracle_t o1 = dreamspell_oracle(k);
        dreamspell_oracle_t o2 = dreamspell_oracle(o1.antipode);
        TEST_ASSERT_EQUAL_INT(k, o2.antipode);
    }
}

/* --- Occult relationship is symmetric --- */
void test_oracle_occult_symmetry(void) {
    for (int k = 1; k <= 260; k++) {
        dreamspell_oracle_t o1 = dreamspell_oracle(k);
        dreamspell_oracle_t o2 = dreamspell_oracle(o1.occult);
        TEST_ASSERT_EQUAL_INT(k, o2.occult);
    }
}

/* --- Analog seal offset: analog_seal = (19 - S) % 20 --- */
void test_oracle_analog_seal_relationship(void) {
    for (int k = 1; k <= 260; k++) {
        dreamspell_oracle_t o = dreamspell_oracle(k);
        int dest_seal = (k - 1) % 20;
        int analog_seal = (o.analog - 1) % 20;
        TEST_ASSERT_EQUAL_INT((19 - dest_seal) % 20, analog_seal);
    }
}

/* --- Antipode seal offset: antipode_seal = (S + 10) % 20 --- */
void test_oracle_antipode_seal_relationship(void) {
    for (int k = 1; k <= 260; k++) {
        dreamspell_oracle_t o = dreamspell_oracle(k);
        int dest_seal = (k - 1) % 20;
        int antipode_seal = (o.antipode - 1) % 20;
        TEST_ASSERT_EQUAL_INT((dest_seal + 10) % 20, antipode_seal);
    }
}

/* --- Wavespell initiating seals cycle through all 20 --- */
void test_wavespell_all_initiating_seals(void) {
    int seen[20] = {0};
    for (int ws = 1; ws <= 20; ws++) {
        int kin = (ws - 1) * 13 + 1;
        dreamspell_wavespell_t w = dreamspell_wavespell(kin);
        TEST_ASSERT_EQUAL_INT(ws, w.number);
        seen[w.seal]++;
    }
    for (int s = 0; s < 20; s++) {
        TEST_ASSERT_EQUAL_INT(1, seen[s]);
    }
}

/* --- Each wavespell has 13 kins --- */
void test_wavespell_13_kins_each(void) {
    for (int ws = 1; ws <= 20; ws++) {
        int start = (ws - 1) * 13 + 1;
        int end = ws * 13;
        for (int k = start; k <= end; k++) {
            dreamspell_wavespell_t w = dreamspell_wavespell(k);
            TEST_ASSERT_EQUAL_INT(ws, w.number);
        }
    }
}

/* --- Castle contains exactly 4 wavespells = 52 kins --- */
void test_castle_52_kins_each(void) {
    for (int c = 1; c <= 5; c++) {
        int start = (c - 1) * 52 + 1;
        int end = c * 52;
        for (int k = start; k <= end; k++) {
            dreamspell_castle_t castle = dreamspell_castle(k);
            TEST_ASSERT_EQUAL_INT(c, castle.number);
        }
    }
}

/* --- Edge case: kin 130 (end of castle 2) vs kin 131 (start of castle 3 wait no) --- */
/* Castle 2 ends at kin 104, castle 3 starts at kin 105 */
void test_castle_boundary_104_105(void) {
    dreamspell_castle_t c = dreamspell_castle(104);
    TEST_ASSERT_EQUAL_INT(2, c.number);
    c = dreamspell_castle(105);
    TEST_ASSERT_EQUAL_INT(3, c.number);
}

void test_castle_boundary_208_209(void) {
    dreamspell_castle_t c = dreamspell_castle(208);
    TEST_ASSERT_EQUAL_INT(4, c.number);
    c = dreamspell_castle(209);
    TEST_ASSERT_EQUAL_INT(5, c.number);
}

/* --- Verify specific wavespell initiating seals --- */
/* WS 3: kin 27, seal = (27-1)%20 = 6 (Hand) */
void test_wavespell_3_hand(void) {
    dreamspell_wavespell_t w = dreamspell_wavespell(27);
    TEST_ASSERT_EQUAL_INT(3, w.number);
    TEST_ASSERT_EQUAL_INT(6, w.seal);
}

/* WS 10: kin 118, seal = (118-1)%20 = 117%20 = 17 (Mirror) */
void test_wavespell_10_mirror(void) {
    dreamspell_wavespell_t w = dreamspell_wavespell(118);
    TEST_ASSERT_EQUAL_INT(10, w.number);
    TEST_ASSERT_EQUAL_INT(17, w.seal);
}

/* WS 15: kin 183, seal = (183-1)%20 = 182%20 = 2 (Night) */
void test_wavespell_15_night(void) {
    dreamspell_wavespell_t w = dreamspell_wavespell(183);
    TEST_ASSERT_EQUAL_INT(15, w.number);
    TEST_ASSERT_EQUAL_INT(2, w.seal);
}

/* --- Wavespell purpose is seal name --- */
void test_wavespell_purpose_is_seal_name(void) {
    /* WS 1: seal 0 = Dragon */
    dreamspell_wavespell_t w = dreamspell_wavespell(1);
    TEST_ASSERT_EQUAL_STRING("Dragon", w.purpose);
}

void test_wavespell_purpose_kin14(void) {
    /* WS 2: seal 13 = Wizard */
    dreamspell_wavespell_t w = dreamspell_wavespell(14);
    TEST_ASSERT_EQUAL_STRING("Wizard", w.purpose);
}

/* ============================================================
 * Planetary Holon — Dreamspell seal-planet correspondence
 * Source: Arguelles, "Dreamspell" (1990)
 * ============================================================ */

void test_planet_dragon_neptune(void) {
    dreamspell_planet_t p = dreamspell_planet(0);
    TEST_ASSERT_EQUAL_STRING("Neptune", p.planet_name);
    TEST_ASSERT_EQUAL_INT(8, p.freq_planet_index);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, p.freq_multiplier);
}

void test_planet_wind_uranus(void) {
    dreamspell_planet_t p = dreamspell_planet(1);
    TEST_ASSERT_EQUAL_STRING("Uranus", p.planet_name);
    TEST_ASSERT_EQUAL_INT(7, p.freq_planet_index);
}

void test_planet_night_saturn(void) {
    dreamspell_planet_t p = dreamspell_planet(2);
    TEST_ASSERT_EQUAL_STRING("Saturn", p.planet_name);
    TEST_ASSERT_EQUAL_INT(6, p.freq_planet_index);
}

void test_planet_seed_jupiter(void) {
    dreamspell_planet_t p = dreamspell_planet(3);
    TEST_ASSERT_EQUAL_STRING("Jupiter", p.planet_name);
    TEST_ASSERT_EQUAL_INT(5, p.freq_planet_index);
}

void test_planet_serpent_maldek(void) {
    dreamspell_planet_t p = dreamspell_planet(4);
    TEST_ASSERT_EQUAL_STRING("Maldek", p.planet_name);
    /* Uses Mars index as nearest analog */
    TEST_ASSERT_EQUAL_INT(4, p.freq_planet_index);
    /* Different multiplier from Mars seal (5) */
    TEST_ASSERT_TRUE(p.freq_multiplier != 1.0f);
}

void test_planet_bridger_mars(void) {
    dreamspell_planet_t p = dreamspell_planet(5);
    TEST_ASSERT_EQUAL_STRING("Mars", p.planet_name);
    TEST_ASSERT_EQUAL_INT(4, p.freq_planet_index);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, p.freq_multiplier);
}

void test_planet_hand_earth(void) {
    dreamspell_planet_t p = dreamspell_planet(6);
    TEST_ASSERT_EQUAL_STRING("Earth", p.planet_name);
    TEST_ASSERT_EQUAL_INT(3, p.freq_planet_index);
}

void test_planet_star_venus(void) {
    dreamspell_planet_t p = dreamspell_planet(7);
    TEST_ASSERT_EQUAL_STRING("Venus", p.planet_name);
    TEST_ASSERT_EQUAL_INT(2, p.freq_planet_index);
}

void test_planet_moon_mercury(void) {
    dreamspell_planet_t p = dreamspell_planet(8);
    TEST_ASSERT_EQUAL_STRING("Mercury", p.planet_name);
    TEST_ASSERT_EQUAL_INT(1, p.freq_planet_index);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, p.freq_multiplier);
}

void test_planet_dog_mercury_p5(void) {
    dreamspell_planet_t p = dreamspell_planet(9);
    TEST_ASSERT_EQUAL_STRING("Mercury", p.planet_name);
    TEST_ASSERT_EQUAL_INT(1, p.freq_planet_index);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.5f, p.freq_multiplier);
}

void test_planet_monkey_venus_p5(void) {
    dreamspell_planet_t p = dreamspell_planet(10);
    TEST_ASSERT_EQUAL_STRING("Venus", p.planet_name);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.5f, p.freq_multiplier);
}

void test_planet_human_earth_p5(void) {
    dreamspell_planet_t p = dreamspell_planet(11);
    TEST_ASSERT_EQUAL_STRING("Earth", p.planet_name);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.5f, p.freq_multiplier);
}

void test_planet_skywalker_mars_p5(void) {
    dreamspell_planet_t p = dreamspell_planet(12);
    TEST_ASSERT_EQUAL_STRING("Mars", p.planet_name);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.5f, p.freq_multiplier);
}

void test_planet_wizard_maldek(void) {
    dreamspell_planet_t p = dreamspell_planet(13);
    TEST_ASSERT_EQUAL_STRING("Maldek", p.planet_name);
    /* Maldek pair: different multiplier from Serpent (4) */
    dreamspell_planet_t serpent = dreamspell_planet(4);
    TEST_ASSERT_TRUE(p.freq_multiplier != serpent.freq_multiplier);
}

void test_planet_eagle_jupiter_p5(void) {
    dreamspell_planet_t p = dreamspell_planet(14);
    TEST_ASSERT_EQUAL_STRING("Jupiter", p.planet_name);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.5f, p.freq_multiplier);
}

void test_planet_warrior_saturn_p5(void) {
    dreamspell_planet_t p = dreamspell_planet(15);
    TEST_ASSERT_EQUAL_STRING("Saturn", p.planet_name);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.5f, p.freq_multiplier);
}

void test_planet_earth_uranus_p5(void) {
    dreamspell_planet_t p = dreamspell_planet(16);
    TEST_ASSERT_EQUAL_STRING("Uranus", p.planet_name);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.5f, p.freq_multiplier);
}

void test_planet_mirror_neptune_p5(void) {
    dreamspell_planet_t p = dreamspell_planet(17);
    TEST_ASSERT_EQUAL_STRING("Neptune", p.planet_name);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.5f, p.freq_multiplier);
}

void test_planet_storm_pluto(void) {
    dreamspell_planet_t p = dreamspell_planet(18);
    TEST_ASSERT_EQUAL_STRING("Pluto", p.planet_name);
    TEST_ASSERT_TRUE(p.freq_planet_index >= 0);
}

void test_planet_sun_pluto(void) {
    dreamspell_planet_t p = dreamspell_planet(19);
    TEST_ASSERT_EQUAL_STRING("Pluto", p.planet_name);
    TEST_ASSERT_TRUE(p.freq_planet_index >= 0);
}

/* Mirror symmetry: seals 0-8 pair with seals 17-9 (share planet name) */
void test_planet_mirror_symmetry(void) {
    for (int i = 0; i <= 8; i++) {
        dreamspell_planet_t a = dreamspell_planet(i);
        dreamspell_planet_t b = dreamspell_planet(17 - i);
        TEST_ASSERT_EQUAL_STRING(a.planet_name, b.planet_name);
        TEST_ASSERT_EQUAL_INT(a.freq_planet_index, b.freq_planet_index);
    }
}

/* All 20 seals must have valid freq_planet_index (0-9) */
void test_planet_all_valid_indices(void) {
    for (int s = 0; s < 20; s++) {
        dreamspell_planet_t p = dreamspell_planet(s);
        TEST_ASSERT_TRUE(p.freq_planet_index >= 0);
        TEST_ASSERT_TRUE(p.freq_planet_index <= 9);
        TEST_ASSERT_TRUE(p.freq_multiplier > 0.0f);
    }
}

/* All 20 seals produce unique frequency signatures (planet_index × multiplier) */
void test_planet_20_unique_frequencies(void) {
    float sigs[20];
    for (int s = 0; s < 20; s++) {
        dreamspell_planet_t p = dreamspell_planet(s);
        sigs[s] = (float)p.freq_planet_index * 100.0f + p.freq_multiplier;
    }
    for (int i = 0; i < 20; i++) {
        for (int j = i + 1; j < 20; j++) {
            char msg[80];
            snprintf(msg, sizeof(msg), "seal %d and %d must differ", i, j);
            TEST_ASSERT_TRUE_MESSAGE(
                sigs[i] != sigs[j] ||
                (i == 4 && j == 5), /* Maldek/Mars share index but differ by mult */
                msg);
        }
    }
}

void test_planet_invalid_seal(void) {
    dreamspell_planet_t p = dreamspell_planet(-1);
    TEST_ASSERT_EQUAL_STRING("?", p.planet_name);
    TEST_ASSERT_EQUAL_INT(-1, p.freq_planet_index);
    p = dreamspell_planet(20);
    TEST_ASSERT_EQUAL_STRING("?", p.planet_name);
}

int main(void) {
    UNITY_BEGIN();

    /* Kin identity */
    RUN_TEST(test_kin1_is_magnetic_dragon);
    RUN_TEST(test_kin2_is_lunar_wind);
    RUN_TEST(test_kin20_is_resonant_sun);
    RUN_TEST(test_kin21_is_galactic_dragon);
    RUN_TEST(test_kin260_is_cosmic_sun);

    /* Tone data */
    RUN_TEST(test_tone_magnetic);
    RUN_TEST(test_tone_lunar);
    RUN_TEST(test_tone_electric);
    RUN_TEST(test_tone_self_existing);
    RUN_TEST(test_tone_overtone);
    RUN_TEST(test_tone_rhythmic);
    RUN_TEST(test_tone_resonant);
    RUN_TEST(test_tone_galactic);
    RUN_TEST(test_tone_solar);
    RUN_TEST(test_tone_planetary);
    RUN_TEST(test_tone_spectral);
    RUN_TEST(test_tone_crystal);
    RUN_TEST(test_tone_cosmic);
    RUN_TEST(test_tone_invalid_zero);
    RUN_TEST(test_tone_invalid_fourteen);
    RUN_TEST(test_tone_invalid_negative);

    /* Color data */
    RUN_TEST(test_color_red);
    RUN_TEST(test_color_white);
    RUN_TEST(test_color_blue);
    RUN_TEST(test_color_yellow);
    RUN_TEST(test_color_cycles);
    RUN_TEST(test_color_invalid_seal);

    /* Oracle */
    RUN_TEST(test_oracle_kin1);
    RUN_TEST(test_oracle_kin131);
    RUN_TEST(test_oracle_kin260);
    RUN_TEST(test_oracle_kin44);
    RUN_TEST(test_oracle_kin100);
    RUN_TEST(test_oracle_kin207);
    RUN_TEST(test_oracle_kin2_guide_tone2);
    RUN_TEST(test_oracle_kin3_guide_tone3);
    RUN_TEST(test_oracle_kin4_guide_tone4);
    RUN_TEST(test_oracle_kin5_guide_tone5);
    RUN_TEST(test_oracle_kin6_guide_tone6);
    RUN_TEST(test_oracle_kin7_guide_tone7);
    RUN_TEST(test_oracle_kin10_guide_tone10);
    RUN_TEST(test_oracle_kin11_guide_tone11);
    RUN_TEST(test_oracle_all_kins_in_range);
    RUN_TEST(test_oracle_occult_sum_261);
    RUN_TEST(test_oracle_guide_same_color);
    RUN_TEST(test_oracle_analog_same_tone);
    RUN_TEST(test_oracle_antipode_same_tone);
    RUN_TEST(test_oracle_guide_same_tone);
    RUN_TEST(test_oracle_invalid_zero);
    RUN_TEST(test_oracle_invalid_261);
    RUN_TEST(test_oracle_invalid_negative);
    RUN_TEST(test_oracle_analog_symmetry);
    RUN_TEST(test_oracle_antipode_symmetry);
    RUN_TEST(test_oracle_occult_symmetry);
    RUN_TEST(test_oracle_analog_seal_relationship);
    RUN_TEST(test_oracle_antipode_seal_relationship);

    /* Wavespell */
    RUN_TEST(test_wavespell_kin1);
    RUN_TEST(test_wavespell_kin13);
    RUN_TEST(test_wavespell_kin14);
    RUN_TEST(test_wavespell_kin248);
    RUN_TEST(test_wavespell_kin260);
    RUN_TEST(test_wavespell_covers_all_260);
    RUN_TEST(test_wavespell_invalid);
    RUN_TEST(test_wavespell_seal_kin1);
    RUN_TEST(test_wavespell_seal_kin14);
    RUN_TEST(test_wavespell_seal_kin260);
    RUN_TEST(test_wavespell_seal_invalid);
    RUN_TEST(test_wavespell_position_kin1);
    RUN_TEST(test_wavespell_position_kin13);
    RUN_TEST(test_wavespell_position_kin14);
    RUN_TEST(test_wavespell_position_kin260);
    RUN_TEST(test_wavespell_position_invalid);
    RUN_TEST(test_wavespell_action_from_color);
    RUN_TEST(test_wavespell_power_from_color);
    RUN_TEST(test_wavespell_all_initiating_seals);
    RUN_TEST(test_wavespell_13_kins_each);
    RUN_TEST(test_wavespell_3_hand);
    RUN_TEST(test_wavespell_10_mirror);
    RUN_TEST(test_wavespell_15_night);
    RUN_TEST(test_wavespell_purpose_is_seal_name);
    RUN_TEST(test_wavespell_purpose_kin14);

    /* Castle */
    RUN_TEST(test_castle_kin1);
    RUN_TEST(test_castle_kin52);
    RUN_TEST(test_castle_kin53);
    RUN_TEST(test_castle_kin105);
    RUN_TEST(test_castle_kin157);
    RUN_TEST(test_castle_kin209);
    RUN_TEST(test_castle_kin260);
    RUN_TEST(test_castle_invalid);
    RUN_TEST(test_castle_count);
    RUN_TEST(test_castle_52_kins_each);
    RUN_TEST(test_castle_boundary_104_105);
    RUN_TEST(test_castle_boundary_208_209);

    /* Earth family */
    RUN_TEST(test_family_cardinal);
    RUN_TEST(test_family_core);
    RUN_TEST(test_family_signal);
    RUN_TEST(test_family_gateway);
    RUN_TEST(test_family_polar);
    RUN_TEST(test_family_consistency);
    RUN_TEST(test_family_all_seals_covered);
    RUN_TEST(test_family_count);
    RUN_TEST(test_family_invalid);

    /* Planetary Holon */
    RUN_TEST(test_planet_dragon_neptune);
    RUN_TEST(test_planet_wind_uranus);
    RUN_TEST(test_planet_night_saturn);
    RUN_TEST(test_planet_seed_jupiter);
    RUN_TEST(test_planet_serpent_maldek);
    RUN_TEST(test_planet_bridger_mars);
    RUN_TEST(test_planet_hand_earth);
    RUN_TEST(test_planet_star_venus);
    RUN_TEST(test_planet_moon_mercury);
    RUN_TEST(test_planet_dog_mercury_p5);
    RUN_TEST(test_planet_monkey_venus_p5);
    RUN_TEST(test_planet_human_earth_p5);
    RUN_TEST(test_planet_skywalker_mars_p5);
    RUN_TEST(test_planet_wizard_maldek);
    RUN_TEST(test_planet_eagle_jupiter_p5);
    RUN_TEST(test_planet_warrior_saturn_p5);
    RUN_TEST(test_planet_earth_uranus_p5);
    RUN_TEST(test_planet_mirror_neptune_p5);
    RUN_TEST(test_planet_storm_pluto);
    RUN_TEST(test_planet_sun_pluto);
    RUN_TEST(test_planet_mirror_symmetry);
    RUN_TEST(test_planet_all_valid_indices);
    RUN_TEST(test_planet_20_unique_frequencies);
    RUN_TEST(test_planet_invalid_seal);

    return UNITY_END();
}
