/* test_diamond_room.c — Tests for Diamond Room procedural crystal data.
 * TDD RED phase: tests written before implementation. */

#include "../../unity/unity.h"
#include "../../../src/systems/earth/diamond_room.h"

#include <math.h>
#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* ===== dr_default_input ===== */

void test_default_input_kin_zero(void) {
    dr_input_t in = dr_default_input();
    TEST_ASSERT_EQUAL_INT(0, in.kin);
}

void test_default_input_seal_negative(void) {
    dr_input_t in = dr_default_input();
    TEST_ASSERT_EQUAL_INT(-1, in.seal);
}

void test_default_input_tone_zero(void) {
    dr_input_t in = dr_default_input();
    TEST_ASSERT_EQUAL_INT(0, in.tone);
}

void test_default_input_sun_sign_negative(void) {
    dr_input_t in = dr_default_input();
    TEST_ASSERT_EQUAL_INT(-1, in.sun_sign);
}

void test_default_input_hexagram_zero(void) {
    dr_input_t in = dr_default_input();
    TEST_ASSERT_EQUAL_INT(0, in.hexagram);
}

void test_default_input_systems_explored_zero(void) {
    dr_input_t in = dr_default_input();
    TEST_ASSERT_EQUAL_INT(0, in.systems_explored);
}

void test_default_input_hour_fraction_zero(void) {
    dr_input_t in = dr_default_input();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)in.hour_fraction);
}

void test_default_input_explored_all_zero(void) {
    dr_input_t in = dr_default_input();
    for (int i = 0; i < DR_MAX_FACETS; i++) {
        TEST_ASSERT_EQUAL_INT(0, in.explored[i]);
    }
}

void test_default_input_engagement_all_zero(void) {
    dr_input_t in = dr_default_input();
    for (int i = 0; i < DR_MAX_FACETS; i++) {
        TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)in.engagement[i]);
    }
}

/* ===== dr_compute_seed — crystal system from zodiac ===== */

void test_seed_fire_sign_aries_cubic(void) {
    dr_input_t in = dr_default_input();
    in.sun_sign = 0; /* Aries */
    dr_seed_t s = dr_compute_seed(&in);
    TEST_ASSERT_EQUAL_INT(DR_CRYSTAL_CUBIC, s.system);
}

void test_seed_fire_sign_leo_cubic(void) {
    dr_input_t in = dr_default_input();
    in.sun_sign = 4; /* Leo */
    dr_seed_t s = dr_compute_seed(&in);
    TEST_ASSERT_EQUAL_INT(DR_CRYSTAL_CUBIC, s.system);
}

void test_seed_fire_sign_sagittarius_cubic(void) {
    dr_input_t in = dr_default_input();
    in.sun_sign = 8; /* Sagittarius */
    dr_seed_t s = dr_compute_seed(&in);
    TEST_ASSERT_EQUAL_INT(DR_CRYSTAL_CUBIC, s.system);
}

void test_seed_earth_sign_taurus_hexagonal(void) {
    dr_input_t in = dr_default_input();
    in.sun_sign = 1; /* Taurus */
    dr_seed_t s = dr_compute_seed(&in);
    TEST_ASSERT_EQUAL_INT(DR_CRYSTAL_HEXAGONAL, s.system);
}

void test_seed_earth_sign_virgo_hexagonal(void) {
    dr_input_t in = dr_default_input();
    in.sun_sign = 5; /* Virgo */
    dr_seed_t s = dr_compute_seed(&in);
    TEST_ASSERT_EQUAL_INT(DR_CRYSTAL_HEXAGONAL, s.system);
}

void test_seed_air_sign_gemini_tetragonal(void) {
    dr_input_t in = dr_default_input();
    in.sun_sign = 2; /* Gemini */
    dr_seed_t s = dr_compute_seed(&in);
    TEST_ASSERT_EQUAL_INT(DR_CRYSTAL_TETRAGONAL, s.system);
}

void test_seed_water_sign_cancer_orthorhombic(void) {
    dr_input_t in = dr_default_input();
    in.sun_sign = 3; /* Cancer */
    dr_seed_t s = dr_compute_seed(&in);
    TEST_ASSERT_EQUAL_INT(DR_CRYSTAL_ORTHORHOMBIC, s.system);
}

void test_seed_unknown_zodiac_monoclinic(void) {
    dr_input_t in = dr_default_input();
    /* sun_sign = -1 (default) */
    dr_seed_t s = dr_compute_seed(&in);
    TEST_ASSERT_EQUAL_INT(DR_CRYSTAL_MONOCLINIC, s.system);
}

/* ===== dr_compute_seed — symmetry from castle ===== */

void test_seed_castle_0_symmetry_2(void) {
    dr_input_t in = dr_default_input();
    in.castle = 0;
    dr_seed_t s = dr_compute_seed(&in);
    TEST_ASSERT_EQUAL_INT(2, s.symmetry_order);
}

void test_seed_castle_4_symmetry_6(void) {
    dr_input_t in = dr_default_input();
    in.castle = 4;
    dr_seed_t s = dr_compute_seed(&in);
    TEST_ASSERT_EQUAL_INT(6, s.symmetry_order);
}

