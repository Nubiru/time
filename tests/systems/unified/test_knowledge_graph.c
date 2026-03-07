/* test_knowledge_graph.c — Knowledge Graph Data module tests
 * TDD RED phase: tests written before implementation.
 * Tests the contributor knowledge transmission network. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/knowledge_graph.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ===== kg_contributor_count ===== */

void test_count_at_least_20(void)
{
    TEST_ASSERT_TRUE(kg_contributor_count() >= 20);
}

/* ===== kg_contributor_get ===== */

void test_get_king_wen(void)
{
    kg_contributor_t c = kg_contributor_get(0);
    TEST_ASSERT_EQUAL_INT(0, c.id);
    TEST_ASSERT_EQUAL_STRING("King Wen", c.name);
    TEST_ASSERT_EQUAL_INT(-1050, c.year);
    TEST_ASSERT_EQUAL_STRING("Chinese", c.culture);
    TEST_ASSERT_EQUAL_INT(ERA_ANCIENT, c.era);
    TEST_ASSERT_TRUE(c.system_count >= 1);
    TEST_ASSERT_EQUAL_INT(KG_SYS_ICHING, c.systems[0]);
}

void test_get_ptolemy(void)
{
    kg_contributor_t c = kg_contributor_get(4);
    TEST_ASSERT_EQUAL_INT(4, c.id);
    TEST_ASSERT_EQUAL_STRING("Ptolemy", c.name);
    TEST_ASSERT_EQUAL_INT(150, c.year);
    TEST_ASSERT_EQUAL_STRING("Greek", c.culture);
    TEST_ASSERT_EQUAL_INT(ERA_CLASSICAL, c.era);
    TEST_ASSERT_TRUE(c.system_count >= 2);
}

void test_get_kepler(void)
{
    kg_contributor_t c = kg_contributor_get(11);
    TEST_ASSERT_EQUAL_INT(11, c.id);
    TEST_ASSERT_EQUAL_STRING("Kepler", c.name);
    TEST_ASSERT_EQUAL_INT(1609, c.year);
    TEST_ASSERT_EQUAL_STRING("German", c.culture);
    TEST_ASSERT_EQUAL_INT(ERA_EARLY_MODERN, c.era);
}

void test_get_meeus(void)
{
    kg_contributor_t c = kg_contributor_get(13);
    TEST_ASSERT_EQUAL_STRING("Jean Meeus", c.name);
    TEST_ASSERT_EQUAL_INT(1991, c.year);
    TEST_ASSERT_EQUAL_INT(ERA_MODERN, c.era);
    TEST_ASSERT_TRUE(c.system_count >= 2);
}

void test_get_invalid_negative(void)
{
    kg_contributor_t c = kg_contributor_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, c.id);
    TEST_ASSERT_NULL(c.name);
}

void test_get_invalid_too_large(void)
{
    kg_contributor_t c = kg_contributor_get(9999);
    TEST_ASSERT_EQUAL_INT(-1, c.id);
    TEST_ASSERT_NULL(c.name);
}

/* ===== kg_contributor_find ===== */

void test_find_hipparchus(void)
{
    int idx = kg_contributor_find("Hipparchus");
    TEST_ASSERT_EQUAL_INT(2, idx);
}

void test_find_arguelles(void)
{
    int idx = kg_contributor_find("Jose Arguelles");
    TEST_ASSERT_EQUAL_INT(14, idx);
}

void test_find_ra_uru_hu(void)
{
    int idx = kg_contributor_find("Ra Uru Hu");
    TEST_ASSERT_EQUAL_INT(15, idx);
}

void test_find_not_found(void)
{
    int idx = kg_contributor_find("Nobody");
    TEST_ASSERT_EQUAL_INT(-1, idx);
}

void test_find_null(void)
{
    int idx = kg_contributor_find(NULL);
    TEST_ASSERT_EQUAL_INT(-1, idx);
}

/* ===== kg_contributors_for_system ===== */

void test_system_astronomy_has_many(void)
{
    int indices[20];
    int count = kg_contributors_for_system(KG_SYS_ASTRONOMY, indices, 20);
    /* Hipparchus, Eratosthenes, Ptolemy, Aryabhata, Brahmagupta,
       al-Khwarizmi, Copernicus, Kepler, Meeus, Ulugh Beg, Tycho Brahe */
    TEST_ASSERT_TRUE(count >= 8);
}

