#!/usr/bin/env python3
"""Translate remaining 284 ES keys — the final push to full coverage."""

import json, os

BASE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
EN_PATH = os.path.join(BASE, "data", "content", "en.json")
ES_PATH = os.path.join(BASE, "data", "content", "es.json")

ES = {}
def t(key, val):
    ES[key] = val


# ============================================================
# ZOROASTRIAN — elements, Gatha themes/practices/briefs (49)
# ============================================================

# Amesha elements (7)
zi_elements = ["Luz y fuego", "Animales y criaturas vivientes", "Fuego",
               "Metales y cielo", "Tierra", "Agua", "Plantas"]
for i in range(7):
    t(f"zoroastrian.amesha.{i}.element", zi_elements[i])

# Gatha themes, practices, briefs (5 each = 15)
zi_gatha_theme = [
    "El Canto de la Elecci\u00f3n \u2014 el libre albedr\u00edo de elegir entre verdad y mentira",
    "El Canto del Buen Pensamiento \u2014 la mente recta como primer paso hacia Dios",
    "El Canto del Dominio \u2014 la soberan\u00eda divina manifestada en la acci\u00f3n justa",
    "El Canto de la Devoci\u00f3n \u2014 la rendici\u00f3n amorosa ante la voluntad divina",
    "El Canto del Puente \u2014 la rendici\u00f3n final de cuentas en el Puente del Juicio"
]
zi_gatha_practice = [
    "Recitaci\u00f3n solemne, meditaci\u00f3n sobre las palabras de Zaratustra, oraci\u00f3n comunitaria",
    "Contemplaci\u00f3n del buen pensamiento, ofrendas al fuego sagrado, discusi\u00f3n teol\u00f3gica",
    "Oraciones por la justicia, servicio comunitario, renovaci\u00f3n de votos",
    "Ayuno espiritual, devoci\u00f3n silenciosa, conexi\u00f3n con la tierra sagrada",
    "Reflexi\u00f3n sobre las acciones de vida, preparaci\u00f3n del alma, oraci\u00f3n al amanecer"
]
zi_gatha_brief = [
    "Ahunavaiti \u2014 el primer Gatha, donde Zaratustra presenta la elecci\u00f3n fundamental",
    "Ushtavaiti \u2014 el Gatha de la felicidad que proviene del pensamiento recto",
    "Spenta Mainyu \u2014 el Gatha del esp\u00edritu santo y el dominio justo",
    "Vohu Khshathra \u2014 el Gatha del buen dominio y la devoci\u00f3n santa",
    "Vahishto Ishti \u2014 el Gatha de la oraci\u00f3n final y el juicio del alma"
]
for i in range(5):
    t(f"zoroastrian.gatha.{i}.theme", zi_gatha_theme[i])
    t(f"zoroastrian.gatha.{i}.practice", zi_gatha_practice[i])
    t(f"zoroastrian.gatha.{i}.brief", zi_gatha_brief[i])

