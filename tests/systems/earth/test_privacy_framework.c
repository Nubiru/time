/* test_privacy_framework.c — Tests for privacy framework module.
 * TDD: RED phase first, then GREEN with implementation. */

#include "unity.h"
#include "systems/earth/privacy_framework.h"

void setUp(void) {}
void tearDown(void) {}

/* --- Default settings --- */

void test_default_settings_consent_none(void) {
    pf_settings_t s = pf_default_settings();
    TEST_ASSERT_EQUAL_INT(PF_CONSENT_NONE, s.consent);
}

void test_default_settings_retention_session(void) {
    pf_settings_t s = pf_default_settings();
    TEST_ASSERT_EQUAL_INT(PF_RETAIN_SESSION, s.retention);
}

void test_default_settings_no_fields_shared(void) {
    pf_settings_t s = pf_default_settings();
    for (int i = 0; i < PF_FIELD_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(0, s.fields_shared[i]);
    }
}

/* --- Classification --- */

void test_classify_public_timezone(void) {
    TEST_ASSERT_EQUAL_INT(PF_CLASS_PUBLIC, pf_classify(PF_FIELD_TIMEZONE));
}

void test_classify_public_language(void) {
    TEST_ASSERT_EQUAL_INT(PF_CLASS_PUBLIC, pf_classify(PF_FIELD_LANGUAGE));
}

void test_classify_public_theme(void) {
    TEST_ASSERT_EQUAL_INT(PF_CLASS_PUBLIC, pf_classify(PF_FIELD_THEME));
}

void test_classify_private_interests(void) {
    TEST_ASSERT_EQUAL_INT(PF_CLASS_PRIVATE, pf_classify(PF_FIELD_INTERESTS));
}

void test_classify_private_activity(void) {
    TEST_ASSERT_EQUAL_INT(PF_CLASS_PRIVATE, pf_classify(PF_FIELD_ACTIVITY_LEVEL));
}

void test_classify_sensitive_birth_date(void) {
    TEST_ASSERT_EQUAL_INT(PF_CLASS_SENSITIVE, pf_classify(PF_FIELD_BIRTH_DATE));
}

void test_classify_sensitive_current_location(void) {
    TEST_ASSERT_EQUAL_INT(PF_CLASS_SENSITIVE, pf_classify(PF_FIELD_CURRENT_LOCATION));
}

void test_classify_forbidden_real_name(void) {
    TEST_ASSERT_EQUAL_INT(PF_CLASS_FORBIDDEN, pf_classify(PF_FIELD_REAL_NAME));
}

void test_classify_forbidden_contacts(void) {
    TEST_ASSERT_EQUAL_INT(PF_CLASS_FORBIDDEN, pf_classify(PF_FIELD_CONTACTS));
}

void test_classify_forbidden_browsing(void) {
    TEST_ASSERT_EQUAL_INT(PF_CLASS_FORBIDDEN, pf_classify(PF_FIELD_BROWSING));
}

/* --- Access control --- */

void test_access_none_denies_public(void) {
    TEST_ASSERT_EQUAL_INT(0, pf_can_access(PF_FIELD_TIMEZONE, PF_CONSENT_NONE));
}

void test_access_basic_allows_public(void) {
    TEST_ASSERT_EQUAL_INT(1, pf_can_access(PF_FIELD_TIMEZONE, PF_CONSENT_BASIC));
}

void test_access_basic_denies_private(void) {
    TEST_ASSERT_EQUAL_INT(0, pf_can_access(PF_FIELD_INTERESTS, PF_CONSENT_BASIC));
}

void test_access_full_allows_private(void) {
    TEST_ASSERT_EQUAL_INT(1, pf_can_access(PF_FIELD_INTERESTS, PF_CONSENT_FULL));
}

void test_access_full_allows_sensitive(void) {
    TEST_ASSERT_EQUAL_INT(1, pf_can_access(PF_FIELD_BIRTH_DATE, PF_CONSENT_FULL));
}

void test_access_full_denies_forbidden(void) {
    TEST_ASSERT_EQUAL_INT(0, pf_can_access(PF_FIELD_REAL_NAME, PF_CONSENT_FULL));
}

/* --- Consent for field --- */

void test_consent_for_public_field(void) {
    TEST_ASSERT_EQUAL_INT(PF_CONSENT_BASIC, pf_consent_for_field(PF_FIELD_TIMEZONE));
}

void test_consent_for_private_field(void) {
    TEST_ASSERT_EQUAL_INT(PF_CONSENT_FULL, pf_consent_for_field(PF_FIELD_INTERESTS));
}

void test_consent_for_sensitive_field(void) {
    TEST_ASSERT_EQUAL_INT(PF_CONSENT_FULL, pf_consent_for_field(PF_FIELD_BIRTH_DATE));
}

