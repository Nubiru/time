#!/usr/bin/env python3
"""extract_content.py — Extract content strings from 5 interpret modules into JSON.

Reads data/content/en.json and data/content/es.json, adds entries for:
  1. astronomy_interpret.c  (8 planets, 8 moon phases, templates)
  2. islamic_interpret.c    (12 months, 5 prayers, templates)
  3. astrology_interpret.c  (12 signs, 4 elements, 3 modalities, templates)
  4. buddhist_interpret.c   (5 uposatha types, 4 kalpa phases, 12 months, templates)
  5. chinese_interpret.c    (12 animals, 5 elements, templates)

Key format: {system}.{entity}.{id}.{field}
Template format: {system}.tpl.{tier}

Usage:
    python3 scripts/extract_content.py
"""

import json
from pathlib import Path

PROJECT_ROOT = Path(__file__).parent.parent
CONTENT_DIR = PROJECT_ROOT / "data" / "content"
EN_PATH = CONTENT_DIR / "en.json"
ES_PATH = CONTENT_DIR / "es.json"


# ============================================================
# ASTRONOMY — 8 planets, 8 moon phases
# ============================================================

def astronomy_en():
    d = {}

    # --- Planets (0-7) ---
    planets = [
        {
            "name": "Mercury",
            "deity": "Hermes / Nabu / Thoth",
            "domain": "Communication, intellect, commerce, the messenger between worlds",
            "archetype": "The Trickster-Messenger \u2014 quicksilver mind, the psychopomp who bridges realms",
            "brief": "The swift messenger \u2014 closest to the Sun, never far from the source of light",
        },
        {
            "name": "Venus",
            "deity": "Aphrodite / Ishtar / Freya",
            "domain": "Love, beauty, harmony, values, artistic creation",
            "archetype": "The Beloved \u2014 the mirror of desire, beauty as a force that civilizes",
            "brief": "The morning and evening star \u2014 the brightest planet, whose light is borrowed",
        },
        {
            "name": "Earth",
            "deity": "Gaia / Terra / Jord",
            "domain": "Home, embodiment, the body, the ground we stand on",
            "archetype": "The Mother \u2014 the living world that sustains all observation of the cosmos",
            "brief": "The pale blue dot \u2014 the only planet we know that looks back at the sky",
        },
        {
            "name": "Mars",
            "deity": "Ares / Nergal / Tyr",
            "domain": "Action, drive, conflict, courage, the will to assert",
            "archetype": "The Warrior \u2014 the red force that fights for what matters",
            "brief": "The red planet \u2014 iron-rusted, blood-colored, named for the god of war",
        },
        {
            "name": "Jupiter",
            "deity": "Zeus / Marduk / Thor",
            "domain": "Expansion, wisdom, generosity, law, the great benefic",
            "archetype": "The King \u2014 the largest body in the solar system after the Sun, protector and judge",
            "brief": "The great benefic \u2014 more massive than all other planets combined",
        },
        {
            "name": "Saturn",
            "deity": "Kronos / Ninurta / Loki",
            "domain": "Structure, limitation, time, discipline, the great teacher",
            "archetype": "The Elder \u2014 the ringed boundary, the lesson that cannot be avoided",
            "brief": "The ringed teacher \u2014 beautiful and severe, the lord of time and consequence",
        },
        {
            "name": "Uranus",
            "deity": "Ouranos / sky father",
            "domain": "Revolution, originality, sudden insight, the unexpected",
            "archetype": "The Awakener \u2014 the lightning bolt of change, the force that disrupts to liberate",
            "brief": "The tilted rebel \u2014 rolling on its side, the planet that breaks every rule",
        },
        {
            "name": "Neptune",
            "deity": "Poseidon / Ea / Njord",
            "domain": "Imagination, transcendence, illusion, the oceanic unconscious",
            "archetype": "The Mystic \u2014 the dissolving ocean where boundaries cease to exist",
            "brief": "The blue dreamer \u2014 farthest major planet, where the solar system fades into mystery",
        },
    ]

    for i, p in enumerate(planets):
        prefix = f"astronomy.planet.{i}"
        d[f"{prefix}.name"] = p["name"]
        d[f"{prefix}.deity"] = p["deity"]
        d[f"{prefix}.domain"] = p["domain"]
        d[f"{prefix}.archetype"] = p["archetype"]
        d[f"{prefix}.brief"] = p["brief"]

    # --- Moon phases (0-7) ---
    phases = [
        {
            "name": "New Moon",
            "theme": "Beginnings, seed planting, intention setting, the dark of renewal",
            "guidance": "Set intentions, start projects, rest and reflect in the darkness",
            "brief": "The dark seed \u2014 invisible, full of potential, everything begins here",
        },
        {
            "name": "Waxing Crescent",
            "theme": "Emerging intention, first commitment, the courage to begin",
            "guidance": "Take the first step, gather resources, push through initial resistance",
            "brief": "The first sliver \u2014 intention made visible, the young light gathering strength",
        },
        {
            "name": "First Quarter",
            "theme": "Challenge, decision, the crisis of action",
            "guidance": "Make decisions, overcome obstacles, commit to the direction chosen",
            "brief": "The half-lit choice \u2014 light and dark in equal measure, the crossroads",
        },
        {
            "name": "Waxing Gibbous",
            "theme": "Refinement, adjustment, patience before fulfillment",
            "guidance": "Refine the plan, adjust based on feedback, trust the process",
            "brief": "Almost full \u2014 the final preparation, polishing before the peak",
        },
        {
            "name": "Full Moon",
            "theme": "Culmination, illumination, harvest, revelation",
            "guidance": "Celebrate completion, see clearly, harvest what was planted at the new moon",
            "brief": "The full light \u2014 everything illuminated, nothing hidden, the peak",
        },
        {
            "name": "Waning Gibbous",
            "theme": "Gratitude, sharing, teaching, distribution",
            "guidance": "Share what you've learned, give back, express gratitude for the harvest",
            "brief": "The generous decline \u2014 the light begins to give itself away",
        },
        {
            "name": "Last Quarter",
            "theme": "Release, forgiveness, clearing, the crisis of letting go",
            "guidance": "Let go of what no longer serves, forgive, clear space for the new",
            "brief": "The half-lit release \u2014 light and dark again equal, but now the light is leaving",
        },
        {
            "name": "Waning Crescent",
            "theme": "Surrender, rest, preparation for the void, the balsamic phase",
            "guidance": "Rest deeply, surrender control, prepare for the coming darkness",
            "brief": "The last whisper \u2014 the thinnest light before the dark, the wisdom of surrender",
        },
    ]

    for i, ph in enumerate(phases):
        prefix = f"astronomy.phase.{i}"
        d[f"{prefix}.name"] = ph["name"]
        d[f"{prefix}.theme"] = ph["theme"]
        d[f"{prefix}.guidance"] = ph["guidance"]
        d[f"{prefix}.brief"] = ph["brief"]

    # --- Templates ---
    # glance (retrograde): "%s (retrograde) — %s"  -> planet.name, planet.archetype
    d["astronomy.tpl.glance_retrograde"] = "%1$s (retrograde) \u2014 %2$s"
    # glance (with moon phase): "%s — %s + %s"  -> planet.name, planet.archetype, phase.name
    d["astronomy.tpl.glance_phase"] = "%1$s \u2014 %2$s + %3$s"
    # glance (plain): "%s — %s"  -> planet.name, planet.archetype
    d["astronomy.tpl.glance"] = "%1$s \u2014 %2$s"
    # detail base: "Planet: %s. Deity: %s. Domain: %s. %s."
    d["astronomy.tpl.detail"] = "Planet: %1$s. Deity: %2$s. Domain: %3$s. %4$s."
    # detail retrograde suffix
    d["astronomy.tpl.detail_retrograde"] = " RETROGRADE: apparent backward motion \u2014 the planet's themes turn inward for review."
    # detail moon suffix: " Moon: %s — %s."
    d["astronomy.tpl.detail_moon"] = " Moon: %1$s \u2014 %2$s."

    return d


