/* test_climate_history.c — Tests for climate history data module.
 * TDD RED phase: all tests written before implementation. */

#include "../../unity/unity.h"
#include "../../../src/systems/geology/climate_history.h"

#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* --- Event count --- */

void test_climate_event_count_at_least_15(void) {
    TEST_ASSERT_GREATER_OR_EQUAL(15, climate_event_count());
}

/* --- Event lookup by index --- */

void test_climate_event_get_index_0_hadean(void) {
    climate_event_t e = climate_event_get(0);
    TEST_ASSERT_EQUAL_INT(0, e.id);
    TEST_ASSERT_EQUAL_STRING("Hadean Bombardment Warming", e.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 4500.0f, (float)e.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 3800.0f, (float)e.end_ma);
    TEST_ASSERT_EQUAL_INT(CLIMATE_TYPE_WARMING, e.type);
    TEST_ASSERT_EQUAL_INT(CLIMATE_SEVERITY_MAJOR, e.severity);
}

void test_climate_event_get_goe(void) {
    climate_event_t e = climate_event_get(1);
    TEST_ASSERT_EQUAL_STRING("Great Oxidation Event", e.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 2400.0f, (float)e.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 2000.0f, (float)e.end_ma);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, -10.0f, (float)e.temp_anomaly_c);
    TEST_ASSERT_FLOAT_WITHIN(10.0f, 200.0f, (float)e.co2_ppm);
    TEST_ASSERT_EQUAL_INT(CLIMATE_TYPE_ATMOSPHERIC, e.type);
    TEST_ASSERT_EQUAL_INT(CLIMATE_SEVERITY_CATASTROPHIC, e.severity);
}

void test_climate_event_get_sturtian(void) {
    climate_event_t e = climate_event_get(4);
    TEST_ASSERT_EQUAL_STRING("Sturtian Snowball Earth", e.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 717.0f, (float)e.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 660.0f, (float)e.end_ma);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, -50.0f, (float)e.temp_anomaly_c);
    TEST_ASSERT_EQUAL_INT(CLIMATE_TYPE_GLACIATION, e.type);
    TEST_ASSERT_EQUAL_INT(CLIMATE_SEVERITY_CATASTROPHIC, e.severity);
}

void test_climate_event_get_petm(void) {
    climate_event_t e = climate_event_get(10);
    TEST_ASSERT_EQUAL_STRING("PETM (Paleocene-Eocene)", e.name);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 56.0f, (float)e.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 54.0f, (float)e.end_ma);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 6.0f, (float)e.temp_anomaly_c);
    TEST_ASSERT_FLOAT_WITHIN(50.0f, 1000.0f, (float)e.co2_ppm);
    TEST_ASSERT_EQUAL_INT(CLIMATE_TYPE_WARMING, e.type);
}

void test_climate_event_get_little_ice_age(void) {
    climate_event_t e = climate_event_get(14);
    TEST_ASSERT_EQUAL_STRING("Little Ice Age", e.name);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.00065f, (float)e.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.00015f, (float)e.end_ma);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, -1.0f, (float)e.temp_anomaly_c);
    TEST_ASSERT_EQUAL_INT(CLIMATE_TYPE_GLACIATION, e.type);
    TEST_ASSERT_EQUAL_INT(CLIMATE_SEVERITY_MINOR, e.severity);
}

void test_climate_event_get_invalid_negative(void) {
    climate_event_t e = climate_event_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, e.id);
    TEST_ASSERT_NULL(e.name);
}

void test_climate_event_get_invalid_too_large(void) {
    climate_event_t e = climate_event_get(999);
    TEST_ASSERT_EQUAL_INT(-1, e.id);
    TEST_ASSERT_NULL(e.name);
}

/* --- Event name by index --- */

void test_climate_event_name_valid(void) {
    TEST_ASSERT_EQUAL_STRING("Hadean Bombardment Warming",
                             climate_event_name(0));
}

void test_climate_event_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", climate_event_name(-1));
    TEST_ASSERT_EQUAL_STRING("?", climate_event_name(999));
}

/* --- Event at time (Ma) --- */

void test_climate_event_at_4000_returns_hadean(void) {
    /* 4500-3800 Ma range */
    climate_event_t e = climate_event_at(4000.0);
    TEST_ASSERT_EQUAL_STRING("Hadean Bombardment Warming", e.name);
}

void test_climate_event_at_700_returns_sturtian(void) {
    /* Sturtian: 717-660 Ma. 700 is inside. */
    climate_event_t e = climate_event_at(700.0);
    TEST_ASSERT_EQUAL_STRING("Sturtian Snowball Earth", e.name);
}

void test_climate_event_at_2200_returns_most_severe(void) {
    /* Both GOE (2400-2000, CATASTROPHIC) and Huronian (2400-2100, MAJOR)
     * overlap at 2200. Should return GOE (most severe). */
    climate_event_t e = climate_event_at(2200.0);
    TEST_ASSERT_EQUAL_STRING("Great Oxidation Event", e.name);
}

