/* test_thermo.c — Tests for thermochronology data module.
 * TDD RED phase: all tests written before implementation.
 * 8 thermal events, 6 mineral systems, cooling curves,
 * Earth thermal state at any geological age. */

#include "../../unity/unity.h"
#include "../../../src/systems/geology/thermo.h"

#include <math.h>
#include <string.h>

/* Tolerances */
#define TEMP_EPS  1.0f       /* 1 K tolerance for temperatures */
#define AGE_EPS   1.0f       /* 1 Ma tolerance for ages */
#define FRAC_EPS  1e-3f      /* general fractional tolerance */

void setUp(void) { }
void tearDown(void) { }

/* ==== Event count ==== */

void test_event_count_returns_8(void) {
    TEST_ASSERT_EQUAL_INT(8, thermo_event_count());
}

void test_event_count_matches_define(void) {
    TEST_ASSERT_EQUAL_INT(THERMO_EVENT_COUNT, thermo_event_count());
}

/* ==== Event data: thermo_event_get ==== */

void test_event_get_core_formation(void) {
    thermal_event_t e = thermo_event_get(0);
    TEST_ASSERT_EQUAL_INT(0, e.id);
    TEST_ASSERT_EQUAL_STRING("Core Formation", e.name);
    TEST_ASSERT_FLOAT_WITHIN(AGE_EPS, 4500.0f, (float)e.age_ma);
    TEST_ASSERT_FLOAT_WITHIN(AGE_EPS, 100.0f, (float)e.duration_ma);
    TEST_ASSERT_FLOAT_WITHIN(TEMP_EPS, 2000.0f, (float)e.temp_effect_k);
    TEST_ASSERT_NOT_NULL(e.description);
}

void test_event_get_late_heavy_bombardment(void) {
    thermal_event_t e = thermo_event_get(1);
    TEST_ASSERT_EQUAL_INT(1, e.id);
    TEST_ASSERT_EQUAL_STRING("Late Heavy Bombardment", e.name);
    TEST_ASSERT_FLOAT_WITHIN(AGE_EPS, 4100.0f, (float)e.age_ma);
    TEST_ASSERT_FLOAT_WITHIN(AGE_EPS, 300.0f, (float)e.duration_ma);
    TEST_ASSERT_FLOAT_WITHIN(TEMP_EPS, 500.0f, (float)e.temp_effect_k);
}

void test_event_get_mantle_convection_onset(void) {
    thermal_event_t e = thermo_event_get(2);
    TEST_ASSERT_EQUAL_INT(2, e.id);
    TEST_ASSERT_EQUAL_STRING("Mantle Convection Onset", e.name);
    TEST_ASSERT_FLOAT_WITHIN(AGE_EPS, 4400.0f, (float)e.age_ma);
    TEST_ASSERT_FLOAT_WITHIN(TEMP_EPS, -200.0f, (float)e.temp_effect_k);
}

void test_event_get_archean_mantle_plumes(void) {
    thermal_event_t e = thermo_event_get(3);
    TEST_ASSERT_EQUAL_INT(3, e.id);
    TEST_ASSERT_EQUAL_STRING("Archean Mantle Plumes", e.name);
    TEST_ASSERT_FLOAT_WITHIN(AGE_EPS, 3500.0f, (float)e.age_ma);
    TEST_ASSERT_FLOAT_WITHIN(TEMP_EPS, 100.0f, (float)e.temp_effect_k);
}

void test_event_get_great_oxidation(void) {
    thermal_event_t e = thermo_event_get(4);
    TEST_ASSERT_EQUAL_INT(4, e.id);
    TEST_ASSERT_EQUAL_STRING("Great Oxidation Event", e.name);
    TEST_ASSERT_FLOAT_WITHIN(AGE_EPS, 2400.0f, (float)e.age_ma);
    TEST_ASSERT_FLOAT_WITHIN(TEMP_EPS, -50.0f, (float)e.temp_effect_k);
}

