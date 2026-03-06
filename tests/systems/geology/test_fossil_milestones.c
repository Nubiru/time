/* test_fossil_milestones.c — Tests for fossil record milestones data module.
 * TDD RED phase: all tests written before implementation. */

#include "../../unity/unity.h"
#include "../../../src/systems/geology/fossil_milestones.h"

#include <string.h>

void setUp(void) { }
void tearDown(void) { }

/* --- Milestone count --- */

void test_fossil_milestone_count_at_least_21(void) {
    TEST_ASSERT_GREATER_OR_EQUAL(21, fossil_milestone_count());
}

/* --- Milestone lookup by index --- */

void test_fossil_milestone_get_index_0_first_life(void) {
    fossil_milestone_t m = fossil_milestone_get(0);
    TEST_ASSERT_EQUAL_INT(0, m.id);
    TEST_ASSERT_EQUAL_STRING("First Life", m.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 3500.0f, (float)m.ma);
    TEST_ASSERT_EQUAL_INT(FOSSIL_CAT_ORIGIN, m.category);
    TEST_ASSERT_NOT_NULL(m.what_appeared);
    TEST_ASSERT_NOT_NULL(m.what_changed);
    TEST_ASSERT_NOT_NULL(m.significance);
}

void test_fossil_milestone_get_cambrian_explosion(void) {
    fossil_milestone_t m = fossil_milestone_get(5);
    TEST_ASSERT_EQUAL_STRING("Cambrian Explosion", m.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 541.0f, (float)m.ma);
    TEST_ASSERT_EQUAL_INT(FOSSIL_CAT_DIVERSIFICATION, m.category);
}

void test_fossil_milestone_get_kpg_extinction(void) {
    fossil_milestone_t m = fossil_milestone_get(16);
    TEST_ASSERT_EQUAL_STRING("K-Pg Extinction", m.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 66.0f, (float)m.ma);
    TEST_ASSERT_EQUAL_INT(FOSSIL_CAT_EXTINCTION, m.category);
}

void test_fossil_milestone_get_homo_sapiens(void) {
    fossil_milestone_t m = fossil_milestone_get(20);
    TEST_ASSERT_EQUAL_STRING("Homo sapiens", m.name);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.3f, (float)m.ma);
    TEST_ASSERT_EQUAL_INT(FOSSIL_CAT_ORIGIN, m.category);
}

void test_fossil_milestone_get_photosynthesis(void) {
    fossil_milestone_t m = fossil_milestone_get(1);
    TEST_ASSERT_EQUAL_STRING("Photosynthesis", m.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 2700.0f, (float)m.ma);
    TEST_ASSERT_EQUAL_INT(FOSSIL_CAT_INNOVATION, m.category);
}

void test_fossil_milestone_get_first_eukaryotes(void) {
    fossil_milestone_t m = fossil_milestone_get(2);
    TEST_ASSERT_EQUAL_STRING("First Eukaryotes", m.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 2100.0f, (float)m.ma);
    TEST_ASSERT_EQUAL_INT(FOSSIL_CAT_INNOVATION, m.category);
}

void test_fossil_milestone_get_first_land_plants(void) {
    fossil_milestone_t m = fossil_milestone_get(6);
    TEST_ASSERT_EQUAL_STRING("First Land Plants", m.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 470.0f, (float)m.ma);
    TEST_ASSERT_EQUAL_INT(FOSSIL_CAT_COLONIZATION, m.category);
}

void test_fossil_milestone_get_permian_triassic(void) {
    fossil_milestone_t m = fossil_milestone_get(12);
    TEST_ASSERT_EQUAL_STRING("Permian-Triassic Extinction", m.name);
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 252.0f, (float)m.ma);
    TEST_ASSERT_EQUAL_INT(FOSSIL_CAT_EXTINCTION, m.category);
}

void test_fossil_milestone_get_invalid_negative(void) {
    fossil_milestone_t m = fossil_milestone_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, m.id);
    TEST_ASSERT_NULL(m.name);
}

