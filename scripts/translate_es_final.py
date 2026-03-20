#!/usr/bin/env python3
"""Final ES translations — all remaining ~696 untranslated narrative keys.

Run: python3 scripts/translate_es_final.py
"""

import json, os

BASE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
EN_PATH = os.path.join(BASE, "data", "content", "en.json")
ES_PATH = os.path.join(BASE, "data", "content", "es.json")

ES = {}
def t(key, val):
    ES[key] = val


# ============================================================
# CONVERGENCE — system names, traditions, what_aligns (79 keys)
# ============================================================

# Strength descriptions + significance
t("convergence.strength.0.description", "Las corrientes del tiempo fluyen de manera ordinaria")
t("convergence.strength.0.significance", "D\u00eda normal \u2014 los ciclos siguen su curso habitual")
t("convergence.strength.1.description", "Varios sistemas se alinean en temas comunes")
t("convergence.strength.1.significance", "Convergencia notable \u2014 la sincronicidad es visible para quienes observan")
t("convergence.strength.2.description", "M\u00faltiples tradiciones apuntan al mismo significado")
t("convergence.strength.2.significance", "Convergencia significativa \u2014 los hilos del tiempo se entrelazan visiblemente")
t("convergence.strength.3.description", "Una rara alineaci\u00f3n entre muchos sistemas de tiempo")
t("convergence.strength.3.significance", "Convergencia extraordinaria \u2014 el tejido mismo del tiempo revela su patr\u00f3n")

# System names/traditions (21 systems)
conv_systems = [
    ("Gregoriano", "Calendario occidental", "Fecha, estaci\u00f3n, d\u00eda de la semana"),
    ("Hebreo", "Tradici\u00f3n lunar jud\u00eda", "Mes hebreo, festividades, ciclo sabat\u00edco"),
    ("Isl\u00e1mico", "Calendario Hijri", "Mes lunar, tiempos de oraci\u00f3n, festividades"),
    ("Chino", "Zodiaco chino", "Animal del a\u00f1o, elemento, polaridad yin/yang"),
    ("Tzolkin", "Cuenta Dreamspell maya", "Sello, tono, onda encantada, castillo"),
    ("I Ching", "Libro de los cambios", "Hexagrama, juicio, imagen, trigramas"),
    ("Dise\u00f1o Humano", "S\u00edntesis moderna", "Tipo, autoridad, perfil, puerta solar"),
    ("Astrolog\u00eda", "Zodiaco tropical", "Signo solar, signo lunar, ascendente"),
    ("Astronom\u00eda", "Mec\u00e1nica celeste", "Posiciones planetarias, fases lunares"),
    ("Budista", "Era Budista", "A\u00f1o EB, d\u00edas Uposatha, fases de kalpa"),
    ("Hindu", "Panchanga v\u00e9dico", "Tithi, nakshatra, yoga"),
    ("Kabbalah", "\u00c1rbol de la Vida", "Sefirah, camino, meditaci\u00f3n"),
    ("Geolog\u00eda", "Tiempo profundo", "E\u00f3n, extinci\u00f3n, supercontinente"),
    ("Persa", "Calendario solar iran\u00ed", "Mes zoroastriano, festivales"),
    ("Copto", "Calendario de los m\u00e1rtires", "Mes copto, fiestas lit\u00fargicas"),
    ("Coreano", "Calendario Dangun", "Animal, elemento, festivales"),
    ("Tailand\u00e9s", "Calendario budista tailand\u00e9s", "Mes, festivales, estaci\u00f3n"),
    ("Et\u00edope", "Calendario et\u00edope", "Mes, fiestas, estaci\u00f3n"),
    ("Tamil", "Calendario sideral tamil", "Mes rasi, festivales"),
    ("Bah\u00e1\u2019\u00ed", "Calendario Bad\u00ed\u2019", "Mes, d\u00edas santos, atributos divinos"),
    ("Japones", "Eras imperiales", "Era, rokuyo, sekki"),
]

for i, (name, trad, align) in enumerate(conv_systems):
    t(f"convergence.system.{i}.system_name", name)
    t(f"convergence.system.{i}.tradition", trad)
    t(f"convergence.system.{i}.what_aligns", align)

# Pattern names + descriptions (8 patterns)
conv_patterns = [
    ("Renacimiento", "M\u00faltiples sistemas se\u00f1alan nuevos comienzos"),
    ("Transformaci\u00f3n", "Temas de muerte y renacimiento convergen"),
    ("Equilibrio", "Los ciclos alcanzan puntos de equilibrio simult\u00e1neamente"),
    ("Cosecha", "Temas de abundancia y gratitud coinciden"),
    ("Purificaci\u00f3n", "M\u00faltiples tradiciones llaman a la limpieza"),
    ("Celebraci\u00f3n", "Festividades de diferentes culturas coinciden"),
    ("Introspecci\u00f3n", "Los ciclos apuntan hacia adentro simult\u00e1neamente"),
    ("Ascenso", "Energ\u00edas de crecimiento y expansi\u00f3n se alinean"),
]

