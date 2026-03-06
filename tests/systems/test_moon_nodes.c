#include "../unity/unity.h"
#include "../../src/systems/astronomy/moon_nodes.h"

#include <math.h>

void setUp(void) {}
void tearDown(void) {}

#define J2000_JD 2451545.0

/* Mean node at J2000: 1-3 */

void test_mean_node_j2000_reference(void)
{
    /* At J2000 (2000-01-01.5), mean node should be near 125.04° (Meeus formula constant) */
    lunar_nodes_t n = lunar_nodes_mean(J2000_JD);
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 125.04f, (float)n.north_node);
}

void test_mean_node_j2000_sign(void)
{
    lunar_nodes_t n = lunar_nodes_mean(J2000_JD);
    /* 125° / 30 = sign 4 (Leo) */
    TEST_ASSERT_EQUAL_INT(4, n.north_sign);
}

void test_mean_node_j2000_degree(void)
{
    lunar_nodes_t n = lunar_nodes_mean(J2000_JD);
    /* 125.04 - 4*30 = 5.04° in Leo */
    TEST_ASSERT_FLOAT_WITHIN(0.5f, 5.04f, (float)n.north_degree);
}

/* North/South always 180° apart: 4-5 */

void test_north_south_180_apart_j2000(void)
{
    lunar_nodes_t n = lunar_nodes_mean(J2000_JD);
    double diff = fabs(n.north_node - n.south_node);
    if (diff > 180.0) diff = 360.0 - diff;
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 180.0f, (float)diff);
}

void test_north_south_180_apart_other(void)
{
    lunar_nodes_t n = lunar_nodes_mean(2460000.0);
    double diff = fabs(n.north_node - n.south_node);
    if (diff > 180.0) diff = 360.0 - diff;
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 180.0f, (float)diff);
}

/* Retrograde motion: 6-7 */

void test_retrograde_motion(void)
{
    /* Node moves ~19.355°/year retrograde: longitude should decrease over time */
    lunar_nodes_t now = lunar_nodes_mean(J2000_JD);
    lunar_nodes_t later = lunar_nodes_mean(J2000_JD + 365.25);
    /* Accounting for wrap-around: the difference should be about -19.3° */
    double delta = later.north_node - now.north_node;
    if (delta > 180.0) delta -= 360.0;
    if (delta < -180.0) delta += 360.0;
    TEST_ASSERT_TRUE(delta < 0.0);
    TEST_ASSERT_FLOAT_WITHIN(2.0f, -19.35f, (float)delta);
}

void test_retrograde_over_multiple_years(void)
{
    /* Over 18.6 years, should complete one full cycle (~360°) */
    lunar_nodes_t start = lunar_nodes_mean(J2000_JD);
    lunar_nodes_t end = lunar_nodes_mean(J2000_JD + 6798.383);
    /* Should be back near the starting position */
    double diff = fabs(start.north_node - end.north_node);
    if (diff > 180.0) diff = 360.0 - diff;
    TEST_ASSERT_FLOAT_WITHIN(5.0f, 0.0f, (float)diff);
}

/* Sign assignment: 8-9 */

void test_sign_range_valid(void)
{
    for (double jd = J2000_JD; jd < J2000_JD + 7000.0; jd += 500.0) {
        lunar_nodes_t n = lunar_nodes_mean(jd);
        TEST_ASSERT_TRUE(n.north_sign >= 0 && n.north_sign <= 11);
        TEST_ASSERT_TRUE(n.south_sign >= 0 && n.south_sign <= 11);
    }
}

void test_degree_range_valid(void)
{
    for (double jd = J2000_JD; jd < J2000_JD + 7000.0; jd += 500.0) {
        lunar_nodes_t n = lunar_nodes_mean(jd);
        TEST_ASSERT_TRUE(n.north_degree >= 0.0 && n.north_degree < 30.0);
        TEST_ASSERT_TRUE(n.south_degree >= 0.0 && n.south_degree < 30.0);
    }
}

/* Longitude range: 10 */

void test_longitude_range(void)
{
    for (double jd = J2000_JD - 3000.0; jd < J2000_JD + 10000.0; jd += 700.0) {
        lunar_nodes_t n = lunar_nodes_mean(jd);
        TEST_ASSERT_TRUE(n.north_node >= 0.0 && n.north_node < 360.0);
        TEST_ASSERT_TRUE(n.south_node >= 0.0 && n.south_node < 360.0);
    }
}

/* True node: 11-13 */

void test_true_node_close_to_mean(void)
{
    /* True node should be within ~2° of mean node */
    lunar_nodes_t mean = lunar_nodes_mean(J2000_JD);
    lunar_nodes_t true_n = lunar_nodes_true(J2000_JD);
    double diff = fabs(mean.north_node - true_n.north_node);
    if (diff > 180.0) diff = 360.0 - diff;
    TEST_ASSERT_TRUE(diff < 3.0);
}

