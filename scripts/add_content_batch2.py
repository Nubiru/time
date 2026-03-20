#!/usr/bin/env python3
"""Add Tzolkin, Chakra, Numerology, Geology content to en.json and es.json.

Run: python3 scripts/add_content_batch2.py
"""

import json
import os

BASE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
EN_PATH = os.path.join(BASE, "data", "content", "en.json")
ES_PATH = os.path.join(BASE, "data", "content", "es.json")


# ============================================================
# TZOLKIN — Dreamspell (Arguelles) content
# ============================================================

TZ_EN = {
    "tzolkin.tpl.glyph": "KIN %1$d",
    "tzolkin.tpl.glance": "Kin %1$d %2$s %3$s %4$s \xe2\x80\x94 %5$s %6$s",
    "tzolkin.tpl.detail": "%1$s %2$s %3$s: %4$s. Tone %5$d: %6$s \xe2\x80\x94 %7$s the power of %8$s. Seal: %9$s \xe2\x80\x94 %10$s Wavespell %11$d: %12$s wavespell \xe2\x80\x94 %13$s, %14$s. Castle: %15$s \xe2\x80\x94 %16$s. Oracle guide: Kin %17$d. Analog: Kin %18$d. Antipode: Kin %19$d. Occult: Kin %20$d.",
}

TZ_ES = {
    "tzolkin.tpl.glyph": "KIN %1$d",
    "tzolkin.tpl.glance": "Kin %1$d %2$s %3$s %4$s \xe2\x80\x94 %5$s %6$s",
    "tzolkin.tpl.detail": "%1$s %2$s %3$s: %4$s. Tono %5$d: %6$s \xe2\x80\x94 %7$s el poder de %8$s. Sello: %9$s \xe2\x80\x94 %10$s Onda Encantada %11$d: Onda de %12$s \xe2\x80\x94 %13$s, %14$s. Castillo: %15$s \xe2\x80\x94 %16$s. Gu\u00eda or\u00e1culo: Kin %17$d. An\u00e1logo: Kin %18$d. Ant\u00edpoda: Kin %19$d. Oculto: Kin %20$d.",
}

# Seal names (20) and archetypes
SEALS = [
    # (name_en, archetype_en, question_en, power_en, action_en, essence_en,
    #  name_es, archetype_es, question_es, power_es, action_es, essence_es)
    ("Dragon", "Primal nurturance, the mother of creation",
     "What do I need to nurture into being?", "Birth", "Nurtures", "Being",
     "Drag\u00f3n", "Nutrici\u00f3n primordial, la madre de la creaci\u00f3n",
     "\u00bfQu\u00e9 necesito nutrir para que nazca?", "Nacimiento", "Nutre", "Ser"),
    ("Wind", "Divine breath, the messenger of spirit",
     "What truth must I communicate?", "Spirit", "Communicates", "Breath",
     "Viento", "Aliento divino, el mensajero del esp\u00edritu",
     "\u00bfQu\u00e9 verdad debo comunicar?", "Esp\u00edritu", "Comunica", "Aliento"),
    ("Night", "The dreamer, keeper of the void and abundance",
     "What dreams do I hold in the darkness?", "Abundance", "Dreams", "Intuition",
     "Noche", "El so\u00f1ador, guardi\u00e1n del vac\u00edo y la abundancia",
     "\u00bfQu\u00e9 sue\u00f1os guardo en la oscuridad?", "Abundancia", "Sue\u00f1a", "Intuici\u00f3n"),
    ("Seed", "Flowering awareness, the potential of new life",
     "What am I ready to plant?", "Flowering", "Targets", "Awareness",
     "Semilla", "Conciencia floreciente, el potencial de nueva vida",
     "\u00bfQu\u00e9 estoy listo para plantar?", "Florecimiento", "Apunta", "Conciencia"),
    ("Serpent", "Life force, kundalini, the body's wisdom",
     "What instinct am I ignoring?", "Life Force", "Survives", "Instinct",
     "Serpiente", "Fuerza vital, kundalini, la sabidur\u00eda del cuerpo",
     "\u00bfQu\u00e9 instinto estoy ignorando?", "Fuerza Vital", "Sobrevive", "Instinto"),
    ("World-Bridger", "Death and transformation, the bridge between worlds",
     "What must I release to cross over?", "Death", "Equalizes", "Opportunity",
     "Enlazador de Mundos", "Muerte y transformaci\u00f3n, el puente entre mundos",
     "\u00bfQu\u00e9 debo soltar para cruzar?", "Muerte", "Iguala", "Oportunidad"),
    ("Hand", "Healing, accomplishment, knowledge through doing",
     "What can I heal through action?", "Accomplishment", "Knows", "Healing",
     "Mano", "Sanaci\u00f3n, realizaci\u00f3n, conocimiento a trav\u00e9s de la acci\u00f3n",
     "\u00bfQu\u00e9 puedo sanar a trav\u00e9s de la acci\u00f3n?", "Realizaci\u00f3n", "Conoce", "Sanaci\u00f3n"),
    ("Star", "Beauty, art, elegance, the aesthetic principle",
     "Where must I bring beauty?", "Art", "Beautifies", "Elegance",
     "Estrella", "Belleza, arte, elegancia, el principio est\u00e9tico",
     "\u00bfD\u00f3nde debo llevar belleza?", "Arte", "Embellece", "Elegancia"),
    ("Moon", "Purification, flow, the universal water of feeling",
     "What feelings need to flow?", "Universal Water", "Purifies", "Flow",
     "Luna", "Purificaci\u00f3n, flujo, el agua universal del sentimiento",
     "\u00bfQu\u00e9 sentimientos necesitan fluir?", "Agua Universal", "Purifica", "Flujo"),
    ("Dog", "Heart, love, loyalty, unconditional devotion",
     "Who do I love without condition?", "Heart", "Loves", "Loyalty",
     "Perro", "Coraz\u00f3n, amor, lealtad, devoci\u00f3n incondicional",
     "\u00bfA qui\u00e9n amo sin condici\u00f3n?", "Coraz\u00f3n", "Ama", "Lealtad"),
    ("Monkey", "Magic, play, illusion, the divine trickster",
     "Where am I too serious?", "Magic", "Plays", "Illusion",
     "Mono", "Magia, juego, ilusi\u00f3n, el tramposo divino",
     "\u00bfD\u00f3nde me tomo demasiado en serio?", "Magia", "Juega", "Ilusi\u00f3n"),
    ("Human", "Free will, wisdom, the power of choice",
     "What choices define my path?", "Free Will", "Influences", "Wisdom",
     "Humano", "Libre albedr\u00edo, sabidur\u00eda, el poder de la elecci\u00f3n",
     "\u00bfQu\u00e9 elecciones definen mi camino?", "Libre Albedr\u00edo", "Influencia", "Sabidur\u00eda"),
    ("Skywalker", "Space, exploration, wakefulness between worlds",
     "What new territory calls me?", "Space", "Explores", "Wakefulness",
     "Caminante del Cielo", "Espacio, exploraci\u00f3n, vigilia entre mundos",
     "\u00bfQu\u00e9 nuevo territorio me llama?", "Espacio", "Explora", "Vigilia"),
    ("Wizard", "Timelessness, enchantment, receptive magic",
     "What appears when I stop forcing?", "Timelessness", "Enchants", "Receptivity",
     "Mago", "Atemporalidad, encantamiento, magia receptiva",
     "\u00bfQu\u00e9 aparece cuando dejo de forzar?", "Atemporalidad", "Encanta", "Receptividad"),
    ("Eagle", "Vision, the mind's eye, seeing the whole pattern",
     "What can I see from above?", "Vision", "Creates", "Mind",
     "\u00c1guila", "Visi\u00f3n, el ojo de la mente, ver el patr\u00f3n completo",
     "\u00bfQu\u00e9 puedo ver desde arriba?", "Visi\u00f3n", "Crea", "Mente"),
    ("Warrior", "Intelligence, fearlessness, questioning everything",
     "What question am I afraid to ask?", "Intelligence", "Questions", "Fearlessness",
     "Guerrero", "Inteligencia, intrepidez, cuestionarlo todo",
     "\u00bfQu\u00e9 pregunta temo hacer?", "Inteligencia", "Cuestiona", "Intrepidez"),
    ("Earth", "Navigation, synchronicity, the Earth's heartbeat",
     "What does the Earth want from me?", "Navigation", "Evolves", "Synchronicity",
     "Tierra", "Navegaci\u00f3n, sincronicidad, el latido de la Tierra",
     "\u00bfQu\u00e9 quiere la Tierra de m\u00ed?", "Navegaci\u00f3n", "Evoluciona", "Sincronicidad"),
    ("Mirror", "Endlessness, reflection, the hall of mirrors",
     "What am I reflecting back to others?", "Endlessness", "Reflects", "Order",
     "Espejo", "Infinitud, reflexi\u00f3n, la sala de los espejos",
     "\u00bfQu\u00e9 estoy reflejando a los dem\u00e1s?", "Infinitud", "Refleja", "Orden"),
    ("Storm", "Self-generation, transformation, catalytic energy",
     "What transformation am I resisting?", "Self-Generation", "Catalyzes", "Energy",
     "Tormenta", "Autogeneraci\u00f3n, transformaci\u00f3n, energ\u00eda catal\u00edtica",
     "\u00bfQu\u00e9 transformaci\u00f3n estoy resistiendo?", "Autogeneraci\u00f3n", "Cataliza", "Energ\u00eda"),
    ("Sun", "Universal fire, enlightenment, wholeness, completion",
     "What must I illuminate?", "Universal Fire", "Enlightens", "Life",
     "Sol", "Fuego universal, iluminaci\u00f3n, totalidad, consumaci\u00f3n",
     "\u00bfQu\u00e9 debo iluminar?", "Fuego Universal", "Ilumina", "Vida"),
]

