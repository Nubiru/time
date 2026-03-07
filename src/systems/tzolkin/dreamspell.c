#include "dreamspell.h"

/*
 * Dreamspell Oracle Engine — Arguelles system.
 *
 * Seal indexing (0-19, matching tzolkin.h):
 *   0=Dragon, 1=Wind, 2=Night, 3=Seed, 4=Serpent,
 *   5=World-Bridger, 6=Hand, 7=Star, 8=Moon, 9=Dog,
 *   10=Monkey, 11=Human, 12=Skywalker, 13=Wizard,
 *   14=Eagle, 15=Warrior, 16=Earth, 17=Mirror, 18=Storm, 19=Sun
 *
 * Color: seal % 4 → 0=Red, 1=White, 2=Blue, 3=Yellow
 *
 * Source: Arguelles, "Dreamspell" (1990).
 */

/* --- Seal names (bare, without color prefix) --- */

static const char *SEAL_NAMES[20] = {
    "Dragon", "Wind", "Night", "Seed", "Serpent",
    "World-Bridger", "Hand", "Star", "Moon", "Dog",
    "Monkey", "Human", "Skywalker", "Wizard", "Eagle",
    "Warrior", "Earth", "Mirror", "Storm", "Sun"
};

/* --- Tone data --- */

static const char *TONE_NAMES[13] = {
    "Magnetic", "Lunar", "Electric", "Self-Existing",
    "Overtone", "Rhythmic", "Resonant", "Galactic",
    "Solar", "Planetary", "Spectral", "Crystal", "Cosmic"
};

static const char *TONE_ACTIONS[13] = {
    "Unify", "Polarize", "Activate", "Define",
    "Empower", "Organize", "Channel", "Harmonize",
    "Pulse", "Perfect", "Dissolve", "Dedicate", "Endure"
};

static const char *TONE_POWERS[13] = {
    "Purpose", "Challenge", "Service", "Form",
    "Radiance", "Equality", "Attunement", "Integrity",
    "Intention", "Manifestation", "Liberation", "Cooperation", "Presence"
};

/* --- Color data --- */

static const char *COLOR_NAMES[4] = { "Red", "White", "Blue", "Yellow" };
static const char *COLOR_ACTIONS[4] = { "Initiates", "Refines", "Transforms", "Ripens" };
static const char *COLOR_QUALITIES[4] = { "Birth", "Death", "Magic", "Intelligence" };

/* --- Castle data --- */

static const char *CASTLE_NAMES[5] = {
    "Red Castle of Turning",
    "White Castle of Crossing",
    "Blue Castle of Burning",
    "Yellow Castle of Giving",
    "Green Castle of Enchantment"
};

static const char *CASTLE_COLORS[5] = { "Red", "White", "Blue", "Yellow", "Green" };
static const char *CASTLE_THEMES[5] = { "Birth", "Death", "Magic", "Intelligence", "Enchantment" };

/* --- Earth family data ---
 *
 * Family grouping by seal % 5:
 *   0 → Cardinal (East):  Dragon(0),  WB(5),       Monkey(10), Warrior(15)
 *   1 → Core (South):     Wind(1),    Hand(6),     Human(11),  Earth(16)
 *   2 → Signal (West):    Night(2),   Star(7),     Skywalker(12), Mirror(17)
 *   3 → Gateway (Center): Seed(3),    Moon(8),     Wizard(13), Storm(18)
 *   4 → Polar (North):    Serpent(4), Dog(9),      Eagle(14),  Sun(19)
 */

static const char *FAMILY_NAMES[5] = {
    "Cardinal", "Core", "Signal", "Gateway", "Polar"
};

static const char *FAMILY_DIRECTIONS[5] = {
    "East", "South", "West", "Center", "North"
};

static const int FAMILY_MEMBERS[5][4] = {
    { 0,  5, 10, 15},   /* Cardinal */
    { 1,  6, 11, 16},   /* Core */
    { 2,  7, 12, 17},   /* Signal */
    { 3,  8, 13, 18},   /* Gateway */
    { 4,  9, 14, 19}    /* Polar */
};

/* --- Internal helpers --- */

/*
 * Compute kin number from tone (1-13) and seal (0-19) using CRT.
 *
 * kin - 1 ≡ tone - 1 (mod 13) AND kin - 1 ≡ seal (mod 20)
 * Since gcd(13, 20) = 1:
 *   inv(20, 13) = 2     (20 * 2 = 40 ≡ 1 mod 13)
 *   inv(13, 20) = 17    (13 * 17 = 221 ≡ 1 mod 20)
 *   k = ((tone - 1) * 40 + seal * 221) % 260
 *   kin = k + 1
 */
static int kin_from_tone_seal(int tone, int seal)
{
    int k = ((tone - 1) * 40 + seal * 221) % 260;
    return k + 1;
}

/*
 * Guide seal selection by tone group.
 *
 * The guide shares the destiny seal's color (seal % 4).
 * Offsets are multiples of 4, preserving color.
 *
 * Tone group → seal offset:
 *   {1, 6, 11} → +0  (self-guiding)
 *   {2, 7, 12} → +12
 *   {3, 8, 13} → +4
 *   {4, 9}     → +16
 *   {5, 10}    → +8
 */
