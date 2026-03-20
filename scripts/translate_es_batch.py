#!/usr/bin/env python3
"""Write real ES translations for modules currently using EN fallback.

Strategy per field type:
- Native names (Sanskrit, Arabic, Hebrew, Nahuatl, etc.) → KEEP as-is
- English proper nouns → KEEP
- Descriptive text (themes, practices, briefs) → TRANSLATE
- Templates → already correct (positional params work for ES)

Run: python3 scripts/translate_es_batch.py
"""

import json
import os

BASE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
EN_PATH = os.path.join(BASE, "data", "content", "en.json")
ES_PATH = os.path.join(BASE, "data", "content", "es.json")

# Translation overrides: key → Spanish value
# Only keys that need REAL translation (not native terms that stay)
ES_OVERRIDES = {}

def t(key, val):
    """Register a translation."""
    ES_OVERRIDES[key] = val

# ============================================================
# AZTEC — 20 day signs + 4 year bearers
# Nahuatl names STAY. English translations + descriptions → Spanish.
# ============================================================

# Signs: english, quality, brief get translated. nahuatl/deity/direction stay.
aztec_sign_english = [
    "Cocodrilo", "Viento", "Casa", "Lagartija", "Serpiente",
    "Muerte", "Venado", "Conejo", "Agua", "Perro",
    "Mono", "Hierba", "Ca\u00f1a", "Jaguar", "\u00c1guila",
    "Buitre", "Movimiento", "Pedernal", "Lluvia", "Flor"
]
aztec_sign_quality = [
    "Iniciaci\u00f3n, nutrici\u00f3n, comienzos primordiales",
    "Aliento divino, esp\u00edritu, comunicaci\u00f3n",
    "Refugio, protecci\u00f3n, el espacio interior sagrado",
    "Fertilidad, abundancia, los sue\u00f1os de la tierra",
    "Fuerza vital, kundalini, energ\u00eda serpentina",
    "Transformaci\u00f3n, renacimiento, el puente entre mundos",
    "Gracia, gentileza, los caminos del bosque",
    "Fertilidad, abundancia, la fuerza lunar",
    "Purificaci\u00f3n, flujo, el r\u00edo de la vida",
    "Lealtad, gu\u00eda, el compa\u00f1ero del inframundo",
    "Artes, juego, el tejido de la vida",
    "Crecimiento, sanaci\u00f3n, lo medicinal",
    "Autoridad, destino, la flecha del tiempo",
    "Poder, sigilo, el coraz\u00f3n de la noche",
    "Visi\u00f3n, libertad, el mensajero del sol",
    "Purificaci\u00f3n, sabidur\u00eda, el limpiador",
    "Terremoto, cambio, el coraz\u00f3n de la tierra",
    "Sacrificio, discernimiento, la hoja de obsidiana",
    "Fertilidad, renovaci\u00f3n, Tl\u00e1loc llora sobre la tierra",
    "Belleza, completitud, la culminaci\u00f3n de la creaci\u00f3n"
]
aztec_sign_brief = [
    "El cocodrilo primordial \u2014 la tierra flotando en las aguas de la creaci\u00f3n",
    "El aliento divino \u2014 Ehecatl gira en los cuatro vientos",
    "La casa sagrada \u2014 el templo del yo interior",
    "El sue\u00f1o de la lagartija \u2014 lo que yace dormido bajo la tierra",
    "La serpiente emplumada desciende \u2014 la energ\u00eda que asciende por la columna",
    "El se\u00f1or de los muertos \u2014 Mictlantecuhtli abre el camino entre mundos",
    "El venado bajo la luna \u2014 gentileza como fortaleza",
    "El conejo en la luna \u2014 la embriaguez divina de la abundancia",
    "El agua que purifica \u2014 Chalchiuhtlicue limpia lo que el fuego no puede",
    "Xolotl gu\u00eda a los muertos \u2014 la lealtad m\u00e1s all\u00e1 de la muerte",
    "Ozomatli teje la creaci\u00f3n \u2014 las artes como pr\u00e1ctica sagrada",
    "La hierba que sana \u2014 la medicina crece donde la miras",
    "La ca\u00f1a del destino \u2014 lo que se planta determina lo que crece",
    "El jaguar en la oscuridad \u2014 Tezcatlipoca ve a trav\u00e9s de la noche",
    "El \u00e1guila asciende \u2014 la visi\u00f3n desde el lugar m\u00e1s alto",
    "El buitre purifica \u2014 lo que parece destrucci\u00f3n es limpieza",
    "Ollin estremece la tierra \u2014 el movimiento es la \u00fanica constante",
    "La hoja de obsidiana corta \u2014 el sacrificio que abre el camino",
    "Tl\u00e1loc derrama lluvia \u2014 la l\u00e1grima del dios alimenta la tierra",
    "Xochitl corona la creaci\u00f3n \u2014 la belleza como destino final"
]