/* --- Grant and revoke --- */

void test_grant_field_with_sufficient_consent(void) {
    pf_settings_t s = pf_default_settings();
    s = pf_set_consent(s, PF_CONSENT_BASIC);
    s = pf_grant_field(s, PF_FIELD_TIMEZONE);
    TEST_ASSERT_EQUAL_INT(1, s.fields_shared[PF_FIELD_TIMEZONE]);
}

void test_grant_field_denied_without_consent(void) {
    pf_settings_t s = pf_default_settings();
    /* consent is NONE — cannot grant anything */
    s = pf_grant_field(s, PF_FIELD_TIMEZONE);
    TEST_ASSERT_EQUAL_INT(0, s.fields_shared[PF_FIELD_TIMEZONE]);
}

void test_grant_forbidden_field_always_denied(void) {
    pf_settings_t s = pf_default_settings();
    s = pf_set_consent(s, PF_CONSENT_FULL);
    s = pf_grant_field(s, PF_FIELD_REAL_NAME);
    TEST_ASSERT_EQUAL_INT(0, s.fields_shared[PF_FIELD_REAL_NAME]);
}

void test_revoke_field(void) {
    pf_settings_t s = pf_default_settings();
    s = pf_set_consent(s, PF_CONSENT_BASIC);
    s = pf_grant_field(s, PF_FIELD_TIMEZONE);
    TEST_ASSERT_EQUAL_INT(1, s.fields_shared[PF_FIELD_TIMEZONE]);
    s = pf_revoke_field(s, PF_FIELD_TIMEZONE);
    TEST_ASSERT_EQUAL_INT(0, s.fields_shared[PF_FIELD_TIMEZONE]);
}

/* --- Consent changes --- */

void test_set_consent_upgrade(void) {
    pf_settings_t s = pf_default_settings();
    s = pf_set_consent(s, PF_CONSENT_FULL);
    TEST_ASSERT_EQUAL_INT(PF_CONSENT_FULL, s.consent);
}

void test_set_consent_downgrade_revokes_excess(void) {
    pf_settings_t s = pf_default_settings();
    s = pf_set_consent(s, PF_CONSENT_FULL);
    s = pf_grant_field(s, PF_FIELD_INTERESTS);  /* PRIVATE — needs FULL */
    s = pf_grant_field(s, PF_FIELD_TIMEZONE);    /* PUBLIC — needs BASIC */
    /* Downgrade to BASIC */
    s = pf_set_consent(s, PF_CONSENT_BASIC);
    TEST_ASSERT_EQUAL_INT(0, s.fields_shared[PF_FIELD_INTERESTS]); /* revoked */
    TEST_ASSERT_EQUAL_INT(1, s.fields_shared[PF_FIELD_TIMEZONE]);  /* kept */
}

void test_set_consent_to_none_revokes_all(void) {
    pf_settings_t s = pf_default_settings();
    s = pf_set_consent(s, PF_CONSENT_FULL);
    s = pf_grant_field(s, PF_FIELD_TIMEZONE);
    s = pf_grant_field(s, PF_FIELD_BIRTH_DATE);
    s = pf_set_consent(s, PF_CONSENT_NONE);
    for (int i = 0; i < PF_FIELD_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(0, s.fields_shared[i]);
    }
}

/* --- Retention --- */

void test_set_retention(void) {
    pf_settings_t s = pf_default_settings();
    s = pf_set_retention(s, PF_RETAIN_LOCAL);
    TEST_ASSERT_EQUAL_INT(PF_RETAIN_LOCAL, s.retention);
}

/* --- Privacy score --- */

void test_score_default_zero(void) {
    pf_settings_t s = pf_default_settings();
    pf_score_t sc = pf_score(&s);
    TEST_ASSERT_EQUAL_INT(0, sc.score);
    TEST_ASSERT_EQUAL_INT(0, sc.fields_shared);
    TEST_ASSERT_EQUAL_INT(PF_FIELD_COUNT, sc.fields_total);
}

void test_score_with_shared_fields(void) {
    pf_settings_t s = pf_default_settings();
    s = pf_set_consent(s, PF_CONSENT_FULL);
    s = pf_grant_field(s, PF_FIELD_TIMEZONE);
    s = pf_grant_field(s, PF_FIELD_INTERESTS);
    pf_score_t sc = pf_score(&s);
    TEST_ASSERT_EQUAL_INT(2, sc.fields_shared);
    /* shareable = PF_FIELD_COUNT - 3 forbidden = 11 */
    /* score = (2 / 11) * 100 = 18 */
    TEST_ASSERT_EQUAL_INT(18, sc.score);
}

