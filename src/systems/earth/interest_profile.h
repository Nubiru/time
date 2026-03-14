/* interest_profile.h — Interest Profile Data for the Earth social layer.
 * 20 interests across 5 categories, profile matching via cosine similarity. */

#ifndef TIME_INTEREST_PROFILE_H
#define TIME_INTEREST_PROFILE_H

#define IP_MAX_INTERESTS 20
#define IP_CATEGORY_COUNT 5

typedef enum {
    IP_NATURE = 0,       /* hiking, surfing, stargazing, climbing */
    IP_CULTURE,          /* art, music, food, history, architecture */
    IP_SOCIAL,           /* meetups, sports, games, festivals */
    IP_SPIRITUAL,        /* meditation, ceremony, pilgrimage, yoga */
    IP_KNOWLEDGE,        /* lectures, workshops, museums, reading */
    IP_CAT_COUNT
} ip_category_t;

typedef enum {
    IP_VIS_ANONYMOUS = 0,  /* No profile shared */
    IP_VIS_PSEUDONYM,      /* Auto-generated name only */
    IP_VIS_PUBLIC          /* Full profile visible */
} ip_visibility_t;

typedef enum {
    IP_ACT_INTROVERT = 0,  /* Prefers solo / small group */
    IP_ACT_BALANCED,       /* Flexible */
    IP_ACT_EXTROVERT       /* Prefers social / group activities */
} ip_activity_t;

/* Individual interest with weight */
typedef struct {
    int id;                  /* Interest ID (0..N) */
    const char *name;        /* e.g. "surfing", "stargazing" */
    ip_category_t category;  /* Which category */
    int time_system;         /* Maps to Time system (-1 if none) */
    const char *description; /* Brief description */
} ip_interest_t;

/* User's interest profile */
typedef struct {
    double weights[IP_MAX_INTERESTS];   /* 0.0-1.0 per interest */
    double category_affinity[IP_CATEGORY_COUNT]; /* Aggregate per category */
    ip_visibility_t visibility;
    ip_activity_t activity_level;
    double location_radius_km;          /* Exploration radius */
} ip_profile_t;

/* Match result between two profiles */
typedef struct {
    double score;            /* 0.0-1.0 overall compatibility */
    double category_scores[IP_CATEGORY_COUNT]; /* Per-category match */
    int top_shared;          /* Index of highest shared interest */
    int complementary;       /* Index of complementary interest */
} ip_match_t;

/* Create a default profile (all weights zero, anonymous, balanced). */
ip_profile_t ip_default_profile(void);

/* Get interest definition by ID. Returns id=-1 for invalid. */
ip_interest_t ip_interest_get(int id);

/* Total number of defined interests. */
int ip_interest_count(void);

/* Interests in a category. Fills out_ids, returns count. */
int ip_interests_in_category(ip_category_t cat, int *out_ids, int max);

/* Set a single interest weight in the profile. */
ip_profile_t ip_set_weight(ip_profile_t profile, int interest_id, double weight);

/* Recompute category affinities from individual weights. */
ip_profile_t ip_recompute_categories(ip_profile_t profile);

/* Compute match score between two profiles. */
ip_match_t ip_match(ip_profile_t a, ip_profile_t b);

/* Filter interests by minimum weight threshold. Fills out_ids, returns count. */
int ip_active_interests(ip_profile_t profile, double min_weight,
                        int *out_ids, int max);

/* Get category name string. */
const char *ip_category_name(ip_category_t cat);

/* Get visibility name string. */
const char *ip_visibility_name(ip_visibility_t vis);

/* Get activity level name string. */
const char *ip_activity_name(ip_activity_t act);

/* Category count. */
int ip_category_count(void);

/* Map interest to Time system name (returns NULL if no mapping). */
const char *ip_time_system_for(int interest_id);

#endif /* TIME_INTEREST_PROFILE_H */
