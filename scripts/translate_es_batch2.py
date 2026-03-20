#!/usr/bin/env python3
"""ES translations batch 2: Japanese, Pawukon, Bahai, Zoroastrian,
French Republican, Coptic + fill gaps in Celtic/Egyptian/Tarot/Aztec.

Run: python3 scripts/translate_es_batch2.py
"""

import json, os, re

BASE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
EN_PATH = os.path.join(BASE, "data", "content", "en.json")
ES_PATH = os.path.join(BASE, "data", "content", "es.json")

ES = {}
def t(key, val):
    ES[key] = val


# ============================================================
# JAPANESE — 5 eras + 6 rokuyo + 24 sekki
# Japanese names/kanji STAY. Meanings → Spanish.
# ============================================================

# Eras: meaning, spirit, brief translated
ji_era_meaning = [
    "Gran rectitud \u2014 la era de la modernizaci\u00f3n, la apertura al mundo",
    "Gran justicia \u2014 la democracia florece, el emperador como s\u00edmbolo",
    "Paz iluminada \u2014 la reconstrucci\u00f3n, el milagro econ\u00f3mico, la paz",
    "Orden hermoso \u2014 la nueva era comienza con esperanza de armon\u00eda",
    "Er\u00e1tica pr\u00f3xima"
]
ji_era_spirit = [
    "Transformaci\u00f3n radical \u2014 un pa\u00eds feudal se convierte en potencia moderna en una generaci\u00f3n",
    "Expansi\u00f3n y crisis \u2014 la democracia, la guerra, la derrota, la reconstrucci\u00f3n",
    "Paz duradera \u2014 64 a\u00f1os sin guerra, prosperidad sin precedentes, envejecimiento de la naci\u00f3n",
    "Armon\u00eda buscada \u2014 la era actual, equilibrio entre tradici\u00f3n e innovaci\u00f3n",
    "Reservada para el futuro"
]
ji_era_brief = [
    "La era Meiji \u2014 Jap\u00f3n despierta, los samuráis se convierten en ingenieros",
    "La era Taisho \u2014 breve floreci\u00f3n democr\u00e1tica entre imperios",
    "La era Showa \u2014 de la devastaci\u00f3n al milagro, la era m\u00e1s larga",
    "La era Reiwa \u2014 la armon\u00eda como aspiraci\u00f3n, la era actual",
    "La era Heisei \u2014 las d\u00e9cadas perdidas, la paz mantenida"
]

for i, (m, s, b) in enumerate(zip(ji_era_meaning, ji_era_spirit, ji_era_brief)):
    t(f"japanese.era.{i}.meaning", m)
    t(f"japanese.era.{i}.spirit", s)
    t(f"japanese.era.{i}.brief", b)

# Rokuyo: meaning, guidance, brief
ji_rokuyo = [
    ("D\u00eda de gran suerte \u2014 todo prospera excepto al mediod\u00eda",
     "Act\u00faa con confianza en la ma\u00f1ana y la tarde \u2014 el mediod\u00eda es neutro",
     "Sensho \u2014 victoria temprana, el d\u00eda favorece a quienes act\u00faan primero"),
    ("Amigo atrae \u2014 buen d\u00eda para celebraciones, malo para funerales",
     "Las reuniones alegres prosperan \u2014 evita lo sombr\u00edo",
     "Tomobiki \u2014 el amigo sigue, la alegr\u00eda atrae m\u00e1s alegr\u00eda"),
    ("Primero pierdes \u2014 la tarde trae la fortuna",
     "Paciencia por la ma\u00f1ana \u2014 la suerte llega despu\u00e9s del mediod\u00eda",
     "Senbu \u2014 derrota primero, la suerte gira por la tarde"),
    ("El buda descansa \u2014 d\u00eda neutro, ni bueno ni malo",
     "No molestes al destino \u2014 el d\u00eda es para la quietud",
     "Butsumetsu \u2014 la muerte de Buda, un d\u00eda de aceptaci\u00f3n pac\u00edfica"),
    ("Gran seguridad \u2014 el d\u00eda m\u00e1s auspicioso, todo prospera",
     "El mejor d\u00eda para cualquier emprendimiento \u2014 act\u00faa con confianza",
     "Taian \u2014 gran paz, el d\u00eda preferido para bodas y nuevos comienzos"),
    ("Mala suerte total \u2014 solo la ma\u00f1ana temprana es auspiciosa",
     "Si debes actuar, hazlo antes del amanecer completo",
     "Shakko \u2014 peligro rojo, solo las primeras horas son seguras"),
]

