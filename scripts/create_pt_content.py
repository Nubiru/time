#!/usr/bin/env python3
"""Create Portuguese content translations.

Strategy: Start from ES (closest language), adapt for Portuguese.
Gabriel verifies quality. PT shares ~85% vocabulary with ES.

Run: python3 scripts/create_pt_content.py
"""

import json, os, re

BASE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
EN_PATH = os.path.join(BASE, "data", "content", "en.json")
ES_PATH = os.path.join(BASE, "data", "content", "es.json")
PT_PATH = os.path.join(BASE, "data", "content", "pt.json")

with open(EN_PATH, "r", encoding="utf-8") as f:
    en = json.load(f)
with open(ES_PATH, "r", encoding="utf-8") as f:
    es = json.load(f)

# Start PT from EN as base (everything falls back to EN anyway)
pt = {}

# ============================================================
# GREGORIAN — months, seasons, days (Gabriel verifies)
# ============================================================

# Month names
greg_months = [
    "Janeiro", "Fevereiro", "Mar\u00e7o", "Abril", "Maio", "Junho",
    "Julho", "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro"
]
greg_month_origins = [
    "Jano \u2014 deus de duas faces dos come\u00e7os, portais e transi\u00e7\u00f5es",
    "Februa \u2014 festival romano de purifica\u00e7\u00e3o (Lupercalia)",
    "Marte \u2014 deus da guerra e da agricultura, o ano novo romano",
    "Aperire (abrir) \u2014 a terra se abre, os bot\u00f5es se desdobram",
    "Maia \u2014 deusa do crescimento, m\u00e3e de Merc\u00fario",
    "Juno \u2014 rainha dos deuses, patrona do casamento",
    "J\u00falio C\u00e9sar \u2014 reformou o calend\u00e1rio, deificado ap\u00f3s a morte",
    "Augusto C\u00e9sar \u2014 primeiro imperador romano, 'o reverenciado'",
    "Septem (sete) \u2014 o s\u00e9timo m\u00eas do antigo calend\u00e1rio romano",
    "Octo (oito) \u2014 o oitavo m\u00eas do antigo calend\u00e1rio romano",
    "Novem (nove) \u2014 o nono m\u00eas do antigo calend\u00e1rio romano",
    "Decem (dez) \u2014 o d\u00e9cimo m\u00eas do antigo calend\u00e1rio romano"
]
greg_month_quality = [
    "Inverno profundo, reflex\u00e3o, novos come\u00e7os",
    "Final do inverno, purifica\u00e7\u00e3o, prepara\u00e7\u00e3o para a primavera",
    "Equin\u00f3cio de primavera, despertar, crescimento agressivo",
    "Plena primavera, abertura, florescimento",
    "Final da primavera, fertilidade, abund\u00e2ncia come\u00e7ando",
    "Solst\u00edcio de ver\u00e3o, uni\u00e3o, m\u00e1xima luz do dia",
    "Pleno ver\u00e3o, poder, radi\u00e2ncia",
    "Final do ver\u00e3o, colheita come\u00e7a, imp\u00e9rio no \u00e1pice",
    "Equin\u00f3cio de outono, equil\u00edbrio, recolhimento",
    "Meados do outono, aprofundamento, transforma\u00e7\u00e3o",
    "Final do outono, lembran\u00e7a, prepara\u00e7\u00e3o para a escurid\u00e3o",
    "Solst\u00edcio de inverno, Saturnalia, o retorno da luz"
]
greg_month_brief = [
    "O m\u00eas das portas \u2014 uma face olha para tr\u00e1s, outra para frente",
    "O m\u00eas da purifica\u00e7\u00e3o \u2014 os \u00faltimos fogos de purifica\u00e7\u00e3o antes do novo crescimento",
    "O m\u00eas de Marte \u2014 o antigo ano romano come\u00e7ava aqui, com guerra e plantio igualmente",
    "O m\u00eas da abertura \u2014 tudo que estava fechado come\u00e7a a se desdobrar",
    "O m\u00eas do crescimento \u2014 a deusa da terra cujo nome significa 'aquela que \u00e9 grande'",
    "O m\u00eas do casamento \u2014 a rainha do c\u00e9u aben\u00e7oa os dias mais longos",
    "O m\u00eas de C\u00e9sar \u2014 nomeado pelo homem que refez o pr\u00f3prio tempo",
    "O m\u00eas de Augusto \u2014 o imperador que exigiu um m\u00eas t\u00e3o grandioso quanto Julho",
    "O s\u00e9timo m\u00eas \u2014 ainda carregando seu n\u00famero antigo embora seja agora o nono",
    "O oitavo m\u00eas \u2014 o mundo se volta para dentro enquanto a luz se contrai",
    "O nono m\u00eas \u2014 os mortos s\u00e3o honrados enquanto o ano envelhece",
    "O d\u00e9cimo m\u00eas \u2014 na mais profunda escurid\u00e3o, os romanos festejavam e o sol renascia"
]