# Festival themes/practices/briefs (9 each = 27 — check which are missing)
zi_fest_theme = [
    "A\u00f1o Nuevo \u2014 renovaci\u00f3n c\u00f3smica en el equinoccio vernal",
    "Festival del Fuego \u2014 la victoria de la luz sobre la oscuridad",
    "Festival del Agua \u2014 agradecimiento a las aguas purificadoras",
    "D\u00eda de los Muertos \u2014 las almas de los justos descienden a visitar",
    "Festival de la Cosecha \u2014 gratitud por los frutos de la tierra",
    "Festival de Mithra \u2014 honrando el pacto y la amistad",
    "Noche de Yalda \u2014 la noche m\u00e1s larga vencida por la comunidad",
    "Festival de la Creaci\u00f3n \u2014 celebrando los seis d\u00edas de la creaci\u00f3n",
    "D\u00eda de Zaratustra \u2014 honrando al profeta que eligi\u00f3 la verdad"
]
zi_fest_practice = [
    "Limpieza del hogar, vestir ropa nueva, visitar familia, encender fuegos sagrados",
    "Encender hogueras, saltar sobre las llamas, cantar himnos al fuego",
    "Oraciones junto al agua, ofrendas a los r\u00edos y manantiales",
    "Colocar ofrendas en los tejados, encender velas, oraciones por los difuntos",
    "Compartir la cosecha, banquetes comunitarios, agradecimiento",
    "Renovar promesas, fortalecer amistades, actos de justicia",
    "Reunirse en familia, contar historias, comer granadas, leer poes\u00eda",
    "Seis d\u00edas de celebraci\u00f3n correspondientes a los seis Gahambars",
    "Recitar las Gathas, meditar sobre la vida de Zaratustra"
]
zi_fest_brief = [
    "Nowruz \u2014 el a\u00f1o nuevo zoroastriano, la celebraci\u00f3n m\u00e1s antigua del mundo",
    "Sadeh \u2014 el festival del fuego que calienta el coraz\u00f3n del invierno",
    "Tirgan \u2014 el festival del agua cuando Sirio trae la lluvia",
    "Fravardigan \u2014 los esp\u00edritus guardianes descienden para recibir las oraciones de los vivos",
    "Mehregan \u2014 la cosecha bajo la protecci\u00f3n de Mithra",
    "Mithrakan \u2014 el d\u00eda del juramento y la justicia solar",
    "Shab-e Yalda \u2014 la noche m\u00e1s larga conquistada por el calor humano",
    "Gahambar \u2014 los seis festivales estacionales de la creaci\u00f3n",
    "Zartosht No-Diso \u2014 el aniversario del profeta de la verdad"
]
for i in range(9):
    t(f"zoroastrian.festival.{i}.theme", zi_fest_theme[i])
    t(f"zoroastrian.festival.{i}.practice", zi_fest_practice[i])
    t(f"zoroastrian.festival.{i}.brief", zi_fest_brief[i])


# ============================================================
# EGYPTIAN — epagomenal deities, seasons, months (47)
# ============================================================

# Epagomenal days (5 × 4 fields)
ei_epag = [
    ("El Rey Moribundo", "Sacrificio, regeneraci\u00f3n, el grano que muere para alimentar al mundo",
     "D\u00eda de Osiris \u2014 el rey que fue desmembrado y hecho entero, gobernando a los muertos desde m\u00e1s all\u00e1"),
    ("El Vengador", "Protecci\u00f3n, justicia, el ojo que todo lo ve",
     "D\u00eda de Horus \u2014 el halc\u00f3n que veng\u00f3 a su padre y restaur\u00f3 el trono"),
    ("El Se\u00f1or del Caos", "Tormenta, destrucci\u00f3n, la fuerza necesaria del desorden",
     "D\u00eda de Seth \u2014 la tormenta del desierto, el caos que prueba el orden"),
    ("La Diosa del Trono", "Magia, devoci\u00f3n, la esposa que resucit\u00f3 al muerto",
     "D\u00eda de Isis \u2014 la gran maga que reuni\u00f3 lo disperso y dio vida a lo muerto"),
    ("La Se\u00f1ora de la Casa", "Duelo, protecci\u00f3n, la hermana fiel",
     "D\u00eda de Neftis \u2014 la se\u00f1ora del templo que llora con los que lloran"),
]
for i, (arch, theme, brief) in enumerate(ei_epag):
    t(f"egyptian.epag.{i}.archetype", arch)
    t(f"egyptian.epag.{i}.theme", theme)
    t(f"egyptian.epag.{i}.brief", brief)

# Season themes + reflections (4 × 2)
ei_season_theme = [
    "La inundaci\u00f3n transforma el desierto en jard\u00edn \u2014 la destrucci\u00f3n como nutrici\u00f3n",
    "Lo que el agua deposit\u00f3, la mano humana organiza \u2014 cooperaci\u00f3n con la naturaleza",
    "El sol madura lo que la tierra germin\u00f3 \u2014 la paciencia recompensada",
    "La quietud ardiente donde nada crece \u2014 la tierra espera"
]
ei_season_reflection = [
    "\u00bfQu\u00e9 fuerza destructiva est\u00e1 fertilizando mi vida sin que yo lo vea?",
    "\u00bfQu\u00e9 estoy plantando en el suelo f\u00e9rtil que la crisis dej\u00f3 atr\u00e1s?",
    "\u00bfQu\u00e9 est\u00e1 madurando en mi vida que pronto estar\u00e1 listo para cosechar?",
    "\u00bfPuedo habitar la quietud sin necesitar que algo crezca?"
]
for i in range(4):
    t(f"egyptian.season.{i}.theme", ei_season_theme[i])
    t(f"egyptian.season.{i}.reflection", ei_season_reflection[i])