for i, (name, desc) in enumerate(conv_patterns):
    t(f"convergence.pattern.{i}.name", name)
    t(f"convergence.pattern.{i}.description", desc)

# Convergence templates
t("convergence.tpl.glance", "%1$s \u2014 %2$d sistemas alineados")
t("convergence.tpl.detail", "%1$s.")


# ============================================================
# TAROT — remaining light/shadow/brief for 22 Major Arcana (70 keys)
# ============================================================

tarot_light = [
    "Libertad, fe, espontaneidad, vivir el momento",
    "Habilidad, recursos, confianza, el poder de manifestar",
    "Misterio, profundidad, paciencia, sabidur\u00eda silenciosa",
    "Fertilidad, belleza, cuidado maternal, creatividad abundante",
    "Orden, protecci\u00f3n, estructura, visi\u00f3n a largo plazo",
    "Gu\u00eda espiritual, tradici\u00f3n, aprendizaje, puente entre mundos",
    "Amor, armon\u00eda, decisi\u00f3n del coraz\u00f3n, atracci\u00f3n aut\u00e9ntica",
    "Determinaci\u00f3n, victoria, voluntad enfocada, avance",
    "Compasi\u00f3n, valent\u00eda interior, dominar al le\u00f3n con gentileza",
    "Sabidur\u00eda, soledad f\u00e9rtil, b\u00fasqueda de verdad interior",
    "Oportunidad, sincronicidad, ciclos, el destino gira",
    "Verdad, imparcialidad, causa y efecto, claridad moral",
    "Fe, soltar el control, ver desde otra perspectiva",
    "Liberaci\u00f3n, fin necesario, renovaci\u00f3n profunda, desprendimiento",
    "Paciencia, fluidez, alquimia, integrar opuestos",
    "Humor, autoconocimiento, romper cadenas autoimpuestas",
    "Revelaci\u00f3n s\u00fabita, honestidad brutal, liberaci\u00f3n de lo falso",
    "Inspiraci\u00f3n, serenidad, generosidad, fe renovada",
    "Imaginaci\u00f3n, escuchar la intuici\u00f3n, navegar lo desconocido",
    "Vitalidad, claridad, \u00e9xito, la alegr\u00eda de existir",
    "Despertar, perd\u00f3n, renacimiento, la llamada del esp\u00edritu",
    "Integraci\u00f3n, logro, celebraci\u00f3n, el ciclo completo"
]
tarot_shadow = [
    "Imprudencia, negaci\u00f3n, ca\u00edda por ignorar las se\u00f1ales",
    "Manipulaci\u00f3n, enga\u00f1o, habilidad usada para da\u00f1o",
    "Frialdad, secretismo excesivo, desconexion emocional",
    "Sobreprotecci\u00f3n, dependencia, creatividad bloqueada",
    "Tiran\u00eda, rigidez, control excesivo, dominaci\u00f3n",
    "Dogma, conformismo, rechazo de la voz interior",
    "Indecisi\u00f3n, obsesi\u00f3n, relaciones desequilibradas",
    "Agresi\u00f3n, derrota, fuerza bruta sin direcci\u00f3n",
    "Debilidad, crueldad, dominaci\u00f3n por la fuerza",
    "Aislamiento, paranoia, rechazo del mundo",
    "Resistencia al cambio, mala suerte, ciclos repetidos",
    "Parcialidad, injusticia, castigo desproporcionado",
    "Estancamiento, martirio, resistencia in\u00fatil",
    "Resistencia al cambio, aferrarse a lo muerto, estancamiento",
    "Impaciencia, exceso, falta de moderaci\u00f3n",
    "Adicci\u00f3n, materialismo, esclavitud voluntaria",
    "Cat\u00e1strofe, ego destruido, ca\u00edda del pedestal",
    "Desesperanza, desconexion, promesas vac\u00edas",
    "Miedo, confusi\u00f3n, enga\u00f1o, perderse en la ilusi\u00f3n",
    "Ego inflado, quemadura, ceguera por exceso de luz",
    "Culpa, duda, resistirse al despertar",
    "Estancamiento, rechazo a culminar, c\u00edrculo sin fin"
]
tarot_brief = [
    "El Loco \u2014 el salto de fe antes de que comience el viaje",
    "El Mago \u2014 la voluntad consciente que transforma la realidad",
    "La Sacerdotisa \u2014 la guardiana del umbral entre lo visible y lo invisible",
    "La Emperatriz \u2014 la madre creadora que hace florecer la vida",
    "El Emperador \u2014 el arquitecto que construye el orden del mundo",
    "El Hierofante \u2014 el puente entre lo humano y lo sagrado",
    "Los Enamorados \u2014 la encrucijada donde el coraz\u00f3n elige su camino",
    "El Carro \u2014 la voluntad triunfante que avanza contra toda resistencia",
    "La Fuerza \u2014 el coraje gentil que doma al le\u00f3n interior",
    "El Ermita\u00f1o \u2014 la l\u00e1mpara solitaria que ilumina el camino interior",
    "La Rueda \u2014 el giro eterno del destino, nada permanece igual",
    "La Justicia \u2014 la espada que corta con verdad y la balanza que pesa con precisi\u00f3n",
    "El Colgado \u2014 la rendici\u00f3n que revela lo que la lucha oculta",
    "La Muerte \u2014 el fin necesario que hace posible todo comienzo",
    "La Templanza \u2014 el \u00e1ngel que mezcla fuego y agua sin destruir ninguno",
    "El Diablo \u2014 el espejo que muestra las cadenas que t\u00fa mismo forjaste",
    "La Torre \u2014 el rayo que destruye todo lo que no era verdadero",
    "La Estrella \u2014 la esperanza desnuda que brilla despu\u00e9s de la destrucci\u00f3n",
    "La Luna \u2014 el sue\u00f1o despierto donde nada es lo que parece",
    "El Sol \u2014 la alegr\u00eda pura de existir bajo la luz de la verdad",
    "El Juicio \u2014 la trompeta que despierta lo que estaba dormido",
    "El Mundo \u2014 la danza completada, el c\u00edrculo que se cierra en plenitud"
]

