#include "../unity/unity.h"
#include "../../src/render/constellation.h"
#include "../../src/render/star_catalog.h"

void setUp(void) {}
void tearDown(void) {}

/* 1. Has a reasonable number of constellations */
void test_count(void)
{
    int n = constellation_count();
    TEST_ASSERT_TRUE(n >= 35);
    TEST_ASSERT_TRUE(n <= 50);
}

/* 2. First constellation has a name */
void test_first_has_name(void)
{
    constellation_t c = constellation_get(0);
    TEST_ASSERT_NOT_NULL(c.name);
    TEST_ASSERT_TRUE(c.name[0] != '\0');
}

/* 3. All constellations have at least 1 line */
void test_all_have_lines(void)
{
    int n = constellation_count();
    for (int i = 0; i < n; i++) {
        constellation_t c = constellation_get(i);
        TEST_ASSERT_TRUE(c.line_count >= 1);
    }
}

/* 4. All line star indices are valid catalog entries */
void test_star_indices_valid(void)
{
    int n = constellation_count();
    int star_count = star_catalog_count();
    for (int i = 0; i < n; i++) {
        constellation_t c = constellation_get(i);
        for (int j = 0; j < c.line_count; j++) {
            TEST_ASSERT_TRUE(c.lines[j].star_a >= 0);
            TEST_ASSERT_TRUE(c.lines[j].star_a < star_count);
            TEST_ASSERT_TRUE(c.lines[j].star_b >= 0);
            TEST_ASSERT_TRUE(c.lines[j].star_b < star_count);
        }
    }
}

/* 5. No line connects a star to itself */
void test_no_self_lines(void)
{
    int n = constellation_count();
    for (int i = 0; i < n; i++) {
        constellation_t c = constellation_get(i);
        for (int j = 0; j < c.line_count; j++)
            TEST_ASSERT_TRUE(c.lines[j].star_a != c.lines[j].star_b);
    }
}

/* 6. Invalid index returns empty constellation */
void test_invalid_index(void)
{
    constellation_t c = constellation_get(-1);
    TEST_ASSERT_EQUAL_INT(0, c.line_count);
    TEST_ASSERT_NULL(c.name);
    TEST_ASSERT_NULL(c.abbr);

    c = constellation_get(9999);
    TEST_ASSERT_EQUAL_INT(0, c.line_count);
    TEST_ASSERT_NULL(c.name);
    TEST_ASSERT_NULL(c.abbr);
}

/* 7. Orion exists and has multiple lines */
void test_orion_exists(void)
{
    int idx = constellation_find("Orion");
    TEST_ASSERT_TRUE(idx >= 0);
    constellation_t c = constellation_get(idx);
    TEST_ASSERT_TRUE(c.line_count >= 5);
}

/* 8. Crux exists */
void test_crux_exists(void)
{
    int idx = constellation_find("Crux");
    TEST_ASSERT_TRUE(idx >= 0);
}

/* 9. Find returns -1 for unknown name */
void test_find_unknown(void)
{
    int idx = constellation_find("Nonexistent");
    TEST_ASSERT_EQUAL_INT(-1, idx);
}

/* 10. Find returns -1 for NULL */
void test_find_null(void)
{
    int idx = constellation_find((void *)0);
    TEST_ASSERT_EQUAL_INT(-1, idx);
}

/* 11. Total line count is sum of all constellations */
void test_total_lines(void)
{
    int n = constellation_count();
    int sum = 0;
    for (int i = 0; i < n; i++)
        sum += constellation_get(i).line_count;
    TEST_ASSERT_EQUAL_INT(sum, constellation_total_lines());
}

/* 12. Total lines is positive */
void test_total_lines_positive(void)
{
    TEST_ASSERT_TRUE(constellation_total_lines() > 0);
}

