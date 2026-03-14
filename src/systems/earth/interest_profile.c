/* interest_profile.c — 20 interests across 5 categories with profile matching.
 * Pure data module: no malloc, no globals, no side effects. */

#include "interest_profile.h"
#include <math.h>
#include <string.h>

#define IP_DEFAULT_RADIUS_KM 50.0

/* --- Static interest data (20 interests, 5 categories) --- */

static const ip_interest_t INTERESTS[IP_MAX_INTERESTS] = {
    /* Nature (0-3) */
    { 0,  "Surfing",     IP_NATURE,    0,  "Ocean waves, tides, swell conditions"},
    { 1,  "Stargazing",  IP_NATURE,    1,  "Night sky observation, constellations, planets"},
    { 2,  "Hiking",      IP_NATURE,    2,  "Trail exploration, weather, daylight conditions"},
    { 3,  "Climbing",    IP_NATURE,    3,  "Mountain ascents, snow, elevation data"},
    /* Culture (4-7) */
    { 4,  "History",     IP_CULTURE,   4,  "Historical events, achievements, cultural drama"},
    { 5,  "Art",         IP_CULTURE,   -1, "Visual arts, design, creative expression"},
    { 6,  "Music",       IP_CULTURE,   5,  "Sound, frequency, audio experiences"},
    { 7,  "Food",        IP_CULTURE,   -1, "Seasonal cuisine, local specialties"},
    /* Social (8-11) */
    { 8,  "Meetups",     IP_SOCIAL,    -1, "Local gatherings, community events"},
    { 9,  "Sports",      IP_SOCIAL,    -1, "Athletic activities, weather-dependent sports"},
    {10,  "Festivals",   IP_SOCIAL,    6,  "Calendar celebrations across cultures"},
    {11,  "Games",       IP_SOCIAL,    7,  "Tzolkin, I Ching, interactive play"},
    /* Spiritual (12-15) */
    {12,  "Meditation",  IP_SPIRITUAL, 8,  "Buddhist/Hindu contemplative practice"},
    {13,  "Ceremony",    IP_SPIRITUAL, 9,  "Ritual observance, Wheel of Year, Hebrew feasts"},
    {14,  "Pilgrimage",  IP_SPIRITUAL, 10, "Sacred journeys, Kabbalah paths, megalithic sites"},
    {15,  "Yoga",        IP_SPIRITUAL, 11, "Hindu/chakra body-mind practice"},
    /* Knowledge (16-19) */
    {16,  "Lectures",    IP_KNOWLEDGE, 12, "Wisdom traditions, philosophical discourse"},
    {17,  "Workshops",   IP_KNOWLEDGE, -1, "Hands-on learning, achievement data"},
    {18,  "Museums",     IP_KNOWLEDGE, 13, "Geological time, fossil records, deep history"},
    {19,  "Reading",     IP_KNOWLEDGE, 14, "Knowledge graphs, literature, research"},
};

/* Time system name lookup (indexed by time_system value 0..14) */
static const char *TIME_SYSTEM_NAMES[] = {
    "Tidal Systems",       /*  0 — Surfing */
    "Star Field",          /*  1 — Stargazing */
    "Weather Systems",     /*  2 — Hiking */
    "Snow Systems",        /*  3 — Climbing */
    "Historical Drama",    /*  4 — History */
    "Frequency Systems",   /*  5 — Music */
    "Calendar Festivals",  /*  6 — Festivals */
    "Tzolkin/I Ching",     /*  7 — Games */
    "Buddhist/Hindu",      /*  8 — Meditation */
    "Wheel of Year",       /*  9 — Ceremony */
    "Kabbalah Paths",      /* 10 — Pilgrimage */
    "Hindu/Chakra",        /* 11 — Yoga */
    "Wisdom Engine",       /* 12 — Lectures */
    "Geological Time",     /* 13 — Museums */
    "Knowledge Graph",     /* 14 — Reading */
};

#define IP_TIME_SYSTEM_COUNT 15

/* --- Helpers --- */

