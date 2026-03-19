#include "../../unity/unity.h"
#include "../../../src/systems/tzolkin/tzolkin_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ============================================================
 * ti_seal_data — archetype data for 20 seals
 * ============================================================ */

void test_seal_data_dragon(void) {
    ti_seal_t s = ti_seal_data(0);
    TEST_ASSERT_EQUAL_INT(0, s.seal);
    TEST_ASSERT_EQUAL_STRING("Primal nurturance, the mother of creation", s.archetype);
    TEST_ASSERT_EQUAL_STRING("What do I need to nurture into being?", s.question);
    TEST_ASSERT_EQUAL_STRING("Birth", s.power);
    TEST_ASSERT_EQUAL_STRING("Nurtures", s.action);
    TEST_ASSERT_EQUAL_STRING("Being", s.essence);
}

void test_seal_data_hand(void) {
    ti_seal_t s = ti_seal_data(6);
    TEST_ASSERT_EQUAL_INT(6, s.seal);
    TEST_ASSERT_EQUAL_STRING("Healing, accomplishment, knowledge through doing",
                             s.archetype);
    TEST_ASSERT_EQUAL_STRING("What can I heal through action?", s.question);
    TEST_ASSERT_EQUAL_STRING("Accomplishment", s.power);
    TEST_ASSERT_EQUAL_STRING("Knows", s.action);
    TEST_ASSERT_EQUAL_STRING("Healing", s.essence);
}

void test_seal_data_eagle(void) {
    ti_seal_t s = ti_seal_data(14);
    TEST_ASSERT_EQUAL_INT(14, s.seal);
    TEST_ASSERT_EQUAL_STRING("Vision, the mind's eye, seeing the whole pattern",
                             s.archetype);
    TEST_ASSERT_EQUAL_STRING("What can I see from above?", s.question);
    TEST_ASSERT_EQUAL_STRING("Vision", s.power);
    TEST_ASSERT_EQUAL_STRING("Creates", s.action);
    TEST_ASSERT_EQUAL_STRING("Mind", s.essence);
}

void test_seal_data_sun(void) {
    ti_seal_t s = ti_seal_data(19);
    TEST_ASSERT_EQUAL_INT(19, s.seal);
    TEST_ASSERT_EQUAL_STRING("Universal fire, enlightenment, wholeness, completion",
                             s.archetype);
    TEST_ASSERT_EQUAL_STRING("What must I illuminate?", s.question);
    TEST_ASSERT_EQUAL_STRING("Universal Fire", s.power);
    TEST_ASSERT_EQUAL_STRING("Enlightens", s.action);
    TEST_ASSERT_EQUAL_STRING("Life", s.essence);
}

void test_seal_data_all_20_valid(void) {
    for (int i = 0; i < 20; i++) {
        ti_seal_t s = ti_seal_data(i);
        TEST_ASSERT_EQUAL_INT(i, s.seal);
        TEST_ASSERT_NOT_NULL(s.archetype);
        TEST_ASSERT_NOT_NULL(s.question);
        TEST_ASSERT_NOT_NULL(s.power);
        TEST_ASSERT_NOT_NULL(s.action);
        TEST_ASSERT_NOT_NULL(s.essence);
        /* All fields non-empty */
        TEST_ASSERT_TRUE(strlen(s.archetype) > 0);
        TEST_ASSERT_TRUE(strlen(s.question) > 0);
        TEST_ASSERT_TRUE(strlen(s.power) > 0);
        TEST_ASSERT_TRUE(strlen(s.action) > 0);
        TEST_ASSERT_TRUE(strlen(s.essence) > 0);
    }
}

void test_seal_data_invalid_negative(void) {
    ti_seal_t s = ti_seal_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, s.seal);
    TEST_ASSERT_EQUAL_STRING("?", s.archetype);
    TEST_ASSERT_EQUAL_STRING("?", s.question);
    TEST_ASSERT_EQUAL_STRING("?", s.power);
    TEST_ASSERT_EQUAL_STRING("?", s.action);
    TEST_ASSERT_EQUAL_STRING("?", s.essence);
}

void test_seal_data_invalid_20(void) {
    ti_seal_t s = ti_seal_data(20);
    TEST_ASSERT_EQUAL_INT(-1, s.seal);
    TEST_ASSERT_EQUAL_STRING("?", s.archetype);
}

/* ============================================================
 * ti_seal_count
 * ============================================================ */

void test_seal_count(void) {
    TEST_ASSERT_EQUAL_INT(20, ti_seal_count());
}

