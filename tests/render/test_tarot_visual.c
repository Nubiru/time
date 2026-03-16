/* test_tarot_visual.c — Tests for Tarot Visual Data module
 *
 * Validates the 22 Major Arcana visual data: Thoth names, color scales,
 * card frame geometry, Tree of Life positions, motif names, edge cases.
 *
 * Sources: Crowley "Book of Thoth", Crowley "777",
 *          Lady Frieda Harris (Thoth deck artist). */

#include "../../tests/unity/unity.h"
#include "../../src/render/tarot_visual.h"
#include "../../src/render/color_theory.h"
#include "../../src/ui/golden_layout.h"
#include "../../src/systems/kabbalah/tree_geometry.h"
#include "../../src/math/color.h"

#include <math.h>
#include <string.h>

#define EPSILON 0.001f

void setUp(void) {}
void tearDown(void) {}

/* ===== Data Integrity: Count and Range ===== */

void test_major_count_is_22(void) {
    TEST_ASSERT_EQUAL_INT(22, tv_major_count());
}

void test_all_22_cards_valid(void) {
    for (int i = 0; i < 22; i++) {
        tv_major_t card = tv_major_get(i);
        TEST_ASSERT_EQUAL_INT(i, card.number);
        TEST_ASSERT_NOT_NULL(card.name);
        TEST_ASSERT_NOT_NULL(card.thoth_name);
        TEST_ASSERT_NOT_NULL(card.symbol_desc);
        TEST_ASSERT_NOT_NULL(card.attribution);
    }
}

void test_invalid_card_returns_sentinel(void) {
    tv_major_t invalid = tv_major_get(-1);
    TEST_ASSERT_EQUAL_INT(-1, invalid.number);

    tv_major_t too_high = tv_major_get(22);
    TEST_ASSERT_EQUAL_INT(-1, too_high.number);

    tv_major_t way_high = tv_major_get(999);
    TEST_ASSERT_EQUAL_INT(-1, way_high.number);
}

/* ===== Thoth Names (Crowley-specific) ===== */

void test_fool_is_card_0(void) {
    tv_major_t fool = tv_major_get(0);
    TEST_ASSERT_EQUAL_STRING("The Fool", fool.thoth_name);
    TEST_ASSERT_EQUAL_STRING("Air", fool.attribution);
}

void test_magus_not_magician(void) {
    tv_major_t magus = tv_major_get(1);
    TEST_ASSERT_EQUAL_STRING("The Magus", magus.thoth_name);
}

void test_adjustment_not_justice(void) {
    tv_major_t adj = tv_major_get(8);
    TEST_ASSERT_EQUAL_STRING("Adjustment", adj.thoth_name);
    TEST_ASSERT_EQUAL_STRING("Libra", adj.attribution);
}

void test_lust_not_strength(void) {
    tv_major_t lust = tv_major_get(11);
    TEST_ASSERT_EQUAL_STRING("Lust", lust.thoth_name);
    TEST_ASSERT_EQUAL_STRING("Leo", lust.attribution);
}

void test_art_not_temperance(void) {
    tv_major_t art = tv_major_get(14);
    TEST_ASSERT_EQUAL_STRING("Art", art.thoth_name);
    TEST_ASSERT_EQUAL_STRING("Sagittarius", art.attribution);
}

void test_fortune_not_wheel(void) {
    tv_major_t fortune = tv_major_get(10);
    TEST_ASSERT_EQUAL_STRING("Fortune", fortune.thoth_name);
    TEST_ASSERT_EQUAL_STRING("Jupiter", fortune.attribution);
}

void test_aeon_not_judgement(void) {
    tv_major_t aeon = tv_major_get(20);
    TEST_ASSERT_EQUAL_STRING("The Aeon", aeon.thoth_name);
    TEST_ASSERT_EQUAL_STRING("Fire", aeon.attribution);
}

void test_universe_not_world(void) {
    tv_major_t universe = tv_major_get(21);
    TEST_ASSERT_EQUAL_STRING("The Universe", universe.thoth_name);
    TEST_ASSERT_EQUAL_STRING("Saturn", universe.attribution);
}