for i in range(12):
    pt[f"gregorian.month.{i+1}.name"] = greg_months[i]
    pt[f"gregorian.month.{i+1}.origin"] = greg_month_origins[i]
    pt[f"gregorian.month.{i+1}.quality"] = greg_month_quality[i]
    pt[f"gregorian.month.{i+1}.brief"] = greg_month_brief[i]

# Seasons
pt["gregorian.season.0.name"] = "Primavera"
pt["gregorian.season.0.theme"] = "Renascimento, crescimento, potencial tornando-se real"
pt["gregorian.season.0.energy"] = "Expansiva \u2014 energia ascende, sementes germinam, dias se alongam"
pt["gregorian.season.0.brief"] = "A esta\u00e7\u00e3o do vir a ser \u2014 tudo que dormia agora desperta"
pt["gregorian.season.1.name"] = "Ver\u00e3o"
pt["gregorian.season.1.theme"] = "Plenitude, radi\u00e2ncia, express\u00e3o m\u00e1xima"
pt["gregorian.season.1.energy"] = "Radiante \u2014 energia no auge, luz domina, vida em plena flor"
pt["gregorian.season.1.brief"] = "A esta\u00e7\u00e3o do ser \u2014 o mundo em brilho m\u00e1ximo"
pt["gregorian.season.2.name"] = "Outono"
pt["gregorian.season.2.theme"] = "Colheita, libera\u00e7\u00e3o, gratid\u00e3o pelo que cresceu"
pt["gregorian.season.2.energy"] = "Contraindo \u2014 energia se volta para dentro, frutos amadurecem, folhas caem"
pt["gregorian.season.2.brief"] = "A esta\u00e7\u00e3o do deixar ir \u2014 beleza na decad\u00eancia, riqueza na libera\u00e7\u00e3o"
pt["gregorian.season.3.name"] = "Inverno"
pt["gregorian.season.3.theme"] = "Repouso, profundidade, a semente oculta, quietude essencial"
pt["gregorian.season.3.energy"] = "Quieta \u2014 energia recolhida \u00e0s ra\u00edzes, sil\u00eancio, gesta\u00e7\u00e3o na escurid\u00e3o"
pt["gregorian.season.3.brief"] = "A esta\u00e7\u00e3o do repouso \u2014 o que parece morto est\u00e1 sonhando"

# Days
greg_days = ["Segunda-feira", "Ter\u00e7a-feira", "Quarta-feira", "Quinta-feira", "Sexta-feira", "S\u00e1bado", "Domingo"]
greg_planets = ["Lua", "Marte", "Merc\u00fario", "J\u00fapiter", "V\u00e9nus", "Saturno", "Sol"]
greg_day_origins = [
    "Ingl\u00eas antigo Monandaeg (dia da Lua)",
    "Ingl\u00eas antigo Tiwesdaeg (dia de Tyr, Marte n\u00f3rdico)",
    "Ingl\u00eas antigo Wodnesdaeg (dia de Woden, Merc\u00fario n\u00f3rdico)",
    "Ingl\u00eas antigo Thunresdaeg (dia de Thor, J\u00fapiter n\u00f3rdico)",
    "Ingl\u00eas antigo Frigedaeg (dia de Freya, V\u00e9nus n\u00f3rdica)",
    "Latim Saturni dies (dia de Saturno)",
    "Ingl\u00eas antigo Sunnandaeg (dia do Sol)"
]
greg_day_briefs = [
    "O dia da Lua \u2014 intui\u00e7\u00e3o, emo\u00e7\u00e3o, as mar\u00e9s interiores",
    "O dia de Marte \u2014 coragem, a\u00e7\u00e3o, a determina\u00e7\u00e3o do guerreiro",
    "O dia de Merc\u00fario \u2014 comunica\u00e7\u00e3o, ast\u00facia, a perspic\u00e1cia do mensageiro",
    "O dia de J\u00fapiter \u2014 expans\u00e3o, abund\u00e2ncia, a generosidade do deus do trov\u00e3o",
    "O dia de V\u00e9nus \u2014 amor, beleza, a deusa do desejo",
    "O dia de Saturno \u2014 disciplina, estrutura, a li\u00e7\u00e3o do mestre",
    "O dia do Sol \u2014 vitalidade, soberania, o centro de todas as \u00f3rbitas"
]