for i, (m, g, b) in enumerate(ji_rokuyo):
    t(f"japanese.rokuyo.{i}.meaning", m)
    t(f"japanese.rokuyo.{i}.guidance", g)
    t(f"japanese.rokuyo.{i}.brief", b)

# Sekki (24 solar terms): meaning, nature, brief → Spanish
ji_sekki_meaning = [
    "Comienzo de primavera", "Agua de lluvia", "Despertar de insectos",
    "Equinoccio de primavera", "Claridad pura", "Lluvia de granos",
    "Comienzo de verano", "Granos peque\u00f1os maduran", "Granos en espiga",
    "Solsticio de verano", "Calor menor", "Gran calor",
    "Comienzo de oto\u00f1o", "Fin del calor", "Roc\u00edo blanco",
    "Equinoccio de oto\u00f1o", "Roc\u00edo fr\u00edo", "Descenso de la escarcha",
    "Comienzo de invierno", "Nieve menor", "Gran nieve",
    "Solsticio de invierno", "Fr\u00edo menor", "Gran fr\u00edo"
]
ji_sekki_nature = [
    "La primera brisa tibia \u2014 los ciruelos empiezan a florecer bajo cielos fr\u00edos",
    "La nieve se convierte en lluvia \u2014 la tierra bebe",
    "Los insectos emergen de la tierra \u2014 el trueno los despierta",
    "D\u00eda y noche se equilibran \u2014 los cerezos se preparan para florecer",
    "El aire se aclara \u2014 los cerezos est\u00e1n en plena flor",
    "Las lluvias nutren los cultivos \u2014 los campos de arroz se llenan",
    "El verde se profundiza \u2014 las ranas comienzan a cantar",
    "La cebada se dora \u2014 los primeros granos maduran en el campo",
    "El trigo se cosecha \u2014 las mantis religiosas aparecen",
    "El d\u00eda m\u00e1s largo \u2014 las luci\u00e9rnagas bailan sobre los arrozales",
    "La temporada de lluvias termina \u2014 el calor se intensifica",
    "El calor alcanza su m\u00e1ximo \u2014 la temporada del girasol",
    "Las primeras brisas oto\u00f1ales \u2014 los grillos comienzan a cantar",
    "El calor del verano se disipa \u2014 los tifones recorren los mares",
    "La humedad se condensa al amanecer \u2014 las noches se enfrr\u00edan",
    "D\u00eda y noche se equilibran de nuevo \u2014 la cosecha de arroz comienza",
    "Las hojas cambian de color \u2014 los gansos salvajes llegan del norte",
    "La escarcha aparece al amanecer \u2014 los \u00e1rboles se despojan",
    "Los primeros vientos helados \u2014 el camellia florece desafiando el fr\u00edo",
    "La primera nieve fina \u2014 los campos quedan en silencio",
    "La nieve cubre los campos \u2014 los osos hibernan",
    "La noche m\u00e1s larga \u2014 la familia se re\u00fane alrededor del yuzu",
    "El fr\u00edo se profundiza \u2014 el agua comienza a congelarse",
    "El fr\u00edo m\u00e1s intenso \u2014 el mundo espera que la primavera regrese"
]
ji_sekki_brief = [
    "Risshun \u2014 la primavera comienza en el calendario, aunque el fr\u00edo persiste",
    "Usui \u2014 la nieve se derrite, los r\u00edos crecen suavemente",
    "Keichitsu \u2014 el trueno primaveral despierta a las criaturas dormidas",
    "Shunbun \u2014 el equilibrio perfecto, los cerezos a punto de abrirse",
    "Seimei \u2014 el mundo se lava en luz clara y limpia",
    "Kokuu \u2014 la lluvia que alimenta los granos reci\u00e9n plantados",
    "Rikka \u2014 el verde se vuelve exuberante, el verano anuncia su llegada",
    "Shoman \u2014 los primeros frutos del campo alcanzan plenitud",
    "Boshu \u2014 es tiempo de plantar arroz, las espigas crecen",
    "Geshi \u2014 el sol en su punto m\u00e1s alto, la luz domina",
    "Shosho \u2014 el calor comienza a construirse d\u00eda tras d\u00eda",
    "Taisho \u2014 el calor en su apogeo, la naturaleza ruge",
    "Risshu \u2014 la primera brisa fresca entre el calor del verano",
    "Shosho (fin del calor) \u2014 el verano afloja su agarre",
    "Hakuro \u2014 las gotas de roc\u00edo brillan al amanecer fr\u00edo",
    "Shubun \u2014 el equilibrio regresa, la cosecha del arroz comienza",
    "Kanro \u2014 el roc\u00edo se enfr\u00eda, las hojas se ti\u00f1en de rojo y oro",
    "Soko \u2014 la primera escarcha anuncia el invierno que se acerca",
    "Ritto \u2014 el invierno llega oficialmente, las noches se alargan",
    "Shosetsu \u2014 los primeros copos caen silenciosamente",
    "Taisetsu \u2014 la nieve cubre el paisaje, el mundo guarda silencio",
    "Toji \u2014 la noche m\u00e1s larga, despu\u00e9s la luz comienza a crecer",
    "Shokan \u2014 el fr\u00edo se asienta sobre la tierra dormida",
    "Daikan \u2014 el fr\u00edo m\u00e1s profundo, la \u00faltima prueba antes de la primavera"
]

