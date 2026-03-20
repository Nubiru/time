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
