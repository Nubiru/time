/* iching_interpret.c — I Ching hexagram interpretation data
 *
 * Static interpretation data for all 64 hexagrams based on traditional
 * Wilhelm/Baynes concepts. All judgments and images are original paraphrases.
 *
 * Pure functions: no globals, no malloc, no side effects. */

#include "iching_interpret.h"
#include "../../ui/content_i18n.h"
#include <stdio.h>
#include <string.h>

/* ===== Static hexagram data (King Wen order, 1-64) ===== */

static const ii_hexagram_t HEXAGRAM_DATA[64] = {
    /* 1 */
    { 1, "The Creative",
      "Pure creative force initiates all things",
      "Heaven upon heaven — the sage strengthens himself ceaselessly",
      "Creation, strength, initiative",
      "Heaven" },
    /* 2 */
    { 2, "The Receptive",
      "Pure receptive force nourishes all things",
      "Earth upon earth — the sage supports all beings with broad virtue",
      "Receptivity, devotion, nurture",
      "Earth" },
    /* 3 */
    { 3, "Difficulty at the Beginning",
      "Birth pangs of new endeavors require perseverance",
      "Thunder and rain — the sage brings order from chaos",
      "Beginning, struggle, growth",
      "Water/Thunder" },
    /* 4 */
    { 4, "Youthful Folly",
      "The young fool seeks the teacher, not the teacher the fool",
      "Spring at the foot of the mountain — the sage cultivates character",
      "Learning, innocence, guidance",
      "Mountain/Water" },
    /* 5 */
    { 5, "Waiting",
      "Sincerity and patience lead to crossing the great water",
      "Clouds rise to heaven — the sage eats, drinks, and is joyful",
      "Patience, nourishment, trust",
      "Water/Heaven" },
    /* 6 */
    { 6, "Conflict",
      "Inner truth and caution resolve disagreement",
      "Heaven and water move apart — the sage plans before acting",
      "Conflict, caution, mediation",
      "Heaven/Water" },
    /* 7 */
    { 7, "The Army",
      "Discipline and just leadership organize the people",
      "Water within the earth — the sage nurtures the people and gathers strength",
      "Leadership, discipline, strategy",
      "Earth/Water" },
    /* 8 */
    { 8, "Holding Together",
      "Union requires a central figure of sincerity",
      "Water upon the earth — the ancient kings established their states",
      "Unity, alliance, cooperation",
      "Water/Earth" },
    /* 9 */
    { 9, "Small Taming",
      "Gentle restraint achieves what force cannot",
      "Wind over heaven — the sage refines outward form and virtue",
      "Restraint, refinement, patience",
      "Wind/Heaven" },
    /* 10 */
    { 10, "Treading",
      "Treading upon the tiger's tail with care and propriety brings success",
      "Heaven above the lake — the sage distinguishes high and low",
      "Conduct, propriety, courage",
      "Heaven/Lake" },
    /* 11 */
    { 11, "Peace",
      "Heaven and earth unite — the small departs, the great approaches",
      "Earth above heaven — the ruler channels the way of heaven for the people",
      "Peace, harmony, prosperity",
      "Earth/Heaven" },
    /* 12 */
    { 12, "Standstill",
      "Heaven and earth do not communicate — decline and obstruction",
      "Heaven above earth — the sage withdraws into inner worth",
      "Stagnation, withdrawal, patience",
      "Heaven/Earth" },
    /* 13 */
    { 13, "Fellowship",
      "True fellowship among people through shared principles",
      "Heaven with fire — the sage organizes clans and distinguishes things",
      "Community, fellowship, openness",
      "Heaven/Fire" },
    /* 14 */
    { 14, "Great Possession",
      "Fire in heaven — supreme success through understanding what is great",
      "Fire above heaven — the sage curbs evil and promotes good",
      "Abundance, responsibility, generosity",
      "Fire/Heaven" },
    /* 15 */
    { 15, "Modesty",
      "Modesty creates success — the mountain within the earth",
      "Mountain within earth — the sage reduces what is too much and increases what is too little",
      "Humility, balance, modesty",
      "Earth/Mountain" },
    /* 16 */
    { 16, "Enthusiasm",
      "Thunder over earth — enthusiasm moves all things",
      "Thunder over earth — the ancient kings made music to honor merit",
      "Enthusiasm, momentum, inspiration",
      "Thunder/Earth" },
    /* 17 */
    { 17, "Following",
      "Adapting to the time and following with sincerity brings success",
      "Thunder within the lake — the sage rests at nightfall",
      "Adaptability, following, service",
      "Lake/Thunder" },
    /* 18 */
    { 18, "Work on the Decayed",
      "What has been corrupted through neglect can be restored through effort",
      "Wind at the foot of the mountain — the sage stirs up the people and nourishes virtue",
      "Repair, renewal, responsibility",
      "Mountain/Wind" },
    /* 19 */
    { 19, "Approach",
      "The great approaches with willing condescension toward the small",
      "Earth above the lake — the sage teaches and shelters without limit",
      "Advance, influence, generosity",
      "Earth/Lake" },
    /* 20 */
    { 20, "Contemplation",
      "The sage gazes upon the divine meaning beneath appearances",
      "Wind over earth — the ancient kings visited the regions to teach the people",
      "Observation, insight, perspective",
      "Wind/Earth" },
    /* 21 */
    { 21, "Biting Through",
      "Energetic biting through obstacles restores unity",
      "Thunder and lightning — the ancient kings enforced the laws with clarity",
      "Decisiveness, justice, breakthrough",
      "Fire/Thunder" },
    /* 22 */
    { 22, "Grace",
      "Beauty adorns form but substance must remain within",
      "Fire at the foot of the mountain — the sage brightens governance without daring to judge",
      "Beauty, form, adornment",
      "Mountain/Fire" },
    /* 23 */
    { 23, "Splitting Apart",
      "The dark forces peel away what has been built — accept and wait",
      "Mountain resting on the earth — the ruler strengthens the foundation of the people",
      "Decay, erosion, acceptance",
      "Mountain/Earth" },
    /* 24 */
    { 24, "Return",
      "After decay, the turning point — the one light returns below",
      "Thunder within the earth — the ancient kings closed the passes at solstice",
      "Renewal, turning point, return",
      "Earth/Thunder" },
    /* 25 */
    { 25, "Innocence",
      "Acting from genuine nature without ulterior motive brings success",
      "Thunder under heaven — the ancient kings nourished all beings in accord with the season",
      "Innocence, sincerity, spontaneity",
      "Heaven/Thunder" },
    /* 26 */
    { 26, "Great Taming",
      "Firm restraint of great creative power builds reserves of strength",
      "Heaven within the mountain — the sage studies the ancients to cultivate character",
      "Restraint, accumulation, cultivation",
      "Mountain/Heaven" },
    /* 27 */
    { 27, "Nourishment",
      "Attending to what nourishes body and mind brings good fortune",
      "Thunder at the foot of the mountain — the sage is careful with words and temperate in eating",
      "Nourishment, sustenance, care",
      "Mountain/Thunder" },
    /* 28 */
    { 28, "Great Excess",
      "The ridgepole bends — extraordinary measures are needed in extraordinary times",
      "The lake rises above the trees — the sage stands alone without fear",
      "Excess, crisis, courage",
      "Lake/Wind" },
    /* 29 */
    { 29, "The Abysmal",
      "Repeated danger teaches sincerity of heart — flow like water",
      "Water flows on and reaches the goal — the sage teaches through repeated practice",
      "Danger, depth, perseverance",
      "Water/Water" },
    /* 30 */
    { 30, "The Clinging",
      "Luminous clarity depends on something to cling to — accept dependence",
      "Fire doubled produces great brightness — the sage illuminates all regions",
      "Clarity, dependence, brightness",
      "Fire/Fire" },
    /* 31 */
    { 31, "Influence",
      "Mutual attraction between the yielding and the firm brings union",
      "The lake upon the mountain — the sage receives others with openness",
      "Attraction, sensitivity, courtship",
      "Lake/Mountain" },
    /* 32 */
    { 32, "Duration",
      "Enduring consistency in one's way of life brings success",
      "Thunder and wind reinforce each other — the sage stands firm in his direction",
      "Endurance, consistency, marriage",
      "Thunder/Wind" },
    /* 33 */
    { 33, "Retreat",
      "Strategic withdrawal before the advancing dark preserves strength",
      "Mountain under heaven — the sage keeps the inferior at a distance with dignity",
      "Retreat, strategy, dignity",
      "Heaven/Mountain" },
    /* 34 */
    { 34, "Great Power",
      "Great power combined with righteousness moves heaven and earth",
      "Thunder above heaven — the sage treads no path contrary to propriety",
      "Power, strength, righteousness",
      "Thunder/Heaven" },
    /* 35 */
    { 35, "Progress",
      "The bright sun rises over the earth — willing service brings rapid advance",
      "The sun rises over the earth — the sage brightens his bright virtue",
      "Advancement, recognition, daylight",
      "Fire/Earth" },
    /* 36 */
    { 36, "Darkening of the Light",
      "In times of darkness the sage conceals his light to endure adversity",
      "The sun sinks into the earth — the sage veils his brightness in dealings with the multitude",
      "Concealment, endurance, prudence",
      "Earth/Fire" },
    /* 37 */
    { 37, "The Family",
      "The well-ordered family is the foundation of all social order",
      "Wind comes from fire — the sage speaks with substance and acts with consistency",
      "Family, order, domestic harmony",
      "Wind/Fire" },
    /* 38 */
    { 38, "Opposition",
      "Despite outward opposition, underlying affinity makes small things possible",
      "Fire above the lake — the sage remains individual within community",
      "Polarity, tension, independence",
      "Fire/Lake" },
    /* 39 */
    { 39, "Obstruction",
      "Facing an abyss before and a mountain behind — turn inward and gather allies",
      "Water on the mountain — the sage turns attention to self-improvement",
      "Obstacle, introspection, perseverance",
      "Water/Mountain" },
    /* 40 */
    { 40, "Deliverance",
      "When release from tension comes, return to normalcy swiftly",
      "Thunder and rain arise — the sage pardons errors and forgives transgressions",
      "Liberation, forgiveness, relief",
      "Thunder/Water" },
    /* 41 */
    { 41, "Decrease",
      "Decrease of the lower enriches the higher — sincerity in simplicity brings blessings",
      "The lake at the foot of the mountain — the sage controls anger and restrains instincts",
      "Sacrifice, simplicity, sincerity",
      "Mountain/Lake" },
    /* 42 */
    { 42, "Increase",
      "The ruler decreases self to increase the people — generous sacrifice brings benefit",
      "Wind and thunder strengthen each other — the sage moves toward good and corrects faults",
      "Growth, benefit, generosity",
      "Wind/Thunder" },
    /* 43 */
    { 43, "Breakthrough",
      "Resolute action brings dark matters to light — proclaim the danger openly",
      "The lake rises to heaven — the sage distributes blessings to those below",
      "Resolution, determination, openness",
      "Lake/Heaven" },
    /* 44 */
    { 44, "Coming to Meet",
      "The dark principle advances from below — recognize and contain it early",
      "Wind under heaven — the ruler proclaims his commands to the four quarters",
      "Encounter, temptation, vigilance",
      "Heaven/Wind" },
    /* 45 */
    { 45, "Gathering Together",
      "People gather around a strong leader — prepare for the unexpected",
      "The lake over the earth — the sage renews his weapons to meet the unforeseen",
      "Assembly, gathering, preparedness",
      "Lake/Earth" },
    /* 46 */
    { 46, "Pushing Upward",
      "Gentle effort directed steadily upward meets with supreme success",
      "Within the earth wood grows — the sage builds great things from small beginnings",
      "Ascent, effort, growth",
      "Earth/Wind" },
    /* 47 */
    { 47, "Oppression",
      "Exhaustion of resources tests character — the sage stakes his life on following his will",
      "The lake has no water — the sage risks his life to fulfill his purpose",
      "Adversity, exhaustion, resolve",
      "Lake/Water" },
    /* 48 */
    { 48, "The Well",
      "The well nourishes all without being exhausted — seek the unchanging source",
      "Water over wood — the sage encourages the people and counsels mutual aid",
      "Source, constancy, community",
      "Water/Wind" },
    /* 49 */
    { 49, "Revolution",
      "When the time is ripe, revolutionary change commands belief",
      "Fire in the lake — the sage orders the calendar and clarifies the seasons",
      "Transformation, timing, renewal",
      "Lake/Fire" },
    /* 50 */
    { 50, "The Cauldron",
      "The sacred vessel transforms raw material into nourishment for the wise",
      "Fire over wood — the sage consolidates destiny by making his position correct",
      "Transformation, nourishment, culture",
      "Fire/Wind" },
    /* 51 */
    { 51, "The Arousing",
      "Shock and thunder bring fear, then laughter — the sage examines himself through awe",
      "Repeated thunder — the sage sets things in order through fearful reflection",
      "Shock, awakening, initiative",
      "Thunder/Thunder" },
    /* 52 */
    { 52, "Keeping Still",
      "Stillness of the mountain — the sage quiets the mind and does not go beyond his position",
      "Mountains joined together — the sage does not permit thoughts to go beyond the situation",
      "Stillness, meditation, composure",
      "Mountain/Mountain" },
    /* 53 */
    { 53, "Gradual Progress",
      "Gradual development like a tree on the mountain brings lasting results",
      "A tree on the mountain grows slowly — the sage abides in dignity and virtue",
      "Patience, development, steady growth",
      "Wind/Mountain" },
    /* 54 */
    { 54, "The Marrying Maiden",
      "The younger sister follows the elder — knowing one's place brings eventual good",
      "Thunder over the lake — the sage understands the transitory in light of eternity",
      "Subordination, propriety, transitions",
      "Thunder/Lake" },
    /* 55 */
    { 55, "Abundance",
      "Fullness and abundance at the zenith — act now for the sun must set",
      "Thunder and lightning together — the sage decides lawsuits and carries out punishments",
      "Fullness, zenith, decisiveness",
      "Thunder/Fire" },
    /* 56 */
    { 56, "The Wanderer",
      "The traveler succeeds through smallness and adaptability, not grandeur",
      "Fire on the mountain — the sage is clear and cautious in applying penalties",
      "Travel, impermanence, adaptability",
      "Fire/Mountain" },
    /* 57 */
    { 57, "The Gentle",
      "Persistent gentle penetration achieves what aggression cannot",
      "Winds following one another — the sage spreads his commands and carries out his affairs",
      "Penetration, influence, subtlety",
      "Wind/Wind" },
    /* 58 */
    { 58, "The Joyous",
      "True joy comes from inner firmness expressed with outward gentleness",
      "Lakes joined together — the sage joins with friends for discussion and practice",
      "Joy, communication, fellowship",
      "Lake/Lake" },
    /* 59 */
    { 59, "Dispersion",
      "Wind over water disperses rigid separations — use gentle means to dissolve barriers",
      "Wind drives over the water — the ancient kings made offerings and built temples",
      "Dissolution, unity, devotion",
      "Wind/Water" },
    /* 60 */
    { 60, "Limitation",
      "Proper limits bring structure — but bitter limitations should not be held to",
      "Water over the lake — the sage creates number and measure to examine virtue and conduct",
      "Limitation, moderation, structure",
      "Water/Lake" },
    /* 61 */
    { 61, "Inner Truth",
      "Inner truth reaches even pigs and fish — sincerity moves all beings",
      "Wind over the lake — the sage deliberates upon punishments and delays execution",
      "Sincerity, empathy, truth",
      "Wind/Lake" },
    /* 62 */
    { 62, "Small Excess",
      "In small matters excess is permissible — the flying bird leaves its song behind",
      "Thunder on the mountain — the sage is reverent in conduct and sorrowful in mourning",
      "Modesty, detail, caution",
      "Thunder/Mountain" },
    /* 63 */
    { 63, "After Completion",
      "All lines in their proper place — success, but vigilance against disorder",
      "Water over fire — the sage reflects on misfortune and prepares against it",
      "Completion, vigilance, order",
      "Water/Fire" },
    /* 64 */
    { 64, "Before Completion",
      "The final crossing is not yet accomplished — careful discernment of forces is needed",
      "Fire over water — the sage carefully distinguishes things and puts them in their places",
      "Transition, potential, discernment",
      "Fire/Water" },
};