void test_seed_castle_unknown_symmetry_4(void) {
    dr_input_t in = dr_default_input();
    /* castle = -1 default */
    dr_seed_t s = dr_compute_seed(&in);
    TEST_ASSERT_EQUAL_INT(4, s.symmetry_order);
}

/* ===== dr_compute_seed — hue from hexagram ===== */

void test_seed_hexagram_1_hue_golden_angle(void) {
    dr_input_t in = dr_default_input();
    in.hexagram = 1;
    dr_seed_t s = dr_compute_seed(&in);
    double expected = fmod(1.0 * DR_GOLDEN_ANGLE_N, 1.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)expected, (float)s.hue_base);
}

void test_seed_hexagram_32_hue_wraps(void) {
    dr_input_t in = dr_default_input();
    in.hexagram = 32;
    dr_seed_t s = dr_compute_seed(&in);
    double expected = fmod(32.0 * DR_GOLDEN_ANGLE_N, 1.0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)expected, (float)s.hue_base);
    TEST_ASSERT_TRUE(s.hue_base >= 0.0);
    TEST_ASSERT_TRUE(s.hue_base < 1.0);
}

void test_seed_hexagram_unknown_hue_zero(void) {
    dr_input_t in = dr_default_input();
    /* hexagram = 0 (unknown) */
    dr_seed_t s = dr_compute_seed(&in);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)s.hue_base);
}

/* ===== dr_compute_seed — accent hue from seal color ===== */

void test_seed_seal_red_accent(void) {
    dr_input_t in = dr_default_input();
    in.seal = 0; /* Red Dragon — color 0 (Red) */
    dr_seed_t s = dr_compute_seed(&in);
    /* Red seals (0,4,8,12,16): color = seal % 4 = 0, hue = 0.0 */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)s.hue_accent);
}

void test_seed_seal_white_accent(void) {
    dr_input_t in = dr_default_input();
    in.seal = 1; /* White Wind — color 1 (White) */
    dr_seed_t s = dr_compute_seed(&in);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.25f, (float)s.hue_accent);
}

void test_seed_seal_blue_accent(void) {
    dr_input_t in = dr_default_input();
    in.seal = 2; /* Blue Night — color 2 (Blue) */
    dr_seed_t s = dr_compute_seed(&in);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, (float)s.hue_accent);
}

void test_seed_seal_yellow_accent(void) {
    dr_input_t in = dr_default_input();
    in.seal = 3; /* Yellow Seed — color 3 (Yellow) */
    dr_seed_t s = dr_compute_seed(&in);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.75f, (float)s.hue_accent);
}

/* ===== dr_compute_seed — scale from HD gate ===== */

void test_seed_gate_1_scale_low(void) {
    dr_input_t in = dr_default_input();
    in.hd_sun_gate = 1;
    dr_seed_t s = dr_compute_seed(&in);
    TEST_ASSERT_TRUE(s.base_scale >= 0.5);
    TEST_ASSERT_TRUE(s.base_scale <= 1.0);
}

void test_seed_gate_64_scale_high(void) {
    dr_input_t in = dr_default_input();
    in.hd_sun_gate = 64;
    dr_seed_t s = dr_compute_seed(&in);
    TEST_ASSERT_TRUE(s.base_scale >= 0.5);
    TEST_ASSERT_TRUE(s.base_scale <= 1.0);
    /* Gate 64 should produce higher scale than gate 1 */
    dr_input_t in2 = dr_default_input();
    in2.hd_sun_gate = 1;
    dr_seed_t s2 = dr_compute_seed(&in2);
    TEST_ASSERT_TRUE(s.base_scale > s2.base_scale);
}

void test_seed_gate_unknown_default_scale(void) {
    dr_input_t in = dr_default_input();
    dr_seed_t s = dr_compute_seed(&in);
    /* Unknown gate (0): default to phi_inv (0.618) */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.618f, (float)s.base_scale);
}

/* ===== dr_compute_seed — rotation from Chinese ===== */

void test_seed_rotation_deterministic(void) {
    dr_input_t in = dr_default_input();
    in.chinese_animal = 3;
    in.chinese_element = 2;
    dr_seed_t s1 = dr_compute_seed(&in);
    dr_seed_t s2 = dr_compute_seed(&in);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)s1.rotation_seed, (float)s2.rotation_seed);
}

void test_seed_rotation_varies_by_animal(void) {
    dr_input_t in1 = dr_default_input();
    in1.chinese_animal = 0;
    in1.chinese_element = 0;
    dr_input_t in2 = dr_default_input();
    in2.chinese_animal = 6;
    in2.chinese_element = 0;
    dr_seed_t s1 = dr_compute_seed(&in1);
    dr_seed_t s2 = dr_compute_seed(&in2);
    TEST_ASSERT_TRUE(fabs(s1.rotation_seed - s2.rotation_seed) > 0.01);
}

/* ===== dr_compute_seed — vertex count from tone ===== */

