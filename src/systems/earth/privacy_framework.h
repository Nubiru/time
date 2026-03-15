/* privacy_framework.h — Privacy classification, consent, and access control.
 * Foundational module for all social features. Pure functions, no state. */

#ifndef TIME_PRIVACY_FRAMEWORK_H
#define TIME_PRIVACY_FRAMEWORK_H

#define PF_FIELD_COUNT 14

typedef enum {
    PF_CLASS_PUBLIC = 0,     /* timezone, language, theme */
    PF_CLASS_PRIVATE,        /* interests, activity level */
    PF_CLASS_SENSITIVE,      /* location, birth data, profile */
    PF_CLASS_FORBIDDEN       /* real name (unless volunteered), contacts, browsing */
} pf_classification_t;

typedef enum {
    PF_CONSENT_NONE = 0,    /* anonymous visitor */
    PF_CONSENT_BASIC,       /* timezone + interests only */
    PF_CONSENT_FULL         /* location + profile + social */
} pf_consent_t;

typedef enum {
    PF_RETAIN_SESSION = 0,  /* session-only (default) */
    PF_RETAIN_LOCAL,        /* opt-in local storage */
    PF_RETAIN_SYNC          /* opt-in cloud sync (future) */
} pf_retention_t;

typedef enum {
    PF_FIELD_TIMEZONE = 0,
    PF_FIELD_LANGUAGE,
    PF_FIELD_THEME,
    PF_FIELD_INTERESTS,
    PF_FIELD_ACTIVITY_LEVEL,
    PF_FIELD_BIRTH_DATE,
    PF_FIELD_BIRTH_LOCATION,
    PF_FIELD_CURRENT_LOCATION,
    PF_FIELD_PROFILE_NAME,
    PF_FIELD_KIN_NUMBER,
    PF_FIELD_ZODIAC_SIGN,
    PF_FIELD_REAL_NAME,
    PF_FIELD_CONTACTS,
    PF_FIELD_BROWSING
} pf_field_t;

typedef struct {
    pf_consent_t consent;
    pf_retention_t retention;
    int fields_shared[PF_FIELD_COUNT];
} pf_settings_t;

typedef struct {
    int score;
    int fields_shared;
    int fields_total;
    pf_consent_t effective_consent;
} pf_score_t;

/* Create default settings: NONE consent, SESSION retention, nothing shared. */
pf_settings_t pf_default_settings(void);

/* Classify a field by sensitivity level. */
pf_classification_t pf_classify(pf_field_t field);

/* Check if a field is accessible at a given consent level. */
int pf_can_access(pf_field_t field, pf_consent_t consent);

/* Minimum consent level required for a field. */
pf_consent_t pf_consent_for_field(pf_field_t field);

/* Compute privacy score from current settings. */
pf_score_t pf_score(const pf_settings_t *settings);

/* Grant sharing of a field (requires sufficient consent, not FORBIDDEN). */
pf_settings_t pf_grant_field(pf_settings_t settings, pf_field_t field);

/* Revoke sharing of a field. */
pf_settings_t pf_revoke_field(pf_settings_t settings, pf_field_t field);

/* Set consent level. Downgrading auto-revokes excess fields. */
pf_settings_t pf_set_consent(pf_settings_t settings, pf_consent_t consent);

/* Set retention policy. */
pf_settings_t pf_set_retention(pf_settings_t settings, pf_retention_t retention);

/* GDPR: is this field exportable? */
int pf_is_exportable(pf_field_t field);

/* GDPR: is this field deletable? */
int pf_is_deletable(pf_field_t field);

/* GDPR: delete all user data. Revokes all sharing, sets consent NONE. */
pf_settings_t pf_delete_all(pf_settings_t settings);

/* Human-readable name for a classification level. */
const char *pf_classification_name(pf_classification_t cls);

/* Human-readable name for a consent level. */
const char *pf_consent_name(pf_consent_t consent);

/* Human-readable name for a retention policy. */
const char *pf_retention_name(pf_retention_t retention);

/* Human-readable name for a field. */
const char *pf_field_name(pf_field_t field);

/* List all fields at a classification level. Returns count written. */
int pf_fields_at_level(pf_classification_t cls, pf_field_t *out, int max);

#endif /* TIME_PRIVACY_FRAMEWORK_H */