static double clamp01(double v) {
    if (v < 0.0) return 0.0;
    if (v > 1.0) return 1.0;
    return v;
}

/* Count how many interests belong to a category. */
static int category_size(ip_category_t cat) {
    int count = 0;
    for (int i = 0; i < IP_MAX_INTERESTS; i++) {
        if (INTERESTS[i].category == cat) {
            count++;
        }
    }
    return count;
}

/* --- Public API --- */

ip_profile_t ip_default_profile(void) {
    ip_profile_t p;
    memset(&p, 0, sizeof(p));
    p.visibility = IP_VIS_ANONYMOUS;
    p.activity_level = IP_ACT_BALANCED;
    p.location_radius_km = IP_DEFAULT_RADIUS_KM;
    return p;
}

ip_interest_t ip_interest_get(int id) {
    if (id < 0 || id >= IP_MAX_INTERESTS) {
        ip_interest_t invalid;
        memset(&invalid, 0, sizeof(invalid));
        invalid.id = -1;
        return invalid;
    }
    return INTERESTS[id];
}

int ip_interest_count(void) {
    return IP_MAX_INTERESTS;
}

int ip_interests_in_category(ip_category_t cat, int *out_ids, int max) {
    int count = 0;
    for (int i = 0; i < IP_MAX_INTERESTS && count < max; i++) {
        if (INTERESTS[i].category == cat) {
            out_ids[count++] = i;
        }
    }
    return count;
}

ip_profile_t ip_set_weight(ip_profile_t profile, int interest_id, double weight) {
    if (interest_id < 0 || interest_id >= IP_MAX_INTERESTS) {
        return profile;
    }
    profile.weights[interest_id] = clamp01(weight);
    return profile;
}

ip_profile_t ip_recompute_categories(ip_profile_t profile) {
    for (int c = 0; c < IP_CATEGORY_COUNT; c++) {
        double sum = 0.0;
        int size = category_size((ip_category_t)c);
        if (size == 0) {
            profile.category_affinity[c] = 0.0;
            continue;
        }
        for (int i = 0; i < IP_MAX_INTERESTS; i++) {
            if ((int)INTERESTS[i].category == c) {
                sum += profile.weights[i];
            }
        }
        profile.category_affinity[c] = sum / (double)size;
    }
    return profile;
}

