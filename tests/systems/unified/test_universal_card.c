/* test_universal_card.c — Tests for universal card text (LANGUAGE L2.1)
 *
 * TDD: universal_card provides a unified card format for all 16+ systems.
 * Each card has: title (native), subtitle (transliterated + English),
 * essence (one sentence), detail, sources. */

#include "../../unity/unity.h"
#include "../../../src/systems/unified/universal_card.h"

void setUp(void) {}
void tearDown(void) {}

/* ================================================================
 * Helper: build a ts_entry
 * ================================================================ */

static ts_entry_t make_entry(ts_system_t sys, const char *date_str,
                              const char *note, int sig)
{
    ts_entry_t e;
    memset(&e, 0, sizeof(e));
    e.system = sys;
    snprintf(e.date_str, TS_DATE_MAX, "%s", date_str);
    if (note) snprintf(e.note, TS_NOTE_MAX, "%s", note);
    e.significance = sig;
    e.active = 1;
    return e;
}

/* ================================================================
 * ucard_for_system: Gregorian produces valid card
 * ================================================================ */

void test_ucard_gregorian(void)
{
    ts_entry_t entry = make_entry(TS_SYS_GREGORIAN,
                                   "March 19, 2026", NULL, 0);
    ucard_t card = ucard_for_system(&entry);

    TEST_ASSERT_TRUE(card.title[0] != '\0');
    TEST_ASSERT_TRUE(card.essence[0] != '\0');
    TEST_ASSERT_EQUAL_INT(TS_SYS_GREGORIAN, card.system_id);
    TEST_ASSERT_EQUAL_INT(SENSITIVITY_GENERAL, card.sensitivity);
}

/* ================================================================
 * ucard_for_system: Tzolkin produces card with native title
 * ================================================================ */

void test_ucard_tzolkin(void)
{
    ts_entry_t entry = make_entry(TS_SYS_TZOLKIN,
                                   "Kin 207 Blue Crystal Hand", "Portal Day", 2);
    ucard_t card = ucard_for_system(&entry);

    TEST_ASSERT_TRUE(card.title[0] != '\0');
    TEST_ASSERT_TRUE(card.essence[0] != '\0');
    TEST_ASSERT_EQUAL_INT(TS_SYS_TZOLKIN, card.system_id);
}

/* ================================================================
 * ucard_for_system: Hebrew produces card
 * ================================================================ */

void test_ucard_hebrew(void)
{
    ts_entry_t entry = make_entry(TS_SYS_HEBREW,
                                   "5786 Adar 19", "Purim", 3);
    ucard_t card = ucard_for_system(&entry);

    TEST_ASSERT_TRUE(card.title[0] != '\0');
    TEST_ASSERT_EQUAL_INT(TS_SYS_HEBREW, card.system_id);
    /* Hebrew sacred content should be respectful+ */
    TEST_ASSERT_TRUE(card.sensitivity >= SENSITIVITY_RESPECTFUL);
}

/* ================================================================
 * ucard_for_system: Islamic produces card
 * ================================================================ */

void test_ucard_islamic(void)
{
    ts_entry_t entry = make_entry(TS_SYS_ISLAMIC,
                                   "1447 Ramadan 1", "Begin fasting", 3);
    ucard_t card = ucard_for_system(&entry);

    TEST_ASSERT_TRUE(card.title[0] != '\0');
    TEST_ASSERT_EQUAL_INT(TS_SYS_ISLAMIC, card.system_id);
    TEST_ASSERT_TRUE(card.sensitivity >= SENSITIVITY_RESPECTFUL);
}

/* ================================================================
 * ucard_for_system: Chinese produces card
 * ================================================================ */

void test_ucard_chinese(void)
{
    ts_entry_t entry = make_entry(TS_SYS_CHINESE,
                                   "Year of the Snake", "Wood Snake", 1);
    ucard_t card = ucard_for_system(&entry);

    TEST_ASSERT_TRUE(card.title[0] != '\0');
    TEST_ASSERT_EQUAL_INT(TS_SYS_CHINESE, card.system_id);
}

/* ================================================================
 * ucard_for_system: Buddhist produces card
 * ================================================================ */