void test_seed_tone_1_vertices_12(void) {
    dr_input_t in = dr_default_input();
    in.tone = 1;
    dr_seed_t s = dr_compute_seed(&in);
    TEST_ASSERT_EQUAL_INT(12, s.vertex_count);
}

void test_seed_tone_13_vertices_60(void) {
    dr_input_t in = dr_default_input();
    in.tone = 13;
    dr_seed_t s = dr_compute_seed(&in);
    TEST_ASSERT_EQUAL_INT(60, s.vertex_count);
}

void test_seed_tone_unknown_default_vertices(void) {
    dr_input_t in = dr_default_input();
    /* tone = 0 (unknown) */
    dr_seed_t s = dr_compute_seed(&in);
    TEST_ASSERT_EQUAL_INT(20, s.vertex_count);
}

/* ===== dr_compute_seed — NULL safety ===== */

void test_seed_null_input_defaults(void) {
    dr_seed_t s = dr_compute_seed(NULL);
    TEST_ASSERT_EQUAL_INT(DR_CRYSTAL_MONOCLINIC, s.system);
    TEST_ASSERT_EQUAL_INT(4, s.symmetry_order);
    TEST_ASSERT_EQUAL_INT(20, s.vertex_count);
}

/* ===== dr_compute_growth ===== */

void test_growth_no_exploration(void) {
    dr_input_t in = dr_default_input();
    dr_growth_t g = dr_compute_growth(&in);
    TEST_ASSERT_EQUAL_INT(0, g.active_facets);
    TEST_ASSERT_EQUAL_INT(DR_MAX_FACETS, g.total_facets);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)g.complexity);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)g.growth_ratio);
}

void test_growth_all_explored(void) {
    dr_input_t in = dr_default_input();
    in.systems_explored = 16;
    for (int i = 0; i < DR_MAX_FACETS; i++) {
        in.explored[i] = 1;
        in.engagement[i] = 1.0;
    }
    in.total_time_sec = 604800.0; /* 1 week */
    in.milestone_count = 11;
    dr_growth_t g = dr_compute_growth(&in);
    TEST_ASSERT_EQUAL_INT(16, g.active_facets);
    TEST_ASSERT_TRUE(g.complexity > 0.5);
    TEST_ASSERT_TRUE(g.luminosity > 0.3);
    TEST_ASSERT_TRUE(g.particle_density > 0.3);
    TEST_ASSERT_TRUE(g.growth_ratio > 0.5);
}

void test_growth_half_explored(void) {
    dr_input_t in = dr_default_input();
    in.systems_explored = 8;
    for (int i = 0; i < 8; i++) {
        in.explored[i] = 1;
        in.engagement[i] = 0.5;
    }
    dr_growth_t g = dr_compute_growth(&in);
    TEST_ASSERT_EQUAL_INT(8, g.active_facets);
    TEST_ASSERT_TRUE(g.complexity > 0.0);
    TEST_ASSERT_TRUE(g.complexity < 1.0);
    TEST_ASSERT_TRUE(g.growth_ratio > 0.0);
    TEST_ASSERT_TRUE(g.growth_ratio < 1.0);
}

void test_growth_null_input(void) {
    dr_growth_t g = dr_compute_growth(NULL);
    TEST_ASSERT_EQUAL_INT(0, g.active_facets);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)g.complexity);
}

void test_growth_luminosity_increases_with_time(void) {
    dr_input_t in1 = dr_default_input();
    in1.total_time_sec = 100.0;
    dr_input_t in2 = dr_default_input();
    in2.total_time_sec = 100000.0;
    dr_growth_t g1 = dr_compute_growth(&in1);
    dr_growth_t g2 = dr_compute_growth(&in2);
    TEST_ASSERT_TRUE(g2.luminosity > g1.luminosity);
}

void test_growth_particle_density_from_milestones(void) {
    dr_input_t in = dr_default_input();
    in.milestone_count = 5;
    dr_growth_t g = dr_compute_growth(&in);
    TEST_ASSERT_TRUE(g.particle_density > 0.0);
}

void test_growth_values_clamped_0_1(void) {
    dr_input_t in = dr_default_input();
    in.systems_explored = 16;
    in.total_time_sec = 1e9;
    in.milestone_count = 100;
    in.consecutive_days = 1000;
    in.total_visits = 10000;
    dr_growth_t g = dr_compute_growth(&in);
    TEST_ASSERT_TRUE(g.complexity >= 0.0 && g.complexity <= 1.0);
    TEST_ASSERT_TRUE(g.luminosity >= 0.0 && g.luminosity <= 1.0);
    TEST_ASSERT_TRUE(g.particle_density >= 0.0 && g.particle_density <= 1.0);
    TEST_ASSERT_TRUE(g.growth_ratio >= 0.0 && g.growth_ratio <= 1.0);
}

/* ===== dr_compute_facet ===== */

void test_facet_unexplored_system(void) {
    dr_input_t in = dr_default_input();
    dr_facet_t f = dr_compute_facet(&in, 3);
    TEST_ASSERT_EQUAL_INT(3, f.system_id);
    TEST_ASSERT_EQUAL_INT(0, f.active);
    TEST_ASSERT_TRUE(f.opacity < 0.2);
    TEST_ASSERT_TRUE(f.brightness < 0.2);
}

