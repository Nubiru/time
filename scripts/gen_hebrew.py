#!/usr/bin/env python3
"""gen_hebrew.py — Generate Hebrew (he.json) from English content.

Translates high-confidence terms (zodiac, planets, months, days, sefirot,
elements, I Ching trigrams, system names) to Hebrew. Leaves complex
descriptions in English for Gabriel to verify and expand.

Run: python3 scripts/gen_hebrew.py
"""

import json
from pathlib import Path

PROJECT_ROOT = Path(__file__).parent.parent
EN_PATH = PROJECT_ROOT / "data" / "content" / "en.json"
HE_PATH = PROJECT_ROOT / "data" / "content" / "he.json"

# ============================================================
# Hebrew translation dictionaries
# ============================================================

# Exact value replacements (case-sensitive match on English value)
VALUE_MAP = {
    # Zodiac signs
    "Aries": "\u05d8\u05dc\u05d4",          # טלה
    "Taurus": "\u05e9\u05d5\u05e8",          # שור
    "Gemini": "\u05ea\u05d0\u05d5\u05de\u05d9\u05dd",  # תאומים
    "Cancer": "\u05e1\u05e8\u05d8\u05df",    # סרטן
    "Leo": "\u05d0\u05e8\u05d9\u05d4",       # אריה
    "Virgo": "\u05d1\u05ea\u05d5\u05dc\u05d4",  # בתולה
    "Libra": "\u05de\u05d0\u05d6\u05e0\u05d9\u05d9\u05dd",  # מאזניים
    "Scorpio": "\u05e2\u05e7\u05e8\u05d1",   # עקרב
    "Sagittarius": "\u05e7\u05e9\u05ea",     # קשת
    "Capricorn": "\u05d2\u05d3\u05d9",       # גדי
    "Aquarius": "\u05d3\u05dc\u05d9",        # דלי
    "Pisces": "\u05d3\u05d2\u05d9\u05dd",    # דגים

    # Elements
    "Fire": "\u05d0\u05e9",                  # אש
    "Earth": "\u05d0\u05d3\u05de\u05d4",     # אדמה
    "Air": "\u05d0\u05d5\u05d5\u05d9\u05e8", # אוויר
    "Water": "\u05de\u05d9\u05dd",           # מים

    # Modalities
    "Cardinal": "\u05e7\u05e8\u05d3\u05d9\u05e0\u05dc\u05d9",  # קרדינלי
    "Fixed": "\u05e7\u05d1\u05d5\u05e2",     # קבוע
    "Mutable": "\u05de\u05e9\u05ea\u05e0\u05d4",  # משתנה

    # Planets
    "Sun": "\u05e9\u05de\u05e9",             # שמש
    "Moon": "\u05d9\u05e8\u05d7",            # ירח
    "Mercury": "\u05db\u05d5\u05db\u05d1 \u05d7\u05de\u05d4",  # כוכב חמה
    "Venus": "\u05e0\u05d5\u05d2\u05d4",     # נוגה
    "Mars": "\u05de\u05d0\u05d3\u05d9\u05dd", # מאדים
    "Jupiter": "\u05e6\u05d3\u05e7",         # צדק
    "Saturn": "\u05e9\u05d1\u05ea\u05d0\u05d9",  # שבתאי

    # Sefirot
    "Keter": "\u05db\u05ea\u05e8",           # כתר
    "Chokhmah": "\u05d7\u05db\u05de\u05d4",  # חכמה
    "Binah": "\u05d1\u05d9\u05e0\u05d4",     # בינה
    "Chesed": "\u05d7\u05e1\u05d3",          # חסד
    "Gevurah": "\u05d2\u05d1\u05d5\u05e8\u05d4",  # גבורה
    "Tiferet": "\u05ea\u05e4\u05d0\u05e8\u05ea",  # תפארת
    "Netzach": "\u05e0\u05e6\u05d7",         # נצח
    "Hod": "\u05d4\u05d5\u05d3",             # הוד
    "Yesod": "\u05d9\u05e1\u05d5\u05d3",     # יסוד
    "Malkhut": "\u05de\u05dc\u05db\u05d5\u05ea",  # מלכות

    # I Ching trigrams
    "Heaven": "\u05e9\u05de\u05d9\u05dd",    # שמיים
    "Lake": "\u05d0\u05d2\u05dd",            # אגם
    "Thunder": "\u05e8\u05e2\u05dd",         # רעם
    "Wind": "\u05e8\u05d5\u05d7",            # רוח
    "Mountain": "\u05d4\u05e8",              # הר

    # Qualities
    "Wisdom": "\u05d7\u05db\u05de\u05d4",    # חכמה
    "Crown": "\u05db\u05ea\u05e8",           # כתר
    "Beauty": "\u05d9\u05d5\u05e4\u05d9",    # יופי
    "Strength": "\u05d2\u05d1\u05d5\u05e8\u05d4",  # גבורה
    "Foundation": "\u05d9\u05e1\u05d5\u05d3",  # יסוד
    "Victory": "\u05e0\u05d9\u05e6\u05d7\u05d5\u05df",  # ניצחון
    "Splendor": "\u05d4\u05d3\u05e8",        # הדר
    "Kingdom": "\u05de\u05dc\u05db\u05d5\u05ea",  # מלכות

    # Human Design types
    "Generator": "\u05d2\u05e0\u05e8\u05d8\u05d5\u05e8",  # גנרטור
    "Manifestor": "\u05de\u05e0\u05d9\u05e4\u05e1\u05d8\u05d5\u05e8",  # מניפסטור
    "Projector": "\u05e4\u05e8\u05d5\u05d9\u05e7\u05d8\u05d5\u05e8",  # פרויקטור
    "Reflector": "\u05e8\u05e4\u05dc\u05e7\u05d8\u05d5\u05e8",  # רפלקטור

    # Common archetypes
    "The Warrior": "\u05d4\u05dc\u05d5\u05d7\u05dd",      # הלוחם
    "The Teacher": "\u05d4\u05de\u05d5\u05e8\u05d4",       # המורה
    "The Healer": "\u05d4\u05de\u05e8\u05e4\u05d0",       # המרפא
    "The Mother": "\u05d4\u05d0\u05dd",                    # האם
    "The Builder": "\u05d4\u05d1\u05d5\u05e0\u05d4",       # הבונה
    "The Lover": "\u05d4\u05d0\u05d5\u05d4\u05d1",        # האוהב
    "The Mystic": "\u05d4\u05de\u05d9\u05e1\u05d8\u05d9\u05e7\u05df",  # המיסטיקן
    "The King": "\u05d4\u05de\u05dc\u05da",                # המלך
    "The Hermit": "\u05d4\u05e0\u05d6\u05d9\u05e8",        # הנזיר
    "The Magician": "\u05d4\u05e7\u05d5\u05e1\u05dd",      # הקוסם
    "The Twins": "\u05d4\u05ea\u05d0\u05d5\u05de\u05d9\u05dd",  # התאומים
    "The Nurturer": "\u05d4\u05de\u05d8\u05e4\u05dc\u05ea",  # המטפלת
    "The Performer": "\u05d4\u05de\u05d1\u05e6\u05e2",     # המבצע
    "The Analyst": "\u05d4\u05de\u05e0\u05ea\u05d7",       # המנתח
    "The Diplomat": "\u05d4\u05d3\u05d9\u05e4\u05dc\u05d5\u05de\u05d8",  # הדיפלומט
    "The Transformer": "\u05d4\u05de\u05e9\u05e0\u05d4",   # המשנה
    "The Explorer": "\u05d4\u05de\u05d2\u05dc\u05d4",      # המגלה
    "The Achiever": "\u05d4\u05de\u05e9\u05d9\u05d2",      # המשיג
    "The Visionary": "\u05d4\u05d7\u05d5\u05d6\u05d4",     # החוזה
    "The Dreamer": "\u05d4\u05d7\u05d5\u05dc\u05dd",       # החולם

    # Chinese zodiac animals
    "Rat": "\u05d7\u05d5\u05dc\u05d3\u05d4",              # חולדה
    "Ox": "\u05e9\u05d5\u05e8",                            # שור
    "Tiger": "\u05e0\u05de\u05e8",                         # נמר
    "Rabbit": "\u05d0\u05e8\u05e0\u05d1",                  # ארנב
    "Dragon": "\u05d3\u05e8\u05e7\u05d5\u05df",            # דרקון
    "Snake": "\u05e0\u05d7\u05e9",                         # נחש
    "Horse": "\u05e1\u05d5\u05e1",                         # סוס
    "Goat": "\u05e2\u05d6",                                # עז
    "Monkey": "\u05e7\u05d5\u05e3",                        # קוף
    "Rooster": "\u05ea\u05e8\u05e0\u05d2\u05d5\u05dc",    # תרנגול
    "Dog": "\u05db\u05dc\u05d1",                           # כלב
    "Pig": "\u05d7\u05d6\u05d9\u05e8",                     # חזיר

    # Chinese elements
    "Wood": "\u05e2\u05e5",                                # עץ
    "Metal": "\u05de\u05ea\u05db\u05ea",                   # מתכת

    # Tarot Major Arcana
    "The Fool": "\u05d4\u05e9\u05d5\u05d8\u05d4",         # השוטה
    "The High Priestess": "\u05d4\u05db\u05d5\u05d4\u05e0\u05ea \u05d4\u05d2\u05d3\u05d5\u05dc\u05d4",  # הכוהנת הגדולה
    "The Empress": "\u05d4\u05e7\u05d9\u05e1\u05e8\u05d9\u05ea",  # הקיסרית
    "The Emperor": "\u05d4\u05e7\u05d9\u05e1\u05e8",       # הקיסר
    "The Hierophant": "\u05d4\u05db\u05d5\u05d4\u05df \u05d4\u05d2\u05d3\u05d5\u05dc",  # הכוהן הגדול
    "The Lovers": "\u05d4\u05d0\u05d5\u05d4\u05d1\u05d9\u05dd",  # האוהבים
    "The Chariot": "\u05d4\u05de\u05e8\u05db\u05d1\u05d4",  # המרכבה
    "Justice": "\u05e6\u05d3\u05e7",                       # צדק
    "The Wheel of Fortune": "\u05d2\u05dc\u05d2\u05dc \u05d4\u05de\u05d6\u05dc",  # גלגל המזל
    "The Hanged Man": "\u05d4\u05ea\u05dc\u05d5\u05d9",   # התלוי
    "Death": "\u05de\u05d5\u05d5\u05ea",                   # מוות
    "Temperance": "\u05de\u05ea\u05d9\u05e0\u05d5\u05ea",  # מתינות
    "The Devil": "\u05d4\u05e9\u05d8\u05df",               # השטן
    "The Tower": "\u05d4\u05de\u05d2\u05d3\u05dc",         # המגדל
    "The Star": "\u05d4\u05db\u05d5\u05db\u05d1",          # הכוכב
    "The Moon": "\u05d4\u05d9\u05e8\u05d7",                # הירח
    "The Sun": "\u05d4\u05e9\u05de\u05e9",                 # השמש
    "Judgement": "\u05d4\u05d3\u05d9\u05df",               # הדין
    "The World": "\u05d4\u05e2\u05d5\u05dc\u05dd",         # העולם

    # Tarot suits
    "Wands": "\u05de\u05d8\u05d5\u05ea",                   # מטות
    "Cups": "\u05db\u05d5\u05e1\u05d5\u05ea",              # כוסות
    "Swords": "\u05d7\u05e8\u05d1\u05d5\u05ea",            # חרבות
    "Pentacles": "\u05de\u05d8\u05d1\u05e2\u05d5\u05ea",   # מטבעות

    # Astronomy bodies
    "Uranus": "\u05d0\u05d5\u05e8\u05e0\u05d5\u05e1",     # אורנוס
    "Neptune": "\u05e0\u05e4\u05d8\u05d5\u05df",           # נפטון
    "Pluto": "\u05e4\u05dc\u05d5\u05d8\u05d5",             # פלוטו

    # Convergence terms
    "ordinary": "\u05e8\u05d2\u05d9\u05dc",                # רגיל
    "notable": "\u05de\u05e9\u05de\u05e2\u05d5\u05ea\u05d9",  # משמעותי
    "significant": "\u05de\u05e9\u05de\u05e2\u05d5\u05ea\u05d9",  # משמעותי
    "rare": "\u05e0\u05d3\u05d9\u05e8",                    # נדיר
    "extraordinary": "\u05d9\u05d5\u05e6\u05d0 \u05d3\u05d5\u05e4\u05df",  # יוצא דופן

    # Common descriptive terms
    "North": "\u05e6\u05e4\u05d5\u05df",                   # צפון
    "South": "\u05d3\u05e8\u05d5\u05dd",                   # דרום
    "East": "\u05de\u05d6\u05e8\u05d7",                    # מזרח
    "West": "\u05de\u05e2\u05e8\u05d1",                    # מערב
    "Spring": "\u05d0\u05d1\u05d9\u05d1",                  # אביב
    "Summer": "\u05e7\u05d9\u05e5",                        # קיץ
    "Autumn": "\u05e1\u05ea\u05d9\u05d5",                  # סתיו
    "Winter": "\u05d7\u05d5\u05e8\u05e3",                  # חורף
    "Light": "\u05d0\u05d5\u05e8",                         # אור
    "Darkness": "\u05d7\u05d5\u05e9\u05da",                # חושך
    "Life": "\u05d7\u05d9\u05d9\u05dd",                    # חיים
    "Death": "\u05de\u05d5\u05d5\u05ea",                   # מוות
    "Love": "\u05d0\u05d4\u05d1\u05d4",                    # אהבה
    "Peace": "\u05e9\u05dc\u05d5\u05dd",                   # שלום
    "Truth": "\u05d0\u05de\u05ea",                         # אמת
    "Joy": "\u05e9\u05de\u05d7\u05d4",                     # שמחה
    "Creation": "\u05d1\u05e8\u05d9\u05d0\u05d4",          # בריאה
    "Harmony": "\u05d4\u05e8\u05de\u05d5\u05e0\u05d9\u05d4",  # הרמוניה
    "Balance": "\u05d0\u05d9\u05d6\u05d5\u05df",           # איזון
    "Power": "\u05db\u05d5\u05d7",                         # כוח
    "Transformation": "\u05d4\u05e9\u05ea\u05e0\u05d5\u05ea",  # השתנות
    "Renewal": "\u05d4\u05ea\u05d7\u05d3\u05e9\u05d5\u05ea",  # התחדשות
    "Unity": "\u05d0\u05d7\u05d3\u05d5\u05ea",             # אחדות
    "Compassion": "\u05d7\u05de\u05dc\u05d4",              # חמלה
    "Meditation": "\u05de\u05d3\u05d9\u05d8\u05e6\u05d9\u05d4",  # מדיטציה
    "Consciousness": "\u05ea\u05d5\u05d3\u05e2\u05d4",     # תודעה
}

