/* user_prefs.c -- User Preferences Store implementation.
 *
 * Track 47.3: serialization, defaults, validation, merge, diff.
 *
 * Pure functions: no GL, no malloc, no globals, no side effects.
 * All buffers are fixed-size, all strings use snprintf. */

#include "user_prefs.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* --- helpers --- */

static float clamp_f(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static int clamp_i(int v, int lo, int hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static double clamp_d(double v, double lo, double hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

/* Safe append to buffer. Returns chars written (excluding null), or 0
 * if there is no room. Sets *truncated = 1 on overflow. */
static int buf_append(char *buf, int buf_size, int pos,
                      const char *str, int *truncated) {
    if (pos >= buf_size - 1) {
        *truncated = 1;
        return 0;
    }
    int remaining = buf_size - pos;
    int len = snprintf(buf + pos, (size_t)remaining, "%s", str);
    if (len >= remaining) {
        *truncated = 1;
        return remaining - 1; /* what was actually written */
    }
    return len;
}

/* --- up_default --- */

up_prefs_t up_default(void) {
    up_prefs_t p;
    memset(&p, 0, sizeof(p));

    /* Visual */
    p.visual.theme = THEME_COSMOS;
    p.visual.font_scale = 1.0f;
    p.visual.opacity_scale = 1.0f;
    p.visual.animation_speed = 1.0f;
    p.visual.layers_visible = 0xFFFF; /* all 16 layers on */
    p.visual.default_scale_level = 0;

    /* Temporal */
    p.temporal.home_timezone_offset_min = 0;
    p.temporal.preferred_calendar_count = 0;
    for (int i = 0; i < UP_MAX_CALENDAR_SYSTEMS; i++) {
        p.temporal.preferred_calendars[i] = -1;
    }
    p.temporal.use_24h_time = 0;
    p.temporal.week_start_day = 0; /* Sunday */

    /* Social */
    p.social.activity_level = IP_ACT_INTROVERT;
    p.social.visibility = IP_VIS_ANONYMOUS;
    p.social.notification_frequency = 0;
    p.social.roulette_opt_in = 0;
    p.social.consent = PF_CONSENT_NONE;
    p.social.retention = PF_RETAIN_SESSION;

    /* Location */
    p.location.home_lat = 0.0;
    p.location.home_lon = 0.0;
    p.location.exploration_radius_km = 50.0;
    p.location.location_set = 0;

    /* Schema */
    p.version = 1;

    return p;
}

/* --- up_validate --- */

up_prefs_t up_validate(up_prefs_t prefs) {
    /* Visual */
    prefs.visual.font_scale = clamp_f(prefs.visual.font_scale, 0.5f, 2.0f);
    prefs.visual.opacity_scale = clamp_f(prefs.visual.opacity_scale, 0.5f, 1.0f);
    prefs.visual.animation_speed = clamp_f(prefs.visual.animation_speed, 0.5f, 2.0f);
    prefs.visual.default_scale_level = clamp_i(prefs.visual.default_scale_level, 0, 6);
    if (prefs.visual.theme < 0 || prefs.visual.theme >= THEME_COUNT) {
        prefs.visual.theme = THEME_COSMOS;
    }

    /* Temporal */
    prefs.temporal.home_timezone_offset_min =
        clamp_i(prefs.temporal.home_timezone_offset_min, -720, 840);
    prefs.temporal.preferred_calendar_count =
        clamp_i(prefs.temporal.preferred_calendar_count, 0, UP_MAX_CALENDAR_SYSTEMS);
    prefs.temporal.use_24h_time = (prefs.temporal.use_24h_time != 0) ? 1 : 0;
    prefs.temporal.week_start_day = clamp_i(prefs.temporal.week_start_day, 0, 1);

    /* Social */
    prefs.social.activity_level =
        (ip_activity_t)clamp_i((int)prefs.social.activity_level, 0, 2);
    prefs.social.visibility =
        (ip_visibility_t)clamp_i((int)prefs.social.visibility, 0, 2);
    prefs.social.notification_frequency =
        clamp_i(prefs.social.notification_frequency, 0, 3);
    prefs.social.roulette_opt_in =
        (prefs.social.roulette_opt_in != 0) ? 1 : 0;
    prefs.social.consent =
        (pf_consent_t)clamp_i((int)prefs.social.consent, 0, 2);
    prefs.social.retention =
        (pf_retention_t)clamp_i((int)prefs.social.retention, 0, 2);

    /* Location */
    prefs.location.home_lat = clamp_d(prefs.location.home_lat, -90.0, 90.0);
    prefs.location.home_lon = clamp_d(prefs.location.home_lon, -180.0, 180.0);
    prefs.location.exploration_radius_km =
        clamp_d(prefs.location.exploration_radius_km, 1.0, 500.0);
    prefs.location.location_set =
        (prefs.location.location_set != 0) ? 1 : 0;

    /* Version */
    if (prefs.version < 1) prefs.version = 1;

    return prefs;
}

/* --- Serialization helpers --- */

/* Format one key=value\n line into a temp buffer, return its length. */
static int fmt_kv_int(char *line, int line_size,
                      const char *key, int val) {
    return snprintf(line, (size_t)line_size, "%s=%d\n", key, val);
}

static int fmt_kv_float(char *line, int line_size,
                        const char *key, float val) {
    return snprintf(line, (size_t)line_size, "%s=%.4f\n", key, (double)val);
}

static int fmt_kv_double(char *line, int line_size,
                         const char *key, double val) {
    return snprintf(line, (size_t)line_size, "%s=%.6f\n", key, val);
}

/* --- up_serialize_visual --- */

up_serial_result_t up_serialize_visual(const up_visual_t *v,
                                       char *buf, int buf_size) {
    up_serial_result_t res = {0, 0};
    if (buf_size <= 0) { res.truncated = 1; return res; }

    char line[UP_VAL_MAX];
    int pos = 0;

    fmt_kv_int(line, UP_VAL_MAX, "visual.theme", (int)v->theme);
    pos += buf_append(buf, buf_size, pos, line, &res.truncated);

    fmt_kv_float(line, UP_VAL_MAX, "visual.font_scale", v->font_scale);
    pos += buf_append(buf, buf_size, pos, line, &res.truncated);

    fmt_kv_float(line, UP_VAL_MAX, "visual.opacity_scale", v->opacity_scale);
    pos += buf_append(buf, buf_size, pos, line, &res.truncated);

    fmt_kv_float(line, UP_VAL_MAX, "visual.anim_speed", v->animation_speed);
    pos += buf_append(buf, buf_size, pos, line, &res.truncated);

    fmt_kv_int(line, UP_VAL_MAX, "visual.layers", v->layers_visible);
    pos += buf_append(buf, buf_size, pos, line, &res.truncated);

    fmt_kv_int(line, UP_VAL_MAX, "visual.scale_level", v->default_scale_level);
    pos += buf_append(buf, buf_size, pos, line, &res.truncated);

    res.length = pos;
    return res;
}

/* --- up_serialize_temporal --- */

up_serial_result_t up_serialize_temporal(const up_temporal_t *t,
                                         char *buf, int buf_size) {
    up_serial_result_t res = {0, 0};
    if (buf_size <= 0) { res.truncated = 1; return res; }

    char line[UP_VAL_MAX];
    int pos = 0;

    fmt_kv_int(line, UP_VAL_MAX, "temporal.tz_offset",
               t->home_timezone_offset_min);
    pos += buf_append(buf, buf_size, pos, line, &res.truncated);

    fmt_kv_int(line, UP_VAL_MAX, "temporal.cal_count",
               t->preferred_calendar_count);
    pos += buf_append(buf, buf_size, pos, line, &res.truncated);

    /* Serialize preferred calendars as comma-separated list */
    if (t->preferred_calendar_count > 0) {
        /* Use a dedicated buffer large enough for prefix + values + newline.
         * 16 calendars * max 4 digits + commas + "temporal.calendars=" + \n */
        char cal_line[256];
        int cl = snprintf(cal_line, sizeof(cal_line), "temporal.calendars=");
        for (int i = 0; i < t->preferred_calendar_count &&
                         i < UP_MAX_CALENDAR_SYSTEMS; i++) {
            if (t->preferred_calendars[i] < 0) break;
            if (i > 0) {
                cl += snprintf(cal_line + cl,
                               sizeof(cal_line) - (size_t)cl, ",");
            }
            cl += snprintf(cal_line + cl,
                           sizeof(cal_line) - (size_t)cl, "%d",
                           t->preferred_calendars[i]);
        }
        cl += snprintf(cal_line + cl,
                       sizeof(cal_line) - (size_t)cl, "\n");
        (void)cl;
        pos += buf_append(buf, buf_size, pos, cal_line, &res.truncated);
    }

    fmt_kv_int(line, UP_VAL_MAX, "temporal.use_24h", t->use_24h_time);
    pos += buf_append(buf, buf_size, pos, line, &res.truncated);

    fmt_kv_int(line, UP_VAL_MAX, "temporal.week_start", t->week_start_day);
    pos += buf_append(buf, buf_size, pos, line, &res.truncated);

    res.length = pos;
    return res;
}

/* --- up_serialize_social --- */

up_serial_result_t up_serialize_social(const up_social_t *s,
                                       char *buf, int buf_size) {
    up_serial_result_t res = {0, 0};
    if (buf_size <= 0) { res.truncated = 1; return res; }

    char line[UP_VAL_MAX];
    int pos = 0;

    fmt_kv_int(line, UP_VAL_MAX, "social.activity", (int)s->activity_level);
    pos += buf_append(buf, buf_size, pos, line, &res.truncated);

    fmt_kv_int(line, UP_VAL_MAX, "social.visibility", (int)s->visibility);
    pos += buf_append(buf, buf_size, pos, line, &res.truncated);

    fmt_kv_int(line, UP_VAL_MAX, "social.notif_freq",
               s->notification_frequency);
    pos += buf_append(buf, buf_size, pos, line, &res.truncated);

    fmt_kv_int(line, UP_VAL_MAX, "social.roulette", s->roulette_opt_in);
    pos += buf_append(buf, buf_size, pos, line, &res.truncated);

    fmt_kv_int(line, UP_VAL_MAX, "social.consent", (int)s->consent);
    pos += buf_append(buf, buf_size, pos, line, &res.truncated);

    fmt_kv_int(line, UP_VAL_MAX, "social.retention", (int)s->retention);
    pos += buf_append(buf, buf_size, pos, line, &res.truncated);

    res.length = pos;
    return res;
}

/* --- up_serialize_location --- */

up_serial_result_t up_serialize_location(const up_location_t *l,
                                          char *buf, int buf_size) {
    up_serial_result_t res = {0, 0};
    if (buf_size <= 0) { res.truncated = 1; return res; }

    char line[UP_VAL_MAX];
    int pos = 0;

    fmt_kv_double(line, UP_VAL_MAX, "location.lat", l->home_lat);
    pos += buf_append(buf, buf_size, pos, line, &res.truncated);

    fmt_kv_double(line, UP_VAL_MAX, "location.lon", l->home_lon);
    pos += buf_append(buf, buf_size, pos, line, &res.truncated);

    fmt_kv_double(line, UP_VAL_MAX, "location.radius",
                  l->exploration_radius_km);
    pos += buf_append(buf, buf_size, pos, line, &res.truncated);

    fmt_kv_int(line, UP_VAL_MAX, "location.set", l->location_set);
    pos += buf_append(buf, buf_size, pos, line, &res.truncated);

    res.length = pos;
    return res;
}

/* --- up_serialize --- */

up_serial_result_t up_serialize(const up_prefs_t *prefs,
                                char *buf, int buf_size) {
    up_serial_result_t res = {0, 0};
    if (buf_size <= 0) { res.truncated = 1; return res; }

    /* Serialize into a large temp buffer first */
    char tmp[UP_MAX_SERIAL_SIZE];
    int pos = 0;
    int trunc = 0;

    /* Version */
    {
        char line[UP_VAL_MAX];
        fmt_kv_int(line, UP_VAL_MAX, "version", prefs->version);
        pos += buf_append(tmp, UP_MAX_SERIAL_SIZE, pos, line, &trunc);
    }

    /* Visual */
    {
        char sec[512];
        up_serial_result_t sr = up_serialize_visual(&prefs->visual, sec, 512);
        if (sr.length > 0) {
            pos += buf_append(tmp, UP_MAX_SERIAL_SIZE, pos, sec, &trunc);
        }
    }

    /* Temporal */
    {
        char sec[512];
        up_serial_result_t sr = up_serialize_temporal(&prefs->temporal, sec, 512);
        if (sr.length > 0) {
            pos += buf_append(tmp, UP_MAX_SERIAL_SIZE, pos, sec, &trunc);
        }
    }

    /* Social */
    {
        char sec[512];
        up_serial_result_t sr = up_serialize_social(&prefs->social, sec, 512);
        if (sr.length > 0) {
            pos += buf_append(tmp, UP_MAX_SERIAL_SIZE, pos, sec, &trunc);
        }
    }

    /* Location */
    {
        char sec[512];
        up_serial_result_t sr = up_serialize_location(&prefs->location, sec, 512);
        if (sr.length > 0) {
            pos += buf_append(tmp, UP_MAX_SERIAL_SIZE, pos, sec, &trunc);
        }
    }

    /* Copy to output buffer */
    if (pos >= buf_size) {
        memcpy(buf, tmp, (size_t)(buf_size - 1));
        buf[buf_size - 1] = '\0';
        res.length = buf_size - 1;
        res.truncated = 1;
    } else {
        memcpy(buf, tmp, (size_t)pos);
        buf[pos] = '\0';
        res.length = pos;
        res.truncated = trunc;
    }

    return res;
}

/* --- up_parse_kv --- */

int up_parse_kv(const char *buf, int buf_len, up_kv_t *out) {
    memset(out, 0, sizeof(*out));

    if (buf_len <= 0 || buf[0] == '\0') return 0;

    /* Find the end of this line */
    int line_end = 0;
    while (line_end < buf_len && buf[line_end] != '\n') {
        line_end++;
    }

    int consumed = line_end;
    if (line_end < buf_len && buf[line_end] == '\n') {
        consumed = line_end + 1; /* skip the newline */
    }

    if (line_end == 0) return consumed; /* empty line */

    /* Find the '=' separator */
    int eq_pos = -1;
    for (int i = 0; i < line_end; i++) {
        if (buf[i] == '=') {
            eq_pos = i;
            break;
        }
    }

    if (eq_pos < 0) {
        /* Malformed: no '=' found */
        out->key[0] = '\0';
        out->val[0] = '\0';
        return consumed;
    }

    /* Copy key */
    int key_len = eq_pos;
    if (key_len >= UP_KEY_MAX) key_len = UP_KEY_MAX - 1;
    memcpy(out->key, buf, (size_t)key_len);
    out->key[key_len] = '\0';

    /* Copy value */
    int val_start = eq_pos + 1;
    int val_len = line_end - val_start;
    if (val_len < 0) val_len = 0;
    if (val_len >= UP_VAL_MAX) val_len = UP_VAL_MAX - 1;
    memcpy(out->val, buf + val_start, (size_t)val_len);
    out->val[val_len] = '\0';

    return consumed;
}

/* --- up_count_keys --- */

int up_count_keys(const char *buf, int buf_len) {
    int count = 0;
    int pos = 0;
    while (pos < buf_len) {
        up_kv_t kv;
        int consumed = up_parse_kv(buf + pos, buf_len - pos, &kv);
        if (consumed <= 0) break;
        if (kv.key[0] != '\0') count++;
        pos += consumed;
    }
    return count;
}

/* --- up_deserialize --- */

/* Parse a comma-separated list of integers into an array.
 * Returns count parsed. */
static int parse_int_list(const char *val, int *out, int max) {
    int count = 0;
    const char *p = val;
    while (*p != '\0' && count < max) {
        char *end = NULL;
        long v = strtol(p, &end, 10);
        if (end == p) break; /* no number found */
        out[count++] = (int)v;
        p = end;
        if (*p == ',') p++;
    }
    return count;
}

/* --- Per-section deserialize helpers --- */

static int deser_visual(up_visual_t *v, const char *key, const char *val) {
    if (strcmp(key, "visual.theme") == 0) {
        v->theme = (theme_id_t)atoi(val); return 1;
    } else if (strcmp(key, "visual.font_scale") == 0) {
        v->font_scale = (float)atof(val); return 1;
    } else if (strcmp(key, "visual.opacity_scale") == 0) {
        v->opacity_scale = (float)atof(val); return 1;
    } else if (strcmp(key, "visual.anim_speed") == 0) {
        v->animation_speed = (float)atof(val); return 1;
    } else if (strcmp(key, "visual.layers") == 0) {
        v->layers_visible = atoi(val); return 1;
    } else if (strcmp(key, "visual.scale_level") == 0) {
        v->default_scale_level = atoi(val); return 1;
    }
    return 0;
}

static int deser_temporal(up_temporal_t *t, const char *key, const char *val) {
    if (strcmp(key, "temporal.tz_offset") == 0) {
        t->home_timezone_offset_min = atoi(val); return 1;
    } else if (strcmp(key, "temporal.cal_count") == 0) {
        t->preferred_calendar_count = atoi(val); return 1;
    } else if (strcmp(key, "temporal.calendars") == 0) {
        int ids[UP_MAX_CALENDAR_SYSTEMS];
        int n = parse_int_list(val, ids, UP_MAX_CALENDAR_SYSTEMS);
        for (int i = 0; i < n && i < UP_MAX_CALENDAR_SYSTEMS; i++)
            t->preferred_calendars[i] = ids[i];
        if (n < UP_MAX_CALENDAR_SYSTEMS) t->preferred_calendars[n] = -1;
        return 1;
    } else if (strcmp(key, "temporal.use_24h") == 0) {
        t->use_24h_time = atoi(val); return 1;
    } else if (strcmp(key, "temporal.week_start") == 0) {
        t->week_start_day = atoi(val); return 1;
    }
    return 0;
}

static int deser_social(up_social_t *s, const char *key, const char *val) {
    if (strcmp(key, "social.activity") == 0) {
        s->activity_level = (ip_activity_t)atoi(val); return 1;
    } else if (strcmp(key, "social.visibility") == 0) {
        s->visibility = (ip_visibility_t)atoi(val); return 1;
    } else if (strcmp(key, "social.notif_freq") == 0) {
        s->notification_frequency = atoi(val); return 1;
    } else if (strcmp(key, "social.roulette") == 0) {
        s->roulette_opt_in = atoi(val); return 1;
    } else if (strcmp(key, "social.consent") == 0) {
        s->consent = (pf_consent_t)atoi(val); return 1;
    } else if (strcmp(key, "social.retention") == 0) {
        s->retention = (pf_retention_t)atoi(val); return 1;
    }
    return 0;
}

static int deser_location(up_location_t *l, const char *key, const char *val) {
    if (strcmp(key, "location.lat") == 0) {
        l->home_lat = atof(val); return 1;
    } else if (strcmp(key, "location.lon") == 0) {
        l->home_lon = atof(val); return 1;
    } else if (strcmp(key, "location.radius") == 0) {
        l->exploration_radius_km = atof(val); return 1;
    } else if (strcmp(key, "location.set") == 0) {
        l->location_set = atoi(val); return 1;
    }
    return 0;
}

up_prefs_t up_deserialize(const char *buf, int buf_len) {
    up_prefs_t p = up_default();

    if (buf_len <= 0 || buf == NULL) return p;

    int pos = 0;
    while (pos < buf_len) {
        up_kv_t kv;
        int consumed = up_parse_kv(buf + pos, buf_len - pos, &kv);
        if (consumed <= 0) break;
        pos += consumed;

        if (kv.key[0] == '\0') continue;

        if (strcmp(kv.key, "version") == 0) {
            p.version = atoi(kv.val);
        } else if (!deser_visual(&p.visual, kv.key, kv.val) &&
                   !deser_temporal(&p.temporal, kv.key, kv.val) &&
                   !deser_social(&p.social, kv.key, kv.val) &&
                   !deser_location(&p.location, kv.key, kv.val)) {
            /* Unknown keys silently ignored (forward-compat) */
        }
    }

    return p;
}

/* --- up_merge --- */

/* --- Per-section merge helpers (override only if patch differs from default) --- */

static void merge_visual(up_visual_t *out, const up_visual_t *patch,
                         const up_visual_t *def) {
    if ((int)patch->theme != (int)def->theme) out->theme = patch->theme;
    if (fabsf(patch->font_scale - def->font_scale) > 0.001f)
        out->font_scale = patch->font_scale;
    if (fabsf(patch->opacity_scale - def->opacity_scale) > 0.001f)
        out->opacity_scale = patch->opacity_scale;
    if (fabsf(patch->animation_speed - def->animation_speed) > 0.001f)
        out->animation_speed = patch->animation_speed;
    if (patch->layers_visible != def->layers_visible)
        out->layers_visible = patch->layers_visible;
    if (patch->default_scale_level != def->default_scale_level)
        out->default_scale_level = patch->default_scale_level;
}

static void merge_temporal(up_temporal_t *out, const up_temporal_t *patch,
                           const up_temporal_t *def) {
    if (patch->home_timezone_offset_min != def->home_timezone_offset_min)
        out->home_timezone_offset_min = patch->home_timezone_offset_min;
    if (patch->preferred_calendar_count != def->preferred_calendar_count) {
        out->preferred_calendar_count = patch->preferred_calendar_count;
        memcpy(out->preferred_calendars, patch->preferred_calendars,
               sizeof(patch->preferred_calendars));
    }
    if (patch->use_24h_time != def->use_24h_time)
        out->use_24h_time = patch->use_24h_time;
    if (patch->week_start_day != def->week_start_day)
        out->week_start_day = patch->week_start_day;
}

static void merge_social(up_social_t *out, const up_social_t *patch,
                         const up_social_t *def) {
    if ((int)patch->activity_level != (int)def->activity_level)
        out->activity_level = patch->activity_level;
    if ((int)patch->visibility != (int)def->visibility)
        out->visibility = patch->visibility;
    if (patch->notification_frequency != def->notification_frequency)
        out->notification_frequency = patch->notification_frequency;
    if (patch->roulette_opt_in != def->roulette_opt_in)
        out->roulette_opt_in = patch->roulette_opt_in;
    if ((int)patch->consent != (int)def->consent)
        out->consent = patch->consent;
    if ((int)patch->retention != (int)def->retention)
        out->retention = patch->retention;
}

static void merge_location(up_location_t *out, const up_location_t *patch,
                           const up_location_t *def) {
    if (fabs(patch->home_lat - def->home_lat) > 0.0001)
        out->home_lat = patch->home_lat;
    if (fabs(patch->home_lon - def->home_lon) > 0.0001)
        out->home_lon = patch->home_lon;
    if (fabs(patch->exploration_radius_km - def->exploration_radius_km) > 0.001)
        out->exploration_radius_km = patch->exploration_radius_km;
    if (patch->location_set != def->location_set)
        out->location_set = patch->location_set;
}

up_prefs_t up_merge(up_prefs_t base, up_prefs_t patch) {
    up_prefs_t def = up_default();
    up_prefs_t out = base;
    merge_visual(&out.visual, &patch.visual, &def.visual);
    merge_temporal(&out.temporal, &patch.temporal, &def.temporal);
    merge_social(&out.social, &patch.social, &def.social);
    merge_location(&out.location, &patch.location, &def.location);
    return out;
}

/* --- up_diff --- */

static int visual_eq(const up_visual_t *a, const up_visual_t *b) {
    if ((int)a->theme != (int)b->theme) return 0;
    if (fabsf(a->font_scale - b->font_scale) > 0.001f) return 0;
    if (fabsf(a->opacity_scale - b->opacity_scale) > 0.001f) return 0;
    if (fabsf(a->animation_speed - b->animation_speed) > 0.001f) return 0;
    if (a->layers_visible != b->layers_visible) return 0;
    if (a->default_scale_level != b->default_scale_level) return 0;
    return 1;
}

static int temporal_eq(const up_temporal_t *a, const up_temporal_t *b) {
    if (a->home_timezone_offset_min != b->home_timezone_offset_min) return 0;
    if (a->preferred_calendar_count != b->preferred_calendar_count) return 0;
    for (int i = 0; i < a->preferred_calendar_count &&
                     i < UP_MAX_CALENDAR_SYSTEMS; i++) {
        if (a->preferred_calendars[i] != b->preferred_calendars[i]) return 0;
    }
    if (a->use_24h_time != b->use_24h_time) return 0;
    if (a->week_start_day != b->week_start_day) return 0;
    return 1;
}

static int social_eq(const up_social_t *a, const up_social_t *b) {
    if ((int)a->activity_level != (int)b->activity_level) return 0;
    if ((int)a->visibility != (int)b->visibility) return 0;
    if (a->notification_frequency != b->notification_frequency) return 0;
    if (a->roulette_opt_in != b->roulette_opt_in) return 0;
    if ((int)a->consent != (int)b->consent) return 0;
    if ((int)a->retention != (int)b->retention) return 0;
    return 1;
}

static int location_eq(const up_location_t *a, const up_location_t *b) {
    if (fabs(a->home_lat - b->home_lat) > 0.0001) return 0;
    if (fabs(a->home_lon - b->home_lon) > 0.0001) return 0;
    if (fabs(a->exploration_radius_km - b->exploration_radius_km) > 0.001)
        return 0;
    if (a->location_set != b->location_set) return 0;
    return 1;
}

int up_diff(const up_prefs_t *a, const up_prefs_t *b) {
    int flags = 0;
    if (!visual_eq(&a->visual, &b->visual)) flags |= UP_DIFF_VISUAL;
    if (!temporal_eq(&a->temporal, &b->temporal)) flags |= UP_DIFF_TEMPORAL;
    if (!social_eq(&a->social, &b->social)) flags |= UP_DIFF_SOCIAL;
    if (!location_eq(&a->location, &b->location)) flags |= UP_DIFF_LOCATION;
    return flags;
}

/* --- up_to_theme_prefs --- */

theme_prefs_t up_to_theme_prefs(const up_visual_t *v) {
    theme_prefs_t tp = theme_default_prefs();
    tp.font_scale = v->font_scale;
    tp.opacity_scale = v->opacity_scale;
    tp.animation_speed = v->animation_speed;
    /* No custom accent from user_prefs — theme system handles that */
    tp.use_custom_accent = 0;
    return tp;
}

/* --- up_to_privacy_settings --- */

pf_settings_t up_to_privacy_settings(const up_social_t *s) {
    pf_settings_t pf = pf_default_settings();
    pf.consent = s->consent;
    pf.retention = s->retention;
    return pf;
}

/* --- up_has_location --- */

int up_has_location(const up_location_t *l) {
    return l->location_set != 0;
}

/* --- up_summary --- */

int up_summary(const up_prefs_t *prefs, char *buf, int buf_size) {
    if (buf_size <= 0) return 0;

    const char *theme_str =
        (prefs->visual.theme == THEME_COSMOS) ? "Cosmos" : "Dawn";
    const char *time_fmt =
        prefs->temporal.use_24h_time ? "24h" : "12h";
    const char *act_str;
    switch (prefs->social.activity_level) {
        case IP_ACT_INTROVERT: act_str = "Introvert"; break;
        case IP_ACT_BALANCED:  act_str = "Balanced"; break;
        case IP_ACT_EXTROVERT: act_str = "Extrovert"; break;
        default:               act_str = "Unknown"; break;
    }

    int len;
    if (prefs->location.location_set) {
        len = snprintf(buf, (size_t)buf_size,
            "Theme: %s | Time: %s | Social: %s | "
            "Location: %.2f, %.2f (%.0fkm)",
            theme_str, time_fmt, act_str,
            prefs->location.home_lat,
            prefs->location.home_lon,
            prefs->location.exploration_radius_km);
    } else {
        len = snprintf(buf, (size_t)buf_size,
            "Theme: %s | Time: %s | Social: %s | Location: not set",
            theme_str, time_fmt, act_str);
    }

    if (len < 0) return 0;
    if (len >= buf_size) return buf_size - 1;
    return len;
}
