/* test_cosmic_duality.c — Bundahishn cosmic duality timeline tests
 * TDD RED phase: tests written before implementation.
 * 12,000-year cosmic cycle: Creation -> Mixture -> Separation.
 * 7 Amesha Spentas parallel to Kabbalistic Sefirot. */

#include "../../unity/unity.h"
#include "../../../src/systems/zoroastrian/cosmic_duality.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Era determination ===== */

void test_era_creation_start(void)
{
    TEST_ASSERT_EQUAL_INT(CD_ERA_CREATION, cd_era(0));
}

void test_era_creation_mid(void)
{
    TEST_ASSERT_EQUAL_INT(CD_ERA_CREATION, cd_era(2999));
}

void test_era_creation_3000(void)
{
    TEST_ASSERT_EQUAL_INT(CD_ERA_CREATION, cd_era(3000));
}

void test_era_creation_5999(void)
{
    TEST_ASSERT_EQUAL_INT(CD_ERA_CREATION, cd_era(5999));
}

void test_era_mixture_start(void)
{
    TEST_ASSERT_EQUAL_INT(CD_ERA_MIXTURE, cd_era(6000));
}

void test_era_mixture_end(void)
{
    TEST_ASSERT_EQUAL_INT(CD_ERA_MIXTURE, cd_era(8999));
}

void test_era_separation_start(void)
{
    TEST_ASSERT_EQUAL_INT(CD_ERA_SEPARATION, cd_era(9000));
}

void test_era_separation_end(void)
{
    TEST_ASSERT_EQUAL_INT(CD_ERA_SEPARATION, cd_era(11999));
}

/* ===== Era names ===== */

void test_era_name_creation(void)
{
    TEST_ASSERT_EQUAL_STRING("Creation", cd_era_name(CD_ERA_CREATION));
}

void test_era_name_mixture(void)
{
    TEST_ASSERT_EQUAL_STRING("Mixture", cd_era_name(CD_ERA_MIXTURE));
}

void test_era_name_separation(void)
{
    TEST_ASSERT_EQUAL_STRING("Separation", cd_era_name(CD_ERA_SEPARATION));
}

void test_era_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", cd_era_name(CD_ERA_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", cd_era_name(-1));
}

/* ===== Era descriptions ===== */

void test_era_description_creation(void)
{
    const char *desc = cd_era_description(CD_ERA_CREATION);
    TEST_ASSERT_NOT_NULL(desc);
    TEST_ASSERT_TRUE(strlen(desc) > 10);
    /* Should mention Ahura Mazda */
    TEST_ASSERT_NOT_NULL(strstr(desc, "Ahura Mazda"));
}

void test_era_description_mixture(void)
{
    const char *desc = cd_era_description(CD_ERA_MIXTURE);
    TEST_ASSERT_NOT_NULL(desc);
    TEST_ASSERT_NOT_NULL(strstr(desc, "Angra Mainyu"));
}

void test_era_description_separation(void)
{
    const char *desc = cd_era_description(CD_ERA_SEPARATION);
    TEST_ASSERT_NOT_NULL(desc);
    TEST_ASSERT_NOT_NULL(strstr(desc, "Saoshyant"));
}

void test_era_description_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", cd_era_description(CD_ERA_COUNT));
}

/* ===== Period determination ===== */

void test_period_spiritual_start(void)
{
    TEST_ASSERT_EQUAL_INT(CD_PERIOD_SPIRITUAL_CREATION, cd_period(0));
}

void test_period_spiritual_end(void)
{
    TEST_ASSERT_EQUAL_INT(CD_PERIOD_SPIRITUAL_CREATION, cd_period(2999));
}

void test_period_material_start(void)
{
    TEST_ASSERT_EQUAL_INT(CD_PERIOD_MATERIAL_CREATION, cd_period(3000));
}

void test_period_material_end(void)
{
    TEST_ASSERT_EQUAL_INT(CD_PERIOD_MATERIAL_CREATION, cd_period(5999));
}