for i in range(22):
    t(f"tarot.major.{i}.light", tarot_light[i])
    t(f"tarot.major.{i}.shadow", tarot_shadow[i])
    t(f"tarot.major.{i}.brief", tarot_brief[i])

# Suit briefs
t("tarot.suit.0.brief", "Bastos \u2014 el fuego de la voluntad que crea y destruye")
t("tarot.suit.1.brief", "Copas \u2014 el agua del coraz\u00f3n que siente y conecta")
t("tarot.suit.2.brief", "Espadas \u2014 el aire de la mente que corta y revela")
t("tarot.suit.3.brief", "Oros \u2014 la tierra del cuerpo que construye y sostiene")


# ============================================================
# ETHIOPIAN — seasons, feasts, remaining (51 keys)
# ============================================================

eth_month_season = [
    "Fin de la estaci\u00f3n lluviosa (Kiremt), las flores cubren las tierras altas",
    "Estaci\u00f3n de siembra post-lluvia, la tierra est\u00e1 rica y lista",
    "Posibles lluvias cortas, los d\u00edas se acortan, preparaci\u00f3n de cosecha",
    "Comienza la estaci\u00f3n seca, preparativos para Navidad (Genna)",
    "Plena estaci\u00f3n seca, celebraciones de Timkat (Epifan\u00eda)",
    "Estaci\u00f3n seca, semillas germinando de la siembra anterior",
    "Las lluvias peque\u00f1as (Belg) pueden comenzar en algunas regiones",
    "Lluvias Belg, temporada de Pascua, la tierra verdece",
    "Fuerte crecimiento vegetativo, se acercan las lluvias grandes",
    "Comienza Kiremt (estaci\u00f3n lluviosa principal), mons\u00f3n fuerte",
    "Las lluvias m\u00e1s intensas, algunas inundaciones, tierras altas verdes y exuberantes",
    "Las lluvias comienzan a disminuir, preparaci\u00f3n para el a\u00f1o nuevo",
    "Cinco o seis d\u00edas extra entre a\u00f1os, transici\u00f3n sagrada"
]
eth_month_meaning = [
    "A\u00f1o nuevo, comienzo", "Siembra, sembrando", "Reuni\u00f3n, oscurecimiento",
    "Alegr\u00eda, celebraci\u00f3n", "Siembra de semilla, el sol", "Germinaci\u00f3n",
    "Se acerca la cosecha", "La bendici\u00f3n", "Crecimiento fuerte",
    "La gran lluvia", "Lluvia m\u00e1xima, inundaci\u00f3n", "Fin de la lluvia, secado",
    "D\u00edas epagomenales, el mes peque\u00f1o"
]
eth_feast_theme = [
    "A\u00f1o Nuevo \u2014 regalo de joyas, el retorno de la Reina de Saba",
    "Hallazgo de la Vera Cruz \u2014 la luz triunfa sobre la oscuridad",
    "Navidad \u2014 la Natividad, precedida por 43 d\u00edas de ayuno",
    "Epifan\u00eda \u2014 el Bautismo de Cristo, la celebraci\u00f3n m\u00e1s sagrada"
]
eth_feast_practice = [
    "Margaritas amarillas (adey abeba), hogueras (chibo), cantar puerta por puerta, ropa nueva, banquete",
    "Ceremonia de hoguera Demera, margaritas amarillas en patr\u00f3n de cruz, procesiones con sacerdotes y tambores",
    "Servicio religioso toda la noche, romper el ayuno, genna (hockey), shammas blancos, banquete de injera",
    "Festival de tres d\u00edas, procesiones del tabot (r\u00e9plica del Arca), bendici\u00f3n de aguas, multitudes gozosas de blanco"
]