for i in range(20):
    t(f"aztec.sign.{i}.english", aztec_sign_english[i])
    t(f"aztec.sign.{i}.quality", aztec_sign_quality[i])
    t(f"aztec.sign.{i}.brief", aztec_sign_brief[i])

# Year bearers
aztec_bearer_english = ["Ca\u00f1a", "Pedernal", "Casa", "Conejo"]
aztec_bearer_quality = [
    "A\u00f1os del Este \u2014 crecimiento, fertilidad, nuevos emprendimientos",
    "A\u00f1os del Norte \u2014 aridez, desaf\u00edo, prueba espiritual",
    "A\u00f1os del Oeste \u2014 descenso, introspecci\u00f3n, el paso al inframundo",
    "A\u00f1os del Sur \u2014 abundancia, indulgencia, la generosidad del conejo"
]
aztec_bearer_brief = [
    "A\u00f1o de la Ca\u00f1a \u2014 el Este env\u00eda luz y crecimiento al mundo",
    "A\u00f1o del Pedernal \u2014 el Norte trae viento fr\u00edo y purificaci\u00f3n",
    "A\u00f1o de la Casa \u2014 el Oeste abre la puerta al mundo interior",
    "A\u00f1o del Conejo \u2014 el Sur derrama abundancia como pulque de las estrellas"
]

for i in range(4):
    t(f"aztec.bearer.{i}.english", aztec_bearer_english[i])
    t(f"aztec.bearer.{i}.quality", aztec_bearer_quality[i])
    t(f"aztec.bearer.{i}.brief", aztec_bearer_brief[i])

# Aztec templates
t("aztec.tpl.glance", "%1$d %2$s \u2014 A\u00f1o %3$d %4$s")
t("aztec.tpl.detail", "D\u00eda %1$d Signo: %2$s (%3$s). Deidad: %4$s. Direcci\u00f3n: %5$s. %6$s.")
t("aztec.tpl.detail_bearer", " Portador del A\u00f1o: %1$s (%2$s). Direcci\u00f3n: %3$s. %4$s.")


# ============================================================
# PERSIAN — 12 months + 4 festivals
# Persian/Avestan names STAY. Meanings + seasons + briefs → Spanish.
# ============================================================

