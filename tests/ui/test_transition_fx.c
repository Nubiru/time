/* tests/ui/test_transition_fx.c — transition post-processing effect tests */

#include "../unity/unity.h"
#include "../../src/ui/transition_fx.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void setUp(void) {}
void tearDown(void) {}

/* --- Creation / lifecycle: 1-6 --- */

void test_tfx_create_idle(void)
{
    transition_fx_t fx = tfx_create();
    TEST_ASSERT_EQUAL_INT(TFX_NONE, tfx_preset(fx));
    TEST_ASSERT_FALSE(tfx_active(fx));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, tfx_intensity(fx));
}

void test_tfx_activate_fly_to(void)
{
    transition_fx_t fx = tfx_create();
    fx = tfx_activate(fx, TFX_FLY_TO, 1.0f);
    TEST_ASSERT_EQUAL_INT(TFX_FLY_TO, tfx_preset(fx));
    TEST_ASSERT_TRUE(tfx_active(fx));
}

void test_tfx_activate_sets_intensity(void)
{
    transition_fx_t fx = tfx_create();
    fx = tfx_activate(fx, TFX_FLY_TO, 0.75f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.75f, tfx_intensity(fx));
}

void test_tfx_set_progress(void)
{
    transition_fx_t fx = tfx_create();
    fx = tfx_activate(fx, TFX_FLY_TO, 1.0f);
    fx = tfx_set_progress(fx, 0.5f);
    /* Internal progress should be stored — verified via adjustments */
    tfx_adjustments_t adj = tfx_adjustments(fx);
    /* At midpoint of fly-to, bloom should be near peak */
    TEST_ASSERT_TRUE(adj.bloom_boost > 0.1f);
}

void test_tfx_set_intensity(void)
{
    transition_fx_t fx = tfx_create();
    fx = tfx_activate(fx, TFX_CONVERGENCE, 1.0f);
    fx = tfx_set_intensity(fx, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, tfx_intensity(fx));
}

void test_tfx_deactivate(void)
{
    transition_fx_t fx = tfx_create();
    fx = tfx_activate(fx, TFX_FLY_TO, 1.0f);
    fx = tfx_deactivate(fx);
    TEST_ASSERT_EQUAL_INT(TFX_NONE, tfx_preset(fx));
    TEST_ASSERT_FALSE(tfx_active(fx));
    tfx_adjustments_t adj = tfx_adjustments(fx);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, adj.bloom_boost);
}

/* --- Zero adjustments: 7 --- */

void test_tfx_zero_adjustments(void)
{
    tfx_adjustments_t z = tfx_zero();
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, z.bloom_boost);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, z.bloom_threshold);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, z.vignette_boost);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, z.vignette_radius);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, z.exposure_boost);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, z.grain_boost);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, z.warmth_shift);
}

/* --- Fly-to preset: 8-11 --- */

void test_tfx_fly_to_bloom_at_midpoint(void)
{
    transition_fx_t fx = tfx_create();
    fx = tfx_activate(fx, TFX_FLY_TO, 1.0f);
    fx = tfx_set_progress(fx, 0.5f);
    tfx_adjustments_t adj = tfx_adjustments(fx);
    /* Bell curve peak at 0.5: sin(0.5*PI)=1.0, bloom_boost peak = 0.3 */
    TEST_ASSERT_FLOAT_WITHIN(0.02f, 0.3f, adj.bloom_boost);
}

void test_tfx_fly_to_vignette_at_midpoint(void)
{
    transition_fx_t fx = tfx_create();
    fx = tfx_activate(fx, TFX_FLY_TO, 1.0f);
    fx = tfx_set_progress(fx, 0.5f);
    tfx_adjustments_t adj = tfx_adjustments(fx);
    /* vignette_boost peak = +0.2 at midpoint */
    TEST_ASSERT_FLOAT_WITHIN(0.02f, 0.2f, adj.vignette_boost);
    /* vignette_radius peak = -0.15 at midpoint */
    TEST_ASSERT_FLOAT_WITHIN(0.02f, -0.15f, adj.vignette_radius);
}

void test_tfx_fly_to_zero_at_start(void)
{
    transition_fx_t fx = tfx_create();
    fx = tfx_activate(fx, TFX_FLY_TO, 1.0f);
    fx = tfx_set_progress(fx, 0.0f);
    tfx_adjustments_t adj = tfx_adjustments(fx);
    /* sin(0) = 0, so all effects should be zero at start */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, adj.bloom_boost);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, adj.vignette_boost);
}

