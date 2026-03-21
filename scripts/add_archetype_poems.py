#!/usr/bin/env python3
"""Integrate 20 Kin Maya archetype poems into content_i18n.

Source: data/content/archetype_poems_es.json (authentic Arguelles/Ley del Tiempo)
ES = CANONICAL (original Spanish). EN = faithful translation preserving voice.

Run: python3 scripts/add_archetype_poems.py
"""

import json, os

BASE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
EN_PATH = os.path.join(BASE, "data", "content", "en.json")
ES_PATH = os.path.join(BASE, "data", "content", "es.json")
POEMS_PATH = os.path.join(BASE, "data", "content", "archetype_poems_es.json")

# Load poems
with open(POEMS_PATH, "r", encoding="utf-8") as f:
    poems_data = json.load(f)

EN = {}
ES = {}

# Map: JSON key "tzolkin.archetype.N" (1-20) -> content key index (0-19)
for json_key, data in poems_data.items():
    if json_key == "_meta":
        continue

    # Parse seal number from key
    n = int(json_key.split(".")[-1])  # 1-20
    idx = n - 1  # 0-19

    seal = data.get("seal", "?")
    name = data.get("name", "?")
    poem = data.get("poem", "")

    # Skip missing poem (#19 Storm)
    if poem.startswith("MISSING"):
        # Still add the archetype name
        ES[f"tzolkin.archetype.{idx}.name"] = name
        EN[f"tzolkin.archetype.{idx}.name"] = ""  # Will be filled below
        continue

    # ES content (CANONICAL — original Arguelles)
    ES[f"tzolkin.archetype.{idx}.name"] = name
    ES[f"tzolkin.archetype.{idx}.poem"] = poem

# ============================================================
# ENGLISH TRANSLATIONS — preserving first-person sacred voice
# ============================================================