void test_event_get_snowball_earth(void) {
    thermal_event_t e = thermo_event_get(5);
    TEST_ASSERT_EQUAL_INT(5, e.id);
    TEST_ASSERT_EQUAL_STRING("Snowball Earth (Sturtian)", e.name);
    TEST_ASSERT_FLOAT_WITHIN(AGE_EPS, 717.0f, (float)e.age_ma);
    TEST_ASSERT_FLOAT_WITHIN(AGE_EPS, 57.0f, (float)e.duration_ma);
    TEST_ASSERT_FLOAT_WITHIN(TEMP_EPS, -100.0f, (float)e.temp_effect_k);
}

void test_event_get_pangaea_assembly(void) {
    thermal_event_t e = thermo_event_get(6);
    TEST_ASSERT_EQUAL_INT(6, e.id);
    TEST_ASSERT_EQUAL_STRING("Pangaea Assembly", e.name);
    TEST_ASSERT_FLOAT_WITHIN(AGE_EPS, 335.0f, (float)e.age_ma);
    TEST_ASSERT_FLOAT_WITHIN(TEMP_EPS, 30.0f, (float)e.temp_effect_k);
}

void test_event_get_present_equilibrium(void) {
    thermal_event_t e = thermo_event_get(7);
    TEST_ASSERT_EQUAL_INT(7, e.id);
    TEST_ASSERT_EQUAL_STRING("Present Equilibrium", e.name);
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 0.0f, (float)e.age_ma);
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 0.0f, (float)e.duration_ma);
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 0.0f, (float)e.temp_effect_k);
}

void test_event_get_invalid_returns_negative_id(void) {
    thermal_event_t e = thermo_event_get(99);
    TEST_ASSERT_EQUAL_INT(-1, e.id);
}

void test_event_get_negative_index_returns_negative_id(void) {
    thermal_event_t e = thermo_event_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, e.id);
}

/* ==== Event nearest ==== */

void test_event_nearest_at_exact_age(void) {
    /* At exactly 4500 Ma -> Core Formation (index 0) */
    TEST_ASSERT_EQUAL_INT(0, thermo_event_nearest(4500.0));
}

void test_event_nearest_at_present(void) {
    /* At 0 Ma -> Present Equilibrium (index 7) */
    TEST_ASSERT_EQUAL_INT(7, thermo_event_nearest(0.0));
}

void test_event_nearest_at_snowball(void) {
    /* At 717 Ma -> Snowball Earth (index 5) */
    TEST_ASSERT_EQUAL_INT(5, thermo_event_nearest(717.0));
}

void test_event_nearest_between_events(void) {
    /* At 2000 Ma: nearest to Great Oxidation (2400) = 400 Ma
     * vs Archean Plumes (3500) = 1500 Ma
     * vs Snowball (717) = 1283 Ma => Great Oxidation wins */
    TEST_ASSERT_EQUAL_INT(4, thermo_event_nearest(2000.0));
}

/* ==== Earth thermal state ==== */

void test_earth_present(void) {
    earth_thermal_t e = thermo_earth_at(0.0);
    TEST_ASSERT_FLOAT_WITHIN(50.0f, 5200.0f, (float)e.core_temp_k);
    TEST_ASSERT_FLOAT_WITHIN(50.0f, 1600.0f, (float)e.mantle_temp_k);
    TEST_ASSERT_FLOAT_WITHIN(10.0f, 288.0f, (float)e.surface_temp_k);
    TEST_ASSERT_FLOAT_WITHIN(5.0f, 87.0f, (float)e.heat_flow_mw);
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 0.0f, (float)e.age_ma);
}

void test_earth_at_formation(void) {
    earth_thermal_t e = thermo_earth_at(4540.0);
    /* At formation: core ~6500K, mantle ~2200K, surface ~1500K */
    TEST_ASSERT_FLOAT_WITHIN(100.0f, 6500.0f, (float)e.core_temp_k);
    TEST_ASSERT_FLOAT_WITHIN(100.0f, 2200.0f, (float)e.mantle_temp_k);
    TEST_ASSERT_FLOAT_WITHIN(200.0f, 1500.0f, (float)e.surface_temp_k);
    TEST_ASSERT_FLOAT_WITHIN(AGE_EPS, 4540.0f, (float)e.age_ma);
}