for i, s in enumerate(SEALS):
    TZ_EN[f"tzolkin.seal.{i}.name"] = s[0]
    TZ_EN[f"tzolkin.seal.{i}.archetype"] = s[1]
    TZ_EN[f"tzolkin.seal.{i}.question"] = s[2]
    TZ_EN[f"tzolkin.seal.{i}.power"] = s[3]
    TZ_EN[f"tzolkin.seal.{i}.action"] = s[4]
    TZ_EN[f"tzolkin.seal.{i}.essence"] = s[5]
    TZ_ES[f"tzolkin.seal.{i}.name"] = s[6]
    TZ_ES[f"tzolkin.seal.{i}.archetype"] = s[7]
    TZ_ES[f"tzolkin.seal.{i}.question"] = s[8]
    TZ_ES[f"tzolkin.seal.{i}.power"] = s[9]
    TZ_ES[f"tzolkin.seal.{i}.action"] = s[10]
    TZ_ES[f"tzolkin.seal.{i}.essence"] = s[11]

# Colors (4)
COLORS = [
    ("Red", "Initiates", "Birth", "Initiates \xe2\x80\x94 the power of birth",
     "Rojo", "Inicia", "Nacimiento", "Inicia \xe2\x80\x94 el poder del nacimiento"),
    ("White", "Refines", "Death", "Refines \xe2\x80\x94 the power of death",
     "Blanco", "Refina", "Muerte", "Refina \xe2\x80\x94 el poder de la muerte"),
    ("Blue", "Transforms", "Magic", "Transforms \xe2\x80\x94 the power of magic",
     "Azul", "Transforma", "Magia", "Transforma \xe2\x80\x94 el poder de la magia"),
    ("Yellow", "Ripens", "Intelligence", "Ripens \xe2\x80\x94 the power of intelligence",
     "Amarillo", "Madura", "Inteligencia", "Madura \xe2\x80\x94 el poder de la inteligencia"),
]

for i, c in enumerate(COLORS):
    TZ_EN[f"tzolkin.color.{i}.name"] = c[0]
    TZ_EN[f"tzolkin.color.{i}.action"] = c[1]
    TZ_EN[f"tzolkin.color.{i}.quality"] = c[2]
    TZ_EN[f"tzolkin.color.{i}.meaning"] = c[3]
    TZ_ES[f"tzolkin.color.{i}.name"] = c[4]
    TZ_ES[f"tzolkin.color.{i}.action"] = c[5]
    TZ_ES[f"tzolkin.color.{i}.quality"] = c[6]
    TZ_ES[f"tzolkin.color.{i}.meaning"] = c[7]

