/* tarot_visual.c — Visual data for the 22 Major Arcana (Thoth deck)
 *
 * Pure data module. All data computed from color_theory.h mood system
 * and golden_layout.h phi constants. No hardcoded RGB or pixel values.
 *
 * Color scale derivation:
 *   Each card's attribution (element/planet/zodiac) maps to a ct_mood_t.
 *   The mood base color is taken from ct_mood_color().
 *   Four scales are derived by adjusting saturation and lightness:
 *     King (Atziluth):    base mood as-is (brightest, purest)
 *     Queen (Briah):      l *= (GL_PHI_INV + GL_PHI_INV4), s *= (1 - GL_PHI_INV5)
 *     Prince (Yetzirah):  l *= GL_PHI_INV, s *= (GL_PHI_INV + GL_PHI_INV5)
 *     Princess (Assiah):  l *= GL_PHI_INV2, s *= (GL_PHI_INV2 + GL_PHI_INV5)
 *
 * Crowley swaps implemented:
 *   - Card 8 (Adjustment/Libra) sits on path 11 (Lamed)
 *   - Card 11 (Lust/Leo) sits on path 8 (Tet)
 *   - Card 4 (Emperor/Aries) sits on path 17 (Tsade)
 *   - Card 17 (Star/Aquarius) sits on path 4 (He)
 *
 * Sources: Aleister Crowley (Book of Thoth, 777),
 *          Lady Frieda Harris (Thoth deck artist). */

#include "tarot_visual.h"
#include "color_theory.h"
#include "../ui/golden_layout.h"
#include "../systems/kabbalah/tree_geometry.h"

#include <stddef.h>

/* Mood attribution for each of the 22 Major Arcana.
 * Derived from the card's elemental/planetary/zodiacal attribution:
 *   Fire signs (Aries, Leo, Sagittarius), Mars, Sun -> ENERGY or WARMTH
 *   Earth signs (Taurus, Virgo, Capricorn), Saturn, Venus -> HARMONY or NEUTRAL
 *   Air signs (Gemini, Libra, Aquarius), Mercury, Jupiter -> CLARITY or CALM
 *   Water signs (Cancer, Scorpio, Pisces), Moon -> MYSTERY or WISDOM
 *   Mother letters: Aleph=Air=CLARITY, Mem=Water=CALM, Shin=Fire=ENERGY */
static const ct_mood_t CARD_MOODS[TV_MAJOR_COUNT] = {
    CT_MOOD_CLARITY,    /*  0 The Fool       — Aleph, Air (mother) */
    CT_MOOD_CLARITY,    /*  1 The Magus       — Beth, Mercury */
    CT_MOOD_MYSTERY,    /*  2 The Priestess   — Gimel, Moon */
    CT_MOOD_HARMONY,    /*  3 The Empress     — Daleth, Venus */
    CT_MOOD_ENERGY,     /*  4 The Emperor     — Tzaddi, Aries */
    CT_MOOD_HARMONY,    /*  5 The Hierophant  — Vav, Taurus */
    CT_MOOD_CLARITY,    /*  6 The Lovers      — Zayin, Gemini */
    CT_MOOD_MYSTERY,    /*  7 The Chariot     — Cheth, Cancer */
    CT_MOOD_CALM,       /*  8 Adjustment      — Lamed, Libra */
    CT_MOOD_NEUTRAL,    /*  9 The Hermit      — Yod, Virgo */
    CT_MOOD_CALM,       /* 10 Fortune         — Kaph, Jupiter */
    CT_MOOD_ENERGY,     /* 11 Lust            — Teth, Leo */
    CT_MOOD_CALM,       /* 12 The Hanged Man  — Mem, Water (mother) */
    CT_MOOD_MYSTERY,    /* 13 Death           — Nun, Scorpio */
    CT_MOOD_ENERGY,     /* 14 Art             — Samekh, Sagittarius */
    CT_MOOD_NEUTRAL,    /* 15 The Devil       — Ayin, Capricorn */
    CT_MOOD_ENERGY,     /* 16 The Tower       — Peh, Mars */
    CT_MOOD_CALM,       /* 17 The Star        — Heh, Aquarius */
    CT_MOOD_WISDOM,     /* 18 The Moon        — Qoph, Pisces */
    CT_MOOD_WARMTH,     /* 19 The Sun         — Resh, Sun */
    CT_MOOD_ENERGY,     /* 20 The Aeon        — Shin, Fire (mother) */
    CT_MOOD_NEUTRAL,    /* 21 The Universe    — Tav, Saturn */
};

