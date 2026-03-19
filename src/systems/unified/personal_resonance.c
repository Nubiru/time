/* personal_resonance.c — Cross-system echo detector for birth profiles.
 *
 * Detects resonances between a user's birth profile and the current moment.
 * Scans Tzolkin kin/seal/tone/oracle/wavespell, I Ching hexagram,
 * Hebrew month, Chinese animal/element, and zodiac sign.
 *
 * Pure module: no GL, no malloc, no globals, no side effects. */

#include "personal_resonance.h"
#include <string.h>
#include <stdio.h>

/* Tzolkin seal names indexed 0-19 */
static const char *const SEAL_NAMES[20] = {
    "Dragon", "Wind", "Night", "Seed", "Serpent",
    "Worldbridger", "Hand", "Star", "Moon", "Dog",
    "Monkey", "Human", "Skywalker", "Wizard", "Eagle",
    "Warrior", "Earth", "Mirror", "Storm", "Sun"
};

/* Type name lookup */
static const char *const TYPE_NAMES[PR_TYPE_COUNT] = {
    "Seal Match",
    "Tone Match",
    "Kin Match",
    "Oracle Match",
    "Wavespell Match",
    "Hexagram Match",
    "Hebrew Month Match",
    "Chinese Match",
    "Zodiac Match"
};

/* --- Internal: add an echo to result if room remains --- */

static void add_echo(pr_result_t *r, pr_type_t type, const char *system,
                     const char *desc, int strength)
{
    if (r->count >= PR_MAX_ECHOES) { return; }
    pr_echo_t *e = &r->echoes[r->count];
    e->type = type;
    e->system = system;
    e->strength = strength;
    snprintf(e->description, sizeof(e->description), "%s", desc);
    r->count++;
    if (strength > r->strongest) { r->strongest = strength; }
}

/* --- Internal: sort echoes by strength descending (insertion sort) --- */

static void sort_echoes(pr_result_t *r)
{
    for (int i = 1; i < r->count; i++) {
        pr_echo_t tmp = r->echoes[i];
        int j = i - 1;
        while (j >= 0 && r->echoes[j].strength < tmp.strength) {
            r->echoes[j + 1] = r->echoes[j];
            j--;
        }
        r->echoes[j + 1] = tmp;
    }
}

/* --- Internal: check oracle matches --- */

static void check_oracle(pr_result_t *r, const pr_input_t *in)
{
    struct {
        int kin;
        const char *role;
        const char *relationship;
    } oracles[4] = {
        { in->birth_guide_kin,    "destiny guide",          "guide" },
        { in->birth_analog_kin,   "supportive partner",     "partner" },
        { in->birth_antipode_kin, "challenge strengthener",  "challenge" },
        { in->birth_occult_kin,   "hidden power teacher",    "hidden" }
    };

    for (int i = 0; i < 4; i++) {
        if (oracles[i].kin == 0) { continue; }
        if (oracles[i].kin == in->today_kin) {
            char desc[256];
            snprintf(desc, sizeof(desc),
                     "Today's kin is your oracle %s — %s",
                     oracles[i].role, oracles[i].relationship);
            add_echo(r, PR_TYPE_ORACLE_MATCH, "Tzolkin", desc, 3);
            return; /* Only one oracle match per detection */
        }
    }
}

/* --- Public API --- */

pr_input_t pr_default_input(void)
{
    pr_input_t in;
    memset(&in, 0, sizeof(in));
    in.birth_seal = -1;
    in.birth_chinese_animal = -1;
    in.birth_chinese_element = -1;
    in.birth_sun_sign = -1;
    return in;
}

pr_result_t pr_detect(const pr_input_t *input)
{
    pr_result_t r;
    memset(&r, 0, sizeof(r));

    if (!input) { return r; }

    /* KIN_MATCH — strength 5 */
    if (input->birth_kin > 0 && input->birth_kin == input->today_kin) {
        add_echo(&r, PR_TYPE_KIN_MATCH, "Tzolkin",
                 "Your exact birth kin returns today — a galactic birthday!",
                 5);
    }

    /* HEXAGRAM_MATCH — strength 4 */
    if (input->birth_hexagram > 0 &&
        input->birth_hexagram == input->today_hexagram) {
        char desc[256];
        snprintf(desc, sizeof(desc),
                 "Your birth hexagram (#%d) returns today",
                 input->birth_hexagram);
        add_echo(&r, PR_TYPE_HEXAGRAM_MATCH, "I Ching", desc, 4);
    }

    /* ORACLE_MATCH — strength 3 */
    check_oracle(&r, input);

    /* SEAL_MATCH — strength 2 */
    if (input->birth_seal >= 0 && input->birth_seal <= 19 &&
        input->birth_seal == input->today_seal) {
        char desc[256];
        snprintf(desc, sizeof(desc),
                 "Your birth seal (%s) appears today",
                 SEAL_NAMES[input->birth_seal]);
        add_echo(&r, PR_TYPE_SEAL_MATCH, "Tzolkin", desc, 2);
    }

    /* TONE_MATCH — strength 2 */
    if (input->birth_tone > 0 && input->birth_tone == input->today_tone) {
        char desc[256];
        snprintf(desc, sizeof(desc),
                 "Your birth tone (%d) resonates today",
                 input->birth_tone);
        add_echo(&r, PR_TYPE_TONE_MATCH, "Tzolkin", desc, 2);
    }

    /* WAVESPELL_MATCH — strength 2 */
    if (input->birth_wavespell > 0 &&
        input->birth_wavespell == input->today_wavespell) {
        add_echo(&r, PR_TYPE_WAVESPELL_MATCH, "Tzolkin",
                 "You are in your birth wavespell", 2);
    }

    /* HEBREW_MONTH_MATCH — strength 2 */
    if (input->birth_hebrew_month > 0 &&
        input->birth_hebrew_month == input->today_hebrew_month) {
        add_echo(&r, PR_TYPE_HEBREW_MONTH_MATCH, "Hebrew",
                 "You are in your Hebrew birth month", 2);
    }

    /* CHINESE_MATCH — strength 1 (animal OR element) */
    if (input->birth_chinese_animal >= 0 &&
        input->birth_chinese_animal == input->today_chinese_animal) {
        add_echo(&r, PR_TYPE_CHINESE_MATCH, "Chinese",
                 "Your birth animal returns in today's cycle", 1);
    } else if (input->birth_chinese_element >= 0 &&
               input->birth_chinese_element == input->today_chinese_element) {
        add_echo(&r, PR_TYPE_CHINESE_MATCH, "Chinese",
                 "Your birth element returns in today's cycle", 1);
    }

    /* ZODIAC_MATCH — strength 1 */
    if (input->birth_sun_sign >= 0 &&
        input->birth_sun_sign == input->today_sun_sign) {
        add_echo(&r, PR_TYPE_ZODIAC_MATCH, "Astrology",
                 "The Sun is in your birth sign", 1);
    }

    sort_echoes(&r);
    return r;
}

const char *pr_type_name(pr_type_t type)
{
    if (type < 0 || type >= PR_TYPE_COUNT) { return "?"; }
    return TYPE_NAMES[type];
}

int pr_type_count(void)
{
    return PR_TYPE_COUNT;
}
