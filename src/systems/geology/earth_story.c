/* earth_story.c — Cinematic Earth History: 20 dramatic moments across 4.5 Ga.
 * Pure static const data. No malloc, no globals, no side effects.
 * Data synthesized from geological consensus, ICS chart, paleontology. */

#include "earth_story.h"
#include <stddef.h>
#include <stdio.h>
#include <math.h>

/* --- Static moment data (chronological, oldest first, descending Ma) --- */

static const es_moment_t MOMENTS[ES_MOMENT_COUNT] = {
    { /* 0 */
        .index = 0,
        .title = "Birth of Earth",
        .ma = 4540.0,
        .drama = ES_DRAMA_CATASTROPHIC,
        .visual =
            "A swirling accretion disk of dust and molten rock collapses inward. "
            "Glowing planetesimals slam together in flashes of white heat, building "
            "a molten sphere that radiates orange against the black of space.",
        .narrative =
            "Earth forms from the solar nebula by accretion, a violent process of "
            "collision and merging that takes tens of millions of years. The young "
            "planet is entirely molten, a magma ocean thousands of kilometers deep "
            "glowing beneath a thick atmosphere of vaporized rock. Iron sinks to "
            "form the core while lighter silicates float upward, beginning the "
            "differentiation that gives Earth its layered structure.",
        .aftermath =
            "The magma ocean slowly cools, forming the first protocrust. But the "
            "worst impact is still to come.",
        .sound_hint = "deep_rumble"
    },
    { /* 1 */
        .index = 1,
        .title = "Theia Impact",
        .ma = 4500.0,
        .drama = ES_DRAMA_APOCALYPTIC,
        .visual =
            "A Mars-sized body fills half the sky, closing at kilometers per second. "
            "The collision vaporizes both surfaces in a blinding flash. A ring of "
            "molten debris spirals outward, slowly coalescing into the Moon.",
        .narrative =
            "A protoplanet the size of Mars, named Theia, strikes Earth at an oblique "
            "angle. The energy released is almost incomprehensible: both bodies partially "
            "melt, and a vast cloud of vaporized rock is flung into orbit. This debris "
            "disk cools and coalesces within a few thousand years to form the Moon. The "
            "impact tilts Earth's axis, giving us seasons, and the Moon's gravity will "
            "stabilize that tilt for billions of years, making complex life possible.",
        .aftermath =
            "Earth is left with a companion that slows its rotation, raises tides, "
            "and stabilizes the axial tilt that makes seasons gentle enough for life.",
        .sound_hint = "cataclysmic_impact"
    },
    { /* 2 */
        .index = 2,
        .title = "Late Heavy Bombardment",
        .ma = 4100.0,
        .drama = ES_DRAMA_CATASTROPHIC,
        .visual =
            "The night sky blazes with incoming asteroids trailing fire. Craters bloom "
            "across the surface in rapid succession, each impact flashing white then "
            "fading to glowing orange pools of fresh magma.",
        .narrative =
            "A gravitational reshuffling of the outer planets sends a rain of asteroids "
            "and comets into the inner solar system. For over a hundred million years, "
            "Earth's surface is pummeled relentlessly. Each large impact sterilizes the "
            "surface, boiling away any nascent oceans. Yet these same impactors deliver "
            "vast quantities of water and organic molecules from beyond the frost line, "
            "seeding the raw materials for life.",
        .aftermath =
            "The bombardment tapers off, leaving a cratered world rich in water and "
            "organic chemistry, ready for oceans to form.",
        .sound_hint = "impact_barrage"
    },
    { /* 3 */
        .index = 3,
        .title = "First Oceans",
        .ma = 3800.0,
        .drama = ES_DRAMA_NOTABLE,
        .visual =
            "Steam clears to reveal an endless expanse of dark water under a pale "
            "orange sky. Volcanic islands break the surface, trailing plumes of white "
            "vapor. For the first time, blue exists on this world.",
        .narrative =
            "As Earth's surface finally cools below the boiling point of water, the "
            "thick steam atmosphere begins to condense. Rain falls for millions of "
            "years, filling every basin and depression. The first global ocean forms, "
            "dark and iron-rich, under a sky still thick with carbon dioxide. Zircon "
            "crystals from this era prove liquid water existed, and hydrothermal vents "
            "on the ocean floor begin pumping mineral-rich fluid into the deep.",
        .aftermath =
            "Liquid water creates the medium in which chemistry can become biology. "
            "The stage for life is set.",
        .sound_hint = "ocean_waves"
    },
    { /* 4 */
        .index = 4,
        .title = "Spark of Life",
        .ma = 3500.0,
        .drama = ES_DRAMA_DRAMATIC,
        .visual =
            "In shallow, sun-drenched tidal pools, mats of green-brown stromatolites "
            "ripple gently in warm currents. Tiny bubbles of oxygen rise from their "
            "surfaces, catching the light like jewels.",
        .narrative =
            "The oldest stromatolites appear in the fossil record, layered domes built "
            "by communities of cyanobacteria trapping sediment. Something extraordinary "
            "has happened: chemistry has become biology. These microbes harvest sunlight "
            "through photosynthesis, the most important metabolic innovation in Earth's "
            "history. Though the mechanism is still debated, life has crossed the "
            "threshold from replicating molecules to living cells.",
        .aftermath =
            "Life takes hold and never lets go. Bacteria will dominate for the "
            "next two billion years, slowly reshaping the planet's chemistry.",
        .sound_hint = "gentle_bubbling"
    },
    { /* 5 */
        .index = 5,
        .title = "Great Oxidation Event",
        .ma = 2400.0,
        .drama = ES_DRAMA_DRAMATIC,
        .visual =
            "The oceans turn from green-black to rusty red as dissolved iron oxidizes "
            "and precipitates. The sky shifts from orange haze to a paler, clearer "
            "blue. Vast stretches of anaerobic life wither and die.",
        .narrative =
            "Cyanobacteria have been producing oxygen for hundreds of millions of "
            "years, but geological sinks have absorbed it all. Now the sinks are "
            "saturated. Free oxygen accumulates in the atmosphere for the first time, "
            "reaching perhaps two percent. For anaerobic life, this is a catastrophe: "
            "oxygen is a lethal poison to organisms that evolved without it. Earth's "
            "first mass extinction unfolds silently in the microbial world.",
        .aftermath =
            "The ozone layer begins to form, shielding the surface from UV radiation "
            "and opening new habitats for life on land.",
        .sound_hint = "wind_shift"
    },
    { /* 6 */
        .index = 6,
        .title = "Snowball Earth",
        .ma = 717.0,
        .drama = ES_DRAMA_CATASTROPHIC,
        .visual =
            "Ice sheets advance from both poles, meeting at the equator. The entire "
            "planet gleams white under a dim sun. Only volcanic hotspots glow red "
            "through the ice, thin lifelines for whatever survives beneath.",
        .narrative =
            "As the supercontinent Rodinia breaks apart, massive weathering of fresh "
            "rock draws down atmospheric CO2 until a runaway ice-albedo feedback "
            "freezes the entire planet. The Sturtian glaciation locks Earth in ice "
            "for perhaps fifty million years. Oceans freeze to a depth of a kilometer. "
            "Life retreats to hydrothermal vents, thin meltwater layers, and cracks "
            "in the ice. It is the closest Earth has ever come to total sterilization.",
        .aftermath =
            "Volcanic CO2 accumulates until a super-greenhouse effect shatters the "
            "ice. The thaw triggers the Ediacaran explosion of complex animal life.",
        .sound_hint = "ice_cracking"
    },
    { /* 7 */
        .index = 7,
        .title = "Cambrian Explosion",
        .ma = 541.0,
        .drama = ES_DRAMA_DRAMATIC,
        .visual =
            "Shallow seas teem with alien forms: trilobites with crystal eyes, "
            "five-eyed Opabinia, Anomalocaris hunting with grasping appendages. "
            "Every shape of body plan appears in a kaleidoscopic burst of evolution.",
        .narrative =
            "In the span of roughly twenty million years, nearly every major animal "
            "phylum appears in the fossil record. Eyes evolve, triggering an "
            "evolutionary arms race between predator and prey. Shells, spines, and "
            "speed become matters of life and death. The Burgess Shale preserves a "
            "snapshot of this extraordinary moment: a world of biological experiments, "
            "most of which will fail, but whose surviving lineages define all animal "
            "life that follows.",
        .aftermath =
            "The body plans established in this explosion persist to the present "
            "day. Every animal alive, including humans, traces its architecture here.",
        .sound_hint = "underwater_chorus"
    },
    { /* 8 */
        .index = 8,
        .title = "First Fish",
        .ma = 480.0,
        .drama = ES_DRAMA_NOTABLE,
        .visual =
            "Jawless, armored fish glide through murky Ordovician seas, their bony "
            "plates glinting in shafts of sunlight filtering through the water. "
            "Small and slow, they are dwarfed by the invertebrates around them.",
        .narrative =
            "The first vertebrates appear as small, jawless creatures armored in bone. "
            "They seem insignificant beside the giant sea scorpions and nautiloids that "
            "rule the Ordovician oceans. But their internal skeleton, notochord, and "
            "centralized nervous system represent a body plan of extraordinary potential. "
            "From these humble fish, every land vertebrate will eventually descend: "
            "amphibians, reptiles, birds, mammals, and us.",
        .aftermath =
            "Jaws evolve from gill arches, transforming fish into formidable "
            "predators and launching the vertebrate dynasty.",
        .sound_hint = "deep_water_ambience"
    },
    { /* 9 */
        .index = 9,
        .title = "Plants Conquer Land",
        .ma = 470.0,
        .drama = ES_DRAMA_NOTABLE,
        .visual =
            "Low mats of green creep across bare rock near the water's edge. "
            "Tiny spore-bearing stems rise a few centimeters into the air, the "
            "first green on a landscape of brown and grey that stretches to the horizon.",
        .narrative =
            "For billions of years, land has been barren rock and dust. Now simple "
            "plants related to modern liverworts make the leap from water to shore. "
            "They develop waxy cuticles to prevent drying out and rudimentary roots "
            "to anchor in soil. This quiet colonization will transform Earth more "
            "profoundly than any asteroid: plants will create soil, regulate the "
            "water cycle, and pump oxygen to levels that allow giant insects and "
            "the first land animals.",
        .aftermath =
            "Within a hundred million years, towering forests cover the continents, "
            "and the greening of land accelerates oxygen production dramatically.",
        .sound_hint = "wind_through_moss"
    },
    { /* 10 */
        .index = 10,
        .title = "The Great Dying",
        .ma = 252.0,
        .drama = ES_DRAMA_APOCALYPTIC,
        .visual =
            "Volcanic chains stretch horizon to horizon across Siberia, rivers of "
            "lava pouring across a continent. The sky is choked with ash and sulfur. "
            "The oceans turn anoxic, stained purple-black with sulfur bacteria.",
        .narrative =
            "The Siberian Traps erupt for roughly a million years, flooding an area "
            "the size of Western Europe with basalt. The volcanism injects colossal "
            "quantities of CO2, methane, and sulfur into the atmosphere. Temperatures "
            "spike, the ozone layer collapses, and the oceans lose their oxygen. "
            "Ninety-six percent of all marine species and seventy percent of land "
            "vertebrates perish in Earth's worst mass extinction. Life comes closer "
            "to total annihilation than at any other time.",
        .aftermath =
            "Recovery takes ten million years. From the survivors emerge the "
            "archosaurs, ancestors of dinosaurs, who will inherit the emptied world.",
        .sound_hint = "volcanic_roar"
    },
    { /* 11 */
        .index = 11,
        .title = "Dawn of Dinosaurs",
        .ma = 230.0,
        .drama = ES_DRAMA_DRAMATIC,
        .visual =
            "Small bipedal reptiles, no larger than dogs, dart through fern-covered "
            "Triassic floodplains. They are fast and alert, dwarfed by the larger "
            "archosaurs that still dominate the landscape.",
        .narrative =
            "In the aftermath of the Great Dying, the emptied world offers ecological "
            "opportunities on a scale never seen before. Among the archosaurs that "
            "diversify to fill vacant niches, a line of small, upright bipeds appears "
            "in what is now Argentina. Eoraptor and Herrerasaurus are modest creatures, "
            "but their efficient lungs, upright gait, and hollow bones give them an "
            "edge. They are the first true dinosaurs, minor players in the Triassic "
            "who will become the dominant land animals for 165 million years.",
        .aftermath =
            "The end-Triassic extinction clears their competitors, and dinosaurs "
            "radiate into every large-bodied niche on land.",
        .sound_hint = "distant_calls"
    },
    { /* 12 */
        .index = 12,
        .title = "Pangaea Breaks Apart",
        .ma = 200.0,
        .drama = ES_DRAMA_DRAMATIC,
        .visual =
            "The supercontinent cracks along a great rift valley. Lava wells up "
            "from the widening gap as ocean water floods in. Two landmasses drift "
            "apart, and between them a young Atlantic Ocean is born.",
        .narrative =
            "Pangaea, the last great supercontinent, begins to split along a rift "
            "that will become the Atlantic Ocean. Massive volcanic eruptions along "
            "the Central Atlantic Magmatic Province accompany the breakup, triggering "
            "the end-Triassic extinction. As Laurasia separates from Gondwana, ocean "
            "currents reorganize, climates shift, and isolated landmasses become "
            "laboratories for independent evolution. The world's geography begins "
            "its long transformation toward the map we know today.",
        .aftermath =
            "Continental separation drives evolutionary divergence. Dinosaurs on "
            "different landmasses evolve along separate paths for 135 million years.",
        .sound_hint = "tectonic_groan"
    },
    { /* 13 */
        .index = 13,
        .title = "Archaeopteryx Takes Flight",
        .ma = 150.0,
        .drama = ES_DRAMA_NOTABLE,
        .visual =
            "A feathered creature with a dinosaur's toothed jaw and a bird's broad "
            "wings glides between cycad trees in a Jurassic lagoon. Its shadow "
            "ripples across limestone shallows below.",
        .narrative =
            "In the Late Jurassic limestone lagoons of Bavaria, a creature is preserved "
            "that bridges two worlds. Archaeopteryx has the teeth, clawed fingers, and "
            "bony tail of a small theropod dinosaur, but it also has the asymmetric "
            "flight feathers of a bird. Feathered dinosaurs have been diversifying for "
            "millions of years, but Archaeopteryx demonstrates that powered flight has "
            "evolved from within the dinosaur lineage. Birds are not separate from "
            "dinosaurs; they are dinosaurs.",
        .aftermath =
            "Birds diversify through the Cretaceous and survive the asteroid impact, "
            "becoming the ten thousand living species of dinosaur among us today.",
        .sound_hint = "wingbeats"
    },
    { /* 14 */
        .index = 14,
        .title = "Flowers Bloom",
        .ma = 125.0,
        .drama = ES_DRAMA_NOTABLE,
        .visual =
            "Among the dark green conifers and ferns, small white and yellow flowers "
            "open for the first time. Insects hover and land, dusted with pollen. "
            "A quiet revolution in color begins to spread across the continents.",
        .narrative =
            "The first flowering plants appear in the Early Cretaceous, and their "
            "impact on terrestrial ecosystems is profound. By evolving flowers and "
            "fruit, angiosperms forge partnerships with insect pollinators and animal "
            "seed dispersers, creating co-evolutionary feedback loops that accelerate "
            "speciation on both sides. Within fifty million years, flowering plants "
            "dominate every continent. They restructure forests, create grasslands, "
            "and produce the fruits and grains that will eventually feed humanity.",
        .aftermath =
            "Co-evolution with pollinators drives an explosion of insect and plant "
            "diversity that transforms every land ecosystem on Earth.",
        .sound_hint = "buzzing_insects"
    },
    { /* 15 */
        .index = 15,
        .title = "The Asteroid",
        .ma = 66.0,
        .drama = ES_DRAMA_APOCALYPTIC,
        .visual =
            "A point of light swells to fill the sky in seconds. The impact flash "
            "is brighter than the sun. A wall of superheated air races outward, "
            "flattening forests for thousands of kilometers. The world burns.",
        .narrative =
            "A ten-kilometer asteroid strikes the Yucatan Peninsula at twenty "
            "kilometers per second, releasing the energy of a hundred million "
            "nuclear weapons. The impact ignites global firestorms, triggers "
            "mega-tsunamis a kilometer high, and launches so much debris into the "
            "stratosphere that sunlight is blocked for years. Photosynthesis "
            "collapses, food chains disintegrate, and seventy-six percent of all "
            "species vanish, including every non-avian dinosaur. A hundred and "
            "sixty-five million years of dominance end in a single afternoon.",
        .aftermath =
            "The world goes dark and cold for years. When the skies clear, "
            "small mammals emerge into a world emptied of large competitors.",
        .sound_hint = "explosion_then_silence"
    },
    { /* 16 */
        .index = 16,
        .title = "Rise of Mammals",
        .ma = 55.0,
        .drama = ES_DRAMA_NOTABLE,
        .visual =
            "Lush tropical forests cover the warming Earth. Small furry creatures "
            "with bright eyes peer from branches. In the distance, early whales "
            "wade into shallow seas. The world is green, warm, and full of new forms.",
        .narrative =
            "With the dinosaurs gone, mammals radiate into every available niche "
            "with astonishing speed. Within ten million years of the impact, bats "
            "have taken to the air, whales have returned to the sea, and the first "
            "primates are climbing through tropical canopies. The Paleocene-Eocene "
            "Thermal Maximum pushes global temperatures to their highest in sixty "
            "million years, and tropical forests reach the poles. Mammals grow "
            "larger, faster, and more diverse than ever before.",
        .aftermath =
            "Primates diversify in the tropical forests, setting the stage for "
            "the great apes and eventually the human lineage.",
        .sound_hint = "forest_dawn_chorus"
    },
    { /* 17 */
        .index = 17,
        .title = "Upright Walk",
        .ma = 4.0,
        .drama = ES_DRAMA_DRAMATIC,
        .visual =
            "On an open African savanna dotted with acacia trees, a small figure "
            "walks upright on two legs across a volcanic ash field. Footprints "
            "press into the soft grey surface, preserved forever.",
        .narrative =
            "In East Africa, a line of apes begins walking upright on the open "
            "savanna. Australopithecus afarensis, famously represented by the "
            "skeleton nicknamed Lucy, stands barely a meter tall but walks as we "
            "do. Bipedalism frees the hands for carrying, tool use, and eventually "
            "the fine manipulation that will build civilizations. The Laetoli "
            "footprints, preserved in volcanic ash 3.6 million years ago, are the "
            "oldest direct evidence of upright walking in our lineage.",
        .aftermath =
            "Free hands lead to stone tools, larger brains, and the slow "
            "emergence of the genus Homo.",
        .sound_hint = "footsteps_on_earth"
    },
    { /* 18 */
        .index = 18,
        .title = "Fire and Language",
        .ma = 0.3,
        .drama = ES_DRAMA_DRAMATIC,
        .visual =
            "A circle of humans sits around a controlled fire under a canopy of "
            "stars. Faces are lit by flickering orange light. Hands gesture as "
            "stories are told. On a cave wall behind them, painted animals leap.",
        .narrative =
            "Homo sapiens emerges in Africa roughly 300,000 years ago, equipped "
            "with the largest and most complex brain in the history of life. "
            "Controlled fire extends the day, cooks food to release more calories, "
            "and creates a social hearth. Complex language allows abstract thought, "
            "planning, and the transmission of knowledge across generations. Cave "
            "paintings, carved figurines, and burial rituals appear, signaling a "
            "mind capable of art, symbolism, and the contemplation of death.",
        .aftermath =
            "Language and fire create culture, a new form of inheritance that "
            "evolves faster than genes and will reshape the entire planet.",
        .sound_hint = "crackling_fire"
    },
    { /* 19 */
        .index = 19,
        .title = "The Anthropocene",
        .ma = 0.0001,
        .drama = ES_DRAMA_CATASTROPHIC,
        .visual =
            "City lights cover entire continents, visible from orbit. The atmosphere "
            "shimmers with a faint haze. Glaciers retreat, forests shrink, and the "
            "night side of Earth glows like a circuit board.",
        .narrative =
            "In a geological instant, one species transforms the planet more "
            "profoundly than any event since the Great Oxidation. Humans clear "
            "half the world's forests, redirect rivers, acidify oceans, and alter "
            "the composition of the atmosphere itself. The sixth mass extinction "
            "accelerates as species vanish a thousand times faster than the "
            "background rate. For the first time in 4.5 billion years, a single "
            "species holds the power to consciously choose the planet's future.",
        .aftermath =
            "The story is unfinished. What happens next depends on choices being "
            "made right now, in this geological instant.",
        .sound_hint = "city_hum"
    }
};