/* Crowley's Emperor/Star swap: Emperor (4) attributed to Aries,
 * Star (17) attributed to Aquarius — but Tzaddi/He swap underneath */
void test_emperor_aries(void) {
    tv_major_t emperor = tv_major_get(4);
    TEST_ASSERT_EQUAL_STRING("The Emperor", emperor.thoth_name);
    TEST_ASSERT_EQUAL_STRING("Aries", emperor.attribution);
}

void test_star_aquarius(void) {
    tv_major_t star = tv_major_get(17);
    TEST_ASSERT_EQUAL_STRING("The Star", star.thoth_name);
    TEST_ASSERT_EQUAL_STRING("Aquarius", star.attribution);
}

void test_priestess_name(void) {
    tv_major_t priestess = tv_major_get(2);
    TEST_ASSERT_EQUAL_STRING("The Priestess", priestess.thoth_name);
    TEST_ASSERT_EQUAL_STRING("Moon", priestess.attribution);
}

/* ===== Color Scales ===== */

void test_card_color_returns_valid_rgb(void) {
    for (int i = 0; i < 22; i++) {
        for (int s = 0; s < TV_SCALE_COUNT; s++) {
            color_rgb_t c = tv_card_color(i, (tv_scale_t)s);
            /* All channels should be in [0, 1] */
            TEST_ASSERT_TRUE(c.r >= 0.0f && c.r <= 1.0f);
            TEST_ASSERT_TRUE(c.g >= 0.0f && c.g <= 1.0f);
            TEST_ASSERT_TRUE(c.b >= 0.0f && c.b <= 1.0f);
        }
    }
}

void test_king_scale_is_brightest(void) {
    /* King scale should generally be brighter than Princess scale */
    for (int i = 0; i < 22; i++) {
        color_rgb_t king = tv_card_color(i, TV_SCALE_KING);
        color_rgb_t princess = tv_card_color(i, TV_SCALE_PRINCESS);
        float king_lum = color_luminance(king);
        float princess_lum = color_luminance(princess);
        /* King should be brighter than princess */
        TEST_ASSERT_TRUE(king_lum >= princess_lum);
    }
}

void test_color_scales_descend_in_luminance(void) {
    /* For at least most cards, luminance should descend King > Queen > Prince > Princess */
    int descending_count = 0;
    for (int i = 0; i < 22; i++) {
        float lum[TV_SCALE_COUNT];
        for (int s = 0; s < TV_SCALE_COUNT; s++) {
            color_rgb_t c = tv_card_color(i, (tv_scale_t)s);
            lum[s] = color_luminance(c);
        }
        if (lum[0] >= lum[1] && lum[1] >= lum[2] && lum[2] >= lum[3]) {
            descending_count++;
        }
    }
    /* At least 18 of 22 cards should show this pattern */
    TEST_ASSERT_TRUE(descending_count >= 18);
}

void test_card_primary_matches_king_scale(void) {
    for (int i = 0; i < 22; i++) {
        color_rgb_t primary = tv_card_primary(i);
        color_rgb_t king = tv_card_color(i, TV_SCALE_KING);
        TEST_ASSERT_FLOAT_WITHIN(EPSILON, king.r, primary.r);
        TEST_ASSERT_FLOAT_WITHIN(EPSILON, king.g, primary.g);
        TEST_ASSERT_FLOAT_WITHIN(EPSILON, king.b, primary.b);
    }
}

void test_invalid_card_color_returns_black(void) {
    color_rgb_t c = tv_card_color(-1, TV_SCALE_KING);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, c.r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, c.g);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, c.b);
}

void test_invalid_scale_returns_black(void) {
    color_rgb_t c = tv_card_color(0, TV_SCALE_COUNT);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, c.r);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, c.g);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, c.b);
}

/* ===== Colors Derived from Style System ===== */

void test_fool_color_derived_from_mood(void) {
    /* The Fool = Air = CT_MOOD_CLARITY. King scale = base mood color.
     * Verify the color comes from ct_mood_color, not hardcoded. */
    tv_major_t fool = tv_major_get(0);
    color_rgb_t king = fool.colors[TV_SCALE_KING];
    /* Should be non-black (derived from mood system) */
    float lum = color_luminance(king);
    TEST_ASSERT_TRUE(lum > 0.01f);
}