def astronomy_es():
    d = {}

    planets_es = [
        {
            "name": "Mercurio",
            "deity": "Hermes / Nabu / Thoth",
            "domain": "Comunicaci\u00f3n, intelecto, comercio, el mensajero entre mundos",
            "archetype": "El Embaucador-Mensajero \u2014 mente de mercurio, el psicopompo que une reinos",
            "brief": "El mensajero veloz \u2014 el m\u00e1s cercano al Sol, nunca lejos de la fuente de luz",
        },
        {
            "name": "Venus",
            "deity": "Afrodita / Ishtar / Freya",
            "domain": "Amor, belleza, armon\u00eda, valores, creaci\u00f3n art\u00edstica",
            "archetype": "La Amada \u2014 el espejo del deseo, la belleza como fuerza civilizadora",
            "brief": "El lucero del alba y del ocaso \u2014 el planeta m\u00e1s brillante, cuya luz es prestada",
        },
        {
            "name": "Tierra",
            "deity": "Gaia / Terra / Jord",
            "domain": "Hogar, encarnaci\u00f3n, el cuerpo, el suelo que pisamos",
            "archetype": "La Madre \u2014 el mundo vivo que sostiene toda observaci\u00f3n del cosmos",
            "brief": "El punto azul p\u00e1lido \u2014 el \u00fanico planeta que conocemos que devuelve la mirada al cielo",
        },
        {
            "name": "Marte",
            "deity": "Ares / Nergal / Tyr",
            "domain": "Acci\u00f3n, impulso, conflicto, coraje, la voluntad de afirmar",
            "archetype": "El Guerrero \u2014 la fuerza roja que lucha por lo que importa",
            "brief": "El planeta rojo \u2014 oxidado por el hierro, color sangre, nombrado por el dios de la guerra",
        },
        {
            "name": "J\u00fapiter",
            "deity": "Zeus / Marduk / Thor",
            "domain": "Expansi\u00f3n, sabidur\u00eda, generosidad, ley, el gran ben\u00e9fico",
            "archetype": "El Rey \u2014 el cuerpo m\u00e1s grande del sistema solar despu\u00e9s del Sol, protector y juez",
            "brief": "El gran ben\u00e9fico \u2014 m\u00e1s masivo que todos los dem\u00e1s planetas juntos",
        },
        {
            "name": "Saturno",
            "deity": "Kronos / Ninurta / Loki",
            "domain": "Estructura, limitaci\u00f3n, tiempo, disciplina, el gran maestro",
            "archetype": "El Anciano \u2014 el l\u00edmite anillado, la lecci\u00f3n que no puede evitarse",
            "brief": "El maestro anillado \u2014 bello y severo, el se\u00f1or del tiempo y la consecuencia",
        },
        {
            "name": "Urano",
            "deity": "Ouranos / padre del cielo",
            "domain": "Revoluci\u00f3n, originalidad, percepci\u00f3n s\u00fabita, lo inesperado",
            "archetype": "El Despertador \u2014 el rayo del cambio, la fuerza que rompe para liberar",
            "brief": "El rebelde inclinado \u2014 rodando de costado, el planeta que rompe toda regla",
        },
        {
            "name": "Neptuno",
            "deity": "Poseid\u00f3n / Ea / Njord",
            "domain": "Imaginaci\u00f3n, trascendencia, ilusi\u00f3n, el inconsciente oce\u00e1nico",
            "archetype": "El M\u00edstico \u2014 el oc\u00e9ano que disuelve donde las fronteras dejan de existir",
            "brief": "El so\u00f1ador azul \u2014 el planeta mayor m\u00e1s lejano, donde el sistema solar se desvanece en misterio",
        },
    ]

    for i, p in enumerate(planets_es):
        prefix = f"astronomy.planet.{i}"
        d[f"{prefix}.name"] = p["name"]
        d[f"{prefix}.deity"] = p["deity"]
        d[f"{prefix}.domain"] = p["domain"]
        d[f"{prefix}.archetype"] = p["archetype"]
        d[f"{prefix}.brief"] = p["brief"]

    phases_es = [
        {
            "name": "Luna Nueva",
            "theme": "Comienzos, siembra de semillas, fijaci\u00f3n de intenciones, la oscuridad de la renovaci\u00f3n",
            "guidance": "Fijar intenciones, iniciar proyectos, descansar y reflexionar en la oscuridad",
            "brief": "La semilla oscura \u2014 invisible, llena de potencial, todo comienza aqu\u00ed",
        },
        {
            "name": "Cuarto Creciente Inicial",
            "theme": "Intenci\u00f3n emergente, primer compromiso, el coraje de empezar",
            "guidance": "Dar el primer paso, reunir recursos, atravesar la resistencia inicial",
            "brief": "La primera astilla \u2014 la intenci\u00f3n hecha visible, la luz joven ganando fuerza",
        },
        {
            "name": "Cuarto Creciente",
            "theme": "Desaf\u00edo, decisi\u00f3n, la crisis de la acci\u00f3n",
            "guidance": "Tomar decisiones, superar obst\u00e1culos, comprometerse con la direcci\u00f3n elegida",
            "brief": "La elecci\u00f3n a media luz \u2014 luz y oscuridad en igual medida, la encrucijada",
        },
        {
            "name": "Luna Gibosa Creciente",
            "theme": "Refinamiento, ajuste, paciencia antes de la plenitud",
            "guidance": "Refinar el plan, ajustar seg\u00fan la retroalimentaci\u00f3n, confiar en el proceso",
            "brief": "Casi llena \u2014 la preparaci\u00f3n final, el pulido antes de la cumbre",
        },
        {
            "name": "Luna Llena",
            "theme": "Culminaci\u00f3n, iluminaci\u00f3n, cosecha, revelaci\u00f3n",
            "guidance": "Celebrar la culminaci\u00f3n, ver con claridad, cosechar lo sembrado en la luna nueva",
            "brief": "La luz plena \u2014 todo iluminado, nada oculto, la cima",
        },
        {
            "name": "Luna Gibosa Menguante",
            "theme": "Gratitud, compartir, ense\u00f1ar, distribuci\u00f3n",
            "guidance": "Compartir lo aprendido, devolver, expresar gratitud por la cosecha",
            "brief": "El declive generoso \u2014 la luz comienza a entregarse",
        },
        {
            "name": "Cuarto Menguante",
            "theme": "Liberaci\u00f3n, perd\u00f3n, limpieza, la crisis de soltar",
            "guidance": "Soltar lo que ya no sirve, perdonar, abrir espacio para lo nuevo",
            "brief": "La liberaci\u00f3n a media luz \u2014 luz y oscuridad de nuevo iguales, pero ahora la luz se va",
        },
        {
            "name": "Luna Menguante",
            "theme": "Entrega, descanso, preparaci\u00f3n para el vac\u00edo, la fase bals\u00e1mica",
            "guidance": "Descansar profundamente, soltar el control, prepararse para la oscuridad que viene",
            "brief": "El \u00faltimo susurro \u2014 la luz m\u00e1s tenue antes de la oscuridad, la sabidur\u00eda de la entrega",
        },
    ]

    for i, ph in enumerate(phases_es):
        prefix = f"astronomy.phase.{i}"
        d[f"{prefix}.name"] = ph["name"]
        d[f"{prefix}.theme"] = ph["theme"]
        d[f"{prefix}.guidance"] = ph["guidance"]
        d[f"{prefix}.brief"] = ph["brief"]

    # --- Templates ---
    d["astronomy.tpl.glance_retrograde"] = "%1$s (retr\u00f3grado) \u2014 %2$s"
    d["astronomy.tpl.glance_phase"] = "%1$s \u2014 %2$s + %3$s"
    d["astronomy.tpl.glance"] = "%1$s \u2014 %2$s"
    d["astronomy.tpl.detail"] = "Planeta: %1$s. Deidad: %2$s. Dominio: %3$s. %4$s."
    d["astronomy.tpl.detail_retrograde"] = " RETR\u00d3GRADO: movimiento aparente hacia atr\u00e1s \u2014 los temas del planeta se vuelven hacia adentro para revisi\u00f3n."
    d["astronomy.tpl.detail_moon"] = " Luna: %1$s \u2014 %2$s."

    return d


# ============================================================
# ISLAMIC — 12 months, 5 prayers
# ============================================================