void test_earth_at_mid_age(void) {
    earth_thermal_t e = thermo_earth_at(2270.0); /* halfway */
    /* Core should be between 5200 and 6500 */
    TEST_ASSERT_TRUE(e.core_temp_k > 5200.0);
    TEST_ASSERT_TRUE(e.core_temp_k < 6500.0);
    /* Mantle between 1600 and 2200 */
    TEST_ASSERT_TRUE(e.mantle_temp_k > 1600.0);
    TEST_ASSERT_TRUE(e.mantle_temp_k < 2200.0);
}

void test_earth_core_hotter_in_past(void) {
    earth_thermal_t recent = thermo_earth_at(100.0);
    earth_thermal_t ancient = thermo_earth_at(3000.0);
    TEST_ASSERT_TRUE(ancient.core_temp_k > recent.core_temp_k);
}

void test_earth_mantle_hotter_in_past(void) {
    earth_thermal_t recent = thermo_earth_at(100.0);
    earth_thermal_t ancient = thermo_earth_at(3000.0);
    TEST_ASSERT_TRUE(ancient.mantle_temp_k > recent.mantle_temp_k);
}

void test_earth_heat_flow_higher_in_past(void) {
    earth_thermal_t recent = thermo_earth_at(100.0);
    earth_thermal_t ancient = thermo_earth_at(3000.0);
    TEST_ASSERT_TRUE(ancient.heat_flow_mw > recent.heat_flow_mw);
}

void test_earth_present_function(void) {
    earth_thermal_t a = thermo_earth_at(0.0);
    earth_thermal_t b = thermo_earth_present();
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, (float)a.core_temp_k, (float)b.core_temp_k);
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, (float)a.mantle_temp_k, (float)b.mantle_temp_k);
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, (float)a.surface_temp_k, (float)b.surface_temp_k);
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, (float)a.heat_flow_mw, (float)b.heat_flow_mw);
}

/* ==== Mineral count ==== */

void test_mineral_count_returns_6(void) {
    TEST_ASSERT_EQUAL_INT(6, thermo_mineral_count());
}

void test_mineral_count_matches_define(void) {
    TEST_ASSERT_EQUAL_INT(THERMO_MINERAL_COUNT, thermo_mineral_count());
}

/* ==== Mineral data: thermo_mineral_get ==== */

void test_mineral_get_zircon_upb(void) {
    mineral_system_t m = thermo_mineral_get(0);
    TEST_ASSERT_EQUAL_INT(0, m.id);
    TEST_ASSERT_EQUAL_STRING("Zircon", m.mineral);
    TEST_ASSERT_EQUAL_STRING("U-Pb", m.system);
    TEST_ASSERT_FLOAT_WITHIN(TEMP_EPS, 900.0f, (float)m.closure_temp_c);
    TEST_ASSERT_FLOAT_WITHIN(TEMP_EPS, 1173.15f, (float)m.closure_temp_k);
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 1.0f, (float)m.useful_range_min_ma);
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 4500.0f, (float)m.useful_range_max_ma);
}

void test_mineral_get_zircon_ft(void) {
    mineral_system_t m = thermo_mineral_get(1);
    TEST_ASSERT_EQUAL_INT(1, m.id);
    TEST_ASSERT_EQUAL_STRING("Zircon", m.mineral);
    TEST_ASSERT_EQUAL_STRING("Fission Track", m.system);
    TEST_ASSERT_FLOAT_WITHIN(TEMP_EPS, 240.0f, (float)m.closure_temp_c);
    TEST_ASSERT_FLOAT_WITHIN(TEMP_EPS, 513.15f, (float)m.closure_temp_k);
}

void test_mineral_get_apatite_ft(void) {
    mineral_system_t m = thermo_mineral_get(2);
    TEST_ASSERT_EQUAL_STRING("Apatite", m.mineral);
    TEST_ASSERT_EQUAL_STRING("Fission Track", m.system);
    TEST_ASSERT_FLOAT_WITHIN(TEMP_EPS, 110.0f, (float)m.closure_temp_c);
    TEST_ASSERT_FLOAT_WITHIN(TEMP_EPS, 383.15f, (float)m.closure_temp_k);
}

