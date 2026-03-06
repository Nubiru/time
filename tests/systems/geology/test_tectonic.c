/* test_tectonic.c — Tests for tectonic event timeline data module.
 * TDD RED phase: all tests written before implementation. */

#include "../../unity/unity.h"
#include "../../../src/systems/geology/tectonic.h"

#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* --- Supercontinent count --- */

void test_supercontinent_count_is_7(void) {
    TEST_ASSERT_EQUAL_INT(7, tectonic_supercontinent_count());
}

void test_supercontinent_count_matches_define(void) {
    TEST_ASSERT_EQUAL_INT(SUPERCONTINENT_COUNT, tectonic_supercontinent_count());
}

/* --- Supercontinent lookup by index --- */

void test_supercontinent_get_vaalbara(void) {
    supercontinent_t s = tectonic_supercontinent_get(0);
    TEST_ASSERT_EQUAL_INT(0, s.id);
    TEST_ASSERT_EQUAL_STRING("Vaalbara", s.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 3600.0f, (float)s.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 2800.0f, (float)s.end_ma);
    TEST_ASSERT_NOT_NULL(s.description);
}

void test_supercontinent_get_kenorland(void) {
    supercontinent_t s = tectonic_supercontinent_get(1);
    TEST_ASSERT_EQUAL_STRING("Kenorland", s.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 2700.0f, (float)s.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 2100.0f, (float)s.end_ma);
}

void test_supercontinent_get_columbia(void) {
    supercontinent_t s = tectonic_supercontinent_get(2);
    TEST_ASSERT_EQUAL_STRING("Columbia (Nuna)", s.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 1800.0f, (float)s.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 1350.0f, (float)s.end_ma);
}

void test_supercontinent_get_rodinia(void) {
    supercontinent_t s = tectonic_supercontinent_get(3);
    TEST_ASSERT_EQUAL_STRING("Rodinia", s.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 1100.0f, (float)s.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 750.0f, (float)s.end_ma);
}

void test_supercontinent_get_pannotia(void) {
    supercontinent_t s = tectonic_supercontinent_get(4);
    TEST_ASSERT_EQUAL_STRING("Pannotia", s.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 650.0f, (float)s.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 540.0f, (float)s.end_ma);
}

void test_supercontinent_get_pangaea(void) {
    supercontinent_t s = tectonic_supercontinent_get(5);
    TEST_ASSERT_EQUAL_STRING("Pangaea", s.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 335.0f, (float)s.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 175.0f, (float)s.end_ma);
}

void test_supercontinent_get_pangaea_proxima(void) {
    supercontinent_t s = tectonic_supercontinent_get(6);
    TEST_ASSERT_EQUAL_STRING("Pangaea Proxima", s.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 250.0f, (float)s.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 250.0f, (float)s.end_ma);
}

void test_supercontinent_get_invalid_negative(void) {
    supercontinent_t s = tectonic_supercontinent_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, s.id);
    TEST_ASSERT_NULL(s.name);
}

void test_supercontinent_get_invalid_too_large(void) {
    supercontinent_t s = tectonic_supercontinent_get(999);
    TEST_ASSERT_EQUAL_INT(-1, s.id);
    TEST_ASSERT_NULL(s.name);
}

/* --- Supercontinent at time --- */

void test_supercontinent_at_3200_returns_vaalbara(void) {
    /* 3200 Ma is within Vaalbara (3600-2800) */
    supercontinent_t s = tectonic_supercontinent_at(3200.0);
    TEST_ASSERT_EQUAL_STRING("Vaalbara", s.name);
}

void test_supercontinent_at_1000_returns_rodinia(void) {
    /* 1000 Ma is within Rodinia (1100-750) */
    supercontinent_t s = tectonic_supercontinent_at(1000.0);
    TEST_ASSERT_EQUAL_STRING("Rodinia", s.name);
}

void test_supercontinent_at_300_returns_pangaea(void) {
    /* 300 Ma is within Pangaea (335-175) */
    supercontinent_t s = tectonic_supercontinent_at(300.0);
    TEST_ASSERT_EQUAL_STRING("Pangaea", s.name);
}

