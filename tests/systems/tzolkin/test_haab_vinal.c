#include "../../unity/unity.h"
#include "../../../src/systems/tzolkin/haab_vinal.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Constants ===== */

static void test_vinal_count_is_19(void)
{
    TEST_ASSERT_EQUAL_INT(19, hv_vinal_count());
}

static void test_haab_days_is_365(void)
{
    TEST_ASSERT_EQUAL_INT(365, hv_haab_days());
}

static void test_constant_hv_vinal_count(void)
{
    TEST_ASSERT_EQUAL_INT(18, HV_VINAL_COUNT);
}

static void test_constant_hv_total_periods(void)
{
    TEST_ASSERT_EQUAL_INT(19, HV_TOTAL_PERIODS);
}

static void test_constant_hv_vinal_days(void)
{
    TEST_ASSERT_EQUAL_INT(20, HV_VINAL_DAYS);
}

static void test_constant_hv_uayeb_days(void)
{
    TEST_ASSERT_EQUAL_INT(5, HV_UAYEB_DAYS);
}

static void test_constant_hv_haab_days(void)
{
    TEST_ASSERT_EQUAL_INT(365, HV_HAAB_DAYS);
}

/* ===== Day sum = 365 ===== */

static void test_day_sum_equals_365(void)
{
    int total = 0;
    for (int i = 1; i <= 19; i++) {
        haab_vinal_t v = hv_vinal_get(i);
        total += v.days;
    }
    TEST_ASSERT_EQUAL_INT(365, total);
}

static void test_day_sum_18x20_plus_5(void)
{
    TEST_ASSERT_EQUAL_INT(365, 18 * 20 + 5);
}

/* ===== Vinal 1: Pop ===== */

static void test_vinal1_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Pop", hv_vinal_name(1));
}

static void test_vinal1_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("The one who knows", hv_vinal_meaning(1));
}

static void test_vinal1_struct(void)
{
    haab_vinal_t v = hv_vinal_get(1);
    TEST_ASSERT_EQUAL_INT(1, v.number);
    TEST_ASSERT_EQUAL_STRING("Pop", v.name);
    TEST_ASSERT_EQUAL_STRING("The one who knows", v.meaning);
    TEST_ASSERT_EQUAL_INT(20, v.days);
}

/* ===== Vinal 2: Uo ===== */

static void test_vinal2_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Uo", hv_vinal_name(2));
}

static void test_vinal2_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("Listen in silence", hv_vinal_meaning(2));
}

static void test_vinal2_days(void)
{
    haab_vinal_t v = hv_vinal_get(2);
    TEST_ASSERT_EQUAL_INT(20, v.days);
}

/* ===== Vinal 3: Zip ===== */

static void test_vinal3_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Zip", hv_vinal_name(3));
}

static void test_vinal3_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("To integrate the universe", hv_vinal_meaning(3));
}

/* ===== Vinal 4: Zotz ===== */

static void test_vinal4_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Zotz", hv_vinal_name(4));
}

static void test_vinal4_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("Based on knowledge", hv_vinal_meaning(4));
}

/* ===== Vinal 5: Tzec ===== */

static void test_vinal5_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Tzec", hv_vinal_name(5));
}

static void test_vinal5_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("Reaching the foundations", hv_vinal_meaning(5));
}

/* ===== Vinal 6: Xul ===== */

static void test_vinal6_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Xul", hv_vinal_name(6));
}

static void test_vinal6_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("With great wisdom a seed is sown",
                             hv_vinal_meaning(6));
}

/* ===== Vinal 7: Yaxkin ===== */

static void test_vinal7_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Yaxkin", hv_vinal_name(7));
}

static void test_vinal7_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("A small ray of the hidden sun",
                             hv_vinal_meaning(7));
}

/* ===== Vinal 8: Mol ===== */

static void test_vinal8_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Mol", hv_vinal_name(8));
}

static void test_vinal8_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("Which unifies all parts", hv_vinal_meaning(8));
}

/* ===== Vinal 9: Chen ===== */

static void test_vinal9_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Chen", hv_vinal_name(9));
}

static void test_vinal9_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("To enter the inner wisdom source",
                             hv_vinal_meaning(9));
}

/* ===== Vinal 10: Yax ===== */

static void test_vinal10_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Yax", hv_vinal_name(10));
}

static void test_vinal10_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("Where the apprentice clarifies the mind",
                             hv_vinal_meaning(10));
}

/* ===== Vinal 11: Sac ===== */

static void test_vinal11_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Sac", hv_vinal_name(11));
}