# Tones (13)
TONES = [
    ("Magnetic", "Unify", "Purpose", "Magnetic \xe2\x80\x94 Unify Purpose",
     "Magn\u00e9tico", "Unificar", "Prop\u00f3sito", "Magn\u00e9tico \xe2\x80\x94 Unificar Prop\u00f3sito"),
    ("Lunar", "Polarize", "Challenge", "Lunar \xe2\x80\x94 Polarize Challenge",
     "Lunar", "Polarizar", "Desaf\u00edo", "Lunar \xe2\x80\x94 Polarizar Desaf\u00edo"),
    ("Electric", "Activate", "Service", "Electric \xe2\x80\x94 Activate Service",
     "El\u00e9ctrico", "Activar", "Servicio", "El\u00e9ctrico \xe2\x80\x94 Activar Servicio"),
    ("Self-Existing", "Define", "Form", "Self-Existing \xe2\x80\x94 Define Form",
     "Auto-Existente", "Definir", "Forma", "Auto-Existente \xe2\x80\x94 Definir Forma"),
    ("Overtone", "Empower", "Radiance", "Overtone \xe2\x80\x94 Empower Radiance",
     "Entonado", "Empoderar", "Resplandor", "Entonado \xe2\x80\x94 Empoderar Resplandor"),
    ("Rhythmic", "Organize", "Equality", "Rhythmic \xe2\x80\x94 Organize Equality",
     "R\u00edtmico", "Organizar", "Igualdad", "R\u00edtmico \xe2\x80\x94 Organizar Igualdad"),
    ("Resonant", "Channel", "Attunement", "Resonant \xe2\x80\x94 Channel Attunement",
     "Resonante", "Canalizar", "Sinton\u00eda", "Resonante \xe2\x80\x94 Canalizar Sinton\u00eda"),
    ("Galactic", "Harmonize", "Integrity", "Galactic \xe2\x80\x94 Harmonize Integrity",
     "Gal\u00e1ctico", "Armonizar", "Integridad", "Gal\u00e1ctico \xe2\x80\x94 Armonizar Integridad"),
    ("Solar", "Pulse", "Intention", "Solar \xe2\x80\x94 Pulse Intention",
     "Solar", "Pulsar", "Intenci\u00f3n", "Solar \xe2\x80\x94 Pulsar Intenci\u00f3n"),
    ("Planetary", "Perfect", "Manifestation", "Planetary \xe2\x80\x94 Perfect Manifestation",
     "Planetario", "Perfeccionar", "Manifestaci\u00f3n", "Planetario \xe2\x80\x94 Perfeccionar Manifestaci\u00f3n"),
    ("Spectral", "Dissolve", "Liberation", "Spectral \xe2\x80\x94 Dissolve Liberation",
     "Espectral", "Disolver", "Liberaci\u00f3n", "Espectral \xe2\x80\x94 Disolver Liberaci\u00f3n"),
    ("Crystal", "Dedicate", "Cooperation", "Crystal \xe2\x80\x94 Dedicate Cooperation",
     "Cristal", "Dedicar", "Cooperaci\u00f3n", "Cristal \xe2\x80\x94 Dedicar Cooperaci\u00f3n"),
    ("Cosmic", "Endure", "Presence", "Cosmic \xe2\x80\x94 Endure Transcendence",
     "C\u00f3smico", "Perdurar", "Presencia", "C\u00f3smico \xe2\x80\x94 Perdurar Trascendencia"),
]

for i, t in enumerate(TONES):
    n = i + 1
    TZ_EN[f"tzolkin.tone.{n}.name"] = t[0]
    TZ_EN[f"tzolkin.tone.{n}.action"] = t[1]
    TZ_EN[f"tzolkin.tone.{n}.power"] = t[2]
    TZ_EN[f"tzolkin.tone.{n}.meaning"] = t[3]
    TZ_ES[f"tzolkin.tone.{n}.name"] = t[4]
    TZ_ES[f"tzolkin.tone.{n}.action"] = t[5]
    TZ_ES[f"tzolkin.tone.{n}.power"] = t[6]
    TZ_ES[f"tzolkin.tone.{n}.meaning"] = t[7]

# Castles (5)
CASTLES = [
    ("Red Castle of Turning", "Birth",
     "Castillo Rojo del Girar", "Nacimiento"),
    ("White Castle of Crossing", "Death",
     "Castillo Blanco del Cruzar", "Muerte"),
    ("Blue Castle of Burning", "Magic",
     "Castillo Azul del Quemar", "Magia"),
    ("Yellow Castle of Giving", "Intelligence",
     "Castillo Amarillo del Dar", "Inteligencia"),
    ("Green Castle of Enchantment", "Enchantment",
     "Castillo Verde del Encantamiento", "Encantamiento"),
]

for i, c in enumerate(CASTLES):
    n = i + 1
    TZ_EN[f"tzolkin.castle.{n}.name"] = c[0]
    TZ_EN[f"tzolkin.castle.{n}.theme"] = c[1]
    TZ_ES[f"tzolkin.castle.{n}.name"] = c[2]
    TZ_ES[f"tzolkin.castle.{n}.theme"] = c[3]


# ============================================================
# CHAKRA — 7 chakras
# ============================================================

CK_EN = {
    "chakra.tpl.glance": "%1$s (%2$s) \xe2\x80\x94 %3$s",
    "chakra.tpl.detail": "Chakra: %1$s (%2$s). %3$s. Theme: %4$s. Shadow: %5$s. Question: %6$s",
}

CK_ES = {
    "chakra.tpl.glance": "%1$s (%2$s) \xe2\x80\x94 %3$s",
    "chakra.tpl.detail": "Chakra: %1$s (%2$s). %3$s. Tema: %4$s. Sombra: %5$s. Pregunta: %6$s",
}