def islamic_en():
    d = {}

    months = [
        {"name": "Muharram",        "significance": "First month, month of Allah",
         "event": "Ashura (10th) -- Moses delivered from Pharaoh",
         "brief": "Sacred month of mourning and remembrance"},
        {"name": "Safar",           "significance": "Month of journeying",
         "event": "",
         "brief": "Month of travel -- traditionally considered difficult"},
        {"name": "Rabi al-Awwal",   "significance": "Month of the Prophet's birth",
         "event": "Mawlid (12th) -- birthday of Prophet Muhammad",
         "brief": "Month of joy and prophetic celebration"},
        {"name": "Rabi al-Thani",   "significance": "Month of spring's continuation",
         "event": "",
         "brief": "Quiet month of reflection and growth"},
        {"name": "Jumada al-Ula",   "significance": "Month of frozen water",
         "event": "",
         "brief": "Month of endurance through hardship"},
        {"name": "Jumada al-Thani", "significance": "Month of summer's approach",
         "event": "",
         "brief": "Month of preparation and anticipation"},
        {"name": "Rajab",           "significance": "Month of respect",
         "event": "Isra and Mi'raj (27th) -- Night Journey",
         "brief": "Sacred month of spiritual ascent"},
        {"name": "Sha'ban",         "significance": "Month of division/separation",
         "event": "Laylat al-Bara'ah (15th) -- Night of Fortune",
         "brief": "Month of preparation for Ramadan"},
        {"name": "Ramadan",         "significance": "Month of burning heat/fasting",
         "event": "Entire month: Sawm (fasting), Laylat al-Qadr (27th)",
         "brief": "Month of fasting, revelation, and spiritual renewal"},
        {"name": "Shawwal",         "significance": "Month of upliftment",
         "event": "Eid al-Fitr (1st) -- Festival of Breaking Fast",
         "brief": "Month of celebration and voluntary fasting"},
        {"name": "Dhul Qa'dah",     "significance": "Month of rest",
         "event": "",
         "brief": "Sacred month of truce and preparation for Hajj"},
        {"name": "Dhul Hijjah",     "significance": "Month of pilgrimage",
         "event": "Hajj (8th-12th), Eid al-Adha (10th) -- Festival of Sacrifice",
         "brief": "Sacred month of pilgrimage and sacrifice"},
    ]

    for i, m in enumerate(months):
        idx = i + 1  # months are 1-indexed
        prefix = f"islamic.month.{idx}"
        d[f"{prefix}.name"] = m["name"]
        d[f"{prefix}.significance"] = m["significance"]
        d[f"{prefix}.event"] = m["event"]
        d[f"{prefix}.brief"] = m["brief"]

    prayers = [
        {"name": "Fajr",    "arabic": "al-Fajr",    "time_desc": "Pre-dawn, first light",
         "meaning": "Awakening -- beginning before the world stirs"},
        {"name": "Dhuhr",   "arabic": "al-Zuhr",    "time_desc": "Midday, after zenith",
         "meaning": "Remembrance -- pausing at the day's peak"},
        {"name": "Asr",     "arabic": "al-'Asr",    "time_desc": "Afternoon, declining sun",
         "meaning": "Reflection -- awareness as time passes"},
        {"name": "Maghrib", "arabic": "al-Maghrib",  "time_desc": "Just after sunset",
         "meaning": "Gratitude -- marking the day's completion"},
        {"name": "Isha",    "arabic": "al-'Isha",    "time_desc": "Night, after twilight fades",
         "meaning": "Surrender -- trust in the darkness"},
    ]

    for i, p in enumerate(prayers):
        prefix = f"islamic.prayer.{i}"
        d[f"{prefix}.name"] = p["name"]
        d[f"{prefix}.arabic"] = p["arabic"]
        d[f"{prefix}.time_desc"] = p["time_desc"]
        d[f"{prefix}.meaning"] = p["meaning"]

    # --- Templates ---
    # glance: "{year} {month_name} {day} -- {significance}"
    d["islamic.tpl.glance"] = "%1$d %2$s %3$d -- %4$s"
    # detail base: "{month_name} ({brief})."
    d["islamic.tpl.detail"] = "%1$s (%2$s)."
    # detail sacred suffix
    d["islamic.tpl.detail_sacred"] = " This is one of the four sacred months."
    # detail event suffix: " {event}."
    d["islamic.tpl.detail_event"] = " %1$s."

    return d


def islamic_es():
    d = {}

    months_es = [
        {"name": "Muharram",        "significance": "Primer mes, mes de Al\u00e1",
         "event": "Ashura (10) -- Mois\u00e9s liberado del Fara\u00f3n",
         "brief": "Mes sagrado de luto y remembranza"},
        {"name": "Safar",           "significance": "Mes de los viajes",
         "event": "",
         "brief": "Mes de viajes -- tradicionalmente considerado dif\u00edcil"},
        {"name": "Rabi al-Awwal",   "significance": "Mes del nacimiento del Profeta",
         "event": "Mawlid (12) -- nacimiento del Profeta Muhammad",
         "brief": "Mes de alegr\u00eda y celebraci\u00f3n prof\u00e9tica"},
        {"name": "Rabi al-Thani",   "significance": "Mes de la continuaci\u00f3n de la primavera",
         "event": "",
         "brief": "Mes tranquilo de reflexi\u00f3n y crecimiento"},
        {"name": "Jumada al-Ula",   "significance": "Mes del agua helada",
         "event": "",
         "brief": "Mes de resistencia ante la adversidad"},
        {"name": "Jumada al-Thani", "significance": "Mes de la llegada del verano",
         "event": "",
         "brief": "Mes de preparaci\u00f3n y anticipaci\u00f3n"},
        {"name": "Rajab",           "significance": "Mes del respeto",
         "event": "Isra y Mi'raj (27) -- el Viaje Nocturno",
         "brief": "Mes sagrado de ascenso espiritual"},
        {"name": "Sha'ban",         "significance": "Mes de divisi\u00f3n/separaci\u00f3n",
         "event": "Laylat al-Bara'ah (15) -- Noche de la Fortuna",
         "brief": "Mes de preparaci\u00f3n para el Ramad\u00e1n"},
        {"name": "Ramad\u00e1n",    "significance": "Mes del calor ardiente/ayuno",
         "event": "Todo el mes: Sawm (ayuno), Laylat al-Qadr (27)",
         "brief": "Mes de ayuno, revelaci\u00f3n y renovaci\u00f3n espiritual"},
        {"name": "Shawwal",         "significance": "Mes de la elevaci\u00f3n",
         "event": "Eid al-Fitr (1) -- Festival de la Ruptura del Ayuno",
         "brief": "Mes de celebraci\u00f3n y ayuno voluntario"},
        {"name": "Dhul Qa'dah",     "significance": "Mes del descanso",
         "event": "",
         "brief": "Mes sagrado de tregua y preparaci\u00f3n para el Hajj"},
        {"name": "Dhul Hijjah",     "significance": "Mes de la peregrinaci\u00f3n",
         "event": "Hajj (8-12), Eid al-Adha (10) -- Festival del Sacrificio",
         "brief": "Mes sagrado de peregrinaci\u00f3n y sacrificio"},
    ]

    for i, m in enumerate(months_es):
        idx = i + 1
        prefix = f"islamic.month.{idx}"
        d[f"{prefix}.name"] = m["name"]
        d[f"{prefix}.significance"] = m["significance"]
        d[f"{prefix}.event"] = m["event"]
        d[f"{prefix}.brief"] = m["brief"]

    prayers_es = [
        {"name": "Fajr",    "arabic": "al-Fajr",    "time_desc": "Pre-amanecer, primera luz",
         "meaning": "Despertar -- comenzar antes de que el mundo se agite"},
        {"name": "Dhuhr",   "arabic": "al-Zuhr",    "time_desc": "Mediod\u00eda, despu\u00e9s del cenit",
         "meaning": "Remembranza -- pausar en el apogeo del d\u00eda"},
        {"name": "Asr",     "arabic": "al-'Asr",    "time_desc": "Tarde, sol declinante",
         "meaning": "Reflexi\u00f3n -- conciencia del paso del tiempo"},
        {"name": "Maghrib", "arabic": "al-Maghrib",  "time_desc": "Justo despu\u00e9s del ocaso",
         "meaning": "Gratitud -- marcar la plenitud del d\u00eda"},
        {"name": "Isha",    "arabic": "al-'Isha",    "time_desc": "Noche, cuando el crep\u00fasculo se desvanece",
         "meaning": "Entrega -- confianza en la oscuridad"},
    ]

    for i, p in enumerate(prayers_es):
        prefix = f"islamic.prayer.{i}"
        d[f"{prefix}.name"] = p["name"]
        d[f"{prefix}.arabic"] = p["arabic"]
        d[f"{prefix}.time_desc"] = p["time_desc"]
        d[f"{prefix}.meaning"] = p["meaning"]

    # --- Templates ---
    d["islamic.tpl.glance"] = "%1$d %2$s %3$d -- %4$s"
    d["islamic.tpl.detail"] = "%1$s (%2$s)."
    d["islamic.tpl.detail_sacred"] = " Este es uno de los cuatro meses sagrados."
    d["islamic.tpl.detail_event"] = " %1$s."

    return d


# ============================================================
# ASTROLOGY — 12 signs, 4 elements, 3 modalities
# ============================================================