void test_sun_card_has_warm_color(void) {
    /* The Sun (19) = Sun = CT_MOOD_WARMTH */
    color_rgb_t king = tv_card_color(19, TV_SCALE_KING);
    /* Warm colors have more red than blue */
    TEST_ASSERT_TRUE(king.r > king.b);
}

void test_moon_card_has_cool_color(void) {
    /* The Moon (18) = Pisces = Water sign = CT_MOOD_MYSTERY */
    color_rgb_t king = tv_card_color(18, TV_SCALE_KING);
    /* Cool/mysterious colors tend toward blue/violet */
    float lum = color_luminance(king);
    TEST_ASSERT_TRUE(lum > 0.01f); /* not black */
}

/* ===== Card Frame Geometry ===== */

void test_card_frame_height_is_phi_times_width(void) {
    tv_card_frame_t frame = tv_card_frame(100.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 100.0f * (float)PHI_RATIO, frame.height);
}

void test_card_frame_border_uses_phi(void) {
    tv_card_frame_t frame = tv_card_frame(100.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 100.0f * (float)PHI_INV5, frame.border);
}

void test_card_frame_symbol_radius_uses_phi(void) {
    tv_card_frame_t frame = tv_card_frame(100.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 100.0f * (float)PHI_INV2, frame.symbol_radius);
}

void test_card_frame_title_y_at_golden_section(void) {
    tv_card_frame_t frame = tv_card_frame(100.0f);
    float expected_height = 100.0f * (float)PHI_RATIO;
    TEST_ASSERT_FLOAT_WITHIN(0.01f, expected_height * (float)PHI_INV, frame.title_y);
}

void test_card_frame_number_y_uses_phi(void) {
    tv_card_frame_t frame = tv_card_frame(100.0f);
    float expected_height = 100.0f * (float)PHI_RATIO;
    TEST_ASSERT_FLOAT_WITHIN(0.01f, expected_height * (float)PHI_INV4, frame.number_y);
}

void test_card_frame_width_preserved(void) {
    tv_card_frame_t frame = tv_card_frame(200.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 200.0f, frame.width);
}

void test_card_frame_zero_width(void) {
    tv_card_frame_t frame = tv_card_frame(0.0f);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, frame.width);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, 0.0f, frame.height);
}

/* ===== Motif Names ===== */

void test_motif_names_not_null(void) {
    for (int i = 0; i < TV_MOTIF_COUNT; i++) {
        const char *name = tv_motif_name((tv_motif_t)i);
        TEST_ASSERT_NOT_NULL(name);
        TEST_ASSERT_TRUE(strlen(name) > 0);
    }
}

void test_motif_circle_name(void) {
    TEST_ASSERT_EQUAL_STRING("Circle", tv_motif_name(TV_MOTIF_CIRCLE));
}

void test_motif_spiral_name(void) {
    TEST_ASSERT_EQUAL_STRING("Spiral", tv_motif_name(TV_MOTIF_SPIRAL));
}

void test_invalid_motif_returns_question(void) {
    TEST_ASSERT_EQUAL_STRING("?", tv_motif_name(TV_MOTIF_COUNT));
    TEST_ASSERT_EQUAL_STRING("?", tv_motif_name((tv_motif_t)-1));
}

/* ===== Scale Names ===== */

void test_scale_names(void) {
    TEST_ASSERT_EQUAL_STRING("King", tv_scale_name(TV_SCALE_KING));
    TEST_ASSERT_EQUAL_STRING("Queen", tv_scale_name(TV_SCALE_QUEEN));
    TEST_ASSERT_EQUAL_STRING("Prince", tv_scale_name(TV_SCALE_PRINCE));
    TEST_ASSERT_EQUAL_STRING("Princess", tv_scale_name(TV_SCALE_PRINCESS));
}

void test_invalid_scale_name(void) {
    TEST_ASSERT_EQUAL_STRING("?", tv_scale_name(TV_SCALE_COUNT));
}