for i in range(24):
    t(f"japanese.sekki.{i}.meaning", ji_sekki_meaning[i])
    t(f"japanese.sekki.{i}.nature", ji_sekki_nature[i])
    t(f"japanese.sekki.{i}.brief", ji_sekki_brief[i])

# Japanese templates
t("japanese.tpl.glance", "%1$s %2$d %3$s \u2014 %4$s")
t("japanese.tpl.detail", "Era: %1$s (%2$s). %3$s. %4$s. A\u00f1o %5$d. Rokuyo: %6$s \u2014 %7$s. %8$s. Sekki: %9$s (%10$s) \u2014 %11$s. %12$s.")


# ============================================================
# BALINESE PAWUKON — 30 wuku weeks
# Balinese names STAY. Quality/brief → Spanish.
# ============================================================

pwk_quality = [
    "Peligro y precauci\u00f3n \u2014 los esp\u00edritus est\u00e1n agitados",
    "Purificaci\u00f3n y limpieza \u2014 buen momento para ceremonias",
    "Fertilidad y abundancia \u2014 la tierra da sus frutos",
    "Conocimiento y estudio \u2014 la mente est\u00e1 aguda",
    "Fuerza y coraje \u2014 las empresas prosperan",
    "Estabilidad y orden \u2014 buen momento para construir",
    "Peligro espiritual \u2014 se requieren ofrendas de protecci\u00f3n",
    "Salud y vitalidad \u2014 energ\u00eda para la sanaci\u00f3n",
    "Conflicto y tensi\u00f3n \u2014 evitar confrontaciones",
    "Prosperidad material \u2014 buenos negocios",
    "Contemplaci\u00f3n silenciosa \u2014 el mundo interior se profundiza",
    "Creatividad y arte \u2014 la belleza florece",
    "Autoridad y liderazgo \u2014 decisiones importantes",
    "Renovaci\u00f3n espiritual \u2014 tiempo de oraci\u00f3n",
    "Armon\u00eda y equilibrio \u2014 las relaciones florecen",
    "Determinaci\u00f3n y disciplina \u2014 el esfuerzo da resultado",
    "Devoci\u00f3n y fe \u2014 conexi\u00f3n con lo divino",
    "Transformaci\u00f3n y cambio \u2014 lo viejo muere para que nazca lo nuevo",
    "Protecci\u00f3n y seguridad \u2014 los guardianes est\u00e1n activos",
    "Alegr\u00eda y celebraci\u00f3n \u2014 tiempo de festividades",
    "Introspecci\u00f3n y meditaci\u00f3n \u2014 el alma busca respuestas",
    "Energ\u00eda y acci\u00f3n \u2014 el momento de moverse",
    "Paciencia y espera \u2014 la fruta madura a su tiempo",
    "Sabidur\u00eda ancestral \u2014 los abuelos hablan",
    "Pureza y limpieza ritual \u2014 preparaci\u00f3n para lo sagrado",
    "Generosidad y servicio \u2014 dar es recibir",
    "Misterio y lo oculto \u2014 lo invisible se manifiesta",
    "Conexi\u00f3n comunitaria \u2014 la aldea se fortalece",
    "Completitud y cierre \u2014 los ciclos se cierran",
    "Renacimiento y comienzo \u2014 el ciclo se renueva"
]

