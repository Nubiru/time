/* test_panchanga.c -- Panchanga (five-limbed Hindu calendar) tests
 * TDD: tests match the API defined in task.md (Track 17.1).
 * Covers: tithi, vara, yoga, karana, panchanga_compute,
 *         name lookups, count functions, edge cases, purity. */

#include "../../unity/unity.h"
#include "../../../src/systems/hindu/panchanga.h"
#include "../../../src/systems/hindu/nakshatra.h"
#include <math.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== Tithi: basic elongation -> tithi mapping ===== */

void test_tithi_new_moon(void)
{
    /* Elongation 0 -> Tithi 30 (Amavasya) since floor(0/12)+1 = 1,
     * but elongation 0 means new moon which is Amavasya (tithi 30).
     * Actually: floor(0/12)+1 = 1 = Shukla Pratipada at exact 0.
     * At elongation ~0 the new moon just passed.
     * Elongation 354 -> floor(354/12)+1 = 30 = Amavasya. */
    tithi_t t = panchanga_tithi(354.0);
    TEST_ASSERT_EQUAL_INT(30, t.number);
    TEST_ASSERT_EQUAL_STRING("Krishna", t.paksha);
    TEST_ASSERT_EQUAL_STRING("Amavasya", t.name);
    TEST_ASSERT_EQUAL_INT(15, t.paksha_day);
}

void test_tithi_shukla_pratipada(void)
{
    /* Elongation 6 -> floor(6/12)+1 = 1 = Shukla Pratipada */
    tithi_t t = panchanga_tithi(6.0);
    TEST_ASSERT_EQUAL_INT(1, t.number);
    TEST_ASSERT_EQUAL_STRING("Shukla", t.paksha);
    TEST_ASSERT_EQUAL_STRING("Pratipada", t.name);
    TEST_ASSERT_EQUAL_INT(1, t.paksha_day);
}

void test_tithi_purnima(void)
{
    /* Elongation 174 -> floor(174/12)+1 = 15 = Purnima */
    tithi_t t = panchanga_tithi(174.0);
    TEST_ASSERT_EQUAL_INT(15, t.number);
    TEST_ASSERT_EQUAL_STRING("Shukla", t.paksha);
    TEST_ASSERT_EQUAL_STRING("Purnima", t.name);
    TEST_ASSERT_EQUAL_INT(15, t.paksha_day);
}

void test_tithi_krishna_pratipada(void)
{
    /* Elongation 186 -> floor(186/12)+1 = 16 = Krishna Pratipada */
    tithi_t t = panchanga_tithi(186.0);
    TEST_ASSERT_EQUAL_INT(16, t.number);
    TEST_ASSERT_EQUAL_STRING("Krishna", t.paksha);
    TEST_ASSERT_EQUAL_STRING("Pratipada", t.name);
    TEST_ASSERT_EQUAL_INT(1, t.paksha_day);
}

void test_tithi_shukla_ashtami(void)
{
    /* Elongation 90 -> floor(90/12)+1 = 8 = Shukla Ashtami */
    tithi_t t = panchanga_tithi(90.0);
    TEST_ASSERT_EQUAL_INT(8, t.number);
    TEST_ASSERT_EQUAL_STRING("Shukla", t.paksha);
    TEST_ASSERT_EQUAL_STRING("Ashtami", t.name);
    TEST_ASSERT_EQUAL_INT(8, t.paksha_day);
}

void test_tithi_krishna_ekadashi(void)
{
    /* Elongation 300 -> floor(300/12)+1 = 26 = Krishna Ekadashi */
    tithi_t t = panchanga_tithi(300.0);
    TEST_ASSERT_EQUAL_INT(26, t.number);
    TEST_ASSERT_EQUAL_STRING("Krishna", t.paksha);
    TEST_ASSERT_EQUAL_STRING("Ekadashi", t.name);
    TEST_ASSERT_EQUAL_INT(11, t.paksha_day);
}

void test_tithi_count(void)
{
    TEST_ASSERT_EQUAL_INT(30, panchanga_tithi_count());
}

