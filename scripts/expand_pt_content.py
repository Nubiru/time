#!/usr/bin/env python3
"""Expand Portuguese translations from ES base + manual key terms.

Strategy: PT shares ~85% vocabulary with ES. Start from ES,
apply systematic substitutions, hand-override cultural terms.

Run: python3 scripts/expand_pt_content.py
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
with open(PT_PATH, "r", encoding="utf-8") as f:
    pt = json.load(f)


def es_to_pt(text):
    """Apply common ES→PT substitutions. Not perfect but ~80% accurate."""
    if not text:
        return text

    s = text

    # Common word replacements (order matters — longer patterns first)
    replacements = [
        # Articles and connectors
        (" y ", " e "),
        (" del ", " do "),
        (" de la ", " da "),
        (" de los ", " dos "),
        (" de las ", " das "),
        (" el ", " o "),
        (" la ", " a "),
        (" los ", " os "),
        (" las ", " as "),
        (" un ", " um "),
        (" una ", " uma "),
        # Common verbs/words
        (" es ", " \u00e9 "),
        (" está ", " est\u00e1 "),
        (" son ", " s\u00e3o "),
        (" como ", " como "),
        (" pero ", " mas "),
        (" sin ", " sem "),
        (" con ", " com "),
        (" para ", " para "),
        (" que ", " que "),
        (" todo ", " tudo "),
        (" todos ", " todos "),
        (" su ", " sua "),
        (" más ", " mais "),
        (" muy ", " muito "),
        (" también ", " tamb\u00e9m "),
        (" cuando ", " quando "),
        (" donde ", " onde "),
        (" entre ", " entre "),
        (" sobre ", " sobre "),
        (" hasta ", " at\u00e9 "),
        (" desde ", " desde "),
        (" cada ", " cada "),
        (" otro ", " outro "),
        (" otra ", " outra "),
        (" nuevo ", " novo "),
        (" nueva ", " nova "),
        (" primer ", " primeiro "),
        (" primera ", " primeira "),
        (" segundo ", " segundo "),
        (" segundo ", " segundo "),
        # Suffix patterns
        ("ci\u00f3n", "\u00e7\u00e3o"),
        ("cci\u00f3n", "\u00e7\u00e3o"),
        ("si\u00f3n", "s\u00e3o"),
        ("miento", "mento"),
        # Common endings
        ("idad ", "idade "),
        ("ancia ", "\u00e2ncia "),
        ("encia ", "\u00eancia "),
        # Questions
        ("\u00bfQu\u00e9 ", "O que "),
        ("\u00bfC\u00f3mo ", "Como "),
        ("\u00bfD\u00f3nde ", "Onde "),
        ("\u00bfCu\u00e1ndo ", "Quando "),
        ("\u00bfPor qu\u00e9 ", "Por que "),
        ("\u00bfEstoy ", "Estou "),
        ("\u00bfSigo ", "Ainda "),
        ("\u00bfS\u00e9 ", "Sei "),
        ("\u00bfAmo? ", "Amo? "),
        ("\u00bf", ""),
    ]

    for old, new in replacements:
        s = s.replace(old, new)

    return s


# ============================================================
# MANUAL OVERRIDES — key cultural terms that need hand translation
# ============================================================

manual = {}

# Hebrew months — keep Hebrew names, translate qualities
hebrew_qualities_pt = [
    "Reden\u00e7\u00e3o", "Luz", "Cura", "Vis\u00e3o",
    "Retifica\u00e7\u00e3o", "Movimento", "Temor e ju\u00edzo",
    "Equil\u00edbrio", "Confian\u00e7a", "Sono", "Alegria",
    "Ira santa", "Riso"
]
for i, q in enumerate(hebrew_qualities_pt):
    manual[f"hebrew.month.{i+1}.quality"] = q

# I Ching — templates
manual["iching.tpl.glance"] = "Hexagrama %1$d %2$s \u2014 %3$s sobre %4$s"
manual["iching.tpl.detail"] = "%1$s: %2$s. A Imagem: %3$s. Palavras-chave: %4$s. Trigrama superior: %5$s, Inferior: %6$s."

# Human Design — templates
manual["hd.tpl.glance"] = "%1$s %2$s \u2014 Portal %3$d: %4$s"
manual["hd.tpl.detail"] = "Tipo: %1$s. Estrat\u00e9gia: %2$s. Assinatura: %3$s. N\u00e3o-Eu: %4$s. Aura: %5$s. Autoridade: %6$s \u2014 %7$s. Perfil: %8$s (%9$s). %10$s / %11$s. Portal Solar %12$d: %13$s (%14$s). %15$s."

# HD types
manual["hd.type.0.name"] = "Manifestador"
manual["hd.type.1.name"] = "Gerador"
manual["hd.type.2.name"] = "Gerador Manifestante"
manual["hd.type.3.name"] = "Projetor"
manual["hd.type.4.name"] = "Refletor"
manual["hd.type.0.signature"] = "Paz"
manual["hd.type.1.signature"] = "Satisfa\u00e7\u00e3o"
manual["hd.type.2.signature"] = "Satisfa\u00e7\u00e3o"
manual["hd.type.3.signature"] = "\u00caxito"
manual["hd.type.4.signature"] = "Surpresa"

# Kabbalah — templates
manual["kabbalah.tpl.glance"] = "%1$s \u2014 %2$s"
manual["kabbalah.tpl.detail"] = "%1$s: %2$s. A pergunta: %3$s. Planeta: %4$s."

# Hindu — templates
manual["hindu.tpl.glance"] = "%1$s -- %2$s . %3$s . %4$s"
manual["hindu.tpl.detail"] = "%1$s: %2$s. Divindade: %3$s. Dia %4$s para %5$s."
manual["hindu.tpl.nakshatra"] = " Nakshatra: %1$s."
manual["hindu.tpl.yoga"] = " Yoga: %1$s."

# Astronomy — templates
manual["astronomy.tpl.glance"] = "%1$s \u2014 %2$s"

# Astrology — templates
manual["astrology.tpl.glance"] = "%1$s \u2014 %2$s (%3$s)"

# Buddhist — templates
manual["buddhist.tpl.glance"] = "EB %1$d %2$s %3$d \u2014 %4$s"

# Chinese — templates
manual["chinese.tpl.glance"] = "%1$d %2$s %3$s %4$s"

# Islamic — templates
manual["islamic.tpl.glance"] = "%1$s %2$d \u2014 %3$s"

# Geology — templates
manual["geology.tpl.glance"] = "%1$s \u2014 %2$s"
manual["geology.tpl.detail"] = "E\u00f3n: %1$s (%2$s). %3$s. Vida: %4$s."
manual["geology.tpl.extinction"] = " Extin\u00e7\u00e3o mais pr\u00f3xima: %1$s \u2014 %2$s."
manual["geology.tpl.supercontinent"] = " Supercontinente: %1$s (%2$s)."

# Numerology — templates
manual["numerology.tpl.glance"] = "Caminho de Vida %1$d \u2014 %2$s"
manual["numerology.tpl.glance_year"] = "Caminho de Vida %1$d (%2$s) \u2014 Ano %3$d (%4$s)"
manual["numerology.tpl.detail"] = "Caminho de Vida %1$d: %2$s. %3$s. For\u00e7a: %4$s. Desafio: %5$s."
manual["numerology.tpl.detail_year"] = " Ano Pessoal %1$d: %2$s \u2014 %3$s."

# Chakra — templates
manual["chakra.tpl.glance"] = "%1$s (%2$s) \u2014 %3$s"
manual["chakra.tpl.detail"] = "Chakra: %1$s (%2$s). %3$s. Tema: %4$s. Sombra: %5$s. Pergunta: %6$s"

# Tarot — templates and key terms
manual["tarot.tpl.glance"] = "%1$s \u2014 %2$s"
manual["tarot.tpl.detail"] = "%1$s: %2$s. Palavra-chave: %3$s. %4$s. Luz: %5$s. Sombra: %6$s."
manual["tarot.tpl.detail_suit"] = " Naipe: %1$s (%2$s). %3$s. %4$s."

# Tarot suit names
manual["tarot.suit.0.name"] = "Paus"
manual["tarot.suit.0.element"] = "Fogo"
manual["tarot.suit.1.name"] = "Copas"
manual["tarot.suit.1.element"] = "\u00c1gua"
manual["tarot.suit.2.name"] = "Espadas"
manual["tarot.suit.2.element"] = "Ar"
manual["tarot.suit.3.name"] = "Ouros"
manual["tarot.suit.3.element"] = "Terra"

# Tarot Major Arcana PT names
tarot_pt = [
    "O Louco", "O Mago", "A Sacerdotisa", "A Imperatriz",
    "O Imperador", "O Hierofante", "Os Enamorados", "O Carro",
    "A For\u00e7a", "O Eremita", "A Roda da Fortuna", "A Justi\u00e7a",
    "O Enforcado", "A Morte", "A Temperan\u00e7a", "O Diabo",
    "A Torre", "A Estrela", "A Lua", "O Sol",
    "O Julgamento", "O Mundo"
]
for i in range(22):
    manual[f"tarot.major.{i}.name"] = tarot_pt[i]

# All remaining templates (copy from ES patterns adapted to PT)
# Persian
manual["persian.tpl.glance"] = "%1$s %2$d \u2014 %3$s"
manual["persian.tpl.detail"] = "M\u00eas: %1$s (Avest\u00e3: %2$s). %3$s. %4$s."
manual["persian.tpl.detail_festival"] = " Festival: %1$s \u2014 %2$s. %3$s."
# Coptic
manual["coptic.tpl.glance"] = "%1$s %2$d \u2014 %3$s"
manual["coptic.tpl.detail"] = "M\u00eas: %1$s. Origem: %2$s. Liturgia: %3$s."
manual["coptic.tpl.detail_feast"] = " Festa: %1$s \u2014 %2$s. %3$s."
# Korean
manual["korean.tpl.glance"] = "Dangun %1$d \u2014 %2$s %3$s (%4$s) \u2014 %5$s"
manual["korean.tpl.detail"] = "Ano Dangun %1$d. Animal: %2$s. Elemento: %3$s (%4$s). Polaridade: %5$s."
manual["korean.tpl.detail_festival"] = " Festival: %1$s \u2014 %2$s. %3$s."
# Thai + Lao
manual["thai.tpl.glance"] = "EB %1$d %2$s %3$d \u2014 %4$s"
manual["thai.tpl.detail"] = "EB %1$d. M\u00eas: %2$s (do s\u00e2nscrito %3$s). %4$s."
manual["thai.tpl.detail_festival"] = " Festival: %1$s \u2014 %2$s. %3$s."
manual["lao.tpl.glance"] = "EB %1$d %2$s %3$d \u2014 %4$s"
manual["lao.tpl.detail"] = "EB %1$d. M\u00eas: %2$s (do s\u00e2nscrito %3$s). %4$s."
manual["lao.tpl.detail_festival"] = " Festival: %1$s \u2014 %2$s. %3$s."
# Ethiopian
manual["ethiopian.tpl.glance"] = "%1$s %2$d \u2014 %3$s"
manual["ethiopian.tpl.detail"] = "M\u00eas: %1$s. Significado: %2$s. %3$s."
manual["ethiopian.tpl.detail_feast"] = " Festa: %1$s \u2014 %2$s. %3$s."
# Tamil
manual["tamil.tpl.glance"] = "%1$s %2$d \u2014 %3$s"
manual["tamil.tpl.detail"] = "M\u00eas: %1$s (Rasi: %2$s). %3$s. Ano joviano %4$d de 60."
manual["tamil.tpl.detail_festival"] = " Festival: %1$s \u2014 %2$s. %3$s."
# Bahai
manual["bahai.tpl.glance"] = "%1$s (%2$s) dia %3$d \u2014 %4$s"
manual["bahai.tpl.detail"] = "M\u00eas: %1$s (%2$s). %3$s."
manual["bahai.tpl.detail_holy_day"] = " Dia Sagrado: %1$s \u2014 %2$s."
# Myanmar
manual["myanmar.tpl.glance"] = "%1$s %2$d \u2014 %3$s"
manual["myanmar.tpl.detail"] = "M\u00eas: %1$s. %2$s. Festival: %3$s."
manual["myanmar.tpl.detail_thingyan"] = " THINGYAN: o grande festival da \u00e1gua."
manual["myanmar.year_type.0"] = "Comum (354 dias)"
manual["myanmar.year_type.1"] = "Watat Pequeno (384 dias)"
manual["myanmar.year_type.2"] = "Watat Grande (385 dias)"
# Celtic
manual["celtic.tpl.glance"] = "%1$s (%2$s) Dia %3$d \u2014 %4$s"
manual["celtic.tpl.detail"] = "\u00c1rvore: %1$s (Ogham: %2$s). %3$s. %4$s. For\u00e7as: %5$s. Desafios: %6$s."
manual["celtic.tpl.detail_festival"] = " Festival: %1$s \u2014 %2$s. %3$s."
# Egyptian
manual["egyptian.tpl.glance"] = "%1$s %2$d \u2014 %3$s (%4$s)"
manual["egyptian.tpl.detail"] = "M\u00eas: %1$s. Divindade: %2$s. Dom\u00ednio: %3$s. Ano S\u00f3tico %4$d."
manual["egyptian.tpl.detail_season"] = " Esta\u00e7\u00e3o: %1$s \u2014 %2$s."
# Zoroastrian
manual["zoroastrian.tpl.glance"] = "%1$s Dia %2$d \u2014 %3$s"
manual["zoroastrian.tpl.detail"] = "M\u00eas: %1$s. Dia: %2$s. N\u00famero do dia %3$d."
manual["zoroastrian.tpl.detail_festival"] = " Festival: %1$s \u2014 %2$s. %3$s."
# Japanese
manual["japanese.tpl.glance"] = "%1$s %2$d %3$s \u2014 %4$s"
manual["japanese.tpl.detail"] = "Era: %1$s (%2$s). %3$s. %4$s. Ano %5$d. Rokuyo: %6$s \u2014 %7$s. %8$s. Sekki: %9$s (%10$s) \u2014 %11$s. %12$s."
# Aztec
manual["aztec.tpl.glance"] = "%1$d %2$s \u2014 Ano %3$d %4$s"
manual["aztec.tpl.detail"] = "Dia %1$d Signo: %2$s (%3$s). Divindade: %4$s. Dire\u00e7\u00e3o: %5$s. %6$s."
manual["aztec.tpl.detail_bearer"] = " Portador do Ano: %1$s (%2$s). Dire\u00e7\u00e3o: %3$s. %4$s."
# French Republican
manual["french_republican.tpl.glance"] = "Ano %1$d %2$s %3$d \u2014 %4$s"
manual["french_republican.tpl.detail"] = "Ano %1$d. %2$s (%3$s). Dia %4$d. %5$s. %6$s."
manual["french_republican.tpl.detail_sans"] = "Ano %1$d Sans-culottide %2$d: %3$s \u2014 %4$s."
# Pawukon
manual["pawukon.tpl.glance"] = "%1$s Dia %2$d \u2014 %3$s"
manual["pawukon.tpl.detail"] = "Wuku: %1$s. Divindade: %2$s. %3$s. Urip: %4$d."
# Convergence
manual["convergence.tpl.glance"] = "%1$s \u2014 %2$d sistemas alinhados"
manual["convergence.tpl.detail"] = "%1$s."
# Tzolkin (already partially done — add templates)
manual["tzolkin.tpl.glyph"] = "KIN %1$d"
manual["tzolkin.tpl.glance"] = "Kin %1$d %2$s %3$s %4$s \u2014 %5$s %6$s"
manual["tzolkin.tpl.detail"] = "%1$s %2$s %3$s: %4$s. Tom %5$d: %6$s \u2014 %7$s o poder de %8$s. Selo: %9$s \u2014 %10$s Onda Encantada %11$d: Onda de %12$s \u2014 %13$s, %14$s. Castelo: %15$s \u2014 %16$s. Guia or\u00e1culo: Kin %17$d. An\u00e1logo: Kin %18$d. Ant\u00edpoda: Kin %19$d. Oculto: Kin %20$d."


# ============================================================
# BUILD PT FROM ES + MANUAL OVERRIDES
# ============================================================

def main():
    added = 0

    # For every EN key not in PT, try ES→PT auto-translation
    for k in sorted(en.keys()):
        if k in pt:
            continue  # Already translated

        if k in manual:
            pt[k] = manual[k]
            added += 1
        elif k in es and es[k] != en[k]:
            # ES has a real translation — adapt to PT
            pt[k] = es_to_pt(es[k])
            added += 1
        else:
            # Use EN as fallback (native terms, proper nouns)
            pt[k] = en[k]
            added += 1

    pt_sorted = dict(sorted(pt.items()))
    with open(PT_PATH, "w", encoding="utf-8") as f:
        json.dump(pt_sorted, f, indent=2, ensure_ascii=False)
        f.write("\n")

    print(f"PT: {len(pt_sorted)} keys ({added} added)")
    print(f"Manual overrides: {len(manual)}")
    print(f"Auto ES→PT adaptations: {added - len(manual)}")


if __name__ == "__main__":
    main()