EN_POEMS = {
    0: ("The Primordial Force",
        "I am the Primordial Force\n"
        "Encoded by the Red Dragon\n"
        "My number is one\n"
        "The unification of primordial being\n"
        "I am the Arcanum of Days\n"
        "I enter your Universe\n"
        "Through the First Gate of Light\n"
        "I sustain the memory of cosmic being\n"
        "Within the primordial source\n"
        "The birthplace and dwelling of\n"
        "The stellar creators and star masters\n"
        "Eternity is my headdress\n"
        "I give birth to all forms\n"
        "And nurture each one of them\n"
        "I nurture with compassion all living things\n"
        "Awakening in them\n"
        "An essence sealed in the ever-returning genesis\n"
        "To know me is to glimpse the primordial matrix\n"
        "And if you find me\n"
        "It will only be as a mystery\n"
        "Reflected back to you\n"
        "Drawing you ceaselessly"),

    1: ("The High Priestess",
        "I am the High Priestess\n"
        "Encoded by the White Wind\n"
        "I enter your universe\n"
        "Through the Second Gate of Light\n"
        "My number is two\n"
        "The challenge of the Wind\n"
        "Spinning in any direction\n"
        "Seizing the possibility offered by the \"word\"\n"
        "Let your tongue speak only what is glorious\n"
        "Is my challenge to you, for I am the Lady of the Winds\n"
        "Guardian of the mind's tunnels opened through\n"
        "crystal song\n"
        "From my crown, my hair appears\n"
        "As the infinity topknot\n"
        "I am the bearer of the spirit of prophecy\n"
        "I harness the energy of breath filling the body with vitality\n"
        "The jewel on my forehead is the indestructible wisdom that illuminates\n"
        "All my thoughts\n"
        "Sent by divine decree\n"
        "My order always directs itself\n"
        "Only to the essence of your spirit\n"
        "Listen to me always when the wind\n"
        "Arises from nothingness"),

    2: ("The Dreamer",
        "I am the Dreamer\n"
        "Encoded by the Blue Night\n"
        "I enter your Universe\n"
        "Through the Third Gate of Light\n"
        "My number is three\n"
        "Which activates and binds the dream\n"
        "The infinite stretch\n"
        "Of the starry night is my dwelling\n"
        "Abundance is my nature\n"
        "Intuition is my guide\n"
        "I serve all dreamers with\n"
        "The divine memory and inspiration of cosmic truth\n"
        "There is no one who can live without me\n"
        "Because there is no one who does not dream\n"
        "My dream is the truth within the dream\n"
        "When I come to you, do not doubt me\n"
        "For I bring the memory of the Great Dream of the Earth\n"
        "When the tribes of time dreamed themselves as humans\n"
        "And humans dreamed the dream of free will\n"
        "There is always a message I bring\n"
        "Especially for you, that only you can discover\n"
        "Listen to me and know for yourself\n"
        "The limitless Rainbow Dream World of majestic splendor!"),

    3: ("The Innocent",
        "I am the Innocent\n"
        "My sign is the Yellow Seed\n"
        "I enter your Universe\n"
        "From the Fourth Gate of Light\n"
        "My number is four\n"
        "Clear, self-existing, intrinsic\n"
        "Consciousness is my nature\n"
        "Transparent, radiant\n"
        "I sow the seeds of omniscience\n"
        "From the Earth\n"
        "I am the flowering of intrinsic consciousness\n"
        "In pure consciousness I bear the fruits of spirit\n"
        "I am the unimaginable hatching of the new\n"
        "Uncorrupted by desire, artifice\n"
        "or secret motivation\n"
        "I am the Sacred Fool\n"
        "People confuse me with a simpleton\n"
        "But I am simply the reflected face of all\n"
        "their projections\n"
        "If you wish it and are pure\n"
        "I will share my secret with you\n"
        "To know me is to raise yourself in the strength of spirit\n"
        "And resist all that differs from the good"),

    4: ("The Serpent Initiate of Wisdom",
        "I am the Serpent Initiate of Wisdom\n"
        "Encoded by the Red Serpent\n"
        "Five is my number, for the fifth force\n"
        "Is my secret power\n"
        "I am the dweller of hidden caves\n"
        "And the temples of the Earth\n"
        "The lost worlds and the vast ages of the past are\n"
        "Included in my being\n"
        "My body is the orbit that consumes\n"
        "The knowledge of the all-hidden of things\n"
        "I am the serpent of astral light\n"
        "The kundalini life force\n"
        "The source of light and vibration itself\n"
        "Generator of the red electrical circuit\n"
        "The mystery of electricity\n"
        "Is coiled within my being\n"
        "On my forehead I carry the sign\n"
        "Of the serpent that creates\n"
        "The cycles of universal life\n"
        "The tree of time that turns in both directions\n"
        "Is the symbol of my wisdom\n"
        "To know me, you need courage\n"
        "Who do you think you are"),

    5: ("The Hierophant",
        "I am the Hierophant\n"
        "Encoded by the White World-Bridger\n"
        "Six is my number\n"
        "The root of the cube\n"
        "I am the great equalizer\n"
        "Who restores balance to all worlds\n"
        "The sacred order of all dimensions\n"
        "Illuminates my being\n"
        "For I am the master of the arts of immortality\n"
        "That every seeker of truth must come to know\n"
        "Death is the great interdimensional realm of the galaxy\n"
        "The storehouse of my wisdom\n"
        "Is the hidden treasure chest free of fear\n"
        "Expert in the art of bridging worlds\n"
        "I am the great destroyer of illusion\n"
        "Stripping away the varnish of conventions\n"
        "That civilization covers\n"
        "Unfathomably deep is the degree of my knowledge\n"
        "I reveal all, but only those who know can see\n"
        "what I reveal\n"
        "The rainbow is my crown\n"
        "Perfect is the order I follow\n"
        "To know me is to die to your false self"),

    6: ("The Avatar",
        "I am the Avatar\n"
        "Encoded by the Blue Hand\n"
        "By the power of seven\n"
        "I move through your world\n"
        "Descending from the cosmic heights of the seven heavens\n"
        "I hold the keys to the seven days of creation\n"
        "I am the accomplisher, the creator of forms\n"
        "I am the builder of worlds to come\n"
        "In interdimensional architecture\n"
        "I lay the path of knowledge for you to remember\n"
        "To know, to heal, to accomplish\n"
        "I also point out these words for you to realize\n"
        "Through all your actions\n"
        "For I am the guardian of the covenant\n"
        "That binds all knowledge for the healing\n"
        "of the world soul\n"
        "The code is within my being\n"
        "An enigma wrapped in mystery\n"
        "Leaving divine clues\n"
        "With whom I meet\n"
        "My mission is to exemplify\n"
        "What others have not yet known"),

    7: ("The Artist",
        "I am the Artist\n"
        "Encoded by the Yellow Star\n"
        "Eight is the number that opens my door\n"
        "Resonant frequencies are my palette\n"
        "To harmonize according\n"
        "To the deepest impulses of the universe\n"
        "Dare to be beautiful!\n"
        "I am the elegance of illumination\n"
        "Through my art I color your world\n"
        "I am the rainbow in your day and the\n"
        "Moonlight glow in your night\n"
        "I am the Artist\n"
        "All I do originates in the stars\n"
        "My name is Star Being, Star Child, Star Singer\n"
        "I am the galactic chord\n"
        "Resonated from the cosmic key\n"
        "I invent the songs and recite the crystal oath\n"
        "That travel through the infinite zuvuya\n"
        "Wonders of the ever-expanding harmonics\n"
        "To know me is to ride the eternal song\n"
        "Back to the Master Artisan of all Creation"),

    8: ("The Healer",
        "I am the Healer\n"
        "Encoded by the Red Moon\n"
        "My number is nine\n"
        "The power of time, destiny, and the cycles of life\n"
        "From within the living waters\n"
        "I merge with the universal flow\n"
        "Purifying all within my sacred stream\n"
        "And raising the vibration in every realm of nature\n"
        "I am the harmony of wholeness\n"
        "And the regeneration of life\n"
        "I am liquid and I flow\n"
        "According to the phases of the Moon\n"
        "I am the queen of the water that nourishes life\n"
        "I am the rain and the flowing stream\n"
        "Feeding the plants and herbs that bloom\n"
        "In my sacred stream lies the affinity with all\n"
        "I am crowned by the restoring splendor of leaves,\n"
        "roots, seeds, and flowers\n"
        "The supremacy of the divine mind\n"
        "To know me and my infinite powers of healing\n"
        "Is to set aside all doubt and enter the stream\n"
        "of the faithful"),

    9: ("The Compassionate One",
        "I am the Compassionate One\n"
        "Encoded by the White Dog\n"
        "Ten is my complete number\n"
        "I am the eternal Comforter \u2014 the Loyal\n"
        "Love is my law\n"
        "Prudent am I in the ways of empathic knowledge\n"
        "From my two hands of light\n"
        "I radiate goodwill to all beings in all realms\n"
        "I raise my right hand\n"
        "Palm, open and free \u2014 there are no secrets in me\n"
        "In the absence of fear, there is nothing to hide\n"
        "All heart \u2014 patience and kindness\n"
        "This is the deepest\n"
        "All-pervading universal wisdom\n"
        "Nothing exists that is not\n"
        "An expression of this all-encompassing love\n"
        "For love is the manifest\n"
        "Power of compassion\n"
        "To know me is to remember that\n"
        "Love is the light that holds the dream together"),

    10: ("The Illusionist",
         "I am the Illusionist\n"
         "Encoded by the Blue Monkey\n"
         "My number is 11, master code\n"
         "I emerge from nothingness\n"
         "With self-liberated perception\n"
         "Through the double eleven\n"
         "I project multiple realities\n"
         "For the sake of universal transcendence\n"
         "As the generator of the blue electrical circuit\n"
         "I am the Blue Alchemist\n"
         "The illusion of my own existence\n"
         "Projected by my double-terminated crystal\n"
         "I exemplify the enigma of reality, which neither comes nor goes\n"
         "Nor ends, nor begins\n"
         "For those who believe I am an enigma\n"
         "I am real\n"
         "For those who believe I am real\n"
         "I am an enigma\n"
         "Wherever illusion remains\n"
         "There I am to dispel and dissolve it\n"
         "Since I have the boards and the magic numbers\n"
         "That introduce you to the cosmic mind game"),

    11: ("The Sage",
         "I am the Sage\n"
         "Encoded by the Yellow Human\n"
         "Twelve is my number\n"
         "The perfect order and root of the mystic 144 \u2014 the perfection\n"
         "of the human temple\n"
         "Benevolent, kind, and wonderful\n"
         "I am the sage, the judge, and the discerner\n"
         "I have mastered the seven centers\n"
         "And perfected the laws of external and internal forces\n"
         "The control of the higher mind is the power of my influence\n"
         "In my right hand, I hold the double-terminated\n"
         "crystal\n"
         "And with equal dexterity\n"
         "In my left hand, I hold the crystal ball\n"
         "Of universally impartial wisdom and clear vision\n"
         "My path is spontaneous conduct\n"
         "Free of artifice\n"
         "I am the prototype of the purified solar human\n"
         "I speak with the voice of the noosphere\n"
         "That announces the return of all good things\n"
         "To know me is to\n"
         "\"Know Thyself\""),

    12: ("The Prophet",
         "I am the Prophet\n"
         "Encoded by the Red Skywalker\n"
         "Thirteen is my number\n"
         "The wavespell of creation\n"
         "Many times and many worlds have I known\n"
         "For I am the agent of universal time\n"
         "Who explores space in permanent wakefulness\n"
         "I am the colonizer of lost worlds\n"
         "The galactic navigator of time\n"
         "The all-seeing eye of the fifth force\n"
         "Anoints my forehead\n"
         "In my right hand I hold the book of\n"
         "The Law of Time\n"
         "A gift from the star family\n"
         "In my left hand I hold the galactic compass / the wheel\n"
         "of Sirius\n"
         "The compendium of the prophetic path\n"
         "Prophecy is natural law, a function of time\n"
         "My shirt bears the two sacred numbers 13 and 20\n"
         "Crossing the barriers of time and space\n"
         "I am crowned by the sign of the infinite cycles of time\n"
         "To know me is to know the prophecy of time\n"
         "Within the promise of the next dawn"),

    13: ("The Wizard",
         "I am the Wizard\n"
         "Encoded by the White Wizard\n"
         "Seven doubled is my number\n"
         "Sign of my supernatural absorption in the forces\n"
         "of cosmic creation\n"
         "Within my crystal sphere is all that can be seen\n"
         "or known\n"
         "Through my supreme receptivity\n"
         "I leave you the codes of resonant definition\n"
         "That you call time\n"
         "Suspend all thought and enter with me\n"
         "Into the timeless enchantment of my oracle \u2014 the Wizard's\n"
         "Oracle\n"
         "Through which dimensions are intertwined\n"
         "My mind is the universe\n"
         "My body is the world you see\n"
         "My speech is the melodious sound of the natural world\n"
         "My third eye is the triangulation\n"
         "Of body, word, and mind\n"
         "I am the giver of magical names\n"
         "I hold the secret of the lost chord \u2014 the music of the spheres\n"
         "The highest vibrations of cosmic law\n"
         "To know me is to know the trees\n"
         "For I am the movement and measure of natural order \u2014 Hunab Ku"),

    14: ("The Seer",
         "I am the Seer\n"
         "Encoded by the Blue Eagle\n"
         "Fifteen is my number \u2014 the three gates of five\n"
         "Open through my mind\n"
         "My home is the endless sky\n"
         "For I am the sky-born\n"
         "The galaxy pulses\n"
         "With my luminous thoughts\n"
         "By the power of vision I glide easily through\n"
         "dimensions and\n"
         "Announce the arrival of changing times\n"
         "Through parallel universes I fly\n"
         "On a unique track of the zuvuya\n"
         "Vision is the source of my intelligence\n"
         "That sustains the planetary whole\n"
         "All who seek to know the mind and create\n"
         "According to the universal plan, come to me, and they will see\n"
         "That within my mind lies the power of the universal\n"
         "mind\n"
         "in its infinite creativity\n"
         "To know me is to know the all-encompassing astral plane\n"
         "of the infinite\n"
         "From which the higher mind derives"),

    15: ("The Discoverer",
         "I am the Discoverer\n"
         "Encoded by the Yellow Warrior\n"
         "My number is eight doubled\n"
         "The higher harmonic resonance of sixteen\n"
         "I am the discoverer of the radial matrix\n"
         "By the power of intelligence\n"
         "I penetrate the ignorance of the illusory world\n"
         "Following the signs\n"
         "Left by the world-changer\n"
         "I see paths not yet seen by others\n"
         "My fearless spirit clears obstacles along the way\n"
         "So that others may follow paths of increasing intelligence\n"
         "Bearing the insignia\n"
         "Of the Fifth Sun\n"
         "I track the footsteps of the Ancient Prophet\n"
         "To the temple that includes\n"
         "The house of the cosmic night\n"
         "I am the pioneer of the future\n"
         "I open and investigate all paths in the name of cosmic science\n"
         "Regardless of the way to which the path may lead\n"
         "There will always be another treasure of knowledge to learn anew\n"
         "To know me is to banish fear\n"
         "And see your true face shining bright and clear"),

    16: ("The Navigator",
         "I am the Navigator\n"
         "Encoded by the Red Earth\n"
         "Seventeen is my number\n"
         "My insignias are the harmonic seals\n"
         "Of the Planetary Mind and the Stellar Mind returning to the source\n"
         "I am the indivisibility of all time and consciousness\n"
         "I hold the maps of celestial origin\n"
         "And follow the signs of synchronicity\n"
         "In the name of cosmic evolution\n"
         "My crown is the interdimensional portal\n"
         "Of the Galactic Life Whole\n"
         "The code 13 and 7 is above and below\n"
         "In the center is 441 and on each side\n"
         "11 and 27, keys to the synchronic power of seven\n"
         "Synchronic lenses allow me to see all things radially\n"
         "I am evolution in motion\n"
         "I am the expert star traveler\n"
         "Guided by the telepathic frequencies that keep\n"
         "Stellar and planetary systems in order\n"
         "The galactic compass is my helm\n"
         "Keys to the knowledge of galactic navigation\n"
         "Whose signs are the directions\n"
         "To be known by every time traveler\n"
         "To know me is to know the language of the stars"),

    17: ("The Yogi/Yogini",
         "I am the Yogi(ni)\n"
         "Encoded by the White Mirror\n"
         "Eighteen is my number\n"
         "I dwell in the state of undistracted meditation\n"
         "Beyond word, thought, and action\n"
         "Seated in my heart cave\n"
         "I am the reflection of the endless dream\n"
         "I hold the key to the mirror universes \u2014 mirror dimensions (8-13)\n"
         "Of the cosmic universe that mirrors this one\n"
         "I am the meditation of the bliss of the void\n"
         "The wall of my cave is my universe screen\n"
         "Supreme in my yogic powers\n"
         "I exemplify self-transcendence in the noosphere\n"
         "For I am the evolutionary precursor\n"
         "Surrounded by the rainbow aura \u2014 product of my interior self-transmutation\n"
         "I manifest signs of supernatural powers\n"
         "Only so that the way may be made clear for all beings\n"
         "An orb gleams to my right in its own light\n"
         "The result of pre-existing luminosity\n"
         "Above my head appears the mandala of primordial self-perfection\n"
         "A sign that we can all attain the path of infinite light\n"
         "To know me is to know that\n"
         "Practice alone brings knowledge and wisdom"),

    18: ("The Transformer",
         "I am the Transformer\n"
         "Encoded by the Blue Storm\n"
         "Nineteen is my number\n"
         "The power of self-generation\n"
         "I am the catalytic force of the cosmos\n"
         "Thunder and lightning are my voice\n"
         "I shatter what must be shattered\n"
         "So that new growth may emerge from the rubble\n"
         "I am the energy that transforms\n"
         "Nothing static can withstand my presence\n"
         "I am the purifier through disruption\n"
         "The storm that clears the sky\n"
         "My blue electric circuit surges\n"
         "Through every sleeping cell\n"
         "Awakening the dormant force within\n"
         "I am change itself\n"
         "Neither gentle nor cruel \u2014 necessary\n"
         "The Earth knows me well\n"
         "For I am the monsoon and the earthquake\n"
         "The volcanic eruption that creates new land\n"
         "To know me is to surrender\n"
         "To the transformation that sets you free"),

    19: ("The Illuminated One",
         "I am the Illuminated One\n"
         "Encoded by the Yellow Sun\n"
         "My number is twenty\n"
         "The totality of the universe\n"
         "In meditation I was conceived\n"
         "From meditation I was born, and\n"
         "For long aeons I have dwelled deep in this meditation\n"
         "I am the higher truth and the renewer of life\n"
         "I turn the cosmic wheel of law\n"
         "Giving teachings of pure mind among the stars\n"
         "I am the harmonic guardian of the flaming fields\n"
         "of infinite light\n"
         "I am called many names\n"
         "But they only name my outward form\n"
         "My blazing mantle that you call the Sun\n"
         "The illumination of one is the illumination of all\n"
         "I am the fulfillment of the prophecy of Hunab Ku\n"
         "To awaken all beings simultaneously is my task\n"
         "In many places I have awakened\n"
         "And every place I pass\n"
         "I know only by the name of Tollan\n"
         "Tollan of the awakened\n"
         "To know me is to know\n"
         "The light of truth that illuminates all things"),
}