void test_fossil_milestone_get_invalid_too_large(void) {
    fossil_milestone_t m = fossil_milestone_get(999);
    TEST_ASSERT_EQUAL_INT(-1, m.id);
    TEST_ASSERT_NULL(m.name);
}

/* --- Milestone name by index --- */

void test_fossil_milestone_name_valid(void) {
    TEST_ASSERT_EQUAL_STRING("First Life", fossil_milestone_name(0));
}

void test_fossil_milestone_name_invalid(void) {
    TEST_ASSERT_EQUAL_STRING("?", fossil_milestone_name(-1));
    TEST_ASSERT_EQUAL_STRING("?", fossil_milestone_name(999));
}

/* --- Milestone at time (nearest) --- */

void test_fossil_milestone_at_3500_returns_first_life(void) {
    fossil_milestone_t m = fossil_milestone_at(3500.0);
    TEST_ASSERT_EQUAL_STRING("First Life", m.name);
}

void test_fossil_milestone_at_540_returns_cambrian(void) {
    /* 541 is Cambrian, 470 is First Land Plants. 540 is closest to 541. */
    fossil_milestone_t m = fossil_milestone_at(540.0);
    TEST_ASSERT_EQUAL_STRING("Cambrian Explosion", m.name);
}

void test_fossil_milestone_at_65_returns_kpg(void) {
    /* K-Pg is at 66 Ma. 65 is closest to 66. */
    fossil_milestone_t m = fossil_milestone_at(65.0);
    TEST_ASSERT_EQUAL_STRING("K-Pg Extinction", m.name);
}

void test_fossil_milestone_at_0_returns_homo_sapiens(void) {
    /* 0 Ma: closest to Homo sapiens at 0.3 Ma */
    fossil_milestone_t m = fossil_milestone_at(0.0);
    TEST_ASSERT_EQUAL_STRING("Homo sapiens", m.name);
}

void test_fossil_milestone_at_exact_match(void) {
    /* Exact match on 252 Ma = Permian-Triassic */
    fossil_milestone_t m = fossil_milestone_at(252.0);
    TEST_ASSERT_EQUAL_STRING("Permian-Triassic Extinction", m.name);
}

/* --- Milestones in range --- */

void test_fossil_milestones_in_range_big_five_extinctions(void) {
    /* Range 500 to 60 Ma should include all Big Five extinctions:
     * Late Devonian (372), Permian-Triassic (252),
     * Triassic-Jurassic (201 — not in spec but in data at index 13 is first mammals at 225),
     * K-Pg (66). But also Late Ordovician (445) is in range.
     * Just check we get at least some extinction events. */
    int indices[32];
    int found = fossil_milestones_in_range(500.0, 60.0, indices, 32);
    TEST_ASSERT_GREATER_OR_EQUAL(3, found);
}

void test_fossil_milestones_in_range_precambrian(void) {
    /* Range 4000 to 600 Ma: should include First Life, Photosynthesis,
     * Eukaryotes, Sexual Reproduction, Multicellular Life */
    int indices[32];
    int found = fossil_milestones_in_range(4000.0, 600.0, indices, 32);
    TEST_ASSERT_GREATER_OR_EQUAL(5, found);

    /* Verify First Life is in results */
    int has_first_life = 0;
    for (int i = 0; i < found; i++) {
        fossil_milestone_t m = fossil_milestone_get(indices[i]);
        if (strcmp(m.name, "First Life") == 0) has_first_life = 1;
    }
    TEST_ASSERT_TRUE(has_first_life);
}

void test_fossil_milestones_in_range_narrow(void) {
    /* Range 255 to 250: should include Permian-Triassic (252) */
    int indices[8];
    int found = fossil_milestones_in_range(255.0, 250.0, indices, 8);
    TEST_ASSERT_GREATER_OR_EQUAL(1, found);
    fossil_milestone_t m = fossil_milestone_get(indices[0]);
    TEST_ASSERT_EQUAL_STRING("Permian-Triassic Extinction", m.name);
}