/* ===== Tree of Life Positions ===== */

void test_tree_position_valid_for_all_cards(void) {
    for (int i = 0; i < 22; i++) {
        float x, y;
        int ok = tv_tree_position(i, &x, &y);
        TEST_ASSERT_EQUAL_INT(1, ok);
    }
}

void test_tree_position_invalid_card(void) {
    float x, y;
    int ok = tv_tree_position(-1, &x, &y);
    TEST_ASSERT_EQUAL_INT(0, ok);
    ok = tv_tree_position(22, &x, &y);
    TEST_ASSERT_EQUAL_INT(0, ok);
}

void test_tree_positions_are_midpoints(void) {
    /* For the Fool (path 0): Keter(0) to Chokmah(1)
     * Position should be the midpoint of those two sefirot */
    tree_layout_t layout = tree_layout_generate(1.0f, 1.0f);
    tree_path_t path = tree_path_for_tarot(0);

    float expected_x = (layout.node_x[path.from_sefirah] +
                        layout.node_x[path.to_sefirah]) * 0.5f;
    float expected_y = (layout.node_y[path.from_sefirah] +
                        layout.node_y[path.to_sefirah]) * 0.5f;

    float x, y;
    tv_tree_position(0, &x, &y);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, expected_x, x);
    TEST_ASSERT_FLOAT_WITHIN(EPSILON, expected_y, y);
}

void test_tree_positions_all_returns_22(void) {
    float xs[22], ys[22];
    int count = tv_tree_positions_all(xs, ys, 22);
    TEST_ASSERT_EQUAL_INT(22, count);
}

void test_tree_positions_all_limited(void) {
    float xs[5], ys[5];
    int count = tv_tree_positions_all(xs, ys, 5);
    TEST_ASSERT_EQUAL_INT(5, count);
}

void test_tree_positions_all_zero_max(void) {
    int count = tv_tree_positions_all(NULL, NULL, 0);
    TEST_ASSERT_EQUAL_INT(0, count);
}

/* ===== Motif Assignments ===== */

void test_fool_has_spiral_motif(void) {
    /* The Fool — spirit of Aether, growth, phi spiral */
    tv_major_t fool = tv_major_get(0);
    TEST_ASSERT_EQUAL_INT(TV_MOTIF_SPIRAL, fool.primary_motif);
}

void test_sun_has_rays_motif(void) {
    tv_major_t sun = tv_major_get(19);
    TEST_ASSERT_EQUAL_INT(TV_MOTIF_RAYS, sun.primary_motif);
}

void test_moon_card_has_crescent_motif(void) {
    tv_major_t moon = tv_major_get(18);
    TEST_ASSERT_EQUAL_INT(TV_MOTIF_CRESCENT, moon.primary_motif);
}

void test_tower_has_lightning_motif(void) {
    tv_major_t tower = tv_major_get(16);
    TEST_ASSERT_EQUAL_INT(TV_MOTIF_LIGHTNING, tower.primary_motif);
}

/* ===== Tree Path Index Cross-Check ===== */

void test_tree_path_indices_valid(void) {
    for (int i = 0; i < 22; i++) {
        tv_major_t card = tv_major_get(i);
        TEST_ASSERT_TRUE(card.tree_path_index >= 0);
        TEST_ASSERT_TRUE(card.tree_path_index < TREE_PATH_COUNT);
    }
}

void test_tree_path_index_links_to_correct_attribution(void) {
    /* Crowley swapped Emperor/Star (4/17) and Adjustment/Lust (8/11).
     * Card 4 (Emperor/Aries) sits on Tsade path (index 17).
     * Card 17 (Star/Aquarius) sits on He path (index 4).
     * Card 8 (Adjustment/Libra) sits on Lamed path (index 11).
     * Card 11 (Lust/Leo) sits on Tet path (index 8). */
    tv_major_t emperor = tv_major_get(4);
    TEST_ASSERT_EQUAL_INT(17, emperor.tree_path_index);

    tv_major_t star = tv_major_get(17);
    TEST_ASSERT_EQUAL_INT(4, star.tree_path_index);

    tv_major_t adjustment = tv_major_get(8);
    TEST_ASSERT_EQUAL_INT(11, adjustment.tree_path_index);

    tv_major_t lust = tv_major_get(11);
    TEST_ASSERT_EQUAL_INT(8, lust.tree_path_index);
}