for i in range(13):
    t(f"ethiopian.month.{i+1}.season", eth_month_season[i])
    t(f"ethiopian.month.{i+1}.meaning", eth_month_meaning[i])
for i in range(4):
    t(f"ethiopian.feast.{i+1}.theme", eth_feast_theme[i])
    t(f"ethiopian.feast.{i+1}.practice", eth_feast_practice[i])

t("ethiopian.tpl.glance", "%1$s %2$d \u2014 %3$s")
t("ethiopian.tpl.detail", "Mes: %1$s. Significado: %2$s. %3$s.")
t("ethiopian.tpl.detail_feast", " Fiesta: %1$s \u2014 %2$s. %3$s.")


# ============================================================
# THAI + LAO + MYANMAR — seasons, festivals, briefs
# ============================================================

# Thai month seasons (12)
thai_seasons = [
    "Plena estaci\u00f3n fresca \u2014 el mes del capricornio, cosecha de cultivos de estaci\u00f3n fr\u00eda",
    "Estaci\u00f3n fresca menguante \u2014 el mes del acuario, festividades del A\u00f1o Nuevo Chino",
    "Comienza la estaci\u00f3n calurosa \u2014 el mes de piscis, el calor seco crece",
    "Plena estaci\u00f3n calurosa \u2014 el mes de aries, el festival de agua Songkran",
    "Fin de la estaci\u00f3n calurosa \u2014 el mes de tauro, anticipaci\u00f3n del mons\u00f3n",
    "Comienza la estaci\u00f3n lluviosa \u2014 el mes de g\u00e9minis, primeras lluvias monz\u00f3nicas",
    "Estaci\u00f3n lluviosa \u2014 el mes de c\u00e1ncer, comienza la Cuaresma Budista (Khao Phansa)",
    "Plena estaci\u00f3n lluviosa \u2014 el mes de leo, D\u00eda de la Madre (cumplea\u00f1os de la Reina)",
    "Estaci\u00f3n lluviosa tard\u00eda \u2014 el mes de virgo, los arrozales verdes",
    "Estaci\u00f3n lluviosa menguante \u2014 el mes de libra, termina la Cuaresma (Ok Phansa)",
    "Comienza la estaci\u00f3n fresca \u2014 el mes de escorpio, festival Loy Krathong",
    "Estaci\u00f3n fresca \u2014 el mes de sagitario, D\u00eda del Padre (cumplea\u00f1os del Rey)"
]
thai_briefs = [
    "Enero \u2014 nombrado por Capricornio, la estaci\u00f3n fresca alcanza su profundidad",
    "Febrero \u2014 nombrado por Acuario, el \u00faltimo aliento fresco antes del calor",
    "Marzo \u2014 nombrado por Piscis, la transici\u00f3n de lo fresco a lo abrasador",
    "Abril \u2014 nombrado por Aries, el mes m\u00e1s caluroso refrescado por el agua de Songkran",
    "Mayo \u2014 nombrado por Tauro, la tierra espera la lluvia",
    "Junio \u2014 nombrado por G\u00e9minis, el mons\u00f3n se anuncia",
    "Julio \u2014 nombrado por C\u00e1ncer, los monjes entran al retiro de lluvias",
    "Agosto \u2014 nombrado por Leo, las lluvias se intensifican",
    "Septiembre \u2014 nombrado por Virgo, los arrozales brillan esmeralda",
    "Octubre \u2014 nombrado por Libra, el equilibrio regresa cuando las lluvias ceden",
    "Noviembre \u2014 nombrado por Escorpio, las linternas flotantes honran a la diosa del r\u00edo",
    "Diciembre \u2014 nombrado por Sagitario, el regalo de la estaci\u00f3n fresca"
]

for i in range(12):
    t(f"thai.month.{i+1}.season", thai_seasons[i])
    t(f"thai.month.{i+1}.brief", thai_briefs[i])

# Thai festivals
t("thai.festival.1.theme", "Agua, renovaci\u00f3n, respeto \u2014 la limpieza del A\u00f1o Nuevo tailand\u00e9s")
t("thai.festival.1.practice", "Verter agua sobre im\u00e1genes de Buda y manos de mayores, guerras de agua en las calles, construir stupas de arena en templos")
t("thai.festival.1.brief", "El festival del agua \u2014 tres d\u00edas en que Tailandia lava el a\u00f1o viejo con alegr\u00eda y reverencia")
t("thai.festival.2.theme", "Dinast\u00eda, gratitud \u2014 honrando la fundaci\u00f3n de la casa real Chakri")
t("thai.festival.2.practice", "Ceremonia en el Gran Palacio, rendir respeto a reyes pasados, feriado nacional")
t("thai.festival.2.brief", "D\u00eda Conmemorativo Chakri \u2014 honrando al Rey Rama I que fund\u00f3 Bangkok y la dinast\u00eda")
t("thai.festival.3.theme", "Democracia, naci\u00f3n \u2014 conmemorando la primera constituci\u00f3n de Tailandia")
t("thai.festival.3.practice", "Ceremonias nacionales, feriado p\u00fablico, reflexi\u00f3n sobre principios democr\u00e1ticos")
t("thai.festival.3.brief", "D\u00eda de la Constituci\u00f3n \u2014 recordando la transici\u00f3n de monarqu\u00eda absoluta a constitucional")

