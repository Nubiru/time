#include "../../unity/unity.h"
#include "../../../src/systems/tzolkin/telektonon_cards.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

/* ================================================================
 * Book count & total cards
 * ================================================================ */

static void test_book_count_is_4(void)
{
    TEST_ASSERT_EQUAL_INT(4, tk_book_count());
}

static void test_total_cards_is_52(void)
{
    TEST_ASSERT_EQUAL_INT(52, tk_total_cards());
}

/* Verify sum of book card counts equals 52 */
static void test_book_card_counts_sum_to_52(void)
{
    int sum = 0;
    for (int i = 0; i < 4; i++) {
        sum += tk_book_card_count((tk_book_t)i);
    }
    TEST_ASSERT_EQUAL_INT(52, sum);
}

/* ================================================================
 * Book 1: Lost Book of the Seven Generations — 7 cards, Root
 * ================================================================ */

static void test_book1_id(void)
{
    tk_book_info_t b = tk_book_get(TK_BOOK_SEVEN_GENERATIONS);
    TEST_ASSERT_EQUAL_INT(TK_BOOK_SEVEN_GENERATIONS, b.id);
}

static void test_book1_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Lost Book of the Seven Generations",
                             tk_book_name(TK_BOOK_SEVEN_GENERATIONS));
}

static void test_book1_card_count(void)
{
    TEST_ASSERT_EQUAL_INT(7, tk_book_card_count(TK_BOOK_SEVEN_GENERATIONS));
}

static void test_book1_position(void)
{
    tk_book_info_t b = tk_book_get(TK_BOOK_SEVEN_GENERATIONS);
    TEST_ASSERT_EQUAL_STRING("Root", b.position_name);
}

/* ================================================================
 * Book 2: Book of Galactic Time — 13 cards, Galactic Wing
 * ================================================================ */

static void test_book2_id(void)
{
    tk_book_info_t b = tk_book_get(TK_BOOK_GALACTIC_TIME);
    TEST_ASSERT_EQUAL_INT(TK_BOOK_GALACTIC_TIME, b.id);
}

static void test_book2_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Book of Galactic Time",
                             tk_book_name(TK_BOOK_GALACTIC_TIME));
}

static void test_book2_card_count(void)
{
    TEST_ASSERT_EQUAL_INT(13, tk_book_card_count(TK_BOOK_GALACTIC_TIME));
}

static void test_book2_position(void)
{
    tk_book_info_t b = tk_book_get(TK_BOOK_GALACTIC_TIME);
    TEST_ASSERT_EQUAL_STRING("Galactic Wing", b.position_name);
}

/* ================================================================
 * Book 3: Book of Cosmic Form — 4 cards, Solar Wing
 * ================================================================ */

static void test_book3_id(void)
{
    tk_book_info_t b = tk_book_get(TK_BOOK_COSMIC_FORM);
    TEST_ASSERT_EQUAL_INT(TK_BOOK_COSMIC_FORM, b.id);
}

static void test_book3_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Book of Cosmic Form",
                             tk_book_name(TK_BOOK_COSMIC_FORM));
}

static void test_book3_card_count(void)
{
    TEST_ASSERT_EQUAL_INT(4, tk_book_card_count(TK_BOOK_COSMIC_FORM));
}

static void test_book3_position(void)
{
    tk_book_info_t b = tk_book_get(TK_BOOK_COSMIC_FORM);
    TEST_ASSERT_EQUAL_STRING("Solar Wing", b.position_name);
}

/* ================================================================
 * Book 4: Telepathic Book of Planetary Redemption — 28 cards
 * ================================================================ */

static void test_book4_id(void)
{
    tk_book_info_t b = tk_book_get(TK_BOOK_PLANETARY_REDEMPTION);
    TEST_ASSERT_EQUAL_INT(TK_BOOK_PLANETARY_REDEMPTION, b.id);
}