persian_month_meaning = [
    "Esp\u00edritus guardianes de los justos \u2014 los ancestros protectores",
    "El Amesha Spenta de la verdad, la rectitud y el fuego sagrado",
    "El Amesha Spenta del agua, la salud y la perfecci\u00f3n",
    "El yazata de la lluvia y la estrella Sirio, portadora del mons\u00f3n",
    "El Amesha Spenta de las plantas, la inmortalidad y la eternidad",
    "El Amesha Spenta de los metales, el cielo y el poder justo",
    "El yazata del juramento, la amistad y la justicia del sol",
    "El yazata de las aguas c\u00f3smicas y la purificaci\u00f3n",
    "El yazata del fuego, la llama sagrada de Ahura Mazda",
    "El Creador mismo \u2014 tres d\u00edas de este mes llevan su nombre",
    "El Amesha Spenta del buen pensamiento, el pensar recto y los animales",
    "El Amesha Spenta de la tierra, la devoci\u00f3n y la fe amorosa"
]
persian_month_season = [
    "Primavera temprana \u2014 temporada de Nowruz, a\u00f1o nuevo, renacimiento de la naturaleza",
    "Mediados de primavera \u2014 la tierra se calienta, la verdad se hace visible en lo verde",
    "Primavera tard\u00eda \u2014 las aguas fluyen, plenitud de la estaci\u00f3n",
    "Verano temprano \u2014 el calor comienza, oraciones por lluvia",
    "Pleno verano \u2014 calor en su m\u00e1xima, las plantas en pleno poder",
    "Verano tard\u00edo \u2014 se acerca la cosecha, el reino de la luz",
    "Oto\u00f1o temprano \u2014 equinoccio, balance de luz y oscuridad",
    "Mediados de oto\u00f1o \u2014 regresan las lluvias, se honran las aguas",
    "Oto\u00f1o tard\u00edo \u2014 Yalda se acerca, fuego contra la creciente oscuridad",
    "Invierno temprano \u2014 pasado el solsticio, la presencia del creador en la quietud",
    "Pleno invierno \u2014 contemplaci\u00f3n, la disciplina del buen pensamiento",
    "Invierno tard\u00edo \u2014 Chaharshanbe Suri, saltar sobre el fuego, primavera anticipada"
]
persian_month_brief = [
    "Mes de los esp\u00edritus guardianes \u2014 los ancestros bendicen el comienzo del a\u00f1o nuevo",
    "Mes de la mejor verdad \u2014 el orden c\u00f3smico hecho visible en la naturaleza floreciente",
    "Mes de la totalidad \u2014 la perfecci\u00f3n de las aguas que sustentan toda la vida",
    "Mes de Sirio \u2014 la estrella cuyo orto hel\u00edaco trae la ansiada lluvia",
    "Mes de la inmortalidad \u2014 la fuerza eterna que vive en cada cosa verde",
    "Mes del dominio deseable \u2014 la soberan\u00eda justa que gobierna el cielo",
    "Mes de Mithra \u2014 el se\u00f1or del pacto que vigila cada promesa",
    "Mes de las aguas \u2014 las lluvias purificadoras que renuevan la tierra reseca",
    "Mes del fuego \u2014 la llama sagrada que contiene la noche m\u00e1s larga",
    "Mes del Creador \u2014 el propio mes de Ahura Mazda, cuando la luz comienza su retorno",
    "Mes de la buena mente \u2014 el pensamiento recto que discierne la verdad de la mentira",
    "Mes de la santa devoci\u00f3n \u2014 la tierra sagrada prepar\u00e1ndose para florecer una vez m\u00e1s"
]

for i in range(12):
    t(f"persian.month.{i+1}.meaning", persian_month_meaning[i])
    t(f"persian.month.{i+1}.season", persian_month_season[i])
    t(f"persian.month.{i+1}.brief", persian_month_brief[i])

# Persian festivals
t("persian.festival.1.theme", "D\u00eda Nuevo \u2014 renacimiento del mundo en el equinoccio vernal")
t("persian.festival.1.practice", "Mesa Haft Sin con siete \u00edtems simb\u00f3licos, visitar familia, vestir ropa nueva, peces dorados y trigo germinado")
t("persian.festival.1.brief", "La celebraci\u00f3n continua m\u00e1s antigua de la Tierra \u2014 3000+ a\u00f1os dando la bienvenida a la primavera")
t("persian.festival.2.theme", "D\u00e9cimotercero Afuera \u2014 pasar el 13\u00ba d\u00eda del a\u00f1o nuevo en la naturaleza")
t("persian.festival.2.practice", "Picnic al aire libre, atar nudos de hierba para deseos, soltar el sabzeh germinado en agua corriente")
t("persian.festival.2.brief", "D\u00eda de la Naturaleza \u2014 dejar atr\u00e1s la mala suerte pasando el d\u00eda bajo el cielo abierto")
t("persian.festival.3.theme", "Nacimiento de la luz \u2014 la noche m\u00e1s larga del a\u00f1o vencida por la uni\u00f3n")
t("persian.festival.3.practice", "Permanecer despierto toda la noche en familia, comer granadas y sand\u00eda, leer poes\u00eda de Hafez (fal-e Hafez)")
t("persian.festival.3.brief", "La noche de Yalda \u2014 frutas rojas contra la oscuridad, poes\u00eda contra el silencio")
t("persian.festival.4.theme", "Mi\u00e9rcoles Rojo \u2014 festival del fuego, purificaci\u00f3n antes del a\u00f1o nuevo")
t("persian.festival.4.practice", "Saltar sobre hogueras cantando 'dame tu rojo, toma mi amarillo' (salud por enfermedad), fuegos artificiales")
t("persian.festival.4.brief", "El festival del fuego \u2014 saltar sobre las llamas para quemar la tristeza del a\u00f1o viejo")

# Persian templates
t("persian.tpl.glance", "%1$s %2$d \u2014 %3$s")
t("persian.tpl.detail", "Mes: %1$s (Avest\u00e1n: %2$s). %3$s. %4$s.")
t("persian.tpl.detail_festival", " Festival: %1$s \u2014 %2$s. %3$s.")