# Key-pattern-based translations (for keys matching specific patterns)
# Format: (key_contains, field_suffix, value_contains) -> hebrew_value
KEY_TRANSLATIONS = {
    # Gregorian months — translate the month names
    "gregorian.month.1.name": "\u05d9\u05e0\u05d5\u05d0\u05e8",     # ינואר
    "gregorian.month.2.name": "\u05e4\u05d1\u05e8\u05d5\u05d0\u05e8",  # פברואר
    "gregorian.month.3.name": "\u05de\u05e8\u05e5",                  # מרץ
    "gregorian.month.4.name": "\u05d0\u05e4\u05e8\u05d9\u05dc",     # אפריל
    "gregorian.month.5.name": "\u05de\u05d0\u05d9",                  # מאי
    "gregorian.month.6.name": "\u05d9\u05d5\u05e0\u05d9",           # יוני
    "gregorian.month.7.name": "\u05d9\u05d5\u05dc\u05d9",           # יולי
    "gregorian.month.8.name": "\u05d0\u05d5\u05d2\u05d5\u05e1\u05d8",  # אוגוסט
    "gregorian.month.9.name": "\u05e1\u05e4\u05d8\u05de\u05d1\u05e8",  # ספטמבר
    "gregorian.month.10.name": "\u05d0\u05d5\u05e7\u05d8\u05d5\u05d1\u05e8",  # אוקטובר
    "gregorian.month.11.name": "\u05e0\u05d5\u05d1\u05de\u05d1\u05e8",  # נובמבר
    "gregorian.month.12.name": "\u05d3\u05e6\u05de\u05d1\u05e8",     # דצמבר

    # Gregorian days
    "gregorian.day.0.name": "\u05d9\u05d5\u05dd \u05e8\u05d0\u05e9\u05d5\u05df",  # יום ראשון
    "gregorian.day.1.name": "\u05d9\u05d5\u05dd \u05e9\u05e0\u05d9",  # יום שני
    "gregorian.day.2.name": "\u05d9\u05d5\u05dd \u05e9\u05dc\u05d9\u05e9\u05d9",  # יום שלישי
    "gregorian.day.3.name": "\u05d9\u05d5\u05dd \u05e8\u05d1\u05d9\u05e2\u05d9",  # יום רביעי
    "gregorian.day.4.name": "\u05d9\u05d5\u05dd \u05d7\u05de\u05d9\u05e9\u05d9",  # יום חמישי
    "gregorian.day.5.name": "\u05d9\u05d5\u05dd \u05e9\u05d9\u05e9\u05d9",  # יום שישי
    "gregorian.day.6.name": "\u05e9\u05d1\u05ea",                    # שבת

    # Gregorian seasons
    "gregorian.season.0.name": "\u05d0\u05d1\u05d9\u05d1",  # אביב (Spring)
    "gregorian.season.1.name": "\u05e7\u05d9\u05e5",        # קיץ (Summer)
    "gregorian.season.2.name": "\u05e1\u05ea\u05d9\u05d5",  # סתיו (Autumn)
    "gregorian.season.3.name": "\u05d7\u05d5\u05e8\u05e3",  # חורף (Winter)

    # Hebrew months (already Hebrew names — keep originals)
    "hebrew.month.1.name": "\u05e0\u05d9\u05e1\u05df",      # ניסן
    "hebrew.month.2.name": "\u05d0\u05d9\u05d9\u05e8",      # אייר
    "hebrew.month.3.name": "\u05e1\u05d9\u05d5\u05df",      # סיוון
    "hebrew.month.4.name": "\u05ea\u05de\u05d5\u05d6",      # תמוז
    "hebrew.month.5.name": "\u05d0\u05d1",                  # אב
    "hebrew.month.6.name": "\u05d0\u05dc\u05d5\u05dc",      # אלול
    "hebrew.month.7.name": "\u05ea\u05e9\u05e8\u05d9",      # תשרי
    "hebrew.month.8.name": "\u05d7\u05e9\u05d5\u05d5\u05df",  # חשוון
    "hebrew.month.9.name": "\u05db\u05e1\u05dc\u05d5",      # כסלו
    "hebrew.month.10.name": "\u05d8\u05d1\u05ea",           # טבת
    "hebrew.month.11.name": "\u05e9\u05d1\u05d8",           # שבט
    "hebrew.month.12.name": "\u05d0\u05d3\u05e8",           # אדר

    # Hebrew holidays
    "hebrew.holiday.0.name": "\u05e8\u05d0\u05e9 \u05d4\u05e9\u05e0\u05d4",  # ראש השנה
    "hebrew.holiday.1.name": "\u05d9\u05d5\u05dd \u05db\u05d9\u05e4\u05d5\u05e8",  # יום כיפור
    "hebrew.holiday.2.name": "\u05e1\u05d5\u05db\u05d5\u05ea",  # סוכות
    "hebrew.holiday.3.name": "\u05e4\u05e1\u05d7",          # פסח
    "hebrew.holiday.4.name": "\u05e9\u05d1\u05d5\u05e2\u05d5\u05ea",  # שבועות
    "hebrew.holiday.5.name": "\u05d7\u05e0\u05d5\u05db\u05d4",  # חנוכה
    "hebrew.holiday.6.name": "\u05e4\u05d5\u05e8\u05d9\u05dd",  # פורים

    # Islamic months (Hebrew transliteration)
    "islamic.month.1.name": "\u05de\u05d5\u05d7\u05e8\u05dd",  # מוחרם
    "islamic.month.2.name": "\u05e6\u05e4\u05e8",           # צפר
    "islamic.month.3.name": "\u05e8\u05d1\u05d9\u05e2 \u05d0\u05dc-\u05d0\u05d5\u05d5\u05dc",  # רביע אל-אוול
    "islamic.month.4.name": "\u05e8\u05d1\u05d9\u05e2 \u05d0\u05dc-\u05ea\u05e0\u05d9",  # רביע אל-תני
    "islamic.month.5.name": "\u05d2\u05f3\u05d5\u05de\u05d0\u05d3\u05d0 \u05d0\u05dc-\u05d0\u05d5\u05dc\u05d0",  # ג'ומאדא אל-אולא
    "islamic.month.6.name": "\u05d2\u05f3\u05d5\u05de\u05d0\u05d3\u05d0 \u05d0\u05dc-\u05ea\u05e0\u05d9\u05d4",  # ג'ומאדא אל-תניה
    "islamic.month.7.name": "\u05e8\u05d2\u05f3\u05d1",     # רג'ב
    "islamic.month.8.name": "\u05e9\u05e2\u05d1\u05d0\u05df",  # שעבאן
    "islamic.month.9.name": "\u05e8\u05de\u05d3\u05d0\u05df",  # רמדאן
    "islamic.month.10.name": "\u05e9\u05d5\u05d5\u05d0\u05dc",  # שוואל
    "islamic.month.11.name": "\u05d3\u05f3\u05d5 \u05d0\u05dc-\u05e7\u05e2\u05d3\u05d4",  # ד'ו אל-קעדה
    "islamic.month.12.name": "\u05d3\u05f3\u05d5 \u05d0\u05dc-\u05d7\u05d9\u05d2\u05f3\u05d4",  # ד'ו אל-חיג'ה
}


def translate(key, value):
    """Translate a single key-value pair to Hebrew."""
    # 1. Check exact key match
    if key in KEY_TRANSLATIONS:
        return KEY_TRANSLATIONS[key]

    # 2. Check exact value match (for names/terms)
    if value in VALUE_MAP:
        return VALUE_MAP[value]

    # 3. Return English as fallback
    return value


def main():
    with open(EN_PATH, "r", encoding="utf-8") as f:
        en = json.load(f)

    he = {}
    translated = 0
    total = 0

    for key, value in en.items():
        total += 1
        he_val = translate(key, value)
        he[key] = he_val
        if he_val != value:
            translated += 1

    he = dict(sorted(he.items()))

    with open(HE_PATH, "w", encoding="utf-8") as f:
        json.dump(he, f, indent=2, ensure_ascii=False)
        f.write("\n")

    pct = (translated / total * 100) if total > 0 else 0
    print(f"Hebrew translation: {translated}/{total} keys ({pct:.1f}%)")
    print(f"Written to: {HE_PATH}")


if __name__ == "__main__":
    main()