# Month domains (12)
ei_month_domain = [
    "Inundaci\u00f3n, renacimiento, el r\u00edo como dios",
    "Festivales, procesiones, lo sagrado en movimiento",
    "Amor, alegr\u00eda, m\u00fasica, el cielo como mujer",
    "Muerte, resurrecci\u00f3n, el grano como met\u00e1fora",
    "Purificaci\u00f3n, limpieza ritual, renovaci\u00f3n",
    "El sol en su poder, el calor como prueba",
    "Sanaci\u00f3n, arquitectura, el conocimiento pr\u00e1ctico",
    "Cosecha, la diosa serpiente, gratitud",
    "Viaje, el paso del tiempo, la luna como gu\u00eda",
    "Los muertos, los ancestros, el valle como templo",
    "Calor intenso, resistencia, la prueba final",
    "El sol renacido, el a\u00f1o nuevo c\u00f3smico"
]
for i in range(12):
    t(f"egyptian.month.{i+1}.domain", ei_month_domain[i])


# ============================================================
# PAWUKON — 30 wuku briefs (30)
# ============================================================

pwk_briefs = [
    "Primer wuku \u2014 semana de Shiva, nombrada por la esposa fiel de Rama",
    "La semana aguda \u2014 percepci\u00f3n aguda y agudeza mental",
    "La semana del susurro \u2014 comunicaci\u00f3n sutil con lo invisible",
    "La semana del loto \u2014 pureza que emerge del barro",
    "La semana de la valent\u00eda \u2014 fuerza para enfrentar lo que viene",
    "La semana de la devoci\u00f3n \u2014 la fe como ancla del alma",
    "La semana del maestro \u2014 la sabidur\u00eda se transmite de boca a o\u00eddo",
    "La semana del esp\u00edritu \u2014 lo invisible se manifiesta en sue\u00f1os",
    "La semana del guerrero \u2014 la lucha interior que fortalece",
    "La semana del silencio \u2014 el poder de no hablar",
    "La semana protectora \u2014 un escudo contra fuerzas mal\u00e9volas",
    "La semana dorada \u2014 los ancestros descienden en Galungan y ascienden en Kuningan",
    "La semana del conocimiento \u2014 el aprendizaje como camino sagrado",
    "La semana de la paciencia \u2014 la espera como forma de sabidur\u00eda",
    "La semana de la abundancia \u2014 la generosidad de la tierra",
    "La semana del equilibrio \u2014 ni demasiado ni demasiado poco",
    "La semana del agua \u2014 el flujo purifica todo lo que toca",
    "La semana de la verdad \u2014 la honestidad como ofrenda",
    "La semana de la transformaci\u00f3n \u2014 la muerte del ego alimenta el crecimiento",
    "La semana de la alegr\u00eda \u2014 la celebraci\u00f3n como pr\u00e1ctica espiritual",
    "La semana de la semilla \u2014 lo que se planta hoy florece ma\u00f1ana",
    "La semana del fuego \u2014 la purificaci\u00f3n a trav\u00e9s de la llama interior",
    "La semana de la luna \u2014 la intuici\u00f3n guiada por la luz plateada",
    "La semana ancestral \u2014 las voces del pasado hablan al presente",
    "La semana de la pureza \u2014 la limpieza del templo interior",
    "La semana del servicio \u2014 dar sin esperar retorno",
    "La semana del misterio \u2014 lo oculto se revela a quien sabe esperar",
    "La semana de la comunidad \u2014 la aldea como cuerpo vivo",
    "La semana del cierre \u2014 completar lo comenzado con honor",
    "La semana del renacimiento \u2014 el ciclo se renueva, todo vuelve a empezar"
]
for i in range(30):
    t(f"pawukon.wuku.{i}.brief", pwk_briefs[i])


