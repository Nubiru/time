/* test_storage_bridge.c — Tests for native in-memory storage bridge. */

#include "unity.h"
#include "platform/storage_bridge.h"
#include <string.h>

void setUp(void) { sb_reset(); }
void tearDown(void) { (void)0; }

/* 1. Save and load returns same value. */
void test_save_and_load(void)
{
    char buf[256];
    sb_save("time.theme", "dark");
    int n = sb_load("time.theme", buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("dark", buf);
    TEST_ASSERT_EQUAL_INT(4, n);
}

/* 2. Load missing key returns 0. */
void test_load_missing_key(void)
{
    char buf[256];
    int n = sb_load("nonexistent", buf, sizeof(buf));
    TEST_ASSERT_EQUAL_INT(0, n);
}

/* 3. Exists returns 1 after save. */
void test_exists_present(void)
{
    sb_save("time.lat", "40.7");
    TEST_ASSERT_EQUAL_INT(1, sb_exists("time.lat"));
}

/* 4. Exists returns 0 for unknown key. */
void test_exists_missing(void)
{
    TEST_ASSERT_EQUAL_INT(0, sb_exists("unknown"));
}

/* 5. Delete then exists returns 0. */
void test_delete_key(void)
{
    sb_save("time.zoom", "3");
    sb_delete("time.zoom");
    TEST_ASSERT_EQUAL_INT(0, sb_exists("time.zoom"));
    TEST_ASSERT_EQUAL_INT(0, sb_count());
}

/* 6. delete_all removes all time.* keys. */
void test_delete_all(void)
{
    sb_save("time.a", "1");
    sb_save("time.b", "2");
    sb_save("time.c", "3");
    sb_delete_all();
    TEST_ASSERT_EQUAL_INT(0, sb_count());
    TEST_ASSERT_EQUAL_INT(0, sb_exists("time.a"));
    TEST_ASSERT_EQUAL_INT(0, sb_exists("time.b"));
    TEST_ASSERT_EQUAL_INT(0, sb_exists("time.c"));
}

/* 7. delete_all keeps non-time.* keys. */
void test_delete_all_keeps_non_time(void)
{
    sb_save("time.x", "1");
    sb_save("other.key", "keep");
    sb_delete_all();
    TEST_ASSERT_EQUAL_INT(1, sb_count());
    TEST_ASSERT_EQUAL_INT(1, sb_exists("other.key"));

    char buf[256];
    sb_load("other.key", buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("keep", buf);
}

/* 8. Overwrite — save key twice, load returns latest. */
void test_overwrite(void)
{
    char buf[256];
    sb_save("time.mode", "old");
    sb_save("time.mode", "new");
    sb_load("time.mode", buf, sizeof(buf));
    TEST_ASSERT_EQUAL_STRING("new", buf);
    TEST_ASSERT_EQUAL_INT(1, sb_count());
}

/* 9. Count matches number of saved keys. */
void test_count(void)
{
    TEST_ASSERT_EQUAL_INT(0, sb_count());
    sb_save("a", "1");
    TEST_ASSERT_EQUAL_INT(1, sb_count());
    sb_save("b", "2");
    TEST_ASSERT_EQUAL_INT(2, sb_count());
    sb_save("c", "3");
    TEST_ASSERT_EQUAL_INT(3, sb_count());
    sb_delete("b");
    TEST_ASSERT_EQUAL_INT(2, sb_count());
}

/* 10. Export JSON contains saved keys. */
void test_export_json(void)
{
    char buf[1024];
    sb_save("k1", "v1");
    sb_save("k2", "v2");
    int n = sb_export_json(buf, sizeof(buf));
    TEST_ASSERT_GREATER_THAN(0, n);

    /* Must start with { and end with } */
    TEST_ASSERT_EQUAL_CHAR('{', buf[0]);
    TEST_ASSERT_EQUAL_CHAR('}', buf[n - 1]);

    /* Must contain both key-value pairs. */
    TEST_ASSERT_NOT_NULL(strstr(buf, "\"k1\":\"v1\""));
    TEST_ASSERT_NOT_NULL(strstr(buf, "\"k2\":\"v2\""));
}

/* 11. Reset clears everything. */
void test_reset(void)
{
    sb_save("time.a", "1");
    sb_save("time.b", "2");
    sb_reset();
    TEST_ASSERT_EQUAL_INT(0, sb_count());
    TEST_ASSERT_EQUAL_INT(0, sb_exists("time.a"));
    TEST_ASSERT_EQUAL_INT(0, sb_exists("time.b"));
}

/* 12. Geolocation callback fires with success. */
static int s_geo_called;
static int s_geo_success;
static double s_geo_lat;
static double s_geo_lon;

static void geo_cb(int success, double lat, double lon)
{
    s_geo_called  = 1;
    s_geo_success = success;
    s_geo_lat     = lat;
    s_geo_lon     = lon;
}

void test_geolocation_callback(void)
{
    s_geo_called = 0;
    sb_request_geolocation(geo_cb);
    TEST_ASSERT_EQUAL_INT(1, s_geo_called);
    TEST_ASSERT_EQUAL_INT(1, s_geo_success);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)s_geo_lat);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.0f, (float)s_geo_lon);
}

/* 13. download_file is a no-op that doesn't crash. */
void test_download_file_noop(void)
{
    sb_download_file("test.json", "{}", 2);
    sb_download_file(NULL, NULL, 0);
    /* If we get here, it didn't crash. */
    TEST_ASSERT_TRUE(1);
}

/* 14. Load with small buffer truncates safely. */
void test_load_buffer_too_small(void)
{
    char buf[4];
    sb_save("time.long", "abcdefghij");
    int n = sb_load("time.long", buf, sizeof(buf));
    /* Should truncate to "abc" (3 chars + null). */
    TEST_ASSERT_EQUAL_INT(3, n);
    TEST_ASSERT_EQUAL_STRING("abc", buf);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_save_and_load);
    RUN_TEST(test_load_missing_key);
    RUN_TEST(test_exists_present);
    RUN_TEST(test_exists_missing);
    RUN_TEST(test_delete_key);
    RUN_TEST(test_delete_all);
    RUN_TEST(test_delete_all_keeps_non_time);
    RUN_TEST(test_overwrite);
    RUN_TEST(test_count);
    RUN_TEST(test_export_json);
    RUN_TEST(test_reset);
    RUN_TEST(test_geolocation_callback);
    RUN_TEST(test_download_file_noop);
    RUN_TEST(test_load_buffer_too_small);
    return UNITY_END();
}