/* 13. Orion contains Betelgeuse (star 9) */
void test_orion_has_betelgeuse(void)
{
    int idx = constellation_find("Orion");
    constellation_t c = constellation_get(idx);
    int found = 0;
    for (int j = 0; j < c.line_count; j++) {
        if (c.lines[j].star_a == 9 || c.lines[j].star_b == 9) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

/* 14. Orion contains Rigel (star 6) */
void test_orion_has_rigel(void)
{
    int idx = constellation_find("Orion");
    constellation_t c = constellation_get(idx);
    int found = 0;
    for (int j = 0; j < c.line_count; j++) {
        if (c.lines[j].star_a == 6 || c.lines[j].star_b == 6) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

/* 15. Line count per constellation <= CONSTELLATION_MAX_LINES */
void test_line_count_limit(void)
{
    int n = constellation_count();
    for (int i = 0; i < n; i++) {
        constellation_t c = constellation_get(i);
        TEST_ASSERT_TRUE(c.line_count <= CONSTELLATION_MAX_LINES);
    }
}

/* 16. Gemini contains Castor and Pollux */
void test_gemini_stars(void)
{
    int idx = constellation_find("Gemini");
    TEST_ASSERT_TRUE(idx >= 0);
    constellation_t c = constellation_get(idx);
    int has_castor = 0, has_pollux = 0;
    for (int j = 0; j < c.line_count; j++) {
        if (c.lines[j].star_a == 22 || c.lines[j].star_b == 22) has_castor = 1;
        if (c.lines[j].star_a == 16 || c.lines[j].star_b == 16) has_pollux = 1;
    }
    TEST_ASSERT_TRUE(has_castor);
    TEST_ASSERT_TRUE(has_pollux);
}

/* 17. Canis Major contains Sirius */
void test_canis_major_sirius(void)
{
    int idx = constellation_find("Canis Major");
    TEST_ASSERT_TRUE(idx >= 0);
    constellation_t c = constellation_get(idx);
    int found = 0;
    for (int j = 0; j < c.line_count; j++) {
        if (c.lines[j].star_a == 0 || c.lines[j].star_b == 0) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

/* 18. Scorpius contains Antares */
void test_scorpius_antares(void)
{
    int idx = constellation_find("Scorpius");
    TEST_ASSERT_TRUE(idx >= 0);
    constellation_t c = constellation_get(idx);
    int found = 0;
    for (int j = 0; j < c.line_count; j++) {
        if (c.lines[j].star_a == 14 || c.lines[j].star_b == 14) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

/* 19. All constellation names are unique */
void test_unique_names(void)
{
    int n = constellation_count();
    for (int i = 0; i < n; i++) {
        constellation_t ci = constellation_get(i);
        for (int j = i + 1; j < n; j++) {
            constellation_t cj = constellation_get(j);
            int same = 1;
            const char *a = ci.name;
            const char *b = cj.name;
            while (*a && *b) {
                if (*a != *b) { same = 0; break; }
                a++; b++;
            }
            if (*a != *b) same = 0;
            TEST_ASSERT_FALSE(same);
        }
    }
}

/* 20. Zodiac lookup: 0 degrees -> Aries (index 0) */
void test_zodiac_aries(void)
{
    TEST_ASSERT_EQUAL_INT(0, constellation_zodiac_from_longitude(15.0f));
}

/* 21. Zodiac lookup: 180 degrees -> Libra (index 6) */
void test_zodiac_libra(void)
{
    TEST_ASSERT_EQUAL_INT(6, constellation_zodiac_from_longitude(195.0f));
}

/* 22. Zodiac lookup: wraps at 360 */
void test_zodiac_wrap(void)
{
    TEST_ASSERT_EQUAL_INT(0, constellation_zodiac_from_longitude(370.0f));
}

/* 23. Zodiac lookup: negative wraps */
void test_zodiac_negative(void)
{
    TEST_ASSERT_EQUAL_INT(11, constellation_zodiac_from_longitude(-15.0f));
}

/* 24. Purity */
void test_purity(void)
{
    constellation_t a = constellation_get(0);
    constellation_t b = constellation_get(0);
    TEST_ASSERT_EQUAL_INT(a.line_count, b.line_count);
    TEST_ASSERT_TRUE(a.name == b.name); /* same pointer */
    for (int i = 0; i < a.line_count; i++) {
        TEST_ASSERT_EQUAL_INT(a.lines[i].star_a, b.lines[i].star_a);
        TEST_ASSERT_EQUAL_INT(a.lines[i].star_b, b.lines[i].star_b);
    }
}

/* 25. All constellations have a non-empty abbreviation */
void test_all_have_abbr(void)
{
    int n = constellation_count();
    for (int i = 0; i < n; i++) {
        constellation_t c = constellation_get(i);
        TEST_ASSERT_NOT_NULL(c.abbr);
        TEST_ASSERT_TRUE(c.abbr[0] != '\0');
    }
}

/* 26. All abbreviations are 3 characters */
void test_abbr_length(void)
{
    int n = constellation_count();
    for (int i = 0; i < n; i++) {
        constellation_t c = constellation_get(i);
        int len = 0;
        const char *p = c.abbr;
        while (*p) { len++; p++; }
        TEST_ASSERT_EQUAL_INT(3, len);
    }
}

/* 27. constellation_abbr function works */
void test_abbr_function(void)
{
    int idx = constellation_find("Orion");
    const char *ab = constellation_abbr(idx);
    TEST_ASSERT_NOT_NULL(ab);
    TEST_ASSERT_TRUE(ab[0] == 'O' && ab[1] == 'r' && ab[2] == 'i');
}

/* 28. constellation_abbr returns NULL for invalid index */
void test_abbr_invalid(void)
{
    TEST_ASSERT_NULL(constellation_abbr(-1));
    TEST_ASSERT_NULL(constellation_abbr(9999));
}

/* 29. All abbreviations are unique */
void test_unique_abbrs(void)
{
    int n = constellation_count();
    for (int i = 0; i < n; i++) {
        const char *ai = constellation_abbr(i);
        for (int j = i + 1; j < n; j++) {
            const char *aj = constellation_abbr(j);
            int same = (ai[0] == aj[0] && ai[1] == aj[1] && ai[2] == aj[2]);
            TEST_ASSERT_FALSE(same);
        }
    }
}

/* 30. Cassiopeia exists with W-shape lines */
void test_cassiopeia_exists(void)
{
    int idx = constellation_find("Cassiopeia");
    TEST_ASSERT_TRUE(idx >= 0);
    constellation_t c = constellation_get(idx);
    TEST_ASSERT_TRUE(c.line_count >= 4);
}

/* 31. Ursa Major has Big Dipper lines */
void test_ursa_major_expanded(void)
{
    int idx = constellation_find("Ursa Major");
    TEST_ASSERT_TRUE(idx >= 0);
    constellation_t c = constellation_get(idx);
    TEST_ASSERT_TRUE(c.line_count >= 6);
}

/* 32. Leo exists with Regulus */
void test_leo_has_regulus(void)
{
    int idx = constellation_find("Leo");
    TEST_ASSERT_TRUE(idx >= 0);
    constellation_t c = constellation_get(idx);
    int found = 0;
    for (int j = 0; j < c.line_count; j++) {
        if (c.lines[j].star_a == 20 || c.lines[j].star_b == 20) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found);
}

/* 33. Pegasus exists */
void test_pegasus_exists(void)
{
    int idx = constellation_find("Pegasus");
    TEST_ASSERT_TRUE(idx >= 0);
    constellation_t c = constellation_get(idx);
    TEST_ASSERT_TRUE(c.line_count >= 4);
}

/* 34. Virgo exists with Spica */
void test_virgo_exists(void)
{
    int idx = constellation_find("Virgo");
    TEST_ASSERT_TRUE(idx >= 0);
    constellation_t c = constellation_get(idx);
    TEST_ASSERT_TRUE(c.line_count >= 2);
    int found = 0;
    for (int j = 0; j < c.line_count; j++) {
        if (c.lines[j].star_a == 15 || c.lines[j].star_b == 15) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found); /* Spica (index 15) */
}

/* 35. Cancer exists */
void test_cancer_exists(void)
{
    int idx = constellation_find("Cancer");
    TEST_ASSERT_TRUE(idx >= 0);
    constellation_t c = constellation_get(idx);
    TEST_ASSERT_TRUE(c.line_count >= 1);
}

/* 36. Hercules exists with Kornephoros */
void test_hercules_exists(void)
{
    int idx = constellation_find("Hercules");
    TEST_ASSERT_TRUE(idx >= 0);
    constellation_t c = constellation_get(idx);
    TEST_ASSERT_TRUE(c.line_count >= 3);
    int found = 0;
    for (int j = 0; j < c.line_count; j++) {
        if (c.lines[j].star_a == 89 || c.lines[j].star_b == 89) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found); /* Kornephoros (index 89) */
}

/* 37. Canis Minor exists with Procyon */
void test_canis_minor_exists(void)
{
    int idx = constellation_find("Canis Minor");
    TEST_ASSERT_TRUE(idx >= 0);
    constellation_t c = constellation_get(idx);
    TEST_ASSERT_TRUE(c.line_count >= 1);
    int found = 0;
    for (int j = 0; j < c.line_count; j++) {
        if (c.lines[j].star_a == 7 || c.lines[j].star_b == 7) {
            found = 1;
            break;
        }
    }
    TEST_ASSERT_TRUE(found); /* Procyon (index 7) */
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_count);
    RUN_TEST(test_first_has_name);
    RUN_TEST(test_all_have_lines);
    RUN_TEST(test_star_indices_valid);
    RUN_TEST(test_no_self_lines);
    RUN_TEST(test_invalid_index);
    RUN_TEST(test_orion_exists);
    RUN_TEST(test_crux_exists);
    RUN_TEST(test_find_unknown);
    RUN_TEST(test_find_null);
    RUN_TEST(test_total_lines);
    RUN_TEST(test_total_lines_positive);
    RUN_TEST(test_orion_has_betelgeuse);
    RUN_TEST(test_orion_has_rigel);
    RUN_TEST(test_line_count_limit);
    RUN_TEST(test_gemini_stars);
    RUN_TEST(test_canis_major_sirius);
    RUN_TEST(test_scorpius_antares);
    RUN_TEST(test_unique_names);
    RUN_TEST(test_zodiac_aries);
    RUN_TEST(test_zodiac_libra);
    RUN_TEST(test_zodiac_wrap);
    RUN_TEST(test_zodiac_negative);
    RUN_TEST(test_purity);
    RUN_TEST(test_all_have_abbr);
    RUN_TEST(test_abbr_length);
    RUN_TEST(test_abbr_function);
    RUN_TEST(test_abbr_invalid);
    RUN_TEST(test_unique_abbrs);
    RUN_TEST(test_cassiopeia_exists);
    RUN_TEST(test_ursa_major_expanded);
    RUN_TEST(test_leo_has_regulus);
    RUN_TEST(test_pegasus_exists);
    RUN_TEST(test_virgo_exists);
    RUN_TEST(test_cancer_exists);
    RUN_TEST(test_hercules_exists);
    RUN_TEST(test_canis_minor_exists);
    return UNITY_END();
}