def astrology_en():
    d = {}

    signs = [
        {"name": "Aries",       "element": "Fire",  "modality": "Cardinal", "ruler": "Mars",    "archetype": "The Warrior",
         "keywords": "Courage, initiative, independence",
         "shadow": "Impatience, aggression, selfishness",
         "brief": "The spark that begins \u2014 raw courage to exist"},
        {"name": "Taurus",      "element": "Earth", "modality": "Fixed",    "ruler": "Venus",   "archetype": "The Builder",
         "keywords": "Stability, sensuality, persistence",
         "shadow": "Stubbornness, possessiveness, inertia",
         "brief": "The earth that endures \u2014 patient cultivation of beauty"},
        {"name": "Gemini",      "element": "Air",   "modality": "Mutable",  "ruler": "Mercury", "archetype": "The Messenger",
         "keywords": "Communication, curiosity, adaptability",
         "shadow": "Superficiality, restlessness, duplicity",
         "brief": "The breath between \u2014 connecting through language"},
        {"name": "Cancer",      "element": "Water", "modality": "Cardinal", "ruler": "Moon",    "archetype": "The Mother",
         "keywords": "Nurture, emotion, protection",
         "shadow": "Clinginess, moodiness, manipulation",
         "brief": "The shell that protects \u2014 feeling as a form of knowing"},
        {"name": "Leo",         "element": "Fire",  "modality": "Fixed",    "ruler": "Sun",     "archetype": "The King",
         "keywords": "Creativity, generosity, self-expression",
         "shadow": "Pride, drama, dominance",
         "brief": "The fire that sustains \u2014 the courage to shine"},
        {"name": "Virgo",       "element": "Earth", "modality": "Mutable",  "ruler": "Mercury", "archetype": "The Healer",
         "keywords": "Analysis, service, refinement",
         "shadow": "Criticism, anxiety, perfectionism",
         "brief": "The hand that mends \u2014 wholeness through discernment"},
        {"name": "Libra",       "element": "Air",   "modality": "Cardinal", "ruler": "Venus",   "archetype": "The Diplomat",
         "keywords": "Harmony, partnership, justice",
         "shadow": "Indecision, people-pleasing, avoidance",
         "brief": "The scales that balance \u2014 beauty as equilibrium"},
        {"name": "Scorpio",     "element": "Water", "modality": "Fixed",    "ruler": "Pluto",   "archetype": "The Alchemist",
         "keywords": "Transformation, intensity, depth",
         "shadow": "Obsession, jealousy, destruction",
         "brief": "The depth that transforms \u2014 death as doorway"},
        {"name": "Sagittarius", "element": "Fire",  "modality": "Mutable",  "ruler": "Jupiter", "archetype": "The Explorer",
         "keywords": "Philosophy, adventure, truth-seeking",
         "shadow": "Excess, dogmatism, restlessness",
         "brief": "The arrow that seeks \u2014 meaning beyond the horizon"},
        {"name": "Capricorn",   "element": "Earth", "modality": "Cardinal", "ruler": "Saturn",  "archetype": "The Elder",
         "keywords": "Ambition, discipline, mastery",
         "shadow": "Coldness, rigidity, ruthlessness",
         "brief": "The mountain that stands \u2014 authority earned through time"},
        {"name": "Aquarius",    "element": "Air",   "modality": "Fixed",    "ruler": "Uranus",  "archetype": "The Visionary",
         "keywords": "Innovation, humanity, independence",
         "shadow": "Detachment, rebellion, eccentricity",
         "brief": "The wave that carries \u2014 freedom in service of all"},
        {"name": "Pisces",      "element": "Water", "modality": "Mutable",  "ruler": "Neptune", "archetype": "The Mystic",
         "keywords": "Compassion, imagination, transcendence",
         "shadow": "Escapism, confusion, martyrdom",
         "brief": "The ocean that dissolves \u2014 unity beyond separation"},
    ]

    for i, s in enumerate(signs):
        prefix = f"astrology.sign.{i}"
        d[f"{prefix}.name"] = s["name"]
        d[f"{prefix}.element"] = s["element"]
        d[f"{prefix}.modality"] = s["modality"]
        d[f"{prefix}.ruler"] = s["ruler"]
        d[f"{prefix}.archetype"] = s["archetype"]
        d[f"{prefix}.keywords"] = s["keywords"]
        d[f"{prefix}.shadow"] = s["shadow"]
        d[f"{prefix}.brief"] = s["brief"]

    elements = [
        {"name": "Fire",  "quality": "Will, spirit, creative force",
         "expression": "Through action, inspiration, and enthusiasm"},
        {"name": "Earth", "quality": "Form, matter, practical wisdom",
         "expression": "Through building, sustaining, and manifesting"},
        {"name": "Air",   "quality": "Mind, communication, relationship",
         "expression": "Through thinking, connecting, and exchanging"},
        {"name": "Water", "quality": "Feeling, intuition, the unconscious",
         "expression": "Through emotion, empathy, and transformation"},
    ]

    for i, e in enumerate(elements):
        prefix = f"astrology.element.{i}"
        d[f"{prefix}.name"] = e["name"]
        d[f"{prefix}.quality"] = e["quality"]
        d[f"{prefix}.expression"] = e["expression"]

    modalities = [
        {"name": "Cardinal", "quality": "Initiation, leadership, new beginnings",
         "role": "Begins the season \u2014 the spark that starts"},
        {"name": "Fixed",    "quality": "Sustaining, determination, depth",
         "role": "Holds the season \u2014 the strength that endures"},
        {"name": "Mutable",  "quality": "Adaptation, flexibility, transition",
         "role": "Ends the season \u2014 the wisdom that releases"},
    ]

    for i, m in enumerate(modalities):
        prefix = f"astrology.modality.{i}"
        d[f"{prefix}.name"] = m["name"]
        d[f"{prefix}.quality"] = m["quality"]
        d[f"{prefix}.role"] = m["role"]

    # Abbreviations
    abbrs = ["Ari", "Tau", "Gem", "Can", "Leo", "Vir",
             "Lib", "Sco", "Sag", "Cap", "Aqu", "Pis"]
    for i, a in enumerate(abbrs):
        d[f"astrology.sign.{i}.abbr"] = a

    # --- Templates ---
    # glance with ascendant: "Sun in %s . Moon in %s . %s Rising"
    d["astrology.tpl.glance_asc"] = "Sun in %1$s \u00b7 Moon in %2$s \u00b7 %3$s Rising"
    # glance without ascendant: "Sun in %s . Moon in %s"
    d["astrology.tpl.glance"] = "Sun in %1$s \u00b7 Moon in %2$s"
    # detail base: "{sun} Sun: {archetype}. {brief}. Keywords: {kw}. Shadow: {shadow}. Element: {elem} ({quality}). Modality: {mod} ({role}). Moon in {moon}: emotional nature is {moon_archetype}."
    d["astrology.tpl.detail"] = (
        "%1$s Sun: %2$s. %3$s. Keywords: %4$s. Shadow: %5$s. "
        "Element: %6$s (%7$s). Modality: %8$s (%9$s). "
        "Moon in %10$s: emotional nature is %11$s."
    )
    # detail ascendant suffix: " Ascendant in %s: the mask is %s."
    d["astrology.tpl.detail_asc"] = " Ascendant in %1$s: the mask is %2$s."

    return d


