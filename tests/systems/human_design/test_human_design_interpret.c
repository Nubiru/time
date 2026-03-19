#include "../../unity/unity.h"
#include "../../../src/systems/human_design/human_design_interpret.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ============================================================
 * hdi_type_data — archetype data for 5 HD types
 * ============================================================ */

void test_type_manifestor(void) {
    hdi_type_t t = hdi_type_data(HDI_MANIFESTOR);
    TEST_ASSERT_EQUAL_INT(HDI_MANIFESTOR, t.type);
    TEST_ASSERT_EQUAL_STRING("Manifestor", t.name);
    TEST_ASSERT_EQUAL_STRING("To Inform", t.strategy);
    TEST_ASSERT_EQUAL_STRING("Peace", t.signature);
    TEST_ASSERT_EQUAL_STRING("Anger", t.not_self);
}

void test_type_generator(void) {
    hdi_type_t t = hdi_type_data(HDI_GENERATOR);
    TEST_ASSERT_EQUAL_INT(HDI_GENERATOR, t.type);
    TEST_ASSERT_EQUAL_STRING("Generator", t.name);
    TEST_ASSERT_EQUAL_STRING("To Respond", t.strategy);
    TEST_ASSERT_EQUAL_STRING("Satisfaction", t.signature);
    TEST_ASSERT_EQUAL_STRING("Frustration", t.not_self);
}

void test_type_manifesting_generator(void) {
    hdi_type_t t = hdi_type_data(HDI_MANIFESTING_GENERATOR);
    TEST_ASSERT_EQUAL_INT(HDI_MANIFESTING_GENERATOR, t.type);
    TEST_ASSERT_EQUAL_STRING("Manifesting Generator", t.name);
    TEST_ASSERT_EQUAL_STRING("To Respond, Then Inform", t.strategy);
    TEST_ASSERT_EQUAL_STRING("Satisfaction", t.signature);
    TEST_ASSERT_EQUAL_STRING("Frustration", t.not_self);
}

void test_type_projector(void) {
    hdi_type_t t = hdi_type_data(HDI_PROJECTOR);
    TEST_ASSERT_EQUAL_INT(HDI_PROJECTOR, t.type);
    TEST_ASSERT_EQUAL_STRING("Projector", t.name);
    TEST_ASSERT_EQUAL_STRING("To Wait for the Invitation", t.strategy);
    TEST_ASSERT_EQUAL_STRING("Success", t.signature);
    TEST_ASSERT_EQUAL_STRING("Bitterness", t.not_self);
}

void test_type_reflector(void) {
    hdi_type_t t = hdi_type_data(HDI_REFLECTOR);
    TEST_ASSERT_EQUAL_INT(HDI_REFLECTOR, t.type);
    TEST_ASSERT_EQUAL_STRING("Reflector", t.name);
    TEST_ASSERT_EQUAL_STRING("To Wait a Lunar Cycle", t.strategy);
    TEST_ASSERT_EQUAL_STRING("Surprise", t.signature);
    TEST_ASSERT_EQUAL_STRING("Disappointment", t.not_self);
}

void test_type_all_5_valid(void) {
    for (int i = 0; i < 5; i++) {
        hdi_type_t t = hdi_type_data(i);
        TEST_ASSERT_EQUAL_INT(i, t.type);
        TEST_ASSERT_NOT_NULL(t.name);
        TEST_ASSERT_NOT_NULL(t.strategy);
        TEST_ASSERT_NOT_NULL(t.signature);
        TEST_ASSERT_NOT_NULL(t.not_self);
        TEST_ASSERT_NOT_NULL(t.aura);
        TEST_ASSERT_NOT_NULL(t.brief);
        TEST_ASSERT_TRUE(strlen(t.name) > 0);
        TEST_ASSERT_TRUE(strlen(t.strategy) > 0);
        TEST_ASSERT_TRUE(strlen(t.signature) > 0);
        TEST_ASSERT_TRUE(strlen(t.not_self) > 0);
        TEST_ASSERT_TRUE(strlen(t.aura) > 0);
        TEST_ASSERT_TRUE(strlen(t.brief) > 0);
    }
}