static void test_vinal11_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("Dissipating doubt clouds, elevating oneself",
                             hv_vinal_meaning(11));
}

/* ===== Vinal 12: Ceh ===== */

static void test_vinal12_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Ceh", hv_vinal_name(12));
}

static void test_vinal12_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING(
        "Breaking habitual caution, reaching white light",
        hv_vinal_meaning(12));
}

/* ===== Vinal 13: Mac ===== */

static void test_vinal13_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Mac", hv_vinal_name(13));
}

static void test_vinal13_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("Closing the wrong place, entering ecstasy",
                             hv_vinal_meaning(13));
}

/* ===== Vinal 14: Kankin ===== */

static void test_vinal14_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Kankin", hv_vinal_name(14));
}

static void test_vinal14_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("Receiving the light of the one who knows",
                             hv_vinal_meaning(14));
}

/* ===== Vinal 15: Moan ===== */

static void test_vinal15_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Moan", hv_vinal_name(15));
}

static void test_vinal15_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("To see in the darkness", hv_vinal_meaning(15));
}

/* ===== Vinal 16: Pax ===== */

static void test_vinal16_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Pax", hv_vinal_name(16));
}

static void test_vinal16_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("Playing a music of the future",
                             hv_vinal_meaning(16));
}

/* ===== Vinal 17: Kayab ===== */

static void test_vinal17_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Kayab", hv_vinal_name(17));
}

static void test_vinal17_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("With the song and the rhythm",
                             hv_vinal_meaning(17));
}

/* ===== Vinal 18: Cumhu ===== */

static void test_vinal18_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Cumhu", hv_vinal_name(18));
}

static void test_vinal18_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING(
        "At the right place where divination food is obtained",
        hv_vinal_meaning(18));
}

/* ===== Vinal 19: Uayeb ===== */

static void test_vinal19_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Uayeb", hv_vinal_name(19));
}

static void test_vinal19_meaning(void)
{
    TEST_ASSERT_EQUAL_STRING("5-day closing period", hv_vinal_meaning(19));
}

static void test_vinal19_struct(void)
{
    haab_vinal_t v = hv_vinal_get(19);
    TEST_ASSERT_EQUAL_INT(19, v.number);
    TEST_ASSERT_EQUAL_STRING("Uayeb", v.name);
    TEST_ASSERT_EQUAL_INT(5, v.days);
}

static void test_uayeb_is_uayeb(void)
{
    TEST_ASSERT_EQUAL_INT(1, hv_is_uayeb(19));
}

static void test_uayeb_days_is_5(void)
{
    haab_vinal_t v = hv_vinal_get(19);
    TEST_ASSERT_EQUAL_INT(5, v.days);
}

/* ===== Regular vinales are NOT Uayeb ===== */

static void test_regular_vinales_not_uayeb(void)
{
    for (int i = 1; i <= 18; i++) {
        TEST_ASSERT_EQUAL_INT(0, hv_is_uayeb(i));
    }
}

static void test_regular_vinales_have_20_days(void)
{
    for (int i = 1; i <= 18; i++) {
        haab_vinal_t v = hv_vinal_get(i);
        TEST_ASSERT_EQUAL_INT(20, v.days);
    }
}

/* ===== All vinales have valid data ===== */