void test_tfx_fly_to_zero_at_end(void)
{
    transition_fx_t fx = tfx_create();
    fx = tfx_activate(fx, TFX_FLY_TO, 1.0f);
    fx = tfx_set_progress(fx, 1.0f);
    tfx_adjustments_t adj = tfx_adjustments(fx);
    /* sin(PI) ~ 0, so effects should be near zero at end */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, adj.bloom_boost);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, adj.vignette_boost);
}

/* --- Earth view preset: 12-13 --- */

void test_tfx_earth_view_bloom(void)
{
    transition_fx_t fx = tfx_create();
    fx = tfx_activate(fx, TFX_EARTH_VIEW, 1.0f);
    fx = tfx_set_progress(fx, 0.8f);
    tfx_adjustments_t adj = tfx_adjustments(fx);
    /* Ramp up then hold — at progress=0.8 should be near peak bloom 0.4 */
    TEST_ASSERT_TRUE(adj.bloom_boost > 0.3f);
}

void test_tfx_earth_view_exposure(void)
{
    transition_fx_t fx = tfx_create();
    fx = tfx_activate(fx, TFX_EARTH_VIEW, 1.0f);
    fx = tfx_set_progress(fx, 0.8f);
    tfx_adjustments_t adj = tfx_adjustments(fx);
    /* exposure_boost peak = 0.2 */
    TEST_ASSERT_TRUE(adj.exposure_boost > 0.1f);
}

/* --- Birth travel preset: 14-15 --- */

void test_tfx_birth_travel_grain(void)
{
    transition_fx_t fx = tfx_create();
    fx = tfx_activate(fx, TFX_BIRTH_TRAVEL, 1.0f);
    fx = tfx_set_progress(fx, 0.4f);
    tfx_adjustments_t adj = tfx_adjustments(fx);
    /* In the TRAVEL phase (0.15-0.65), grain should be active */
    TEST_ASSERT_TRUE(adj.grain_boost > 0.02f);
}

void test_tfx_birth_travel_warmth(void)
{
    transition_fx_t fx = tfx_create();
    fx = tfx_activate(fx, TFX_BIRTH_TRAVEL, 1.0f);
    fx = tfx_set_progress(fx, 0.4f);
    tfx_adjustments_t adj = tfx_adjustments(fx);
    /* warmth_shift peak = 0.6 during TRAVEL */
    TEST_ASSERT_TRUE(adj.warmth_shift > 0.3f);
}

/* --- Convergence preset: 16-17 --- */

void test_tfx_convergence_bloom(void)
{
    transition_fx_t fx = tfx_create();
    fx = tfx_activate(fx, TFX_CONVERGENCE, 0.8f);
    fx = tfx_set_progress(fx, 0.5f);
    tfx_adjustments_t adj = tfx_adjustments(fx);
    /* bloom_boost = 0.35 * 0.8 = 0.28 (no envelope) */
    TEST_ASSERT_FLOAT_WITHIN(0.02f, 0.28f, adj.bloom_boost);
}

void test_tfx_convergence_no_envelope(void)
{
    transition_fx_t fx = tfx_create();
    fx = tfx_activate(fx, TFX_CONVERGENCE, 1.0f);

    /* Values at progress=0.2 and progress=0.8 should be identical
     * because convergence has no envelope — just intensity. */
    fx = tfx_set_progress(fx, 0.2f);
    tfx_adjustments_t adj_a = tfx_adjustments(fx);
    fx = tfx_set_progress(fx, 0.8f);
    tfx_adjustments_t adj_b = tfx_adjustments(fx);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, adj_a.bloom_boost, adj_b.bloom_boost);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, adj_a.warmth_shift, adj_b.warmth_shift);
}

/* --- Enter zoom preset: 18 --- */

void test_tfx_enter_zoom_reveal_bloom(void)
{
    transition_fx_t fx = tfx_create();
    fx = tfx_activate(fx, TFX_ENTER_ZOOM, 1.0f);

    /* Before reveal (progress < 0.7): bloom should be minimal */
    fx = tfx_set_progress(fx, 0.5f);
    tfx_adjustments_t adj_early = tfx_adjustments(fx);

    /* During reveal (progress > 0.7): bloom should be large */
    fx = tfx_set_progress(fx, 0.95f);
    tfx_adjustments_t adj_reveal = tfx_adjustments(fx);

    TEST_ASSERT_TRUE(adj_reveal.bloom_boost > adj_early.bloom_boost);
    TEST_ASSERT_TRUE(adj_reveal.bloom_boost > 0.3f);
}