for i in range(7):
    pt[f"gregorian.day.{i}.name"] = greg_days[i]
    pt[f"gregorian.day.{i}.planet"] = greg_planets[i]
    pt[f"gregorian.day.{i}.origin"] = greg_day_origins[i]
    pt[f"gregorian.day.{i}.brief"] = greg_day_briefs[i]

# Gregorian templates
pt["gregorian.tpl.glance"] = "%1$s %2$d (%3$s) \u2014 %4$s"
pt["gregorian.tpl.detail"] = "%1$s: %2$s. %3$s. Esta\u00e7\u00e3o: %4$s \u2014 %5$s. %6$s (%7$s): %8$s."

# ============================================================
# ASTRONOMY — planets, moon phases
# ============================================================

astro_planet_names = [
    "Merc\u00fario", "V\u00e9nus", "Terra", "Marte", "J\u00fapiter", "Saturno", "Urano", "Netuno"
]
for i in range(8):
    pt[f"astronomy.planet.{i}.name"] = astro_planet_names[i]

astro_moon_phases = ["Nova", "Crescente C\u00f4ncava", "Quarto Crescente", "Crescente Convexa",
                     "Cheia", "Minguante Convexa", "Quarto Minguante", "Minguante C\u00f4ncava"]
for i in range(8):
    pt[f"astronomy.moon.{i}.name"] = astro_moon_phases[i]

# ============================================================
# ASTROLOGY — zodiac signs (core content for PT)
# ============================================================

astro_signs = [
    ("Carneiro", "\u00c1ries"), ("Touro", "Touro"), ("G\u00eameos", "G\u00eameos"),
    ("Caranguejo", "C\u00e2ncer"), ("Le\u00e3o", "Le\u00e3o"), ("Virgem", "Virgem"),
    ("Balan\u00e7a", "Libra"), ("Escorpi\u00e3o", "Escorpi\u00e3o"),
    ("Sagit\u00e1rio", "Sagit\u00e1rio"), ("Capric\u00f3rnio", "Capric\u00f3rnio"),
    ("Aqu\u00e1rio", "Aqu\u00e1rio"), ("Peixes", "Peixes")
]
for i, (_, name) in enumerate(astro_signs):
    pt[f"astrology.sign.{i}.name"] = name

# Elements
pt["astrology.element.0.name"] = "Fogo"
pt["astrology.element.1.name"] = "Terra"
pt["astrology.element.2.name"] = "Ar"
pt["astrology.element.3.name"] = "Água"

# ============================================================
# TZOLKIN — seals, tones, colors (Dreamspell in Portuguese)
# ============================================================

tz_seals_pt = [
    "Drag\u00e3o", "Vento", "Noite", "Semente", "Serpente",
    "Enla\u00e7ador de Mundos", "M\u00e3o", "Estrela", "Lua", "C\u00e3o",
    "Macaco", "Humano", "Caminhante do C\u00e9u", "Mago", "\u00c1guia",
    "Guerreiro", "Terra", "Espelho", "Tempestade", "Sol"
]
for i in range(20):
    pt[f"tzolkin.seal.{i}.name"] = tz_seals_pt[i]

tz_colors_pt = ["Vermelho", "Branco", "Azul", "Amarelo"]
for i in range(4):
    pt[f"tzolkin.color.{i}.name"] = tz_colors_pt[i]

tz_tones_pt = [
    "Magn\u00e9tico", "Lunar", "El\u00e9trico", "Auto-Existente",
    "Harm\u00f4nico", "R\u00edtmico", "Ressonante", "Gal\u00e1ctico",
    "Solar", "Planet\u00e1rio", "Espectral", "Cristal", "C\u00f3smico"
]
for i in range(13):
    pt[f"tzolkin.tone.{i+1}.name"] = tz_tones_pt[i]


# ============================================================
# WRITE PT JSON
# ============================================================

# Sort and write
pt = dict(sorted(pt.items()))
with open(PT_PATH, "w", encoding="utf-8") as f:
    json.dump(pt, f, indent=2, ensure_ascii=False)
    f.write("\n")

print(f"Created pt.json with {len(pt)} keys")
print(f"Core content: Gregorian months/seasons/days, Astronomy planets/phases,")
print(f"  Astrology signs/elements, Tzolkin seals/colors/tones")