CHAKRAS = [
    ("Muladhara", "Root", "The Foundation",
     "Grounding, safety, survival, physical vitality, belonging to the earth",
     "Fear, anxiety, instability, hoarding, disconnection from the body",
     "Am I safe? Do I belong here?",
     "The root \xe2\x80\x94 LAM \xe2\x80\x94 the foundation upon which everything else is built",
     "Ra\u00edz", "El Fundamento",
     "Enraizamiento, seguridad, supervivencia, vitalidad f\u00edsica, pertenencia a la tierra",
     "Miedo, ansiedad, inestabilidad, acumulaci\u00f3n, desconexi\u00f3n del cuerpo",
     "\u00bfEstoy seguro? \u00bfPertenezco aqu\u00ed?",
     "La ra\u00edz \xe2\x80\x94 LAM \xe2\x80\x94 el fundamento sobre el cual todo lo dem\u00e1s se construye"),
    ("Svadhisthana", "Sacral", "The Creator",
     "Pleasure, creativity, emotional flow, sensuality, healthy desire",
     "Guilt, emotional numbness, addiction, creative block, boundary issues",
     "What do I feel? What do I desire?",
     "The sacral \xe2\x80\x94 VAM \xe2\x80\x94 the water center where emotion and creation flow",
     "Sacro", "El Creador",
     "Placer, creatividad, flujo emocional, sensualidad, deseo saludable",
     "Culpa, adormecimiento emocional, adicci\u00f3n, bloqueo creativo, problemas de l\u00edmites",
     "\u00bfQu\u00e9 siento? \u00bfQu\u00e9 deseo?",
     "El sacro \xe2\x80\x94 VAM \xe2\x80\x94 el centro del agua donde fluyen emoci\u00f3n y creaci\u00f3n"),
    ("Manipura", "Solar Plexus", "The Warrior",
     "Personal power, will, confidence, self-discipline, inner fire",
     "Shame, powerlessness, domination, anger, digestive distress",
     "Who am I? What is my purpose?",
     "The solar plexus \xe2\x80\x94 RAM \xe2\x80\x94 the fire of personal will and self-determination",
     "Plexo Solar", "El Guerrero",
     "Poder personal, voluntad, confianza, autodisciplina, fuego interior",
     "Verg\u00fcenza, impotencia, dominaci\u00f3n, ira, malestar digestivo",
     "\u00bfQui\u00e9n soy? \u00bfCu\u00e1l es mi prop\u00f3sito?",
     "El plexo solar \xe2\x80\x94 RAM \xe2\x80\x94 el fuego de la voluntad personal y la autodeterminaci\u00f3n"),
    ("Anahata", "Heart", "The Healer",
     "Unconditional love, compassion, forgiveness, connection, balance",
     "Grief, jealousy, codependency, isolation, inability to forgive",
     "Do I love? Am I loved?",
     "The heart \xe2\x80\x94 YAM \xe2\x80\x94 the unstruck sound, where human love meets the divine",
     "Coraz\u00f3n", "El Sanador",
     "Amor incondicional, compasi\u00f3n, perd\u00f3n, conexi\u00f3n, equilibrio",
     "Duelo, celos, codependencia, aislamiento, incapacidad de perdonar",
     "\u00bfAmo? \u00bfSoy amado?",
     "El coraz\u00f3n \xe2\x80\x94 YAM \xe2\x80\x94 el sonido no percutido, donde el amor humano encuentra lo divino"),
    ("Vishuddha", "Throat", "The Communicator",
     "Authentic expression, truth, creativity through voice, listening",
     "Lies, silence, over-talking, fear of judgment, creative suppression",
     "What is my truth? Can I speak it?",
     "The throat \xe2\x80\x94 HAM \xe2\x80\x94 the purification center where inner truth becomes outer expression",
     "Garganta", "El Comunicador",
     "Expresi\u00f3n aut\u00e9ntica, verdad, creatividad a trav\u00e9s de la voz, escucha",
     "Mentiras, silencio, hablar en exceso, miedo al juicio, supresi\u00f3n creativa",
     "\u00bfCu\u00e1l es mi verdad? \u00bfPuedo expresarla?",
     "La garganta \xe2\x80\x94 HAM \xe2\x80\x94 el centro de purificaci\u00f3n donde la verdad interior se vuelve expresi\u00f3n exterior"),
    ("Ajna", "Third Eye", "The Seer",
     "Intuition, insight, imagination, clarity, inner vision, wisdom",
     "Illusion, delusion, inability to see patterns, headaches, rigidity",
     "What do I see beyond the visible?",
     "The third eye \xe2\x80\x94 OM \xe2\x80\x94 the seat of intuition where duality dissolves into seeing",
     "Tercer Ojo", "El Vidente",
     "Intuici\u00f3n, percepci\u00f3n, imaginaci\u00f3n, claridad, visi\u00f3n interior, sabidur\u00eda",
     "Ilusi\u00f3n, delirio, incapacidad de ver patrones, dolores de cabeza, rigidez",
     "\u00bfQu\u00e9 veo m\u00e1s all\u00e1 de lo visible?",
     "El tercer ojo \xe2\x80\x94 OM \xe2\x80\x94 la sede de la intuici\u00f3n donde la dualidad se disuelve en visi\u00f3n"),
    ("Sahasrara", "Crown", "The Sage",
     "Transcendence, unity, spiritual connection, pure consciousness",
     "Spiritual bypassing, disconnection from embodiment, nihilism, isolation",
     "Am I connected to something greater?",
     "The crown \xe2\x80\x94 silence \xe2\x80\x94 the thousand-petaled lotus where self dissolves into the infinite",
     "Corona", "El Sabio",
     "Trascendencia, unidad, conexi\u00f3n espiritual, conciencia pura",
     "Bypass espiritual, desconexi\u00f3n de la encarnaci\u00f3n, nihilismo, aislamiento",
     "\u00bfEstoy conectado con algo mayor?",
     "La corona \xe2\x80\x94 silencio \xe2\x80\x94 el loto de mil p\u00e9talos donde el yo se disuelve en lo infinito"),
]

for i, c in enumerate(CHAKRAS):
    # Sanskrit name stays in all languages
    CK_EN[f"chakra.{i}.sanskrit"] = c[0]
    CK_EN[f"chakra.{i}.english"] = c[1]
    CK_EN[f"chakra.{i}.theme"] = c[2]
    CK_EN[f"chakra.{i}.archetype"] = c[3]
    CK_EN[f"chakra.{i}.shadow"] = c[4]
    CK_EN[f"chakra.{i}.question"] = c[5]
    CK_EN[f"chakra.{i}.brief"] = c[6]
    CK_ES[f"chakra.{i}.sanskrit"] = c[0]  # Sanskrit stays
    CK_ES[f"chakra.{i}.english"] = c[7]
    CK_ES[f"chakra.{i}.theme"] = c[8]
    CK_ES[f"chakra.{i}.archetype"] = c[9]
    CK_ES[f"chakra.{i}.shadow"] = c[10]
    CK_ES[f"chakra.{i}.question"] = c[11]
    CK_ES[f"chakra.{i}.brief"] = c[12]


# ============================================================
# NUMEROLOGY — 12 numbers (1-9, 11, 22, 33)
# ============================================================

NUM_EN = {
    "numerology.tpl.glance": "Life Path %1$d \xe2\x80\x94 %2$s",
    "numerology.tpl.glance_year": "Life Path %1$d (%2$s) \xe2\x80\x94 Year %3$d (%4$s)",
    "numerology.tpl.detail": "Life Path %1$d: %2$s. %3$s. Strength: %4$s. Challenge: %5$s.",
    "numerology.tpl.detail_year": " Personal Year %1$d: %2$s \xe2\x80\x94 %3$s.",
}

NUM_ES = {
    "numerology.tpl.glance": "Camino de Vida %1$d \xe2\x80\x94 %2$s",
    "numerology.tpl.glance_year": "Camino de Vida %1$d (%2$s) \xe2\x80\x94 A\u00f1o %3$d (%4$s)",
    "numerology.tpl.detail": "Camino de Vida %1$d: %2$s. %3$s. Fortaleza: %4$s. Desaf\u00edo: %5$s.",
    "numerology.tpl.detail_year": " A\u00f1o Personal %1$d: %2$s \xe2\x80\x94 %3$s.",
}