static void test_book4_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Telepathic Book of Planetary Redemption",
                             tk_book_name(TK_BOOK_PLANETARY_REDEMPTION));
}

static void test_book4_card_count(void)
{
    TEST_ASSERT_EQUAL_INT(28, tk_book_card_count(TK_BOOK_PLANETARY_REDEMPTION));
}

static void test_book4_position(void)
{
    tk_book_info_t b = tk_book_get(TK_BOOK_PLANETARY_REDEMPTION);
    TEST_ASSERT_EQUAL_STRING("Crown / Tower", b.position_name);
}

/* ================================================================
 * Book boundaries — invalid enum values
 * ================================================================ */

static void test_book_get_negative_returns_zeroed(void)
{
    tk_book_info_t b = tk_book_get((tk_book_t)-1);
    TEST_ASSERT_EQUAL_INT(0, b.card_count);
    TEST_ASSERT_NULL(b.name);
    TEST_ASSERT_NULL(b.position_name);
}

static void test_book_get_4_returns_zeroed(void)
{
    tk_book_info_t b = tk_book_get((tk_book_t)4);
    TEST_ASSERT_EQUAL_INT(0, b.card_count);
    TEST_ASSERT_NULL(b.name);
    TEST_ASSERT_NULL(b.position_name);
}

static void test_book_get_99_returns_zeroed(void)
{
    tk_book_info_t b = tk_book_get((tk_book_t)99);
    TEST_ASSERT_EQUAL_INT(0, b.card_count);
    TEST_ASSERT_NULL(b.name);
}

static void test_book_name_negative_returns_null(void)
{
    TEST_ASSERT_NULL(tk_book_name((tk_book_t)-1));
}

static void test_book_name_4_returns_null(void)
{
    TEST_ASSERT_NULL(tk_book_name((tk_book_t)4));
}

static void test_book_card_count_negative_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_book_card_count((tk_book_t)-1));
}

static void test_book_card_count_4_returns_0(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_book_card_count((tk_book_t)4));
}

/* ================================================================
 * Position count
 * ================================================================ */

static void test_position_count_is_7(void)
{
    TEST_ASSERT_EQUAL_INT(7, tk_position_count());
}

/* ================================================================
 * Position 0: Tower — Book 4, Days 1,6,23,28 only
 * ================================================================ */

static void test_pos0_name(void)
{
    tk_card_position_t p = tk_position_get(0);
    TEST_ASSERT_EQUAL_STRING("Tower", p.name);
}

static void test_pos0_name_es(void)
{
    tk_card_position_t p = tk_position_get(0);
    TEST_ASSERT_EQUAL_STRING("Torre", p.name_es);
}

static void test_pos0_source_book(void)
{
    tk_card_position_t p = tk_position_get(0);
    TEST_ASSERT_EQUAL_INT(TK_BOOK_PLANETARY_REDEMPTION, p.source_book);
}

static void test_pos0_changes(void)
{
    tk_card_position_t p = tk_position_get(0);
    TEST_ASSERT_EQUAL_STRING("Days 1,6,23,28 only", p.changes);
}

/* ================================================================
 * Position 1: Crown — Book 4, Daily
 * ================================================================ */

static void test_pos1_name(void)
{
    tk_card_position_t p = tk_position_get(1);
    TEST_ASSERT_EQUAL_STRING("Crown", p.name);
}

static void test_pos1_name_es(void)
{
    tk_card_position_t p = tk_position_get(1);
    TEST_ASSERT_EQUAL_STRING("Corona", p.name_es);
}

static void test_pos1_source_book(void)
{
    tk_card_position_t p = tk_position_get(1);
    TEST_ASSERT_EQUAL_INT(TK_BOOK_PLANETARY_REDEMPTION, p.source_book);
}

static void test_pos1_changes(void)
{
    tk_card_position_t p = tk_position_get(1);
    TEST_ASSERT_EQUAL_STRING("Daily", p.changes);
}