static int guide_seal_offset(int tone)
{
    /* Map tone to group index: tone modulo 5 (with 5→0 mapping) */
    int g = tone % 5;
    switch (g) {
    case 1: return 0;   /* tones 1, 6, 11 */
    case 2: return 12;  /* tones 2, 7, 12 */
    case 3: return 4;   /* tones 3, 8, 13 */
    case 4: return 16;  /* tones 4, 9 */
    case 0: return 8;   /* tones 5, 10 */
    default: return 0;  /* unreachable */
    }
}

/* --- Public API --- */

dreamspell_tone_t dreamspell_tone(int tone)
{
    dreamspell_tone_t t;
    if (tone < 1 || tone > 13) {
        t.number = 0;
        t.name = "?";
        t.action = "?";
        t.power = "?";
        return t;
    }
    t.number = tone;
    t.name = TONE_NAMES[tone - 1];
    t.action = TONE_ACTIONS[tone - 1];
    t.power = TONE_POWERS[tone - 1];
    return t;
}

dreamspell_color_t dreamspell_color(int seal)
{
    dreamspell_color_t c;
    if (seal < 0 || seal > 19) {
        c.name = "?";
        c.action = "?";
        c.quality = "?";
        return c;
    }
    int ci = seal % 4;
    c.name = COLOR_NAMES[ci];
    c.action = COLOR_ACTIONS[ci];
    c.quality = COLOR_QUALITIES[ci];
    return c;
}

dreamspell_oracle_t dreamspell_oracle(int kin)
{
    dreamspell_oracle_t o = {0, 0, 0, 0, 0, 0};
    if (kin < 1 || kin > 260)
        return o;

    int tone = (kin - 1) % 13 + 1;
    int seal = (kin - 1) % 20;

    /* Destiny */
    o.destiny = kin;

    /* Analog: seal_analog = (19 - seal) % 20, same tone */
    int analog_seal = (19 - seal) % 20;
    o.analog = kin_from_tone_seal(tone, analog_seal);

    /* Antipode: seal_antipode = (seal + 10) % 20, same tone */
    int antipode_seal = (seal + 10) % 20;
    o.antipode = kin_from_tone_seal(tone, antipode_seal);

    /* Guide: same color (seal%4), tone-dependent offset */
    int gseal = (seal + guide_seal_offset(tone)) % 20;
    o.guide = kin_from_tone_seal(tone, gseal);

    /* Occult / Hidden: 261 - destiny */
    o.occult = 261 - kin;
    o.hidden = 261 - kin;

    return o;
}

dreamspell_wavespell_t dreamspell_wavespell(int kin)
{
    dreamspell_wavespell_t w = {0, 0, "?", "?", "?"};
    if (kin < 1 || kin > 260)
        return w;

    w.number = (kin - 1) / 13 + 1;

    /* Initiating seal: seal of the first kin in this wavespell */
    int first_kin = (w.number - 1) * 13 + 1;
    w.seal = (first_kin - 1) % 20;

    /* Purpose = seal name, action/power from seal's color */
    w.purpose = SEAL_NAMES[w.seal];
    int ci = w.seal % 4;
    w.action = COLOR_ACTIONS[ci];
    w.power = COLOR_QUALITIES[ci];

    return w;
}

dreamspell_castle_t dreamspell_castle(int kin)
{
    dreamspell_castle_t c = {0, "?", "?", "?", 0, 0};
    if (kin < 1 || kin > 260)
        return c;

    int ci = (kin - 1) / 52;
    c.number = ci + 1;
    c.name = CASTLE_NAMES[ci];
    c.color = CASTLE_COLORS[ci];
    c.theme = CASTLE_THEMES[ci];
    c.start_kin = ci * 52 + 1;
    c.end_kin = (ci + 1) * 52;

    return c;
}

dreamspell_family_t dreamspell_earth_family(int seal)
{
    dreamspell_family_t f;
    if (seal < 0 || seal > 19) {
        f.name = "?";
        f.direction = "?";
        f.members[0] = -1;
        f.members[1] = -1;
        f.members[2] = -1;
        f.members[3] = -1;
        return f;
    }

    int fi = seal % 5;
    f.name = FAMILY_NAMES[fi];
    f.direction = FAMILY_DIRECTIONS[fi];
    f.members[0] = FAMILY_MEMBERS[fi][0];
    f.members[1] = FAMILY_MEMBERS[fi][1];
    f.members[2] = FAMILY_MEMBERS[fi][2];
    f.members[3] = FAMILY_MEMBERS[fi][3];

    return f;
}

int dreamspell_wavespell_seal(int kin)
{
    if (kin < 1 || kin > 260)
        return -1;

    int ws = (kin - 1) / 13 + 1;
    int first_kin = (ws - 1) * 13 + 1;
    return (first_kin - 1) % 20;
}

int dreamspell_wavespell_position(int kin)
{
    if (kin < 1 || kin > 260)
        return 0;
    return (kin - 1) % 13 + 1;
}

int dreamspell_family_count(void)
{
    return 5;
}

int dreamspell_castle_count(void)
{
    return 5;
}