NUMBERS = [
    (1, "The Pioneer",
     "Independence, originality, leadership, initiative",
     "Self-reliance, courage, innovation, the ability to start from nothing",
     "Isolation, selfishness, aggression, inability to cooperate",
     "Number 1 \xe2\x80\x94 the spark of creation, the individual who walks first into the unknown",
     "El Pionero",
     "Independencia, originalidad, liderazgo, iniciativa",
     "Autosuficiencia, coraje, innovaci\u00f3n, la capacidad de empezar de la nada",
     "Aislamiento, ego\u00edsmo, agresi\u00f3n, incapacidad de cooperar",
     "N\u00famero 1 \xe2\x80\x94 la chispa de la creaci\u00f3n, el individuo que camina primero hacia lo desconocido"),
    (2, "The Peacemaker",
     "Cooperation, sensitivity, diplomacy, balance",
     "Empathy, patience, mediation, the gift of seeing both sides",
     "Codependency, over-sensitivity, indecision, self-erasure",
     "Number 2 \xe2\x80\x94 the bridge between opposites, harmony through understanding",
     "El Pacificador",
     "Cooperaci\u00f3n, sensibilidad, diplomacia, equilibrio",
     "Empat\u00eda, paciencia, mediaci\u00f3n, el don de ver ambos lados",
     "Codependencia, hipersensibilidad, indecisi\u00f3n, autoanulaci\u00f3n",
     "N\u00famero 2 \xe2\x80\x94 el puente entre opuestos, armon\u00eda a trav\u00e9s de la comprensi\u00f3n"),
    (3, "The Creator",
     "Expression, joy, creativity, communication",
     "Artistic talent, optimism, charm, the ability to inspire",
     "Scattered energy, superficiality, gossip, emotional avoidance through humor",
     "Number 3 \xe2\x80\x94 the child of 1 and 2, joy made manifest through creative expression",
     "El Creador",
     "Expresi\u00f3n, alegr\u00eda, creatividad, comunicaci\u00f3n",
     "Talento art\u00edstico, optimismo, encanto, la capacidad de inspirar",
     "Energ\u00eda dispersa, superficialidad, chisme, evasi\u00f3n emocional mediante el humor",
     "N\u00famero 3 \xe2\x80\x94 el hijo de 1 y 2, la alegr\u00eda hecha manifiesta a trav\u00e9s de la expresi\u00f3n creativa"),
    (4, "The Builder",
     "Structure, stability, discipline, hard work",
     "Reliability, endurance, practical skill, creating lasting foundations",
     "Rigidity, workaholism, narrow-mindedness, resistance to change",
     "Number 4 \xe2\x80\x94 the four walls, the four seasons, order imposed on chaos",
     "El Constructor",
     "Estructura, estabilidad, disciplina, trabajo arduo",
     "Confiabilidad, resistencia, habilidad pr\u00e1ctica, crear cimientos duraderos",
     "Rigidez, adicci\u00f3n al trabajo, estrechez mental, resistencia al cambio",
     "N\u00famero 4 \xe2\x80\x94 las cuatro paredes, las cuatro estaciones, el orden impuesto sobre el caos"),
    (5, "The Adventurer",
     "Freedom, change, versatility, sensory experience",
     "Adaptability, resourcefulness, magnetism, love of the new",
     "Restlessness, addiction, irresponsibility, fear of commitment",
     "Number 5 \xe2\x80\x94 the five senses alive, the human who needs to taste everything",
     "El Aventurero",
     "Libertad, cambio, versatilidad, experiencia sensorial",
     "Adaptabilidad, ingenio, magnetismo, amor por lo nuevo",
     "Inquietud, adicci\u00f3n, irresponsabilidad, miedo al compromiso",
     "N\u00famero 5 \xe2\x80\x94 los cinco sentidos vivos, el humano que necesita probar todo"),
    (6, "The Nurturer",
     "Responsibility, love, service, domestic harmony",
     "Compassion, healing, artistic beauty, devotion to family",
     "Control, martyrdom, perfectionism, meddling in others' lives",
     "Number 6 \xe2\x80\x94 the hexagram of balance, love expressed through responsibility",
     "El Protector",
     "Responsabilidad, amor, servicio, armon\u00eda dom\u00e9stica",
     "Compasi\u00f3n, sanaci\u00f3n, belleza art\u00edstica, devoci\u00f3n a la familia",
     "Control, martirio, perfeccionismo, entrometerse en la vida ajena",
     "N\u00famero 6 \xe2\x80\x94 el hexagrama del equilibrio, el amor expresado a trav\u00e9s de la responsabilidad"),
    (7, "The Seeker",
     "Analysis, introspection, spiritual depth, wisdom",
     "Profound thought, research ability, intuition, inner knowing",
     "Isolation, cynicism, secretiveness, intellectual arrogance",
     "Number 7 \xe2\x80\x94 the sacred number, the mind that questions until it touches mystery",
     "El Buscador",
     "An\u00e1lisis, introspecci\u00f3n, profundidad espiritual, sabidur\u00eda",
     "Pensamiento profundo, capacidad de investigaci\u00f3n, intuici\u00f3n, conocimiento interior",
     "Aislamiento, cinismo, secretismo, arrogancia intelectual",
     "N\u00famero 7 \xe2\x80\x94 el n\u00famero sagrado, la mente que pregunta hasta tocar el misterio"),
    (8, "The Powerhouse",
     "Authority, abundance, manifestation, karmic balance",
     "Business acumen, leadership, material mastery, the courage to wield power",
     "Greed, domination, workaholism, abuse of authority",
     "Number 8 \xe2\x80\x94 the infinity loop, as above so below, material power as spiritual test",
     "El Poderoso",
     "Autoridad, abundancia, manifestaci\u00f3n, equilibrio k\u00e1rmico",
     "Astucia para los negocios, liderazgo, dominio material, el coraje de ejercer el poder",
     "Codicia, dominaci\u00f3n, adicci\u00f3n al trabajo, abuso de autoridad",
     "N\u00famero 8 \xe2\x80\x94 el lazo infinito, como arriba es abajo, el poder material como prueba espiritual"),
    (9, "The Humanitarian",
     "Completion, wisdom, compassion, universal love",
     "Selflessness, artistic vision, global awareness, the ability to let go",
     "Martyrdom, resentment, emotional distance, difficulty receiving",
     "Number 9 \xe2\x80\x94 the final digit, completion that opens into new beginning",
     "El Humanitario",
     "Completitud, sabidur\u00eda, compasi\u00f3n, amor universal",
     "Generosidad, visi\u00f3n art\u00edstica, conciencia global, la capacidad de soltar",
     "Martirio, resentimiento, distancia emocional, dificultad para recibir",
     "N\u00famero 9 \xe2\x80\x94 el d\u00edgito final, la completitud que se abre hacia un nuevo comienzo"),
    (11, "The Illuminator",
     "Spiritual insight, inspiration, visionary leadership, heightened intuition",
     "Channel for higher wisdom, catalytic presence, inspiring others",
     "Anxiety, nervous tension, impracticality, unrealized potential",
     "Master 11 \xe2\x80\x94 the double pillar, a gateway between the mundane and the divine",
     "El Iluminador",
     "Percepci\u00f3n espiritual, inspiraci\u00f3n, liderazgo visionario, intuici\u00f3n elevada",
     "Canal de sabidur\u00eda superior, presencia catal\u00edtica, inspirar a otros",
     "Ansiedad, tensi\u00f3n nerviosa, impracticabilidad, potencial no realizado",
     "Maestro 11 \xe2\x80\x94 el doble pilar, una puerta entre lo mundano y lo divino"),
    (22, "The Master Builder",
     "Turning vision into reality, large-scale manifestation, practical idealism",
     "Ability to build lasting institutions, global vision with practical skill",
     "Overwhelm, self-doubt, giving up on the dream, settling for 4",
     "Master 22 \xe2\x80\x94 the architect of the impossible, vision grounded in brick and mortar",
     "El Maestro Constructor",
     "Convertir la visi\u00f3n en realidad, manifestaci\u00f3n a gran escala, idealismo pr\u00e1ctico",
     "Capacidad de construir instituciones duraderas, visi\u00f3n global con habilidad pr\u00e1ctica",
     "Agobio, inseguridad, renunciar al sue\u00f1o, conformarse con el 4",
     "Maestro 22 \xe2\x80\x94 el arquitecto de lo imposible, la visi\u00f3n cimentada en ladrillo y argamasa"),
    (33, "The Master Teacher",
     "Selfless service, healing through love, spiritual upliftment",
     "Compassion without limit, ability to heal by presence alone",
     "Martyrdom, self-sacrifice to the point of destruction, cosmic guilt",
     "Master 33 \xe2\x80\x94 the Christ number, love so vast it transforms everything it touches",
     "El Maestro Maestro",
     "Servicio desinteresado, sanaci\u00f3n a trav\u00e9s del amor, elevaci\u00f3n espiritual",
     "Compasi\u00f3n sin l\u00edmite, capacidad de sanar solo con la presencia",
     "Martirio, autosacrificio hasta la destrucci\u00f3n, culpa c\u00f3smica",
     "Maestro 33 \xe2\x80\x94 el n\u00famero cr\u00edstico, un amor tan vasto que transforma todo lo que toca"),
]