t("thai.tpl.glance", "EB %1$d %2$s %3$d \u2014 %4$s")
t("thai.tpl.detail", "EB %1$d. Mes: %2$s (del s\u00e1nscrito %3$s). %4$s.")
t("thai.tpl.detail_festival", " Festival: %1$s \u2014 %2$s. %3$s.")

# Lao month seasons + briefs (12 each)
lao_seasons = [
    "Plena estaci\u00f3n fresca \u2014 el mes del capricornio, nieblas del Mekong, templos tranquilos",
    "Estaci\u00f3n fresca menguante \u2014 el mes del acuario, Boun Pha Vet (J\u00e1taka Vessantara)",
    "Comienza estaci\u00f3n calurosa \u2014 el mes de piscis, calor seco sobre arrozales",
    "Plena estaci\u00f3n calurosa \u2014 el mes de aries, festival de agua Pi Mai Lao",
    "Fin de estaci\u00f3n calurosa \u2014 el mes de tauro, cohetes Boun Bang Fai llaman la lluvia",
    "Comienza estaci\u00f3n lluviosa \u2014 el mes de g\u00e9minis, Visakha Boucha (triple aniversario)",
    "Estaci\u00f3n lluviosa \u2014 el mes de c\u00e1ncer, Khao Phansa (comienza Cuaresma Budista)",
    "Plena estaci\u00f3n lluviosa \u2014 el mes de leo, trasplante de arroz, el Mekong crece",
    "Estaci\u00f3n lluviosa tard\u00eda \u2014 el mes de virgo, Khao Phadap Din (honrar ancestros)",
    "Fin de estaci\u00f3n lluviosa \u2014 el mes de libra, Ok Phansa (fin de Cuaresma), carreras de botes",
    "Comienza estaci\u00f3n fresca \u2014 el mes de escorpio, Boun That Luang en la gran stupa",
    "Estaci\u00f3n fresca \u2014 el mes de sagitario, D\u00eda Nacional (2 de diciembre), cierre del a\u00f1o"
]
lao_briefs = [
    "Enero \u2014 nombrado por Capricornio, la estaci\u00f3n fresca envuelve los valles del r\u00edo",
    "Febrero \u2014 nombrado por Acuario, los monjes cantan el relato del \u00faltimo nacimiento de Buda",
    "Marzo \u2014 nombrado por Piscis, la tierra espera entre cosechas",
    "Abril \u2014 nombrado por Aries, Nang Sangkhan desciende mientras Laos lava el a\u00f1o viejo",
    "Mayo \u2014 nombrado por Tauro, las aldeas lanzan cohetes para invocar el mons\u00f3n",
    "Junio \u2014 nombrado por G\u00e9minis, se honran el nacimiento, despertar y parinibbana de Buda",
    "Julio \u2014 nombrado por C\u00e1ncer, los monjes entran al retiro de tres meses de lluvias",
    "Agosto \u2014 nombrado por Leo, el mons\u00f3n alimenta el r\u00edo y los arrozales",
    "Septiembre \u2014 nombrado por Virgo, ofrendas en templos para los difuntos",
    "Octubre \u2014 nombrado por Libra, botes de fuego Lai Heua Fai flotan por el Mekong",
    "Noviembre \u2014 nombrado por Escorpio, la naci\u00f3n se re\u00fane en That Luang en Vienti\u00e1n",
    "Diciembre \u2014 nombrado por Sagitario, Laos celebra su fundaci\u00f3n y el fresco regresa"
]
lao_fest_theme = [
    "Agua, renovaci\u00f3n, baci \u2014 la limpieza del A\u00f1o Nuevo laosiano",
    "Soberan\u00eda, memoria \u2014 fundaci\u00f3n de la Rep\u00fablica Democr\u00e1tica Popular Lao",
    "Libertad, historia \u2014 liberaci\u00f3n del dominio colonial franc\u00e9s"
]
lao_fest_practice = [
    "Verter agua sobre im\u00e1genes de Buda y mayores, ceremonia baci de atado de hilos, stupas de arena en templos, procesi\u00f3n de la doncella Nang Sangkhan en Luang Prabang",
    "Ceremonias nacionales, feriado p\u00fablico, reflexi\u00f3n sobre la independencia (2 de diciembre de 1975)",
    "Ceremonias y recuerdo de la declaraci\u00f3n de independencia Lao Issara (12 de octubre de 1945)"
]
lao_fest_brief = [
    "Pi Mai \u2014 tres d\u00edas en que Laos lava el a\u00f1o viejo: Sangkhan Luang (despedida), Sangkhan Nao (el d\u00eda entre), Sangkhan Khuen (nuevo amanecer)",
    "D\u00eda Nacional \u2014 honrando el establecimiento de la RDP Lao y el largo camino a la soberan\u00eda",
    "D\u00eda de la Liberaci\u00f3n \u2014 el momento en que Laos declar\u00f3 su independencia de la administraci\u00f3n colonial"
]