void test_period_mixture_start(void)
{
    TEST_ASSERT_EQUAL_INT(CD_PERIOD_MIXTURE, cd_period(6000));
}

void test_period_mixture_end(void)
{
    TEST_ASSERT_EQUAL_INT(CD_PERIOD_MIXTURE, cd_period(8999));
}

void test_period_renovation_start(void)
{
    TEST_ASSERT_EQUAL_INT(CD_PERIOD_RENOVATION, cd_period(9000));
}

void test_period_renovation_end(void)
{
    TEST_ASSERT_EQUAL_INT(CD_PERIOD_RENOVATION, cd_period(11999));
}

/* ===== Period names ===== */

void test_period_name_spiritual(void)
{
    TEST_ASSERT_EQUAL_STRING("Spiritual Creation",
                             cd_period_name(CD_PERIOD_SPIRITUAL_CREATION));
}

void test_period_name_material(void)
{
    TEST_ASSERT_EQUAL_STRING("Material Creation",
                             cd_period_name(CD_PERIOD_MATERIAL_CREATION));
}

void test_period_name_mixture(void)
{
    TEST_ASSERT_EQUAL_STRING("Mixture",
                             cd_period_name(CD_PERIOD_MIXTURE));
}

void test_period_name_renovation(void)
{
    TEST_ASSERT_EQUAL_STRING("Renovation",
                             cd_period_name(CD_PERIOD_RENOVATION));
}

void test_period_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", cd_period_name(CD_PERIOD_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", cd_period_name(-1));
}

/* ===== Progress ===== */

void test_progress_start(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 0.0, cd_progress(0));
}

void test_progress_half(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 0.5, cd_progress(6000));
}

void test_progress_end(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 1.0, cd_progress(12000));
}

void test_progress_quarter(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 0.25, cd_progress(3000));
}

void test_progress_three_quarters(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 0.75, cd_progress(9000));
}

/* ===== Full position ===== */

void test_position_at_0(void)
{
    cd_position_t pos = cd_position(0);
    TEST_ASSERT_EQUAL_INT(CD_ERA_CREATION, pos.era);
    TEST_ASSERT_EQUAL_INT(CD_PERIOD_SPIRITUAL_CREATION, pos.period);
    TEST_ASSERT_EQUAL_INT(0, pos.cycle_year);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 0.0, pos.progress);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 0.0, pos.era_progress);
    TEST_ASSERT_NOT_NULL(pos.description);
}

void test_position_at_3000(void)
{
    cd_position_t pos = cd_position(3000);
    TEST_ASSERT_EQUAL_INT(CD_ERA_CREATION, pos.era);
    TEST_ASSERT_EQUAL_INT(CD_PERIOD_MATERIAL_CREATION, pos.period);
    TEST_ASSERT_EQUAL_INT(3000, pos.cycle_year);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 0.25, pos.progress);
    /* era_progress: 3000 into a 6000-year era = 0.5 */
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 0.5, pos.era_progress);
}

void test_position_at_7500(void)
{
    cd_position_t pos = cd_position(7500);
    TEST_ASSERT_EQUAL_INT(CD_ERA_MIXTURE, pos.era);
    TEST_ASSERT_EQUAL_INT(CD_PERIOD_MIXTURE, pos.period);
    TEST_ASSERT_EQUAL_INT(7500, pos.cycle_year);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 0.625, pos.progress);
    /* era_progress: 1500 into a 3000-year era = 0.5 */
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 0.5, pos.era_progress);
}

void test_position_at_10000(void)
{
    cd_position_t pos = cd_position(10000);
    TEST_ASSERT_EQUAL_INT(CD_ERA_SEPARATION, pos.era);
    TEST_ASSERT_EQUAL_INT(CD_PERIOD_RENOVATION, pos.period);
    TEST_ASSERT_EQUAL_INT(10000, pos.cycle_year);
}

void test_position_description_spiritual(void)
{
    cd_position_t pos = cd_position(1500);
    TEST_ASSERT_NOT_NULL(strstr(pos.description, "Ahura Mazda"));
}