void test_system_iching(void)
{
    int indices[10];
    int count = kg_contributors_for_system(KG_SYS_ICHING, indices, 10);
    /* King Wen, Richard Wilhelm */
    TEST_ASSERT_TRUE(count >= 2);
}

void test_system_kabbalah(void)
{
    int indices[10];
    int count = kg_contributors_for_system(KG_SYS_KABBALAH, indices, 10);
    /* Moses de Leon, Isaac Luria */
    TEST_ASSERT_TRUE(count >= 2);
}

void test_system_limit_respected(void)
{
    int indices[2];
    int count = kg_contributors_for_system(KG_SYS_ASTRONOMY, indices, 2);
    TEST_ASSERT_EQUAL_INT(2, count);
}

/* ===== kg_contributors_by_era ===== */

void test_era_ancient(void)
{
    int indices[10];
    int count = kg_contributors_by_era(ERA_ANCIENT, indices, 10);
    /* King Wen, Siddhartha Gautama */
    TEST_ASSERT_TRUE(count >= 2);
}

void test_era_classical(void)
{
    int indices[10];
    int count = kg_contributors_by_era(ERA_CLASSICAL, indices, 10);
    /* Hipparchus, Eratosthenes, Ptolemy, Aryabhata, Hillel II */
    TEST_ASSERT_TRUE(count >= 4);
}

void test_era_modern(void)
{
    int indices[10];
    int count = kg_contributors_by_era(ERA_MODERN, indices, 10);
    /* Richard Wilhelm, Meeus, Arguelles, Ra Uru Hu */
    TEST_ASSERT_TRUE(count >= 4);
}

/* ===== kg_contributors_by_culture ===== */

void test_culture_greek(void)
{
    int indices[10];
    int count = kg_contributors_by_culture("Greek", indices, 10);
    /* Hipparchus, Eratosthenes, Ptolemy */
    TEST_ASSERT_TRUE(count >= 3);
}

void test_culture_indian(void)
{
    int indices[10];
    int count = kg_contributors_by_culture("Indian", indices, 10);
    /* Siddhartha Gautama, Aryabhata, Brahmagupta */
    TEST_ASSERT_TRUE(count >= 3);
}

void test_culture_german(void)
{
    int indices[10];
    int count = kg_contributors_by_culture("German", indices, 10);
    /* Richard Wilhelm, Kepler */
    TEST_ASSERT_TRUE(count >= 2);
}

void test_culture_null(void)
{
    int indices[10];
    int count = kg_contributors_by_culture(NULL, indices, 10);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* ===== kg_contributor_systems ===== */

void test_contributor_systems_ptolemy(void)
{
    kg_system_t systems[KG_MAX_SYSTEMS];
    int count = kg_contributor_systems(4, systems, KG_MAX_SYSTEMS);
    TEST_ASSERT_TRUE(count >= 2);
    /* Should include ASTRONOMY and ASTROLOGY */
    int has_astronomy = 0;
    int has_astrology = 0;
    for (int i = 0; i < count; i++) {
        if (systems[i] == KG_SYS_ASTRONOMY) has_astronomy = 1;
        if (systems[i] == KG_SYS_ASTROLOGY) has_astrology = 1;
    }
    TEST_ASSERT_TRUE(has_astronomy);
    TEST_ASSERT_TRUE(has_astrology);
}

void test_contributor_systems_invalid(void)
{
    kg_system_t systems[KG_MAX_SYSTEMS];
    int count = kg_contributor_systems(-1, systems, KG_MAX_SYSTEMS);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* ===== kg_era_name ===== */

void test_era_name_ancient(void)
{
    TEST_ASSERT_EQUAL_STRING("Ancient", kg_era_name(ERA_ANCIENT));
}

void test_era_name_classical(void)
{
    TEST_ASSERT_EQUAL_STRING("Classical", kg_era_name(ERA_CLASSICAL));
}

void test_era_name_medieval(void)
{
    TEST_ASSERT_EQUAL_STRING("Medieval", kg_era_name(ERA_MEDIEVAL));
}

void test_era_name_early_modern(void)
{
    TEST_ASSERT_EQUAL_STRING("Early Modern", kg_era_name(ERA_EARLY_MODERN));
}

void test_era_name_modern(void)
{
    TEST_ASSERT_EQUAL_STRING("Modern", kg_era_name(ERA_MODERN));
}

void test_era_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", kg_era_name(ERA_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", kg_era_name((knowledge_era_t)-1));
}

