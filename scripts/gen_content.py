#!/usr/bin/env python3
"""gen_content.py — Generate C content translation header from JSON locale files.

Reads data/content/*.json and produces a single C header file with
static const string arrays for each locale's content translations.

Usage:
    python3 scripts/gen_content.py [--output src/ui/content_generated.h]

The generated header contains:
    - Per-locale sorted key-value arrays (reusing i18n_entry_t)
    - COUNT macros for each locale
    - English defaults as the canonical key set

Requires: Python 3.6+ (f-strings, pathlib)
No external dependencies.
"""

import json
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).parent.parent
CONTENT_DIR = PROJECT_ROOT / "data" / "content"
DEFAULT_OUTPUT = PROJECT_ROOT / "src" / "ui" / "content_generated.h"

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
    """Convert a UTF-8 string to a C string literal with hex escapes."""
    result = []
    encoded = text.encode("utf-8")
    in_hex = False

    for byte in encoded:
        if byte < 0x80:
            char = chr(byte)
            if in_hex and char in HEX_DIGITS:
                result.append('" "')
            in_hex = False
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
            result.append(f"\\x{byte:02x}")
            in_hex = True

    return '"' + "".join(result) + '"'


def load_content():
    """Load all JSON content files from data/content/."""
    content = {}
    for json_file in sorted(CONTENT_DIR.glob("*.json")):
        locale = json_file.stem
        if locale not in LOCALE_ENUM:
            print(f"Warning: {json_file.name} — unknown locale '{locale}', skipping")
            continue
        with open(json_file, "r", encoding="utf-8") as f:
            data = json.load(f)
        content[locale] = data
        print(f"  Loaded {locale}: {len(data)} keys")
    return content


def generate_header(content, output_path):
    """Generate the C header file."""
    if "en" not in content:
        print("Error: en.json required for canonical key set")
        sys.exit(1)

    en_keys = sorted(content["en"].keys())
    lines = []
    lines.append("/* content_generated.h — AUTO-GENERATED from data/content JSON files.")
    lines.append(" * Do not edit manually. Run: python3 scripts/gen_content.py")
    lines.append(" * Content translation arrays for content_i18n.c. */")
    lines.append("")
    lines.append("#ifndef TIME_CONTENT_GENERATED_H")
    lines.append("#define TIME_CONTENT_GENERATED_H")
    lines.append("")
    lines.append('#include "i18n.h"')
    lines.append("")

    # English defaults (canonical)
    lines.append(f"/* ---- EN ({len(en_keys)} keys, canonical) ---- */")
    lines.append("")
    lines.append("static const i18n_entry_t gen_content_en[] = {")
    for key in en_keys:
        c_val = utf8_to_c_string(content["en"][key])
        lines.append(f'    {{ "{key}", {c_val} }},')
    lines.append("};")
    lines.append("")
    lines.append("#define GEN_CONTENT_EN_COUNT \\")
    lines.append("    ((int)(sizeof(gen_content_en) / sizeof(gen_content_en[0])))")
    lines.append("")

    # Other locales
    for locale in sorted(content.keys()):
        if locale == "en":
            continue
        data = content[locale]
        array_name = f"gen_content_{locale}"
        count_name = f"GEN_CONTENT_{locale.upper()}_COUNT"

        # Only emit keys that exist in en (canonical)
        locale_keys = sorted(k for k in en_keys if k in data)

        lines.append(f"/* ---- {locale.upper()} ({len(locale_keys)} keys) ---- */")
        lines.append("")
        lines.append(f"static const i18n_entry_t {array_name}[] = {{")
        for key in locale_keys:
            c_val = utf8_to_c_string(data[key])
            lines.append(f'    {{ "{key}", {c_val} }},')
        lines.append("};")
        lines.append("")
        lines.append(f"#define {count_name} \\")
        lines.append(f"    ((int)(sizeof({array_name}) / sizeof({array_name}[0])))")
        lines.append("")

    lines.append("#endif /* TIME_CONTENT_GENERATED_H */")
    lines.append("")

    output_path.parent.mkdir(parents=True, exist_ok=True)
    with open(output_path, "w", encoding="utf-8") as f:
        f.write("\n".join(lines))

    total = sum(len(v) for k, v in content.items() if k != "en")
    print(f"\nGenerated: {output_path}")
    print(f"  EN keys: {len(en_keys)}")
    print(f"  Locales: {len(content) - 1}")
    print(f"  Total translated strings: {total}")


def main():
    output = DEFAULT_OUTPUT
    if len(sys.argv) > 2 and sys.argv[1] == "--output":
        output = Path(sys.argv[2])

    print("gen_content.py — Content JSON to C header codegen")
    print(f"  Source: {CONTENT_DIR}")
    print(f"  Output: {output}")
    print()

    if not CONTENT_DIR.exists():
        print(f"Error: {CONTENT_DIR} does not exist")
        sys.exit(1)

    print("Loading content:")
    content = load_content()
    if not content:
        print("No content files found.")
        sys.exit(1)

    # Validate
    en_keys = set(content.get("en", {}).keys())
    for locale, data in content.items():
        if locale == "en":
            continue
        missing = [k for k in sorted(en_keys) if k not in data]
        extra = [k for k in sorted(data.keys()) if k not in en_keys]
        if missing:
            print(f"  Warning: {locale} missing {len(missing)} keys: {missing[:3]}...")
        if extra:
            print(f"  Warning: {locale} has {len(extra)} extra keys: {extra[:3]}...")

    generate_header(content, output)
    print("\nDone.")


if __name__ == "__main__":
    main()