void test_position_description_material(void)
{
    cd_position_t pos = cd_position(4000);
    TEST_ASSERT_NOT_NULL(strstr(pos.description, "physical form"));
}

void test_position_description_mixture(void)
{
    cd_position_t pos = cd_position(7000);
    TEST_ASSERT_NOT_NULL(strstr(pos.description, "intertwined"));
}

void test_position_description_renovation(void)
{
    cd_position_t pos = cd_position(10000);
    TEST_ASSERT_NOT_NULL(strstr(pos.description, "renovation"));
}

/* ===== Amesha Spentas ===== */

void test_amesha_spenta_ahura_mazda(void)
{
    cd_amesha_spenta_t a = cd_amesha_spenta(0);
    TEST_ASSERT_EQUAL_INT(0, a.index);
    TEST_ASSERT_EQUAL_STRING("Ahura Mazda", a.avestan_name);
    TEST_ASSERT_EQUAL_STRING("Wise Lord", a.meaning);
    TEST_ASSERT_EQUAL_STRING("All creation", a.domain);
    TEST_ASSERT_EQUAL_STRING("Fire/Light", a.element);
    TEST_ASSERT_EQUAL_STRING("Keter", a.sefirah_parallel);
    TEST_ASSERT_EQUAL_INT(1, a.day_number);
}

void test_amesha_spenta_vohu_manah(void)
{
    cd_amesha_spenta_t a = cd_amesha_spenta(1);
    TEST_ASSERT_EQUAL_STRING("Vohu Manah", a.avestan_name);
    TEST_ASSERT_EQUAL_STRING("Good Mind", a.meaning);
    TEST_ASSERT_EQUAL_STRING("Cattle/Animals", a.domain);
    TEST_ASSERT_EQUAL_STRING("Air", a.element);
    TEST_ASSERT_EQUAL_STRING("Chokmah", a.sefirah_parallel);
    TEST_ASSERT_EQUAL_INT(2, a.day_number);
}

void test_amesha_spenta_asha_vahishta(void)
{
    cd_amesha_spenta_t a = cd_amesha_spenta(2);
    TEST_ASSERT_EQUAL_STRING("Asha Vahishta", a.avestan_name);
    TEST_ASSERT_EQUAL_STRING("Best Righteousness", a.meaning);
    TEST_ASSERT_EQUAL_STRING("Fire", a.domain);
    TEST_ASSERT_EQUAL_STRING("Fire", a.element);
    TEST_ASSERT_EQUAL_STRING("Binah", a.sefirah_parallel);
    TEST_ASSERT_EQUAL_INT(3, a.day_number);
}

void test_amesha_spenta_khshathra(void)
{
    cd_amesha_spenta_t a = cd_amesha_spenta(3);
    TEST_ASSERT_EQUAL_STRING("Khshathra Vairya", a.avestan_name);
    TEST_ASSERT_EQUAL_STRING("Desirable Dominion", a.meaning);
    TEST_ASSERT_EQUAL_STRING("Metals/Sky", a.domain);
    TEST_ASSERT_EQUAL_STRING("Metal", a.element);
    TEST_ASSERT_EQUAL_STRING("Chesed", a.sefirah_parallel);
    TEST_ASSERT_EQUAL_INT(4, a.day_number);
}

void test_amesha_spenta_armaiti(void)
{
    cd_amesha_spenta_t a = cd_amesha_spenta(4);
    TEST_ASSERT_EQUAL_STRING("Spenta Armaiti", a.avestan_name);
    TEST_ASSERT_EQUAL_STRING("Holy Devotion", a.meaning);
    TEST_ASSERT_EQUAL_STRING("Earth", a.domain);
    TEST_ASSERT_EQUAL_STRING("Earth", a.element);
    TEST_ASSERT_EQUAL_STRING("Gevurah", a.sefirah_parallel);
    TEST_ASSERT_EQUAL_INT(5, a.day_number);
}