/* ============================================================
 * ti_color_meaning — 4 colors
 * ============================================================ */

void test_color_meaning_red(void) {
    const char *m = ti_color_meaning(0);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_TRUE(strstr(m, "Initiates") != NULL);
    TEST_ASSERT_TRUE(strstr(m, "power") != NULL);
    TEST_ASSERT_TRUE(strstr(m, "birth") != NULL);
}

void test_color_meaning_white(void) {
    const char *m = ti_color_meaning(1);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_TRUE(strstr(m, "Refines") != NULL);
    TEST_ASSERT_TRUE(strstr(m, "power") != NULL);
    TEST_ASSERT_TRUE(strstr(m, "death") != NULL);
}

void test_color_meaning_blue(void) {
    const char *m = ti_color_meaning(2);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_TRUE(strstr(m, "Transforms") != NULL);
    TEST_ASSERT_TRUE(strstr(m, "power") != NULL);
    TEST_ASSERT_TRUE(strstr(m, "magic") != NULL);
}

void test_color_meaning_yellow(void) {
    const char *m = ti_color_meaning(3);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_TRUE(strstr(m, "Ripens") != NULL);
    TEST_ASSERT_TRUE(strstr(m, "power") != NULL);
    TEST_ASSERT_TRUE(strstr(m, "intelligence") != NULL);
}

void test_color_meaning_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", ti_color_meaning(-1));
    TEST_ASSERT_EQUAL_STRING("?", ti_color_meaning(4));
}

/* ============================================================
 * ti_tone_meaning — 13 tones
 * ============================================================ */

void test_tone_meaning_magnetic(void) {
    const char *m = ti_tone_meaning(1);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_TRUE(strstr(m, "Magnetic") != NULL);
    TEST_ASSERT_TRUE(strstr(m, "Unify") != NULL);
    TEST_ASSERT_TRUE(strstr(m, "Purpose") != NULL);
}

void test_tone_meaning_resonant(void) {
    const char *m = ti_tone_meaning(7);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_TRUE(strstr(m, "Resonant") != NULL);
    TEST_ASSERT_TRUE(strstr(m, "Channel") != NULL);
    TEST_ASSERT_TRUE(strstr(m, "Attunement") != NULL);
}

void test_tone_meaning_crystal(void) {
    const char *m = ti_tone_meaning(12);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_TRUE(strstr(m, "Crystal") != NULL);
    TEST_ASSERT_TRUE(strstr(m, "Dedicate") != NULL);
    TEST_ASSERT_TRUE(strstr(m, "Cooperation") != NULL);
}

void test_tone_meaning_cosmic(void) {
    const char *m = ti_tone_meaning(13);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_TRUE(strstr(m, "Cosmic") != NULL);
    TEST_ASSERT_TRUE(strstr(m, "Endure") != NULL);
    TEST_ASSERT_TRUE(strstr(m, "Transcendence") != NULL);
}

void test_tone_meaning_all_13_valid(void) {
    for (int i = 1; i <= 13; i++) {
        const char *m = ti_tone_meaning(i);
        TEST_ASSERT_NOT_NULL(m);
        TEST_ASSERT_TRUE(strlen(m) > 0);
        /* Every tone meaning should contain a dash separator */
        TEST_ASSERT_NOT_NULL(strstr(m, " — "));
    }
}

void test_tone_meaning_invalid_zero(void) {
    TEST_ASSERT_EQUAL_STRING("?", ti_tone_meaning(0));
}

void test_tone_meaning_invalid_14(void) {
    TEST_ASSERT_EQUAL_STRING("?", ti_tone_meaning(14));
}

/* ============================================================
 * ti_interpret — tiered interpretation
 * ============================================================ */

/* Kin 1: Red Magnetic Dragon
 * tone=1 (Magnetic), seal=0 (Dragon), color=Red */
void test_interpret_kin1_glyph(void) {
    tzolkin_interp_t r = ti_interpret(1, 0, 1);
    TEST_ASSERT_EQUAL_STRING("KIN 1", r.glyph);
}

void test_interpret_kin1_glance(void) {
    tzolkin_interp_t r = ti_interpret(1, 0, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Magnetic"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Dragon"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Red"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Kin 1"));
}

/* Kin 207: Blue Crystal Hand
 * tone=12 (Crystal), seal=6 (Hand), color=Blue */