void test_type_invalid_neg1(void) {
    hdi_type_t t = hdi_type_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, t.type);
    TEST_ASSERT_EQUAL_STRING("?", t.name);
    TEST_ASSERT_EQUAL_STRING("?", t.strategy);
    TEST_ASSERT_EQUAL_STRING("?", t.signature);
    TEST_ASSERT_EQUAL_STRING("?", t.not_self);
    TEST_ASSERT_EQUAL_STRING("?", t.aura);
    TEST_ASSERT_EQUAL_STRING("?", t.brief);
}

void test_type_invalid_5(void) {
    hdi_type_t t = hdi_type_data(5);
    TEST_ASSERT_EQUAL_INT(-1, t.type);
    TEST_ASSERT_EQUAL_STRING("?", t.name);
}

/* ============================================================
 * hdi_authority_data — 7 authority types
 * ============================================================ */

void test_authority_emotional(void) {
    hdi_authority_t a = hdi_authority_data(HDI_AUTH_EMOTIONAL);
    TEST_ASSERT_EQUAL_INT(HDI_AUTH_EMOTIONAL, a.authority);
    TEST_ASSERT_EQUAL_STRING("Emotional", a.name);
    TEST_ASSERT_NOT_NULL(strstr(a.process, "wave"));
}

void test_authority_sacral(void) {
    hdi_authority_t a = hdi_authority_data(HDI_AUTH_SACRAL);
    TEST_ASSERT_EQUAL_INT(HDI_AUTH_SACRAL, a.authority);
    TEST_ASSERT_EQUAL_STRING("Sacral", a.name);
}

void test_authority_splenic(void) {
    hdi_authority_t a = hdi_authority_data(HDI_AUTH_SPLENIC);
    TEST_ASSERT_EQUAL_INT(HDI_AUTH_SPLENIC, a.authority);
    TEST_ASSERT_EQUAL_STRING("Splenic", a.name);
}

void test_authority_ego(void) {
    hdi_authority_t a = hdi_authority_data(HDI_AUTH_EGO);
    TEST_ASSERT_EQUAL_INT(HDI_AUTH_EGO, a.authority);
    TEST_ASSERT_EQUAL_STRING("Ego", a.name);
}

void test_authority_self_projected(void) {
    hdi_authority_t a = hdi_authority_data(HDI_AUTH_SELF_PROJECTED);
    TEST_ASSERT_EQUAL_INT(HDI_AUTH_SELF_PROJECTED, a.authority);
    TEST_ASSERT_EQUAL_STRING("Self-Projected", a.name);
}

void test_authority_mental(void) {
    hdi_authority_t a = hdi_authority_data(HDI_AUTH_MENTAL);
    TEST_ASSERT_EQUAL_INT(HDI_AUTH_MENTAL, a.authority);
    TEST_ASSERT_EQUAL_STRING("Mental", a.name);
}

void test_authority_lunar(void) {
    hdi_authority_t a = hdi_authority_data(HDI_AUTH_LUNAR);
    TEST_ASSERT_EQUAL_INT(HDI_AUTH_LUNAR, a.authority);
    TEST_ASSERT_EQUAL_STRING("Lunar", a.name);
}

void test_authority_all_7_valid(void) {
    for (int i = 0; i < 7; i++) {
        hdi_authority_t a = hdi_authority_data(i);
        TEST_ASSERT_EQUAL_INT(i, a.authority);
        TEST_ASSERT_NOT_NULL(a.name);
        TEST_ASSERT_NOT_NULL(a.process);
        TEST_ASSERT_NOT_NULL(a.signal);
        TEST_ASSERT_NOT_NULL(a.brief);
        TEST_ASSERT_TRUE(strlen(a.name) > 0);
        TEST_ASSERT_TRUE(strlen(a.process) > 0);
        TEST_ASSERT_TRUE(strlen(a.signal) > 0);
        TEST_ASSERT_TRUE(strlen(a.brief) > 0);
    }
}