static void test_all_vinales_have_name(void)
{
    for (int i = 1; i <= 19; i++) {
        const char *name = hv_vinal_name(i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(strlen(name) > 0);
    }
}

static void test_all_vinales_have_meaning(void)
{
    for (int i = 1; i <= 19; i++) {
        const char *meaning = hv_vinal_meaning(i);
        TEST_ASSERT_NOT_NULL(meaning);
        TEST_ASSERT_TRUE(strlen(meaning) > 0);
    }
}

static void test_all_vinales_correct_number(void)
{
    for (int i = 1; i <= 19; i++) {
        haab_vinal_t v = hv_vinal_get(i);
        TEST_ASSERT_EQUAL_INT(i, v.number);
    }
}

static void test_all_names_unique(void)
{
    const char *names[19];
    for (int i = 0; i < 19; i++) {
        names[i] = hv_vinal_name(i + 1);
    }
    for (int i = 0; i < 19; i++) {
        for (int j = i + 1; j < 19; j++) {
            TEST_ASSERT_FALSE(strcmp(names[i], names[j]) == 0);
        }
    }
}

/* ===== vinal_for_day_of_year ===== */

static void test_day0_is_vinal1(void)
{
    TEST_ASSERT_EQUAL_INT(1, hv_vinal_for_day_of_year(0));
}

static void test_day19_is_vinal1(void)
{
    TEST_ASSERT_EQUAL_INT(1, hv_vinal_for_day_of_year(19));
}

static void test_day20_is_vinal2(void)
{
    TEST_ASSERT_EQUAL_INT(2, hv_vinal_for_day_of_year(20));
}

static void test_day39_is_vinal2(void)
{
    TEST_ASSERT_EQUAL_INT(2, hv_vinal_for_day_of_year(39));
}

static void test_day40_is_vinal3(void)
{
    TEST_ASSERT_EQUAL_INT(3, hv_vinal_for_day_of_year(40));
}

static void test_day100_is_vinal6(void)
{
    /* day 100 / 20 = 5, +1 = vinal 6 */
    TEST_ASSERT_EQUAL_INT(6, hv_vinal_for_day_of_year(100));
}

static void test_day199_is_vinal10(void)
{
    /* day 199 / 20 = 9, +1 = vinal 10 */
    TEST_ASSERT_EQUAL_INT(10, hv_vinal_for_day_of_year(199));
}

static void test_day339_is_vinal17(void)
{
    /* day 339 / 20 = 16, +1 = vinal 17 */
    TEST_ASSERT_EQUAL_INT(17, hv_vinal_for_day_of_year(339));
}

static void test_day340_is_vinal18(void)
{
    /* day 340 / 20 = 17, +1 = vinal 18 */
    TEST_ASSERT_EQUAL_INT(18, hv_vinal_for_day_of_year(340));
}

static void test_day359_is_vinal18(void)
{
    /* day 359 / 20 = 17, +1 = vinal 18 (last day of vinal 18) */
    TEST_ASSERT_EQUAL_INT(18, hv_vinal_for_day_of_year(359));
}

static void test_day360_is_uayeb(void)
{
    TEST_ASSERT_EQUAL_INT(19, hv_vinal_for_day_of_year(360));
}

static void test_day364_is_uayeb(void)
{
    TEST_ASSERT_EQUAL_INT(19, hv_vinal_for_day_of_year(364));
}

static void test_day361_is_uayeb(void)
{
    TEST_ASSERT_EQUAL_INT(19, hv_vinal_for_day_of_year(361));
}

static void test_day362_is_uayeb(void)
{
    TEST_ASSERT_EQUAL_INT(19, hv_vinal_for_day_of_year(362));
}

static void test_day363_is_uayeb(void)
{
    TEST_ASSERT_EQUAL_INT(19, hv_vinal_for_day_of_year(363));
}

/* Each vinal boundary: first day of each vinal */

static void test_vinal_boundaries_first_day(void)
{
    for (int v = 1; v <= 18; v++) {
        int first_day = (v - 1) * 20;
        TEST_ASSERT_EQUAL_INT(v, hv_vinal_for_day_of_year(first_day));
    }
    /* Uayeb starts at day 360 */
    TEST_ASSERT_EQUAL_INT(19, hv_vinal_for_day_of_year(360));
}

/* Each vinal boundary: last day of each vinal */

static void test_vinal_boundaries_last_day(void)
{
    for (int v = 1; v <= 18; v++) {
        int last_day = v * 20 - 1;
        TEST_ASSERT_EQUAL_INT(v, hv_vinal_for_day_of_year(last_day));
    }
    /* Uayeb ends at day 364 */
    TEST_ASSERT_EQUAL_INT(19, hv_vinal_for_day_of_year(364));
}

/* ===== Boundary: invalid inputs ===== */

static void test_invalid_num_0_get(void)
{
    haab_vinal_t v = hv_vinal_get(0);
    TEST_ASSERT_EQUAL_INT(0, v.number);
    TEST_ASSERT_NULL(v.name);
    TEST_ASSERT_NULL(v.meaning);
    TEST_ASSERT_EQUAL_INT(0, v.days);
}

static void test_invalid_num_20_get(void)
{
    haab_vinal_t v = hv_vinal_get(20);
    TEST_ASSERT_EQUAL_INT(0, v.number);
    TEST_ASSERT_NULL(v.name);
    TEST_ASSERT_NULL(v.meaning);
    TEST_ASSERT_EQUAL_INT(0, v.days);
}

static void test_invalid_num_neg1_get(void)
{
    haab_vinal_t v = hv_vinal_get(-1);
    TEST_ASSERT_EQUAL_INT(0, v.number);
    TEST_ASSERT_NULL(v.name);
}

static void test_invalid_num_100_get(void)
{
    haab_vinal_t v = hv_vinal_get(100);
    TEST_ASSERT_EQUAL_INT(0, v.number);
    TEST_ASSERT_NULL(v.name);
}

static void test_invalid_num_0_name(void)
{
    TEST_ASSERT_NULL(hv_vinal_name(0));
}

static void test_invalid_num_20_name(void)
{
    TEST_ASSERT_NULL(hv_vinal_name(20));
}

static void test_invalid_num_neg1_name(void)
{
    TEST_ASSERT_NULL(hv_vinal_name(-1));
}

static void test_invalid_num_0_meaning(void)
{
    TEST_ASSERT_NULL(hv_vinal_meaning(0));
}

static void test_invalid_num_20_meaning(void)
{
    TEST_ASSERT_NULL(hv_vinal_meaning(20));
}

static void test_invalid_day_neg1(void)
{
    TEST_ASSERT_EQUAL_INT(0, hv_vinal_for_day_of_year(-1));
}

static void test_invalid_day_365(void)
{
    TEST_ASSERT_EQUAL_INT(0, hv_vinal_for_day_of_year(365));
}

static void test_invalid_day_1000(void)
{
    TEST_ASSERT_EQUAL_INT(0, hv_vinal_for_day_of_year(1000));
}

static void test_invalid_day_neg100(void)
{
    TEST_ASSERT_EQUAL_INT(0, hv_vinal_for_day_of_year(-100));
}

/* ===== is_uayeb boundary ===== */

static void test_is_uayeb_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, hv_is_uayeb(0));
}