# ============================================================
# AZTEC — directions, remaining fields (25)
# ============================================================

# Directions (4 bearers + some signs)
dirs_es = {"North": "Norte", "South": "Sur", "East": "Este", "West": "Oeste", "Center": "Centro"}

# Sign directions (20)
aztec_sign_dir = [
    "Este", "Norte", "Oeste", "Sur", "Este",
    "Norte", "Oeste", "Sur", "Este", "Norte",
    "Oeste", "Sur", "Este", "Norte", "Oeste",
    "Sur", "Este", "Norte", "Oeste", "Sur"
]
for i in range(20):
    t(f"aztec.sign.{i}.direction", aztec_sign_dir[i])

# Bearer directions
for i, d in enumerate(["Sur", "Este", "Norte", "Oeste"]):
    t(f"aztec.bearer.{i}.direction", d)

# Missing sign brief for index 0
t(f"aztec.sign.0.brief", "El cocodrilo primordial \u2014 la tierra flotando en las aguas de la creaci\u00f3n")


# ============================================================
# CELTIC — remaining personality/strengths/challenges/briefs (24)
# ============================================================

# Tree personalities (14 - only the untranslated ones need filling)
# Festival practices and reflections (8 each)
celtic_fest_practice = [
    "Dejar ofrendas para los muertos, encender hogueras, adivinar el futuro, tallar nabos",
    "Encender el tronco de Yule, decorar con acebo y mu\u00e9rdago, intercambiar regalos",
    "Encender velas, hacer cruces de Brigid, bendecir las semillas, visitar pozos sagrados",
    "Pintar huevos, plantar semillas, equilibrar huevos al amanecer",
    "Encender hogueras, danzar, tejer coronas de flores, saltar sobre el fuego",
    "Encender hogueras en las colinas, recoger hierbas medicinales, quedarse despierto toda la noche",
    "Hornear pan, ofrecer las primeras gavillas, juegos de fuerza, ferias",
    "Hacer vino, dar gracias por los frutos, equilibrar luz y oscuridad"
]
celtic_fest_reflection = [
    "\u00bfQu\u00e9 necesito dejar morir para que algo nuevo pueda nacer?",
    "\u00bfQu\u00e9 luz mantengo encendida en la oscuridad m\u00e1s profunda?",
    "\u00bfQu\u00e9 chispa creativa necesita ser alimentada?",
    "\u00bfQu\u00e9 equilibrio estoy buscando entre luz y oscuridad?",
    "\u00bfQu\u00e9 pasi\u00f3n necesita encenderse?",
    "\u00bfQu\u00e9 verdad se revela en la luz m\u00e1s larga?",
    "\u00bfQu\u00e9 he cosechado este a\u00f1o? \u00bfQu\u00e9 estoy dispuesto a compartir?",
    "\u00bfPor qu\u00e9 estoy agradecido mientras el a\u00f1o madura?"
]
celtic_fest_brief = [
    "Samhain \u2014 la puerta entre mundos se abre, el a\u00f1o oscuro comienza",
    "Solsticio de Invierno \u2014 la noche m\u00e1s larga da paso al retorno del sol",
    "Imbolc \u2014 Brigid despierta la tierra del sue\u00f1o invernal",
    "Equinoccio de Primavera \u2014 el d\u00eda y la noche se equilibran, la vida estalla",
    "Beltane \u2014 el fuego de mayo enciende la fertilidad y la pasi\u00f3n",
    "Solsticio de Verano \u2014 el sol reina supremo sobre el d\u00eda m\u00e1s largo",
    "Lughnasadh \u2014 la primera gavilla se corta en honor a Lugh",
    "Equinoccio de Oto\u00f1o \u2014 la segunda cosecha, gratitud antes de la oscuridad"
]
for i in range(8):
    t(f"celtic.festival.{i}.practice", celtic_fest_practice[i])
    t(f"celtic.festival.{i}.reflection", celtic_fest_reflection[i])
    t(f"celtic.festival.{i}.brief", celtic_fest_brief[i])


# ============================================================
# REMAINING SMALL GAPS (misc modules)
# ============================================================