void test_mineral_get_apatite_uthe(void) {
    mineral_system_t m = thermo_mineral_get(3);
    TEST_ASSERT_EQUAL_STRING("Apatite", m.mineral);
    TEST_ASSERT_EQUAL_STRING("(U-Th)/He", m.system);
    TEST_ASSERT_FLOAT_WITHIN(TEMP_EPS, 70.0f, (float)m.closure_temp_c);
    TEST_ASSERT_FLOAT_WITHIN(TEMP_EPS, 343.15f, (float)m.closure_temp_k);
}

void test_mineral_get_muscovite(void) {
    mineral_system_t m = thermo_mineral_get(4);
    TEST_ASSERT_EQUAL_STRING("Muscovite", m.mineral);
    TEST_ASSERT_EQUAL_STRING("K-Ar", m.system);
    TEST_ASSERT_FLOAT_WITHIN(TEMP_EPS, 350.0f, (float)m.closure_temp_c);
    TEST_ASSERT_FLOAT_WITHIN(TEMP_EPS, 623.15f, (float)m.closure_temp_k);
}

void test_mineral_get_hornblende(void) {
    mineral_system_t m = thermo_mineral_get(5);
    TEST_ASSERT_EQUAL_STRING("Hornblende", m.mineral);
    TEST_ASSERT_EQUAL_STRING("K-Ar", m.system);
    TEST_ASSERT_FLOAT_WITHIN(TEMP_EPS, 500.0f, (float)m.closure_temp_c);
    TEST_ASSERT_FLOAT_WITHIN(TEMP_EPS, 773.15f, (float)m.closure_temp_k);
}

void test_mineral_get_invalid_returns_negative_id(void) {
    mineral_system_t m = thermo_mineral_get(99);
    TEST_ASSERT_EQUAL_INT(-1, m.id);
}

void test_mineral_get_negative_index(void) {
    mineral_system_t m = thermo_mineral_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, m.id);
}

/* ==== Mineral closure temp consistency ==== */

void test_mineral_closure_k_equals_c_plus_273(void) {
    int i;
    for (i = 0; i < thermo_mineral_count(); i++) {
        mineral_system_t m = thermo_mineral_get(i);
        float expected_k = (float)(m.closure_temp_c + 273.15);
        TEST_ASSERT_FLOAT_WITHIN(0.01f, expected_k, (float)m.closure_temp_k);
    }
}

/* ==== Mineral find ==== */

void test_mineral_find_zircon(void) {
    mineral_system_t m = thermo_mineral_find("Zircon");
    TEST_ASSERT_EQUAL_STRING("Zircon", m.mineral);
    /* Should return first match (id=0, U-Pb) */
    TEST_ASSERT_EQUAL_INT(0, m.id);
}

void test_mineral_find_apatite(void) {
    mineral_system_t m = thermo_mineral_find("Apatite");
    TEST_ASSERT_EQUAL_STRING("Apatite", m.mineral);
    /* First apatite is id=2, Fission Track */
    TEST_ASSERT_EQUAL_INT(2, m.id);
}

void test_mineral_find_hornblende(void) {
    mineral_system_t m = thermo_mineral_find("Hornblende");
    TEST_ASSERT_EQUAL_INT(5, m.id);
}

void test_mineral_find_not_found(void) {
    mineral_system_t m = thermo_mineral_find("Unobtanium");
    TEST_ASSERT_EQUAL_INT(-1, m.id);
}

void test_mineral_find_null(void) {
    mineral_system_t m = thermo_mineral_find(NULL);
    TEST_ASSERT_EQUAL_INT(-1, m.id);
}

/* ==== Mineral in range ==== */

void test_mineral_in_range_zircon_upb_valid(void) {
    TEST_ASSERT_TRUE(thermo_mineral_in_range(0, 1000.0));
}

void test_mineral_in_range_zircon_upb_too_young(void) {
    TEST_ASSERT_FALSE(thermo_mineral_in_range(0, 0.5));
}