void test_facet_explored_system(void) {
    dr_input_t in = dr_default_input();
    in.explored[5] = 1;
    in.engagement[5] = 0.8;
    dr_facet_t f = dr_compute_facet(&in, 5);
    TEST_ASSERT_EQUAL_INT(5, f.system_id);
    TEST_ASSERT_EQUAL_INT(1, f.active);
    TEST_ASSERT_TRUE(f.opacity > 0.5);
    TEST_ASSERT_TRUE(f.brightness > 0.5);
    TEST_ASSERT_TRUE(f.saturation > 0.3);
}

void test_facet_hue_golden_angle_distributed(void) {
    dr_input_t in = dr_default_input();
    in.explored[0] = 1;
    in.explored[1] = 1;
    dr_facet_t f0 = dr_compute_facet(&in, 0);
    dr_facet_t f1 = dr_compute_facet(&in, 1);
    /* Facets should have different hues (golden angle separation) */
    TEST_ASSERT_TRUE(fabs(f0.hue - f1.hue) > 0.1);
}

void test_facet_hue_matches_system_hue(void) {
    dr_input_t in = dr_default_input();
    dr_facet_t f = dr_compute_facet(&in, 7);
    double expected_hue = dr_system_hue(7);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)expected_hue, (float)f.hue);
}

void test_facet_size_scales_with_engagement(void) {
    dr_input_t in = dr_default_input();
    in.explored[2] = 1;
    in.engagement[2] = 0.2;
    dr_facet_t f_low = dr_compute_facet(&in, 2);

    in.engagement[2] = 0.9;
    dr_facet_t f_high = dr_compute_facet(&in, 2);

    TEST_ASSERT_TRUE(f_high.size > f_low.size);
}

void test_facet_invalid_system_id(void) {
    dr_input_t in = dr_default_input();
    dr_facet_t f = dr_compute_facet(&in, -1);
    TEST_ASSERT_EQUAL_INT(-1, f.system_id);
    TEST_ASSERT_EQUAL_INT(0, f.active);
}

void test_facet_out_of_range_system_id(void) {
    dr_input_t in = dr_default_input();
    dr_facet_t f = dr_compute_facet(&in, 20);
    TEST_ASSERT_EQUAL_INT(20, f.system_id);
    TEST_ASSERT_EQUAL_INT(0, f.active);
}

void test_facet_null_input(void) {
    dr_facet_t f = dr_compute_facet(NULL, 0);
    TEST_ASSERT_EQUAL_INT(0, f.active);
}

void test_facet_full_engagement_high_values(void) {
    dr_input_t in = dr_default_input();
    in.explored[0] = 1;
    in.engagement[0] = 1.0;
    dr_facet_t f = dr_compute_facet(&in, 0);
    TEST_ASSERT_TRUE(f.saturation > 0.6);
    TEST_ASSERT_TRUE(f.brightness > 0.8);
    TEST_ASSERT_TRUE(f.size > 0.8);
    TEST_ASSERT_TRUE(f.opacity > 0.8);
}

/* ===== dr_compute_ambience ===== */

void test_ambience_midnight_dark(void) {
    dr_input_t in = dr_default_input();
    in.hour_fraction = 0.0; /* midnight */
    dr_growth_t g = dr_compute_growth(&in);
    dr_ambience_t a = dr_compute_ambience(&in, &g);
    TEST_ASSERT_TRUE(a.bg_brightness < 0.1);
}

void test_ambience_noon_brightest(void) {
    dr_input_t in = dr_default_input();
    in.hour_fraction = 0.5; /* noon */
    dr_growth_t g = dr_compute_growth(&in);
    dr_ambience_t a = dr_compute_ambience(&in, &g);
    TEST_ASSERT_TRUE(a.bg_brightness > 0.4);
}

void test_ambience_hue_from_hour(void) {
    dr_input_t in = dr_default_input();
    in.hour_fraction = 0.25; /* 6am */
    dr_growth_t g = dr_compute_growth(&in);
    dr_ambience_t a = dr_compute_ambience(&in, &g);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.25f, (float)a.bg_hue);
}

void test_ambience_rotation_speed_decreases_with_complexity(void) {
    dr_input_t in1 = dr_default_input();
    dr_growth_t g1 = dr_compute_growth(&in1);

    dr_input_t in2 = dr_default_input();
    in2.systems_explored = 16;
    in2.total_time_sec = 604800.0;
    dr_growth_t g2 = dr_compute_growth(&in2);

    dr_ambience_t a1 = dr_compute_ambience(&in1, &g1);
    dr_ambience_t a2 = dr_compute_ambience(&in2, &g2);

    TEST_ASSERT_TRUE(a2.rotation_speed < a1.rotation_speed);
}

void test_ambience_glow_from_growth(void) {
    dr_input_t in = dr_default_input();
    in.systems_explored = 16;
    dr_growth_t g = dr_compute_growth(&in);
    dr_ambience_t a = dr_compute_ambience(&in, &g);
    TEST_ASSERT_TRUE(a.ambient_glow > 0.0);
}