for i in range(30):
    t(f"pawukon.wuku.{i}.quality", pwk_quality[i])

# Pawukon templates
t("pawukon.tpl.glance", "%1$s D\u00eda %2$d \u2014 %3$s")
t("pawukon.tpl.detail", "Wuku: %1$s. Deidad: %2$s. %3$s. Urip: %4$d.")


# ============================================================
# BAHAI — 20 months + 8 holy days
# Arabic names STAY. English names + quality + brief → Spanish.
# ============================================================

bhi_month_english = [
    "D\u00edas de Ha (intercalarios)", "Esplendor", "Gloria", "Belleza",
    "Grandeza", "Luz", "Misericordia", "Palabras",
    "Perfecci\u00f3n", "Nombres", "Poder\u00edo", "Voluntad",
    "Conocimiento", "Poder", "Palabra", "Preguntas",
    "Honor", "Soberan\u00eda", "Dominio", "Alteza"
]
bhi_month_quality = [
    "D\u00edas fuera del calendario dedicados a la hospitalidad, los regalos y la caridad",
    "El resplandor de la revelaci\u00f3n de Dios \u2014 el primer atributo, el amanecer del a\u00f1o nuevo",
    "La majestad y grandeza de la creaci\u00f3n de Dios",
    "La perfecci\u00f3n est\u00e9tica manifiesta en la creaci\u00f3n y el car\u00e1cter humano",
    "La grandeza m\u00e1s all\u00e1 de toda medida \u2014 asombro ante lo infinito",
    "Iluminaci\u00f3n espiritual, la luz del conocimiento y la gu\u00eda",
    "Compasi\u00f3n divina y perd\u00f3n \u2014 amor en acci\u00f3n",
    "La Palabra creadora de Dios \u2014 la revelaci\u00f3n como lenguaje, el poder de la palabra",
    "La b\u00fasqueda de la excelencia en todas las cosas \u2014 completitud sin defecto",
    "Los nombres y atributos de Dios reflejados en la creaci\u00f3n",
    "Poder espiritual y fortaleza \u2014 el coraje de la convicci\u00f3n",
    "Prop\u00f3sito divino \u2014 rendici\u00f3n al plan de Dios y participaci\u00f3n activa en \u00e9l",
    "La b\u00fasqueda de la verdad a trav\u00e9s de la ciencia y la revelaci\u00f3n por igual",
    "Fuerza creadora \u2014 el poder de transformar el yo y la sociedad",
    "Expresi\u00f3n veraz \u2014 la responsabilidad y sacralidad de las palabras",
    "El esp\u00edritu de indagaci\u00f3n \u2014 investigaci\u00f3n independiente de la verdad",
    "Dignidad, nobleza, el honor de servir a la humanidad",
    "La autoridad suprema de Dios \u2014 el reconocimiento de un poder m\u00e1s all\u00e1 de todos los poderes",
    "El dominio de Dios sobre la creaci\u00f3n \u2014 el \u00faltimo mes antes del Ayuno",
    "La aspiraci\u00f3n m\u00e1s elevada \u2014 el mes del ayuno, ascenso espiritual"
]
bhi_month_brief = [
    "Los d\u00edas intercalarios \u2014 4 o 5 d\u00edas de generosidad antes del Ayuno",
    "Mes de Esplendor \u2014 Naw-R\u00faz inicia el a\u00f1o bah\u00e1\u2019\u00ed con luz",
    "Mes de Gloria \u2014 la magnificencia que brilla a trav\u00e9s de todo lo creado",
    "Mes de Belleza \u2014 la belleza como reflejo de lo divino",
    "Mes de Grandeza \u2014 la inmensidad de la creaci\u00f3n de Dios m\u00e1s all\u00e1 de la comprensi\u00f3n",
    "Mes de Luz \u2014 la luz que disipa la ignorancia y revela la verdad",
    "Mes de Misericordia \u2014 el tierno cuidado de Dios por cada criatura",
    "Mes de Palabras \u2014 la Palabra a trav\u00e9s de la cual todo fue creado",
    "Mes de Perfecci\u00f3n \u2014 el est\u00e1ndar divino hacia el cual la humanidad se esfuerza",
    "Mes de Nombres \u2014 cada atributo divino tiene un nombre, cada nombre un mundo",
    "Mes de Poder\u00edo \u2014 la fuerza que proviene de la fe, no de la fuerza",
    "Mes de Voluntad \u2014 la intersecci\u00f3n de la elecci\u00f3n humana y el prop\u00f3sito divino",
    "Mes de Conocimiento \u2014 la armon\u00eda de ciencia y religi\u00f3n, alas gemelas de un p\u00e1jaro",
    "Mes de Poder \u2014 la fuerza divina que permite la transformaci\u00f3n",
    "Mes de Palabra \u2014 las palabras como recipientes de verdad o instrumentos de da\u00f1o",
    "Mes de Preguntas \u2014 el deber del buscador de preguntar y descubrir por s\u00ed mismo",
    "Mes de Honor \u2014 la verdadera nobleza medida por el servicio, no por la posici\u00f3n",
    "Mes de Soberan\u00eda \u2014 el rey de reyes a quien toda soberan\u00eda pertenece",
    "Mes de Dominio \u2014 el dominio que abarca todos los mundos",
    "Mes de Alteza \u2014 el mes del Ayuno, 19 d\u00edas de abstinencia de amanecer a atardecer"
]