void test_mineral_in_range_apatite_uthe_valid(void) {
    TEST_ASSERT_TRUE(thermo_mineral_in_range(3, 50.0));
}

void test_mineral_in_range_apatite_uthe_too_old(void) {
    TEST_ASSERT_FALSE(thermo_mineral_in_range(3, 300.0));
}

void test_mineral_in_range_invalid_index(void) {
    TEST_ASSERT_FALSE(thermo_mineral_in_range(99, 100.0));
}

void test_mineral_in_range_negative_index(void) {
    TEST_ASSERT_FALSE(thermo_mineral_in_range(-1, 100.0));
}

void test_mineral_in_range_at_boundary_min(void) {
    /* Zircon U-Pb: min = 1.0 */
    TEST_ASSERT_TRUE(thermo_mineral_in_range(0, 1.0));
}

void test_mineral_in_range_at_boundary_max(void) {
    /* Zircon U-Pb: max = 4500.0 */
    TEST_ASSERT_TRUE(thermo_mineral_in_range(0, 4500.0));
}

/* ==== Best mineral ==== */

void test_best_mineral_for_ancient_rock(void) {
    /* 4000 Ma: Zircon U-Pb (id=0, closure 900C) should win — highest closure temp */
    int best = thermo_best_mineral(4000.0);
    TEST_ASSERT_EQUAL_INT(0, best);
}

void test_best_mineral_for_young_sample(void) {
    /* 0.05 Ma (50 Ka): only Apatite (U-Th)/He covers (0.01-200) */
    int best = thermo_best_mineral(0.05);
    TEST_ASSERT_EQUAL_INT(3, best);
}

void test_best_mineral_for_medium_age(void) {
    /* 100 Ma: multiple systems cover it; prefer highest closure temp in range */
    int best = thermo_best_mineral(100.0);
    TEST_ASSERT_TRUE(best >= 0 && best < THERMO_MINERAL_COUNT);
    TEST_ASSERT_TRUE(thermo_mineral_in_range(best, 100.0));
}

void test_best_mineral_returns_negative_for_out_of_range(void) {
    /* 0.001 Ma: no system covers this */
    TEST_ASSERT_EQUAL_INT(-1, thermo_best_mineral(0.001));
}

/* ==== Core cooling curve ==== */

void test_core_cooling_curve_returns_count(void) {
    double temps[10];
    double ages[10];
    int n = thermo_core_cooling_curve(4540.0, temps, ages, 10);
    TEST_ASSERT_EQUAL_INT(10, n);
}

void test_core_cooling_curve_first_point_is_present(void) {
    double temps[5];
    double ages[5];
    thermo_core_cooling_curve(4540.0, temps, ages, 5);
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 0.0f, (float)ages[0]);
    TEST_ASSERT_FLOAT_WITHIN(100.0f, 5200.0f, (float)temps[0]);
}

void test_core_cooling_curve_last_point_is_max_age(void) {
    double temps[5];
    double ages[5];
    thermo_core_cooling_curve(4540.0, temps, ages, 5);
    TEST_ASSERT_FLOAT_WITHIN(AGE_EPS, 4540.0f, (float)ages[4]);
    TEST_ASSERT_FLOAT_WITHIN(200.0f, 6500.0f, (float)temps[4]);
}

void test_core_cooling_curve_monotonically_increasing_temp(void) {
    double temps[20];
    double ages[20];
    int i;
    thermo_core_cooling_curve(4540.0, temps, ages, 20);
    /* Older -> hotter core, so temps increase as ages increase */
    for (i = 1; i < 20; i++) {
        TEST_ASSERT_TRUE(temps[i] >= temps[i - 1]);
    }
}