def astrology_es():
    d = {}

    signs_es = [
        {"name": "Aries",       "element": "Fuego",  "modality": "Cardinal",  "ruler": "Marte",    "archetype": "El Guerrero",
         "keywords": "Coraje, iniciativa, independencia",
         "shadow": "Impaciencia, agresi\u00f3n, ego\u00edsmo",
         "brief": "La chispa que inicia \u2014 el coraje crudo de existir"},
        {"name": "Tauro",       "element": "Tierra", "modality": "Fijo",      "ruler": "Venus",    "archetype": "El Constructor",
         "keywords": "Estabilidad, sensualidad, persistencia",
         "shadow": "Terquedad, posesividad, inercia",
         "brief": "La tierra que perdura \u2014 cultivo paciente de la belleza"},
        {"name": "G\u00e9minis", "element": "Aire",  "modality": "Mutable",   "ruler": "Mercurio", "archetype": "El Mensajero",
         "keywords": "Comunicaci\u00f3n, curiosidad, adaptabilidad",
         "shadow": "Superficialidad, inquietud, duplicidad",
         "brief": "El aliento entre \u2014 conectar a trav\u00e9s del lenguaje"},
        {"name": "C\u00e1ncer", "element": "Agua",   "modality": "Cardinal",  "ruler": "Luna",     "archetype": "La Madre",
         "keywords": "Nutrir, emoci\u00f3n, protecci\u00f3n",
         "shadow": "Apego, cambios de humor, manipulaci\u00f3n",
         "brief": "La concha que protege \u2014 el sentir como forma de conocer"},
        {"name": "Leo",         "element": "Fuego",  "modality": "Fijo",      "ruler": "Sol",      "archetype": "El Rey",
         "keywords": "Creatividad, generosidad, autoexpresi\u00f3n",
         "shadow": "Orgullo, drama, dominaci\u00f3n",
         "brief": "El fuego que sostiene \u2014 el coraje de brillar"},
        {"name": "Virgo",       "element": "Tierra", "modality": "Mutable",   "ruler": "Mercurio", "archetype": "El Sanador",
         "keywords": "An\u00e1lisis, servicio, refinamiento",
         "shadow": "Cr\u00edtica, ansiedad, perfeccionismo",
         "brief": "La mano que repara \u2014 plenitud a trav\u00e9s del discernimiento"},
        {"name": "Libra",       "element": "Aire",   "modality": "Cardinal",  "ruler": "Venus",    "archetype": "El Diplom\u00e1tico",
         "keywords": "Armon\u00eda, asociaci\u00f3n, justicia",
         "shadow": "Indecisi\u00f3n, complacencia, evasi\u00f3n",
         "brief": "La balanza que equilibra \u2014 la belleza como equilibrio"},
        {"name": "Escorpio",    "element": "Agua",   "modality": "Fijo",      "ruler": "Plut\u00f3n", "archetype": "El Alquimista",
         "keywords": "Transformaci\u00f3n, intensidad, profundidad",
         "shadow": "Obsesi\u00f3n, celos, destrucci\u00f3n",
         "brief": "La profundidad que transforma \u2014 la muerte como portal"},
        {"name": "Sagitario",   "element": "Fuego",  "modality": "Mutable",   "ruler": "J\u00fapiter", "archetype": "El Explorador",
         "keywords": "Filosof\u00eda, aventura, b\u00fasqueda de verdad",
         "shadow": "Exceso, dogmatismo, inquietud",
         "brief": "La flecha que busca \u2014 significado m\u00e1s all\u00e1 del horizonte"},
        {"name": "Capricornio", "element": "Tierra", "modality": "Cardinal",  "ruler": "Saturno",  "archetype": "El Anciano",
         "keywords": "Ambici\u00f3n, disciplina, maestr\u00eda",
         "shadow": "Frialdad, rigidez, crueldad",
         "brief": "La monta\u00f1a que permanece \u2014 autoridad ganada con el tiempo"},
        {"name": "Acuario",     "element": "Aire",   "modality": "Fijo",      "ruler": "Urano",    "archetype": "El Visionario",
         "keywords": "Innovaci\u00f3n, humanidad, independencia",
         "shadow": "Desapego, rebeld\u00eda, excentricidad",
         "brief": "La ola que lleva \u2014 libertad al servicio de todos"},
        {"name": "Piscis",      "element": "Agua",   "modality": "Mutable",   "ruler": "Neptuno",  "archetype": "El M\u00edstico",
         "keywords": "Compasi\u00f3n, imaginaci\u00f3n, trascendencia",
         "shadow": "Escapismo, confusi\u00f3n, martirio",
         "brief": "El oc\u00e9ano que disuelve \u2014 unidad m\u00e1s all\u00e1 de la separaci\u00f3n"},
    ]

    for i, s in enumerate(signs_es):
        prefix = f"astrology.sign.{i}"
        d[f"{prefix}.name"] = s["name"]
        d[f"{prefix}.element"] = s["element"]
        d[f"{prefix}.modality"] = s["modality"]
        d[f"{prefix}.ruler"] = s["ruler"]
        d[f"{prefix}.archetype"] = s["archetype"]
        d[f"{prefix}.keywords"] = s["keywords"]
        d[f"{prefix}.shadow"] = s["shadow"]
        d[f"{prefix}.brief"] = s["brief"]

    elements_es = [
        {"name": "Fuego",  "quality": "Voluntad, esp\u00edritu, fuerza creativa",
         "expression": "A trav\u00e9s de la acci\u00f3n, la inspiraci\u00f3n y el entusiasmo"},
        {"name": "Tierra", "quality": "Forma, materia, sabidur\u00eda pr\u00e1ctica",
         "expression": "A trav\u00e9s de construir, sostener y manifestar"},
        {"name": "Aire",   "quality": "Mente, comunicaci\u00f3n, relaci\u00f3n",
         "expression": "A trav\u00e9s del pensamiento, la conexi\u00f3n y el intercambio"},
        {"name": "Agua",   "quality": "Sentimiento, intuici\u00f3n, el inconsciente",
         "expression": "A trav\u00e9s de la emoci\u00f3n, la empat\u00eda y la transformaci\u00f3n"},
    ]

    for i, e in enumerate(elements_es):
        prefix = f"astrology.element.{i}"
        d[f"{prefix}.name"] = e["name"]
        d[f"{prefix}.quality"] = e["quality"]
        d[f"{prefix}.expression"] = e["expression"]

    modalities_es = [
        {"name": "Cardinal", "quality": "Iniciaci\u00f3n, liderazgo, nuevos comienzos",
         "role": "Inicia la estaci\u00f3n \u2014 la chispa que enciende"},
        {"name": "Fijo",     "quality": "Sostenimiento, determinaci\u00f3n, profundidad",
         "role": "Sostiene la estaci\u00f3n \u2014 la fuerza que perdura"},
        {"name": "Mutable",  "quality": "Adaptaci\u00f3n, flexibilidad, transici\u00f3n",
         "role": "Cierra la estaci\u00f3n \u2014 la sabidur\u00eda que libera"},
    ]

    for i, m in enumerate(modalities_es):
        prefix = f"astrology.modality.{i}"
        d[f"{prefix}.name"] = m["name"]
        d[f"{prefix}.quality"] = m["quality"]
        d[f"{prefix}.role"] = m["role"]

    # Same abbreviations (universal)
    abbrs = ["Ari", "Tau", "Gem", "Can", "Leo", "Vir",
             "Lib", "Sco", "Sag", "Cap", "Aqu", "Pis"]
    for i, a in enumerate(abbrs):
        d[f"astrology.sign.{i}.abbr"] = a

    # --- Templates ---
    d["astrology.tpl.glance_asc"] = "Sol en %1$s \u00b7 Luna en %2$s \u00b7 %3$s Ascendente"
    d["astrology.tpl.glance"] = "Sol en %1$s \u00b7 Luna en %2$s"
    d["astrology.tpl.detail"] = (
        "Sol en %1$s: %2$s. %3$s. Palabras clave: %4$s. Sombra: %5$s. "
        "Elemento: %6$s (%7$s). Modalidad: %8$s (%9$s). "
        "Luna en %10$s: la naturaleza emocional es %11$s."
    )
    d["astrology.tpl.detail_asc"] = " Ascendente en %1$s: la m\u00e1scara es %2$s."

    return d


# ============================================================
# BUDDHIST — 5 uposatha types, 4 kalpa phases, 12 months
# ============================================================