for i in range(20):
    t(f"bahai.month.{i}.english", bhi_month_english[i])
    t(f"bahai.month.{i}.quality", bhi_month_quality[i])
    t(f"bahai.month.{i}.brief", bhi_month_brief[i])

# Bahai holy days
bhi_holy_theme = [
    "A\u00f1o Nuevo \u2014 1 Bah\u00e1, el equinoccio vernal, renacimiento y renovaci\u00f3n",
    "Declaraci\u00f3n de Bah\u00e1\u2019u\u2019ll\u00e1h en el Jard\u00edn de Ridv\u00e1n \u2014 la Festividad M\u00e1s Grande",
    "La familia de Bah\u00e1\u2019u\u2019ll\u00e1h se une a \u00c9l en el jard\u00edn",
    "Partida del jard\u00edn \u2014 conclusi\u00f3n de la Festividad M\u00e1s Grande",
    "El B\u00e1b declara Su misi\u00f3n como Heraldo de una nueva era",
    "Fallecimiento del Fundador \u2014 recuerdo solemne a las 3 AM",
    "Ejecuci\u00f3n del B\u00e1b por fusilamiento en Tabriz",
    "D\u00edas Santos Gemelos celebrando los nacimientos de las Manifestaciones Gemelas"
]
bhi_holy_brief = [
    "El A\u00f1o Nuevo Bah\u00e1\u2019\u00ed \u2014 el d\u00eda en que el Sol entra en Aries, terminando el Ayuno",
    "El d\u00eda m\u00e1s sagrado \u2014 cuando Bah\u00e1\u2019u\u2019ll\u00e1h anunci\u00f3 Su misi\u00f3n en el jard\u00edn del para\u00edso",
    "El noveno d\u00eda de Ridv\u00e1n \u2014 reuni\u00f3n en el jard\u00edn de la declaraci\u00f3n",
    "El duod\u00e9cimo y \u00faltimo d\u00eda de Ridv\u00e1n \u2014 el festival de 12 d\u00edas concluye",
    "La declaraci\u00f3n del B\u00e1b \u2014 la puerta se abre, la nueva dispensaci\u00f3n comienza",
    "La ascensi\u00f3n de Bah\u00e1\u2019u\u2019ll\u00e1h \u2014 la Manifestaci\u00f3n de Dios parte de este mundo",
    "El martirio del B\u00e1b \u2014 el Heraldo dio Su vida por la promesa de un mundo nuevo",
    "Los Cumplea\u00f1os Gemelos \u2014 dos d\u00edas consecutivos de alegr\u00eda por dos Revelaciones consecutivas"
]