/* ===== kg_system_name ===== */

void test_system_name_astronomy(void)
{
    TEST_ASSERT_EQUAL_STRING("Astronomy", kg_system_name(KG_SYS_ASTRONOMY));
}

void test_system_name_tzolkin(void)
{
    TEST_ASSERT_EQUAL_STRING("Tzolkin", kg_system_name(KG_SYS_TZOLKIN));
}

void test_system_name_human_design(void)
{
    TEST_ASSERT_EQUAL_STRING("Human Design", kg_system_name(KG_SYS_HUMAN_DESIGN));
}

void test_system_name_kabbalah(void)
{
    TEST_ASSERT_EQUAL_STRING("Kabbalah", kg_system_name(KG_SYS_KABBALAH));
}

void test_system_name_invalid(void)
{
    TEST_ASSERT_EQUAL_STRING("?", kg_system_name(KG_SYS_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", kg_system_name((kg_system_t)-1));
}

/* ===== kg_culture_count / kg_culture_get ===== */

void test_culture_count_at_least_10(void)
{
    /* Greek, Chinese, Indian, German, Belgian, American, Canadian,
       Jewish, Arab, Spanish, Polish, Danish, Italian, Timurid */
    TEST_ASSERT_TRUE(kg_culture_count() >= 10);
}

void test_culture_get_valid(void)
{
    const char *c = kg_culture_get(0);
    TEST_ASSERT_NOT_NULL(c);
    TEST_ASSERT_TRUE(strlen(c) > 0);
}

void test_culture_get_invalid(void)
{
    TEST_ASSERT_NULL(kg_culture_get(-1));
    TEST_ASSERT_NULL(kg_culture_get(1000));
}

/* ===== kg_edge_count ===== */

void test_edge_count(void)
{
    /* 11 directed influence edges in the graph:
     * Wen->Wilhelm, Hip->Pto, Pto->Arya, Pto->Cop, Pto->Khw,
     * Arya->Brah, Brah->Khw, Cop->Kep, Tycho->Kep, Kep->Meeus,
     * Leon->Luria */
    TEST_ASSERT_EQUAL_INT(11, kg_edge_count());
}

/* ===== Influence edge consistency ===== */

void test_influence_king_wen_to_wilhelm(void)
{
    kg_contributor_t wen = kg_contributor_get(0);
    kg_contributor_t wil = kg_contributor_get(6);
    /* King Wen's influenced should include Richard Wilhelm (6) */
    int found_fwd = 0;
    for (int i = 0; i < wen.successor_count; i++) {
        if (wen.influenced[i] == 6) found_fwd = 1;
    }
    TEST_ASSERT_TRUE(found_fwd);
    /* Richard Wilhelm's influenced_by should include King Wen (0) */
    int found_bwd = 0;
    for (int i = 0; i < wil.influence_count; i++) {
        if (wil.influenced_by[i] == 0) found_bwd = 1;
    }
    TEST_ASSERT_TRUE(found_bwd);
}

void test_influence_hipparchus_to_ptolemy(void)
{
    kg_contributor_t hip = kg_contributor_get(2);
    kg_contributor_t pto = kg_contributor_get(4);
    int found_fwd = 0;
    for (int i = 0; i < hip.successor_count; i++) {
        if (hip.influenced[i] == 4) found_fwd = 1;
    }
    TEST_ASSERT_TRUE(found_fwd);
    int found_bwd = 0;
    for (int i = 0; i < pto.influence_count; i++) {
        if (pto.influenced_by[i] == 2) found_bwd = 1;
    }
    TEST_ASSERT_TRUE(found_bwd);
}

void test_influence_kepler_has_two_predecessors(void)
{
    kg_contributor_t kep = kg_contributor_get(11);
    /* Copernicus (10) and Tycho Brahe (12) */
    TEST_ASSERT_TRUE(kep.influence_count >= 2);
    int has_copernicus = 0;
    int has_tycho = 0;
    for (int i = 0; i < kep.influence_count; i++) {
        if (kep.influenced_by[i] == 10) has_copernicus = 1;
        if (kep.influenced_by[i] == 12) has_tycho = 1;
    }
    TEST_ASSERT_TRUE(has_copernicus);
    TEST_ASSERT_TRUE(has_tycho);
}

void test_influence_al_khwarizmi_has_two_predecessors(void)
{
    kg_contributor_t alk = kg_contributor_get(8);
    /* Ptolemy (4) and Brahmagupta (7) */
    TEST_ASSERT_TRUE(alk.influence_count >= 2);
    int has_ptolemy = 0;
    int has_brahma = 0;
    for (int i = 0; i < alk.influence_count; i++) {
        if (alk.influenced_by[i] == 4) has_ptolemy = 1;
        if (alk.influenced_by[i] == 7) has_brahma = 1;
    }
    TEST_ASSERT_TRUE(has_ptolemy);
    TEST_ASSERT_TRUE(has_brahma);
}

/* ===== kg_knowledge_path ===== */

void test_path_astrology_astronomy_bridge(void)
{
    /* Ptolemy works on BOTH ASTROLOGY and ASTRONOMY -> direct bridge */
    kg_path_t p = kg_knowledge_path(KG_SYS_ASTROLOGY, KG_SYS_ASTRONOMY);
    TEST_ASSERT_TRUE(p.length >= 1);
    /* Ptolemy (4) should be on the path */
    int has_ptolemy = 0;
    for (int i = 0; i < p.length; i++) {
        if (p.path[i] == 4) has_ptolemy = 1;
    }
    TEST_ASSERT_TRUE(has_ptolemy);
}

void test_path_iching_astronomy_no_connection(void)
{
    /* I Ching (King Wen, Wilhelm) has no edge connection to any
       astronomy contributor */
    kg_path_t p = kg_knowledge_path(KG_SYS_ICHING, KG_SYS_ASTRONOMY);
    TEST_ASSERT_EQUAL_INT(0, p.length);
}

void test_path_same_system(void)
{
    /* Path from a system to itself should find at least one contributor */
    kg_path_t p = kg_knowledge_path(KG_SYS_ASTRONOMY, KG_SYS_ASTRONOMY);
    TEST_ASSERT_TRUE(p.length >= 1);
}

void test_path_hindu_astronomy(void)
{
    /* Aryabhata (5) works on both HINDU and ASTRONOMY -> direct bridge */
    kg_path_t p = kg_knowledge_path(KG_SYS_HINDU, KG_SYS_ASTRONOMY);
    TEST_ASSERT_TRUE(p.length >= 1);
}

void test_path_kabbalah_astronomy_no_connection(void)
{
    /* Kabbalah contributors (Moses de Leon, Isaac Luria) have no edges
       connecting to any Astronomy contributor */
    kg_path_t p = kg_knowledge_path(KG_SYS_KABBALAH, KG_SYS_ASTRONOMY);
    TEST_ASSERT_EQUAL_INT(0, p.length);
}

void test_path_hindu_gregorian(void)
{
    /* HINDU: Aryabhata (5), Brahmagupta (7).
     * GREGORIAN: Meeus (13), Pope Gregory XIII (18).
     * Undirected path: Aryabhata(5) <-> Ptolemy(4) <-> Copernicus(10)
     *   <-> Kepler(11) <-> Meeus(13).
     * Should find a path with length >= 2. */
    kg_path_t p = kg_knowledge_path(KG_SYS_HINDU, KG_SYS_GREGORIAN);
    TEST_ASSERT_TRUE(p.length >= 2);
}

void test_path_islamic_astronomy(void)
{
    /* al-Khwarizmi (8) works on both ISLAMIC and ASTRONOMY -> bridge */
    kg_path_t p = kg_knowledge_path(KG_SYS_ISLAMIC, KG_SYS_ASTRONOMY);
    TEST_ASSERT_TRUE(p.length >= 1);
}

void test_path_length_bounded(void)
{
    kg_path_t p = kg_knowledge_path(KG_SYS_HINDU, KG_SYS_GREGORIAN);
    TEST_ASSERT_TRUE(p.length <= KG_MAX_PATH);
}

/* ===== Key work strings ===== */

void test_key_work_king_wen(void)
{
    kg_contributor_t c = kg_contributor_get(0);
    TEST_ASSERT_NOT_NULL(c.key_work);
    TEST_ASSERT_TRUE(strlen(c.key_work) > 0);
}

void test_key_work_meeus(void)
{
    kg_contributor_t c = kg_contributor_get(13);
    TEST_ASSERT_NOT_NULL(c.key_work);
    /* Should reference Astronomical Algorithms */
    TEST_ASSERT_NOT_NULL(strstr(c.key_work, "Astronomical Algorithms"));
}

/* ===== All IDs are sequential 0..N-1 ===== */

void test_ids_sequential(void)
{
    int n = kg_contributor_count();
    for (int i = 0; i < n; i++) {
        kg_contributor_t c = kg_contributor_get(i);
        TEST_ASSERT_EQUAL_INT(i, c.id);
    }
}

/* ===== Runner ===== */

int main(void)
{
    UNITY_BEGIN();

    /* Count */
    RUN_TEST(test_count_at_least_20);

    /* Get by index */
    RUN_TEST(test_get_king_wen);
    RUN_TEST(test_get_ptolemy);
    RUN_TEST(test_get_kepler);
    RUN_TEST(test_get_meeus);
    RUN_TEST(test_get_invalid_negative);
    RUN_TEST(test_get_invalid_too_large);

    /* Find by name */
    RUN_TEST(test_find_hipparchus);
    RUN_TEST(test_find_arguelles);
    RUN_TEST(test_find_ra_uru_hu);
    RUN_TEST(test_find_not_found);
    RUN_TEST(test_find_null);

    /* Contributors for system */
    RUN_TEST(test_system_astronomy_has_many);
    RUN_TEST(test_system_iching);
    RUN_TEST(test_system_kabbalah);
    RUN_TEST(test_system_limit_respected);

    /* Contributors by era */
    RUN_TEST(test_era_ancient);
    RUN_TEST(test_era_classical);
    RUN_TEST(test_era_modern);

    /* Contributors by culture */
    RUN_TEST(test_culture_greek);
    RUN_TEST(test_culture_indian);
    RUN_TEST(test_culture_german);
    RUN_TEST(test_culture_null);

    /* Contributor systems */
    RUN_TEST(test_contributor_systems_ptolemy);
    RUN_TEST(test_contributor_systems_invalid);

    /* Era names */
    RUN_TEST(test_era_name_ancient);
    RUN_TEST(test_era_name_classical);
    RUN_TEST(test_era_name_medieval);
    RUN_TEST(test_era_name_early_modern);
    RUN_TEST(test_era_name_modern);
    RUN_TEST(test_era_name_invalid);

    /* System names */
    RUN_TEST(test_system_name_astronomy);
    RUN_TEST(test_system_name_tzolkin);
    RUN_TEST(test_system_name_human_design);
    RUN_TEST(test_system_name_kabbalah);
    RUN_TEST(test_system_name_invalid);

    /* Culture count/get */
    RUN_TEST(test_culture_count_at_least_10);
    RUN_TEST(test_culture_get_valid);
    RUN_TEST(test_culture_get_invalid);

    /* Edge count */
    RUN_TEST(test_edge_count);

    /* Influence consistency */
    RUN_TEST(test_influence_king_wen_to_wilhelm);
    RUN_TEST(test_influence_hipparchus_to_ptolemy);
    RUN_TEST(test_influence_kepler_has_two_predecessors);
    RUN_TEST(test_influence_al_khwarizmi_has_two_predecessors);

    /* Knowledge path */
    RUN_TEST(test_path_astrology_astronomy_bridge);
    RUN_TEST(test_path_iching_astronomy_no_connection);
    RUN_TEST(test_path_same_system);
    RUN_TEST(test_path_hindu_astronomy);
    RUN_TEST(test_path_kabbalah_astronomy_no_connection);
    RUN_TEST(test_path_hindu_gregorian);
    RUN_TEST(test_path_islamic_astronomy);
    RUN_TEST(test_path_length_bounded);

    /* Key work */
    RUN_TEST(test_key_work_king_wen);
    RUN_TEST(test_key_work_meeus);

    /* Sequential IDs */
    RUN_TEST(test_ids_sequential);

    return UNITY_END();
}
