/* privacy_framework.c — Privacy classification, consent, and access control.
 * Pure functions. No GL, no malloc, no globals, no side effects. */

#include "privacy_framework.h"

/* --- Static classification table --- */

static const pf_classification_t s_field_class[PF_FIELD_COUNT] = {
    PF_CLASS_PUBLIC,     /* PF_FIELD_TIMEZONE */
    PF_CLASS_PUBLIC,     /* PF_FIELD_LANGUAGE */
    PF_CLASS_PUBLIC,     /* PF_FIELD_THEME */
    PF_CLASS_PRIVATE,    /* PF_FIELD_INTERESTS */
    PF_CLASS_PRIVATE,    /* PF_FIELD_ACTIVITY_LEVEL */
    PF_CLASS_SENSITIVE,  /* PF_FIELD_BIRTH_DATE */
    PF_CLASS_SENSITIVE,  /* PF_FIELD_BIRTH_LOCATION */
    PF_CLASS_SENSITIVE,  /* PF_FIELD_CURRENT_LOCATION */
    PF_CLASS_SENSITIVE,  /* PF_FIELD_PROFILE_NAME */
    PF_CLASS_SENSITIVE,  /* PF_FIELD_KIN_NUMBER */
    PF_CLASS_SENSITIVE,  /* PF_FIELD_ZODIAC_SIGN */
    PF_CLASS_FORBIDDEN,  /* PF_FIELD_REAL_NAME */
    PF_CLASS_FORBIDDEN,  /* PF_FIELD_CONTACTS */
    PF_CLASS_FORBIDDEN   /* PF_FIELD_BROWSING */
};

/* --- Name tables --- */

static const char *s_class_names[] = {
    "public", "private", "sensitive", "forbidden"
};

static const char *s_consent_names[] = {
    "none", "basic", "full"
};

static const char *s_retention_names[] = {
    "session", "local", "sync"
};

static const char *s_field_names[PF_FIELD_COUNT] = {
    "timezone", "language", "theme",
    "interests", "activity_level",
    "birth_date", "birth_location", "current_location",
    "profile_name", "kin_number", "zodiac_sign",
    "real_name", "contacts", "browsing"
};

/* --- Implementation --- */

pf_settings_t pf_default_settings(void) {
    pf_settings_t s;
    s.consent = PF_CONSENT_NONE;
    s.retention = PF_RETAIN_SESSION;
    for (int i = 0; i < PF_FIELD_COUNT; i++) {
        s.fields_shared[i] = 0;
    }
    return s;
}

pf_classification_t pf_classify(pf_field_t field) {
    if (field < 0 || field >= PF_FIELD_COUNT) {
        return PF_CLASS_FORBIDDEN;
    }
    return s_field_class[field];
}

int pf_can_access(pf_field_t field, pf_consent_t consent) {
    pf_classification_t cls = pf_classify(field);
    if (cls == PF_CLASS_FORBIDDEN) {
        return 0;
    }
    if (consent == PF_CONSENT_NONE) {
        return 0;
    }
    if (consent == PF_CONSENT_BASIC) {
        return cls == PF_CLASS_PUBLIC ? 1 : 0;
    }
    /* PF_CONSENT_FULL — allows PUBLIC, PRIVATE, SENSITIVE */
    return 1;
}

pf_consent_t pf_consent_for_field(pf_field_t field) {
    pf_classification_t cls = pf_classify(field);
    switch (cls) {
        case PF_CLASS_PUBLIC:    return PF_CONSENT_BASIC;
        case PF_CLASS_PRIVATE:   return PF_CONSENT_FULL;
        case PF_CLASS_SENSITIVE: return PF_CONSENT_FULL;
        case PF_CLASS_FORBIDDEN: return PF_CONSENT_FULL;
    }
    return PF_CONSENT_FULL;
}

pf_score_t pf_score(const pf_settings_t *settings) {
    pf_score_t result;
    result.fields_total = PF_FIELD_COUNT;
    result.effective_consent = settings->consent;
    result.fields_shared = 0;

    int shareable = 0;
    for (int i = 0; i < PF_FIELD_COUNT; i++) {
        if (s_field_class[i] != PF_CLASS_FORBIDDEN) {
            shareable++;
            if (settings->fields_shared[i]) {
                result.fields_shared++;
            }
        }
    }

    if (shareable > 0) {
        result.score = (result.fields_shared * 100) / shareable;
    } else {
        result.score = 0;
    }

    return result;
}

pf_settings_t pf_grant_field(pf_settings_t settings, pf_field_t field) {
    if (field < 0 || field >= PF_FIELD_COUNT) {
        return settings;
    }
    /* FORBIDDEN fields can never be granted */
    if (s_field_class[field] == PF_CLASS_FORBIDDEN) {
        return settings;
    }
    /* Check consent is sufficient */
    if (!pf_can_access(field, settings.consent)) {
        return settings;
    }
    settings.fields_shared[field] = 1;
    return settings;
}

pf_settings_t pf_revoke_field(pf_settings_t settings, pf_field_t field) {
    if (field < 0 || field >= PF_FIELD_COUNT) {
        return settings;
    }
    settings.fields_shared[field] = 0;
    return settings;
}

pf_settings_t pf_set_consent(pf_settings_t settings, pf_consent_t consent) {
    settings.consent = consent;
    /* When downgrading, revoke fields that exceed the new consent level */
    for (int i = 0; i < PF_FIELD_COUNT; i++) {
        if (settings.fields_shared[i] && !pf_can_access((pf_field_t)i, consent)) {
            settings.fields_shared[i] = 0;
        }
    }
    return settings;
}

pf_settings_t pf_set_retention(pf_settings_t settings, pf_retention_t retention) {
    settings.retention = retention;
    return settings;
}

int pf_is_exportable(pf_field_t field) {
    /* All user data fields are exportable (GDPR right of access) */
    return (field >= 0 && field < PF_FIELD_COUNT) ? 1 : 0;
}

int pf_is_deletable(pf_field_t field) {
    /* All user data fields are deletable (GDPR right to erasure) */
    return (field >= 0 && field < PF_FIELD_COUNT) ? 1 : 0;
}

pf_settings_t pf_delete_all(pf_settings_t settings) {
    settings.consent = PF_CONSENT_NONE;
    /* Retention policy preserved — user may want to know it was set */
    for (int i = 0; i < PF_FIELD_COUNT; i++) {
        settings.fields_shared[i] = 0;
    }
    return settings;
}

const char *pf_classification_name(pf_classification_t cls) {
    if (cls < 0 || cls > PF_CLASS_FORBIDDEN) {
        return "unknown";
    }
    return s_class_names[cls];
}

const char *pf_consent_name(pf_consent_t consent) {
    if (consent < 0 || consent > PF_CONSENT_FULL) {
        return "unknown";
    }
    return s_consent_names[consent];
}

const char *pf_retention_name(pf_retention_t retention) {
    if (retention < 0 || retention > PF_RETAIN_SYNC) {
        return "unknown";
    }
    return s_retention_names[retention];
}

const char *pf_field_name(pf_field_t field) {
    if (field < 0 || field >= PF_FIELD_COUNT) {
        return "unknown";
    }
    return s_field_names[field];
}

int pf_fields_at_level(pf_classification_t cls, pf_field_t *out, int max) {
    int count = 0;
    for (int i = 0; i < PF_FIELD_COUNT && count < max; i++) {
        if (s_field_class[i] == cls) {
            out[count++] = (pf_field_t)i;
        }
    }
    return count;
}