# ============================================================
# KOREAN — 7 festivals + 5 elements
# Korean names + Hangul STAY. Themes/practices/briefs → Spanish.
# ============================================================

korean_fest_theme = [
    "Nuevos comienzos \u2014 honrar ancestros, renovar lazos familiares",
    "Plenitud \u2014 la primera luna llena, deseando un a\u00f1o pr\u00f3spero",
    "Llegada de la primavera \u2014 las golondrinas regresan, la naturaleza despierta",
    "Energ\u00eda yang m\u00e1xima \u2014 el d\u00eda m\u00e1s yang del a\u00f1o (5\u00ba d\u00eda, 5\u00ba mes)",
    "Amor y reuni\u00f3n \u2014 la Tejedora y el Vaquero cruzan la V\u00eda L\u00e1ctea",
    "Gratitud \u2014 acci\u00f3n de gracias por la cosecha, abundancia compartida con los ancestros",
    "Longevidad \u2014 ascender a las alturas, apreciaci\u00f3n del crisantemo"
]
korean_fest_practice = [
    "Charye (ritos ancestrales), sebae (reverencia a mayores por bendiciones), tteokguk (sopa de pastel de arroz para ganar un a\u00f1o)",
    "Romper nueces (bureom), comer arroz de cinco granos (ogokbap), contemplar la luna, festivales de fuego (quemar daljip)",
    "Panqueques de flores (hwajeon), paseos primaverales, dar la bienvenida a las golondrinas",
    "Ssireum (lucha), columpiarse, lavar el cabello en agua de iris, surichwitteok (pasteles de arroz con hierbas)",
    "Desear a las estrellas, fideos de harina de trigo (milguksu), ba\u00f1arse en lluvia bendecida por las estrellas",
    "Songpyeon (pasteles de arroz en forma de media luna), charye (ritos ancestrales), visitar tumbas (seongmyo), juegos populares",
    "Escalar monta\u00f1as, vino de crisantemo (gukhwaju), panqueques de crisantemo, poes\u00eda"
]
korean_fest_brief = [
    "A\u00f1o Nuevo Lunar \u2014 la festividad m\u00e1s importante, cuando toda la naci\u00f3n regresa a casa",
    "Gran Luna Llena \u2014 la primera luna llena del a\u00f1o quema la mala suerte",
    "Doble Tercero \u2014 las golondrinas regresan del sur y la primavera se declara",
    "Doble Quinto \u2014 cuando la energ\u00eda yang del cielo es m\u00e1s fuerte y la tierra celebra con lucha",
    "Festival Estelar \u2014 el encuentro anual de los amantes celestiales a trav\u00e9s del puente de urracas",
    "Luna de la Cosecha \u2014 el D\u00eda de Acci\u00f3n de Gracias de Corea, cuando la luna llena oto\u00f1al brilla sobre la abundancia compartida",
    "Doble Noveno \u2014 ascender a terreno alto para ver lejos, beber la flor de la longevidad"
]

for i in range(7):
    t(f"korean.festival.{i}.theme", korean_fest_theme[i])
    t(f"korean.festival.{i}.practice", korean_fest_practice[i])
    t(f"korean.festival.{i}.brief", korean_fest_brief[i])

# Korean elements
korean_el_meaning = [
    "Crecimiento, expansi\u00f3n, benevolencia, la fuerza que asciende",
    "Transformaci\u00f3n, pasi\u00f3n, propiedad, la fuerza que se extiende",
    "Estabilidad, centramiento, confianza, la fuerza que sostiene",
    "Precisi\u00f3n, rectitud, claridad, la fuerza que contrae",
    "Sabidur\u00eda, profundidad, adaptabilidad, la fuerza que desciende"
]
korean_el_brief = [
    "Madera \u2014 la fuerza verde que empuja hacia arriba a trav\u00e9s de todos los obst\u00e1culos",
    "Fuego \u2014 el resplandor que ilumina y transforma",
    "Tierra \u2014 el centro que nutre y enra\u00edza todas las cosas",
    "Metal \u2014 el filo afilado que refina y deja ir",
    "Agua \u2014 la quietud profunda que encuentra su camino a trav\u00e9s de todo"
]

for i in range(5):
    t(f"korean.element.{i}.meaning", korean_el_meaning[i])
    t(f"korean.element.{i}.brief", korean_el_brief[i])