void test_fossil_milestones_in_range_no_results(void) {
    /* Range 3000 to 2800: between First Life (3500) and Photosynthesis (2700) */
    int indices[8];
    int found = fossil_milestones_in_range(3000.0, 2800.0, indices, 8);
    TEST_ASSERT_EQUAL_INT(0, found);
}

void test_fossil_milestones_in_range_max_cap(void) {
    /* Wide range but capped at 3 results */
    int indices[3];
    int found = fossil_milestones_in_range(4000.0, 0.0, indices, 3);
    TEST_ASSERT_EQUAL_INT(3, found);
}

void test_fossil_milestones_in_range_null_array(void) {
    int found = fossil_milestones_in_range(4000.0, 0.0, NULL, 10);
    TEST_ASSERT_EQUAL_INT(0, found);
}

/* --- Category counting --- */

void test_fossil_milestones_of_category_origin(void) {
    /* First Life, Multicellular, Insects, Mammals, Primates, Hominids, Homo sapiens = 7 */
    int count = fossil_milestones_of_category(FOSSIL_CAT_ORIGIN);
    TEST_ASSERT_GREATER_OR_EQUAL(7, count);
}

void test_fossil_milestones_of_category_extinction(void) {
    /* Late Devonian, Permian-Triassic, K-Pg = 3 extinction events */
    int count = fossil_milestones_of_category(FOSSIL_CAT_EXTINCTION);
    TEST_ASSERT_GREATER_OR_EQUAL(3, count);
}

void test_fossil_milestones_of_category_innovation(void) {
    /* Photosynthesis, Eukaryotes, Reptiles, Flowering Plants, Stone Tools = 5 */
    int count = fossil_milestones_of_category(FOSSIL_CAT_INNOVATION);
    TEST_ASSERT_GREATER_OR_EQUAL(5, count);
}

/* --- Category name --- */

void test_fossil_category_name_origin(void) {
    TEST_ASSERT_EQUAL_STRING("Origin", fossil_category_name(FOSSIL_CAT_ORIGIN));
}

void test_fossil_category_name_extinction(void) {
    TEST_ASSERT_EQUAL_STRING("Extinction", fossil_category_name(FOSSIL_CAT_EXTINCTION));
}

void test_fossil_category_name_colonization(void) {
    TEST_ASSERT_EQUAL_STRING("Colonization", fossil_category_name(FOSSIL_CAT_COLONIZATION));
}

void test_fossil_category_name_diversification(void) {
    TEST_ASSERT_EQUAL_STRING("Diversification", fossil_category_name(FOSSIL_CAT_DIVERSIFICATION));
}

void test_fossil_category_name_innovation(void) {
    TEST_ASSERT_EQUAL_STRING("Innovation", fossil_category_name(FOSSIL_CAT_INNOVATION));
}

/* --- Extinction detection --- */

void test_fossil_is_extinction_at_kpg(void) {
    /* K-Pg at 66 Ma, tolerance 1 Ma */
    TEST_ASSERT_TRUE(fossil_is_extinction(66.0, 1.0));
}

void test_fossil_is_extinction_near_kpg(void) {
    /* 67 Ma with tolerance 2 Ma: 67 is within 2 of 66 */
    TEST_ASSERT_TRUE(fossil_is_extinction(67.0, 2.0));
}

void test_fossil_is_extinction_at_permian(void) {
    /* Permian-Triassic at 252 Ma */
    TEST_ASSERT_TRUE(fossil_is_extinction(252.0, 1.0));
}

void test_fossil_is_extinction_at_devonian(void) {
    /* Late Devonian at 372 Ma */
    TEST_ASSERT_TRUE(fossil_is_extinction(372.0, 1.0));
}

void test_fossil_is_extinction_false_at_541(void) {
    /* 541 Ma is Cambrian Explosion (DIVERSIFICATION, not EXTINCTION) */
    TEST_ASSERT_FALSE(fossil_is_extinction(541.0, 1.0));
}