/* ================================================================
 * Position 2: Heart — Book 2, Monthly
 * ================================================================ */

static void test_pos2_name(void)
{
    tk_card_position_t p = tk_position_get(2);
    TEST_ASSERT_EQUAL_STRING("Heart", p.name);
}

static void test_pos2_name_es(void)
{
    tk_card_position_t p = tk_position_get(2);
    TEST_ASSERT_EQUAL_STRING("Corazon", p.name_es);
}

static void test_pos2_source_book(void)
{
    tk_card_position_t p = tk_position_get(2);
    TEST_ASSERT_EQUAL_INT(TK_BOOK_GALACTIC_TIME, p.source_book);
}

static void test_pos2_changes(void)
{
    tk_card_position_t p = tk_position_get(2);
    TEST_ASSERT_EQUAL_STRING("Monthly", p.changes);
}

/* ================================================================
 * Position 3: Galactic Wing — Book 2, Daily
 * ================================================================ */

static void test_pos3_name(void)
{
    tk_card_position_t p = tk_position_get(3);
    TEST_ASSERT_EQUAL_STRING("Galactic Wing", p.name);
}

static void test_pos3_name_es(void)
{
    tk_card_position_t p = tk_position_get(3);
    TEST_ASSERT_EQUAL_STRING("Ala Galactica", p.name_es);
}

static void test_pos3_source_book(void)
{
    tk_card_position_t p = tk_position_get(3);
    TEST_ASSERT_EQUAL_INT(TK_BOOK_GALACTIC_TIME, p.source_book);
}

static void test_pos3_changes(void)
{
    tk_card_position_t p = tk_position_get(3);
    TEST_ASSERT_EQUAL_STRING("Daily", p.changes);
}

/* ================================================================
 * Position 4: Solar Wing Weekly — Book 3, Weekly
 * ================================================================ */

static void test_pos4_name(void)
{
    tk_card_position_t p = tk_position_get(4);
    TEST_ASSERT_EQUAL_STRING("Solar Wing Weekly", p.name);
}

static void test_pos4_name_es(void)
{
    tk_card_position_t p = tk_position_get(4);
    TEST_ASSERT_EQUAL_STRING("Ala Solar Semanal", p.name_es);
}

static void test_pos4_source_book(void)
{
    tk_card_position_t p = tk_position_get(4);
    TEST_ASSERT_EQUAL_INT(TK_BOOK_COSMIC_FORM, p.source_book);
}

static void test_pos4_changes(void)
{
    tk_card_position_t p = tk_position_get(4);
    TEST_ASSERT_EQUAL_STRING("Weekly", p.changes);
}

/* ================================================================
 * Position 5: Solar Wing Daily — Book 3, Daily
 * ================================================================ */

static void test_pos5_name(void)
{
    tk_card_position_t p = tk_position_get(5);
    TEST_ASSERT_EQUAL_STRING("Solar Wing Daily", p.name);
}

static void test_pos5_name_es(void)
{
    tk_card_position_t p = tk_position_get(5);
    TEST_ASSERT_EQUAL_STRING("Ala Solar Diaria", p.name_es);
}

static void test_pos5_source_book(void)
{
    tk_card_position_t p = tk_position_get(5);
    TEST_ASSERT_EQUAL_INT(TK_BOOK_COSMIC_FORM, p.source_book);
}

static void test_pos5_changes(void)
{
    tk_card_position_t p = tk_position_get(5);
    TEST_ASSERT_EQUAL_STRING("Daily", p.changes);
}

/* ================================================================
 * Position 6: Root — Book 1, Annually
 * ================================================================ */

static void test_pos6_name(void)
{
    tk_card_position_t p = tk_position_get(6);
    TEST_ASSERT_EQUAL_STRING("Root", p.name);
}

static void test_pos6_name_es(void)
{
    tk_card_position_t p = tk_position_get(6);
    TEST_ASSERT_EQUAL_STRING("Raiz", p.name_es);
}

