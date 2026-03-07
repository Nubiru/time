/* test_earth_drama.c -- Earth History Dramatization tests
 * TDD RED phase: tests written before implementation.
 * Tests atmosphere evolution, biosphere state, dramatic snapshots,
 * human arrival scale, drama text, and atmosphere transitions.
 *
 * Atmosphere timeline (5 states):
 *   Reducing (4540-2400 Ma), Transitional (2400-2000 Ma),
 *   Oxidizing (2000-600 Ma), Ozone (600-400 Ma), Modern (400-0 Ma)
 *
 * Biosphere milestones:
 *   >3800 Ma: nothing, 3800-2700: prokaryotes, 2700-2100: +cyanobacteria,
 *   2100-1000: +eukaryotes, 1000-541: +multicellular, 541-470: +animals,
 *   470-375: +land plants, 375-225: +tetrapods, 225-66: +mammals/dinos,
 *   66-0.3: +primates, 0.3-0: +Homo sapiens */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/earth_drama.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== ed_atmosphere_at — atmosphere state lookup ===== */

void test_atmosphere_at_hadean_is_reducing(void)
{
    ed_atmosphere_info_t a = ed_atmosphere_at(4000.0);
    TEST_ASSERT_EQUAL_INT(ED_ATMO_REDUCING, a.state);
}

void test_atmosphere_at_reducing_has_no_o2(void)
{
    ed_atmosphere_info_t a = ed_atmosphere_at(3000.0);
    TEST_ASSERT_FLOAT_WITHIN((float)0.5, (float)0.0, (float)a.o2_percent);
}

void test_atmosphere_at_reducing_not_uv_shielded(void)
{
    ed_atmosphere_info_t a = ed_atmosphere_at(3000.0);
    TEST_ASSERT_EQUAL_INT(0, a.uv_shielded);
}

void test_atmosphere_at_transitional_goe(void)
{
    ed_atmosphere_info_t a = ed_atmosphere_at(2200.0);
    TEST_ASSERT_EQUAL_INT(ED_ATMO_TRANSITIONAL, a.state);
}

void test_atmosphere_at_transitional_trace_o2(void)
{
    ed_atmosphere_info_t a = ed_atmosphere_at(2200.0);
    /* GOE transitional: ~1-2% O2 */
    TEST_ASSERT_FLOAT_WITHIN((float)2.0, (float)1.5, (float)a.o2_percent);
}

void test_atmosphere_at_oxidizing(void)
{
    ed_atmosphere_info_t a = ed_atmosphere_at(1000.0);
    TEST_ASSERT_EQUAL_INT(ED_ATMO_OXIDIZING, a.state);
}

void test_atmosphere_at_oxidizing_significant_o2(void)
{
    ed_atmosphere_info_t a = ed_atmosphere_at(1000.0);
    /* Oxidizing: ~2-15% O2 */
    TEST_ASSERT_TRUE(a.o2_percent >= 2.0);
    TEST_ASSERT_TRUE(a.o2_percent <= 15.0);
}

void test_atmosphere_at_ozone_era(void)
{
    ed_atmosphere_info_t a = ed_atmosphere_at(500.0);
    TEST_ASSERT_EQUAL_INT(ED_ATMO_OZONE, a.state);
}

void test_atmosphere_at_ozone_uv_shielded(void)
{
    ed_atmosphere_info_t a = ed_atmosphere_at(500.0);
    TEST_ASSERT_EQUAL_INT(1, a.uv_shielded);
}

void test_atmosphere_at_modern(void)
{
    ed_atmosphere_info_t a = ed_atmosphere_at(200.0);
    TEST_ASSERT_EQUAL_INT(ED_ATMO_MODERN, a.state);
}

void test_atmosphere_at_modern_21_percent_o2(void)
{
    ed_atmosphere_info_t a = ed_atmosphere_at(0.0);
    TEST_ASSERT_FLOAT_WITHIN((float)2.0, (float)21.0, (float)a.o2_percent);
}

void test_atmosphere_at_modern_uv_shielded(void)
{
    ed_atmosphere_info_t a = ed_atmosphere_at(100.0);
    TEST_ASSERT_EQUAL_INT(1, a.uv_shielded);
}

void test_atmosphere_at_has_description(void)
{
    ed_atmosphere_info_t a = ed_atmosphere_at(3000.0);
    TEST_ASSERT_NOT_NULL(a.description);
    TEST_ASSERT_TRUE(strlen(a.description) > 0);
}