void test_tithi_name_lookup(void)
{
    TEST_ASSERT_EQUAL_STRING("Pratipada", panchanga_tithi_name(1));
    TEST_ASSERT_EQUAL_STRING("Purnima", panchanga_tithi_name(15));
    TEST_ASSERT_EQUAL_STRING("Amavasya", panchanga_tithi_name(30));
    TEST_ASSERT_NULL(panchanga_tithi_name(0));
    TEST_ASSERT_NULL(panchanga_tithi_name(31));
}

/* ===== Vara: weekday from JD ===== */

void test_vara_sunday(void)
{
    /* JD 2451545.0 = 2000-01-01 12:00 = Saturday
     * JD 2451547.0 = 2000-01-03 = Monday
     * JD 2451546.0 = 2000-01-02 = Sunday */
    vara_t v = panchanga_vara(2451546.0);
    TEST_ASSERT_EQUAL_INT(0, v.number);
    TEST_ASSERT_EQUAL_STRING("Ravivara", v.name);
    TEST_ASSERT_EQUAL_STRING("Surya", v.deity);
    TEST_ASSERT_EQUAL_STRING("Sun", v.planet);
}

void test_vara_saturday(void)
{
    /* JD 2451545.0 = 2000-01-01 = Saturday */
    vara_t v = panchanga_vara(2451545.0);
    TEST_ASSERT_EQUAL_INT(6, v.number);
    TEST_ASSERT_EQUAL_STRING("Shanivara", v.name);
    TEST_ASSERT_EQUAL_STRING("Shani", v.deity);
    TEST_ASSERT_EQUAL_STRING("Saturn", v.planet);
}

void test_vara_monday(void)
{
    /* JD 2451547.0 = 2000-01-03 = Monday */
    vara_t v = panchanga_vara(2451547.0);
    TEST_ASSERT_EQUAL_INT(1, v.number);
    TEST_ASSERT_EQUAL_STRING("Somavara", v.name);
    TEST_ASSERT_EQUAL_STRING("Chandra", v.deity);
    TEST_ASSERT_EQUAL_STRING("Moon", v.planet);
}

void test_vara_count(void)
{
    TEST_ASSERT_EQUAL_INT(7, panchanga_vara_count());
}

void test_vara_name_lookup(void)
{
    TEST_ASSERT_EQUAL_STRING("Ravivara", panchanga_vara_name(0));
    TEST_ASSERT_EQUAL_STRING("Shanivara", panchanga_vara_name(6));
    TEST_ASSERT_NULL(panchanga_vara_name(-1));
    TEST_ASSERT_NULL(panchanga_vara_name(7));
}

/* ===== Yoga: sun_sidereal + moon_sidereal ===== */

void test_yoga_first(void)
{
    /* Sum = 0 -> yoga index 0 = Vishkambha */
    yoga_t y = panchanga_yoga(0.0, 0.0);
    TEST_ASSERT_EQUAL_INT(0, y.number);
    TEST_ASSERT_EQUAL_STRING("Vishkambha", y.name);
    TEST_ASSERT_EQUAL_STRING("Inauspicious", y.quality);
}

void test_yoga_priti(void)
{
    /* Sum = 14.0 -> 14/(360/27) = 14/13.333 = 1.05 -> index 1 = Priti */
    yoga_t y = panchanga_yoga(7.0, 7.0);
    TEST_ASSERT_EQUAL_INT(1, y.number);
    TEST_ASSERT_EQUAL_STRING("Priti", y.name);
    TEST_ASSERT_EQUAL_STRING("Auspicious", y.quality);
}

void test_yoga_last(void)
{
    /* Sum = 350 -> 350/13.333 = 26.25 -> index 26 = Vaidhriti */
    yoga_t y = panchanga_yoga(175.0, 175.0);
    TEST_ASSERT_EQUAL_INT(26, y.number);
    TEST_ASSERT_EQUAL_STRING("Vaidhriti", y.name);
    TEST_ASSERT_EQUAL_STRING("Inauspicious", y.quality);
}

void test_yoga_wrapping(void)
{
    /* Sum = 200 + 200 = 400, mod 360 = 40 -> 40/13.333 = 3.0 -> index 3 = Saubhagya */
    yoga_t y = panchanga_yoga(200.0, 200.0);
    TEST_ASSERT_EQUAL_INT(3, y.number);
    TEST_ASSERT_EQUAL_STRING("Saubhagya", y.name);
    TEST_ASSERT_EQUAL_STRING("Auspicious", y.quality);
}