void test_ucard_buddhist(void)
{
    ts_entry_t entry = make_entry(TS_SYS_BUDDHIST,
                                   "2570 BE", "Uposatha", 2);
    ucard_t card = ucard_for_system(&entry);

    TEST_ASSERT_TRUE(card.title[0] != '\0');
    TEST_ASSERT_EQUAL_INT(TS_SYS_BUDDHIST, card.system_id);
    TEST_ASSERT_TRUE(card.sensitivity >= SENSITIVITY_RESPECTFUL);
}

/* ================================================================
 * ucard_for_system: Hindu produces card
 * ================================================================ */

void test_ucard_hindu(void)
{
    ts_entry_t entry = make_entry(TS_SYS_HINDU,
                                   "Phalguna Purnima", "Holi", 3);
    ucard_t card = ucard_for_system(&entry);

    TEST_ASSERT_TRUE(card.title[0] != '\0');
    TEST_ASSERT_EQUAL_INT(TS_SYS_HINDU, card.system_id);
    TEST_ASSERT_TRUE(card.sensitivity >= SENSITIVITY_RESPECTFUL);
}

/* ================================================================
 * ucard_for_system: I Ching produces card
 * ================================================================ */

void test_ucard_iching(void)
{
    ts_entry_t entry = make_entry(TS_SYS_ICHING,
                                   "Hexagram 24: Return", NULL, 1);
    ucard_t card = ucard_for_system(&entry);

    TEST_ASSERT_TRUE(card.title[0] != '\0');
    TEST_ASSERT_EQUAL_INT(TS_SYS_ICHING, card.system_id);
}

/* ================================================================
 * ucard_for_system: Astrology produces card
 * ================================================================ */

void test_ucard_astrology(void)
{
    ts_entry_t entry = make_entry(TS_SYS_ASTROLOGY,
                                   "Sun in Pisces", NULL, 0);
    ucard_t card = ucard_for_system(&entry);

    TEST_ASSERT_TRUE(card.title[0] != '\0');
    TEST_ASSERT_EQUAL_INT(TS_SYS_ASTROLOGY, card.system_id);
}

/* ================================================================
 * ucard_for_system: Human Design produces card
 * ================================================================ */

void test_ucard_human_design(void)
{
    ts_entry_t entry = make_entry(TS_SYS_HUMAN_DESIGN,
                                   "Gate 47 Line 2", NULL, 1);
    ucard_t card = ucard_for_system(&entry);

    TEST_ASSERT_TRUE(card.title[0] != '\0');
    TEST_ASSERT_EQUAL_INT(TS_SYS_HUMAN_DESIGN, card.system_id);
}

/* ================================================================
 * ucard_for_system: Kabbalah produces card
 * ================================================================ */

void test_ucard_kabbalah(void)
{
    ts_entry_t entry = make_entry(TS_SYS_KABBALAH,
                                   "Tiferet", "Beauty", 2);
    ucard_t card = ucard_for_system(&entry);

    TEST_ASSERT_TRUE(card.title[0] != '\0');
    TEST_ASSERT_EQUAL_INT(TS_SYS_KABBALAH, card.system_id);
    TEST_ASSERT_TRUE(card.sensitivity >= SENSITIVITY_RESPECTFUL);
}

/* ================================================================
 * ucard_for_system: Geological produces card
 * ================================================================ */

void test_ucard_geological(void)
{
    ts_entry_t entry = make_entry(TS_SYS_GEOLOGICAL,
                                   "Holocene Epoch", NULL, 0);
    ucard_t card = ucard_for_system(&entry);

    TEST_ASSERT_TRUE(card.title[0] != '\0');
    TEST_ASSERT_EQUAL_INT(TS_SYS_GEOLOGICAL, card.system_id);
    TEST_ASSERT_EQUAL_INT(SENSITIVITY_GENERAL, card.sensitivity);
}

/* ================================================================
 * ucard_for_system: Cosmic produces card
 * ================================================================ */