/* ===== ed_atmosphere_name — state name string ===== */

void test_atmosphere_name_reducing(void)
{
    const char *name = ed_atmosphere_name(ED_ATMO_REDUCING);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_EQUAL_STRING("Reducing", name);
}

void test_atmosphere_name_transitional(void)
{
    const char *name = ed_atmosphere_name(ED_ATMO_TRANSITIONAL);
    TEST_ASSERT_EQUAL_STRING("Transitional", name);
}

void test_atmosphere_name_oxidizing(void)
{
    const char *name = ed_atmosphere_name(ED_ATMO_OXIDIZING);
    TEST_ASSERT_EQUAL_STRING("Oxidizing", name);
}

void test_atmosphere_name_ozone(void)
{
    const char *name = ed_atmosphere_name(ED_ATMO_OZONE);
    TEST_ASSERT_EQUAL_STRING("Ozone", name);
}

void test_atmosphere_name_modern(void)
{
    const char *name = ed_atmosphere_name(ED_ATMO_MODERN);
    TEST_ASSERT_EQUAL_STRING("Modern", name);
}

/* ===== ed_atmosphere_transition_count / get ===== */

void test_atmosphere_transition_count_is_5(void)
{
    /* 5 atmosphere states = 5 transition entries */
    TEST_ASSERT_EQUAL_INT(5, ed_atmosphere_transition_count());
}

void test_atmosphere_transition_first_is_reducing(void)
{
    ed_atmosphere_info_t t = ed_atmosphere_transition_get(0);
    TEST_ASSERT_EQUAL_INT(ED_ATMO_REDUCING, t.state);
}

void test_atmosphere_transition_last_is_modern(void)
{
    int last = ed_atmosphere_transition_count() - 1;
    ed_atmosphere_info_t t = ed_atmosphere_transition_get(last);
    TEST_ASSERT_EQUAL_INT(ED_ATMO_MODERN, t.state);
}

void test_atmosphere_transition_invalid_index(void)
{
    ed_atmosphere_info_t t = ed_atmosphere_transition_get(-1);
    TEST_ASSERT_NULL(t.description);
}

void test_atmosphere_transition_out_of_range(void)
{
    ed_atmosphere_info_t t = ed_atmosphere_transition_get(99);
    TEST_ASSERT_NULL(t.description);
}

/* ===== ed_biosphere_at — what was alive ===== */

void test_biosphere_pre_life_nothing(void)
{
    ed_biosphere_t b = ed_biosphere_at(4000.0);
    TEST_ASSERT_EQUAL_INT(0, b.count);
    TEST_ASSERT_EQUAL_INT(0, b.multicellular);
    TEST_ASSERT_EQUAL_INT(0, b.land_life);
    TEST_ASSERT_EQUAL_INT(0, b.intelligence);
}

void test_biosphere_prokaryotes_era(void)
{
    ed_biosphere_t b = ed_biosphere_at(3500.0);
    TEST_ASSERT_TRUE(b.count > 0);
    TEST_ASSERT_NOT_NULL(b.dominant);
    TEST_ASSERT_EQUAL_INT(0, b.multicellular);
}

void test_biosphere_cyanobacteria_era(void)
{
    ed_biosphere_t b = ed_biosphere_at(2500.0);
    TEST_ASSERT_TRUE(b.count >= 2);
    TEST_ASSERT_EQUAL_INT(0, b.multicellular);
}

void test_biosphere_eukaryotes_era(void)
{
    ed_biosphere_t b = ed_biosphere_at(1500.0);
    TEST_ASSERT_TRUE(b.count >= 3);
    TEST_ASSERT_EQUAL_INT(0, b.multicellular);
}

void test_biosphere_multicellular_ediacaran(void)
{
    ed_biosphere_t b = ed_biosphere_at(600.0);
    TEST_ASSERT_EQUAL_INT(1, b.multicellular);
    TEST_ASSERT_EQUAL_INT(0, b.land_life);
}

void test_biosphere_cambrian_animals(void)
{
    ed_biosphere_t b = ed_biosphere_at(500.0);
    TEST_ASSERT_EQUAL_INT(1, b.multicellular);
    TEST_ASSERT_EQUAL_INT(0, b.land_life);
}