void test_amesha_spenta_haurvatat(void)
{
    cd_amesha_spenta_t a = cd_amesha_spenta(5);
    TEST_ASSERT_EQUAL_STRING("Haurvatat", a.avestan_name);
    TEST_ASSERT_EQUAL_STRING("Wholeness", a.meaning);
    TEST_ASSERT_EQUAL_STRING("Water", a.domain);
    TEST_ASSERT_EQUAL_STRING("Water", a.element);
    TEST_ASSERT_EQUAL_STRING("Tiferet", a.sefirah_parallel);
    TEST_ASSERT_EQUAL_INT(6, a.day_number);
}

void test_amesha_spenta_ameretat(void)
{
    cd_amesha_spenta_t a = cd_amesha_spenta(6);
    TEST_ASSERT_EQUAL_STRING("Ameretat", a.avestan_name);
    TEST_ASSERT_EQUAL_STRING("Immortality", a.meaning);
    TEST_ASSERT_EQUAL_STRING("Plants", a.domain);
    TEST_ASSERT_EQUAL_STRING("Wood", a.element);
    TEST_ASSERT_EQUAL_STRING("Netzach", a.sefirah_parallel);
    TEST_ASSERT_EQUAL_INT(7, a.day_number);
}

void test_amesha_spenta_invalid(void)
{
    cd_amesha_spenta_t a = cd_amesha_spenta(-1);
    TEST_ASSERT_EQUAL_INT(-1, a.index);
    TEST_ASSERT_EQUAL_STRING("?", a.avestan_name);
}

void test_amesha_spenta_invalid_high(void)
{
    cd_amesha_spenta_t a = cd_amesha_spenta(7);
    TEST_ASSERT_EQUAL_INT(-1, a.index);
    TEST_ASSERT_EQUAL_STRING("?", a.avestan_name);
}

/* ===== Day-to-Amesha-Spenta mapping ===== */

void test_day_mapping_day1(void)
{
    cd_amesha_spenta_t a = cd_amesha_spenta_for_day(1);
    TEST_ASSERT_EQUAL_STRING("Ahura Mazda", a.avestan_name);
    TEST_ASSERT_EQUAL_INT(0, a.index);
}

void test_day_mapping_day7(void)
{
    cd_amesha_spenta_t a = cd_amesha_spenta_for_day(7);
    TEST_ASSERT_EQUAL_STRING("Ameretat", a.avestan_name);
    TEST_ASSERT_EQUAL_INT(6, a.index);
}

void test_day_mapping_day4(void)
{
    cd_amesha_spenta_t a = cd_amesha_spenta_for_day(4);
    TEST_ASSERT_EQUAL_STRING("Khshathra Vairya", a.avestan_name);
}

void test_day_mapping_day8_invalid(void)
{
    cd_amesha_spenta_t a = cd_amesha_spenta_for_day(8);
    TEST_ASSERT_EQUAL_INT(-1, a.index);
    TEST_ASSERT_EQUAL_STRING("?", a.avestan_name);
}

void test_day_mapping_day30_invalid(void)
{
    cd_amesha_spenta_t a = cd_amesha_spenta_for_day(30);
    TEST_ASSERT_EQUAL_INT(-1, a.index);
}

void test_day_mapping_day0_invalid(void)
{
    cd_amesha_spenta_t a = cd_amesha_spenta_for_day(0);
    TEST_ASSERT_EQUAL_INT(-1, a.index);
}

/* ===== Geological parallels ===== */

void test_geo_parallel_year_0(void)
{
    cd_geo_map_t g = cd_geological_parallel(0);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 4540.0, g.geological_ma);
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 0.0, g.cosmic_years);
    TEST_ASSERT_NOT_NULL(g.geological_parallel);
    TEST_ASSERT_NOT_NULL(strstr(g.geological_parallel, "Hadean"));
}

void test_geo_parallel_year_3000(void)
{
    cd_geo_map_t g = cd_geological_parallel(3000);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 3405.0, g.geological_ma);
    TEST_ASSERT_NOT_NULL(strstr(g.geological_parallel, "Archean"));
}

