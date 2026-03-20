#!/usr/bin/env python3
"""Extract content strings from *_interpret.c files into en.json.

Reads C source files, extracts static string arrays, generates JSON content keys.
Run: python3 scripts/extract_interpret_content.py
"""

import json
import os
import re

BASE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
EN_PATH = os.path.join(BASE, "data", "content", "en.json")
ES_PATH = os.path.join(BASE, "data", "content", "es.json")
SRC = os.path.join(BASE, "src", "systems")


def read_file(path):
    with open(path, "r", encoding="utf-8") as f:
        return f.read()


def extract_strings(text):
    """Extract C string literals from a struct initializer line."""
    # Match "..." strings, handling escaped quotes and \xHH sequences
    return re.findall(r'"((?:[^"\\]|\\.)*)"', text)


def extract_array_entries(c_code, array_pattern):
    """Extract entries from a static const array."""
    # Find the array block
    m = re.search(array_pattern + r'\s*=\s*\{', c_code)
    if not m:
        return []

    start = m.end()
    # Find matching closing brace (track nesting)
    depth = 1
    pos = start
    while pos < len(c_code) and depth > 0:
        if c_code[pos] == '{':
            depth += 1
        elif c_code[pos] == '}':
            depth -= 1
        pos += 1

    block = c_code[start:pos - 1]

    # Split by top-level entries { ... }
    entries = []
    entry_depth = 0
    entry_start = None
    for i, ch in enumerate(block):
        if ch == '{':
            if entry_depth == 0:
                entry_start = i
            entry_depth += 1
        elif ch == '}':
            entry_depth -= 1
            if entry_depth == 0 and entry_start is not None:
                entries.append(block[entry_start:i + 1])
                entry_start = None

    return entries


def extract_string_array(c_code, pattern):
    """Extract a simple string array like SEAL_NAMES[20]."""
    m = re.search(pattern + r'\s*=\s*\{([^}]+)\}', c_code, re.DOTALL)
    if not m:
        return []
    return re.findall(r'"((?:[^"\\]|\\.)*)"', m.group(1))


# ============================================================
# Module definitions: (prefix, file, arrays_config)
# arrays_config = [(key_prefix, array_regex, fields)]
# ============================================================