for i in range(8):
    t(f"bahai.holy_day.{i+1}.theme", bhi_holy_theme[i])
    t(f"bahai.holy_day.{i+1}.brief", bhi_holy_brief[i])

# Bahai templates
t("bahai.tpl.glance", "%1$s (%2$s) d\u00eda %3$d \u2014 %4$s")
t("bahai.tpl.detail", "Mes: %1$s (%2$s). %3$s.")
t("bahai.tpl.detail_holy_day", " D\u00eda Sagrado: %1$s \u2014 %2$s.")


# ============================================================
# ZOROASTRIAN — 7 Amesha Spenta + 5 Gathas + 9 festivals
# Avestan names STAY. Meanings/virtues/briefs → Spanish.
# ============================================================

zi_amesha_meaning = [
    "Mente Recta", "Mejor Verdad", "Dominio Deseable",
    "Santa Devoci\u00f3n", "Totalidad", "Inmortalidad", "Esp\u00edritu Santo"
]
zi_amesha_domain = [
    "Animales, buena mente, pensamiento recto",
    "Fuego sagrado, verdad, rectitud c\u00f3smica",
    "Metales, cielo, poder justo",
    "Tierra, devoci\u00f3n, fe amorosa",
    "Agua, salud, perfecci\u00f3n",
    "Plantas, inmortalidad, eternidad",
    "Fuego divino, esp\u00edritu santo, la presencia de Ahura Mazda"
]
zi_amesha_virtue = [
    "Pensamiento recto \u2014 discernir la verdad de la mentira",
    "Verdad c\u00f3smica \u2014 el orden que sustenta la creaci\u00f3n",
    "Soberan\u00eda justa \u2014 poder al servicio del bien",
    "Fe devota \u2014 rendici\u00f3n amorosa a lo divino",
    "Plenitud \u2014 la b\u00fasqueda de la perfecci\u00f3n en todas las cosas",
    "Inmortalidad \u2014 la vida eterna que supera la muerte",
    "Santidad \u2014 la chispa divina en toda la creaci\u00f3n"
]
zi_amesha_brief = [
    "Vohu Manah \u2014 la mente buena que elige la verdad",
    "Asha Vahishta \u2014 la mejor verdad, el orden c\u00f3smico del fuego sagrado",
    "Khshathra Vairya \u2014 el dominio justo que sirve en lugar de dominar",
    "Spenta Armaiti \u2014 la tierra sagrada, devoci\u00f3n encarnada",
    "Haurvatat \u2014 la totalidad del agua que todo sana",
    "Ameretat \u2014 la inmortalidad verde de cada planta",
    "Spenta Mainyu \u2014 el esp\u00edritu santo de Ahura Mazda"
]