for i in range(12):
    t(f"lao.month.{i+1}.season", lao_seasons[i])
    t(f"lao.month.{i+1}.brief", lao_briefs[i])
for i in range(3):
    t(f"lao.festival.{i+1}.theme", lao_fest_theme[i])
    t(f"lao.festival.{i+1}.practice", lao_fest_practice[i])
    t(f"lao.festival.{i+1}.brief", lao_fest_brief[i])

t("lao.tpl.glance", "EB %1$d %2$s %3$d \u2014 %4$s")
t("lao.tpl.detail", "EB %1$d. Mes: %2$s (del s\u00e1nscrito %3$s). %4$s.")
t("lao.tpl.detail_festival", " Festival: %1$s \u2014 %2$s. %3$s.")

# Myanmar month seasons, festivals, briefs (39 keys)
mm_seasons = [
    "Estaci\u00f3n calurosa, A\u00f1o Nuevo de Myanmar (mediados de abril)",
    "Mes m\u00e1s caluroso, pre-mons\u00f3n", "Mons\u00f3n temprano, primeras lluvias",
    "Mons\u00f3n, comienza la Cuaresma Budista", "Lluvias monz\u00f3nicas intensas",
    "Mons\u00f3n tard\u00edo, inundaciones cediendo", "Post-mons\u00f3n, fin de Cuaresma",
    "Comienza estaci\u00f3n fresca", "Estaci\u00f3n fresca, clima agradable",
    "Mes m\u00e1s fr\u00edo", "Transici\u00f3n de fresco a c\u00e1lido", "Pre-verano, calent\u00e1ndose"
]
mm_festivals = [
    "Festival de agua Thingyan, celebraciones de A\u00f1o Nuevo de Myanmar",
    "Regar el \u00e1rbol Bodhi (luna llena de Kason, iluminaci\u00f3n de Buda)",
    "Ex\u00e1menes budistas, el estudio del Dhamma se intensifica",
    "Luna llena de Waso: inicio de Vassa (Cuaresma), ofrenda de t\u00fanicas a monjes",
    "Sorteo de t\u00fanicas, propiciaci\u00f3n de esp\u00edritus, festivales nat",
    "Festivales de carreras de botes en r\u00edos y lagos",
    "Festival de Luces (luna llena de Thadingyut), fin de Vassa, festivales de iluminaci\u00f3n",
    "Festival de luces Tazaungdaing, competencias de tejido de t\u00fanicas",
    "Festivales de esp\u00edritus nat, m\u00fasica y danza tradicional",
    "Festival de la cosecha, banquetes de aldea, eventos ecuestres",
    "Festival de htamane (arroz pegajoso), cocina comunitaria",
    "Construcci\u00f3n de pagodas de arena, festival de la pagoda Shwedagon"
]
mm_briefs = [
    "Primer mes \u2014 el gran festival del agua lava el a\u00f1o viejo",
    "Mes del \u00e1rbol Bodhi \u2014 agua vertida sobre \u00e1rboles sagrados en la luna llena",
    "Mes de estudio \u2014 las lluvias comienzan y los monjes profundizan su aprendizaje",
    "Mes de Cuaresma \u2014 Vassa comienza, los monjes se retiran por tres meses de lluvia",
    "Mes de sorteo \u2014 las comunidades sortean para ofrecer t\u00fanicas durante las lluvias",
    "Mes de carreras de botes \u2014 los r\u00edos est\u00e1n llenos y los botes compiten sobre las aguas crecidas",
    "Mes de luces \u2014 Buda desciende del cielo, el mundo se ilumina para darle la bienvenida",
    "Mes de tejido \u2014 competencias nocturnas de tejido de t\u00fanicas a la luz de la luna llena",
    "Mes de esp\u00edritus nat \u2014 los esp\u00edritus son honrados con m\u00fasica y ofrendas",
    "Mes de la cosecha \u2014 el arroz se recoge y las aldeas celebran con juegos",
    "Mes del arroz pegajoso \u2014 las comunidades se re\u00fanen a cocinar htamane en grandes ollas",
    "Mes de pagodas \u2014 stupas de arena en las orillas del r\u00edo, el gran festival de Shwedagon"
]

for i in range(12):
    t(f"myanmar.month.{i}.season", mm_seasons[i])
    t(f"myanmar.month.{i}.festival", mm_festivals[i])
    t(f"myanmar.month.{i}.brief", mm_briefs[i])