ip_match_t ip_match(ip_profile_t a, ip_profile_t b) {
    ip_match_t result;
    memset(&result, 0, sizeof(result));
    result.top_shared = -1;
    result.complementary = -1;

    /* Cosine similarity of weight vectors */
    double dot = 0.0;
    double mag_a = 0.0;
    double mag_b = 0.0;
    for (int i = 0; i < IP_MAX_INTERESTS; i++) {
        dot += a.weights[i] * b.weights[i];
        mag_a += a.weights[i] * a.weights[i];
        mag_b += b.weights[i] * b.weights[i];
    }
    mag_a = sqrt(mag_a);
    mag_b = sqrt(mag_b);

    double base_score = 0.0;
    if (mag_a > 0.0 && mag_b > 0.0) {
        base_score = dot / (mag_a * mag_b);
    }

    /* Category overlap */
    double cat_min_sum = 0.0;
    double cat_a_sum = 0.0;
    double cat_b_sum = 0.0;
    for (int c = 0; c < IP_CATEGORY_COUNT; c++) {
        double ca = a.category_affinity[c];
        double cb = b.category_affinity[c];
        cat_min_sum += (ca < cb) ? ca : cb;
        cat_a_sum += ca;
        cat_b_sum += cb;
    }
    double cat_max_sum = (cat_a_sum > cat_b_sum) ? cat_a_sum : cat_b_sum;
    double cat_overlap = 0.0;
    if (cat_max_sum > 0.0) {
        cat_overlap = cat_min_sum / cat_max_sum;
    }

    /* Per-category scores: cosine similarity within each category */
    for (int c = 0; c < IP_CATEGORY_COUNT; c++) {
        double cdot = 0.0;
        double cma = 0.0;
        double cmb = 0.0;
        for (int i = 0; i < IP_MAX_INTERESTS; i++) {
            if ((int)INTERESTS[i].category == c) {
                cdot += a.weights[i] * b.weights[i];
                cma += a.weights[i] * a.weights[i];
                cmb += b.weights[i] * b.weights[i];
            }
        }
        cma = sqrt(cma);
        cmb = sqrt(cmb);
        if (cma > 0.0 && cmb > 0.0) {
            result.category_scores[c] = cdot / (cma * cmb);
        } else {
            result.category_scores[c] = 0.0;
        }
    }

    /* Complementary bonus: find pair (i,k) maximizing
     * a.weights[i] * (1 - b.weights[i]) * b.weights[k] * (1 - a.weights[k])
     * For simplicity, find the interest with max |a-b| difference. */
    double max_diff = -1.0;
    for (int i = 0; i < IP_MAX_INTERESTS; i++) {
        double diff = fabs(a.weights[i] - b.weights[i]);
        if (diff > max_diff) {
            max_diff = diff;
            result.complementary = i;
        }
    }

    /* Compute complement bonus using the spec formula:
     * For each pair (i,k), compute a[i]*(1-b[i])*b[k]*(1-a[k]).
     * Take the maximum. */
    double complement = 0.0;
    for (int i = 0; i < IP_MAX_INTERESTS; i++) {
        for (int k = 0; k < IP_MAX_INTERESTS; k++) {
            double val = a.weights[i] * (1.0 - b.weights[i]) *
                         b.weights[k] * (1.0 - a.weights[k]);
            if (val > complement) {
                complement = val;
            }
        }
    }

    /* Top shared: interest with highest min(a.weight, b.weight) */
    double best_shared = -1.0;
    for (int i = 0; i < IP_MAX_INTERESTS; i++) {
        double mn = (a.weights[i] < b.weights[i]) ? a.weights[i] : b.weights[i];
        if (mn > best_shared) {
            best_shared = mn;
            result.top_shared = i;
        }
    }

    /* Final score */
    result.score = clamp01(base_score * 0.7 + cat_overlap * 0.2 + complement * 0.1);

    return result;
}

int ip_active_interests(ip_profile_t profile, double min_weight,
                        int *out_ids, int max) {
    int count = 0;
    for (int i = 0; i < IP_MAX_INTERESTS && count < max; i++) {
        if (profile.weights[i] > min_weight) {
            out_ids[count++] = i;
        }
    }
    return count;
}

const char *ip_category_name(ip_category_t cat) {
    switch (cat) {
        case IP_NATURE:    return "Nature";
        case IP_CULTURE:   return "Culture";
        case IP_SOCIAL:    return "Social";
        case IP_SPIRITUAL: return "Spiritual";
        case IP_KNOWLEDGE: return "Knowledge";
        case IP_CAT_COUNT: break;
    }
    return "Unknown";
}

const char *ip_visibility_name(ip_visibility_t vis) {
    switch (vis) {
        case IP_VIS_ANONYMOUS: return "Anonymous";
        case IP_VIS_PSEUDONYM: return "Pseudonym";
        case IP_VIS_PUBLIC:    return "Public";
    }
    return "Unknown";
}

const char *ip_activity_name(ip_activity_t act) {
    switch (act) {
        case IP_ACT_INTROVERT: return "Introvert";
        case IP_ACT_BALANCED:  return "Balanced";
        case IP_ACT_EXTROVERT: return "Extrovert";
    }
    return "Unknown";
}

int ip_category_count(void) {
    return IP_CATEGORY_COUNT;
}

const char *ip_time_system_for(int interest_id) {
    if (interest_id < 0 || interest_id >= IP_MAX_INTERESTS) {
        return NULL;
    }
    int ts = INTERESTS[interest_id].time_system;
    if (ts < 0 || ts >= IP_TIME_SYSTEM_COUNT) {
        return NULL;
    }
    return TIME_SYSTEM_NAMES[ts];
}