static void test_pos6_source_book(void)
{
    tk_card_position_t p = tk_position_get(6);
    TEST_ASSERT_EQUAL_INT(TK_BOOK_SEVEN_GENERATIONS, p.source_book);
}

static void test_pos6_changes(void)
{
    tk_card_position_t p = tk_position_get(6);
    TEST_ASSERT_EQUAL_STRING("Annually", p.changes);
}

/* ================================================================
 * Position boundaries — invalid indices
 * ================================================================ */

static void test_position_get_negative_returns_zeroed(void)
{
    tk_card_position_t p = tk_position_get(-1);
    TEST_ASSERT_NULL(p.name);
    TEST_ASSERT_NULL(p.name_es);
    TEST_ASSERT_NULL(p.changes);
}

static void test_position_get_7_returns_zeroed(void)
{
    tk_card_position_t p = tk_position_get(7);
    TEST_ASSERT_NULL(p.name);
    TEST_ASSERT_NULL(p.name_es);
    TEST_ASSERT_NULL(p.changes);
}

static void test_position_get_99_returns_zeroed(void)
{
    tk_card_position_t p = tk_position_get(99);
    TEST_ASSERT_NULL(p.name);
}

/* ================================================================
 * Position name function
 * ================================================================ */

static void test_position_name_tower(void)
{
    TEST_ASSERT_EQUAL_STRING("Tower", tk_position_name(TK_POS_TOWER));
}

static void test_position_name_crown(void)
{
    TEST_ASSERT_EQUAL_STRING("Crown", tk_position_name(TK_POS_CROWN));
}

static void test_position_name_heart(void)
{
    TEST_ASSERT_EQUAL_STRING("Heart", tk_position_name(TK_POS_HEART));
}

static void test_position_name_galactic_wing(void)
{
    TEST_ASSERT_EQUAL_STRING("Galactic Wing",
                             tk_position_name(TK_POS_GALACTIC_WING));
}

static void test_position_name_solar_wing_weekly(void)
{
    TEST_ASSERT_EQUAL_STRING("Solar Wing Weekly",
                             tk_position_name(TK_POS_SOLAR_WING_WEEKLY));
}

static void test_position_name_solar_wing_daily(void)
{
    TEST_ASSERT_EQUAL_STRING("Solar Wing Daily",
                             tk_position_name(TK_POS_SOLAR_WING_DAILY));
}

static void test_position_name_root(void)
{
    TEST_ASSERT_EQUAL_STRING("Root", tk_position_name(TK_POS_ROOT));
}

static void test_position_name_negative_returns_null(void)
{
    TEST_ASSERT_NULL(tk_position_name((tk_position_t)-1));
}

static void test_position_name_7_returns_null(void)
{
    TEST_ASSERT_NULL(tk_position_name((tk_position_t)7));
}

/* ================================================================
 * Tower active — days 1, 6, 23, 28
 * ================================================================ */

static void test_tower_active_day1(void)
{
    TEST_ASSERT_EQUAL_INT(1, tk_is_tower_active(1));
}

static void test_tower_active_day6(void)
{
    TEST_ASSERT_EQUAL_INT(1, tk_is_tower_active(6));
}

static void test_tower_active_day23(void)
{
    TEST_ASSERT_EQUAL_INT(1, tk_is_tower_active(23));
}

static void test_tower_active_day28(void)
{
    TEST_ASSERT_EQUAL_INT(1, tk_is_tower_active(28));
}

static void test_tower_inactive_day2(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_is_tower_active(2));
}

static void test_tower_inactive_day5(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_is_tower_active(5));
}

static void test_tower_inactive_day7(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_is_tower_active(7));
}

static void test_tower_inactive_day14(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_is_tower_active(14));
}

static void test_tower_inactive_day22(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_is_tower_active(22));
}

static void test_tower_inactive_day27(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_is_tower_active(27));
}