void test_authority_invalid_neg1(void) {
    hdi_authority_t a = hdi_authority_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, a.authority);
    TEST_ASSERT_EQUAL_STRING("?", a.name);
    TEST_ASSERT_EQUAL_STRING("?", a.process);
    TEST_ASSERT_EQUAL_STRING("?", a.signal);
    TEST_ASSERT_EQUAL_STRING("?", a.brief);
}

void test_authority_invalid_7(void) {
    hdi_authority_t a = hdi_authority_data(7);
    TEST_ASSERT_EQUAL_INT(-1, a.authority);
    TEST_ASSERT_EQUAL_STRING("?", a.name);
}

/* ============================================================
 * hdi_profile_data — 12 profiles
 * ============================================================ */

void test_profile_1_3(void) {
    hdi_profile_t p = hdi_profile_data(1, 3);
    TEST_ASSERT_EQUAL_INT(1, p.line1);
    TEST_ASSERT_EQUAL_INT(3, p.line2);
    TEST_ASSERT_EQUAL_STRING("1/3", p.name);
    TEST_ASSERT_EQUAL_STRING("Investigator / Martyr", p.theme);
}

void test_profile_4_6(void) {
    hdi_profile_t p = hdi_profile_data(4, 6);
    TEST_ASSERT_EQUAL_INT(4, p.line1);
    TEST_ASSERT_EQUAL_INT(6, p.line2);
    TEST_ASSERT_EQUAL_STRING("4/6", p.name);
    TEST_ASSERT_EQUAL_STRING("Opportunist / Role Model", p.theme);
}

void test_profile_6_2(void) {
    hdi_profile_t p = hdi_profile_data(6, 2);
    TEST_ASSERT_EQUAL_INT(6, p.line1);
    TEST_ASSERT_EQUAL_INT(2, p.line2);
    TEST_ASSERT_EQUAL_STRING("6/2", p.name);
    TEST_ASSERT_EQUAL_STRING("Role Model / Hermit", p.theme);
}

void test_profile_all_12_valid(void) {
    /* The 12 valid profiles */
    int pairs[12][2] = {
        {1,3}, {1,4}, {2,4}, {2,5}, {3,5}, {3,6},
        {4,6}, {4,1}, {5,1}, {5,2}, {6,2}, {6,3}
    };
    for (int i = 0; i < 12; i++) {
        hdi_profile_t p = hdi_profile_data(pairs[i][0], pairs[i][1]);
        TEST_ASSERT_EQUAL_INT(pairs[i][0], p.line1);
        TEST_ASSERT_EQUAL_INT(pairs[i][1], p.line2);
        TEST_ASSERT_NOT_NULL(p.name);
        TEST_ASSERT_NOT_NULL(p.theme);
        TEST_ASSERT_NOT_NULL(p.conscious_role);
        TEST_ASSERT_NOT_NULL(p.unconscious_role);
        TEST_ASSERT_NOT_NULL(p.brief);
        TEST_ASSERT_TRUE(strlen(p.name) > 0);
        TEST_ASSERT_TRUE(strlen(p.theme) > 0);
        TEST_ASSERT_TRUE(strlen(p.conscious_role) > 0);
        TEST_ASSERT_TRUE(strlen(p.unconscious_role) > 0);
        TEST_ASSERT_TRUE(strlen(p.brief) > 0);
    }
}

void test_profile_invalid_1_1(void) {
    hdi_profile_t p = hdi_profile_data(1, 1);
    TEST_ASSERT_EQUAL_INT(-1, p.line1);
    TEST_ASSERT_EQUAL_STRING("?", p.name);
    TEST_ASSERT_EQUAL_STRING("?", p.theme);
}