/* --- Invalid moment sentinel --- */

static es_moment_t es_invalid(void) {
    es_moment_t m;
    m.index = -1;
    m.title = NULL;
    m.ma = 0.0;
    m.drama = ES_DRAMA_QUIET;
    m.visual = NULL;
    m.narrative = NULL;
    m.aftermath = NULL;
    m.sound_hint = NULL;
    return m;
}

/* --- Public API --- */

es_moment_t es_moment_get(int index)
{
    if (index < 0 || index >= ES_MOMENT_COUNT) {
        return es_invalid();
    }
    return MOMENTS[index];
}

int es_moment_count(void)
{
    return ES_MOMENT_COUNT;
}

es_moment_t es_moment_nearest(double ma)
{
    int best = 0;
    double best_dist = fabs(ma - MOMENTS[0].ma);

    for (int i = 1; i < ES_MOMENT_COUNT; i++) {
        double dist = fabs(ma - MOMENTS[i].ma);
        if (dist < best_dist) {
            best_dist = dist;
            best = i;
        }
    }
    return MOMENTS[best];
}

int es_moments_by_drama(es_drama_t min_drama, int *out_indices, int out_max)
{
    if (out_indices == NULL || out_max <= 0) {
        return 0;
    }
    int count = 0;
    for (int i = 0; i < ES_MOMENT_COUNT && count < out_max; i++) {
        if ((int)MOMENTS[i].drama >= (int)min_drama) {
            out_indices[count] = i;
            count++;
        }
    }
    return count;
}