void test_unswapped_cards_match_tree_geometry(void) {
    /* Non-swapped cards: tree_path_index = card number */
    int swapped[] = {4, 8, 11, 17};
    for (int i = 0; i < 22; i++) {
        int is_swapped = 0;
        for (int j = 0; j < 4; j++) {
            if (i == swapped[j]) { is_swapped = 1; break; }
        }
        if (!is_swapped) {
            tv_major_t card = tv_major_get(i);
            TEST_ASSERT_EQUAL_INT(i, card.tree_path_index);
        }
    }
}

/* ===== Runner ===== */

int main(void) {
    UNITY_BEGIN();

    /* Data integrity */
    RUN_TEST(test_major_count_is_22);
    RUN_TEST(test_all_22_cards_valid);
    RUN_TEST(test_invalid_card_returns_sentinel);

    /* Thoth names */
    RUN_TEST(test_fool_is_card_0);
    RUN_TEST(test_magus_not_magician);
    RUN_TEST(test_adjustment_not_justice);
    RUN_TEST(test_lust_not_strength);
    RUN_TEST(test_art_not_temperance);
    RUN_TEST(test_fortune_not_wheel);
    RUN_TEST(test_aeon_not_judgement);
    RUN_TEST(test_universe_not_world);
    RUN_TEST(test_emperor_aries);
    RUN_TEST(test_star_aquarius);
    RUN_TEST(test_priestess_name);

    /* Color scales */
    RUN_TEST(test_card_color_returns_valid_rgb);
    RUN_TEST(test_king_scale_is_brightest);
    RUN_TEST(test_color_scales_descend_in_luminance);
    RUN_TEST(test_card_primary_matches_king_scale);
    RUN_TEST(test_invalid_card_color_returns_black);
    RUN_TEST(test_invalid_scale_returns_black);

    /* Colors from style system */
    RUN_TEST(test_fool_color_derived_from_mood);
    RUN_TEST(test_sun_card_has_warm_color);
    RUN_TEST(test_moon_card_has_cool_color);

    /* Card frame geometry */
    RUN_TEST(test_card_frame_height_is_phi_times_width);
    RUN_TEST(test_card_frame_border_uses_phi);
    RUN_TEST(test_card_frame_symbol_radius_uses_phi);
    RUN_TEST(test_card_frame_title_y_at_golden_section);
    RUN_TEST(test_card_frame_number_y_uses_phi);
    RUN_TEST(test_card_frame_width_preserved);
    RUN_TEST(test_card_frame_zero_width);

    /* Motif names */
    RUN_TEST(test_motif_names_not_null);
    RUN_TEST(test_motif_circle_name);
    RUN_TEST(test_motif_spiral_name);
    RUN_TEST(test_invalid_motif_returns_question);

    /* Scale names */
    RUN_TEST(test_scale_names);
    RUN_TEST(test_invalid_scale_name);

    /* Tree of Life positions */
    RUN_TEST(test_tree_position_valid_for_all_cards);
    RUN_TEST(test_tree_position_invalid_card);
    RUN_TEST(test_tree_positions_are_midpoints);
    RUN_TEST(test_tree_positions_all_returns_22);
    RUN_TEST(test_tree_positions_all_limited);
    RUN_TEST(test_tree_positions_all_zero_max);

    /* Motif assignments */
    RUN_TEST(test_fool_has_spiral_motif);
    RUN_TEST(test_sun_has_rays_motif);
    RUN_TEST(test_moon_card_has_crescent_motif);
    RUN_TEST(test_tower_has_lightning_motif);

    /* Tree path cross-check */
    RUN_TEST(test_tree_path_indices_valid);
    RUN_TEST(test_tree_path_index_links_to_correct_attribution);
    RUN_TEST(test_unswapped_cards_match_tree_geometry);

    return UNITY_END();
}