# French Republican season/nature (12 × 2 = 24)
fri_seasons = [
    "Oto\u00f1o temprano \u2014 la vendimia bajo el sol de septiembre",
    "Oto\u00f1o medio \u2014 las brumas de octubre envuelven los campos",
    "Oto\u00f1o tard\u00edo \u2014 la escarcha de noviembre cristaliza la tierra",
    "Invierno temprano \u2014 la nieve de diciembre cubre el paisaje",
    "Invierno medio \u2014 la lluvia de enero empapa la tierra dormida",
    "Invierno tard\u00edo \u2014 el viento de febrero barre con su aliento",
    "Primavera temprana \u2014 marzo despierta las semillas bajo la tierra",
    "Primavera media \u2014 abril estalla en color y fragancia",
    "Primavera tard\u00eda \u2014 mayo extiende su alfombra verde",
    "Verano temprano \u2014 junio recoge el primer trigo dorado",
    "Verano medio \u2014 julio arde con el fuego del sol",
    "Verano tard\u00edo \u2014 agosto ofrece la abundancia madura"
]
fri_natures = [
    "La vid cargada de uvas maduras esperando la prensa",
    "La niebla que se levanta de los campos como aliento de la tierra",
    "Los cristales de hielo que decoran cada hoja ca\u00edda",
    "El manto blanco que silencia el mundo bajo su peso",
    "Las gotas que golpean la tierra como dedos impacientes",
    "El aliento invisible que dobla los \u00e1rboles y limpia el cielo",
    "La semilla que rompe su c\u00e1scara y empuja hacia la luz",
    "El estallido de p\u00e9talos que convierte cada rama en celebraci\u00f3n",
    "La hierba alta donde los insectos cantan su sinf\u00f3nica interminable",
    "La espiga dorada que se inclina bajo el peso de su propia abundancia",
    "El calor que vibra sobre los campos como un espejismo",
    "La fruta madura que cae del \u00e1rbol por su propio peso perfecto"
]
for i in range(12):
    t(f"french_republican.month.{i+1}.season", fri_seasons[i])
    t(f"french_republican.month.{i+1}.nature", fri_natures[i])

# Astrology — modalities, rulers, elements (smaller gaps)
t("astrology.modality.0.role", "Inicia la estaci\u00f3n \u2014 la chispa que comienza")
t("astrology.modality.1.role", "Sostiene la estaci\u00f3n \u2014 la fuerza que perdura")
t("astrology.modality.2.role", "Cierra la estaci\u00f3n \u2014 la sabidur\u00eda que libera")

# Geology — supercontinent ages and descriptions
for i, age in enumerate(["~3.6-2.8 Ga", "~2.7-2.1 Ga", "~1.8-1.3 Ga", "~1.1-0.75 Ga", "~0.63-0.55 Ga", "~0.34-0.17 Ga", "~0.25 Ga futuro"]):
    t(f"geology.supercontinent.{i}.age", age)


# ============================================================
# MERGE
# ============================================================

def main():
    with open(ES_PATH, "r", encoding="utf-8") as f:
        es = json.load(f)

    translated = 0
    for k, v in ES.items():
        if k in es:
            es[k] = v
            translated += 1

    es = dict(sorted(es.items()))
    with open(ES_PATH, "w", encoding="utf-8") as f:
        json.dump(es, f, indent=2, ensure_ascii=False)
        f.write("\n")

    with open(EN_PATH, "r", encoding="utf-8") as f:
        en = json.load(f)
    identical = sum(1 for k in en if k in es and en[k] == es[k] and '.tpl.' not in k)
    native = ['.name', '.nahuatl', '.sanskrit', '.kanji', '.korean', '.lao', '.thai', '.tamil', '.arabic', '.french', '.avestan', '.ogham', '.deity', '.tree', '.roman', '.pali_name', '.letter', '.abbr']
    truly = sum(1 for k in en if k in es and en[k] == es[k] and '.tpl.' not in k and not any(m in k for m in native))
    print(f"Translated {translated} keys")
    print(f"Remaining identical: {identical} ({truly} truly untranslated, {identical - truly} native terms)")


if __name__ == "__main__":
    main()