void test_ambience_null_input(void) {
    dr_growth_t g = dr_compute_growth(NULL);
    dr_ambience_t a = dr_compute_ambience(NULL, &g);
    TEST_ASSERT_TRUE(a.bg_brightness >= 0.0);
    TEST_ASSERT_TRUE(a.rotation_speed >= 0.0);
}

void test_ambience_null_growth(void) {
    dr_input_t in = dr_default_input();
    dr_ambience_t a = dr_compute_ambience(&in, NULL);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)a.ambient_glow);
}

/* ===== dr_compute (full room) ===== */

void test_compute_full_room_facet_count(void) {
    dr_input_t in = dr_default_input();
    dr_room_t r = dr_compute(&in);
    TEST_ASSERT_EQUAL_INT(DR_MAX_FACETS, r.facet_count);
}

void test_compute_full_room_seed_populated(void) {
    dr_input_t in = dr_default_input();
    in.sun_sign = 4;   /* Leo → Cubic */
    in.hexagram = 23;
    dr_room_t r = dr_compute(&in);
    TEST_ASSERT_EQUAL_INT(DR_CRYSTAL_CUBIC, r.seed.system);
    TEST_ASSERT_TRUE(r.seed.hue_base > 0.0);
}

void test_compute_full_room_growth_populated(void) {
    dr_input_t in = dr_default_input();
    in.systems_explored = 5;
    dr_room_t r = dr_compute(&in);
    TEST_ASSERT_EQUAL_INT(5, r.growth.active_facets);
}

void test_compute_full_room_deterministic(void) {
    dr_input_t in = dr_default_input();
    in.kin = 196;
    in.seal = 16;
    in.tone = 6;
    in.sun_sign = 8;
    in.hexagram = 48;
    in.hd_sun_gate = 33;
    in.chinese_animal = 7;
    in.chinese_element = 3;
    in.castle = 3;
    in.systems_explored = 10;
    in.hour_fraction = 0.75;
    dr_room_t r1 = dr_compute(&in);
    dr_room_t r2 = dr_compute(&in);
    TEST_ASSERT_EQUAL_INT(r1.seed.system, r2.seed.system);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)r1.seed.hue_base, (float)r2.seed.hue_base);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)r1.ambience.bg_hue, (float)r2.ambience.bg_hue);
}

void test_compute_null_returns_defaults(void) {
    dr_room_t r = dr_compute(NULL);
    TEST_ASSERT_EQUAL_INT(DR_CRYSTAL_MONOCLINIC, r.seed.system);
    TEST_ASSERT_EQUAL_INT(DR_MAX_FACETS, r.facet_count);
}

/* ===== dr_system_hue ===== */

void test_system_hue_0(void) {
    double h = dr_system_hue(0);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)h);
}

void test_system_hue_1(void) {
    double h = dr_system_hue(1);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, (float)DR_GOLDEN_ANGLE_N, (float)h);
}

void test_system_hue_all_in_range(void) {
    for (int i = 0; i < DR_MAX_FACETS; i++) {
        double h = dr_system_hue(i);
        TEST_ASSERT_TRUE(h >= 0.0);
        TEST_ASSERT_TRUE(h < 1.0);
    }
}

void test_system_hue_all_distinct(void) {
    double hues[DR_MAX_FACETS];
    for (int i = 0; i < DR_MAX_FACETS; i++) {
        hues[i] = dr_system_hue(i);
    }
    for (int i = 0; i < DR_MAX_FACETS; i++) {
        for (int j = i + 1; j < DR_MAX_FACETS; j++) {
            TEST_ASSERT_TRUE(fabs(hues[i] - hues[j]) > 0.01);
        }
    }
}

void test_system_hue_negative_returns_zero(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)dr_system_hue(-1));
}

void test_system_hue_out_of_range_returns_zero(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)dr_system_hue(20));
}

/* ===== dr_crystal_name ===== */

void test_crystal_name_cubic(void) {
    TEST_ASSERT_EQUAL_STRING("Cubic", dr_crystal_name(DR_CRYSTAL_CUBIC));
}

void test_crystal_name_hexagonal(void) {
    TEST_ASSERT_EQUAL_STRING("Hexagonal", dr_crystal_name(DR_CRYSTAL_HEXAGONAL));
}

void test_crystal_name_tetragonal(void) {
    TEST_ASSERT_EQUAL_STRING("Tetragonal", dr_crystal_name(DR_CRYSTAL_TETRAGONAL));
}

void test_crystal_name_orthorhombic(void) {
    TEST_ASSERT_EQUAL_STRING("Orthorhombic", dr_crystal_name(DR_CRYSTAL_ORTHORHOMBIC));
}

void test_crystal_name_monoclinic(void) {
    TEST_ASSERT_EQUAL_STRING("Monoclinic", dr_crystal_name(DR_CRYSTAL_MONOCLINIC));
}

void test_crystal_name_triclinic(void) {
    TEST_ASSERT_EQUAL_STRING("Triclinic", dr_crystal_name(DR_CRYSTAL_TRICLINIC));
}