for i in range(7):
    t(f"zoroastrian.amesha.{i}.meaning", zi_amesha_meaning[i])
    t(f"zoroastrian.amesha.{i}.domain", zi_amesha_domain[i])
    t(f"zoroastrian.amesha.{i}.virtue", zi_amesha_virtue[i])
    t(f"zoroastrian.amesha.{i}.brief", zi_amesha_brief[i])

# Zoroastrian templates
t("zoroastrian.tpl.glance", "%1$s D\u00eda %2$d \u2014 %3$s")
t("zoroastrian.tpl.detail", "Mes: %1$s. D\u00eda: %2$s. N\u00famero del d\u00eda %3$d.")
t("zoroastrian.tpl.detail_festival", " Festival: %1$s \u2014 %2$s. %3$s.")


# ============================================================
# FRENCH REPUBLICAN — 12 months + 6 sans-culottides
# French names STAY. English/season/nature/brief → Spanish.
# ============================================================

fri_month_english = [
    "Vendimia", "Bruma", "Escarcha", "Nieve",
    "Lluvia", "Viento", "Germinaci\u00f3n", "Floraci\u00f3n",
    "Pradera", "Cosecha", "Calor", "Fruto"
]
fri_month_brief = [
    "Mes de la vendimia \u2014 las uvas maduran bajo el sol de septiembre",
    "Mes de la bruma \u2014 la niebla envuelve los campos de octubre",
    "Mes de la escarcha \u2014 el fr\u00edo de noviembre cristaliza la tierra",
    "Mes de la nieve \u2014 diciembre cubre el paisaje de blanco",
    "Mes de la lluvia \u2014 enero empapa la tierra dormida",
    "Mes del viento \u2014 febrero barre el mundo con su aliento",
    "Mes de la germinaci\u00f3n \u2014 marzo despierta las semillas bajo la tierra",
    "Mes de la floraci\u00f3n \u2014 abril estalla en color y fragancia",
    "Mes de la pradera \u2014 mayo extiende su alfombra verde",
    "Mes de la cosecha \u2014 junio recoge el primer trigo dorado",
    "Mes del calor \u2014 julio arde con el fuego del sol de verano",
    "Mes del fruto \u2014 agosto ofrece la abundancia madura del \u00e1rbol"
]

for i in range(12):
    t(f"french_republican.month.{i+1}.english", fri_month_english[i])
    t(f"french_republican.month.{i+1}.brief", fri_month_brief[i])

# Sans-culottides virtues
fri_sans_virtue = [
    "El genio creativo de la humanidad", "El trabajo que dignifica",
    "Las haza\u00f1as que inspiran", "Las recompensas merecidas",
    "La voz del pueblo soberano", "La celebraci\u00f3n de la Revoluci\u00f3n"
]
fri_sans_brief = [
    "D\u00eda del Genio \u2014 el esp\u00edritu creativo que hace avanzar la civilizaci\u00f3n",
    "D\u00eda del Trabajo \u2014 la nobleza del esfuerzo humano",
    "D\u00eda de las Haza\u00f1as \u2014 los actos heroicos que definen a un pueblo",
    "D\u00eda de las Recompensas \u2014 lo que se siembra con justicia se cosecha con honor",
    "D\u00eda de la Opini\u00f3n \u2014 la voz libre del ciudadano",
    "D\u00eda de la Revoluci\u00f3n \u2014 la transformaci\u00f3n que fund\u00f3 la Rep\u00fablica"
]

for i in range(6):
    t(f"french_republican.sans.{i}.virtue", fri_sans_virtue[i])
    t(f"french_republican.sans.{i}.brief", fri_sans_brief[i])

# French Republican templates
t("french_republican.tpl.glance", "A\u00f1o %1$d %2$s %3$d \u2014 %4$s")
t("french_republican.tpl.detail", "A\u00f1o %1$d. %2$s (%3$s). D\u00eda %4$d. %5$s. %6$s.")
t("french_republican.tpl.detail_sans", "A\u00f1o %1$d Sans-culottide %2$d: %3$s \u2014 %4$s.")