def buddhist_en():
    d = {}

    uposathas = [
        {"name": "No Observance",
         "practice": "Regular day",
         "meaning": "An ordinary day in the cycle"},
        {"name": "New Moon Observance",
         "practice": "Monastics recite Patimokkha; laity observe 8 precepts",
         "meaning": "Darkness as potential \u2014 the seed moment before renewal"},
        {"name": "Full Moon Observance",
         "practice": "Most important \u2014 full Patimokkha recital, intensive practice",
         "meaning": "Illumination \u2014 the Buddha's birth, enlightenment, and parinibbana all on full moons"},
        {"name": "Waxing Half-Moon",
         "practice": "Lighter observance, meditation and reflection",
         "meaning": "Growing light \u2014 effort and aspiration building"},
        {"name": "Waning Half-Moon",
         "practice": "Lighter observance, letting go practice",
         "meaning": "Releasing \u2014 wisdom of impermanence, letting go"},
    ]

    for i, u in enumerate(uposathas):
        prefix = f"buddhist.uposatha.{i}"
        d[f"{prefix}.name"] = u["name"]
        d[f"{prefix}.practice"] = u["practice"]
        d[f"{prefix}.meaning"] = u["meaning"]

    kalpas = [
        {"name": "Formation",   "pali": "Vivatta",
         "description": "Worlds arise from emptiness \u2014 matter condenses, beings take form",
         "meaning": "The cosmic breath inward \u2014 creation as inevitable arising"},
        {"name": "Existence",   "pali": "Vivattathaayi",
         "description": "Beings live, die, and are reborn \u2014 the wheel turns",
         "meaning": "The present era \u2014 sentient experience in all its suffering and beauty"},
        {"name": "Destruction", "pali": "Samvatta",
         "description": "Worlds dissolve through fire, water, or wind",
         "meaning": "The cosmic breath outward \u2014 all conditioned things are impermanent"},
        {"name": "Emptiness",   "pali": "Samvattathaayi",
         "description": "Vast darkness between worlds \u2014 nothing remains",
         "meaning": "The great rest \u2014 potential without form, like dreamless sleep"},
    ]

    for i, k in enumerate(kalpas):
        prefix = f"buddhist.kalpa.{i}"
        d[f"{prefix}.name"] = k["name"]
        d[f"{prefix}.pali"] = k["pali"]
        d[f"{prefix}.description"] = k["description"]
        d[f"{prefix}.meaning"] = k["meaning"]

    months = [
        {"pali_name": "Citta",      "season": "Hot",   "observance": "Songkran/New Year in some traditions",
         "meaning": "Month of awakening \u2014 nature stirs"},
        {"pali_name": "Vesakha",    "season": "Hot",   "observance": "Vesak \u2014 Buddha's birth, enlightenment, parinibbana",
         "meaning": "The most sacred month \u2014 triple anniversary"},
        {"pali_name": "Jettha",     "season": "Hot",   "observance": "",
         "meaning": "Month of endurance through heat"},
        {"pali_name": "Asalha",     "season": "Rainy", "observance": "Asalha Puja \u2014 First Sermon, Dhammacakkappavattana",
         "meaning": "Month of the first teaching \u2014 the Wheel turns"},
        {"pali_name": "Savana",     "season": "Rainy", "observance": "Vassa begins \u2014 Rain Retreat starts",
         "meaning": "Month of withdrawal \u2014 monastics enter retreat"},
        {"pali_name": "Bhaddapada", "season": "Rainy", "observance": "Vassa continues",
         "meaning": "Month of devoted practice"},
        {"pali_name": "Assayuja",   "season": "Rainy", "observance": "Vassa ends \u2014 Pavarana ceremony",
         "meaning": "Month of invitation \u2014 community reconciliation"},
        {"pali_name": "Kattika",    "season": "Cool",  "observance": "Kathina \u2014 robe offering to monastics",
         "meaning": "Month of generosity \u2014 dana and gratitude"},
        {"pali_name": "Maggasira",  "season": "Cool",  "observance": "",
         "meaning": "Month of the path \u2014 quiet walking"},
        {"pali_name": "Phussa",     "season": "Cool",  "observance": "",
         "meaning": "Month of nourishment"},
        {"pali_name": "Magha",      "season": "Cool",  "observance": "Magha Puja \u2014 Sangha Day, 1250 monks gathered",
         "meaning": "Month of the Sangha \u2014 community of awakening"},
        {"pali_name": "Phalguna",   "season": "Hot",   "observance": "",
         "meaning": "Month of completion \u2014 cycle prepares to renew"},
    ]

    for i, m in enumerate(months):
        idx = i + 1
        prefix = f"buddhist.month.{idx}"
        d[f"{prefix}.pali_name"] = m["pali_name"]
        d[f"{prefix}.season"] = m["season"]
        d[f"{prefix}.observance"] = m["observance"]
        d[f"{prefix}.meaning"] = m["meaning"]

    # --- Templates ---
    # glance (with uposatha): "BE %d . %s . %s"  -> be_year, pali_name, uposatha.name
    d["buddhist.tpl.glance_uposatha"] = "BE %1$d \u00b7 %2$s \u00b7 %3$s"
    # glance (no uposatha): "BE %d . %s . %s"  -> be_year, pali_name, season
    d["buddhist.tpl.glance"] = "BE %1$d \u00b7 %2$s \u00b7 %3$s"
    # detail base: "{pali_name}: {meaning}."
    d["buddhist.tpl.detail"] = "%1$s: %2$s."
    # detail uposatha suffix: " {practice}. {meaning}."
    d["buddhist.tpl.detail_uposatha"] = " %1$s. %2$s."
    # detail kalpa suffix: " {name} phase ({pali}): {meaning}."
    d["buddhist.tpl.detail_kalpa"] = " %1$s phase (%2$s): %3$s."

    return d


def buddhist_es():
    d = {}

    uposathas_es = [
        {"name": "Sin Observancia",
         "practice": "D\u00eda regular",
         "meaning": "Un d\u00eda ordinario en el ciclo"},
        {"name": "Observancia de Luna Nueva",
         "practice": "Los mon\u00e1sticos recitan el Patimokkha; los laicos observan 8 preceptos",
         "meaning": "La oscuridad como potencial \u2014 el momento semilla antes de la renovaci\u00f3n"},
        {"name": "Observancia de Luna Llena",
         "practice": "La m\u00e1s importante \u2014 recitaci\u00f3n completa del Patimokkha, pr\u00e1ctica intensiva",
         "meaning": "Iluminaci\u00f3n \u2014 el nacimiento, la iluminaci\u00f3n y el parinibbana del Buda, todos en lunas llenas"},
        {"name": "Media Luna Creciente",
         "practice": "Observancia menor, meditaci\u00f3n y reflexi\u00f3n",
         "meaning": "Luz creciente \u2014 esfuerzo y aspiraci\u00f3n en construcci\u00f3n"},
        {"name": "Media Luna Menguante",
         "practice": "Observancia menor, pr\u00e1ctica de soltar",
         "meaning": "Liberaci\u00f3n \u2014 sabidur\u00eda de la impermanencia, soltar"},
    ]

    for i, u in enumerate(uposathas_es):
        prefix = f"buddhist.uposatha.{i}"
        d[f"{prefix}.name"] = u["name"]
        d[f"{prefix}.practice"] = u["practice"]
        d[f"{prefix}.meaning"] = u["meaning"]

    kalpas_es = [
        {"name": "Formaci\u00f3n", "pali": "Vivatta",
         "description": "Mundos surgen de la vacuidad \u2014 la materia se condensa, los seres toman forma",
         "meaning": "La respiraci\u00f3n c\u00f3smica hacia adentro \u2014 la creaci\u00f3n como surgimiento inevitable"},
        {"name": "Existencia",     "pali": "Vivattathaayi",
         "description": "Los seres viven, mueren y renacen \u2014 la rueda gira",
         "meaning": "La era presente \u2014 la experiencia sintiente en todo su sufrimiento y belleza"},
        {"name": "Destrucci\u00f3n", "pali": "Samvatta",
         "description": "Los mundos se disuelven por fuego, agua o viento",
         "meaning": "La respiraci\u00f3n c\u00f3smica hacia afuera \u2014 todo lo condicionado es impermanente"},
        {"name": "Vacuidad",       "pali": "Samvattathaayi",
         "description": "Vasta oscuridad entre mundos \u2014 nada permanece",
         "meaning": "El gran reposo \u2014 potencial sin forma, como un sue\u00f1o sin sue\u00f1os"},
    ]

    for i, k in enumerate(kalpas_es):
        prefix = f"buddhist.kalpa.{i}"
        d[f"{prefix}.name"] = k["name"]
        d[f"{prefix}.pali"] = k["pali"]
        d[f"{prefix}.description"] = k["description"]
        d[f"{prefix}.meaning"] = k["meaning"]

    months_es = [
        {"pali_name": "Citta",      "season": "Calurosa", "observance": "Songkran/A\u00f1o Nuevo en algunas tradiciones",
         "meaning": "Mes del despertar \u2014 la naturaleza se agita"},
        {"pali_name": "Vesakha",    "season": "Calurosa", "observance": "Vesak \u2014 nacimiento, iluminaci\u00f3n y parinibbana del Buda",
         "meaning": "El mes m\u00e1s sagrado \u2014 triple aniversario"},
        {"pali_name": "Jettha",     "season": "Calurosa", "observance": "",
         "meaning": "Mes de resistencia ante el calor"},
        {"pali_name": "Asalha",     "season": "Lluviosa", "observance": "Asalha Puja \u2014 Primer Serm\u00f3n, Dhammacakkappavattana",
         "meaning": "Mes de la primera ense\u00f1anza \u2014 la Rueda gira"},
        {"pali_name": "Savana",     "season": "Lluviosa", "observance": "Comienza Vassa \u2014 inicio del Retiro de Lluvias",
         "meaning": "Mes de recogimiento \u2014 los mon\u00e1sticos entran en retiro"},
        {"pali_name": "Bhaddapada", "season": "Lluviosa", "observance": "Vassa contin\u00faa",
         "meaning": "Mes de pr\u00e1ctica devota"},
        {"pali_name": "Assayuja",   "season": "Lluviosa", "observance": "Vassa termina \u2014 ceremonia de Pavarana",
         "meaning": "Mes de la invitaci\u00f3n \u2014 reconciliaci\u00f3n comunitaria"},
        {"pali_name": "Kattika",    "season": "Fresca",   "observance": "Kathina \u2014 ofrenda de t\u00fanicas a los mon\u00e1sticos",
         "meaning": "Mes de la generosidad \u2014 dana y gratitud"},
        {"pali_name": "Maggasira",  "season": "Fresca",   "observance": "",
         "meaning": "Mes del sendero \u2014 caminar en silencio"},
        {"pali_name": "Phussa",     "season": "Fresca",   "observance": "",
         "meaning": "Mes del alimento"},
        {"pali_name": "Magha",      "season": "Fresca",   "observance": "Magha Puja \u2014 D\u00eda de la Sangha, 1250 monjes reunidos",
         "meaning": "Mes de la Sangha \u2014 comunidad del despertar"},
        {"pali_name": "Phalguna",   "season": "Calurosa", "observance": "",
         "meaning": "Mes de la completud \u2014 el ciclo se prepara para renovarse"},
    ]

    for i, m in enumerate(months_es):
        idx = i + 1
        prefix = f"buddhist.month.{idx}"
        d[f"{prefix}.pali_name"] = m["pali_name"]
        d[f"{prefix}.season"] = m["season"]
        d[f"{prefix}.observance"] = m["observance"]
        d[f"{prefix}.meaning"] = m["meaning"]

    # --- Templates ---
    d["buddhist.tpl.glance_uposatha"] = "EB %1$d \u00b7 %2$s \u00b7 %3$s"
    d["buddhist.tpl.glance"] = "EB %1$d \u00b7 %2$s \u00b7 %3$s"
    d["buddhist.tpl.detail"] = "%1$s: %2$s."
    d["buddhist.tpl.detail_uposatha"] = " %1$s. %2$s."
    d["buddhist.tpl.detail_kalpa"] = " Fase de %1$s (%2$s): %3$s."

    return d