void test_supercontinent_at_gap_returns_zeroed(void) {
    /* 100 Ma is in the gap after Pangaea broke up (175 Ma) */
    supercontinent_t s = tectonic_supercontinent_at(100.0);
    TEST_ASSERT_EQUAL_INT(0, s.id);
    TEST_ASSERT_NULL(s.name);
}

void test_supercontinent_at_4000_before_any(void) {
    /* 4000 Ma is before any supercontinent */
    supercontinent_t s = tectonic_supercontinent_at(4000.0);
    TEST_ASSERT_NULL(s.name);
}

void test_supercontinent_at_exact_start(void) {
    /* Exact start boundary: 335 Ma = Pangaea start */
    supercontinent_t s = tectonic_supercontinent_at(335.0);
    TEST_ASSERT_EQUAL_STRING("Pangaea", s.name);
}

void test_supercontinent_at_exact_end(void) {
    /* Exact end boundary: 175 Ma = Pangaea end */
    supercontinent_t s = tectonic_supercontinent_at(175.0);
    TEST_ASSERT_EQUAL_STRING("Pangaea", s.name);
}

/* --- Supercontinent name at time --- */

void test_supercontinent_name_at_300_returns_pangaea(void) {
    const char *name = tectonic_supercontinent_name_at(300.0);
    TEST_ASSERT_EQUAL_STRING("Pangaea", name);
}

void test_supercontinent_name_at_gap_returns_none(void) {
    const char *name = tectonic_supercontinent_name_at(100.0);
    TEST_ASSERT_EQUAL_STRING("None", name);
}

/* --- In supercontinent check --- */

void test_in_supercontinent_true(void) {
    TEST_ASSERT_TRUE(tectonic_in_supercontinent(300.0));
}

void test_in_supercontinent_false(void) {
    TEST_ASSERT_FALSE(tectonic_in_supercontinent(100.0));
}

/* --- Next supercontinent --- */

void test_next_supercontinent_from_500(void) {
    /* From 500 Ma, next supercontinent forming is Pangaea at 335 Ma */
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 335.0f, (float)tectonic_next_supercontinent(500.0));
}

void test_next_supercontinent_from_2000(void) {
    /* From 2000 Ma, next supercontinent forming is Columbia at 1800 Ma */
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 1800.0f, (float)tectonic_next_supercontinent(2000.0));
}

void test_next_supercontinent_from_100(void) {
    /* From 100 Ma, no supercontinent with start_ma < 100 except Pangaea Proxima
     * which has start_ma=250. 250 < 100 is false. So returns 0.0. */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, (float)tectonic_next_supercontinent(100.0));
}

void test_next_supercontinent_from_0(void) {
    /* From 0 Ma (present), no supercontinent with start_ma < 0 */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, (float)tectonic_next_supercontinent(0.0));
}

/* --- Tectonic event count --- */

void test_event_count_at_least_21(void) {
    TEST_ASSERT_GREATER_OR_EQUAL(21, tectonic_event_count());
}

/* --- Tectonic event lookup by index --- */

void test_event_get_isua(void) {
    tectonic_event_t e = tectonic_event_get(0);
    TEST_ASSERT_EQUAL_INT(0, e.id);
    TEST_ASSERT_EQUAL_STRING("Isua Greenstone Belt", e.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 3800.0f, (float)e.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 3700.0f, (float)e.end_ma);
    TEST_ASSERT_EQUAL_STRING("volcanism", e.type);
    TEST_ASSERT_NOT_NULL(e.description);
}

void test_event_get_pangaea_breakup_volcanism(void) {
    /* Siberian Traps at index 13 */
    tectonic_event_t e = tectonic_event_get(13);
    TEST_ASSERT_EQUAL_STRING("Siberian Traps", e.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 252.0f, (float)e.start_ma);
    TEST_ASSERT_EQUAL_STRING("volcanism", e.type);
}

void test_event_get_alpine_orogeny(void) {
    tectonic_event_t e = tectonic_event_get(18);
    TEST_ASSERT_EQUAL_STRING("Alpine Orogeny", e.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 65.0f, (float)e.start_ma);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 0.0f, (float)e.end_ma);
    TEST_ASSERT_EQUAL_STRING("orogeny", e.type);
}