# Korean templates
t("korean.tpl.glance", "Dangun %1$d \u2014 %2$s %3$s (%4$s) \u2014 %5$s")
t("korean.tpl.detail", "A\u00f1o Dangun %1$d. Animal: %2$s. Elemento: %3$s (%4$s). Polaridad: %5$s.")
t("korean.tpl.detail_festival", " Festival: %1$s \u2014 %2$s. %3$s.")


# ============================================================
# EGYPTIAN — seasons + months + epagomenal
# Egyptian names/deity names STAY. Descriptions → Spanish.
# ============================================================

egyptian_season_meaning = [
    "La inundaci\u00f3n \u2014 el Nilo se desborda, trayendo vida",
    "La siembra \u2014 las aguas se retiran, los campos se plantan",
    "La cosecha \u2014 la tierra entrega sus frutos bajo el sol",
    "El calor \u2014 el sol en su cenit, la tierra espera el retorno del agua"
]
egyptian_season_brief = [
    "La estaci\u00f3n de la inundaci\u00f3n \u2014 el regalo del Nilo que hizo posible la civilizaci\u00f3n",
    "La estaci\u00f3n de la siembra \u2014 los campos emergen del limo, la vida brota",
    "La estaci\u00f3n de la cosecha \u2014 la abundancia recogida antes de que regrese el calor",
    "La estaci\u00f3n del calor \u2014 la quietud bajo el sol brillante, esperando a que el r\u00edo vuelva a crecer"
]

for i in range(4):
    t(f"egyptian.season.{i}.meaning", egyptian_season_meaning[i])
    t(f"egyptian.season.{i}.brief", egyptian_season_brief[i])

# Egyptian templates
t("egyptian.tpl.glance", "%1$s %2$d \u2014 %3$s (%4$s)")
t("egyptian.tpl.detail", "Mes: %1$s. Deidad: %2$s. Dominio: %3$s. A\u00f1o S\u00f3tico %4$d.")
t("egyptian.tpl.detail_season", " Estaci\u00f3n: %1$s \u2014 %2$s.")


# ============================================================
# TAROT — 22 Major Arcana + 4 suits
# Card names have established Spanish translations.
# ============================================================

tarot_major_names = [
    "El Loco", "El Mago", "La Sacerdotisa", "La Emperatriz",
    "El Emperador", "El Hierofante", "Los Enamorados", "El Carro",
    "La Fuerza", "El Ermita\u00f1o", "La Rueda de la Fortuna", "La Justicia",
    "El Colgado", "La Muerte", "La Templanza", "El Diablo",
    "La Torre", "La Estrella", "La Luna", "El Sol",
    "El Juicio", "El Mundo"
]
tarot_major_archetype = [
    "El inocente divino", "El transformador consciente", "La guardiana del misterio",
    "La madre creadora", "La estructura del poder", "El puente a lo sagrado",
    "La encrucijada del coraz\u00f3n", "La voluntad en movimiento",
    "El coraje del coraz\u00f3n", "La l\u00e1mpara interior", "El giro eterno",
    "La espada de la verdad", "La rendici\u00f3n al misterio", "La gran transformaci\u00f3n",
    "La alquimia del equilibrio", "El maestro de las cadenas",
    "El rel\u00e1mpago de la verdad", "La esperanza desnuda", "El sue\u00f1o despierto",
    "La alegr\u00eda radiante", "El despertar final", "La danza completa"
]
tarot_major_keyword = [
    "Comienzos", "Manifestaci\u00f3n", "Intuici\u00f3n", "Abundancia",
    "Autoridad", "Tradici\u00f3n", "Elecci\u00f3n", "Determinaci\u00f3n",
    "Coraje", "Soledad", "Destino", "Equilibrio",
    "Rendici\u00f3n", "Transformaci\u00f3n", "Equilibrio", "Atadura",
    "Revelaci\u00f3n", "Esperanza", "Ilusi\u00f3n", "Alegr\u00eda",
    "Renacimiento", "Completitud"
]

for i in range(22):
    t(f"tarot.major.{i}.name", tarot_major_names[i])
    t(f"tarot.major.{i}.archetype", tarot_major_archetype[i])
    t(f"tarot.major.{i}.keyword", tarot_major_keyword[i])