void test_interpret_kin207_glance(void) {
    tzolkin_interp_t r = ti_interpret(207, 6, 12);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Crystal"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Hand"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Blue"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Kin 207"));
    /* Glance: "{tone_action} {seal_power}" -> "Dedicate Accomplishment" */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Dedicate"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Accomplishment"));
}

/* Kin 260: Yellow Cosmic Sun
 * tone=13 (Cosmic), seal=19 (Sun), color=Yellow */
void test_interpret_kin260_glance(void) {
    tzolkin_interp_t r = ti_interpret(260, 19, 13);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Cosmic"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Sun"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Yellow"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Kin 260"));
}

void test_interpret_glyph_format(void) {
    tzolkin_interp_t r1 = ti_interpret(1, 0, 1);
    TEST_ASSERT_EQUAL_STRING("KIN 1", r1.glyph);

    tzolkin_interp_t r100 = ti_interpret(100, 19, 9);
    TEST_ASSERT_EQUAL_STRING("KIN 100", r100.glyph);

    tzolkin_interp_t r260 = ti_interpret(260, 19, 13);
    TEST_ASSERT_EQUAL_STRING("KIN 260", r260.glyph);
}

/* Detail: wavespell info */
void test_interpret_detail_wavespell(void) {
    /* Kin 207: wavespell 16 (kins 196-208), seal 15=Warrior */
    tzolkin_interp_t r = ti_interpret(207, 6, 12);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Wavespell"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "wavespell"));
}

/* Detail: castle info */
void test_interpret_detail_castle(void) {
    /* Kin 207: castle 4 (Yellow Castle, kins 157-208) */
    tzolkin_interp_t r = ti_interpret(207, 6, 12);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Castle"));
}

/* Detail: oracle kins (guide, analog, antipode, occult) */
void test_interpret_detail_oracle(void) {
    tzolkin_interp_t r = ti_interpret(207, 6, 12);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "guide"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "nalog"));   /* Analog */
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "ntipode")); /* Antipode */
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "ccult"));   /* Occult */
}

/* Detail: seal question */
void test_interpret_detail_seal_question(void) {
    /* Kin 1 (Dragon): question is "What do I need to nurture into being?" */
    tzolkin_interp_t r = ti_interpret(1, 0, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "nurture"));
}

/* Detail: contains archetype info */
void test_interpret_detail_archetype(void) {
    /* Kin 207 (Hand): archetype contains "Healing" */
    tzolkin_interp_t r = ti_interpret(207, 6, 12);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Hand"));
}

/* Detail: contains tone info */
void test_interpret_detail_tone(void) {
    /* Kin 207 tone 12 = Crystal */
    tzolkin_interp_t r = ti_interpret(207, 6, 12);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Crystal"));
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Dedicate"));
}

/* Invalid kin: 0 */
void test_interpret_invalid_kin_zero(void) {
    tzolkin_interp_t r = ti_interpret(0, 0, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.glyph, "?"));
}

/* Invalid kin: 261 */
void test_interpret_invalid_kin_261(void) {
    tzolkin_interp_t r = ti_interpret(261, 0, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.glyph, "?"));
}

/* Invalid tone: 0 */
void test_interpret_invalid_tone(void) {
    tzolkin_interp_t r = ti_interpret(1, 0, 0);
    TEST_ASSERT_NOT_NULL(strstr(r.glyph, "?"));
}

/* Invalid seal: -1 */
void test_interpret_invalid_seal(void) {
    tzolkin_interp_t r = ti_interpret(1, -1, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.glyph, "?"));
}