static void test_tower_inactive_day0(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_is_tower_active(0));
}

static void test_tower_inactive_day29(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_is_tower_active(29));
}

static void test_tower_inactive_negative(void)
{
    TEST_ASSERT_EQUAL_INT(0, tk_is_tower_active(-1));
}

/* Sweep: exactly 4 tower-active days in a 28-day moon */
static void test_tower_active_sweep_count(void)
{
    int count = 0;
    for (int d = 1; d <= 28; d++) {
        count += tk_is_tower_active(d);
    }
    TEST_ASSERT_EQUAL_INT(4, count);
}

/* ================================================================
 * Purity — same input, same output
 * ================================================================ */

static void test_purity_book_get_same_result(void)
{
    tk_book_info_t a = tk_book_get(TK_BOOK_GALACTIC_TIME);
    tk_book_info_t b = tk_book_get(TK_BOOK_GALACTIC_TIME);
    TEST_ASSERT_EQUAL_INT(a.id, b.id);
    TEST_ASSERT_EQUAL_INT(a.card_count, b.card_count);
    TEST_ASSERT_EQUAL_STRING(a.name, b.name);
    TEST_ASSERT_EQUAL_STRING(a.position_name, b.position_name);
}

static void test_purity_position_get_same_result(void)
{
    tk_card_position_t a = tk_position_get(3);
    tk_card_position_t b = tk_position_get(3);
    TEST_ASSERT_EQUAL_INT(a.position, b.position);
    TEST_ASSERT_EQUAL_STRING(a.name, b.name);
    TEST_ASSERT_EQUAL_STRING(a.name_es, b.name_es);
    TEST_ASSERT_EQUAL_INT(a.source_book, b.source_book);
    TEST_ASSERT_EQUAL_STRING(a.changes, b.changes);
}

/* ================================================================
 * Sweeps: all books have valid strings
 * ================================================================ */

static void test_all_books_have_valid_data(void)
{
    for (int i = 0; i < 4; i++) {
        tk_book_info_t b = tk_book_get((tk_book_t)i);
        TEST_ASSERT_EQUAL_INT(i, (int)b.id);
        TEST_ASSERT_NOT_NULL(b.name);
        TEST_ASSERT_NOT_NULL(b.position_name);
        TEST_ASSERT_TRUE(b.card_count > 0);
        TEST_ASSERT_TRUE(strlen(b.name) > 0);
        TEST_ASSERT_TRUE(strlen(b.position_name) > 0);
    }
}

/* ================================================================
 * Sweeps: all positions have valid strings
 * ================================================================ */

static void test_all_positions_have_valid_data(void)
{
    for (int i = 0; i < 7; i++) {
        tk_card_position_t p = tk_position_get(i);
        TEST_ASSERT_EQUAL_INT(i, (int)p.position);
        TEST_ASSERT_NOT_NULL(p.name);
        TEST_ASSERT_NOT_NULL(p.name_es);
        TEST_ASSERT_NOT_NULL(p.changes);
        TEST_ASSERT_TRUE(strlen(p.name) > 0);
        TEST_ASSERT_TRUE(strlen(p.name_es) > 0);
        TEST_ASSERT_TRUE(strlen(p.changes) > 0);
    }
}

/* ================================================================
 * Cross-checks: position enum matches position_get index
 * ================================================================ */

static void test_position_enum_matches_index(void)
{
    TEST_ASSERT_EQUAL_INT(TK_POS_TOWER,             tk_position_get(0).position);
    TEST_ASSERT_EQUAL_INT(TK_POS_CROWN,             tk_position_get(1).position);
    TEST_ASSERT_EQUAL_INT(TK_POS_HEART,             tk_position_get(2).position);
    TEST_ASSERT_EQUAL_INT(TK_POS_GALACTIC_WING,     tk_position_get(3).position);
    TEST_ASSERT_EQUAL_INT(TK_POS_SOLAR_WING_WEEKLY, tk_position_get(4).position);
    TEST_ASSERT_EQUAL_INT(TK_POS_SOLAR_WING_DAILY,  tk_position_get(5).position);
    TEST_ASSERT_EQUAL_INT(TK_POS_ROOT,              tk_position_get(6).position);
}