# Tarot suits
t("tarot.suit.0.name", "Bastos")
t("tarot.suit.0.element", "Fuego")
t("tarot.suit.0.domain", "Voluntad, creatividad, emprendimiento, esp\u00edritu")
t("tarot.suit.0.quality", "Energ\u00eda que inicia, inspira y transforma")
t("tarot.suit.1.name", "Copas")
t("tarot.suit.1.element", "Agua")
t("tarot.suit.1.domain", "Emoci\u00f3n, relaciones, intuici\u00f3n, coraz\u00f3n")
t("tarot.suit.1.quality", "Flujo que siente, conecta y nutre")
t("tarot.suit.2.name", "Espadas")
t("tarot.suit.2.element", "Aire")
t("tarot.suit.2.domain", "Mente, verdad, conflicto, comunicaci\u00f3n")
t("tarot.suit.2.quality", "Claridad que corta, analiza y revela")
t("tarot.suit.3.name", "Oros")
t("tarot.suit.3.element", "Tierra")
t("tarot.suit.3.domain", "Materia, cuerpo, trabajo, prosperidad")
t("tarot.suit.3.quality", "Sustancia que construye, sostiene y manifiesta")

# Tarot templates
t("tarot.tpl.glance", "%1$s \u2014 %2$s")
t("tarot.tpl.detail", "%1$s: %2$s. Palabra clave: %3$s. %4$s. Luz: %5$s. Sombra: %6$s.")
t("tarot.tpl.detail_suit", " Palo: %1$s (%2$s). %3$s. %4$s.")


# ============================================================
# CELTIC — 14 trees + 8 festivals
# Tree/Ogham names STAY. Descriptions → Spanish.
# ============================================================

celtic_tree_archetype = [
    "El Portal Entre Mundos", "El Pionero", "El Protector",
    "El \u00c1rbol del Mundo", "El Explorador", "El Instructor",
    "El Aristocr\u00e1tico", "El Rey Antiguo", "El Visionario",
    "La Reina del Bosque", "El Mago", "La Madre Pr\u00f3diga",
    "El Druida", "El Anciano"
]

for i in range(14):
    t(f"celtic.tree.{i}.archetype", celtic_tree_archetype[i])

celtic_fest_name_es = [
    "Samhain", "Solsticio de Invierno", "Imbolc", "Equinoccio de Primavera",
    "Beltane", "Solsticio de Verano", "Lughnasadh", "Equinoccio de Oto\u00f1o"
]
celtic_fest_theme = [
    "El velo se adelgaza \u2014 los muertos caminan, el a\u00f1o oscuro comienza",
    "La noche m\u00e1s larga \u2014 el sol renace, la esperanza persiste",
    "Las primeras se\u00f1ales \u2014 la diosa despierta, las ovejas dan leche",
    "Balance perfecto \u2014 la luz y la oscuridad se igualan, la primavera estalla",
    "El fuego de mayo \u2014 fertilidad, pasi\u00f3n, la uni\u00f3n de lo divino",
    "El sol en su trono \u2014 el d\u00eda m\u00e1s largo, poder en su m\u00e1xima expresi\u00f3n",
    "La primera cosecha \u2014 Lugh honrado, el grano segado, el sacrificio del rey del cereal",
    "La segunda cosecha \u2014 balance nuevamente, gratitud antes de que la oscuridad crezca"
]

for i in range(8):
    t(f"celtic.festival.{i}.theme", celtic_fest_theme[i])

# Celtic templates
t("celtic.tpl.glance", "%1$s (%2$s) D\u00eda %3$d \u2014 %4$s")
t("celtic.tpl.detail", "\u00c1rbol: %1$s (Ogham: %2$s). %3$s. %4$s. Fortalezas: %5$s. Desaf\u00edos: %6$s.")
t("celtic.tpl.detail_festival", " Festival: %1$s \u2014 %2$s. %3$s.")


# ============================================================
# MERGE
# ============================================================

def main():
    with open(EN_PATH, "r", encoding="utf-8") as f:
        en = json.load(f)
    with open(ES_PATH, "r", encoding="utf-8") as f:
        es = json.load(f)

    translated = 0
    for k, v in ES_OVERRIDES.items():
        if k in es:
            es[k] = v
            translated += 1
        else:
            print(f"  WARNING: key '{k}' not found in es.json")

    es = dict(sorted(es.items()))
    with open(ES_PATH, "w", encoding="utf-8") as f:
        json.dump(es, f, indent=2, ensure_ascii=False)
        f.write("\n")

    print(f"Translated {translated} keys in ES")
    # Count remaining untranslated
    identical = sum(1 for k in en if k in es and en[k] == es[k])
    print(f"Remaining identical EN/ES: {identical}")


if __name__ == "__main__":
    main()