void test_yoga_count(void)
{
    TEST_ASSERT_EQUAL_INT(27, panchanga_yoga_count());
}

void test_yoga_name_lookup(void)
{
    TEST_ASSERT_EQUAL_STRING("Vishkambha", panchanga_yoga_name(0));
    TEST_ASSERT_EQUAL_STRING("Vaidhriti", panchanga_yoga_name(26));
    TEST_ASSERT_NULL(panchanga_yoga_name(-1));
    TEST_ASSERT_NULL(panchanga_yoga_name(27));
}

void test_yoga_variyan_neutral(void)
{
    /* Variyan is index 17 -> quality "Neutral" */
    /* Sum needs to be in range [17*13.333, 18*13.333) = [226.667, 240.0) */
    yoga_t y = panchanga_yoga(115.0, 115.0);
    TEST_ASSERT_EQUAL_INT(17, y.number);
    TEST_ASSERT_EQUAL_STRING("Variyan", y.name);
    TEST_ASSERT_EQUAL_STRING("Neutral", y.quality);
}

/* ===== Karana: half-tithi ===== */

void test_karana_kimstughna(void)
{
    /* Position 0: elongation in [0,6) -> Kimstughna (fixed) */
    karana_t k = panchanga_karana(3.0);
    TEST_ASSERT_EQUAL_INT(0, k.number);
    TEST_ASSERT_EQUAL_STRING("Kimstughna", k.name);
    TEST_ASSERT_TRUE(k.is_fixed);
}

void test_karana_bava(void)
{
    /* Position 1: elongation in [6,12) -> Bava (moving, first of 7-cycle) */
    karana_t k = panchanga_karana(8.0);
    TEST_ASSERT_EQUAL_INT(1, k.number);
    TEST_ASSERT_EQUAL_STRING("Bava", k.name);
    TEST_ASSERT_FALSE(k.is_fixed);
}

void test_karana_vishti(void)
{
    /* Position 7: elongation in [42,48) -> Vishti (moving, 7th)
     * (pos-1) % 7 = 6 -> Vishti (index 7 in full list) */
    karana_t k = panchanga_karana(45.0);
    TEST_ASSERT_EQUAL_INT(7, k.number);
    TEST_ASSERT_EQUAL_STRING("Vishti", k.name);
    TEST_ASSERT_FALSE(k.is_fixed);
}

void test_karana_shakuni(void)
{
    /* Position 57: elongation in [342,348) -> Shakuni (fixed) */
    karana_t k = panchanga_karana(345.0);
    TEST_ASSERT_EQUAL_INT(8, k.number);
    TEST_ASSERT_EQUAL_STRING("Shakuni", k.name);
    TEST_ASSERT_TRUE(k.is_fixed);
}

void test_karana_chatushpada(void)
{
    /* Position 58: elongation in [348,354) -> Chatushpada (fixed) */
    karana_t k = panchanga_karana(350.0);
    TEST_ASSERT_EQUAL_INT(9, k.number);
    TEST_ASSERT_EQUAL_STRING("Chatushpada", k.name);
    TEST_ASSERT_TRUE(k.is_fixed);
}

void test_karana_naga(void)
{
    /* Position 59: elongation in [354,360) -> Naga (fixed) */
    karana_t k = panchanga_karana(357.0);
    TEST_ASSERT_EQUAL_INT(10, k.number);
    TEST_ASSERT_EQUAL_STRING("Naga", k.name);
    TEST_ASSERT_TRUE(k.is_fixed);
}

void test_karana_moving_cycle(void)
{
    /* Position 8: elongation in [48,54) -> (8-1)%7 = 0 -> Bava again */
    karana_t k = panchanga_karana(50.0);
    TEST_ASSERT_EQUAL_INT(1, k.number);
    TEST_ASSERT_EQUAL_STRING("Bava", k.name);
}

void test_karana_count(void)
{
    TEST_ASSERT_EQUAL_INT(11, panchanga_karana_count());
}