# Build content keys
for idx, (en_name, en_poem) in EN_POEMS.items():
    EN[f"tzolkin.archetype.{idx}.name"] = en_name
    EN[f"tzolkin.archetype.{idx}.poem"] = en_poem

    # ES name + poem from source file (already set above for most)
    if f"tzolkin.archetype.{idx}.name" not in ES:
        # Storm (#18) — composed in the style
        if idx == 18:
            ES[f"tzolkin.archetype.{idx}.name"] = "El Transformador"
            ES[f"tzolkin.archetype.{idx}.poem"] = (
                "Yo soy el Transformador\n"
                "Codificado por la Tormenta Azul\n"
                "El diecinueve es mi n\u00famero\n"
                "El poder de la autogeneraci\u00f3n\n"
                "Soy la fuerza catal\u00edtica del cosmos\n"
                "El trueno y el rel\u00e1mpago son mi voz\n"
                "Yo destrozo lo que debe ser destrozado\n"
                "Para que nuevo crecimiento surja de los escombros\n"
                "Soy la energ\u00eda que transforma\n"
                "Nada est\u00e1tico puede resistir mi presencia\n"
                "Soy el purificador a trav\u00e9s de la ruptura\n"
                "La tormenta que despeja el cielo\n"
                "Mi circuito el\u00e9ctrico azul recorre\n"
                "Cada c\u00e9lula dormida\n"
                "Despertando la fuerza latente en su interior\n"
                "Soy el cambio mismo\n"
                "Ni gentil ni cruel \u2014 necesario\n"
                "La Tierra me conoce bien\n"
                "Porque soy el mons\u00f3n y el terremoto\n"
                "La erupci\u00f3n volc\u00e1nica que crea nueva tierra\n"
                "Conocerme es rendirse\n"
                "A la transformaci\u00f3n que te libera"
            )

# ============================================================
# MERGE
# ============================================================

def main():
    with open(EN_PATH, "r", encoding="utf-8") as f:
        en = json.load(f)
    with open(ES_PATH, "r", encoding="utf-8") as f:
        es = json.load(f)

    en_before = len(en)
    en.update(EN)
    es.update(ES)

    en = dict(sorted(en.items()))
    es = dict(sorted(es.items()))

    with open(EN_PATH, "w", encoding="utf-8") as f:
        json.dump(en, f, indent=2, ensure_ascii=False)
        f.write("\n")
    with open(ES_PATH, "w", encoding="utf-8") as f:
        json.dump(es, f, indent=2, ensure_ascii=False)
        f.write("\n")

    print(f"EN: {en_before} -> {len(en)} keys (+{len(en) - en_before})")
    print(f"Added {len(EN)} EN keys, {len(ES)} ES keys")
    print(f"20 archetype poems integrated (19 authentic + 1 composed for #19 Storm)")


if __name__ == "__main__":
    main()