for n in NUMBERS:
    num = n[0]
    NUM_EN[f"numerology.{num}.name"] = n[1]
    NUM_EN[f"numerology.{num}.quality"] = n[2]
    NUM_EN[f"numerology.{num}.strength"] = n[3]
    NUM_EN[f"numerology.{num}.challenge"] = n[4]
    NUM_EN[f"numerology.{num}.brief"] = n[5]
    NUM_ES[f"numerology.{num}.name"] = n[6]
    NUM_ES[f"numerology.{num}.quality"] = n[7]
    NUM_ES[f"numerology.{num}.strength"] = n[8]
    NUM_ES[f"numerology.{num}.challenge"] = n[9]
    NUM_ES[f"numerology.{num}.brief"] = n[10]


# ============================================================
# GEOLOGY — 4 eons + 5 extinctions + 7 supercontinents
# ============================================================

GEO_EN = {
    "geology.tpl.glance": "%1$s \xe2\x80\x94 %2$s",
    "geology.tpl.detail": "Eon: %1$s (%2$s). %3$s. Life: %4$s.",
    "geology.tpl.extinction": " Nearest extinction: %1$s \xe2\x80\x94 %2$s.",
    "geology.tpl.supercontinent": " Supercontinent: %1$s (%2$s).",
}

GEO_ES = {
    "geology.tpl.glance": "%1$s \xe2\x80\x94 %2$s",
    "geology.tpl.detail": "E\u00f3n: %1$s (%2$s). %3$s. Vida: %4$s.",
    "geology.tpl.extinction": " Extinci\u00f3n m\u00e1s cercana: %1$s \xe2\x80\x94 %2$s.",
    "geology.tpl.supercontinent": " Supercontinente: %1$s (%2$s).",
}

EONS = [
    ("Hadean", "Hades (Greek underworld)",
     "Molten hell \xe2\x80\x94 the bombardment era when Earth was a ball of magma under constant asteroid rain",
     "No life \xe2\x80\x94 surface too hot for water, atmosphere of hydrogen and helium",
     "The age of fire \xe2\x80\x94 4.54 to 4.0 billion years ago, Earth forming from the solar nebula",
     "Hadeico", "Hades (inframundo griego)",
     "Infierno fundido \xe2\x80\x94 la era del bombardeo cuando la Tierra era una bola de magma bajo lluvia constante de asteroides",
     "Sin vida \xe2\x80\x94 superficie demasiado caliente para el agua, atm\u00f3sfera de hidr\u00f3geno y helio",
     "La era del fuego \xe2\x80\x94 4.54 a 4.0 mil millones de a\u00f1os, la Tierra form\u00e1ndose de la nebulosa solar"),
    ("Archean", "Arkhe (Greek: beginning)",
     "First oceans, first continents \xe2\x80\x94 the planet cools enough for liquid water and the first rocks",
     "First life appears \xe2\x80\x94 prokaryotes, stromatolites, the Great Oxidation Event begins",
     "The age of beginnings \xe2\x80\x94 4.0 to 2.5 Ga, when life invented itself in a world without oxygen",
     "Arcaico", "Arkhe (griego: comienzo)",
     "Primeros oc\u00e9anos, primeros continentes \xe2\x80\x94 el planeta se enfr\u00eda lo suficiente para agua l\u00edquida y las primeras rocas",
     "Aparece la primera vida \xe2\x80\x94 procariotas, estromatolitos, comienza el Gran Evento de Oxidaci\u00f3n",
     "La era de los comienzos \xe2\x80\x94 4.0 a 2.5 Ga, cuando la vida se invent\u00f3 a s\u00ed misma en un mundo sin ox\u00edgeno"),
    ("Proterozoic", "Proteros (Greek: earlier) + zoe (life)",
     "Snowball Earth episodes, oxygen rising \xe2\x80\x94 the long preparation for complex life",
     "Eukaryotes evolve, first multicellular organisms \xe2\x80\x94 the Ediacaran fauna at the end",
     "The age of hidden life \xe2\x80\x94 2.5 Ga to 538 Ma, the longest eon, oxygen slowly transforming everything",
     "Proterozoico", "Proteros (griego: anterior) + zoe (vida)",
     "Episodios de Tierra Bola de Nieve, ox\u00edgeno en ascenso \xe2\x80\x94 la larga preparaci\u00f3n para la vida compleja",
     "Evolucionan los eucariotas, primeros organismos multicelulares \xe2\x80\x94 la fauna de Ediacara al final",
     "La era de la vida oculta \xe2\x80\x94 2.5 Ga a 538 Ma, el e\u00f3n m\u00e1s largo, el ox\u00edgeno transform\u00e1ndolo todo lentamente"),
    ("Phanerozoic", "Phaneros (Greek: visible) + zoe (life)",
     "The age we live in \xe2\x80\x94 all familiar life, all mass extinctions, all human history",
     "Cambrian explosion to now \xe2\x80\x94 trilobites, dinosaurs, mammals, humans, all in 538 million years",
     "The age of visible life \xe2\x80\x94 538 Ma to present, everything we recognize as life",
     "Fanerozoico", "Phaneros (griego: visible) + zoe (vida)",
     "La era en que vivimos \xe2\x80\x94 toda la vida familiar, todas las extinciones masivas, toda la historia humana",
     "Explosi\u00f3n c\u00e1mbrica hasta ahora \xe2\x80\x94 trilobites, dinosaurios, mam\u00edferos, humanos, todo en 538 millones de a\u00f1os",
     "La era de la vida visible \xe2\x80\x94 538 Ma al presente, todo lo que reconocemos como vida"),
]