void test_ucard_cosmic(void)
{
    ts_entry_t entry = make_entry(TS_SYS_COSMIC,
                                   "13.8 Gya", "Stelliferous era", 0);
    ucard_t card = ucard_for_system(&entry);

    TEST_ASSERT_TRUE(card.title[0] != '\0');
    TEST_ASSERT_EQUAL_INT(TS_SYS_COSMIC, card.system_id);
}

/* ================================================================
 * ucard_for_system: NULL entry returns empty card
 * ================================================================ */

void test_ucard_null_entry(void)
{
    ucard_t card = ucard_for_system(NULL);
    TEST_ASSERT_EQUAL_STRING("", card.title);
    TEST_ASSERT_EQUAL_INT(-1, card.system_id);
}

/* ================================================================
 * ucard_for_system: inactive entry returns empty card
 * ================================================================ */

void test_ucard_inactive_entry(void)
{
    ts_entry_t entry = make_entry(TS_SYS_HEBREW, "5786", NULL, 0);
    entry.active = 0;
    ucard_t card = ucard_for_system(&entry);
    TEST_ASSERT_EQUAL_STRING("", card.title);
}

/* ================================================================
 * ucard_system_name: all known systems
 * ================================================================ */

void test_ucard_system_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Gregorian", ucard_system_name(TS_SYS_GREGORIAN));
    TEST_ASSERT_EQUAL_STRING("Tzolkin", ucard_system_name(TS_SYS_TZOLKIN));
    TEST_ASSERT_EQUAL_STRING("Hebrew", ucard_system_name(TS_SYS_HEBREW));
    TEST_ASSERT_EQUAL_STRING("?", ucard_system_name(-1));
    TEST_ASSERT_EQUAL_STRING("?", ucard_system_name(99));
}

/* ================================================================
 * ucard_format_brief: compact format
 * ================================================================ */

void test_ucard_format_brief(void)
{
    ts_entry_t entry = make_entry(TS_SYS_TZOLKIN,
                                   "Kin 207 Blue Crystal Hand", "Portal Day", 2);
    ucard_t card = ucard_for_system(&entry);

    char buf[256];
    int len = ucard_format_brief(&card, buf, sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    TEST_ASSERT_TRUE(buf[0] != '\0');
}

/* ================================================================
 * ucard_format_full: expanded format
 * ================================================================ */

void test_ucard_format_full(void)
{
    ts_entry_t entry = make_entry(TS_SYS_HEBREW,
                                   "5786 Adar 19", "Purim", 3);
    ucard_t card = ucard_for_system(&entry);

    char buf[1024];
    int len = ucard_format_full(&card, buf, sizeof(buf));
    TEST_ASSERT_TRUE(len > 0);
    /* Full format should include essence */
    TEST_ASSERT_NOT_NULL(strstr(buf, card.essence));
}

/* ================================================================
 * ucard_for_system: Haab produces card
 * ================================================================ */

void test_ucard_haab(void)
{
    ts_entry_t entry = make_entry(TS_SYS_HAAB,
                                   "12 Kumk'u", NULL, 0);
    ucard_t card = ucard_for_system(&entry);

    TEST_ASSERT_TRUE(card.title[0] != '\0');
    TEST_ASSERT_EQUAL_INT(TS_SYS_HAAB, card.system_id);
}

/* ================================================================
 * Runner
 * ================================================================ */

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_ucard_gregorian);
    RUN_TEST(test_ucard_tzolkin);
    RUN_TEST(test_ucard_hebrew);
    RUN_TEST(test_ucard_islamic);
    RUN_TEST(test_ucard_chinese);
    RUN_TEST(test_ucard_buddhist);
    RUN_TEST(test_ucard_hindu);
    RUN_TEST(test_ucard_iching);
    RUN_TEST(test_ucard_astrology);
    RUN_TEST(test_ucard_human_design);
    RUN_TEST(test_ucard_kabbalah);
    RUN_TEST(test_ucard_geological);
    RUN_TEST(test_ucard_cosmic);
    RUN_TEST(test_ucard_null_entry);
    RUN_TEST(test_ucard_inactive_entry);
    RUN_TEST(test_ucard_system_name);
    RUN_TEST(test_ucard_format_brief);
    RUN_TEST(test_ucard_format_full);
    RUN_TEST(test_ucard_haab);

    return UNITY_END();
}