void test_event_get_invalid_negative(void) {
    tectonic_event_t e = tectonic_event_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, e.id);
    TEST_ASSERT_NULL(e.name);
}

void test_event_get_invalid_too_large(void) {
    tectonic_event_t e = tectonic_event_get(999);
    TEST_ASSERT_EQUAL_INT(-1, e.id);
    TEST_ASSERT_NULL(e.name);
}

/* --- Events between time range --- */

void test_events_between_permian_covers_siberian_traps(void) {
    /* Siberian Traps (252-250 Ma) should overlap with range 260-245 */
    int indices[32];
    int found = tectonic_events_between(260.0, 245.0, indices, 32);
    TEST_ASSERT_GREATER_OR_EQUAL(1, found);
    /* Verify Siberian Traps is in results */
    int has_siberian = 0;
    for (int i = 0; i < found; i++) {
        tectonic_event_t e = tectonic_event_get(indices[i]);
        if (strcmp(e.name, "Siberian Traps") == 0) has_siberian = 1;
    }
    TEST_ASSERT_TRUE(has_siberian);
}

void test_events_between_wide_range(void) {
    /* Full range should return all events */
    int indices[32];
    int found = tectonic_events_between(4000.0, 0.0, indices, 32);
    TEST_ASSERT_EQUAL_INT(tectonic_event_count(), found);
}

void test_events_between_no_results(void) {
    /* Range 4500-4000 Ma: before all events (oldest is Isua at 3800) */
    int indices[8];
    int found = tectonic_events_between(4500.0, 4000.0, indices, 8);
    TEST_ASSERT_EQUAL_INT(0, found);
}

void test_events_between_swapped_order(void) {
    /* Pass younger first (reversed), should still work */
    int indices[32];
    int found = tectonic_events_between(245.0, 260.0, indices, 32);
    TEST_ASSERT_GREATER_OR_EQUAL(1, found);
}

void test_events_between_cap(void) {
    /* Wide range but capped at 3 */
    int indices[3];
    int found = tectonic_events_between(4000.0, 0.0, indices, 3);
    TEST_ASSERT_EQUAL_INT(3, found);
}

void test_events_between_null_array(void) {
    int found = tectonic_events_between(4000.0, 0.0, NULL, 10);
    TEST_ASSERT_EQUAL_INT(0, found);
}

/* --- Events by type --- */

void test_events_by_type_orogeny(void) {
    int indices[32];
    int found = tectonic_events_by_type("orogeny", indices, 32);
    /* We have at least 10 orogenies in the data */
    TEST_ASSERT_GREATER_OR_EQUAL(10, found);
    /* Verify all results are orogeny type */
    for (int i = 0; i < found; i++) {
        tectonic_event_t e = tectonic_event_get(indices[i]);
        TEST_ASSERT_EQUAL_STRING("orogeny", e.type);
    }
}

void test_events_by_type_volcanism(void) {
    int indices[32];
    int found = tectonic_events_by_type("volcanism", indices, 32);
    TEST_ASSERT_GREATER_OR_EQUAL(5, found);
}

void test_events_by_type_rift(void) {
    int indices[32];
    int found = tectonic_events_by_type("rift", indices, 32);
    TEST_ASSERT_GREATER_OR_EQUAL(4, found);
}

void test_events_by_type_invalid(void) {
    int indices[8];
    int found = tectonic_events_by_type("earthquake", indices, 8);
    TEST_ASSERT_EQUAL_INT(0, found);
}

void test_events_by_type_null_type(void) {
    int indices[8];
    int found = tectonic_events_by_type(NULL, indices, 8);
    TEST_ASSERT_EQUAL_INT(0, found);
}

void test_events_by_type_null_array(void) {
    int found = tectonic_events_by_type("orogeny", NULL, 10);
    TEST_ASSERT_EQUAL_INT(0, found);
}

/* --- Data ordering --- */