/* ================================================================
 * Cross-checks: tower position sources from Book 4
 * ================================================================ */

static void test_tower_and_crown_source_book4(void)
{
    TEST_ASSERT_EQUAL_INT(TK_BOOK_PLANETARY_REDEMPTION,
                          tk_position_get(0).source_book);
    TEST_ASSERT_EQUAL_INT(TK_BOOK_PLANETARY_REDEMPTION,
                          tk_position_get(1).source_book);
}

/* ================================================================
 * Cross-checks: book_name matches book_get name
 * ================================================================ */

static void test_book_name_matches_book_get(void)
{
    for (int i = 0; i < 4; i++) {
        tk_book_info_t b = tk_book_get((tk_book_t)i);
        const char *name = tk_book_name((tk_book_t)i);
        TEST_ASSERT_EQUAL_STRING(b.name, name);
    }
}

/* ================================================================
 * Cross-checks: book_card_count matches book_get card_count
 * ================================================================ */

static void test_book_card_count_matches_book_get(void)
{
    for (int i = 0; i < 4; i++) {
        tk_book_info_t b = tk_book_get((tk_book_t)i);
        int count = tk_book_card_count((tk_book_t)i);
        TEST_ASSERT_EQUAL_INT(b.card_count, count);
    }
}

/* ================================================================
 * Largest book has 28 cards (Book 4)
 * ================================================================ */

static void test_largest_book_is_planetary_redemption(void)
{
    int max_count = 0;
    tk_book_t max_book = TK_BOOK_SEVEN_GENERATIONS;
    for (int i = 0; i < 4; i++) {
        int c = tk_book_card_count((tk_book_t)i);
        if (c > max_count) {
            max_count = c;
            max_book = (tk_book_t)i;
        }
    }
    TEST_ASSERT_EQUAL_INT(28, max_count);
    TEST_ASSERT_EQUAL_INT(TK_BOOK_PLANETARY_REDEMPTION, max_book);
}

/* ================================================================
 * main
 * ================================================================ */