void test_fossil_is_extinction_false_outside_tolerance(void) {
    /* 70 Ma with tolerance 1 Ma: K-Pg is at 66, difference is 4 > 1 */
    TEST_ASSERT_FALSE(fossil_is_extinction(70.0, 1.0));
}

/* --- Data ordering check --- */

void test_fossil_milestones_chronological_order(void) {
    /* All milestones should be in chronological order (oldest first = highest Ma) */
    int count = fossil_milestone_count();
    for (int i = 1; i < count; i++) {
        fossil_milestone_t prev = fossil_milestone_get(i - 1);
        fossil_milestone_t curr = fossil_milestone_get(i);
        TEST_ASSERT_TRUE_MESSAGE(
            prev.ma >= curr.ma,
            "Milestones must be in chronological order (oldest first)");
    }
}

/* --- main --- */

int main(void) {
    UNITY_BEGIN();

    /* Milestone count */
    RUN_TEST(test_fossil_milestone_count_at_least_21);

    /* Milestone lookup by index */
    RUN_TEST(test_fossil_milestone_get_index_0_first_life);
    RUN_TEST(test_fossil_milestone_get_cambrian_explosion);
    RUN_TEST(test_fossil_milestone_get_kpg_extinction);
    RUN_TEST(test_fossil_milestone_get_homo_sapiens);
    RUN_TEST(test_fossil_milestone_get_photosynthesis);
    RUN_TEST(test_fossil_milestone_get_first_eukaryotes);
    RUN_TEST(test_fossil_milestone_get_first_land_plants);
    RUN_TEST(test_fossil_milestone_get_permian_triassic);
    RUN_TEST(test_fossil_milestone_get_invalid_negative);
    RUN_TEST(test_fossil_milestone_get_invalid_too_large);

    /* Milestone name */
    RUN_TEST(test_fossil_milestone_name_valid);
    RUN_TEST(test_fossil_milestone_name_invalid);

    /* Milestone at time (nearest) */
    RUN_TEST(test_fossil_milestone_at_3500_returns_first_life);
    RUN_TEST(test_fossil_milestone_at_540_returns_cambrian);
    RUN_TEST(test_fossil_milestone_at_65_returns_kpg);
    RUN_TEST(test_fossil_milestone_at_0_returns_homo_sapiens);
    RUN_TEST(test_fossil_milestone_at_exact_match);

    /* Milestones in range */
    RUN_TEST(test_fossil_milestones_in_range_big_five_extinctions);
    RUN_TEST(test_fossil_milestones_in_range_precambrian);
    RUN_TEST(test_fossil_milestones_in_range_narrow);
    RUN_TEST(test_fossil_milestones_in_range_no_results);
    RUN_TEST(test_fossil_milestones_in_range_max_cap);
    RUN_TEST(test_fossil_milestones_in_range_null_array);

    /* Category counting */
    RUN_TEST(test_fossil_milestones_of_category_origin);
    RUN_TEST(test_fossil_milestones_of_category_extinction);
    RUN_TEST(test_fossil_milestones_of_category_innovation);

    /* Category names */
    RUN_TEST(test_fossil_category_name_origin);
    RUN_TEST(test_fossil_category_name_extinction);
    RUN_TEST(test_fossil_category_name_colonization);
    RUN_TEST(test_fossil_category_name_diversification);
    RUN_TEST(test_fossil_category_name_innovation);

    /* Extinction detection */
    RUN_TEST(test_fossil_is_extinction_at_kpg);
    RUN_TEST(test_fossil_is_extinction_near_kpg);
    RUN_TEST(test_fossil_is_extinction_at_permian);
    RUN_TEST(test_fossil_is_extinction_at_devonian);
    RUN_TEST(test_fossil_is_extinction_false_at_541);
    RUN_TEST(test_fossil_is_extinction_false_outside_tolerance);

    /* Data ordering */
    RUN_TEST(test_fossil_milestones_chronological_order);

    return UNITY_END();
}