t("myanmar.tpl.glance", "%1$s %2$d \u2014 %3$s")
t("myanmar.tpl.detail", "Mes: %1$s. %2$s. Festival: %3$s.")
t("myanmar.tpl.detail_thingyan", " THINGYAN: el gran festival del agua.")
t("myanmar.year_type.0", "Com\u00fan (354 d\u00edas)")
t("myanmar.year_type.1", "Watat Peque\u00f1o (384 d\u00edas)")
t("myanmar.year_type.2", "Watat Grande (385 d\u00edas)")


# ============================================================
# TAMIL — rasi, seasons, festivals (48 keys)
# ============================================================

tamil_rasi = [
    "Mesha (Aries)", "Rishabha (Tauro)", "Mithuna (G\u00e9minis)",
    "Kataka (C\u00e1ncer)", "Simha (Leo)", "Kanya (Virgo)",
    "Thula (Libra)", "Vrischika (Escorpio)", "Dhanus (Sagitario)",
    "Makara (Capricornio)", "Kumbha (Acuario)", "Meena (Piscis)"
]
tamil_season = [
    "Estaci\u00f3n calurosa, A\u00f1o Nuevo Tamil", "Calor m\u00e1ximo, festivales de templos",
    "Inicio del mons\u00f3n suroeste", "Mons\u00f3n, mes auspicioso",
    "Mons\u00f3n tard\u00edo, preparativos de cosecha", "Post-mons\u00f3n, temporada de Navaratri",
    "Comienza mons\u00f3n noreste", "Festival de luces, comienza estaci\u00f3n fresca",
    "Estaci\u00f3n fresca, devociones del amanecer", "Festival de cosecha Pongal",
    "Estaci\u00f3n fresca tard\u00eda", "Pre-verano, matrimonios en templos"
]
tamil_brief = [
    "Primer mes \u2014 el Sol entra en Aries sideral y el a\u00f1o tamil comienza",
    "Mes de celebraciones templarias \u2014 el rasi del toro, firme bajo el sol abrasador",
    "El mes de los gemelos \u2014 el mons\u00f3n llega y la tierra reseca bebe",
    "Mes de Aadi \u2014 el rasi del cangrejo, plenitud monz\u00f3nica, Aadi Perukku (festival del r\u00edo)",
    "Mes del le\u00f3n \u2014 Avani Avittam (renovaci\u00f3n del hilo sagrado) y abundancia monz\u00f3nica",
    "Mes de Navaratri \u2014 nueve noches de la diosa, el rasi de la doncella",
    "Mes de la balanza \u2014 temporada de Deepavali, equilibrio entre luz y oscuridad",
    "Mes de Karthigai \u2014 el festival de l\u00e1mparas, las Pl\u00e9yades en el cenit",
    "Mes m\u00e1s sagrado \u2014 kolams de harina de arroz Margazhi al amanecer, el rasi del arquero",
    "Mes de Pongal \u2014 la fiesta de la cosecha, comienza el viaje del Sol hacia el norte",
    "Mes del aguador \u2014 Maha Shivaratri, la gran noche de Shiva",
    "\u00daltimo mes \u2014 Panguni Uthiram (bodas divinas), el rasi de piscis cierra el a\u00f1o"
]
tamil_fest_theme = [
    "A\u00f1o Nuevo \u2014 el Sol sideral entra en Aries, comienzos auspiciosos",
    "Acci\u00f3n de gracias por la cosecha \u2014 gratitud al Sol, las vacas y la tierra",
    "Matrimonio divino \u2014 la boda c\u00f3smica de Meenakshi y Sundareswarar",
    "Victoria de la luz \u2014 triunfo sobre la oscuridad, retorno de la rectitud"
]
tamil_fest_practice = [
    "Ver el kanni (bandeja auspiciosa), mango pachadi (plato dulce-agrio-amargo que simboliza los sabores de la vida), visitar templos",
    "Hervir arroz en ollas nuevas hasta que rebose (pongal = desbordamiento), decorar vacas, arte kolam, hogueras",
    "Festival de diez d\u00edas en Madurai, procesi\u00f3n de carrozas, recreaci\u00f3n del matrimonio divino",
    "L\u00e1mparas de aceite, petardos, ropa nueva, dulces, ba\u00f1o de aceite al amanecer, reuniones familiares"
]
tamil_fest_brief = [
    "A\u00f1o Nuevo Tamil \u2014 1 de Chittirai, cuando el Sol sideral cruza a Mesha",
    "El festival de la cosecha \u2014 cuatro d\u00edas en el mes Thai cuando la abundancia se celebra con ollas desbordantes",
    "La boda celestial \u2014 el gran festival de Madurai celebrando a la diosa que despos\u00f3 a Shiva",
    "El festival de las luces \u2014 cada l\u00e1mpara declara que la oscuridad no puede perdurar"
]

for i in range(12):
    t(f"tamil.month.{i+1}.rasi", tamil_rasi[i])
    t(f"tamil.month.{i+1}.season", tamil_season[i])
    t(f"tamil.month.{i+1}.brief", tamil_brief[i])
for i in range(4):
    t(f"tamil.festival.{i+1}.theme", tamil_fest_theme[i])
    t(f"tamil.festival.{i+1}.practice", tamil_fest_practice[i])
    t(f"tamil.festival.{i+1}.brief", tamil_fest_brief[i])