static void test_is_uayeb_neg1(void)
{
    TEST_ASSERT_EQUAL_INT(0, hv_is_uayeb(-1));
}

static void test_is_uayeb_20(void)
{
    TEST_ASSERT_EQUAL_INT(0, hv_is_uayeb(20));
}

static void test_is_uayeb_18(void)
{
    TEST_ASSERT_EQUAL_INT(0, hv_is_uayeb(18));
}

/* ===== Purity: same input, same output ===== */

static void test_purity_get(void)
{
    haab_vinal_t a = hv_vinal_get(7);
    haab_vinal_t b = hv_vinal_get(7);
    TEST_ASSERT_EQUAL_INT(a.number, b.number);
    TEST_ASSERT_EQUAL_STRING(a.name, b.name);
    TEST_ASSERT_EQUAL_STRING(a.meaning, b.meaning);
    TEST_ASSERT_EQUAL_INT(a.days, b.days);
}

static void test_purity_name(void)
{
    const char *a = hv_vinal_name(12);
    const char *b = hv_vinal_name(12);
    TEST_ASSERT_EQUAL_STRING(a, b);
}

static void test_purity_for_day(void)
{
    int a = hv_vinal_for_day_of_year(200);
    int b = hv_vinal_for_day_of_year(200);
    TEST_ASSERT_EQUAL_INT(a, b);
}