/* Compute the 4 color scales for a card from its mood.
 * King:     base mood color (brightest)
 * Queen:    lightness *= (GL_PHI_INV + GL_PHI_INV4), saturation *= (1 - GL_PHI_INV5)
 * Prince:   lightness *= GL_PHI_INV, saturation *= (GL_PHI_INV + GL_PHI_INV5)
 * Princess: lightness *= GL_PHI_INV2, saturation *= (GL_PHI_INV2 + GL_PHI_INV5) */
static void compute_color_scales(ct_mood_t mood, color_rgb_t out[TV_COLOR_SCALES])
{
    color_hsl_t base = ct_mood_color(mood);

    /* Scale factors derived from golden ratio powers */
    /* King: base as-is */
    out[TV_SCALE_KING] = color_hsl_to_rgb(base);

    /* Queen: rich, deep — GL_PHI_INV applied to lightness with offset */
    color_hsl_t queen = base;
    queen.l = base.l * (float)(GL_PHI_INV + GL_PHI_INV4);
    queen.s = base.s * (float)(1.0 - GL_PHI_INV5);
    out[TV_SCALE_QUEEN] = color_hsl_to_rgb(queen);

    /* Prince: muted — GL_PHI_INV applied to lightness */
    color_hsl_t prince = base;
    prince.l = base.l * (float)GL_PHI_INV;
    prince.s = base.s * (float)(GL_PHI_INV + GL_PHI_INV5);
    out[TV_SCALE_PRINCE] = color_hsl_to_rgb(prince);

    /* Princess: dark — GL_PHI_INV2 applied to lightness */
    color_hsl_t princess = base;
    princess.l = base.l * (float)GL_PHI_INV2;
    princess.s = base.s * (float)(GL_PHI_INV2 + GL_PHI_INV5);
    out[TV_SCALE_PRINCESS] = color_hsl_to_rgb(princess);
}

/* Tree path index mapping: Crowley swaps applied.
 * Card 4 (Emperor) -> path 17 (Tsade), Card 17 (Star) -> path 4 (He)
 * Card 8 (Adjustment) -> path 11 (Lamed), Card 11 (Lust) -> path 8 (Tet) */
static int card_to_tree_path(int card_number)
{
    switch (card_number) {
    case 4:  return 17;  /* Emperor on Tsade path */
    case 17: return 4;   /* Star on He path */
    case 8:  return 11;  /* Adjustment on Lamed path */
    case 11: return 8;   /* Lust on Tet path */
    default: return card_number;
    }
}

/* Static card data.
 * Traditional names, Thoth names, motifs, symbol descriptions, attributions.
 * Colors computed lazily since they depend on ct_mood_color(). */
typedef struct {
    const char *name;
    const char *thoth_name;
    tv_motif_t primary_motif;
    tv_motif_t secondary_motif;
    const char *symbol_desc;
    const char *attribution;
} card_static_t;