/* Invalid sentinel returned for out-of-range king_wen */
static const ii_hexagram_t INVALID_HEXAGRAM = {
    -1, "?", "?", "?", "?", "?"
};

ii_hexagram_t ii_hexagram_data(int king_wen)
{
    if (king_wen < 1 || king_wen > 64)
        return INVALID_HEXAGRAM;
    return HEXAGRAM_DATA[king_wen - 1];
}

iching_interp_t ii_interpret(int king_wen, const char *upper_tri,
                             const char *lower_tri)
{
    iching_interp_t r;
    memset(&r, 0, sizeof(r));

    if (king_wen < 1 || king_wen > 64) {
        snprintf(r.glyph,  sizeof(r.glyph),  "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    const char *up = upper_tri ? upper_tri : "?";
    const char *lo = lower_tri ? lower_tri : "?";
    const ii_hexagram_t *h = &HEXAGRAM_DATA[king_wen - 1];

    snprintf(r.glyph, sizeof(r.glyph), "%d", king_wen);

    snprintf(r.glance, sizeof(r.glance),
             "Hexagram %d %s \xe2\x80\x94 %s over %s",
             king_wen, h->name, up, lo);

    snprintf(r.detail, sizeof(r.detail),
             "%s: %s. The Image: %s. Keywords: %s. "
             "Upper trigram: %s, Lower: %s.",
             h->name, h->judgment, h->image, h->keywords, up, lo);

    return r;
}

int ii_hexagram_count(void)
{
    return 64;
}

iching_interp_t ii_interpret_locale(int king_wen, const char *upper_tri,
                                    const char *lower_tri,
                                    i18n_locale_t locale)
{
    if (locale == I18N_LOCALE_EN) {
        return ii_interpret(king_wen, upper_tri, lower_tri);
    }

    iching_interp_t r;
    memset(&r, 0, sizeof(r));

    if (king_wen < 1 || king_wen > 64) {
        snprintf(r.glyph,  sizeof(r.glyph),  "?");
        snprintf(r.glance, sizeof(r.glance), "?");
        snprintf(r.detail, sizeof(r.detail), "?");
        return r;
    }

    const char *up = upper_tri ? upper_tri : "?";
    const char *lo = lower_tri ? lower_tri : "?";

    char key[64];

    snprintf(key, sizeof(key), "iching.hex.%d.name", king_wen);
    const char *name = content_get(key, locale);

    snprintf(key, sizeof(key), "iching.hex.%d.judgment", king_wen);
    const char *judgment = content_get(key, locale);

    snprintf(key, sizeof(key), "iching.hex.%d.image", king_wen);
    const char *image = content_get(key, locale);

    snprintf(key, sizeof(key), "iching.hex.%d.keywords", king_wen);
    const char *keywords = content_get(key, locale);

    /* Glyph: hexagram number */
    snprintf(r.glyph, sizeof(r.glyph), "%d", king_wen);

    /* Glance */
    const char *tpl_glance = content_get("iching.tpl.glance", locale);
    snprintf(r.glance, sizeof(r.glance), tpl_glance,
             king_wen, name, up, lo);

    /* Detail */
    const char *tpl_detail = content_get("iching.tpl.detail", locale);
    snprintf(r.detail, sizeof(r.detail), tpl_detail,
             name, judgment, image, keywords, up, lo);

    return r;
}