/* Kin 1: Magnetic tone action = "Unify", Dragon power = "Birth" */
void test_interpret_kin1_glance_keywords(void) {
    tzolkin_interp_t r = ti_interpret(1, 0, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Unify"));
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Birth"));
}

/* Seal 6 (Hand) essence is "Healing" */
void test_seal_hand_essence_healing(void) {
    ti_seal_t s = ti_seal_data(6);
    TEST_ASSERT_EQUAL_STRING("Healing", s.essence);
}

/* Seal archetype: Wind */
void test_seal_data_wind(void) {
    ti_seal_t s = ti_seal_data(1);
    TEST_ASSERT_EQUAL_INT(1, s.seal);
    TEST_ASSERT_EQUAL_STRING("Divine breath, the messenger of spirit", s.archetype);
    TEST_ASSERT_EQUAL_STRING("What truth must I communicate?", s.question);
    TEST_ASSERT_EQUAL_STRING("Spirit", s.power);
    TEST_ASSERT_EQUAL_STRING("Communicates", s.action);
    TEST_ASSERT_EQUAL_STRING("Breath", s.essence);
}

/* Seal archetype: Serpent */
void test_seal_data_serpent(void) {
    ti_seal_t s = ti_seal_data(4);
    TEST_ASSERT_EQUAL_INT(4, s.seal);
    TEST_ASSERT_EQUAL_STRING("Life force, kundalini, the body's wisdom", s.archetype);
    TEST_ASSERT_EQUAL_STRING("What instinct am I ignoring?", s.question);
    TEST_ASSERT_EQUAL_STRING("Life Force", s.power);
    TEST_ASSERT_EQUAL_STRING("Survives", s.action);
    TEST_ASSERT_EQUAL_STRING("Instinct", s.essence);
}

/* Seal archetype: Monkey */
void test_seal_data_monkey(void) {
    ti_seal_t s = ti_seal_data(10);
    TEST_ASSERT_EQUAL_INT(10, s.seal);
    TEST_ASSERT_EQUAL_STRING("Magic, play, illusion, the divine trickster",
                             s.archetype);
    TEST_ASSERT_EQUAL_STRING("Where am I too serious?", s.question);
    TEST_ASSERT_EQUAL_STRING("Magic", s.power);
    TEST_ASSERT_EQUAL_STRING("Plays", s.action);
    TEST_ASSERT_EQUAL_STRING("Illusion", s.essence);
}

/* Tone 5 (Overtone) meaning */
void test_tone_meaning_overtone(void) {
    const char *m = ti_tone_meaning(5);
    TEST_ASSERT_NOT_NULL(m);
    TEST_ASSERT_NOT_NULL(strstr(m, "Overtone"));
    TEST_ASSERT_NOT_NULL(strstr(m, "Empower"));
    TEST_ASSERT_NOT_NULL(strstr(m, "Radiance"));
}

/* ============================================================
 * main
 * ============================================================ */

int main(void) {
    UNITY_BEGIN();

    /* ti_seal_data */
    RUN_TEST(test_seal_data_dragon);
    RUN_TEST(test_seal_data_hand);
    RUN_TEST(test_seal_data_eagle);
    RUN_TEST(test_seal_data_sun);
    RUN_TEST(test_seal_data_wind);
    RUN_TEST(test_seal_data_serpent);
    RUN_TEST(test_seal_data_monkey);
    RUN_TEST(test_seal_data_all_20_valid);
    RUN_TEST(test_seal_data_invalid_negative);
    RUN_TEST(test_seal_data_invalid_20);
    RUN_TEST(test_seal_hand_essence_healing);

    /* ti_seal_count */
    RUN_TEST(test_seal_count);

    /* ti_color_meaning */
    RUN_TEST(test_color_meaning_red);
    RUN_TEST(test_color_meaning_white);
    RUN_TEST(test_color_meaning_blue);
    RUN_TEST(test_color_meaning_yellow);
    RUN_TEST(test_color_meaning_invalid);

    /* ti_tone_meaning */
    RUN_TEST(test_tone_meaning_magnetic);
    RUN_TEST(test_tone_meaning_resonant);
    RUN_TEST(test_tone_meaning_crystal);
    RUN_TEST(test_tone_meaning_cosmic);
    RUN_TEST(test_tone_meaning_overtone);
    RUN_TEST(test_tone_meaning_all_13_valid);
    RUN_TEST(test_tone_meaning_invalid_zero);
    RUN_TEST(test_tone_meaning_invalid_14);

    /* ti_interpret */
    RUN_TEST(test_interpret_kin1_glyph);
    RUN_TEST(test_interpret_kin1_glance);
    RUN_TEST(test_interpret_kin1_glance_keywords);
    RUN_TEST(test_interpret_kin207_glance);
    RUN_TEST(test_interpret_kin260_glance);
    RUN_TEST(test_interpret_glyph_format);
    RUN_TEST(test_interpret_detail_wavespell);
    RUN_TEST(test_interpret_detail_castle);
    RUN_TEST(test_interpret_detail_oracle);
    RUN_TEST(test_interpret_detail_seal_question);
    RUN_TEST(test_interpret_detail_archetype);
    RUN_TEST(test_interpret_detail_tone);
    RUN_TEST(test_interpret_invalid_kin_zero);
    RUN_TEST(test_interpret_invalid_kin_261);
    RUN_TEST(test_interpret_invalid_tone);
    RUN_TEST(test_interpret_invalid_seal);

    return UNITY_END();
}