int main(void)
{
    UNITY_BEGIN();

    /* Counts & totals */
    RUN_TEST(test_book_count_is_4);
    RUN_TEST(test_total_cards_is_52);
    RUN_TEST(test_book_card_counts_sum_to_52);

    /* Book 1 — Seven Generations */
    RUN_TEST(test_book1_id);
    RUN_TEST(test_book1_name);
    RUN_TEST(test_book1_card_count);
    RUN_TEST(test_book1_position);

    /* Book 2 — Galactic Time */
    RUN_TEST(test_book2_id);
    RUN_TEST(test_book2_name);
    RUN_TEST(test_book2_card_count);
    RUN_TEST(test_book2_position);

    /* Book 3 — Cosmic Form */
    RUN_TEST(test_book3_id);
    RUN_TEST(test_book3_name);
    RUN_TEST(test_book3_card_count);
    RUN_TEST(test_book3_position);

    /* Book 4 — Planetary Redemption */
    RUN_TEST(test_book4_id);
    RUN_TEST(test_book4_name);
    RUN_TEST(test_book4_card_count);
    RUN_TEST(test_book4_position);

    /* Book boundaries */
    RUN_TEST(test_book_get_negative_returns_zeroed);
    RUN_TEST(test_book_get_4_returns_zeroed);
    RUN_TEST(test_book_get_99_returns_zeroed);
    RUN_TEST(test_book_name_negative_returns_null);
    RUN_TEST(test_book_name_4_returns_null);
    RUN_TEST(test_book_card_count_negative_returns_0);
    RUN_TEST(test_book_card_count_4_returns_0);

    /* Position count */
    RUN_TEST(test_position_count_is_7);

    /* Position 0 — Tower */
    RUN_TEST(test_pos0_name);
    RUN_TEST(test_pos0_name_es);
    RUN_TEST(test_pos0_source_book);
    RUN_TEST(test_pos0_changes);

    /* Position 1 — Crown */
    RUN_TEST(test_pos1_name);
    RUN_TEST(test_pos1_name_es);
    RUN_TEST(test_pos1_source_book);
    RUN_TEST(test_pos1_changes);

    /* Position 2 — Heart */
    RUN_TEST(test_pos2_name);
    RUN_TEST(test_pos2_name_es);
    RUN_TEST(test_pos2_source_book);
    RUN_TEST(test_pos2_changes);

    /* Position 3 — Galactic Wing */
    RUN_TEST(test_pos3_name);
    RUN_TEST(test_pos3_name_es);
    RUN_TEST(test_pos3_source_book);
    RUN_TEST(test_pos3_changes);

    /* Position 4 — Solar Wing Weekly */
    RUN_TEST(test_pos4_name);
    RUN_TEST(test_pos4_name_es);
    RUN_TEST(test_pos4_source_book);
    RUN_TEST(test_pos4_changes);

    /* Position 5 — Solar Wing Daily */
    RUN_TEST(test_pos5_name);
    RUN_TEST(test_pos5_name_es);
    RUN_TEST(test_pos5_source_book);
    RUN_TEST(test_pos5_changes);

    /* Position 6 — Root */
    RUN_TEST(test_pos6_name);
    RUN_TEST(test_pos6_name_es);
    RUN_TEST(test_pos6_source_book);
    RUN_TEST(test_pos6_changes);

    /* Position boundaries */
    RUN_TEST(test_position_get_negative_returns_zeroed);
    RUN_TEST(test_position_get_7_returns_zeroed);
    RUN_TEST(test_position_get_99_returns_zeroed);

    /* Position name function */
    RUN_TEST(test_position_name_tower);
    RUN_TEST(test_position_name_crown);
    RUN_TEST(test_position_name_heart);
    RUN_TEST(test_position_name_galactic_wing);
    RUN_TEST(test_position_name_solar_wing_weekly);
    RUN_TEST(test_position_name_solar_wing_daily);
    RUN_TEST(test_position_name_root);
    RUN_TEST(test_position_name_negative_returns_null);
    RUN_TEST(test_position_name_7_returns_null);

    /* Tower active */
    RUN_TEST(test_tower_active_day1);
    RUN_TEST(test_tower_active_day6);
    RUN_TEST(test_tower_active_day23);
    RUN_TEST(test_tower_active_day28);
    RUN_TEST(test_tower_inactive_day2);
    RUN_TEST(test_tower_inactive_day5);
    RUN_TEST(test_tower_inactive_day7);
    RUN_TEST(test_tower_inactive_day14);
    RUN_TEST(test_tower_inactive_day22);
    RUN_TEST(test_tower_inactive_day27);
    RUN_TEST(test_tower_inactive_day0);
    RUN_TEST(test_tower_inactive_day29);
    RUN_TEST(test_tower_inactive_negative);
    RUN_TEST(test_tower_active_sweep_count);

    /* Purity */
    RUN_TEST(test_purity_book_get_same_result);
    RUN_TEST(test_purity_position_get_same_result);

    /* Sweeps */
    RUN_TEST(test_all_books_have_valid_data);
    RUN_TEST(test_all_positions_have_valid_data);

    /* Cross-checks */
    RUN_TEST(test_position_enum_matches_index);
    RUN_TEST(test_tower_and_crown_source_book4);
    RUN_TEST(test_book_name_matches_book_get);
    RUN_TEST(test_book_card_count_matches_book_get);
    RUN_TEST(test_largest_book_is_planetary_redemption);

    return UNITY_END();
}