void test_profile_invalid_0_0(void) {
    hdi_profile_t p = hdi_profile_data(0, 0);
    TEST_ASSERT_EQUAL_INT(-1, p.line1);
    TEST_ASSERT_EQUAL_STRING("?", p.name);
}

void test_profile_invalid_7_1(void) {
    hdi_profile_t p = hdi_profile_data(7, 1);
    TEST_ASSERT_EQUAL_INT(-1, p.line1);
    TEST_ASSERT_EQUAL_STRING("?", p.name);
}

/* ============================================================
 * hdi_center_data — 9 center themes
 * ============================================================ */

void test_center_head(void) {
    hdi_center_theme_t c = hdi_center_data(0);
    TEST_ASSERT_EQUAL_INT(0, c.center);
    TEST_ASSERT_EQUAL_STRING("Head", c.name);
    TEST_ASSERT_NOT_NULL(c.theme_defined);
    TEST_ASSERT_NOT_NULL(c.theme_open);
    TEST_ASSERT_NOT_NULL(c.question);
    TEST_ASSERT_NOT_NULL(c.biological);
}

void test_center_sacral(void) {
    hdi_center_theme_t c = hdi_center_data(6);
    TEST_ASSERT_EQUAL_INT(6, c.center);
    TEST_ASSERT_EQUAL_STRING("Sacral", c.name);
}

void test_center_root(void) {
    hdi_center_theme_t c = hdi_center_data(8);
    TEST_ASSERT_EQUAL_INT(8, c.center);
    TEST_ASSERT_EQUAL_STRING("Root", c.name);
}

void test_center_all_9_valid(void) {
    for (int i = 0; i < 9; i++) {
        hdi_center_theme_t c = hdi_center_data(i);
        TEST_ASSERT_EQUAL_INT(i, c.center);
        TEST_ASSERT_NOT_NULL(c.name);
        TEST_ASSERT_NOT_NULL(c.theme_defined);
        TEST_ASSERT_NOT_NULL(c.theme_open);
        TEST_ASSERT_NOT_NULL(c.question);
        TEST_ASSERT_NOT_NULL(c.biological);
        TEST_ASSERT_TRUE(strlen(c.name) > 0);
        TEST_ASSERT_TRUE(strlen(c.theme_defined) > 0);
        TEST_ASSERT_TRUE(strlen(c.theme_open) > 0);
        TEST_ASSERT_TRUE(strlen(c.question) > 0);
        TEST_ASSERT_TRUE(strlen(c.biological) > 0);
    }
}

void test_center_invalid_neg1(void) {
    hdi_center_theme_t c = hdi_center_data(-1);
    TEST_ASSERT_EQUAL_INT(-1, c.center);
    TEST_ASSERT_EQUAL_STRING("?", c.name);
    TEST_ASSERT_EQUAL_STRING("?", c.theme_defined);
    TEST_ASSERT_EQUAL_STRING("?", c.theme_open);
    TEST_ASSERT_EQUAL_STRING("?", c.question);
    TEST_ASSERT_EQUAL_STRING("?", c.biological);
}

void test_center_invalid_9(void) {
    hdi_center_theme_t c = hdi_center_data(9);
    TEST_ASSERT_EQUAL_INT(-1, c.center);
    TEST_ASSERT_EQUAL_STRING("?", c.name);
}

/* ============================================================
 * hdi_interpret — tiered composition
 * ============================================================ */

void test_interpret_glyph_generator(void) {
    human_design_interp_t r = hdi_interpret(HDI_GENERATOR, HDI_AUTH_SACRAL,
                                            1, 3, 1);
    TEST_ASSERT_EQUAL_STRING("Gen", r.glyph);
}