/* ===== main ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Constants */
    RUN_TEST(test_vinal_count_is_19);
    RUN_TEST(test_haab_days_is_365);
    RUN_TEST(test_constant_hv_vinal_count);
    RUN_TEST(test_constant_hv_total_periods);
    RUN_TEST(test_constant_hv_vinal_days);
    RUN_TEST(test_constant_hv_uayeb_days);
    RUN_TEST(test_constant_hv_haab_days);

    /* Day sums */
    RUN_TEST(test_day_sum_equals_365);
    RUN_TEST(test_day_sum_18x20_plus_5);

    /* Vinal 1 — Pop */
    RUN_TEST(test_vinal1_name);
    RUN_TEST(test_vinal1_meaning);
    RUN_TEST(test_vinal1_struct);

    /* Vinal 2 — Uo */
    RUN_TEST(test_vinal2_name);
    RUN_TEST(test_vinal2_meaning);
    RUN_TEST(test_vinal2_days);

    /* Vinal 3 — Zip */
    RUN_TEST(test_vinal3_name);
    RUN_TEST(test_vinal3_meaning);

    /* Vinal 4 — Zotz */
    RUN_TEST(test_vinal4_name);
    RUN_TEST(test_vinal4_meaning);

    /* Vinal 5 — Tzec */
    RUN_TEST(test_vinal5_name);
    RUN_TEST(test_vinal5_meaning);

    /* Vinal 6 — Xul */
    RUN_TEST(test_vinal6_name);
    RUN_TEST(test_vinal6_meaning);

    /* Vinal 7 — Yaxkin */
    RUN_TEST(test_vinal7_name);
    RUN_TEST(test_vinal7_meaning);

    /* Vinal 8 — Mol */
    RUN_TEST(test_vinal8_name);
    RUN_TEST(test_vinal8_meaning);

    /* Vinal 9 — Chen */
    RUN_TEST(test_vinal9_name);
    RUN_TEST(test_vinal9_meaning);

    /* Vinal 10 — Yax */
    RUN_TEST(test_vinal10_name);
    RUN_TEST(test_vinal10_meaning);

    /* Vinal 11 — Sac */
    RUN_TEST(test_vinal11_name);
    RUN_TEST(test_vinal11_meaning);

    /* Vinal 12 — Ceh */
    RUN_TEST(test_vinal12_name);
    RUN_TEST(test_vinal12_meaning);

    /* Vinal 13 — Mac */
    RUN_TEST(test_vinal13_name);
    RUN_TEST(test_vinal13_meaning);

    /* Vinal 14 — Kankin */
    RUN_TEST(test_vinal14_name);
    RUN_TEST(test_vinal14_meaning);

    /* Vinal 15 — Moan */
    RUN_TEST(test_vinal15_name);
    RUN_TEST(test_vinal15_meaning);

    /* Vinal 16 — Pax */
    RUN_TEST(test_vinal16_name);
    RUN_TEST(test_vinal16_meaning);

    /* Vinal 17 — Kayab */
    RUN_TEST(test_vinal17_name);
    RUN_TEST(test_vinal17_meaning);

    /* Vinal 18 — Cumhu */
    RUN_TEST(test_vinal18_name);
    RUN_TEST(test_vinal18_meaning);

    /* Vinal 19 — Uayeb */
    RUN_TEST(test_vinal19_name);
    RUN_TEST(test_vinal19_meaning);
    RUN_TEST(test_vinal19_struct);
    RUN_TEST(test_uayeb_is_uayeb);
    RUN_TEST(test_uayeb_days_is_5);

    /* Regular vinales */
    RUN_TEST(test_regular_vinales_not_uayeb);
    RUN_TEST(test_regular_vinales_have_20_days);

    /* Sweep all vinales */
    RUN_TEST(test_all_vinales_have_name);
    RUN_TEST(test_all_vinales_have_meaning);
    RUN_TEST(test_all_vinales_correct_number);
    RUN_TEST(test_all_names_unique);

    /* vinal_for_day_of_year — specific days */
    RUN_TEST(test_day0_is_vinal1);
    RUN_TEST(test_day19_is_vinal1);
    RUN_TEST(test_day20_is_vinal2);
    RUN_TEST(test_day39_is_vinal2);
    RUN_TEST(test_day40_is_vinal3);
    RUN_TEST(test_day100_is_vinal6);
    RUN_TEST(test_day199_is_vinal10);
    RUN_TEST(test_day339_is_vinal17);
    RUN_TEST(test_day340_is_vinal18);
    RUN_TEST(test_day359_is_vinal18);
    RUN_TEST(test_day360_is_uayeb);
    RUN_TEST(test_day361_is_uayeb);
    RUN_TEST(test_day362_is_uayeb);
    RUN_TEST(test_day363_is_uayeb);
    RUN_TEST(test_day364_is_uayeb);

    /* vinal_for_day_of_year — boundaries */
    RUN_TEST(test_vinal_boundaries_first_day);
    RUN_TEST(test_vinal_boundaries_last_day);

    /* Invalid inputs — get */
    RUN_TEST(test_invalid_num_0_get);
    RUN_TEST(test_invalid_num_20_get);
    RUN_TEST(test_invalid_num_neg1_get);
    RUN_TEST(test_invalid_num_100_get);

    /* Invalid inputs — name/meaning */
    RUN_TEST(test_invalid_num_0_name);
    RUN_TEST(test_invalid_num_20_name);
    RUN_TEST(test_invalid_num_neg1_name);
    RUN_TEST(test_invalid_num_0_meaning);
    RUN_TEST(test_invalid_num_20_meaning);

    /* Invalid inputs — day_of_year */
    RUN_TEST(test_invalid_day_neg1);
    RUN_TEST(test_invalid_day_365);
    RUN_TEST(test_invalid_day_1000);
    RUN_TEST(test_invalid_day_neg100);

    /* is_uayeb boundary */
    RUN_TEST(test_is_uayeb_0);
    RUN_TEST(test_is_uayeb_neg1);
    RUN_TEST(test_is_uayeb_20);
    RUN_TEST(test_is_uayeb_18);

    /* Purity */
    RUN_TEST(test_purity_get);
    RUN_TEST(test_purity_name);
    RUN_TEST(test_purity_for_day);

    return UNITY_END();
}