void test_score_effective_consent(void) {
    pf_settings_t s = pf_default_settings();
    s = pf_set_consent(s, PF_CONSENT_FULL);
    pf_score_t sc = pf_score(&s);
    TEST_ASSERT_EQUAL_INT(PF_CONSENT_FULL, sc.effective_consent);
}

/* --- GDPR: export and delete --- */

void test_exportable_user_field(void) {
    TEST_ASSERT_EQUAL_INT(1, pf_is_exportable(PF_FIELD_BIRTH_DATE));
}

void test_exportable_all_user_fields(void) {
    /* All fields are exportable (user-provided or user-controlled) */
    for (int i = 0; i < PF_FIELD_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(1, pf_is_exportable((pf_field_t)i));
    }
}

void test_deletable_user_field(void) {
    TEST_ASSERT_EQUAL_INT(1, pf_is_deletable(PF_FIELD_INTERESTS));
}

void test_delete_all_revokes_everything(void) {
    pf_settings_t s = pf_default_settings();
    s = pf_set_consent(s, PF_CONSENT_FULL);
    s = pf_grant_field(s, PF_FIELD_TIMEZONE);
    s = pf_grant_field(s, PF_FIELD_BIRTH_DATE);
    s = pf_set_retention(s, PF_RETAIN_LOCAL);
    s = pf_delete_all(s);
    TEST_ASSERT_EQUAL_INT(PF_CONSENT_NONE, s.consent);
    TEST_ASSERT_EQUAL_INT(PF_RETAIN_LOCAL, s.retention); /* retention preserved */
    for (int i = 0; i < PF_FIELD_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(0, s.fields_shared[i]);
    }
}

/* --- Name strings --- */

void test_classification_name_public(void) {
    const char *name = pf_classification_name(PF_CLASS_PUBLIC);
    TEST_ASSERT_EQUAL_STRING("public", name);
}

void test_classification_name_forbidden(void) {
    const char *name = pf_classification_name(PF_CLASS_FORBIDDEN);
    TEST_ASSERT_EQUAL_STRING("forbidden", name);
}

void test_consent_name_none(void) {
    TEST_ASSERT_EQUAL_STRING("none", pf_consent_name(PF_CONSENT_NONE));
}

void test_consent_name_full(void) {
    TEST_ASSERT_EQUAL_STRING("full", pf_consent_name(PF_CONSENT_FULL));
}

void test_retention_name_session(void) {
    TEST_ASSERT_EQUAL_STRING("session", pf_retention_name(PF_RETAIN_SESSION));
}

void test_retention_name_sync(void) {
    TEST_ASSERT_EQUAL_STRING("sync", pf_retention_name(PF_RETAIN_SYNC));
}

void test_field_name_timezone(void) {
    TEST_ASSERT_EQUAL_STRING("timezone", pf_field_name(PF_FIELD_TIMEZONE));
}

void test_field_name_browsing(void) {
    TEST_ASSERT_EQUAL_STRING("browsing", pf_field_name(PF_FIELD_BROWSING));
}

/* --- Fields at level --- */

void test_fields_at_public_level(void) {
    pf_field_t out[PF_FIELD_COUNT];
    int count = pf_fields_at_level(PF_CLASS_PUBLIC, out, PF_FIELD_COUNT);
    TEST_ASSERT_EQUAL_INT(3, count);
    /* Should contain timezone, language, theme */
    int found_tz = 0, found_lang = 0, found_theme = 0;
    for (int i = 0; i < count; i++) {
        if (out[i] == PF_FIELD_TIMEZONE) found_tz = 1;
        if (out[i] == PF_FIELD_LANGUAGE) found_lang = 1;
        if (out[i] == PF_FIELD_THEME) found_theme = 1;
    }
    TEST_ASSERT_EQUAL_INT(1, found_tz);
    TEST_ASSERT_EQUAL_INT(1, found_lang);
    TEST_ASSERT_EQUAL_INT(1, found_theme);
}

void test_fields_at_forbidden_level(void) {
    pf_field_t out[PF_FIELD_COUNT];
    int count = pf_fields_at_level(PF_CLASS_FORBIDDEN, out, PF_FIELD_COUNT);
    TEST_ASSERT_EQUAL_INT(3, count);
}

void test_fields_at_level_max_limit(void) {
    pf_field_t out[2];
    int count = pf_fields_at_level(PF_CLASS_SENSITIVE, out, 2);
    /* SENSITIVE has 6 fields, but max is 2 */
    TEST_ASSERT_EQUAL_INT(2, count);
}

/* --- Edge cases --- */

void test_grant_basic_field_with_full_consent(void) {
    /* FULL consent should allow granting PUBLIC fields */
    pf_settings_t s = pf_default_settings();
    s = pf_set_consent(s, PF_CONSENT_FULL);
    s = pf_grant_field(s, PF_FIELD_LANGUAGE);
    TEST_ASSERT_EQUAL_INT(1, s.fields_shared[PF_FIELD_LANGUAGE]);
}