void test_interpret_glyph_projector(void) {
    human_design_interp_t r = hdi_interpret(HDI_PROJECTOR, HDI_AUTH_SPLENIC,
                                            3, 5, 48);
    TEST_ASSERT_EQUAL_STRING("Prj", r.glyph);
}

void test_interpret_glyph_manifestor(void) {
    human_design_interp_t r = hdi_interpret(HDI_MANIFESTOR, HDI_AUTH_EMOTIONAL,
                                            5, 1, 34);
    TEST_ASSERT_EQUAL_STRING("Mft", r.glyph);
}

void test_interpret_glyph_manifesting_gen(void) {
    human_design_interp_t r = hdi_interpret(HDI_MANIFESTING_GENERATOR,
                                            HDI_AUTH_SACRAL, 2, 4, 20);
    TEST_ASSERT_EQUAL_STRING("MaG", r.glyph);
}

void test_interpret_glyph_reflector(void) {
    human_design_interp_t r = hdi_interpret(HDI_REFLECTOR, HDI_AUTH_LUNAR,
                                            6, 2, 61);
    TEST_ASSERT_EQUAL_STRING("Ref", r.glyph);
}

void test_interpret_glance_contains_type(void) {
    human_design_interp_t r = hdi_interpret(HDI_GENERATOR, HDI_AUTH_SACRAL,
                                            1, 3, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "Generator"));
}

void test_interpret_glance_contains_profile(void) {
    human_design_interp_t r = hdi_interpret(HDI_GENERATOR, HDI_AUTH_SACRAL,
                                            1, 3, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "1/3"));
}

void test_interpret_glance_contains_gate_name(void) {
    human_design_interp_t r = hdi_interpret(HDI_GENERATOR, HDI_AUTH_SACRAL,
                                            1, 3, 1);
    /* Gate 1 = "The Creative" */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "The Creative"));
}

void test_interpret_detail_contains_strategy(void) {
    human_design_interp_t r = hdi_interpret(HDI_GENERATOR, HDI_AUTH_SACRAL,
                                            1, 3, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "To Respond"));
}

void test_interpret_detail_contains_signature(void) {
    human_design_interp_t r = hdi_interpret(HDI_GENERATOR, HDI_AUTH_SACRAL,
                                            1, 3, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Satisfaction"));
}

void test_interpret_detail_contains_not_self(void) {
    human_design_interp_t r = hdi_interpret(HDI_GENERATOR, HDI_AUTH_SACRAL,
                                            1, 3, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Frustration"));
}

void test_interpret_detail_contains_authority_name(void) {
    human_design_interp_t r = hdi_interpret(HDI_GENERATOR, HDI_AUTH_SACRAL,
                                            1, 3, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Sacral"));
}

void test_interpret_detail_contains_profile_theme(void) {
    human_design_interp_t r = hdi_interpret(HDI_GENERATOR, HDI_AUTH_SACRAL,
                                            1, 3, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Investigator / Martyr"));
}

void test_interpret_detail_contains_gate_keyword(void) {
    human_design_interp_t r = hdi_interpret(HDI_GENERATOR, HDI_AUTH_SACRAL,
                                            1, 3, 1);
    /* Gate 1 keyword = "Self-Expression" */
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "Self-Expression"));
}

void test_interpret_invalid_type(void) {
    human_design_interp_t r = hdi_interpret(99, HDI_AUTH_SACRAL, 1, 3, 1);
    TEST_ASSERT_EQUAL_STRING("?", r.glyph);
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "?"));
}

void test_interpret_invalid_authority(void) {
    human_design_interp_t r = hdi_interpret(HDI_GENERATOR, 99, 1, 3, 1);
    /* Still produces output — authority "?" embedded */
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "?"));
}

void test_interpret_invalid_profile(void) {
    human_design_interp_t r = hdi_interpret(HDI_GENERATOR, HDI_AUTH_SACRAL,
                                            0, 0, 1);
    TEST_ASSERT_NOT_NULL(strstr(r.detail, "?"));
}

