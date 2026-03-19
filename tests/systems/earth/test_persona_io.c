/* test_persona_io.c -- Tests for persona I/O integration layer.
 * Verifies save/load roundtrip through storage_bridge. */

#include "unity.h"
#include "systems/earth/persona_io.h"
#include "systems/earth/persona_persist.h"
#include "platform/storage_bridge.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>

void setUp(void)  { sb_reset(); }
void tearDown(void) { (void)0; }

/* ===== 1. New visitor — empty storage → returns 0, all defaults ===== */

void test_load_all_new_visitor(void)
{
    up_prefs_t prefs;
    ip_profile_t interests;
    ps_space_t space;
    pf_settings_t privacy;
    int by = -1, bm = -1, bd = -1, bhl = -1;
    double blat = 999.0, blon = 999.0;

    int result = pio_load_all(&prefs, &interests, &space, &privacy,
                              &by, &bm, &bd, &bhl, &blat, &blon);

    TEST_ASSERT_EQUAL_INT(0, result);

    /* Prefs should be defaults */
    up_prefs_t def = up_default();
    TEST_ASSERT_EQUAL_INT((int)def.visual.theme, (int)prefs.visual.theme);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, def.visual.font_scale,
                             prefs.visual.font_scale);

    /* Interests should be defaults */
    ip_profile_t idef = ip_default_profile();
    TEST_ASSERT_EQUAL_INT((int)idef.visibility, (int)interests.visibility);

    /* Space should be empty */
    TEST_ASSERT_EQUAL_INT(0, space.favorite_count);
    TEST_ASSERT_EQUAL_INT(0, space.cycle_count);
    TEST_ASSERT_EQUAL_INT(0, space.bookmark_count);

    /* Privacy should be defaults */
    pf_settings_t pdef = pf_default_settings();
    TEST_ASSERT_EQUAL_INT((int)pdef.consent, (int)privacy.consent);
    TEST_ASSERT_EQUAL_INT((int)pdef.retention, (int)privacy.retention);
}

/* ===== 2. Save and load prefs ===== */

void test_save_and_load_prefs(void)
{
    up_prefs_t prefs = up_default();
    prefs.visual.theme = THEME_DAWN;
    prefs.visual.font_scale = 1.5f;
    prefs.visual.opacity_scale = 0.75f;
    prefs.visual.animation_speed = 1.8f;
    prefs.visual.layers_visible = 0xFF;
    prefs.visual.default_scale_level = 3;
    prefs.temporal.home_timezone_offset_min = -300;
    prefs.temporal.use_24h_time = 1;
    prefs.temporal.week_start_day = 1;
    prefs.location.home_lat = 40.7128;
    prefs.location.home_lon = -74.006;
    prefs.location.exploration_radius_km = 50.0;
    prefs.location.location_set = 1;

    pio_save_prefs(&prefs);

    /* Load back */
    up_prefs_t loaded;
    ip_profile_t interests;
    ps_space_t space;
    pf_settings_t privacy;
    int by, bm, bd, bhl;
    double blat, blon;

    pio_load_all(&loaded, &interests, &space, &privacy,
                 &by, &bm, &bd, &bhl, &blat, &blon);

    TEST_ASSERT_EQUAL_INT((int)THEME_DAWN, (int)loaded.visual.theme);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.5f, loaded.visual.font_scale);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.75f, loaded.visual.opacity_scale);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.8f, loaded.visual.animation_speed);
    TEST_ASSERT_EQUAL_INT(0xFF, loaded.visual.layers_visible);
    TEST_ASSERT_EQUAL_INT(3, loaded.visual.default_scale_level);
    TEST_ASSERT_EQUAL_INT(-300, loaded.temporal.home_timezone_offset_min);
    TEST_ASSERT_EQUAL_INT(1, loaded.temporal.use_24h_time);
    TEST_ASSERT_EQUAL_INT(1, loaded.temporal.week_start_day);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 40.7128, loaded.location.home_lat);
    TEST_ASSERT_FLOAT_WITHIN(0.001, -74.006, loaded.location.home_lon);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 50.0, loaded.location.exploration_radius_km);
    TEST_ASSERT_EQUAL_INT(1, loaded.location.location_set);
}

/* ===== 3. Save and load birth data ===== */