MODULES = {
    "persian": {
        "file": "persian/persian_interpret.c",
        "arrays": [
            ("persian.month", r'static const \w+ MONTHS\[12\]',
             ["name", "avestan", "meaning", "season", "brief"], 1),
            ("persian.festival", r'static const \w+ FESTIVALS\[4\]',
             ["name", "theme", "practice", "brief"], 1),
        ],
        "templates_en": {
            "persian.tpl.glance": "%1$s %2$d \xe2\x80\x94 %3$s",
            "persian.tpl.detail": "Month: %1$s (Avestan: %2$s). %3$s. %4$s.",
            "persian.tpl.detail_festival": " Festival: %1$s \xe2\x80\x94 %2$s. %3$s.",
        },
    },
    "coptic": {
        "file": "coptic/coptic_interpret.c",
        "arrays": [
            ("coptic.month", r'static const \w+ MONTHS\[13\]',
             ["name", "origin", "liturgy", "brief"], 1),
            ("coptic.feast", r'static const \w+ FEASTS\[6\]',
             ["name", "theme", "practice", "brief"], 1),
        ],
        "templates_en": {
            "coptic.tpl.glance": "%1$s %2$d \xe2\x80\x94 %3$s",
            "coptic.tpl.detail": "Month: %1$s. Origin: %2$s. Liturgy: %3$s.",
            "coptic.tpl.detail_feast": " Feast: %1$s \xe2\x80\x94 %2$s. %3$s.",
        },
    },
    "korean": {
        "file": "korean/korean_interpret.c",
        "arrays": [
            ("korean.festival", r'static const \w+ FESTIVALS\[7\]',
             ["name", "korean", "theme", "practice", "brief"], 0),
            ("korean.element", r'static const \w+ ELEMENTS\[5\]',
             ["korean", "meaning", "season", "brief"], 0),
        ],
        "templates_en": {
            "korean.tpl.glance": "Dangun %1$d \xe2\x80\x94 %2$s %3$s (%4$s) \xe2\x80\x94 %5$s",
            "korean.tpl.detail": "Dangun year %1$d. Animal: %2$s. Element: %3$s (%4$s). Polarity: %5$s.",
            "korean.tpl.detail_festival": " Festival: %1$s \xe2\x80\x94 %2$s. %3$s.",
        },
    },
    "thai": {
        "file": "thai/thai_interpret.c",
        "arrays": [
            ("thai.month", r'static const \w+ MONTHS\[12\]',
             ["thai", "origin", "season", "brief"], 1),
            ("thai.festival", r'static const \w+ FESTIVALS\[3\]',
             ["name", "theme", "practice", "brief"], 1),
        ],
        "templates_en": {
            "thai.tpl.glance": "BE %1$d %2$s %3$d \xe2\x80\x94 %4$s",
            "thai.tpl.detail": "BE %1$d. Month: %2$s (from Sanskrit %3$s). %4$s.",
            "thai.tpl.detail_festival": " Festival: %1$s \xe2\x80\x94 %2$s. %3$s.",
        },
    },
    "ethiopian": {
        "file": "ethiopian/ethiopian_interpret.c",
        "arrays": [
            ("ethiopian.month", r'static const \w+ MONTHS\[13\]',
             ["name", "meaning", "season", "brief"], 1),
            ("ethiopian.feast", r'static const \w+ FEASTS\[4\]',
             ["name", "theme", "practice", "brief"], 1),
        ],
        "templates_en": {
            "ethiopian.tpl.glance": "%1$s %2$d \xe2\x80\x94 %3$s",
            "ethiopian.tpl.detail": "Month: %1$s. Meaning: %2$s. %3$s.",
            "ethiopian.tpl.detail_feast": " Feast: %1$s \xe2\x80\x94 %2$s. %3$s.",
        },
    },
    "tamil": {
        "file": "tamil/tamil_interpret.c",
        "arrays": [
            ("tamil.month", r'static const \w+ MONTHS\[12\]',
             ["tamil", "rasi", "season", "brief"], 1),
            ("tamil.festival", r'static const \w+ FESTIVALS\[4\]',
             ["name", "theme", "practice", "brief"], 1),
        ],
        "templates_en": {
            "tamil.tpl.glance": "%1$s %2$d \xe2\x80\x94 %3$s",
            "tamil.tpl.detail": "Month: %1$s (Rasi: %2$s). %3$s. Jovian year %4$d of 60.",
            "tamil.tpl.detail_festival": " Festival: %1$s \xe2\x80\x94 %2$s. %3$s.",
        },
    },
    "bahai": {
        "file": "bahai/bahai_interpret.c",
        "arrays": [
            ("bahai.month", r'static const \w+ MONTHS\[20\]',
             ["arabic", "english", "quality", "brief"], 0),
            ("bahai.holy_day", r'static const \w+ HOLY_DAYS\[8\]',
             ["name", "theme", "brief"], 1),
        ],
        "templates_en": {
            "bahai.tpl.glance": "%1$s (%2$s) day %3$d \xe2\x80\x94 %4$s",
            "bahai.tpl.detail": "Month: %1$s (%2$s). %3$s.",
            "bahai.tpl.detail_holy_day": " Holy Day: %1$s \xe2\x80\x94 %2$s.",
        },
    },
    "myanmar": {
        "file": "myanmar/myanmar_interpret.c",
        "arrays": [
            ("myanmar.month", r'static const \w+ MONTHS\[12\]',
             ["name", "season", "festival", "brief"], 0),
        ],
        "templates_en": {
            "myanmar.tpl.glance": "%1$s %2$d \xe2\x80\x94 %3$s",
            "myanmar.tpl.detail": "Month: %1$s. %2$s. Festival: %3$s.",
            "myanmar.tpl.detail_thingyan": " THINGYAN: the great water festival.",
            "myanmar.year_type.0": "Common (354 days)",
            "myanmar.year_type.1": "Small Watat (384 days)",
            "myanmar.year_type.2": "Big Watat (385 days)",
        },
    },
    "lao": {
        "file": "lao/lao_interpret.c",
        "arrays": [
            ("lao.month", r'static const \w+ MONTHS\[12\]',
             ["lao", "origin", "season", "brief"], 1),
            ("lao.festival", r'static const \w+ FESTIVALS\[3\]',
             ["name", "theme", "practice", "brief"], 1),
        ],
        "templates_en": {
            "lao.tpl.glance": "BE %1$d %2$s %3$d \xe2\x80\x94 %4$s",
            "lao.tpl.detail": "BE %1$d. Month: %2$s (from Sanskrit %3$s). %4$s.",
            "lao.tpl.detail_festival": " Festival: %1$s \xe2\x80\x94 %2$s. %3$s.",
        },
    },
    # ============ BATCH 3: Complex modules ============
    "celtic": {
        "file": "celtic/celtic_interpret.c",
        "arrays": [
            ("celtic.tree", r'static const \w+ TREES\[14\]',
             ["tree", "ogham", "archetype", "personality", "strengths", "challenges", "brief"], 0),
            ("celtic.festival", r'static const \w+ FESTIVALS\[8\]',
             ["name", "theme", "practice", "reflection", "brief"], 0),
        ],
        "templates_en": {
            "celtic.tpl.glance": "%1$s (%2$s) \xe2\x80\x94 %3$s",
            "celtic.tpl.detail": "Tree: %1$s (Ogham: %2$s). %3$s. %4$s. Strengths: %5$s. Challenges: %6$s.",
            "celtic.tpl.detail_festival": " Festival: %1$s \xe2\x80\x94 %2$s. %3$s.",
        },
    },
    "egyptian": {
        "file": "egyptian/egyptian_interpret.c",
        "arrays": [
            ("egyptian.season", r'static const \w+ SEASONS\[4\]',
             ["name", "meaning", "theme", "reflection", "brief"], 0),
            ("egyptian.month", r'static const \w+ MONTHS\[12\]',
             ["name", "deity", "domain", "brief"], 1),
            ("egyptian.epag", r'static const \w+ EPAGS\[5\]',
             ["deity", "archetype", "theme", "brief"], 0),
        ],
        "templates_en": {
            "egyptian.tpl.glance": "%1$s %2$d \xe2\x80\x94 %3$s",
            "egyptian.tpl.detail": "Month: %1$s. Deity: %2$s. Domain: %3$s.",
            "egyptian.tpl.detail_season": " Season: %1$s \xe2\x80\x94 %2$s.",
        },
    },
    "zoroastrian": {
        "file": "zoroastrian/zoroastrian_interpret.c",
        "arrays": [
            ("zoroastrian.amesha", r'static const \w+ AMESHAS\[7\]',
             ["avestan", "meaning", "domain", "element", "virtue", "brief"], 0),
            ("zoroastrian.gatha", r'static const \w+ GATHAS\[5\]',
             ["name", "theme", "practice", "brief"], 0),
            ("zoroastrian.festival", r'static const \w+ FESTIVALS\[9\]',
             ["name", "theme", "practice", "brief"], 0),
        ],
        "templates_en": {
            "zoroastrian.tpl.glance": "%1$s \xe2\x80\x94 %2$s",
            "zoroastrian.tpl.detail": "Amesha Spenta: %1$s (%2$s). Domain: %3$s. Element: %4$s. Virtue: %5$s.",
            "zoroastrian.tpl.detail_festival": " Festival: %1$s \xe2\x80\x94 %2$s. %3$s.",
        },
    },
    "japanese": {
        "file": "japanese/japanese_interpret.c",
        "arrays": [
            ("japanese.era", r'static const \w+ ERAS\[5\]',
             ["name", "kanji", "meaning", "spirit", "brief"], 0),
            ("japanese.rokuyo", r'static const \w+ ROKUYO\[6\]',
             ["name", "meaning", "guidance", "brief"], 0),
            ("japanese.sekki", r'static const \w+ SEKKI\[24\]',
             ["name", "kanji", "meaning", "nature", "brief"], 0),
        ],
        "templates_en": {
            "japanese.tpl.glance": "%1$s %2$d %3$s \xe2\x80\x94 %4$s",
            "japanese.tpl.detail": "Era: %1$s (%2$s). %3$s. Rokuyo: %4$s \xe2\x80\x94 %5$s. Sekki: %6$s \xe2\x80\x94 %7$s.",
        },
    },
    "tarot": {
        "file": "tarot/tarot_interpret.c",
        "arrays": [
            ("tarot.major", r'static const \w+ MAJORS\[22\]',
             ["name", "archetype", "keyword", "light", "shadow", "brief"], 0),
            ("tarot.suit", r'static const \w+ SUITS\[4\]',
             ["name", "element", "domain", "quality", "brief"], 0),
        ],
        "templates_en": {
            "tarot.tpl.glance": "%1$s \xe2\x80\x94 %2$s",
            "tarot.tpl.detail": "%1$s: %2$s. Keyword: %3$s. Light: %4$s. Shadow: %5$s.",
            "tarot.tpl.detail_suit": " Suit: %1$s (%2$s). %3$s. %4$s.",
        },
    },
    "aztec": {
        "file": "aztec/aztec_interpret.c",
        "arrays": [
            ("aztec.sign", r'static const \w+ SIGNS\[20\]',
             ["nahuatl", "english", "deity", "direction", "quality", "brief"], 0),
            ("aztec.bearer", r'static const \w+ BEARERS\[4\]',
             ["nahuatl", "english", "direction", "quality", "brief"], 0),
        ],
        "templates_en": {
            "aztec.tpl.glance": "%1$s (%2$s) \xe2\x80\x94 %3$s",
            "aztec.tpl.detail": "Day Sign: %1$s (%2$s). Deity: %3$s. Direction: %4$s. %5$s.",
            "aztec.tpl.detail_bearer": " Year Bearer: %1$s (%2$s). Direction: %3$s. %4$s.",
        },
    },
    "french_republican": {
        "file": "french_republican/french_republican_interpret.c",
        "arrays": [
            ("french_republican.month", r'static const \w+ MONTHS\[12\]',
             ["french", "english", "season", "nature", "brief"], 1),
            ("french_republican.sans", r'static const \w+ SANS\[6\]',
             ["french", "virtue", "brief"], 0),
        ],
        "templates_en": {
            "french_republican.tpl.glance": "An %1$d %2$s %3$d \xe2\x80\x94 %4$s",
            "french_republican.tpl.detail": "Month: %1$s (%2$s). %3$s. %4$s.",
            "french_republican.tpl.detail_sans": " Sans-culottide: %1$s \xe2\x80\x94 %2$s.",
        },
    },
    "pawukon": {
        "file": "balinese/pawukon_interpret.c",
        "arrays": [
            ("pawukon.wuku", r'static const \w+ WUKUS\[30\]',
             ["name", "deity", "quality", "brief"], 0),
        ],
        "templates_en": {
            "pawukon.tpl.glance": "%1$s \xe2\x80\x94 %2$s . %3$s",
            "pawukon.tpl.detail": "Wuku: %1$s. Deity: %2$s. %3$s.",
        },
    },
    "convergence": {
        "file": "unified/convergence_interpret.c",
        "arrays": [
            ("convergence.strength", r'static const \w+ s_strengths\[4\]',
             ["name", "description", "significance"], 0),
            ("convergence.system", r'static const \w+ s_systems\[21\]',
             ["system_name", "tradition", "what_aligns"], 0),
            ("convergence.pattern", r'static const \w+ s_patterns\[8\]',
             ["name", "description"], 0),
        ],
        "templates_en": {
            "convergence.tpl.glance": "%1$s \xe2\x80\x94 %2$s systems aligning",
            "convergence.tpl.detail": "Convergence: %1$s. %2$s.",
        },
    },
}