void test_crystal_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", dr_crystal_name(DR_CRYSTAL_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", dr_crystal_name((dr_crystal_system_t)99));
}

/* ===== dr_crystal_system_count ===== */

void test_crystal_system_count(void) {
    TEST_ASSERT_EQUAL_INT(DR_CRYSTAL_COUNT, dr_crystal_system_count());
}

/* ===== Rich birth profile integration ===== */

void test_rich_profile_produces_unique_room(void) {
    /* Kin 196, Yellow Rhythmic Warrior */
    dr_input_t in = dr_default_input();
    in.kin = 196;
    in.seal = 16;       /* Warrior */
    in.tone = 6;        /* Rhythmic */
    in.sun_sign = 8;    /* Sagittarius → Cubic */
    in.hexagram = 48;   /* The Well */
    in.hd_sun_gate = 33;
    in.chinese_animal = 7;  /* Horse */
    in.chinese_element = 3; /* Water */
    in.castle = 3;
    in.systems_explored = 12;
    for (int i = 0; i < 12; i++) {
        in.explored[i] = 1;
        in.engagement[i] = 0.3 + i * 0.05;
    }
    in.consecutive_days = 45;
    in.total_visits = 200;
    in.total_time_sec = 86400.0; /* 1 day total */
    in.milestone_count = 7;
    in.hour_fraction = 0.75;  /* 6pm */

    dr_room_t r = dr_compute(&in);

    /* Crystal system from Sag (fire) */
    TEST_ASSERT_EQUAL_INT(DR_CRYSTAL_CUBIC, r.seed.system);
    /* Castle 3 → symmetry 5 */
    TEST_ASSERT_EQUAL_INT(5, r.seed.symmetry_order);
    /* Tone 6 → 8 + 6*4 = 32 vertices */
    TEST_ASSERT_EQUAL_INT(32, r.seed.vertex_count);
    /* 12 active facets */
    TEST_ASSERT_EQUAL_INT(12, r.growth.active_facets);
    /* All values in range */
    TEST_ASSERT_TRUE(r.seed.hue_base >= 0.0 && r.seed.hue_base < 1.0);
    TEST_ASSERT_TRUE(r.seed.base_scale >= 0.5 && r.seed.base_scale <= 1.0);
    TEST_ASSERT_TRUE(r.growth.complexity >= 0.0 && r.growth.complexity <= 1.0);
    TEST_ASSERT_TRUE(r.ambience.bg_brightness >= 0.0 && r.ambience.bg_brightness <= 1.0);
}

/* ===== Facet Brightness API (E7) ===== */

void test_facet_brightness_active(void) {
    dr_input_t in = dr_default_input();
    in.explored[3] = 1;
    in.engagement[3] = 0.8;
    dr_room_t r = dr_compute(&in);
    double b = dr_facet_brightness(&r, 3);
    TEST_ASSERT_TRUE(b > 0.5); /* Active + high engagement = bright */
}

void test_facet_brightness_inactive(void) {
    dr_input_t in = dr_default_input();
    dr_room_t r = dr_compute(&in);
    double b = dr_facet_brightness(&r, 0);
    TEST_ASSERT_TRUE(b < 0.2); /* Inactive = dim */
}

void test_facet_brightness_null_room(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, dr_facet_brightness(NULL, 0));
}

void test_facet_brightness_invalid_id(void) {
    dr_input_t in = dr_default_input();
    dr_room_t r = dr_compute(&in);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, dr_facet_brightness(&r, -1));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, dr_facet_brightness(&r, 99));
}

void test_total_luminosity_no_exploration(void) {
    dr_input_t in = dr_default_input();
    dr_room_t r = dr_compute(&in);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, dr_total_luminosity(&r));
}

void test_total_luminosity_with_time(void) {
    dr_input_t in = dr_default_input();
    in.total_time_sec = 300000.0; /* ~3.5 days */
    dr_room_t r = dr_compute(&in);
    double lum = dr_total_luminosity(&r);
    TEST_ASSERT_TRUE(lum > 0.0);
    TEST_ASSERT_TRUE(lum <= 1.0);
}

void test_total_luminosity_null(void) {
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, dr_total_luminosity(NULL));
}

void test_birth_color_basic(void) {
    dr_seed_t seed;
    memset(&seed, 0, sizeof(seed));
    seed.hue_base = 0.0;   /* Red-ish */
    seed.base_scale = 0.7;
    float rgba[4] = {0};
    dr_birth_color(&seed, rgba);
    /* Should produce non-zero color with full alpha */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, rgba[3]);
    /* At hue=0 (red), red channel should be dominant */
    TEST_ASSERT_TRUE(rgba[0] > rgba[2]); /* R > B */
}

void test_birth_color_null_seed(void) {
    float rgba[4] = {0};
    dr_birth_color(NULL, rgba);
    /* Should get default gray */
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, rgba[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 1.0f, rgba[3]);
}

void test_birth_color_null_rgba(void) {
    dr_seed_t seed;
    memset(&seed, 0, sizeof(seed));
    /* Should not crash */
    dr_birth_color(&seed, NULL);
}