void test_karana_name_lookup(void)
{
    TEST_ASSERT_EQUAL_STRING("Kimstughna", panchanga_karana_name(0));
    TEST_ASSERT_EQUAL_STRING("Bava", panchanga_karana_name(1));
    TEST_ASSERT_EQUAL_STRING("Naga", panchanga_karana_name(10));
    TEST_ASSERT_NULL(panchanga_karana_name(-1));
    TEST_ASSERT_NULL(panchanga_karana_name(11));
}

/* ===== panchanga_compute: integration ===== */

void test_compute_known_date(void)
{
    /* JD 2451545.0 = 2000-01-01 12:00 TT (Saturday)
     * Use approximate longitudes: Sun ~280.5, Moon ~30.0 (hypothetical)
     * Elongation = 30 - 280.5 = -250.5 + 360 = 109.5
     * Tithi = floor(109.5/12)+1 = 10 = Shukla Dashami */
    double jd = 2451545.0;
    double sun_lon = 280.5;
    double moon_lon = 30.0;

    panchanga_t p = panchanga_compute(jd, sun_lon, moon_lon);

    /* Vara: Saturday = 6 */
    TEST_ASSERT_EQUAL_INT(6, p.vara.number);
    TEST_ASSERT_EQUAL_STRING("Shanivara", p.vara.name);

    /* Tithi: elongation 109.5 -> floor(109.5/12)+1 = 10 */
    TEST_ASSERT_EQUAL_INT(10, p.tithi.number);
    TEST_ASSERT_EQUAL_STRING("Shukla", p.tithi.paksha);

    /* Nakshatra index: computed from moon_sidereal */
    TEST_ASSERT_TRUE(p.nakshatra_index >= 0 && p.nakshatra_index < 27);

    /* Yoga and karana should have valid data */
    TEST_ASSERT_NOT_NULL(p.yoga.name);
    TEST_ASSERT_NOT_NULL(p.karana.name);
}

void test_compute_full_moon(void)
{
    /* Elongation = 180 = full moon -> Tithi 15 (Purnima) */
    double jd = 2451546.0; /* Sunday */
    double sun_lon = 0.0;
    double moon_lon = 180.0;

    panchanga_t p = panchanga_compute(jd, sun_lon, moon_lon);

    TEST_ASSERT_EQUAL_INT(0, p.vara.number); /* Sunday */
    TEST_ASSERT_EQUAL_INT(16, p.tithi.number); /* floor(180/12)+1=16 */
    TEST_ASSERT_EQUAL_STRING("Krishna", p.tithi.paksha);
}

void test_compute_new_moon(void)
{
    /* Elongation ~0 -> Tithi 1 (Shukla Pratipada at exact 0) */
    double jd = 2451547.0; /* Monday */
    double sun_lon = 100.0;
    double moon_lon = 100.0;

    panchanga_t p = panchanga_compute(jd, sun_lon, moon_lon);

    TEST_ASSERT_EQUAL_INT(1, p.vara.number); /* Monday */
    TEST_ASSERT_EQUAL_INT(1, p.tithi.number); /* floor(0/12)+1=1 */
    TEST_ASSERT_EQUAL_STRING("Shukla", p.tithi.paksha);
}

/* ===== Edge cases ===== */

void test_tithi_exact_boundary(void)
{
    /* Elongation exactly 12.0 -> floor(12/12)+1 = 2 = Dvitiya */
    tithi_t t = panchanga_tithi(12.0);
    TEST_ASSERT_EQUAL_INT(2, t.number);
    TEST_ASSERT_EQUAL_STRING("Dvitiya", t.name);
}

void test_tithi_exact_180(void)
{
    /* Elongation exactly 180.0 -> floor(180/12)+1 = 16 = Krishna Pratipada */
    tithi_t t = panchanga_tithi(180.0);
    TEST_ASSERT_EQUAL_INT(16, t.number);
    TEST_ASSERT_EQUAL_STRING("Krishna", t.paksha);
    TEST_ASSERT_EQUAL_STRING("Pratipada", t.name);
}

void test_karana_elongation_zero(void)
{
    /* Elongation 0 -> position 0 -> Kimstughna */
    karana_t k = panchanga_karana(0.0);
    TEST_ASSERT_EQUAL_INT(0, k.number);
    TEST_ASSERT_EQUAL_STRING("Kimstughna", k.name);
}