# ============================================================
# CHINESE — 12 animals, 5 elements
# ============================================================

def chinese_en():
    d = {}

    animals = [
        {"name": "Rat",     "archetype": "The Strategist",
         "strengths": "Resourceful, quick-witted, adaptable",
         "challenges": "Cunning, restless, manipulative",
         "compatibility": "Dragon, Monkey, Ox",
         "brief": "First in the cycle \u2014 the clever mind that finds opportunity in chaos"},
        {"name": "Ox",      "archetype": "The Steadfast",
         "strengths": "Patient, reliable, methodical",
         "challenges": "Stubborn, rigid, slow to change",
         "compatibility": "Rat, Snake, Rooster",
         "brief": "The strength that plows through \u2014 patient endurance conquers all"},
        {"name": "Tiger",   "archetype": "The Brave",
         "strengths": "Courageous, passionate, confident",
         "challenges": "Reckless, volatile, domineering",
         "compatibility": "Horse, Dog, Pig",
         "brief": "The roar that awakens \u2014 fearless action in service of justice"},
        {"name": "Rabbit",  "archetype": "The Diplomat",
         "strengths": "Elegant, gentle, compassionate",
         "challenges": "Timid, superficial, escapist",
         "compatibility": "Goat, Pig, Dog",
         "brief": "The grace that disarms \u2014 softness as a form of power"},
        {"name": "Dragon",  "archetype": "The Emperor",
         "strengths": "Charismatic, ambitious, visionary",
         "challenges": "Arrogant, demanding, impulsive",
         "compatibility": "Rat, Monkey, Rooster",
         "brief": "The only mythical animal \u2014 the dream of transcendence made manifest"},
        {"name": "Snake",   "archetype": "The Philosopher",
         "strengths": "Wise, intuitive, graceful",
         "challenges": "Secretive, suspicious, possessive",
         "compatibility": "Ox, Rooster, Dragon",
         "brief": "The coil that strikes true \u2014 deep wisdom beneath a still surface"},
        {"name": "Horse",   "archetype": "The Free Spirit",
         "strengths": "Energetic, independent, adventurous",
         "challenges": "Impatient, irresponsible, anxious",
         "compatibility": "Tiger, Goat, Dog",
         "brief": "The gallop that liberates \u2014 freedom as a way of being"},
        {"name": "Goat",    "archetype": "The Artist",
         "strengths": "Creative, gentle, empathetic",
         "challenges": "Indecisive, pessimistic, dependent",
         "compatibility": "Rabbit, Horse, Pig",
         "brief": "The heart that creates \u2014 beauty born from sensitivity"},
        {"name": "Monkey",  "archetype": "The Inventor",
         "strengths": "Clever, playful, versatile",
         "challenges": "Tricky, arrogant, unreliable",
         "compatibility": "Rat, Dragon, Snake",
         "brief": "The mind that plays \u2014 genius disguised as mischief"},
        {"name": "Rooster", "archetype": "The Observer",
         "strengths": "Honest, diligent, perceptive",
         "challenges": "Critical, abrasive, vain",
         "compatibility": "Ox, Snake, Dragon",
         "brief": "The voice that calls truth \u2014 precision in all things"},
        {"name": "Dog",     "archetype": "The Guardian",
         "strengths": "Loyal, honest, protective",
         "challenges": "Anxious, stubborn, judgmental",
         "compatibility": "Tiger, Rabbit, Horse",
         "brief": "The faithfulness that anchors \u2014 loyalty as the highest virtue"},
        {"name": "Pig",     "archetype": "The Generous",
         "strengths": "Kind, generous, optimistic",
         "challenges": "Naive, indulgent, gullible",
         "compatibility": "Rabbit, Goat, Tiger",
         "brief": "The abundance that overflows \u2014 generosity without calculation"},
    ]

    for i, a in enumerate(animals):
        prefix = f"chinese.animal.{i}"
        d[f"{prefix}.name"] = a["name"]
        d[f"{prefix}.archetype"] = a["archetype"]
        d[f"{prefix}.strengths"] = a["strengths"]
        d[f"{prefix}.challenges"] = a["challenges"]
        d[f"{prefix}.compatibility"] = a["compatibility"]
        d[f"{prefix}.brief"] = a["brief"]

    elements = [
        {"name": "Wood",  "nature": "Growth, expansion, benevolence",       "season": "Spring",
         "expression": "Through creativity, vision, and upward movement"},
        {"name": "Fire",  "nature": "Passion, transformation, brilliance",  "season": "Summer",
         "expression": "Through enthusiasm, warmth, and illumination"},
        {"name": "Earth", "nature": "Stability, nourishment, centering",    "season": "Late Summer",
         "expression": "Through grounding, nurture, and practical care"},
        {"name": "Metal", "nature": "Precision, clarity, righteousness",    "season": "Autumn",
         "expression": "Through refinement, letting go, and sharp focus"},
        {"name": "Water", "nature": "Wisdom, depth, adaptability",          "season": "Winter",
         "expression": "Through flow, stillness, and the power of yielding"},
    ]

    for i, e in enumerate(elements):
        prefix = f"chinese.element.{i}"
        d[f"{prefix}.name"] = e["name"]
        d[f"{prefix}.nature"] = e["nature"]
        d[f"{prefix}.season"] = e["season"]
        d[f"{prefix}.expression"] = e["expression"]

    # Polarity labels
    d["chinese.polarity.yang"] = "Yang"
    d["chinese.polarity.yin"] = "Yin"

    # --- Templates ---
    # glance: "{element} {animal} ({polarity}) — {brief}"
    d["chinese.tpl.glance"] = "%1$s %2$s (%3$s) \u2014 %4$s"
    # detail: "{element} {animal}: {archetype}. Strengths: {strengths}. Challenges: {challenges}. Best with: {compat}. Element: {element} ({nature}) — {expression}. Year {cycle} of 60."
    d["chinese.tpl.detail"] = (
        "%1$s %2$s: %3$s. Strengths: %4$s. Challenges: %5$s. "
        "Best with: %6$s. Element: %7$s (%8$s) \u2014 %9$s. "
        "Year %10$d of 60."
    )

    return d