void test_geo_parallel_year_6000(void)
{
    cd_geo_map_t g = cd_geological_parallel(6000);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 2270.0, g.geological_ma);
    TEST_ASSERT_NOT_NULL(strstr(g.geological_parallel, "Proterozoic"));
}

void test_geo_parallel_year_9000(void)
{
    cd_geo_map_t g = cd_geological_parallel(9000);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 1135.0, g.geological_ma);
    TEST_ASSERT_NOT_NULL(g.geological_parallel);
}

void test_geo_parallel_year_12000(void)
{
    cd_geo_map_t g = cd_geological_parallel(12000);
    TEST_ASSERT_FLOAT_WITHIN(1.0, 0.0, g.geological_ma);
    TEST_ASSERT_NOT_NULL(strstr(g.geological_parallel, "Phanerozoic"));
}

/* ===== Amesha Spenta count ===== */

void test_amesha_count(void)
{
    TEST_ASSERT_EQUAL_INT(7, cd_amesha_spenta_count());
    TEST_ASSERT_EQUAL_INT(CD_AMESHA_COUNT, cd_amesha_spenta_count());
}

/* ===== Yazdgerdi to cycle year ===== */

void test_yazdgerdi_year_1(void)
{
    TEST_ASSERT_EQUAL_INT(9000, cd_cycle_year_from_yazdgerdi(1));
}

void test_yazdgerdi_year_100(void)
{
    TEST_ASSERT_EQUAL_INT(9099, cd_cycle_year_from_yazdgerdi(100));
}

void test_yazdgerdi_year_3000(void)
{
    /* 9000 + 2999 = 11999 */
    TEST_ASSERT_EQUAL_INT(11999, cd_cycle_year_from_yazdgerdi(3000));
}

void test_yazdgerdi_year_3001_clamped(void)
{
    /* 9000 + 3000 = 12000 -> clamped to 11999 */
    TEST_ASSERT_EQUAL_INT(11999, cd_cycle_year_from_yazdgerdi(3001));
}

void test_yazdgerdi_year_negative_clamped(void)
{
    /* Would produce cycle_year < 0, clamp to 0 */
    TEST_ASSERT_EQUAL_INT(0, cd_cycle_year_from_yazdgerdi(-10000));
}

/* ===== Edge cases ===== */

void test_era_negative_year(void)
{
    /* Negative years should clamp: treat as 0 */
    TEST_ASSERT_EQUAL_INT(CD_ERA_CREATION, cd_era(-1));
}

void test_era_beyond_cycle(void)
{
    /* Years >= 12000 should clamp to last era */
    TEST_ASSERT_EQUAL_INT(CD_ERA_SEPARATION, cd_era(12000));
    TEST_ASSERT_EQUAL_INT(CD_ERA_SEPARATION, cd_era(99999));
}

void test_period_negative_year(void)
{
    TEST_ASSERT_EQUAL_INT(CD_PERIOD_SPIRITUAL_CREATION, cd_period(-5));
}

void test_period_beyond_cycle(void)
{
    TEST_ASSERT_EQUAL_INT(CD_PERIOD_RENOVATION, cd_period(12000));
    TEST_ASSERT_EQUAL_INT(CD_PERIOD_RENOVATION, cd_period(50000));
}

void test_progress_negative_clamped(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 0.0, cd_progress(-100));
}

void test_progress_beyond_clamped(void)
{
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 1.0, cd_progress(99999));
}

void test_geo_parallel_negative(void)
{
    cd_geo_map_t g = cd_geological_parallel(-100);
    /* Clamped to 0 -> 4540 Ma */
    TEST_ASSERT_FLOAT_WITHIN(1.0, 4540.0, g.geological_ma);
    TEST_ASSERT_NOT_NULL(g.geological_parallel);
}