void test_save_and_load_birth(void)
{
    pio_save_birth(1990, 7, 15, 1, 34.0522, -118.2437);

    up_prefs_t prefs;
    ip_profile_t interests;
    ps_space_t space;
    pf_settings_t privacy;
    int by, bm, bd, bhl;
    double blat, blon;

    int result = pio_load_all(&prefs, &interests, &space, &privacy,
                              &by, &bm, &bd, &bhl, &blat, &blon);

    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_INT(1990, by);
    TEST_ASSERT_EQUAL_INT(7, bm);
    TEST_ASSERT_EQUAL_INT(15, bd);
    TEST_ASSERT_EQUAL_INT(1, bhl);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 34.0522, blat);
    TEST_ASSERT_FLOAT_WITHIN(0.001, -118.2437, blon);
}

/* ===== 4. Save and load interests ===== */

void test_save_and_load_interests(void)
{
    ip_profile_t profile = ip_default_profile();
    profile.weights[0] = 0.9;
    profile.weights[3] = 0.5;
    profile.weights[7] = 0.3;
    profile.visibility = IP_VIS_PUBLIC;
    profile.activity_level = IP_ACT_EXTROVERT;
    profile.location_radius_km = 100.0;

    pio_save_interests(&profile);

    up_prefs_t prefs;
    ip_profile_t loaded;
    ps_space_t space;
    pf_settings_t privacy;
    int by, bm, bd, bhl;
    double blat, blon;

    pio_load_all(&prefs, &loaded, &space, &privacy,
                 &by, &bm, &bd, &bhl, &blat, &blon);

    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.9, loaded.weights[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.5, loaded.weights[3]);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.3, loaded.weights[7]);
    TEST_ASSERT_EQUAL_INT((int)IP_VIS_PUBLIC, (int)loaded.visibility);
    TEST_ASSERT_EQUAL_INT((int)IP_ACT_EXTROVERT, (int)loaded.activity_level);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 100.0, loaded.location_radius_km);
}

/* ===== 5. Save and load space with cycles and bookmarks ===== */

void test_save_and_load_space(void)
{
    ps_space_t space = ps_create();
    space = ps_add_favorite(space, 2);
    space = ps_add_favorite(space, 5);
    space = ps_add_cycle(space, "Saturn Return", 3, 2451545.0, 2462000.0);
    space = ps_bookmark(space, 2451545.0, "Y2K moment", 0);
    space = ps_mark_explored(space, 1);
    space = ps_mark_explored(space, 4);

    pio_save_space(&space);

    up_prefs_t prefs;
    ip_profile_t interests;
    ps_space_t loaded;
    pf_settings_t privacy;
    int by, bm, bd, bhl;
    double blat, blon;

    pio_load_all(&prefs, &interests, &loaded, &privacy,
                 &by, &bm, &bd, &bhl, &blat, &blon);

    TEST_ASSERT_EQUAL_INT(2, loaded.favorite_count);
    TEST_ASSERT_EQUAL_INT(2, loaded.favorite_systems[0]);
    TEST_ASSERT_EQUAL_INT(5, loaded.favorite_systems[1]);

    TEST_ASSERT_EQUAL_INT(1, loaded.cycle_count);
    TEST_ASSERT_EQUAL_STRING("Saturn Return", loaded.cycles[0].name);
    TEST_ASSERT_EQUAL_INT(3, loaded.cycles[0].system_id);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 2451545.0, loaded.cycles[0].start_jd);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 2462000.0, loaded.cycles[0].next_event_jd);

    TEST_ASSERT_EQUAL_INT(1, loaded.bookmark_count);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 2451545.0, loaded.bookmarks[0].jd);
    TEST_ASSERT_EQUAL_STRING("Y2K moment", loaded.bookmarks[0].note);
    TEST_ASSERT_EQUAL_INT(0, loaded.bookmarks[0].system_id);

    TEST_ASSERT_EQUAL_INT(2, loaded.explored_count);
}

/* ===== 6. Save and load privacy ===== */

void test_save_and_load_privacy(void)
{
    pf_settings_t settings = pf_default_settings();
    settings.consent = PF_CONSENT_FULL;
    settings.retention = PF_RETAIN_LOCAL;
    settings = pf_grant_field(settings, PF_FIELD_INTERESTS);
    settings = pf_grant_field(settings, PF_FIELD_TIMEZONE);

    pio_save_privacy(&settings);

    up_prefs_t prefs;
    ip_profile_t interests;
    ps_space_t space;
    pf_settings_t loaded;
    int by, bm, bd, bhl;
    double blat, blon;

    pio_load_all(&prefs, &interests, &space, &loaded,
                 &by, &bm, &bd, &bhl, &blat, &blon);

    TEST_ASSERT_EQUAL_INT((int)PF_CONSENT_FULL, (int)loaded.consent);
    TEST_ASSERT_EQUAL_INT((int)PF_RETAIN_LOCAL, (int)loaded.retention);
    TEST_ASSERT_EQUAL_INT(1, loaded.fields_shared[PF_FIELD_INTERESTS]);
    TEST_ASSERT_EQUAL_INT(1, loaded.fields_shared[PF_FIELD_TIMEZONE]);
    TEST_ASSERT_EQUAL_INT(0, loaded.fields_shared[PF_FIELD_REAL_NAME]);
}