void test_biosphere_land_plants(void)
{
    ed_biosphere_t b = ed_biosphere_at(450.0);
    TEST_ASSERT_EQUAL_INT(1, b.land_life);
}

void test_biosphere_tetrapods(void)
{
    ed_biosphere_t b = ed_biosphere_at(350.0);
    TEST_ASSERT_EQUAL_INT(1, b.land_life);
    TEST_ASSERT_EQUAL_INT(1, b.multicellular);
}

void test_biosphere_dinosaur_era(void)
{
    ed_biosphere_t b = ed_biosphere_at(150.0);
    TEST_ASSERT_NOT_NULL(b.dominant);
    TEST_ASSERT_EQUAL_INT(1, b.land_life);
    TEST_ASSERT_EQUAL_INT(0, b.intelligence);
}

void test_biosphere_mammal_dominance(void)
{
    ed_biosphere_t b = ed_biosphere_at(30.0);
    TEST_ASSERT_NOT_NULL(b.dominant);
    TEST_ASSERT_EQUAL_INT(0, b.intelligence);
}

void test_biosphere_humans(void)
{
    ed_biosphere_t b = ed_biosphere_at(0.1);
    TEST_ASSERT_EQUAL_INT(1, b.intelligence);
    TEST_ASSERT_EQUAL_INT(1, b.multicellular);
    TEST_ASSERT_EQUAL_INT(1, b.land_life);
}

void test_biosphere_life_forms_not_null(void)
{
    ed_biosphere_t b = ed_biosphere_at(100.0);
    for (int i = 0; i < b.count; i++) {
        TEST_ASSERT_NOT_NULL(b.life_forms[i]);
    }
}

void test_biosphere_count_within_max(void)
{
    ed_biosphere_t b = ed_biosphere_at(0.0);
    TEST_ASSERT_TRUE(b.count <= ED_MAX_ALIVE);
    TEST_ASSERT_TRUE(b.count > 0);
}

/* ===== ed_human_scale — human arrival scale ===== */

void test_human_scale_earth_age(void)
{
    ed_human_scale_t s = ed_human_scale();
    TEST_ASSERT_FLOAT_WITHIN((float)1.0, (float)ED_EARTH_AGE_MA,
                             (float)s.earth_age_ma);
}

void test_human_scale_human_age(void)
{
    ed_human_scale_t s = ed_human_scale();
    TEST_ASSERT_FLOAT_WITHIN((float)0.05, (float)0.3, (float)s.human_age_ma);
}

void test_human_scale_ratio(void)
{
    ed_human_scale_t s = ed_human_scale();
    double expected = 0.3 / ED_EARTH_AGE_MA;
    TEST_ASSERT_FLOAT_WITHIN((float)1e-6, (float)expected, (float)s.ratio);
}

void test_human_scale_24h_very_small(void)
{
    ed_human_scale_t s = ed_human_scale();
    /* ratio = 0.3/4540 ~= 6.6e-5, * 86400 ~= 5.71 seconds before midnight */
    TEST_ASSERT_FLOAT_WITHIN((float)1.0, (float)5.71, (float)s.if_earth_were_24h);
    TEST_ASSERT_TRUE(s.if_earth_were_24h > 0.0);
}

void test_human_scale_year_late_december(void)
{
    ed_human_scale_t s = ed_human_scale();
    /* Humans arrive very late in the year (day ~365) */
    TEST_ASSERT_TRUE(s.if_earth_were_year > 364.0);
    TEST_ASSERT_TRUE(s.if_earth_were_year <= 365.0);
}

void test_human_scale_analogy_not_null(void)
{
    ed_human_scale_t s = ed_human_scale();
    TEST_ASSERT_NOT_NULL(s.analogy);
    TEST_ASSERT_TRUE(strlen(s.analogy) > 0);
}

/* ===== ed_drama_text — dramatic one-liners ===== */

void test_drama_text_hadean(void)
{
    const char *text = ed_drama_text(4200.0);
    TEST_ASSERT_NOT_NULL(text);
    TEST_ASSERT_TRUE(strlen(text) > 0);
}

void test_drama_text_cambrian(void)
{
    const char *text = ed_drama_text(520.0);
    TEST_ASSERT_NOT_NULL(text);
}

void test_drama_text_present(void)
{
    const char *text = ed_drama_text(0.0);
    TEST_ASSERT_NOT_NULL(text);
}