t("tamil.tpl.glance", "%1$s %2$d \u2014 %3$s")
t("tamil.tpl.detail", "Mes: %1$s (Rasi: %2$s). %3$s. A\u00f1o joviano %4$d de 60.")
t("tamil.tpl.detail_festival", " Festival: %1$s \u2014 %2$s. %3$s.")


# ============================================================
# COPTIC — remaining origin, liturgy fields (38 keys)
# ============================================================

cop_origin = [
    "Del egipcio Thoth (Djehuty), dios de la sabidur\u00eda y la escritura",
    "Del egipcio Ipet (festival Opet), procesi\u00f3n de la barca sagrada",
    "Del egipcio Hathor, diosa del amor, la alegr\u00eda y el cielo",
    "Del egipcio Ka-her-Ka (alma sobre alma), misterios de Osiris",
    "Del egipcio Djebaty, asociado con la purificaci\u00f3n",
    "Del egipcio Mekhir, mes del sol ardiente",
    "Del egipcio Pa-en-Amenhotep (Imhotep), arquitecto-sanador",
    "Del egipcio Pa-en-Renenutet, diosa de la cosecha",
    "Del egipcio Pa-en-Khonsu, dios luna del paso",
    "Del egipcio Pa-en-Inet (festival del valle), honrando a los muertos",
    "Del egipcio Ipip, asociado con el gran calor",
    "Del egipcio Wep-Renpet (nacimiento de Ra), el sol renacido",
    "D\u00edas epagomenales (griego epagomenai), fuera de los meses regulares"
]
cop_liturgy = [
    "Inicio del a\u00f1o copto, conmemoraci\u00f3n de los m\u00e1rtires",
    "Estaci\u00f3n de siembra, celebraciones parroquiales",
    "Comienza la preparaci\u00f3n del Adviento en la tradici\u00f3n copta",
    "Ayuno de Adviento copto, preparaci\u00f3n para Navidad (29 Koiak)",
    "Temporada de Teofan\u00eda/Epifan\u00eda, celebraciones bautismales",
    "Tiempo ordinario, festividades de santos locales parroquiales",
    "La Anunciaci\u00f3n (29 Paremhat), fiesta de la Theotokos",
    "La Gran Cuaresma se profundiza, se acerca la Semana Santa",
    "Temporada pascual, Pascua y su resplandor",
    "Comienza el Ayuno de los Ap\u00f3stoles, honrando a Pedro y Pablo",
    "Apogeo del verano, conmemoraci\u00f3n de santos y m\u00e1rtires",
    "Ayuno y fiesta de la Asunci\u00f3n de la Virgen Mar\u00eda",
    "D\u00edas de oraci\u00f3n y reflexi\u00f3n entre a\u00f1os, sin ayuno"
]
cop_feast_theme = [
    "A\u00f1o Nuevo de los M\u00e1rtires \u2014 recuerdo de quienes murieron por la fe",
    "Descubrimiento de la Vera Cruz por la Emperatriz Helena en Jerusal\u00e9n",
    "La Natividad de Cristo \u2014 culminaci\u00f3n del ayuno de Adviento de 43 d\u00edas",
    "Teofan\u00eda \u2014 el Bautismo de Cristo en el Jord\u00e1n, Dios revelado",
    "El \u00e1ngel Gabriel anuncia a la Virgen Mar\u00eda \u2014 la Encarnaci\u00f3n comienza",
    "La Resurrecci\u00f3n de Cristo \u2014 la fiesta de fiestas, alegr\u00eda de alegr\u00edas"
]
cop_feast_practice = [
    "Vestiduras rojas, d\u00e1tiles rojos, encender velas por los m\u00e1rtires, visitar iglesias",
    "Encender hogueras (demera), cantar himnos, celebraci\u00f3n al aire libre el 17 de Thout",
    "Liturgia de medianoche (29 Koiak / 7 de enero), romper el ayuno con comidas festivas",
    "Bendici\u00f3n de aguas, liturgia de renovaci\u00f3n bautismal el 11 de Tobi (19 de enero)",
    "Liturgia gozosa el 29 de Paremhat (7 de abril), himnos especiales a la Theotokos",
    "55 d\u00edas de Gran Cuaresma, liturgia pascual de medianoche, romper ayuno, huevos rojos, saludo '\u00a1Cristo ha resucitado!'"
]

for i in range(13):
    t(f"coptic.month.{i+1}.origin", cop_origin[i])
    t(f"coptic.month.{i+1}.liturgy", cop_liturgy[i])
for i in range(6):
    t(f"coptic.feast.{i+1}.theme", cop_feast_theme[i])
    t(f"coptic.feast.{i+1}.practice", cop_feast_practice[i])


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
    print(f"Translated {translated} keys")
    print(f"Remaining identical EN/ES (non-template): {identical}")


if __name__ == "__main__":
    main()