/* ===== Runner ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Era determination */
    RUN_TEST(test_era_creation_start);
    RUN_TEST(test_era_creation_mid);
    RUN_TEST(test_era_creation_3000);
    RUN_TEST(test_era_creation_5999);
    RUN_TEST(test_era_mixture_start);
    RUN_TEST(test_era_mixture_end);
    RUN_TEST(test_era_separation_start);
    RUN_TEST(test_era_separation_end);

    /* Era names */
    RUN_TEST(test_era_name_creation);
    RUN_TEST(test_era_name_mixture);
    RUN_TEST(test_era_name_separation);
    RUN_TEST(test_era_name_invalid);

    /* Era descriptions */
    RUN_TEST(test_era_description_creation);
    RUN_TEST(test_era_description_mixture);
    RUN_TEST(test_era_description_separation);
    RUN_TEST(test_era_description_invalid);

    /* Period determination */
    RUN_TEST(test_period_spiritual_start);
    RUN_TEST(test_period_spiritual_end);
    RUN_TEST(test_period_material_start);
    RUN_TEST(test_period_material_end);
    RUN_TEST(test_period_mixture_start);
    RUN_TEST(test_period_mixture_end);
    RUN_TEST(test_period_renovation_start);
    RUN_TEST(test_period_renovation_end);

    /* Period names */
    RUN_TEST(test_period_name_spiritual);
    RUN_TEST(test_period_name_material);
    RUN_TEST(test_period_name_mixture);
    RUN_TEST(test_period_name_renovation);
    RUN_TEST(test_period_name_invalid);

    /* Progress */
    RUN_TEST(test_progress_start);
    RUN_TEST(test_progress_half);
    RUN_TEST(test_progress_end);
    RUN_TEST(test_progress_quarter);
    RUN_TEST(test_progress_three_quarters);

    /* Full position */
    RUN_TEST(test_position_at_0);
    RUN_TEST(test_position_at_3000);
    RUN_TEST(test_position_at_7500);
    RUN_TEST(test_position_at_10000);
    RUN_TEST(test_position_description_spiritual);
    RUN_TEST(test_position_description_material);
    RUN_TEST(test_position_description_mixture);
    RUN_TEST(test_position_description_renovation);

    /* Amesha Spentas */
    RUN_TEST(test_amesha_spenta_ahura_mazda);
    RUN_TEST(test_amesha_spenta_vohu_manah);
    RUN_TEST(test_amesha_spenta_asha_vahishta);
    RUN_TEST(test_amesha_spenta_khshathra);
    RUN_TEST(test_amesha_spenta_armaiti);
    RUN_TEST(test_amesha_spenta_haurvatat);
    RUN_TEST(test_amesha_spenta_ameretat);
    RUN_TEST(test_amesha_spenta_invalid);
    RUN_TEST(test_amesha_spenta_invalid_high);

    /* Day mapping */
    RUN_TEST(test_day_mapping_day1);
    RUN_TEST(test_day_mapping_day7);
    RUN_TEST(test_day_mapping_day4);
    RUN_TEST(test_day_mapping_day8_invalid);
    RUN_TEST(test_day_mapping_day30_invalid);
    RUN_TEST(test_day_mapping_day0_invalid);

    /* Geological parallels */
    RUN_TEST(test_geo_parallel_year_0);
    RUN_TEST(test_geo_parallel_year_3000);
    RUN_TEST(test_geo_parallel_year_6000);
    RUN_TEST(test_geo_parallel_year_9000);
    RUN_TEST(test_geo_parallel_year_12000);

    /* Amesha Spenta count */
    RUN_TEST(test_amesha_count);

    /* Yazdgerdi conversion */
    RUN_TEST(test_yazdgerdi_year_1);
    RUN_TEST(test_yazdgerdi_year_100);
    RUN_TEST(test_yazdgerdi_year_3000);
    RUN_TEST(test_yazdgerdi_year_3001_clamped);
    RUN_TEST(test_yazdgerdi_year_negative_clamped);

    /* Edge cases */
    RUN_TEST(test_era_negative_year);
    RUN_TEST(test_era_beyond_cycle);
    RUN_TEST(test_period_negative_year);
    RUN_TEST(test_period_beyond_cycle);
    RUN_TEST(test_progress_negative_clamped);
    RUN_TEST(test_progress_beyond_clamped);
    RUN_TEST(test_geo_parallel_negative);

    return UNITY_END();
}