void test_yoga_sum_zero(void)
{
    /* Sun and moon both at 0 sidereal -> sum 0 -> index 0 = Vishkambha */
    yoga_t y = panchanga_yoga(0.0, 0.0);
    TEST_ASSERT_EQUAL_INT(0, y.number);
}

/* ===== All 30 tithi names are non-null ===== */

void test_all_tithi_names_valid(void)
{
    for (int i = 1; i <= 30; i++) {
        const char *name = panchanga_tithi_name(i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(strlen(name) > 0);
    }
}

/* ===== All 27 yoga names are non-null ===== */

void test_all_yoga_names_valid(void)
{
    for (int i = 0; i < 27; i++) {
        const char *name = panchanga_yoga_name(i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(strlen(name) > 0);
    }
}

/* ===== All 7 vara names are non-null ===== */

void test_all_vara_names_valid(void)
{
    for (int i = 0; i < 7; i++) {
        const char *name = panchanga_vara_name(i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(strlen(name) > 0);
    }
}

/* ===== All 11 karana names are non-null ===== */

void test_all_karana_names_valid(void)
{
    for (int i = 0; i < 11; i++) {
        const char *name = panchanga_karana_name(i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(strlen(name) > 0);
    }
}

/* ===== Vara cycles correctly over 7 days ===== */

void test_vara_seven_day_cycle(void)
{
    /* Starting from JD 2451545.0 (Saturday=6), check a full week */
    double base_jd = 2451545.0;
    int expected[] = {6, 0, 1, 2, 3, 4, 5};
    for (int i = 0; i < 7; i++) {
        vara_t v = panchanga_vara(base_jd + (double)i);
        TEST_ASSERT_EQUAL_INT(expected[i], v.number);
    }
}

/* ===== Runner ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Tithi */
    RUN_TEST(test_tithi_new_moon);
    RUN_TEST(test_tithi_shukla_pratipada);
    RUN_TEST(test_tithi_purnima);
    RUN_TEST(test_tithi_krishna_pratipada);
    RUN_TEST(test_tithi_shukla_ashtami);
    RUN_TEST(test_tithi_krishna_ekadashi);
    RUN_TEST(test_tithi_count);
    RUN_TEST(test_tithi_name_lookup);
    RUN_TEST(test_tithi_exact_boundary);
    RUN_TEST(test_tithi_exact_180);

    /* Vara */
    RUN_TEST(test_vara_sunday);
    RUN_TEST(test_vara_saturday);
    RUN_TEST(test_vara_monday);
    RUN_TEST(test_vara_count);
    RUN_TEST(test_vara_name_lookup);
    RUN_TEST(test_vara_seven_day_cycle);
    RUN_TEST(test_all_vara_names_valid);

    /* Yoga */
    RUN_TEST(test_yoga_first);
    RUN_TEST(test_yoga_priti);
    RUN_TEST(test_yoga_last);
    RUN_TEST(test_yoga_wrapping);
    RUN_TEST(test_yoga_count);
    RUN_TEST(test_yoga_name_lookup);
    RUN_TEST(test_yoga_variyan_neutral);
    RUN_TEST(test_yoga_sum_zero);

    /* Karana */
    RUN_TEST(test_karana_kimstughna);
    RUN_TEST(test_karana_bava);
    RUN_TEST(test_karana_vishti);
    RUN_TEST(test_karana_shakuni);
    RUN_TEST(test_karana_chatushpada);
    RUN_TEST(test_karana_naga);
    RUN_TEST(test_karana_moving_cycle);
    RUN_TEST(test_karana_count);
    RUN_TEST(test_karana_name_lookup);

    /* panchanga_compute integration */
    RUN_TEST(test_compute_known_date);
    RUN_TEST(test_compute_full_moon);
    RUN_TEST(test_compute_new_moon);

    /* Edge cases */
    RUN_TEST(test_karana_elongation_zero);

    /* Completeness */
    RUN_TEST(test_all_tithi_names_valid);
    RUN_TEST(test_all_yoga_names_valid);
    RUN_TEST(test_all_vara_names_valid);
    RUN_TEST(test_all_karana_names_valid);

    return UNITY_END();
}