void test_birth_color_varies_with_hue(void) {
    float rgba1[4], rgba2[4];
    dr_seed_t s1 = {0}, s2 = {0};
    s1.hue_base = 0.0;
    s1.base_scale = 0.5;
    s2.hue_base = 0.5;
    s2.base_scale = 0.5;
    dr_birth_color(&s1, rgba1);
    dr_birth_color(&s2, rgba2);
    /* Different hues should produce different colors */
    int different = (fabs(rgba1[0] - rgba2[0]) > 0.01) ||
                    (fabs(rgba1[1] - rgba2[1]) > 0.01) ||
                    (fabs(rgba1[2] - rgba2[2]) > 0.01);
    TEST_ASSERT_TRUE(different);
}

/* ===== Usage Bridge ===== */

void test_fill_engagement_basic(void) {
    dr_input_t in = dr_default_input();
    double scores[] = {0.8, 0.0, 0.5, 0.0, 1.0};
    dr_fill_engagement(&in, scores, 5);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.8, in.engagement[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, in.engagement[1]);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.5, in.engagement[2]);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1.0, in.engagement[4]);
}

void test_fill_engagement_sets_explored(void) {
    dr_input_t in = dr_default_input();
    double scores[] = {0.5, 0.0, 0.3};
    dr_fill_engagement(&in, scores, 3);
    TEST_ASSERT_EQUAL_INT(1, in.explored[0]);
    TEST_ASSERT_EQUAL_INT(0, in.explored[1]);
    TEST_ASSERT_EQUAL_INT(1, in.explored[2]);
    TEST_ASSERT_EQUAL_INT(2, in.systems_explored);
}

void test_fill_engagement_clamps(void) {
    dr_input_t in = dr_default_input();
    double scores[] = {-0.5, 1.5};
    dr_fill_engagement(&in, scores, 2);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, in.engagement[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1.0, in.engagement[1]);
}

void test_fill_engagement_null_input(void) {
    double scores[] = {0.5};
    dr_fill_engagement(NULL, scores, 1); /* should not crash */
}

void test_fill_engagement_null_scores(void) {
    dr_input_t in = dr_default_input();
    dr_fill_engagement(&in, NULL, 5); /* should not crash */
}

void test_fill_engagement_excess_count(void) {
    dr_input_t in = dr_default_input();
    double scores[20];
    for (int i = 0; i < 20; i++) scores[i] = 0.5;
    dr_fill_engagement(&in, scores, 20);
    /* Should cap at DR_MAX_FACETS=16, not overflow */
    TEST_ASSERT_EQUAL_INT(16, in.systems_explored);
}

void test_fill_engagement_integration(void) {
    /* Bridge scores into input, compute room, verify brightness reflects scores */
    dr_input_t in = dr_default_input();
    double scores[16] = {0};
    scores[5] = 0.9;  /* system 5 heavily explored */
    scores[10] = 0.1; /* system 10 barely touched */
    dr_fill_engagement(&in, scores, 16);

    dr_room_t r = dr_compute(&in);
    double b5 = dr_facet_brightness(&r, 5);
    double b10 = dr_facet_brightness(&r, 10);
    double b0 = dr_facet_brightness(&r, 0); /* unexplored */

    TEST_ASSERT_TRUE(b5 > b10);   /* more engaged = brighter */
    TEST_ASSERT_TRUE(b10 > b0);   /* explored > unexplored */
}

/* ===== main ===== */