void test_drama_text_dinosaur_era(void)
{
    const char *text = ed_drama_text(150.0);
    TEST_ASSERT_NOT_NULL(text);
    TEST_ASSERT_TRUE(strlen(text) > 0);
}

/* ===== ed_snapshot_at — full dramatic snapshot ===== */

void test_snapshot_at_returns_queried_ma(void)
{
    ed_snapshot_t s = ed_snapshot_at(500.0);
    TEST_ASSERT_FLOAT_WITHIN((float)0.01, (float)500.0, (float)s.ma);
}

void test_snapshot_at_has_era_name(void)
{
    ed_snapshot_t s = ed_snapshot_at(100.0);
    TEST_ASSERT_NOT_NULL(s.era_name);
}

void test_snapshot_at_has_period_name(void)
{
    ed_snapshot_t s = ed_snapshot_at(100.0);
    TEST_ASSERT_NOT_NULL(s.period_name);
}

void test_snapshot_at_has_atmosphere(void)
{
    ed_snapshot_t s = ed_snapshot_at(3000.0);
    TEST_ASSERT_EQUAL_INT(ED_ATMO_REDUCING, s.atmosphere.state);
}

void test_snapshot_at_has_biosphere(void)
{
    ed_snapshot_t s = ed_snapshot_at(500.0);
    TEST_ASSERT_TRUE(s.biosphere.count > 0);
}

void test_snapshot_at_has_drama_text(void)
{
    ed_snapshot_t s = ed_snapshot_at(250.0);
    TEST_ASSERT_NOT_NULL(s.drama_text);
}

void test_snapshot_at_glaciation_flag(void)
{
    /* Carboniferous Ice Age: 360-260 Ma */
    ed_snapshot_t s = ed_snapshot_at(300.0);
    TEST_ASSERT_EQUAL_INT(1, s.is_glaciated);
}

void test_snapshot_at_no_glaciation(void)
{
    /* Jurassic: warm, no major glaciation */
    ed_snapshot_t s = ed_snapshot_at(180.0);
    TEST_ASSERT_EQUAL_INT(0, s.is_glaciated);
}

void test_snapshot_at_extinction_flag(void)
{
    /* K-Pg extinction at ~66 Ma */
    ed_snapshot_t s = ed_snapshot_at(66.0);
    TEST_ASSERT_EQUAL_INT(1, s.is_extinction);
}

void test_snapshot_at_no_extinction(void)
{
    ed_snapshot_t s = ed_snapshot_at(100.0);
    TEST_ASSERT_EQUAL_INT(0, s.is_extinction);
}

/* ===== Edge cases ===== */

void test_atmosphere_at_exact_boundary_2400(void)
{
    /* 2400 Ma is the boundary between reducing and transitional */
    ed_atmosphere_info_t a = ed_atmosphere_at(2400.0);
    TEST_ASSERT_EQUAL_INT(ED_ATMO_TRANSITIONAL, a.state);
}

void test_atmosphere_at_exact_boundary_400(void)
{
    /* 400 Ma is boundary between ozone and modern */
    ed_atmosphere_info_t a = ed_atmosphere_at(400.0);
    TEST_ASSERT_EQUAL_INT(ED_ATMO_MODERN, a.state);
}

void test_biosphere_at_exactly_zero(void)
{
    ed_biosphere_t b = ed_biosphere_at(0.0);
    TEST_ASSERT_EQUAL_INT(1, b.intelligence);
}

void test_snapshot_at_very_early_earth(void)
{
    ed_snapshot_t s = ed_snapshot_at(4500.0);
    TEST_ASSERT_EQUAL_INT(ED_ATMO_REDUCING, s.atmosphere.state);
    TEST_ASSERT_EQUAL_INT(0, s.biosphere.count);
}

void test_atmosphere_at_negative_returns_modern(void)
{
    /* Negative Ma (future) should return modern */
    ed_atmosphere_info_t a = ed_atmosphere_at(-1.0);
    TEST_ASSERT_EQUAL_INT(ED_ATMO_MODERN, a.state);
}

void test_biosphere_dominant_is_meaningful_string(void)
{
    ed_biosphere_t b = ed_biosphere_at(150.0);
    TEST_ASSERT_NOT_NULL(b.dominant);
    TEST_ASSERT_TRUE(strlen(b.dominant) > 3);
}