/* ===== 7. Full roundtrip — save all, load all ===== */

void test_full_roundtrip(void)
{
    /* Save each type */
    up_prefs_t prefs = up_default();
    prefs.visual.theme = THEME_DAWN;
    prefs.temporal.use_24h_time = 1;
    pio_save_prefs(&prefs);

    pio_save_birth(1985, 3, 20, 1, 48.8566, 2.3522);

    ip_profile_t interests = ip_default_profile();
    interests.weights[0] = 0.8;
    interests.visibility = IP_VIS_PSEUDONYM;
    pio_save_interests(&interests);

    ps_space_t space = ps_create();
    space = ps_add_favorite(space, 7);
    space = ps_bookmark(space, 2460000.0, "Eclipse day", 1);
    pio_save_space(&space);

    pf_settings_t privacy = pf_default_settings();
    privacy.consent = PF_CONSENT_BASIC;
    privacy.retention = PF_RETAIN_LOCAL;
    pio_save_privacy(&privacy);

    /* Load everything back */
    up_prefs_t lp;
    ip_profile_t li;
    ps_space_t ls;
    pf_settings_t lpv;
    int by, bm, bd, bhl;
    double blat, blon;

    int result = pio_load_all(&lp, &li, &ls, &lpv,
                              &by, &bm, &bd, &bhl, &blat, &blon);

    /* Birth data found */
    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_INT(1985, by);
    TEST_ASSERT_EQUAL_INT(3, bm);
    TEST_ASSERT_EQUAL_INT(20, bd);
    TEST_ASSERT_EQUAL_INT(1, bhl);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 48.8566, blat);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 2.3522, blon);

    /* Prefs */
    TEST_ASSERT_EQUAL_INT((int)THEME_DAWN, (int)lp.visual.theme);
    TEST_ASSERT_EQUAL_INT(1, lp.temporal.use_24h_time);

    /* Interests */
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.8, li.weights[0]);
    TEST_ASSERT_EQUAL_INT((int)IP_VIS_PSEUDONYM, (int)li.visibility);

    /* Space */
    TEST_ASSERT_EQUAL_INT(1, ls.favorite_count);
    TEST_ASSERT_EQUAL_INT(7, ls.favorite_systems[0]);
    TEST_ASSERT_EQUAL_INT(1, ls.bookmark_count);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 2460000.0, ls.bookmarks[0].jd);
    TEST_ASSERT_EQUAL_STRING("Eclipse day", ls.bookmarks[0].note);

    /* Privacy */
    TEST_ASSERT_EQUAL_INT((int)PF_CONSENT_BASIC, (int)lpv.consent);
    TEST_ASSERT_EQUAL_INT((int)PF_RETAIN_LOCAL, (int)lpv.retention);
}

/* ===== 8. Delete all clears storage ===== */

void test_delete_all(void)
{
    /* Save some data */
    pio_save_birth(2000, 1, 1, 0, 0.0, 0.0);
    up_prefs_t prefs = up_default();
    prefs.visual.theme = THEME_DAWN;
    pio_save_prefs(&prefs);

    /* Verify data exists */
    TEST_ASSERT_EQUAL_INT(1, sb_exists("time.birth.year"));

    pio_delete_all();

    /* Should be a new visitor now */
    up_prefs_t lp;
    ip_profile_t li;
    ps_space_t ls;
    pf_settings_t lpv;
    int by, bm, bd, bhl;
    double blat, blon;

    int result = pio_load_all(&lp, &li, &ls, &lpv,
                              &by, &bm, &bd, &bhl, &blat, &blon);
    TEST_ASSERT_EQUAL_INT(0, result);
}

/* ===== 9. Partial data — prefs only, no birth ===== */