void test_true_north_south_180_apart(void)
{
    lunar_nodes_t n = lunar_nodes_true(J2000_JD);
    double diff = fabs(n.north_node - n.south_node);
    if (diff > 180.0) diff = 360.0 - diff;
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 180.0f, (float)diff);
}

void test_true_node_different_from_mean(void)
{
    /* True node should not be exactly equal to mean (correction applied) */
    lunar_nodes_t mean = lunar_nodes_mean(J2000_JD);
    lunar_nodes_t true_n = lunar_nodes_true(J2000_JD);
    TEST_ASSERT_TRUE(fabs(mean.north_node - true_n.north_node) > 0.001);
}

/* Names and symbols: 14-17 */

void test_north_name(void)
{
    TEST_ASSERT_EQUAL_STRING("North Node", lunar_node_north_name());
}

void test_south_name(void)
{
    TEST_ASSERT_EQUAL_STRING("South Node", lunar_node_south_name());
}

void test_north_symbol(void)
{
    const char *s = lunar_node_north_symbol();
    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_TRUE(s[0] != '\0');
}

void test_south_symbol(void)
{
    const char *s = lunar_node_south_symbol();
    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_TRUE(s[0] != '\0');
}

/* Proximity: 18-19 */

void test_proximity_near_north(void)
{
    lunar_nodes_t n = lunar_nodes_mean(J2000_JD);
    /* Longitude exactly at north node should detect proximity */
    int result = lunar_node_proximity(n.north_node, 5.0, J2000_JD);
    TEST_ASSERT_EQUAL_INT(1, result);
}

void test_proximity_near_south(void)
{
    lunar_nodes_t n = lunar_nodes_mean(J2000_JD);
    int result = lunar_node_proximity(n.south_node, 5.0, J2000_JD);
    TEST_ASSERT_EQUAL_INT(2, result);
}

void test_proximity_not_near(void)
{
    lunar_nodes_t n = lunar_nodes_mean(J2000_JD);
    /* 90° away from north node should not be near either node */
    double far = n.north_node + 90.0;
    if (far >= 360.0) far -= 360.0;
    int result = lunar_node_proximity(far, 5.0, J2000_JD);
    TEST_ASSERT_EQUAL_INT(0, result);
}

/* Nodal period: 20 */

void test_nodal_period(void)
{
    TEST_ASSERT_FLOAT_WITHIN(1.0f, 6798.383f, (float)lunar_nodal_period());
}

/* Purity: 21 */

void test_purity(void)
{
    lunar_nodes_t a = lunar_nodes_mean(J2000_JD);
    lunar_nodes_t b = lunar_nodes_mean(J2000_JD);
    TEST_ASSERT_EQUAL_FLOAT((float)a.north_node, (float)b.north_node);
    TEST_ASSERT_EQUAL_FLOAT((float)a.south_node, (float)b.south_node);
    TEST_ASSERT_EQUAL_INT(a.north_sign, b.north_sign);

    lunar_nodes_t c = lunar_nodes_true(2460000.0);
    lunar_nodes_t d = lunar_nodes_true(2460000.0);
    TEST_ASSERT_EQUAL_FLOAT((float)c.north_node, (float)d.north_node);
}

/* South sign opposite: 22 */

void test_opposite_signs(void)
{
    /* North and south signs should be ~6 signs apart */
    lunar_nodes_t n = lunar_nodes_mean(J2000_JD);
    int diff = (n.south_sign - n.north_sign + 12) % 12;
    TEST_ASSERT_EQUAL_INT(6, diff);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_mean_node_j2000_reference);
    RUN_TEST(test_mean_node_j2000_sign);
    RUN_TEST(test_mean_node_j2000_degree);
    RUN_TEST(test_north_south_180_apart_j2000);
    RUN_TEST(test_north_south_180_apart_other);
    RUN_TEST(test_retrograde_motion);
    RUN_TEST(test_retrograde_over_multiple_years);
    RUN_TEST(test_sign_range_valid);
    RUN_TEST(test_degree_range_valid);
    RUN_TEST(test_longitude_range);
    RUN_TEST(test_true_node_close_to_mean);
    RUN_TEST(test_true_north_south_180_apart);
    RUN_TEST(test_true_node_different_from_mean);
    RUN_TEST(test_north_name);
    RUN_TEST(test_south_name);
    RUN_TEST(test_north_symbol);
    RUN_TEST(test_south_symbol);
    RUN_TEST(test_proximity_near_north);
    RUN_TEST(test_proximity_near_south);
    RUN_TEST(test_proximity_not_near);
    RUN_TEST(test_nodal_period);
    RUN_TEST(test_purity);
    RUN_TEST(test_opposite_signs);
    return UNITY_END();
}