/* --- Intensity scaling: 19 --- */

void test_tfx_intensity_scales_all(void)
{
    transition_fx_t fx = tfx_create();

    /* Full intensity at midpoint */
    fx = tfx_activate(fx, TFX_FLY_TO, 1.0f);
    fx = tfx_set_progress(fx, 0.5f);
    tfx_adjustments_t full = tfx_adjustments(fx);

    /* Half intensity at midpoint */
    fx = tfx_set_intensity(fx, 0.5f);
    tfx_adjustments_t half = tfx_adjustments(fx);

    TEST_ASSERT_FLOAT_WITHIN(0.02f, full.bloom_boost * 0.5f, half.bloom_boost);
    TEST_ASSERT_FLOAT_WITHIN(0.02f, full.vignette_boost * 0.5f, half.vignette_boost);
}

/* --- Stateless compute: 20 --- */

void test_tfx_compute_stateless(void)
{
    /* tfx_compute should give same result as activate+set_progress */
    transition_fx_t fx = tfx_create();
    fx = tfx_activate(fx, TFX_FLY_TO, 1.0f);
    fx = tfx_set_progress(fx, 0.5f);
    tfx_adjustments_t from_state = tfx_adjustments(fx);

    tfx_adjustments_t from_compute = tfx_compute(TFX_FLY_TO, 1.0f, 0.5f);

    TEST_ASSERT_FLOAT_WITHIN(0.001f, from_state.bloom_boost, from_compute.bloom_boost);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, from_state.vignette_boost, from_compute.vignette_boost);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, from_state.warmth_shift, from_compute.warmth_shift);
}

/* --- Combine additive: 21 --- */

void test_tfx_combine_additive(void)
{
    tfx_adjustments_t a = tfx_zero();
    a.bloom_boost = 0.1f;
    a.warmth_shift = 0.3f;

    tfx_adjustments_t b = tfx_zero();
    b.bloom_boost = 0.2f;
    b.exposure_boost = 0.15f;

    tfx_adjustments_t c = tfx_combine(a, b);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.3f, c.bloom_boost);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.3f, c.warmth_shift);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.15f, c.exposure_boost);
}

/* --- TFX_NONE always zero: 22 --- */

void test_tfx_none_preset_zero(void)
{
    tfx_adjustments_t adj = tfx_compute(TFX_NONE, 1.0f, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, adj.bloom_boost);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, adj.vignette_boost);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, adj.grain_boost);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, adj.warmth_shift);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, adj.exposure_boost);
}

void test_tfx_tick_advances_elapsed(void)
{
    transition_fx_t fx = tfx_create();
    fx = tfx_activate(fx, TFX_FLY_TO, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, fx.elapsed);

    fx = tfx_tick(fx, 0.016f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.016f, fx.elapsed);

    fx = tfx_tick(fx, 0.016f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.032f, fx.elapsed);

    /* Negative dt should not advance */
    fx = tfx_tick(fx, -1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.032f, fx.elapsed);

    /* Zero dt should not advance */
    fx = tfx_tick(fx, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.032f, fx.elapsed);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_tfx_create_idle);
    RUN_TEST(test_tfx_activate_fly_to);
    RUN_TEST(test_tfx_activate_sets_intensity);
    RUN_TEST(test_tfx_set_progress);
    RUN_TEST(test_tfx_set_intensity);
    RUN_TEST(test_tfx_deactivate);
    RUN_TEST(test_tfx_zero_adjustments);
    RUN_TEST(test_tfx_fly_to_bloom_at_midpoint);
    RUN_TEST(test_tfx_fly_to_vignette_at_midpoint);
    RUN_TEST(test_tfx_fly_to_zero_at_start);
    RUN_TEST(test_tfx_fly_to_zero_at_end);
    RUN_TEST(test_tfx_earth_view_bloom);
    RUN_TEST(test_tfx_earth_view_exposure);
    RUN_TEST(test_tfx_birth_travel_grain);
    RUN_TEST(test_tfx_birth_travel_warmth);
    RUN_TEST(test_tfx_convergence_bloom);
    RUN_TEST(test_tfx_convergence_no_envelope);
    RUN_TEST(test_tfx_enter_zoom_reveal_bloom);
    RUN_TEST(test_tfx_intensity_scales_all);
    RUN_TEST(test_tfx_compute_stateless);
    RUN_TEST(test_tfx_combine_additive);
    RUN_TEST(test_tfx_none_preset_zero);
    RUN_TEST(test_tfx_tick_advances_elapsed);
    return UNITY_END();
}