def chinese_es():
    d = {}

    animals_es = [
        {"name": "Rata",    "archetype": "El Estratega",
         "strengths": "Ingenioso, agudo, adaptable",
         "challenges": "Astuto, inquieto, manipulador",
         "compatibility": "Drag\u00f3n, Mono, Buey",
         "brief": "Primero en el ciclo \u2014 la mente astuta que encuentra oportunidad en el caos"},
        {"name": "Buey",    "archetype": "El Constante",
         "strengths": "Paciente, confiable, met\u00f3dico",
         "challenges": "Terco, r\u00edgido, lento al cambio",
         "compatibility": "Rata, Serpiente, Gallo",
         "brief": "La fuerza que ara \u2014 la resistencia paciente conquista todo"},
        {"name": "Tigre",   "archetype": "El Valiente",
         "strengths": "Valiente, apasionado, seguro",
         "challenges": "Temerario, vol\u00e1til, dominante",
         "compatibility": "Caballo, Perro, Cerdo",
         "brief": "El rugido que despierta \u2014 acci\u00f3n sin miedo al servicio de la justicia"},
        {"name": "Conejo",  "archetype": "El Diplom\u00e1tico",
         "strengths": "Elegante, gentil, compasivo",
         "challenges": "T\u00edmido, superficial, escapista",
         "compatibility": "Cabra, Cerdo, Perro",
         "brief": "La gracia que desarma \u2014 la suavidad como forma de poder"},
        {"name": "Drag\u00f3n", "archetype": "El Emperador",
         "strengths": "Carism\u00e1tico, ambicioso, visionario",
         "challenges": "Arrogante, exigente, impulsivo",
         "compatibility": "Rata, Mono, Gallo",
         "brief": "El \u00fanico animal m\u00edtico \u2014 el sue\u00f1o de trascendencia hecho manifiesto"},
        {"name": "Serpiente", "archetype": "El Fil\u00f3sofo",
         "strengths": "Sabio, intuitivo, elegante",
         "challenges": "Secreto, suspicaz, posesivo",
         "compatibility": "Buey, Gallo, Drag\u00f3n",
         "brief": "La espiral que acierta \u2014 sabidur\u00eda profunda bajo una superficie quieta"},
        {"name": "Caballo", "archetype": "El Esp\u00edritu Libre",
         "strengths": "En\u00e9rgico, independiente, aventurero",
         "challenges": "Impaciente, irresponsable, ansioso",
         "compatibility": "Tigre, Cabra, Perro",
         "brief": "El galope que libera \u2014 la libertad como forma de ser"},
        {"name": "Cabra",   "archetype": "El Artista",
         "strengths": "Creativo, gentil, emp\u00e1tico",
         "challenges": "Indeciso, pesimista, dependiente",
         "compatibility": "Conejo, Caballo, Cerdo",
         "brief": "El coraz\u00f3n que crea \u2014 belleza nacida de la sensibilidad"},
        {"name": "Mono",    "archetype": "El Inventor",
         "strengths": "Astuto, juguet\u00f3n, vers\u00e1til",
         "challenges": "Tramposo, arrogante, poco fiable",
         "compatibility": "Rata, Drag\u00f3n, Serpiente",
         "brief": "La mente que juega \u2014 genialidad disfrazada de travesura"},
        {"name": "Gallo",   "archetype": "El Observador",
         "strengths": "Honesto, diligente, perceptivo",
         "challenges": "Cr\u00edtico, \u00e1spero, vanidoso",
         "compatibility": "Buey, Serpiente, Drag\u00f3n",
         "brief": "La voz que proclama verdad \u2014 precisi\u00f3n en todas las cosas"},
        {"name": "Perro",   "archetype": "El Guardi\u00e1n",
         "strengths": "Leal, honesto, protector",
         "challenges": "Ansioso, terco, cr\u00edtico",
         "compatibility": "Tigre, Conejo, Caballo",
         "brief": "La fidelidad que ancla \u2014 la lealtad como la virtud m\u00e1s alta"},
        {"name": "Cerdo",   "archetype": "El Generoso",
         "strengths": "Amable, generoso, optimista",
         "challenges": "Ingenuo, indulgente, cr\u00e9dulo",
         "compatibility": "Conejo, Cabra, Tigre",
         "brief": "La abundancia que desborda \u2014 generosidad sin c\u00e1lculo"},
    ]

    for i, a in enumerate(animals_es):
        prefix = f"chinese.animal.{i}"
        d[f"{prefix}.name"] = a["name"]
        d[f"{prefix}.archetype"] = a["archetype"]
        d[f"{prefix}.strengths"] = a["strengths"]
        d[f"{prefix}.challenges"] = a["challenges"]
        d[f"{prefix}.compatibility"] = a["compatibility"]
        d[f"{prefix}.brief"] = a["brief"]

    elements_es = [
        {"name": "Madera", "nature": "Crecimiento, expansi\u00f3n, benevolencia",    "season": "Primavera",
         "expression": "A trav\u00e9s de la creatividad, la visi\u00f3n y el movimiento ascendente"},
        {"name": "Fuego",  "nature": "Pasi\u00f3n, transformaci\u00f3n, brillantez",  "season": "Verano",
         "expression": "A trav\u00e9s del entusiasmo, la calidez y la iluminaci\u00f3n"},
        {"name": "Tierra", "nature": "Estabilidad, alimento, centramiento",           "season": "Final del Verano",
         "expression": "A trav\u00e9s del arraigo, el cuidado y la atenci\u00f3n pr\u00e1ctica"},
        {"name": "Metal",  "nature": "Precisi\u00f3n, claridad, rectitud",            "season": "Oto\u00f1o",
         "expression": "A trav\u00e9s del refinamiento, el soltar y el enfoque agudo"},
        {"name": "Agua",   "nature": "Sabidur\u00eda, profundidad, adaptabilidad",    "season": "Invierno",
         "expression": "A trav\u00e9s del fluir, la quietud y el poder de ceder"},
    ]

    for i, e in enumerate(elements_es):
        prefix = f"chinese.element.{i}"
        d[f"{prefix}.name"] = e["name"]
        d[f"{prefix}.nature"] = e["nature"]
        d[f"{prefix}.season"] = e["season"]
        d[f"{prefix}.expression"] = e["expression"]

    # Polarity labels
    d["chinese.polarity.yang"] = "Yang"
    d["chinese.polarity.yin"] = "Yin"

    # --- Templates ---
    d["chinese.tpl.glance"] = "%1$s %2$s (%3$s) \u2014 %4$s"
    d["chinese.tpl.detail"] = (
        "%1$s %2$s: %3$s. Fortalezas: %4$s. Desaf\u00edos: %5$s. "
        "Mejor con: %6$s. Elemento: %7$s (%8$s) \u2014 %9$s. "
        "A\u00f1o %10$d de 60."
    )

    return d


# ============================================================
# Main
# ============================================================

def main():
    # Load existing files (or start empty)
    en = {}
    es = {}

    if EN_PATH.exists():
        with open(EN_PATH, "r", encoding="utf-8") as f:
            en = json.load(f)
        print(f"Loaded en.json: {len(en)} existing keys")
    else:
        print("en.json not found, starting fresh")

    if ES_PATH.exists():
        with open(ES_PATH, "r", encoding="utf-8") as f:
            es = json.load(f)
        print(f"Loaded es.json: {len(es)} existing keys")
    else:
        print("es.json not found, starting fresh")

    # Gather new entries
    new_en = {}
    new_es = {}

    systems = [
        ("astronomy",  astronomy_en,  astronomy_es),
        ("islamic",    islamic_en,    islamic_es),
        ("astrology",  astrology_en,  astrology_es),
        ("buddhist",   buddhist_en,   buddhist_es),
        ("chinese",    chinese_en,    chinese_es),
    ]

    for name, en_fn, es_fn in systems:
        en_data = en_fn()
        es_data = es_fn()
        new_en.update(en_data)
        new_es.update(es_data)
        print(f"  {name}: {len(en_data)} EN keys, {len(es_data)} ES keys")

    # Merge into existing
    en_before = len(en)
    es_before = len(es)
    en.update(new_en)
    es.update(new_es)
    en_added = len(en) - en_before
    es_added = len(es) - es_before

    # Write back (sorted keys, UTF-8)
    CONTENT_DIR.mkdir(parents=True, exist_ok=True)

    with open(EN_PATH, "w", encoding="utf-8") as f:
        json.dump(en, f, indent=2, sort_keys=True, ensure_ascii=False)
        f.write("\n")
    print(f"\nWrote en.json: {len(en)} total keys ({en_added} added)")

    with open(ES_PATH, "w", encoding="utf-8") as f:
        json.dump(es, f, indent=2, sort_keys=True, ensure_ascii=False)
        f.write("\n")
    print(f"Wrote es.json: {len(es)} total keys ({es_added} added)")

    # Summary
    print(f"\n--- Summary ---")
    print(f"  New EN keys from 5 modules: {len(new_en)}")
    print(f"  New ES keys from 5 modules: {len(new_es)}")
    print(f"  EN total: {len(en)}")
    print(f"  ES total: {len(es)}")

    # Verify parity
    en_only = set(en.keys()) - set(es.keys())
    es_only = set(es.keys()) - set(en.keys())
    if en_only:
        print(f"\n  WARNING: {len(en_only)} keys in EN but not ES:")
        for k in sorted(en_only)[:10]:
            print(f"    - {k}")
    if es_only:
        print(f"\n  WARNING: {len(es_only)} keys in ES but not EN:")
        for k in sorted(es_only)[:10]:
            print(f"    - {k}")
    if not en_only and not es_only:
        print(f"\n  Parity: EN and ES have identical key sets.")


if __name__ == "__main__":
    main()