const char *es_moment_title(int index)
{
    if (index < 0 || index >= ES_MOMENT_COUNT) {
        return "?";
    }
    return MOMENTS[index].title;
}

const char *es_drama_name(es_drama_t drama)
{
    switch (drama) {
        case ES_DRAMA_QUIET:       return "Quiet";
        case ES_DRAMA_NOTABLE:     return "Notable";
        case ES_DRAMA_DRAMATIC:    return "Dramatic";
        case ES_DRAMA_CATASTROPHIC: return "Catastrophic";
        case ES_DRAMA_APOCALYPTIC: return "Apocalyptic";
        default:                   return "?";
    }
}

int es_moment_format(int index, char *buf, int buf_size)
{
    if (buf == NULL || buf_size <= 0 || index < 0 || index >= ES_MOMENT_COUNT) {
        return 0;
    }
    const es_moment_t *m = &MOMENTS[index];
    int written = snprintf(buf, (size_t)buf_size,
        "Moment %d: %s\n"
        "Time: %.4f Ma | Drama: %s\n"
        "Sound: %s\n"
        "\nVisual: %s\n"
        "\n%s\n"
        "\nAftermath: %s\n",
        m->index, m->title,
        m->ma, es_drama_name(m->drama),
        m->sound_hint,
        m->visual,
        m->narrative,
        m->aftermath);

    if (written < 0) {
        buf[0] = '\0';
        return 0;
    }
    if (written >= buf_size) {
        buf[buf_size - 1] = '\0';
        return buf_size - 1;
    }
    return written;
}