static const card_static_t CARD_DATA[TV_MAJOR_COUNT] = {
    { /* 0 */
        "The Fool", "The Fool",
        TV_MOTIF_SPIRAL, TV_MOTIF_CIRCLE,
        "Spirit of Aether — pure potential before manifestation",
        "Air"
    },
    { /* 1 */
        "The Magician", "The Magus",
        TV_MOTIF_LEMNISCATE, TV_MOTIF_HEXAGRAM,
        "Magician of Power — the will directing the elements",
        "Mercury"
    },
    { /* 2 */
        "The High Priestess", "The Priestess",
        TV_MOTIF_CRESCENT, TV_MOTIF_COLUMN,
        "Priestess of the Silver Star — hidden knowledge, lunar wisdom",
        "Moon"
    },
    { /* 3 */
        "The Empress", "The Empress",
        TV_MOTIF_PENTAGRAM, TV_MOTIF_WAVE,
        "Daughter of the Mighty Ones — love, beauty, fertile creation",
        "Venus"
    },
    { /* 4 */
        "The Emperor", "The Emperor",
        TV_MOTIF_SQUARE, TV_MOTIF_CROSS,
        "Son of the Morning — sovereign power, martial authority",
        "Aries"
    },
    { /* 5 */
        "The Hierophant", "The Hierophant",
        TV_MOTIF_COLUMN, TV_MOTIF_HEXAGRAM,
        "Magus of the Eternal — teaching, tradition, inner hearing",
        "Taurus"
    },
    { /* 6 */
        "The Lovers", "The Lovers",
        TV_MOTIF_HEXAGRAM, TV_MOTIF_TRIANGLE,
        "Children of the Voice — union of opposites, choice",
        "Gemini"
    },
    { /* 7 */
        "The Chariot", "The Chariot",
        TV_MOTIF_SQUARE, TV_MOTIF_CROSS,
        "Child of the Powers of Water — triumph through will",
        "Cancer"
    },
    { /* 8 */
        "Justice", "Adjustment",
        TV_MOTIF_CROSS, TV_MOTIF_COLUMN,
        "Daughter of the Lords of Truth — balance, karma, equilibrium",
        "Libra"
    },
    { /* 9 */
        "The Hermit", "The Hermit",
        TV_MOTIF_EYE, TV_MOTIF_COLUMN,
        "Prophet of the Eternal — solitary wisdom, inner light",
        "Virgo"
    },
    { /* 10 */
        "Wheel of Fortune", "Fortune",
        TV_MOTIF_CIRCLE, TV_MOTIF_SPIRAL,
        "Lord of the Forces of Life — the wheel of cycles and change",
        "Jupiter"
    },
    { /* 11 */
        "Strength", "Lust",
        TV_MOTIF_LEMNISCATE, TV_MOTIF_TRIANGLE,
        "Daughter of the Flaming Sword — ecstasy, courage, primal force",
        "Leo"
    },
    { /* 12 */
        "The Hanged Man", "The Hanged Man",
        TV_MOTIF_CROSS, TV_MOTIF_WAVE,
        "Spirit of the Mighty Waters — surrender, reversal, sacrifice",
        "Water"
    },
    { /* 13 */
        "Death", "Death",
        TV_MOTIF_ANKH, TV_MOTIF_WAVE,
        "Child of the Great Transformers — transformation, rebirth",
        "Scorpio"
    },
    { /* 14 */
        "Temperance", "Art",
        TV_MOTIF_TRIANGLE, TV_MOTIF_HEXAGRAM,
        "Daughter of the Reconcilers — the art of alchemy, synthesis",
        "Sagittarius"
    },
    { /* 15 */
        "The Devil", "The Devil",
        TV_MOTIF_PENTAGRAM, TV_MOTIF_TRIANGLE,
        "Lord of the Gates of Matter — creative energy, mirth",
        "Capricorn"
    },
    { /* 16 */
        "The Tower", "The Tower",
        TV_MOTIF_LIGHTNING, TV_MOTIF_TRIANGLE,
        "Lord of the Hosts of the Mighty — war, destruction, revelation",
        "Mars"
    },
    { /* 17 */
        "The Star", "The Star",
        TV_MOTIF_RAYS, TV_MOTIF_WAVE,
        "Daughter of the Firmament — hope, meditation, the natural world",
        "Aquarius"
    },
    { /* 18 */
        "The Moon", "The Moon",
        TV_MOTIF_CRESCENT, TV_MOTIF_WAVE,
        "Ruler of Flux and Reflux — illusion, dreams, the unconscious",
        "Pisces"
    },
    { /* 19 */
        "The Sun", "The Sun",
        TV_MOTIF_RAYS, TV_MOTIF_CIRCLE,
        "Lord of the Fire of the World — glory, joy, illumination",
        "Sun"
    },
    { /* 20 */
        "Judgement", "The Aeon",
        TV_MOTIF_TRIANGLE, TV_MOTIF_RAYS,
        "Spirit of the Primal Fire — new aeon, awakening, rebirth",
        "Fire"
    },
    { /* 21 */
        "The World", "The Universe",
        TV_MOTIF_CIRCLE, TV_MOTIF_SQUARE,
        "The Great One of the Night of Time — completion, cosmic dance",
        "Saturn"
    },
};

/* Motif name lookup */
static const char *MOTIF_NAMES[TV_MOTIF_COUNT] = {
    "Circle",       /* TV_MOTIF_CIRCLE */
    "Triangle",     /* TV_MOTIF_TRIANGLE */
    "Square",       /* TV_MOTIF_SQUARE */
    "Hexagram",     /* TV_MOTIF_HEXAGRAM */
    "Pentagram",    /* TV_MOTIF_PENTAGRAM */
    "Spiral",       /* TV_MOTIF_SPIRAL */
    "Cross",        /* TV_MOTIF_CROSS */
    "Lemniscate",   /* TV_MOTIF_LEMNISCATE */
    "Crescent",     /* TV_MOTIF_CRESCENT */
    "Lightning",    /* TV_MOTIF_LIGHTNING */
    "Rays",         /* TV_MOTIF_RAYS */
    "Wave",         /* TV_MOTIF_WAVE */
    "Column",       /* TV_MOTIF_COLUMN */
    "Eye",          /* TV_MOTIF_EYE */
    "Ankh",         /* TV_MOTIF_ANKH */
};