void test_interpret_invalid_gate(void) {
    human_design_interp_t r = hdi_interpret(HDI_GENERATOR, HDI_AUTH_SACRAL,
                                            1, 3, 0);
    /* Gate 0 is invalid, gate name returns "?" */
    TEST_ASSERT_NOT_NULL(strstr(r.glance, "?"));
}

/* ============================================================
 * Counts
 * ============================================================ */

void test_type_count(void) {
    TEST_ASSERT_EQUAL_INT(5, hdi_type_count());
}

void test_authority_count(void) {
    TEST_ASSERT_EQUAL_INT(7, hdi_authority_count());
}

void test_center_count(void) {
    TEST_ASSERT_EQUAL_INT(9, hdi_center_count());
}

/* ============================================================
 * Runner
 * ============================================================ */

int main(void) {
    UNITY_BEGIN();

    /* hdi_type_data */
    RUN_TEST(test_type_manifestor);
    RUN_TEST(test_type_generator);
    RUN_TEST(test_type_manifesting_generator);
    RUN_TEST(test_type_projector);
    RUN_TEST(test_type_reflector);
    RUN_TEST(test_type_all_5_valid);
    RUN_TEST(test_type_invalid_neg1);
    RUN_TEST(test_type_invalid_5);

    /* hdi_authority_data */
    RUN_TEST(test_authority_emotional);
    RUN_TEST(test_authority_sacral);
    RUN_TEST(test_authority_splenic);
    RUN_TEST(test_authority_ego);
    RUN_TEST(test_authority_self_projected);
    RUN_TEST(test_authority_mental);
    RUN_TEST(test_authority_lunar);
    RUN_TEST(test_authority_all_7_valid);
    RUN_TEST(test_authority_invalid_neg1);
    RUN_TEST(test_authority_invalid_7);

    /* hdi_profile_data */
    RUN_TEST(test_profile_1_3);
    RUN_TEST(test_profile_4_6);
    RUN_TEST(test_profile_6_2);
    RUN_TEST(test_profile_all_12_valid);
    RUN_TEST(test_profile_invalid_1_1);
    RUN_TEST(test_profile_invalid_0_0);
    RUN_TEST(test_profile_invalid_7_1);

    /* hdi_center_data */
    RUN_TEST(test_center_head);
    RUN_TEST(test_center_sacral);
    RUN_TEST(test_center_root);
    RUN_TEST(test_center_all_9_valid);
    RUN_TEST(test_center_invalid_neg1);
    RUN_TEST(test_center_invalid_9);

    /* hdi_interpret */
    RUN_TEST(test_interpret_glyph_generator);
    RUN_TEST(test_interpret_glyph_projector);
    RUN_TEST(test_interpret_glyph_manifestor);
    RUN_TEST(test_interpret_glyph_manifesting_gen);
    RUN_TEST(test_interpret_glyph_reflector);
    RUN_TEST(test_interpret_glance_contains_type);
    RUN_TEST(test_interpret_glance_contains_profile);
    RUN_TEST(test_interpret_glance_contains_gate_name);
    RUN_TEST(test_interpret_detail_contains_strategy);
    RUN_TEST(test_interpret_detail_contains_signature);
    RUN_TEST(test_interpret_detail_contains_not_self);
    RUN_TEST(test_interpret_detail_contains_authority_name);
    RUN_TEST(test_interpret_detail_contains_profile_theme);
    RUN_TEST(test_interpret_detail_contains_gate_keyword);
    RUN_TEST(test_interpret_invalid_type);
    RUN_TEST(test_interpret_invalid_authority);
    RUN_TEST(test_interpret_invalid_profile);
    RUN_TEST(test_interpret_invalid_gate);

    /* counts */
    RUN_TEST(test_type_count);
    RUN_TEST(test_authority_count);
    RUN_TEST(test_center_count);

    return UNITY_END();
}