int main(void) {
    UNITY_BEGIN();

    /* dr_default_input */
    RUN_TEST(test_default_input_kin_zero);
    RUN_TEST(test_default_input_seal_negative);
    RUN_TEST(test_default_input_tone_zero);
    RUN_TEST(test_default_input_sun_sign_negative);
    RUN_TEST(test_default_input_hexagram_zero);
    RUN_TEST(test_default_input_systems_explored_zero);
    RUN_TEST(test_default_input_hour_fraction_zero);
    RUN_TEST(test_default_input_explored_all_zero);
    RUN_TEST(test_default_input_engagement_all_zero);

    /* dr_compute_seed — crystal system */
    RUN_TEST(test_seed_fire_sign_aries_cubic);
    RUN_TEST(test_seed_fire_sign_leo_cubic);
    RUN_TEST(test_seed_fire_sign_sagittarius_cubic);
    RUN_TEST(test_seed_earth_sign_taurus_hexagonal);
    RUN_TEST(test_seed_earth_sign_virgo_hexagonal);
    RUN_TEST(test_seed_air_sign_gemini_tetragonal);
    RUN_TEST(test_seed_water_sign_cancer_orthorhombic);
    RUN_TEST(test_seed_unknown_zodiac_monoclinic);

    /* dr_compute_seed — symmetry */
    RUN_TEST(test_seed_castle_0_symmetry_2);
    RUN_TEST(test_seed_castle_4_symmetry_6);
    RUN_TEST(test_seed_castle_unknown_symmetry_4);

    /* dr_compute_seed — hue */
    RUN_TEST(test_seed_hexagram_1_hue_golden_angle);
    RUN_TEST(test_seed_hexagram_32_hue_wraps);
    RUN_TEST(test_seed_hexagram_unknown_hue_zero);

    /* dr_compute_seed — accent */
    RUN_TEST(test_seed_seal_red_accent);
    RUN_TEST(test_seed_seal_white_accent);
    RUN_TEST(test_seed_seal_blue_accent);
    RUN_TEST(test_seed_seal_yellow_accent);

    /* dr_compute_seed — scale */
    RUN_TEST(test_seed_gate_1_scale_low);
    RUN_TEST(test_seed_gate_64_scale_high);
    RUN_TEST(test_seed_gate_unknown_default_scale);

    /* dr_compute_seed — rotation */
    RUN_TEST(test_seed_rotation_deterministic);
    RUN_TEST(test_seed_rotation_varies_by_animal);

    /* dr_compute_seed — vertices */
    RUN_TEST(test_seed_tone_1_vertices_12);
    RUN_TEST(test_seed_tone_13_vertices_60);
    RUN_TEST(test_seed_tone_unknown_default_vertices);

    /* dr_compute_seed — null */
    RUN_TEST(test_seed_null_input_defaults);

    /* dr_compute_growth */
    RUN_TEST(test_growth_no_exploration);
    RUN_TEST(test_growth_all_explored);
    RUN_TEST(test_growth_half_explored);
    RUN_TEST(test_growth_null_input);
    RUN_TEST(test_growth_luminosity_increases_with_time);
    RUN_TEST(test_growth_particle_density_from_milestones);
    RUN_TEST(test_growth_values_clamped_0_1);

    /* dr_compute_facet */
    RUN_TEST(test_facet_unexplored_system);
    RUN_TEST(test_facet_explored_system);
    RUN_TEST(test_facet_hue_golden_angle_distributed);
    RUN_TEST(test_facet_hue_matches_system_hue);
    RUN_TEST(test_facet_size_scales_with_engagement);
    RUN_TEST(test_facet_invalid_system_id);
    RUN_TEST(test_facet_out_of_range_system_id);
    RUN_TEST(test_facet_null_input);
    RUN_TEST(test_facet_full_engagement_high_values);

    /* dr_compute_ambience */
    RUN_TEST(test_ambience_midnight_dark);
    RUN_TEST(test_ambience_noon_brightest);
    RUN_TEST(test_ambience_hue_from_hour);
    RUN_TEST(test_ambience_rotation_speed_decreases_with_complexity);
    RUN_TEST(test_ambience_glow_from_growth);
    RUN_TEST(test_ambience_null_input);
    RUN_TEST(test_ambience_null_growth);

    /* dr_compute (full room) */
    RUN_TEST(test_compute_full_room_facet_count);
    RUN_TEST(test_compute_full_room_seed_populated);
    RUN_TEST(test_compute_full_room_growth_populated);
    RUN_TEST(test_compute_full_room_deterministic);
    RUN_TEST(test_compute_null_returns_defaults);

    /* dr_system_hue */
    RUN_TEST(test_system_hue_0);
    RUN_TEST(test_system_hue_1);
    RUN_TEST(test_system_hue_all_in_range);
    RUN_TEST(test_system_hue_all_distinct);
    RUN_TEST(test_system_hue_negative_returns_zero);
    RUN_TEST(test_system_hue_out_of_range_returns_zero);

    /* dr_crystal_name */
    RUN_TEST(test_crystal_name_cubic);
    RUN_TEST(test_crystal_name_hexagonal);
    RUN_TEST(test_crystal_name_tetragonal);
    RUN_TEST(test_crystal_name_orthorhombic);
    RUN_TEST(test_crystal_name_monoclinic);
    RUN_TEST(test_crystal_name_triclinic);
    RUN_TEST(test_crystal_name_invalid);

    /* dr_crystal_system_count */
    RUN_TEST(test_crystal_system_count);

    /* Integration */
    RUN_TEST(test_rich_profile_produces_unique_room);

    /* Facet Brightness API — E7 (7) */
    RUN_TEST(test_facet_brightness_active);
    RUN_TEST(test_facet_brightness_inactive);
    RUN_TEST(test_facet_brightness_null_room);
    RUN_TEST(test_facet_brightness_invalid_id);
    RUN_TEST(test_total_luminosity_no_exploration);
    RUN_TEST(test_total_luminosity_with_time);
    RUN_TEST(test_total_luminosity_null);

    /* Birth Color (4) */
    RUN_TEST(test_birth_color_basic);
    RUN_TEST(test_birth_color_null_seed);
    RUN_TEST(test_birth_color_null_rgba);
    RUN_TEST(test_birth_color_varies_with_hue);

    /* Usage Bridge (6) */
    RUN_TEST(test_fill_engagement_basic);
    RUN_TEST(test_fill_engagement_sets_explored);
    RUN_TEST(test_fill_engagement_clamps);
    RUN_TEST(test_fill_engagement_null_input);
    RUN_TEST(test_fill_engagement_null_scores);
    RUN_TEST(test_fill_engagement_excess_count);

    /* Integration: bridge → room (1) */
    RUN_TEST(test_fill_engagement_integration);

    return UNITY_END();
}
