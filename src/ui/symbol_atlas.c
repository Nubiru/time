#include "symbol_atlas.h"

#include <stddef.h>
#include <string.h>

/* ============================================================
 * Category sizes
 * ============================================================ */

static const int CAT_COUNTS[SYM_CAT_COUNT] = {
    12, /* ZODIAC */
     8, /* PLANET */
     5, /* ASPECT */
     8, /* TRIGRAM */
    20, /* MAYAN_SEAL */
     8  /* LUNAR_PHASE */
};

/* ============================================================
 * Trigram glyphs (8x12 each)
 * Order: Qian, Dui, Li, Zhen, Xun, Kan, Gen, Kun
 * Three horizontal lines: solid=yang, broken=yin
 * Bottom line = row 8-9, middle = row 5-6, top = row 2-3
 * ============================================================ */

/* Qian (Heaven): three solid lines ━━━ ━━━ ━━━ */
static const unsigned char GLYPH_TRIGRAM_QIAN[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,
    0,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,
    0,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* Dui (Lake): yin-yang-yang (top broken, middle solid, bottom solid) */
static const unsigned char GLYPH_TRIGRAM_DUI[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,1,1,0,0,1,1,0,
    0,1,1,0,0,1,1,0,
    0,0,0,0,0,0,0,0,
    0,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,
    0,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* Li (Fire): yang-yin-yang (top solid, middle broken, bottom solid) */
static const unsigned char GLYPH_TRIGRAM_LI[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,
    0,1,1,0,0,1,1,0,
    0,1,1,0,0,1,1,0,
    0,0,0,0,0,0,0,0,
    0,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* Zhen (Thunder): yin-yin-yang (top broken, middle broken, bottom solid) */
static const unsigned char GLYPH_TRIGRAM_ZHEN[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,1,1,0,0,1,1,0,
    0,1,1,0,0,1,1,0,
    0,0,0,0,0,0,0,0,
    0,1,1,0,0,1,1,0,
    0,1,1,0,0,1,1,0,
    0,0,0,0,0,0,0,0,
    0,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* Xun (Wind): yang-yang-yin (top solid, middle solid, bottom broken) */
static const unsigned char GLYPH_TRIGRAM_XUN[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,
    0,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,
    0,1,1,0,0,1,1,0,
    0,1,1,0,0,1,1,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* Kan (Water): yin-yang-yin (top broken, middle solid, bottom broken) */
static const unsigned char GLYPH_TRIGRAM_KAN[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,1,1,0,0,1,1,0,
    0,1,1,0,0,1,1,0,
    0,0,0,0,0,0,0,0,
    0,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,
    0,1,1,0,0,1,1,0,
    0,1,1,0,0,1,1,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* Gen (Mountain): yang-yin-yin (top solid, middle broken, bottom broken) */
static const unsigned char GLYPH_TRIGRAM_GEN[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,
    0,1,1,0,0,1,1,0,
    0,1,1,0,0,1,1,0,
    0,0,0,0,0,0,0,0,
    0,1,1,0,0,1,1,0,
    0,1,1,0,0,1,1,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* Kun (Earth): three broken lines ━ ━ ━ ━ ━ ━ */
static const unsigned char GLYPH_TRIGRAM_KUN[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,1,1,0,0,1,1,0,
    0,1,1,0,0,1,1,0,
    0,0,0,0,0,0,0,0,
    0,1,1,0,0,1,1,0,
    0,1,1,0,0,1,1,0,
    0,0,0,0,0,0,0,0,
    0,1,1,0,0,1,1,0,
    0,1,1,0,0,1,1,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

static const unsigned char *TRIGRAM_GLYPHS[8] = {
    GLYPH_TRIGRAM_QIAN, GLYPH_TRIGRAM_DUI, GLYPH_TRIGRAM_LI,
    GLYPH_TRIGRAM_ZHEN, GLYPH_TRIGRAM_XUN, GLYPH_TRIGRAM_KAN,
    GLYPH_TRIGRAM_GEN,  GLYPH_TRIGRAM_KUN
};

/* ============================================================
 * Mayan day seal glyphs (8x12 each, Dreamspell/Arguelles)
 * 20 seals: geometric/abstract representations
 * ============================================================ */

/* 0: Dragon (Imix) — jaws opening downward, nurturing */
static const unsigned char GLYPH_SEAL_DRAGON[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,1,1,1,1,0,0,
    0,1,1,1,1,1,1,0,
    0,1,0,1,1,0,1,0,
    0,1,0,0,0,0,1,0,
    0,1,1,0,0,1,1,0,
    0,0,1,0,0,1,0,0,
    0,0,1,0,0,1,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* 1: Wind (Ik) — spiral breath */
static const unsigned char GLYPH_SEAL_WIND[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,1,1,1,0,0,
    0,0,1,0,0,0,0,0,
    0,0,1,1,1,0,0,0,
    0,0,0,0,1,0,0,0,
    0,0,1,1,0,0,0,0,
    0,1,0,0,0,0,0,0,
    0,1,1,1,1,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* 2: Night (Akbal) — dark house, half circle */
static const unsigned char GLYPH_SEAL_NIGHT[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,1,1,1,1,0,0,
    0,1,0,0,0,0,1,0,
    0,1,0,0,0,0,1,0,
    0,1,0,0,0,0,1,0,
    0,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* 3: Seed (Kan) — sprouting kernel, diamond */
static const unsigned char GLYPH_SEAL_SEED[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,1,0,0,1,0,0,
    0,1,0,0,0,0,1,0,
    0,1,0,1,1,0,1,0,
    0,1,0,0,0,0,1,0,
    0,0,1,0,0,1,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* 4: Serpent (Chicchan) — coiled snake */
static const unsigned char GLYPH_SEAL_SERPENT[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,1,1,0,0,0,0,
    0,1,0,0,1,0,0,0,
    0,0,0,0,0,1,0,0,
    0,0,0,0,0,1,0,0,
    0,0,0,0,1,0,0,0,
    0,0,0,1,0,0,0,0,
    0,0,1,0,0,0,0,0,
    0,0,1,0,0,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* 5: World-Bridger (Cimi) — skull/bridge, equal-armed cross */
static const unsigned char GLYPH_SEAL_WORLDBRIDGER[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* 6: Hand (Manik) — open palm, deer hoof */
static const unsigned char GLYPH_SEAL_HAND[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,1,0,1,0,1,0,
    0,0,1,0,1,0,1,0,
    0,0,1,0,1,0,1,0,
    0,0,1,1,1,1,1,0,
    0,1,1,1,1,1,1,0,
    0,1,1,1,1,1,0,0,
    0,0,1,1,1,0,0,0,
    0,0,0,1,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* 7: Star (Lamat) — four-pointed star */
static const unsigned char GLYPH_SEAL_STAR[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,0,
    0,0,0,1,1,0,0,0,
    0,0,1,0,0,1,0,0,
    0,1,0,0,0,0,1,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* 8: Moon (Muluc) — crescent */
static const unsigned char GLYPH_SEAL_MOON[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,1,1,1,0,0,
    0,0,1,1,0,0,1,0,
    0,1,1,0,0,0,0,0,
    0,1,1,0,0,0,0,0,
    0,1,1,0,0,0,0,0,
    0,1,1,0,0,0,0,0,
    0,0,1,1,0,0,1,0,
    0,0,0,1,1,1,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* 9: Dog (Oc) — loyal companion, simple dog face */
static const unsigned char GLYPH_SEAL_DOG[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,1,0,0,0,0,1,0,
    0,1,1,0,0,1,1,0,
    0,0,1,1,1,1,0,0,
    0,0,1,0,0,1,0,0,
    0,0,1,1,1,1,0,0,
    0,0,0,1,1,0,0,0,
    0,0,1,1,1,1,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* 10: Monkey (Chuen) — playful spiral */
static const unsigned char GLYPH_SEAL_MONKEY[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,1,1,1,0,0,0,
    0,1,0,0,0,1,0,0,
    0,1,0,1,1,0,1,0,
    0,0,0,1,0,0,1,0,
    0,0,0,0,1,1,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* 11: Human (Eb) — road/path, human silhouette */
static const unsigned char GLYPH_SEAL_HUMAN[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,1,1,1,1,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,1,0,0,1,0,0,
    0,1,0,0,0,0,1,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* 12: Skywalker (Ben) — tall reed, vertical lines */
static const unsigned char GLYPH_SEAL_SKYWALKER[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,1,1,1,1,0,0,
    0,0,0,1,1,0,0,0,
    0,1,1,1,1,1,1,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* 13: Wizard (Ix) — jaguar spots, magic eye */
static const unsigned char GLYPH_SEAL_WIZARD[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,1,1,1,1,0,0,
    0,1,0,0,0,0,1,0,
    0,1,0,1,1,0,1,0,
    0,1,0,1,1,0,1,0,
    0,1,0,0,0,0,1,0,
    0,0,1,1,1,1,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* 14: Eagle (Men) — wings spread, vision */
static const unsigned char GLYPH_SEAL_EAGLE[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,1,1,1,1,0,0,
    0,1,1,1,1,1,1,0,
    1,0,0,1,1,0,0,1,
    1,0,0,0,0,0,0,1,
    0,0,0,1,1,0,0,0,
    0,0,1,0,0,1,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* 15: Warrior (Cib) — shield/wisdom, squared circle */
static const unsigned char GLYPH_SEAL_WARRIOR[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,1,1,1,1,1,1,0,
    0,1,0,0,0,0,1,0,
    0,1,0,1,1,0,1,0,
    0,1,0,1,1,0,1,0,
    0,1,0,0,0,0,1,0,
    0,1,1,1,1,1,1,0,
    0,0,0,1,1,0,0,0,
    0,0,1,1,1,1,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* 16: Earth (Caban) — earth force, layered ground */
static const unsigned char GLYPH_SEAL_EARTH[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,
    0,0,1,1,1,1,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,1,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* 17: Mirror (Etznab) — knife/truth, angular reflection */
static const unsigned char GLYPH_SEAL_MIRROR[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,1,1,0,0,0,
    0,0,1,0,0,1,0,0,
    0,1,0,0,0,0,1,0,
    0,1,0,0,0,0,1,0,
    0,1,1,1,1,1,1,0,
    0,1,0,0,0,0,1,0,
    0,0,1,0,0,1,0,0,
    0,0,0,1,1,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* 18: Storm (Cauac) — cloud with rain */
static const unsigned char GLYPH_SEAL_STORM[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,1,1,1,1,0,0,
    0,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,0,
    0,0,1,1,1,1,0,0,
    0,0,0,0,0,0,0,0,
    0,0,1,0,1,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,1,0,1,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* 19: Sun (Ahau) — radiant circle, solar lord */
static const unsigned char GLYPH_SEAL_SUN[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,1,1,0,0,0,
    0,1,0,0,0,0,1,0,
    0,0,1,1,1,1,0,0,
    1,0,1,0,0,1,0,1,
    1,0,1,0,0,1,0,1,
    0,0,1,1,1,1,0,0,
    0,1,0,0,0,0,1,0,
    0,0,0,1,1,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

static const unsigned char *MAYAN_SEAL_GLYPHS[20] = {
    GLYPH_SEAL_DRAGON, GLYPH_SEAL_WIND, GLYPH_SEAL_NIGHT,
    GLYPH_SEAL_SEED, GLYPH_SEAL_SERPENT, GLYPH_SEAL_WORLDBRIDGER,
    GLYPH_SEAL_HAND, GLYPH_SEAL_STAR, GLYPH_SEAL_MOON,
    GLYPH_SEAL_DOG, GLYPH_SEAL_MONKEY, GLYPH_SEAL_HUMAN,
    GLYPH_SEAL_SKYWALKER, GLYPH_SEAL_WIZARD, GLYPH_SEAL_EAGLE,
    GLYPH_SEAL_WARRIOR, GLYPH_SEAL_EARTH, GLYPH_SEAL_MIRROR,
    GLYPH_SEAL_STORM, GLYPH_SEAL_SUN
};

/* ============================================================
 * Lunar phase glyphs (8x12 each)
 * 0=New, 1=Waxing Crescent, 2=First Quarter, 3=Waxing Gibbous,
 * 4=Full, 5=Waning Gibbous, 6=Last Quarter, 7=Waning Crescent
 * ============================================================ */

/* 0: New Moon — thin circle outline */
static const unsigned char GLYPH_LUNAR_NEW[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,1,1,1,1,0,0,
    0,1,0,0,0,0,1,0,
    0,1,0,0,0,0,1,0,
    0,1,0,0,0,0,1,0,
    0,1,0,0,0,0,1,0,
    0,1,0,0,0,0,1,0,
    0,0,1,1,1,1,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* 1: Waxing Crescent — right side lit */
static const unsigned char GLYPH_LUNAR_WAX_CRESCENT[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,1,1,1,1,0,0,
    0,1,0,0,0,1,1,0,
    0,1,0,0,0,1,1,0,
    0,1,0,0,0,1,1,0,
    0,1,0,0,0,1,1,0,
    0,1,0,0,0,1,1,0,
    0,0,1,1,1,1,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* 2: First Quarter — right half filled */
static const unsigned char GLYPH_LUNAR_FIRST_Q[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,1,1,1,1,0,0,
    0,1,0,0,1,1,1,0,
    0,1,0,0,1,1,1,0,
    0,1,0,0,1,1,1,0,
    0,1,0,0,1,1,1,0,
    0,1,0,0,1,1,1,0,
    0,0,1,1,1,1,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* 3: Waxing Gibbous — mostly filled, left sliver empty */
static const unsigned char GLYPH_LUNAR_WAX_GIBBOUS[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,1,1,1,1,0,0,
    0,1,0,1,1,1,1,0,
    0,1,0,1,1,1,1,0,
    0,1,0,1,1,1,1,0,
    0,1,0,1,1,1,1,0,
    0,1,0,1,1,1,1,0,
    0,0,1,1,1,1,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* 4: Full Moon — filled circle */
static const unsigned char GLYPH_LUNAR_FULL[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,1,1,1,1,0,0,
    0,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,0,
    0,1,1,1,1,1,1,0,
    0,0,1,1,1,1,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* 5: Waning Gibbous — mostly filled, right sliver empty */
static const unsigned char GLYPH_LUNAR_WAN_GIBBOUS[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,1,1,1,1,0,0,
    0,1,1,1,1,0,1,0,
    0,1,1,1,1,0,1,0,
    0,1,1,1,1,0,1,0,
    0,1,1,1,1,0,1,0,
    0,1,1,1,1,0,1,0,
    0,0,1,1,1,1,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* 6: Last Quarter — left half filled */
static const unsigned char GLYPH_LUNAR_LAST_Q[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,1,1,1,1,0,0,
    0,1,1,1,0,0,1,0,
    0,1,1,1,0,0,1,0,
    0,1,1,1,0,0,1,0,
    0,1,1,1,0,0,1,0,
    0,1,1,1,0,0,1,0,
    0,0,1,1,1,1,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

/* 7: Waning Crescent — left side lit */
static const unsigned char GLYPH_LUNAR_WAN_CRESCENT[GLYPH_BYTES] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,1,1,1,1,0,0,
    0,1,1,0,0,0,1,0,
    0,1,1,0,0,0,1,0,
    0,1,1,0,0,0,1,0,
    0,1,1,0,0,0,1,0,
    0,1,1,0,0,0,1,0,
    0,0,1,1,1,1,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
};

static const unsigned char *LUNAR_PHASE_GLYPHS[8] = {
    GLYPH_LUNAR_NEW, GLYPH_LUNAR_WAX_CRESCENT, GLYPH_LUNAR_FIRST_Q,
    GLYPH_LUNAR_WAX_GIBBOUS, GLYPH_LUNAR_FULL, GLYPH_LUNAR_WAN_GIBBOUS,
    GLYPH_LUNAR_LAST_Q, GLYPH_LUNAR_WAN_CRESCENT
};

/* ============================================================
 * Name tables
 * ============================================================ */

static const char *CAT_NAMES[SYM_CAT_COUNT] = {
    "Zodiac", "Planet", "Aspect", "Trigram", "Mayan Seal", "Lunar Phase"
};

static const char *ZODIAC_NAMES[12] = {
    "Aries", "Taurus", "Gemini", "Cancer", "Leo", "Virgo",
    "Libra", "Scorpio", "Sagittarius", "Capricorn", "Aquarius", "Pisces"
};

static const char *PLANET_NAMES[8] = {
    "Mercury", "Venus", "Earth", "Mars",
    "Jupiter", "Saturn", "Uranus", "Neptune"
};

static const char *ASPECT_NAMES[5] = {
    "Conjunction", "Opposition", "Trine", "Square", "Sextile"
};

static const char *TRIGRAM_NAMES[8] = {
    "Qian", "Dui", "Li", "Zhen", "Xun", "Kan", "Gen", "Kun"
};

static const char *SEAL_NAMES[20] = {
    "Dragon", "Wind", "Night", "Seed", "Serpent",
    "World-Bridger", "Hand", "Star", "Moon", "Dog",
    "Monkey", "Human", "Skywalker", "Wizard", "Eagle",
    "Warrior", "Earth", "Mirror", "Storm", "Sun"
};

static const char *LUNAR_NAMES[8] = {
    "New Moon", "Waxing Crescent", "First Quarter", "Waxing Gibbous",
    "Full Moon", "Waning Gibbous", "Last Quarter", "Waning Crescent"
};

/* ============================================================
 * Atlas layout: 8 columns x 8 rows = 64 cells for 61 symbols
 * ============================================================ */

#define ATLAS_COLS 8
#define ATLAS_ROWS 8

/* Compute the flat index (0-60) for a (category, index) pair.
 * Returns -1 for invalid. */
static int flat_index(int category, int index)
{
    if (category < 0 || category >= SYM_CAT_COUNT) return -1;
    if (index < 0 || index >= CAT_COUNTS[category]) return -1;

    int offset = 0;
    for (int c = 0; c < category; c++) {
        offset += CAT_COUNTS[c];
    }
    return offset + index;
}

/* ============================================================
 * Public API
 * ============================================================ */

int symbol_atlas_count(int category)
{
    if (category < 0 || category >= SYM_CAT_COUNT) return 0;
    return CAT_COUNTS[category];
}

int symbol_atlas_total(void)
{
    int total = 0;
    for (int c = 0; c < SYM_CAT_COUNT; c++) {
        total += CAT_COUNTS[c];
    }
    return total;
}

const unsigned char *symbol_atlas_glyph(int category, int index)
{
    if (category < 0 || category >= SYM_CAT_COUNT) return NULL;
    if (index < 0 || index >= CAT_COUNTS[category]) return NULL;

    switch (category) {
        case SYM_CAT_ZODIAC:      return zodiac_glyph(index);
        case SYM_CAT_PLANET:      return planet_glyph(index);
        case SYM_CAT_ASPECT:      return aspect_glyph(index);
        case SYM_CAT_TRIGRAM:     return TRIGRAM_GLYPHS[index];
        case SYM_CAT_MAYAN_SEAL: return MAYAN_SEAL_GLYPHS[index];
        case SYM_CAT_LUNAR_PHASE: return LUNAR_PHASE_GLYPHS[index];
        default: return NULL;
    }
}

glyph_uv_t symbol_atlas_uv(int category, int index)
{
    glyph_uv_t uv = {0.0f, 0.0f, 0.0f, 0.0f};
    int fi = flat_index(category, index);
    if (fi < 0) return uv;

    int col = fi % ATLAS_COLS;
    int row = fi / ATLAS_COLS;

    uv.u0 = (float)col / (float)ATLAS_COLS;
    uv.v0 = (float)row / (float)ATLAS_ROWS;
    uv.u1 = (float)(col + 1) / (float)ATLAS_COLS;
    uv.v1 = (float)(row + 1) / (float)ATLAS_ROWS;
    return uv;
}

symbol_atlas_info_t symbol_atlas_info(void)
{
    symbol_atlas_info_t info;
    info.cols = ATLAS_COLS;
    info.rows = ATLAS_ROWS;
    info.cell_w = GLYPH_WIDTH;
    info.cell_h = GLYPH_HEIGHT;
    return info;
}

int symbol_atlas_texture_size(void)
{
    return ATLAS_COLS * GLYPH_WIDTH * ATLAS_ROWS * GLYPH_HEIGHT * 4;
}

void symbol_atlas_pack(unsigned char *buf, int buf_size)
{
    int expected = symbol_atlas_texture_size();
    if (buf_size < expected) return;

    memset(buf, 0, (size_t)expected);

    int tex_w = ATLAS_COLS * GLYPH_WIDTH;
    int total = symbol_atlas_total();

    for (int fi = 0; fi < total; fi++) {
        /* Find category and index from flat index */
        int cat = 0, idx = fi;
        for (int c = 0; c < SYM_CAT_COUNT; c++) {
            if (idx < CAT_COUNTS[c]) { cat = c; break; }
            idx -= CAT_COUNTS[c];
        }

        const unsigned char *glyph = symbol_atlas_glyph(cat, idx);
        if (!glyph) continue;

        int atlas_col = fi % ATLAS_COLS;
        int atlas_row = fi / ATLAS_COLS;
        int base_x = atlas_col * GLYPH_WIDTH;
        int base_y = atlas_row * GLYPH_HEIGHT;

        for (int gy = 0; gy < GLYPH_HEIGHT; gy++) {
            for (int gx = 0; gx < GLYPH_WIDTH; gx++) {
                int px = base_x + gx;
                int py = base_y + gy;
                int pixel_offset = (py * tex_w + px) * 4;

                if (glyph[gy * GLYPH_WIDTH + gx]) {
                    buf[pixel_offset + 0] = 255; /* R */
                    buf[pixel_offset + 1] = 255; /* G */
                    buf[pixel_offset + 2] = 255; /* B */
                    buf[pixel_offset + 3] = 255; /* A */
                }
            }
        }
    }
}

const char *symbol_atlas_category_name(int category)
{
    if (category < 0 || category >= SYM_CAT_COUNT) return NULL;
    return CAT_NAMES[category];
}

const char *symbol_atlas_symbol_name(int category, int index)
{
    if (category < 0 || category >= SYM_CAT_COUNT) return NULL;
    if (index < 0 || index >= CAT_COUNTS[category]) return NULL;

    switch (category) {
        case SYM_CAT_ZODIAC:      return ZODIAC_NAMES[index];
        case SYM_CAT_PLANET:      return PLANET_NAMES[index];
        case SYM_CAT_ASPECT:      return ASPECT_NAMES[index];
        case SYM_CAT_TRIGRAM:     return TRIGRAM_NAMES[index];
        case SYM_CAT_MAYAN_SEAL: return SEAL_NAMES[index];
        case SYM_CAT_LUNAR_PHASE: return LUNAR_NAMES[index];
        default: return NULL;
    }
}
