#!/usr/bin/env python3
"""gen_translations.py — Generate C translation headers from JSON locale files.

Reads data/translations/*.json and produces a single C header file with
static const translation arrays for each locale. Output is suitable for
#include in i18n.c or as a standalone compilation unit.

Usage:
    python3 scripts/gen_translations.py [--output src/ui/i18n_generated.h]

The generated header contains:
    - Per-locale i18n_entry_t arrays (key-value pairs)
    - COUNT macros for each locale
    - A dispatch table mapping locale enum -> array

UTF-8 strings are emitted using hex escapes with string concatenation
breaks where a hex escape is followed by a hex digit (a-f, A-F, 0-9).

Requires: Python 3.6+ (f-strings, pathlib)
No external dependencies.
"""

import json
import os
import sys
from pathlib import Path

# Project root (scripts/ is one level down)
PROJECT_ROOT = Path(__file__).parent.parent
TRANSLATIONS_DIR = PROJECT_ROOT / "data" / "translations"
DEFAULT_OUTPUT = PROJECT_ROOT / "src" / "ui" / "i18n_generated.h"

# Locale enum names (must match i18n.h I18N_LOCALE_* order)
LOCALE_ENUM = {
    "en": "I18N_LOCALE_EN",
    "es": "I18N_LOCALE_ES",
    "ar": "I18N_LOCALE_AR",
    "he": "I18N_LOCALE_HE",
    "zh": "I18N_LOCALE_ZH",
    "hi": "I18N_LOCALE_HI",
    "ja": "I18N_LOCALE_JA",
    "fr": "I18N_LOCALE_FR",
    "pt": "I18N_LOCALE_PT",
    "de": "I18N_LOCALE_DE",
    "ru": "I18N_LOCALE_RU",
    "ko": "I18N_LOCALE_KO",
    "th": "I18N_LOCALE_TH",
    "tr": "I18N_LOCALE_TR",
    "id": "I18N_LOCALE_ID",
    "my": "I18N_LOCALE_MY",
    "am": "I18N_LOCALE_AM",
    "bn": "I18N_LOCALE_BN",
    "vi": "I18N_LOCALE_VI",
    "sw": "I18N_LOCALE_SW",
    "fa": "I18N_LOCALE_FA",
}

HEX_DIGITS = set("0123456789abcdefABCDEF")


def utf8_to_c_string(text):
    """Convert a UTF-8 string to a C string literal with hex escapes.

    Non-ASCII bytes are encoded as \\xNN sequences. String concatenation
    ("" "") is inserted where a hex escape is followed by a hex digit
    to prevent GCC from consuming extra digits.
    """
    result = []
    encoded = text.encode("utf-8")
    i = 0
    in_hex = False  # True if the last emitted char was a hex escape

    while i < len(encoded):
        byte = encoded[i]
        if byte < 0x80:
            # ASCII character
            char = chr(byte)
            if in_hex and char in HEX_DIGITS:
                # Break string to prevent hex digit consumption
                result.append('" "')
            in_hex = False

            # Escape special C characters
            if char == '"':
                result.append('\\"')
            elif char == '\\':
                result.append('\\\\')
            elif char == '\n':
                result.append('\\n')
            elif char == '\t':
                result.append('\\t')
            else:
                result.append(char)
        else:
            # Non-ASCII: emit hex escape
            if in_hex and False:
                # Consecutive hex escapes don't need breaks (\xNN\xNN is fine)
                pass
            result.append(f"\\x{byte:02x}")
            in_hex = True

        i += 1

    return '"' + "".join(result) + '"'


def load_translations():
    """Load all JSON translation files from data/translations/."""
    translations = {}
    for json_file in sorted(TRANSLATIONS_DIR.glob("*.json")):
        locale = json_file.stem  # e.g., "es" from "es.json"
        if locale not in LOCALE_ENUM:
            print(f"Warning: {json_file.name} — unknown locale '{locale}', skipping")
            continue
        with open(json_file, "r", encoding="utf-8") as f:
            data = json.load(f)
        translations[locale] = data
        print(f"  Loaded {locale}: {len(data)} keys")
    return translations