void test_double_grant_idempotent(void) {
    pf_settings_t s = pf_default_settings();
    s = pf_set_consent(s, PF_CONSENT_BASIC);
    s = pf_grant_field(s, PF_FIELD_TIMEZONE);
    s = pf_grant_field(s, PF_FIELD_TIMEZONE);
    TEST_ASSERT_EQUAL_INT(1, s.fields_shared[PF_FIELD_TIMEZONE]);
}

void test_revoke_unshared_field_noop(void) {
    pf_settings_t s = pf_default_settings();
    s = pf_revoke_field(s, PF_FIELD_TIMEZONE);
    TEST_ASSERT_EQUAL_INT(0, s.fields_shared[PF_FIELD_TIMEZONE]);
}

void test_score_all_shareable_fields_shared(void) {
    pf_settings_t s = pf_default_settings();
    s = pf_set_consent(s, PF_CONSENT_FULL);
    /* Grant all non-forbidden fields */
    for (int i = 0; i < PF_FIELD_COUNT; i++) {
        s = pf_grant_field(s, (pf_field_t)i);
    }
    pf_score_t sc = pf_score(&s);
    /* 11 shareable fields (14 - 3 forbidden) */
    TEST_ASSERT_EQUAL_INT(11, sc.fields_shared);
    TEST_ASSERT_EQUAL_INT(100, sc.score);
}

int main(void) {
    UNITY_BEGIN();

    /* Default settings (3) */
    RUN_TEST(test_default_settings_consent_none);
    RUN_TEST(test_default_settings_retention_session);
    RUN_TEST(test_default_settings_no_fields_shared);

    /* Classification (7) */
    RUN_TEST(test_classify_public_timezone);
    RUN_TEST(test_classify_public_language);
    RUN_TEST(test_classify_public_theme);
    RUN_TEST(test_classify_private_interests);
    RUN_TEST(test_classify_private_activity);
    RUN_TEST(test_classify_sensitive_birth_date);
    RUN_TEST(test_classify_sensitive_current_location);
    RUN_TEST(test_classify_forbidden_real_name);
    RUN_TEST(test_classify_forbidden_contacts);
    RUN_TEST(test_classify_forbidden_browsing);

    /* Access control (6) */
    RUN_TEST(test_access_none_denies_public);
    RUN_TEST(test_access_basic_allows_public);
    RUN_TEST(test_access_basic_denies_private);
    RUN_TEST(test_access_full_allows_private);
    RUN_TEST(test_access_full_allows_sensitive);
    RUN_TEST(test_access_full_denies_forbidden);

    /* Consent for field (3) */
    RUN_TEST(test_consent_for_public_field);
    RUN_TEST(test_consent_for_private_field);
    RUN_TEST(test_consent_for_sensitive_field);

    /* Grant and revoke (4) */
    RUN_TEST(test_grant_field_with_sufficient_consent);
    RUN_TEST(test_grant_field_denied_without_consent);
    RUN_TEST(test_grant_forbidden_field_always_denied);
    RUN_TEST(test_revoke_field);

    /* Consent changes (3) */
    RUN_TEST(test_set_consent_upgrade);
    RUN_TEST(test_set_consent_downgrade_revokes_excess);
    RUN_TEST(test_set_consent_to_none_revokes_all);

    /* Retention (1) */
    RUN_TEST(test_set_retention);

    /* Privacy score (3) */
    RUN_TEST(test_score_default_zero);
    RUN_TEST(test_score_with_shared_fields);
    RUN_TEST(test_score_effective_consent);

    /* GDPR (4) */
    RUN_TEST(test_exportable_user_field);
    RUN_TEST(test_exportable_all_user_fields);
    RUN_TEST(test_deletable_user_field);
    RUN_TEST(test_delete_all_revokes_everything);

    /* Name strings (8) */
    RUN_TEST(test_classification_name_public);
    RUN_TEST(test_classification_name_forbidden);
    RUN_TEST(test_consent_name_none);
    RUN_TEST(test_consent_name_full);
    RUN_TEST(test_retention_name_session);
    RUN_TEST(test_retention_name_sync);
    RUN_TEST(test_field_name_timezone);
    RUN_TEST(test_field_name_browsing);

    /* Fields at level (3) */
    RUN_TEST(test_fields_at_public_level);
    RUN_TEST(test_fields_at_forbidden_level);
    RUN_TEST(test_fields_at_level_max_limit);

    /* Edge cases (4) */
    RUN_TEST(test_grant_basic_field_with_full_consent);
    RUN_TEST(test_double_grant_idempotent);
    RUN_TEST(test_revoke_unshared_field_noop);
    RUN_TEST(test_score_all_shareable_fields_shared);

    return UNITY_END();
}