/* ===== main ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Atmosphere state lookup */
    RUN_TEST(test_atmosphere_at_hadean_is_reducing);
    RUN_TEST(test_atmosphere_at_reducing_has_no_o2);
    RUN_TEST(test_atmosphere_at_reducing_not_uv_shielded);
    RUN_TEST(test_atmosphere_at_transitional_goe);
    RUN_TEST(test_atmosphere_at_transitional_trace_o2);
    RUN_TEST(test_atmosphere_at_oxidizing);
    RUN_TEST(test_atmosphere_at_oxidizing_significant_o2);
    RUN_TEST(test_atmosphere_at_ozone_era);
    RUN_TEST(test_atmosphere_at_ozone_uv_shielded);
    RUN_TEST(test_atmosphere_at_modern);
    RUN_TEST(test_atmosphere_at_modern_21_percent_o2);
    RUN_TEST(test_atmosphere_at_modern_uv_shielded);
    RUN_TEST(test_atmosphere_at_has_description);

    /* Atmosphere name */
    RUN_TEST(test_atmosphere_name_reducing);
    RUN_TEST(test_atmosphere_name_transitional);
    RUN_TEST(test_atmosphere_name_oxidizing);
    RUN_TEST(test_atmosphere_name_ozone);
    RUN_TEST(test_atmosphere_name_modern);

    /* Atmosphere transitions */
    RUN_TEST(test_atmosphere_transition_count_is_5);
    RUN_TEST(test_atmosphere_transition_first_is_reducing);
    RUN_TEST(test_atmosphere_transition_last_is_modern);
    RUN_TEST(test_atmosphere_transition_invalid_index);
    RUN_TEST(test_atmosphere_transition_out_of_range);

    /* Biosphere */
    RUN_TEST(test_biosphere_pre_life_nothing);
    RUN_TEST(test_biosphere_prokaryotes_era);
    RUN_TEST(test_biosphere_cyanobacteria_era);
    RUN_TEST(test_biosphere_eukaryotes_era);
    RUN_TEST(test_biosphere_multicellular_ediacaran);
    RUN_TEST(test_biosphere_cambrian_animals);
    RUN_TEST(test_biosphere_land_plants);
    RUN_TEST(test_biosphere_tetrapods);
    RUN_TEST(test_biosphere_dinosaur_era);
    RUN_TEST(test_biosphere_mammal_dominance);
    RUN_TEST(test_biosphere_humans);
    RUN_TEST(test_biosphere_life_forms_not_null);
    RUN_TEST(test_biosphere_count_within_max);

    /* Human scale */
    RUN_TEST(test_human_scale_earth_age);
    RUN_TEST(test_human_scale_human_age);
    RUN_TEST(test_human_scale_ratio);
    RUN_TEST(test_human_scale_24h_very_small);
    RUN_TEST(test_human_scale_year_late_december);
    RUN_TEST(test_human_scale_analogy_not_null);

    /* Drama text */
    RUN_TEST(test_drama_text_hadean);
    RUN_TEST(test_drama_text_cambrian);
    RUN_TEST(test_drama_text_present);
    RUN_TEST(test_drama_text_dinosaur_era);

    /* Full snapshot */
    RUN_TEST(test_snapshot_at_returns_queried_ma);
    RUN_TEST(test_snapshot_at_has_era_name);
    RUN_TEST(test_snapshot_at_has_period_name);
    RUN_TEST(test_snapshot_at_has_atmosphere);
    RUN_TEST(test_snapshot_at_has_biosphere);
    RUN_TEST(test_snapshot_at_has_drama_text);
    RUN_TEST(test_snapshot_at_glaciation_flag);
    RUN_TEST(test_snapshot_at_no_glaciation);
    RUN_TEST(test_snapshot_at_extinction_flag);
    RUN_TEST(test_snapshot_at_no_extinction);

    /* Edge cases */
    RUN_TEST(test_atmosphere_at_exact_boundary_2400);
    RUN_TEST(test_atmosphere_at_exact_boundary_400);
    RUN_TEST(test_biosphere_at_exactly_zero);
    RUN_TEST(test_snapshot_at_very_early_earth);
    RUN_TEST(test_atmosphere_at_negative_returns_modern);
    RUN_TEST(test_biosphere_dominant_is_meaningful_string);

    return UNITY_END();
}