for i, e in enumerate(EONS):
    GEO_EN[f"geology.eon.{i}.name"] = e[0]
    GEO_EN[f"geology.eon.{i}.meaning"] = e[1]
    GEO_EN[f"geology.eon.{i}.character"] = e[2]
    GEO_EN[f"geology.eon.{i}.life"] = e[3]
    GEO_EN[f"geology.eon.{i}.brief"] = e[4]
    GEO_ES[f"geology.eon.{i}.name"] = e[5]
    GEO_ES[f"geology.eon.{i}.meaning"] = e[6]
    GEO_ES[f"geology.eon.{i}.character"] = e[7]
    GEO_ES[f"geology.eon.{i}.life"] = e[8]
    GEO_ES[f"geology.eon.{i}.brief"] = e[9]

EXTINCTIONS = [
    ("Late Ordovician", "End-Ordovician (~443 Ma)",
     "Glaciation of Gondwana, sea level drop, ocean anoxia",
     "Recovery took 5-10 million years; life diversified anew in the Silurian",
     "The ice extinction \xe2\x80\x94 85% of marine species lost when Gondwana froze at the South Pole",
     "Ordov\u00edcico Tard\u00edo", "Fin del Ordov\u00edcico (~443 Ma)",
     "Glaciaci\u00f3n de Gondwana, descenso del nivel del mar, anoxia oce\u00e1nica",
     "La recuperaci\u00f3n tom\u00f3 5-10 millones de a\u00f1os; la vida se diversific\u00f3 nuevamente en el Sil\u00farico",
     "La extinci\u00f3n del hielo \xe2\x80\x94 85% de especies marinas perdidas cuando Gondwana se congel\u00f3 en el Polo Sur"),
    ("Late Devonian", "Late Devonian (~372 Ma)",
     "Multiple pulses over 20 My: ocean anoxia, possible volcanism, Appalachian weathering",
     "Armored fish disappeared; tetrapods (our ancestors) survived and walked onto land",
     "The slow extinction \xe2\x80\x94 75% of species lost in pulses over 20 million years",
     "Dev\u00f3nico Tard\u00edo", "Dev\u00f3nico Tard\u00edo (~372 Ma)",
     "M\u00faltiples pulsos durante 20 Ma: anoxia oce\u00e1nica, posible vulcanismo, meteorizaci\u00f3n de los Apalaches",
     "Los peces acorazados desaparecieron; los tetr\u00e1podos (nuestros ancestros) sobrevivieron y caminaron a tierra",
     "La extinci\u00f3n lenta \xe2\x80\x94 75% de especies perdidas en pulsos durante 20 millones de a\u00f1os"),
    ("End-Permian", "End-Permian (~252 Ma)",
     "Siberian Traps volcanism, global warming, ocean acidification and anoxia",
     "Recovery took 10 million years; the emptied world was inherited by archosaurs leading to dinosaurs",
     "The Great Dying \xe2\x80\x94 96% of all species lost, the worst catastrophe life has ever faced",
     "Fin del P\u00e9rmico", "Fin del P\u00e9rmico (~252 Ma)",
     "Vulcanismo de las Trampas Siberianas, calentamiento global, acidificaci\u00f3n y anoxia oce\u00e1nica",
     "La recuperaci\u00f3n tom\u00f3 10 millones de a\u00f1os; el mundo vaciado fue heredado por arcosaurios hacia los dinosaurios",
     "La Gran Mortandad \xe2\x80\x94 96% de todas las especies perdidas, la peor cat\u00e1strofe que la vida haya enfrentado"),
    ("End-Triassic", "End-Triassic (~201 Ma)",
     "Central Atlantic Magmatic Province volcanism, CO2 spike, ocean acidification",
     "Dinosaurs inherited the emptied niches, beginning their 135-million-year reign",
     "The dinosaur's opportunity \xe2\x80\x94 80% of species lost, clearing the stage for the Jurassic giants",
     "Fin del Tri\u00e1sico", "Fin del Tri\u00e1sico (~201 Ma)",
     "Vulcanismo de la Provincia Magm\u00e1tica del Atl\u00e1ntico Central, pico de CO2, acidificaci\u00f3n oce\u00e1nica",
     "Los dinosaurios heredaron los nichos vaciados, comenzando su reinado de 135 millones de a\u00f1os",
     "La oportunidad del dinosaurio \xe2\x80\x94 80% de especies perdidas, despejando el escenario para los gigantes del Jur\u00e1sico"),
    ("End-Cretaceous", "End-Cretaceous (~66 Ma)",
     "Chicxulub asteroid impact + Deccan Traps volcanism, impact winter, global fires",
     "Mammals diversified explosively; birds survived as the last dinosaurs; our lineage began",
     "The asteroid \xe2\x80\x94 76% of species including all non-avian dinosaurs, ending one world and beginning ours",
     "Fin del Cret\u00e1cico", "Fin del Cret\u00e1cico (~66 Ma)",
     "Impacto del asteroide Chicxulub + vulcanismo de las Trampas del Dec\u00e1n, invierno por impacto, incendios globales",
     "Los mam\u00edferos se diversificaron explosivamente; las aves sobrevivieron como los \u00faltimos dinosaurios; nuestra estirpe comenz\u00f3",
     "El asteroide \xe2\x80\x94 76% de especies incluyendo todos los dinosaurios no aviarios, terminando un mundo y comenzando el nuestro"),
]