void test_core_cooling_curve_null_temps_returns_zero(void) {
    double ages[5];
    int n = thermo_core_cooling_curve(4540.0, NULL, ages, 5);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_core_cooling_curve_null_ages_returns_zero(void) {
    double temps[5];
    int n = thermo_core_cooling_curve(4540.0, temps, NULL, 5);
    TEST_ASSERT_EQUAL_INT(0, n);
}

void test_core_cooling_curve_zero_points_returns_zero(void) {
    double temps[1];
    double ages[1];
    int n = thermo_core_cooling_curve(4540.0, temps, ages, 0);
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* ==== Mantle cooling curve ==== */

void test_mantle_cooling_curve_returns_count(void) {
    double temps[10];
    double ages[10];
    int n = thermo_mantle_cooling_curve(4540.0, temps, ages, 10);
    TEST_ASSERT_EQUAL_INT(10, n);
}

void test_mantle_cooling_curve_first_point_is_present(void) {
    double temps[5];
    double ages[5];
    thermo_mantle_cooling_curve(4540.0, temps, ages, 5);
    TEST_ASSERT_FLOAT_WITHIN(FRAC_EPS, 0.0f, (float)ages[0]);
    TEST_ASSERT_FLOAT_WITHIN(100.0f, 1600.0f, (float)temps[0]);
}

void test_mantle_cooling_curve_last_point_is_max_age(void) {
    double temps[5];
    double ages[5];
    thermo_mantle_cooling_curve(4540.0, temps, ages, 5);
    TEST_ASSERT_FLOAT_WITHIN(AGE_EPS, 4540.0f, (float)ages[4]);
    TEST_ASSERT_FLOAT_WITHIN(200.0f, 2200.0f, (float)temps[4]);
}

void test_mantle_cooling_curve_monotonically_increasing_temp(void) {
    double temps[20];
    double ages[20];
    int i;
    thermo_mantle_cooling_curve(4540.0, temps, ages, 20);
    for (i = 1; i < 20; i++) {
        TEST_ASSERT_TRUE(temps[i] >= temps[i - 1]);
    }
}

/* ==== All events validation ==== */

void test_all_events_have_names(void) {
    int i;
    for (i = 0; i < thermo_event_count(); i++) {
        thermal_event_t e = thermo_event_get(i);
        TEST_ASSERT_NOT_NULL(e.name);
        TEST_ASSERT_NOT_NULL(e.description);
    }
}

void test_all_events_id_matches_index(void) {
    int i;
    for (i = 0; i < thermo_event_count(); i++) {
        thermal_event_t e = thermo_event_get(i);
        TEST_ASSERT_EQUAL_INT(i, e.id);
    }
}

void test_all_minerals_have_names(void) {
    int i;
    for (i = 0; i < thermo_mineral_count(); i++) {
        mineral_system_t m = thermo_mineral_get(i);
        TEST_ASSERT_NOT_NULL(m.mineral);
        TEST_ASSERT_NOT_NULL(m.system);
    }
}

void test_all_minerals_id_matches_index(void) {
    int i;
    for (i = 0; i < thermo_mineral_count(); i++) {
        mineral_system_t m = thermo_mineral_get(i);
        TEST_ASSERT_EQUAL_INT(i, m.id);
    }
}

void test_all_minerals_range_min_lt_max(void) {
    int i;
    for (i = 0; i < thermo_mineral_count(); i++) {
        mineral_system_t m = thermo_mineral_get(i);
        TEST_ASSERT_TRUE(m.useful_range_min_ma < m.useful_range_max_ma);
    }
}

/* ==== Runner ==== */

int main(void) {
    UNITY_BEGIN();

    /* Event count (2 tests) */
    RUN_TEST(test_event_count_returns_8);
    RUN_TEST(test_event_count_matches_define);

    /* Event data (10 tests) */
    RUN_TEST(test_event_get_core_formation);
    RUN_TEST(test_event_get_late_heavy_bombardment);
    RUN_TEST(test_event_get_mantle_convection_onset);
    RUN_TEST(test_event_get_archean_mantle_plumes);
    RUN_TEST(test_event_get_great_oxidation);
    RUN_TEST(test_event_get_snowball_earth);
    RUN_TEST(test_event_get_pangaea_assembly);
    RUN_TEST(test_event_get_present_equilibrium);
    RUN_TEST(test_event_get_invalid_returns_negative_id);
    RUN_TEST(test_event_get_negative_index_returns_negative_id);

    /* Event nearest (4 tests) */
    RUN_TEST(test_event_nearest_at_exact_age);
    RUN_TEST(test_event_nearest_at_present);
    RUN_TEST(test_event_nearest_at_snowball);
    RUN_TEST(test_event_nearest_between_events);

    /* Earth thermal state (7 tests) */
    RUN_TEST(test_earth_present);
    RUN_TEST(test_earth_at_formation);
    RUN_TEST(test_earth_at_mid_age);
    RUN_TEST(test_earth_core_hotter_in_past);
    RUN_TEST(test_earth_mantle_hotter_in_past);
    RUN_TEST(test_earth_heat_flow_higher_in_past);
    RUN_TEST(test_earth_present_function);

    /* Mineral count (2 tests) */
    RUN_TEST(test_mineral_count_returns_6);
    RUN_TEST(test_mineral_count_matches_define);

    /* Mineral data (8 tests) */
    RUN_TEST(test_mineral_get_zircon_upb);
    RUN_TEST(test_mineral_get_zircon_ft);
    RUN_TEST(test_mineral_get_apatite_ft);
    RUN_TEST(test_mineral_get_apatite_uthe);
    RUN_TEST(test_mineral_get_muscovite);
    RUN_TEST(test_mineral_get_hornblende);
    RUN_TEST(test_mineral_get_invalid_returns_negative_id);
    RUN_TEST(test_mineral_get_negative_index);

    /* Mineral closure temp consistency (1 test) */
    RUN_TEST(test_mineral_closure_k_equals_c_plus_273);

    /* Mineral find (5 tests) */
    RUN_TEST(test_mineral_find_zircon);
    RUN_TEST(test_mineral_find_apatite);
    RUN_TEST(test_mineral_find_hornblende);
    RUN_TEST(test_mineral_find_not_found);
    RUN_TEST(test_mineral_find_null);

    /* Mineral in range (8 tests) */
    RUN_TEST(test_mineral_in_range_zircon_upb_valid);
    RUN_TEST(test_mineral_in_range_zircon_upb_too_young);
    RUN_TEST(test_mineral_in_range_apatite_uthe_valid);
    RUN_TEST(test_mineral_in_range_apatite_uthe_too_old);
    RUN_TEST(test_mineral_in_range_invalid_index);
    RUN_TEST(test_mineral_in_range_negative_index);
    RUN_TEST(test_mineral_in_range_at_boundary_min);
    RUN_TEST(test_mineral_in_range_at_boundary_max);

    /* Best mineral (4 tests) */
    RUN_TEST(test_best_mineral_for_ancient_rock);
    RUN_TEST(test_best_mineral_for_young_sample);
    RUN_TEST(test_best_mineral_for_medium_age);
    RUN_TEST(test_best_mineral_returns_negative_for_out_of_range);

    /* Core cooling curve (7 tests) */
    RUN_TEST(test_core_cooling_curve_returns_count);
    RUN_TEST(test_core_cooling_curve_first_point_is_present);
    RUN_TEST(test_core_cooling_curve_last_point_is_max_age);
    RUN_TEST(test_core_cooling_curve_monotonically_increasing_temp);
    RUN_TEST(test_core_cooling_curve_null_temps_returns_zero);
    RUN_TEST(test_core_cooling_curve_null_ages_returns_zero);
    RUN_TEST(test_core_cooling_curve_zero_points_returns_zero);

    /* Mantle cooling curve (4 tests) */
    RUN_TEST(test_mantle_cooling_curve_returns_count);
    RUN_TEST(test_mantle_cooling_curve_first_point_is_present);
    RUN_TEST(test_mantle_cooling_curve_last_point_is_max_age);
    RUN_TEST(test_mantle_cooling_curve_monotonically_increasing_temp);

    /* All data validation (5 tests) */
    RUN_TEST(test_all_events_have_names);
    RUN_TEST(test_all_events_id_matches_index);
    RUN_TEST(test_all_minerals_have_names);
    RUN_TEST(test_all_minerals_id_matches_index);
    RUN_TEST(test_all_minerals_range_min_lt_max);

    return UNITY_END();
}