def decode_c_string(s):
    """Decode C escape sequences in a string."""
    # Handle \xHH hex escapes
    def hex_replace(m):
        return bytes([int(m.group(1), 16)]).decode('utf-8', errors='replace')

    s = re.sub(r'\\x([0-9a-fA-F]{2})', hex_replace, s)
    s = s.replace('\\n', '\n').replace('\\t', '\t').replace('\\"', '"')
    s = s.replace("\\'", "'").replace('\\\\', '\\')
    return s


def process_module(name, config):
    """Process a single module, return dict of EN keys."""
    filepath = os.path.join(SRC, config["file"])
    c_code = read_file(filepath)
    en_data = {}

    # Add templates
    if "templates_en" in config:
        en_data.update(config["templates_en"])

    # Process each array
    for key_prefix, array_regex, fields, start_idx in config["arrays"]:
        entries = extract_array_entries(c_code, array_regex)
        for i, entry in enumerate(entries):
            strings = extract_strings(entry)
            idx = i + start_idx
            for j, field in enumerate(fields):
                if j < len(strings):
                    val = decode_c_string(strings[j])
                    en_data[f"{key_prefix}.{idx}.{field}"] = val

    return en_data


def main():
    with open(EN_PATH, "r", encoding="utf-8") as f:
        en = json.load(f)
    with open(ES_PATH, "r", encoding="utf-8") as f:
        es = json.load(f)

    en_before = len(en)
    total_new = 0

    for name, config in MODULES.items():
        en_data = process_module(name, config)
        en.update(en_data)
        # For now, ES gets the same as EN (native terms stay, descriptions fall back)
        es.update(en_data)
        total_new += len(en_data)
        print(f"  {name}: {len(en_data)} keys")

    en = dict(sorted(en.items()))
    es = dict(sorted(es.items()))

    with open(EN_PATH, "w", encoding="utf-8") as f:
        json.dump(en, f, indent=2, ensure_ascii=False)
        f.write("\n")
    with open(ES_PATH, "w", encoding="utf-8") as f:
        json.dump(es, f, indent=2, ensure_ascii=False)
        f.write("\n")

    print(f"\nEN: {en_before} -> {len(en)} keys (+{len(en) - en_before})")
    print(f"ES: {len(es)} keys (EN fallback for now)")
    print(f"Total new: {total_new}")


if __name__ == "__main__":
    main()