void test_partial_data(void)
{
    up_prefs_t prefs = up_default();
    prefs.visual.font_scale = 1.75f;
    prefs.location.home_lat = 51.5074;
    prefs.location.home_lon = -0.1278;
    prefs.location.location_set = 1;
    pio_save_prefs(&prefs);

    /* No birth saved */

    up_prefs_t lp;
    ip_profile_t li;
    ps_space_t ls;
    pf_settings_t lpv;
    int by = -1, bm = -1, bd = -1, bhl = -1;
    double blat = 999.0, blon = 999.0;

    int result = pio_load_all(&lp, &li, &ls, &lpv,
                              &by, &bm, &bd, &bhl, &blat, &blon);

    TEST_ASSERT_EQUAL_INT(0, result); /* No birth data */
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.75f, lp.visual.font_scale);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 51.5074, lp.location.home_lat);
    TEST_ASSERT_FLOAT_WITHIN(0.001, -0.1278, lp.location.home_lon);
    TEST_ASSERT_EQUAL_INT(1, lp.location.location_set);
}

/* ===== 10. Check migration — new storage ===== */

void test_check_migration_new(void)
{
    /* No schema version in storage */
    TEST_ASSERT_EQUAL_INT(0, sb_exists("time.schema_version"));

    pio_check_migration();

    /* Schema version should now be saved */
    char buf[64];
    int len = sb_load("time.schema_version", buf, sizeof(buf));
    TEST_ASSERT_GREATER_THAN(0, len);
    TEST_ASSERT_EQUAL_INT(PIO_SCHEMA_VERSION, atoi(buf));
}

/* ===== 11. Check migration — already current ===== */

void test_check_migration_current(void)
{
    char ver_str[16];
    snprintf(ver_str, sizeof(ver_str), "%d", PIO_SCHEMA_VERSION);
    sb_save("time.schema_version", ver_str);

    /* Save some other data to verify it persists */
    sb_save("time.birth.year", "1990");

    pio_check_migration();

    /* Schema version unchanged, other data intact */
    char buf[64];
    sb_load("time.schema_version", buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT(PIO_SCHEMA_VERSION, atoi(buf));

    sb_load("time.birth.year", buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("1990", buf);
}

/* ===== 12. Export — does not crash, triggers download ===== */

void test_export(void)
{
    /* Save some data */
    pio_save_birth(1995, 6, 21, 0, 0.0, 0.0);
    up_prefs_t prefs = up_default();
    prefs.visual.theme = THEME_DAWN;
    pio_save_prefs(&prefs);

    /* Export should not crash (sb_download_file is a no-op on native) */
    pio_export();

    /* Verify data is still intact after export */
    char buf[64];
    int len = sb_load("time.birth.year", buf, sizeof(buf));
    TEST_ASSERT_GREATER_THAN(0, len);
    TEST_ASSERT_EQUAL_STRING("1995", buf);
}

/* ===== 13. Null pointers in load_all ===== */

void test_load_all_null_pointers(void)
{
    /* Save some data so there's something to load */
    pio_save_birth(2000, 12, 25, 0, 0.0, 0.0);

    /* Should not crash with NULL outputs (only birth return matters) */
    int result = pio_load_all(NULL, NULL, NULL, NULL,
                              NULL, NULL, NULL, NULL, NULL, NULL);
    TEST_ASSERT_EQUAL_INT(1, result);
}

/* ===== 14. Overwrite save — second save replaces first ===== */

void test_overwrite_save(void)
{
    pio_save_birth(1990, 1, 1, 0, 0.0, 0.0);
    pio_save_birth(2000, 12, 31, 1, 45.0, 90.0);

    up_prefs_t prefs;
    ip_profile_t interests;
    ps_space_t space;
    pf_settings_t privacy;
    int by, bm, bd, bhl;
    double blat, blon;

    int result = pio_load_all(&prefs, &interests, &space, &privacy,
                              &by, &bm, &bd, &bhl, &blat, &blon);

    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_INT(2000, by);
    TEST_ASSERT_EQUAL_INT(12, bm);
    TEST_ASSERT_EQUAL_INT(31, bd);
    TEST_ASSERT_EQUAL_INT(1, bhl);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 45.0, blat);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 90.0, blon);
}

/* ===== runner ===== */

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_load_all_new_visitor);
    RUN_TEST(test_save_and_load_prefs);
    RUN_TEST(test_save_and_load_birth);
    RUN_TEST(test_save_and_load_interests);
    RUN_TEST(test_save_and_load_space);
    RUN_TEST(test_save_and_load_privacy);
    RUN_TEST(test_full_roundtrip);
    RUN_TEST(test_delete_all);
    RUN_TEST(test_partial_data);
    RUN_TEST(test_check_migration_new);
    RUN_TEST(test_check_migration_current);
    RUN_TEST(test_export);
    RUN_TEST(test_load_all_null_pointers);
    RUN_TEST(test_overwrite_save);
    return UNITY_END();
}