void test_climate_event_at_no_match(void) {
    /* 3500 Ma: between Hadean warming (ends 3800) and GOE (starts 2400) */
    climate_event_t e = climate_event_at(3500.0);
    TEST_ASSERT_EQUAL_INT(-1, e.id);
    TEST_ASSERT_NULL(e.name);
}

void test_climate_event_at_boundary_start(void) {
    /* start_ma is inclusive: 717 should match Sturtian */
    climate_event_t e = climate_event_at(717.0);
    TEST_ASSERT_EQUAL_STRING("Sturtian Snowball Earth", e.name);
}

void test_climate_event_at_boundary_end(void) {
    /* end_ma is inclusive: 660 should match Sturtian */
    climate_event_t e = climate_event_at(660.0);
    TEST_ASSERT_EQUAL_STRING("Sturtian Snowball Earth", e.name);
}

/* --- Severity and type name strings --- */

void test_climate_severity_name_minor(void) {
    TEST_ASSERT_EQUAL_STRING("Minor", climate_severity_name(CLIMATE_SEVERITY_MINOR));
}

void test_climate_severity_name_catastrophic(void) {
    TEST_ASSERT_EQUAL_STRING("Catastrophic",
                             climate_severity_name(CLIMATE_SEVERITY_CATASTROPHIC));
}

void test_climate_type_name_glaciation(void) {
    TEST_ASSERT_EQUAL_STRING("Glaciation",
                             climate_type_name(CLIMATE_TYPE_GLACIATION));
}

void test_climate_type_name_impact(void) {
    TEST_ASSERT_EQUAL_STRING("Impact",
                             climate_type_name(CLIMATE_TYPE_IMPACT));
}

/* --- Events of type / severity --- */

void test_climate_events_of_type_glaciation(void) {
    /* Huronian, Sturtian, Marinoan, Late Ordovician, Carboniferous,
     * Pleistocene (OSCILLATION actually), Little Ice Age = at least 5 GLACIATION */
    int count = climate_events_of_type(CLIMATE_TYPE_GLACIATION);
    TEST_ASSERT_GREATER_OR_EQUAL(5, count);
}

void test_climate_events_of_type_warming(void) {
    /* Hadean, Cambrian, PT, PETM, Mid-Miocene, Holocene = at least 5 */
    int count = climate_events_of_type(CLIMATE_TYPE_WARMING);
    TEST_ASSERT_GREATER_OR_EQUAL(5, count);
}

void test_climate_events_of_severity_catastrophic(void) {
    /* GOE, Sturtian, Marinoan, PT = at least 4 */
    int count = climate_events_of_severity(CLIMATE_SEVERITY_CATASTROPHIC);
    TEST_ASSERT_GREATER_OR_EQUAL(4, count);
}

/* --- Range query --- */

void test_climate_events_in_range_snowball_era(void) {
    /* Range 720 to 630 Ma: should include Sturtian (717-660) and
     * Marinoan (650-635). */
    int indices[16];
    int found = climate_events_in_range(720.0, 630.0, indices, 16);
    TEST_ASSERT_GREATER_OR_EQUAL(2, found);

    /* Verify both snowball events are in results */
    int has_sturtian = 0, has_marinoan = 0;
    for (int i = 0; i < found; i++) {
        climate_event_t e = climate_event_get(indices[i]);
        if (strcmp(e.name, "Sturtian Snowball Earth") == 0) has_sturtian = 1;
        if (strcmp(e.name, "Marinoan Snowball Earth") == 0) has_marinoan = 1;
    }
    TEST_ASSERT_TRUE(has_sturtian);
    TEST_ASSERT_TRUE(has_marinoan);
}

void test_climate_events_in_range_no_events(void) {
    /* Range 3700-2500 Ma: gap between Hadean warming and GOE */
    int indices[16];
    int found = climate_events_in_range(3700.0, 2500.0, indices, 16);
    TEST_ASSERT_EQUAL_INT(0, found);
}

void test_climate_events_in_range_max_results_cap(void) {
    /* Very wide range, but capped at 2 results */
    int indices[2];
    int found = climate_events_in_range(5000.0, 0.0, indices, 2);
    TEST_ASSERT_EQUAL_INT(2, found);
}

void test_climate_events_in_range_partial_overlap(void) {
    /* Range 2300-2050 Ma: overlaps with GOE (2400-2000) and
     * Huronian (2400-2100), even though range doesn't fully contain them. */
    int indices[16];
    int found = climate_events_in_range(2300.0, 2050.0, indices, 16);
    TEST_ASSERT_GREATER_OR_EQUAL(2, found);
}

/* --- Glaciation check --- */

void test_climate_is_glaciated_sturtian(void) {
    /* 700 Ma is within Sturtian (717-660 Ma) */
    TEST_ASSERT_TRUE(climate_is_glaciated(700.0));
}