def load_key_order():
    """Load the canonical key order from en.json."""
    en_path = TRANSLATIONS_DIR / "en.json"
    if not en_path.exists():
        print(f"Error: {en_path} not found (needed for key ordering)")
        sys.exit(1)
    with open(en_path, "r", encoding="utf-8") as f:
        en = json.load(f)
    return list(en.keys())


def generate_header(translations, key_order, output_path):
    """Generate the C header file."""
    lines = []
    lines.append("/* i18n_generated.h — AUTO-GENERATED from data/translations JSON files.")
    lines.append(" * Do not edit manually. Run: python3 scripts/gen_translations.py")
    lines.append(" * Generated locale arrays for i18n.c locale dispatch table. */")
    lines.append("")
    lines.append("#ifndef TIME_I18N_GENERATED_H")
    lines.append("#define TIME_I18N_GENERATED_H")
    lines.append("")
    lines.append('#include "i18n.h"')
    lines.append("")

    locale_arrays = {}  # locale -> array_name

    for locale in sorted(translations.keys()):
        if locale == "en":
            continue  # English is the default, not stored in locale arrays

        data = translations[locale]
        enum_name = LOCALE_ENUM[locale]
        array_name = f"gen_{locale}_strings"
        count_name = f"GEN_{locale.upper()}_COUNT"
        locale_arrays[locale] = (array_name, count_name)

        lines.append(f"/* ---- {locale.upper()} ({len(data)} keys) ---- */")
        lines.append("")
        lines.append(f"static const i18n_entry_t {array_name}[] = {{")

        for key in key_order:
            if key in data:
                c_val = utf8_to_c_string(data[key])
                lines.append(f'    {{ "{key}", {c_val} }},')

        lines.append("};")
        lines.append("")
        lines.append(f"#define {count_name} \\")
        lines.append(f"    ((int)(sizeof({array_name}) / sizeof({array_name}[0])))")
        lines.append("")

    # Generate dispatch table helper
    lines.append("/* Dispatch table entries for generated locales.")
    lines.append(" * Add these to locale_tables[] in i18n.c: */")
    lines.append("/*")
    for locale, (arr, cnt) in sorted(locale_arrays.items()):
        enum = LOCALE_ENUM[locale]
        lines.append(f"    [{enum}] = {{ {arr}, {cnt} }},")
    lines.append("*/")
    lines.append("")
    lines.append("#endif /* TIME_I18N_GENERATED_H */")
    lines.append("")

    output_path.parent.mkdir(parents=True, exist_ok=True)
    with open(output_path, "w", encoding="utf-8") as f:
        f.write("\n".join(lines))

    total_keys = sum(len(v) for k, v in translations.items() if k != "en")
    print(f"\nGenerated: {output_path}")
    print(f"  Locales: {len(locale_arrays)} (excluding en)")
    print(f"  Total translated strings: {total_keys}")


def main():
    output = DEFAULT_OUTPUT
    if len(sys.argv) > 2 and sys.argv[1] == "--output":
        output = Path(sys.argv[2])

    print("gen_translations.py — JSON to C header codegen")
    print(f"  Source: {TRANSLATIONS_DIR}")
    print(f"  Output: {output}")
    print()

    if not TRANSLATIONS_DIR.exists():
        print(f"Error: {TRANSLATIONS_DIR} does not exist")
        sys.exit(1)

    print("Loading translations:")
    translations = load_translations()

    if not translations:
        print("No translation files found.")
        sys.exit(1)

    key_order = load_key_order()
    print(f"\nKey order: {len(key_order)} keys from en.json")

    # Validate: check all locales have all keys
    for locale, data in translations.items():
        if locale == "en":
            continue
        missing = [k for k in key_order if k not in data]
        extra = [k for k in data if k not in key_order]
        if missing:
            print(f"  Warning: {locale} missing {len(missing)} keys: {missing[:3]}...")
        if extra:
            print(f"  Warning: {locale} has {len(extra)} extra keys: {extra[:3]}...")

    generate_header(translations, key_order, output)
    print("\nDone.")


if __name__ == "__main__":
    main()