void test_supercontinents_chronological_order(void) {
    /* Supercontinents should be in oldest-first order (highest Ma first),
     * except Pangaea Proxima which is a future placeholder */
    int count = tectonic_supercontinent_count();
    for (int i = 1; i < count - 1; i++) {
        supercontinent_t prev = tectonic_supercontinent_get(i - 1);
        supercontinent_t curr = tectonic_supercontinent_get(i);
        TEST_ASSERT_TRUE_MESSAGE(
            prev.start_ma >= curr.start_ma,
            "Supercontinents must be in chronological order (oldest first)");
    }
}

void test_events_chronological_order(void) {
    /* Events should be in oldest-first order */
    int count = tectonic_event_count();
    for (int i = 1; i < count; i++) {
        tectonic_event_t prev = tectonic_event_get(i - 1);
        tectonic_event_t curr = tectonic_event_get(i);
        TEST_ASSERT_TRUE_MESSAGE(
            prev.start_ma >= curr.start_ma,
            "Events must be in chronological order (oldest first)");
    }
}

/* --- main --- */

int main(void) {
    UNITY_BEGIN();

    /* Supercontinent count */
    RUN_TEST(test_supercontinent_count_is_7);
    RUN_TEST(test_supercontinent_count_matches_define);

    /* Supercontinent lookup by index */
    RUN_TEST(test_supercontinent_get_vaalbara);
    RUN_TEST(test_supercontinent_get_kenorland);
    RUN_TEST(test_supercontinent_get_columbia);
    RUN_TEST(test_supercontinent_get_rodinia);
    RUN_TEST(test_supercontinent_get_pannotia);
    RUN_TEST(test_supercontinent_get_pangaea);
    RUN_TEST(test_supercontinent_get_pangaea_proxima);
    RUN_TEST(test_supercontinent_get_invalid_negative);
    RUN_TEST(test_supercontinent_get_invalid_too_large);

    /* Supercontinent at time */
    RUN_TEST(test_supercontinent_at_3200_returns_vaalbara);
    RUN_TEST(test_supercontinent_at_1000_returns_rodinia);
    RUN_TEST(test_supercontinent_at_300_returns_pangaea);
    RUN_TEST(test_supercontinent_at_gap_returns_zeroed);
    RUN_TEST(test_supercontinent_at_4000_before_any);
    RUN_TEST(test_supercontinent_at_exact_start);
    RUN_TEST(test_supercontinent_at_exact_end);

    /* Supercontinent name at time */
    RUN_TEST(test_supercontinent_name_at_300_returns_pangaea);
    RUN_TEST(test_supercontinent_name_at_gap_returns_none);

    /* In supercontinent */
    RUN_TEST(test_in_supercontinent_true);
    RUN_TEST(test_in_supercontinent_false);

    /* Next supercontinent */
    RUN_TEST(test_next_supercontinent_from_500);
    RUN_TEST(test_next_supercontinent_from_2000);
    RUN_TEST(test_next_supercontinent_from_100);
    RUN_TEST(test_next_supercontinent_from_0);

    /* Event count */
    RUN_TEST(test_event_count_at_least_21);

    /* Event lookup */
    RUN_TEST(test_event_get_isua);
    RUN_TEST(test_event_get_pangaea_breakup_volcanism);
    RUN_TEST(test_event_get_alpine_orogeny);
    RUN_TEST(test_event_get_invalid_negative);
    RUN_TEST(test_event_get_invalid_too_large);

    /* Events between */
    RUN_TEST(test_events_between_permian_covers_siberian_traps);
    RUN_TEST(test_events_between_wide_range);
    RUN_TEST(test_events_between_no_results);
    RUN_TEST(test_events_between_swapped_order);
    RUN_TEST(test_events_between_cap);
    RUN_TEST(test_events_between_null_array);

    /* Events by type */
    RUN_TEST(test_events_by_type_orogeny);
    RUN_TEST(test_events_by_type_volcanism);
    RUN_TEST(test_events_by_type_rift);
    RUN_TEST(test_events_by_type_invalid);
    RUN_TEST(test_events_by_type_null_type);
    RUN_TEST(test_events_by_type_null_array);

    /* Data ordering */
    RUN_TEST(test_supercontinents_chronological_order);
    RUN_TEST(test_events_chronological_order);

    return UNITY_END();
}