/* Scale name lookup */
static const char *SCALE_NAMES[TV_SCALE_COUNT] = {
    "King",         /* TV_SCALE_KING */
    "Queen",        /* TV_SCALE_QUEEN */
    "Prince",       /* TV_SCALE_PRINCE */
    "Princess",     /* TV_SCALE_PRINCESS */
};

/* --- Public API --- */

tv_major_t tv_major_get(int number)
{
    tv_major_t card;

    if (number < 0 || number >= TV_MAJOR_COUNT) {
        card.number = -1;
        card.name = NULL;
        card.thoth_name = NULL;
        card.colors[0] = (color_rgb_t){0, 0, 0};
        card.colors[1] = (color_rgb_t){0, 0, 0};
        card.colors[2] = (color_rgb_t){0, 0, 0};
        card.colors[3] = (color_rgb_t){0, 0, 0};
        card.primary_motif = TV_MOTIF_CIRCLE;
        card.secondary_motif = TV_MOTIF_CIRCLE;
        card.symbol_desc = NULL;
        card.tree_path_index = -1;
        card.attribution = NULL;
        return card;
    }

    const card_static_t *data = &CARD_DATA[number];

    card.number = number;
    card.name = data->name;
    card.thoth_name = data->thoth_name;
    card.primary_motif = data->primary_motif;
    card.secondary_motif = data->secondary_motif;
    card.symbol_desc = data->symbol_desc;
    card.tree_path_index = card_to_tree_path(number);
    card.attribution = data->attribution;

    /* Compute colors from mood system */
    compute_color_scales(CARD_MOODS[number], card.colors);

    return card;
}

int tv_major_count(void)
{
    return TV_MAJOR_COUNT;
}

color_rgb_t tv_card_color(int number, tv_scale_t scale)
{
    color_rgb_t black = {0, 0, 0};

    if (number < 0 || number >= TV_MAJOR_COUNT) return black;
    if (scale < 0 || scale >= TV_SCALE_COUNT) return black;

    color_rgb_t colors[TV_COLOR_SCALES];
    compute_color_scales(CARD_MOODS[number], colors);
    return colors[scale];
}

color_rgb_t tv_card_primary(int number)
{
    return tv_card_color(number, TV_SCALE_KING);
}

const char *tv_motif_name(tv_motif_t motif)
{
    if (motif < 0 || motif >= TV_MOTIF_COUNT) return "?";
    return MOTIF_NAMES[motif];
}

const char *tv_scale_name(tv_scale_t scale)
{
    if (scale < 0 || scale >= TV_SCALE_COUNT) return "?";
    return SCALE_NAMES[scale];
}

tv_card_frame_t tv_card_frame(float width)
{
    tv_card_frame_t frame;
    frame.width = width;
    frame.height = width * (float)GL_PHI;
    frame.border = width * (float)GL_PHI_INV5;
    frame.symbol_radius = width * (float)GL_PHI_INV2;
    frame.title_y = frame.height * (float)GL_PHI_INV;
    frame.number_y = frame.height * (float)GL_PHI_INV4;
    return frame;
}

int tv_tree_position(int number, float *out_x, float *out_y)
{
    if (number < 0 || number >= TV_MAJOR_COUNT) return 0;

    int path_idx = card_to_tree_path(number);
    tree_path_t path = tree_path_get(path_idx);

    /* Generate a unit tree layout to get normalized positions */
    tree_layout_t layout = tree_layout_generate(1.0f, 1.0f);

    int from = path.from_sefirah;
    int to = path.to_sefirah;

    *out_x = (layout.node_x[from] + layout.node_x[to]) * 0.5f;
    *out_y = (layout.node_y[from] + layout.node_y[to]) * 0.5f;

    return 1;
}

int tv_tree_positions_all(float *out_x, float *out_y, int max_out)
{
    if (max_out <= 0) return 0;

    int count = max_out < TV_MAJOR_COUNT ? max_out : TV_MAJOR_COUNT;

    for (int i = 0; i < count; i++) {
        tv_tree_position(i, &out_x[i], &out_y[i]);
    }

    return count;
}