for i, x in enumerate(EXTINCTIONS):
    GEO_EN[f"geology.extinction.{i}.name"] = x[0]
    GEO_EN[f"geology.extinction.{i}.period"] = x[1]
    GEO_EN[f"geology.extinction.{i}.cause"] = x[2]
    GEO_EN[f"geology.extinction.{i}.aftermath"] = x[3]
    GEO_EN[f"geology.extinction.{i}.brief"] = x[4]
    GEO_ES[f"geology.extinction.{i}.name"] = x[5]
    GEO_ES[f"geology.extinction.{i}.period"] = x[6]
    GEO_ES[f"geology.extinction.{i}.cause"] = x[7]
    GEO_ES[f"geology.extinction.{i}.aftermath"] = x[8]
    GEO_ES[f"geology.extinction.{i}.brief"] = x[9]

SUPERCONTINENTS = [
    ("Vaalbara", "~3.6-2.8 Ga",
     "Oldest proposed supercontinent \xe2\x80\x94 evidence from South Africa (Kaapvaal) and Australia (Pilbara)",
     "The first gathering \xe2\x80\x94 Earth's earliest attempt to assemble its crust into one mass",
     "Supercontinente m\u00e1s antiguo propuesto \xe2\x80\x94 evidencia de Sud\u00e1frica (Kaapvaal) y Australia (Pilbara)",
     "La primera reuni\u00f3n \xe2\x80\x94 el primer intento de la Tierra de ensamblar su corteza en una sola masa"),
    ("Kenorland", "~2.7-2.1 Ga",
     "Archean supercontinent that preceded the Great Oxidation Event",
     "The continent that breathed \xe2\x80\x94 its breakup may have triggered the rise of atmospheric oxygen",
     "Supercontinente arcaico que precedi\u00f3 al Gran Evento de Oxidaci\u00f3n",
     "El continente que respir\u00f3 \xe2\x80\x94 su ruptura pudo haber desencadenado el ascenso del ox\u00edgeno atmosf\u00e9rico"),
    ("Columbia", "~1.8-1.3 Ga",
     "Also called Nuna \xe2\x80\x94 first well-documented supercontinent, all major cratons joined",
     "The first proven unity \xe2\x80\x94 Earth's continents locked together for 500 million years",
     "Tambi\u00e9n llamado Nuna \xe2\x80\x94 primer supercontinente bien documentado, todos los cratones principales unidos",
     "La primera unidad probada \xe2\x80\x94 los continentes de la Tierra unidos durante 500 millones de a\u00f1os"),
    ("Rodinia", "~1.1-0.75 Ga",
     "Supercontinent whose breakup may have triggered Snowball Earth glaciations",
     "The continent that froze the world \xe2\x80\x94 its rifting led to the most extreme ice ages",
     "Supercontinente cuya ruptura pudo haber desencadenado las glaciaciones de Tierra Bola de Nieve",
     "El continente que congel\u00f3 el mundo \xe2\x80\x94 su fractura llev\u00f3 a las eras de hielo m\u00e1s extremas"),
    ("Pannotia", "~0.63-0.55 Ga",
     "Brief assembly at the end of the Ediacaran, just before the Cambrian explosion",
     "The fleeting assembly \xe2\x80\x94 together and apart in 80 million years, bookending the first complex animals",
     "Breve ensamblaje al final del Ediacara, justo antes de la explosi\u00f3n c\u00e1mbrica",
     "La asamblea fugaz \xe2\x80\x94 juntos y separados en 80 millones de a\u00f1os, enmarcando los primeros animales complejos"),
    ("Pangaea", "~0.34-0.17 Ga",
     "The most famous supercontinent \xe2\x80\x94 Permian to Jurassic, all land united",
     "The one we remember \xe2\x80\x94 the supercontinent that shaped the world we know",
     "El supercontinente m\u00e1s famoso \xe2\x80\x94 P\u00e9rmico al Jur\u00e1sico, toda la tierra unida",
     "El que recordamos \xe2\x80\x94 el supercontinente que dio forma al mundo que conocemos"),
    ("Pangaea Proxima", "~0.25 Ga future",
     "Predicted next supercontinent \xe2\x80\x94 Atlantic closing, new assembly in 250 million years",
     "The one to come \xe2\x80\x94 the Atlantic will close and the continents will embrace again",
     "Pr\u00f3ximo supercontinente predicho \xe2\x80\x94 el Atl\u00e1ntico cerr\u00e1ndose, nuevo ensamblaje en 250 millones de a\u00f1os",
     "El que vendr\u00e1 \xe2\x80\x94 el Atl\u00e1ntico se cerrar\u00e1 y los continentes se abrazar\u00e1n de nuevo"),
]

for i, s in enumerate(SUPERCONTINENTS):
    # Name stays in all languages (proper nouns)
    GEO_EN[f"geology.supercontinent.{i}.name"] = s[0]
    GEO_EN[f"geology.supercontinent.{i}.age"] = s[1]
    GEO_EN[f"geology.supercontinent.{i}.description"] = s[2]
    GEO_EN[f"geology.supercontinent.{i}.brief"] = s[3]
    GEO_ES[f"geology.supercontinent.{i}.name"] = s[0]  # Proper noun
    GEO_ES[f"geology.supercontinent.{i}.age"] = s[1]    # Scientific
    GEO_ES[f"geology.supercontinent.{i}.description"] = s[4]
    GEO_ES[f"geology.supercontinent.{i}.brief"] = s[5]


# ============================================================
# MERGE
# ============================================================

def merge_and_write():
    with open(EN_PATH, "r", encoding="utf-8") as f:
        en = json.load(f)
    with open(ES_PATH, "r", encoding="utf-8") as f:
        es = json.load(f)

    en_before = len(en)
    es_before = len(es)

    for d in [TZ_EN, CK_EN, NUM_EN, GEO_EN]:
        en.update(d)
    for d in [TZ_ES, CK_ES, NUM_ES, GEO_ES]:
        es.update(d)

    en = dict(sorted(en.items()))
    es = dict(sorted(es.items()))

    with open(EN_PATH, "w", encoding="utf-8") as f:
        json.dump(en, f, indent=2, ensure_ascii=False)
        f.write("\n")
    with open(ES_PATH, "w", encoding="utf-8") as f:
        json.dump(es, f, indent=2, ensure_ascii=False)
        f.write("\n")

    print(f"EN: {en_before} -> {len(en)} keys (+{len(en) - en_before})")
    print(f"ES: {es_before} -> {len(es)} keys (+{len(es) - es_before})")


if __name__ == "__main__":
    merge_and_write()