# ============================================================
# COPTIC — 13 months + 6 feasts
# Coptic names STAY. Origin/liturgy/brief → Spanish.
# ============================================================

cop_month_brief = [
    "Mes de Thoth \u2014 el antiguo dios escriba abre el a\u00f1o nuevo de los m\u00e1rtires",
    "Mes de Paopi \u2014 el festival sagrado que honraba el don del r\u00edo",
    "Mes de Hathor \u2014 la diosa vaca de la alegr\u00eda da paso a la expectativa del Adviento",
    "Mes de Koiak \u2014 los antiguos ritos de Osiris se convierten en el ayuno antes de la Navidad de Cristo",
    "Mes de Tobi \u2014 las aguas de purificaci\u00f3n y el bautismo de Cristo",
    "Mes de Meshir \u2014 el sol ardiente que una vez resec\u00f3 Egipto ahora calienta a los fieles",
    "Mes de Paremhat \u2014 el mes del constructor-sanador coronado por el saludo del \u00e1ngel",
    "Mes de Parmouti \u2014 la diosa de la cosecha encuentra el ayuno de Cuaresma",
    "Mes de Pashons \u2014 el mes del dios luna recibe la luz de la resurrecci\u00f3n",
    "Mes de Paoni \u2014 el antiguo valle de los muertos se convierte en el ayuno apost\u00f3lico",
    "Mes de Epip \u2014 el calor feroz que prob\u00f3 tanto al antiguo Egipto como a los primeros cristianos",
    "Mes de Mesori \u2014 el renacimiento de Ra se convierte en la Asunci\u00f3n de la Theotokos",
    "El mes peque\u00f1o \u2014 cinco o seis d\u00edas suspendidos entre a\u00f1os, pausa sagrada"
]

for i in range(13):
    t(f"coptic.month.{i+1}.brief", cop_month_brief[i])

cop_feast_brief = [
    "El A\u00f1o Nuevo Copto \u2014 1 Thout, te\u00f1ido de rojo por la sangre de los m\u00e1rtires",
    "La Cruz encontrada \u2014 las hogueras reflejan las llamas que Helena encendi\u00f3 para hallar la madera enterrada",
    "El Verbo hecho carne \u2014 tras 43 d\u00edas de ayuno, la Luz entra al mundo",
    "Dios revelado en el agua \u2014 la paloma desciende, la voz habla, la Trinidad aparece",
    "El saludo del \u00e1ngel \u2014 el momento en que el cielo pidi\u00f3 permiso a la tierra",
    "La Resurrecci\u00f3n \u2014 la muerte derrotada, la piedra removida, la primera luz del domingo"
]

for i in range(6):
    t(f"coptic.feast.{i+1}.brief", cop_feast_brief[i])

# Coptic templates
t("coptic.tpl.glance", "%1$s %2$d \u2014 %3$s")
t("coptic.tpl.detail", "Mes: %1$s. Origen: %2$s. Liturgia: %3$s.")
t("coptic.tpl.detail_feast", " Fiesta: %1$s \u2014 %2$s. %3$s.")


# ============================================================
# MERGE
# ============================================================

def main():
    with open(ES_PATH, "r", encoding="utf-8") as f:
        es = json.load(f)

    translated = 0
    missing = 0
    for k, v in ES.items():
        if k in es:
            es[k] = v
            translated += 1
        else:
            missing += 1
            print(f"  WARNING: '{k}' not in es.json")

    es = dict(sorted(es.items()))
    with open(ES_PATH, "w", encoding="utf-8") as f:
        json.dump(es, f, indent=2, ensure_ascii=False)
        f.write("\n")

    with open(EN_PATH, "r", encoding="utf-8") as f:
        en = json.load(f)
    identical = sum(1 for k in en if k in es and en[k] == es[k] and '.tpl.' not in k)
    print(f"Translated {translated} keys ({missing} missing)")
    print(f"Remaining identical EN/ES (non-template): {identical}")


if __name__ == "__main__":
    main()