void test_climate_is_glaciated_false_at_100(void) {
    /* 100 Ma: no glaciation active */
    TEST_ASSERT_FALSE(climate_is_glaciated(100.0));
}

void test_climate_is_glaciated_little_ice_age(void) {
    /* 0.0004 Ma (~400 years ago) within Little Ice Age (0.00065-0.00015) */
    TEST_ASSERT_TRUE(climate_is_glaciated(0.0004));
}

void test_climate_is_glaciated_not_for_warming(void) {
    /* 4000 Ma is Hadean warming, not glaciation */
    TEST_ASSERT_FALSE(climate_is_glaciated(4000.0));
}

/* --- Holocene Optimum data check --- */

void test_climate_holocene_optimum(void) {
    climate_event_t e = climate_event_get(13);
    TEST_ASSERT_EQUAL_STRING("Holocene Optimum", e.name);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.008f, (float)e.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.005f, (float)e.end_ma);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 1.0f, (float)e.temp_anomaly_c);
    TEST_ASSERT_EQUAL_INT(CLIMATE_TYPE_WARMING, e.type);
    TEST_ASSERT_EQUAL_INT(CLIMATE_SEVERITY_MINOR, e.severity);
}

/* --- Permian-Triassic data check --- */

void test_climate_permian_triassic(void) {
    climate_event_t e = climate_event_get(9);
    TEST_ASSERT_EQUAL_STRING("Permian-Triassic Extinction", e.name);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 252.0f, (float)e.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 251.0f, (float)e.end_ma);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 10.0f, (float)e.temp_anomaly_c);
    TEST_ASSERT_EQUAL_INT(CLIMATE_TYPE_WARMING, e.type);
    TEST_ASSERT_EQUAL_INT(CLIMATE_SEVERITY_CATASTROPHIC, e.severity);
}

/* --- Pleistocene Ice Ages (OSCILLATION type) --- */

void test_climate_pleistocene_type_oscillation(void) {
    climate_event_t e = climate_event_get(12);
    TEST_ASSERT_EQUAL_STRING("Pleistocene Ice Ages", e.name);
    TEST_ASSERT_EQUAL_INT(CLIMATE_TYPE_OSCILLATION, e.type);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2.58f, (float)e.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.012f, (float)e.end_ma);
}

/* --- main --- */

int main(void) {
    UNITY_BEGIN();

    /* Event count */
    RUN_TEST(test_climate_event_count_at_least_15);

    /* Event lookup by index */
    RUN_TEST(test_climate_event_get_index_0_hadean);
    RUN_TEST(test_climate_event_get_goe);
    RUN_TEST(test_climate_event_get_sturtian);
    RUN_TEST(test_climate_event_get_petm);
    RUN_TEST(test_climate_event_get_little_ice_age);
    RUN_TEST(test_climate_event_get_invalid_negative);
    RUN_TEST(test_climate_event_get_invalid_too_large);

    /* Event name */
    RUN_TEST(test_climate_event_name_valid);
    RUN_TEST(test_climate_event_name_invalid);

    /* Event at time */
    RUN_TEST(test_climate_event_at_4000_returns_hadean);
    RUN_TEST(test_climate_event_at_700_returns_sturtian);
    RUN_TEST(test_climate_event_at_2200_returns_most_severe);
    RUN_TEST(test_climate_event_at_no_match);
    RUN_TEST(test_climate_event_at_boundary_start);
    RUN_TEST(test_climate_event_at_boundary_end);

    /* Severity and type names */
    RUN_TEST(test_climate_severity_name_minor);
    RUN_TEST(test_climate_severity_name_catastrophic);
    RUN_TEST(test_climate_type_name_glaciation);
    RUN_TEST(test_climate_type_name_impact);

    /* Events of type / severity */
    RUN_TEST(test_climate_events_of_type_glaciation);
    RUN_TEST(test_climate_events_of_type_warming);
    RUN_TEST(test_climate_events_of_severity_catastrophic);

    /* Range query */
    RUN_TEST(test_climate_events_in_range_snowball_era);
    RUN_TEST(test_climate_events_in_range_no_events);
    RUN_TEST(test_climate_events_in_range_max_results_cap);
    RUN_TEST(test_climate_events_in_range_partial_overlap);

    /* Glaciation check */
    RUN_TEST(test_climate_is_glaciated_sturtian);
    RUN_TEST(test_climate_is_glaciated_false_at_100);
    RUN_TEST(test_climate_is_glaciated_little_ice_age);
    RUN_TEST(test_climate_is_glaciated_not_for_warming);

    /* Specific event data verification */
    RUN_TEST(test_climate_holocene_optimum);
    RUN_TEST(test_climate_permian_triassic);
    RUN_TEST(test_climate_pleistocene_type_oscillation);

    return UNITY_END();
}
