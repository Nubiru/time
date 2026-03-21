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

    # I Ching hexagram names
    "The Creative": "\u05d4\u05d9\u05d5\u05e6\u05e8",       # היוצר
    "The Receptive": "\u05d4\u05e7\u05d5\u05dc\u05d8\u05ea", # הקולטת
    "Difficulty at the Beginning": "\u05e7\u05d5\u05e9\u05d9 \u05d1\u05d4\u05ea\u05d7\u05dc\u05d4",
    "Youthful Folly": "\u05d0\u05d9\u05d5\u05d5\u05dc\u05ea \u05e0\u05e2\u05d5\u05e8\u05d9\u05dd",
    "Waiting": "\u05d4\u05de\u05ea\u05e0\u05d4",
    "Conflict": "\u05e1\u05db\u05e1\u05d5\u05da",
    "The Army": "\u05d4\u05e6\u05d1\u05d0",
    "Holding Together": "\u05d0\u05d7\u05d3\u05d5\u05ea",
    "Small Taming": "\u05e8\u05d9\u05e1\u05d5\u05df \u05e7\u05d8\u05df",
    "Treading": "\u05d3\u05e8\u05d9\u05db\u05d4",
    "Peace": "\u05e9\u05dc\u05d5\u05dd",
    "Standstill": "\u05e7\u05d9\u05e4\u05d0\u05d5\u05df",
    "Fellowship": "\u05d0\u05d7\u05d5\u05d5\u05d4",
    "Great Possession": "\u05e7\u05e0\u05d9\u05d9\u05df \u05d2\u05d3\u05d5\u05dc",
    "Modesty": "\u05e6\u05e0\u05d9\u05e2\u05d5\u05ea",
    "Enthusiasm": "\u05d4\u05ea\u05dc\u05d4\u05d1\u05d5\u05ea",
    "Following": "\u05de\u05e2\u05e7\u05d1",
    "Work on the Decayed": "\u05ea\u05d9\u05e7\u05d5\u05df \u05d4\u05e0\u05e8\u05e7\u05d1",
    "Approach": "\u05d4\u05ea\u05e7\u05e8\u05d1\u05d5\u05ea",
    "Contemplation": "\u05d4\u05ea\u05d1\u05d5\u05e0\u05e0\u05d5\u05ea",
    "Biting Through": "\u05e0\u05e9\u05d9\u05db\u05d4 \u05d7\u05d5\u05d3\u05e8\u05ea",
    "Grace": "\u05d7\u05df",
    "Splitting Apart": "\u05d4\u05ea\u05e4\u05e8\u05e7\u05d5\u05ea",
    "Return": "\u05d7\u05d6\u05e8\u05d4",
    "Innocence": "\u05ea\u05de\u05d9\u05de\u05d5\u05ea",
    "Great Taming": "\u05e8\u05d9\u05e1\u05d5\u05df \u05d2\u05d3\u05d5\u05dc",
    "Nourishment": "\u05d4\u05d6\u05e0\u05d4",
    "Great Preponderance": "\u05e2\u05d5\u05d3\u05e3 \u05d2\u05d3\u05d5\u05dc",
    "The Abysmal": "\u05d4\u05ea\u05d4\u05d5\u05dd",
    "The Clinging": "\u05d4\u05e0\u05d0\u05d7\u05d6\u05ea",
    "Influence": "\u05d4\u05e9\u05e4\u05e2\u05d4",
    "Duration": "\u05de\u05e9\u05da",
    "Retreat": "\u05e0\u05e1\u05d9\u05d2\u05d4",
    "Great Power": "\u05db\u05d5\u05d7 \u05d2\u05d3\u05d5\u05dc",
    "Progress": "\u05d4\u05ea\u05e7\u05d3\u05de\u05d5\u05ea",
    "Darkening of the Light": "\u05d4\u05d7\u05e9\u05db\u05ea \u05d4\u05d0\u05d5\u05e8",
    "The Family": "\u05d4\u05de\u05e9\u05e4\u05d7\u05d4",
    "Opposition": "\u05e0\u05d9\u05d2\u05d5\u05d3",
    "Obstruction": "\u05de\u05db\u05e9\u05d5\u05dc",
    "Deliverance": "\u05e9\u05d7\u05e8\u05d5\u05e8",
    "Decrease": "\u05de\u05d9\u05e2\u05d5\u05d8",
    "Increase": "\u05ea\u05d5\u05e1\u05e4\u05ea",
    "Breakthrough": "\u05e4\u05e8\u05d9\u05e6\u05ea \u05d3\u05e8\u05da",
    "Coming to Meet": "\u05de\u05e4\u05d2\u05e9",
    "Gathering Together": "\u05d4\u05ea\u05db\u05e0\u05e1\u05d5\u05ea",
    "Pushing Upward": "\u05e2\u05dc\u05d9\u05d9\u05d4",
    "Oppression": "\u05d3\u05d9\u05db\u05d5\u05d9",
    "The Well": "\u05d4\u05d1\u05d0\u05e8",
    "Revolution": "\u05de\u05d4\u05e4\u05db\u05d4",
    "The Cauldron": "\u05d4\u05d3\u05d5\u05d3",
    "The Arousing": "\u05d4\u05de\u05e2\u05d5\u05e8\u05e8",
    "Keeping Still": "\u05e9\u05e7\u05d8",
    "Development": "\u05d4\u05ea\u05e4\u05ea\u05d7\u05d5\u05ea",
    "The Marrying Maiden": "\u05d4\u05e0\u05e2\u05e8\u05d4 \u05d4\u05e0\u05d9\u05e9\u05d0\u05ea",
    "Abundance": "\u05e9\u05e4\u05e2",
    "The Wanderer": "\u05d4\u05e0\u05d5\u05d3\u05d3",
    "The Gentle": "\u05d4\u05e2\u05d3\u05d9\u05e0\u05d4",
    "The Joyous": "\u05d4\u05e9\u05de\u05d7\u05d4",
    "Dispersion": "\u05e4\u05d9\u05d6\u05d5\u05e8",
    "Limitation": "\u05d4\u05d2\u05d1\u05dc\u05d4",
    "Inner Truth": "\u05d0\u05de\u05ea \u05e4\u05e0\u05d9\u05de\u05d9\u05ea",
    "Small Preponderance": "\u05e2\u05d5\u05d3\u05e3 \u05e7\u05d8\u05df",
    "After Completion": "\u05dc\u05d0\u05d7\u05e8 \u05d4\u05e9\u05dc\u05de\u05d4",
    "Before Completion": "\u05dc\u05e4\u05e0\u05d9 \u05d4\u05e9\u05dc\u05de\u05d4",

    # Tzolkin seal names
    "Dragon": "\u05d3\u05e8\u05e7\u05d5\u05df",              # דרקון
    "Night": "\u05dc\u05d9\u05dc\u05d4",                     # לילה
    "Seed": "\u05d6\u05e8\u05e2",                             # זרע
    "Serpent": "\u05e0\u05d7\u05e9",                          # נחש (already in Chinese zodiac)
    "World-Bridger": "\u05de\u05d2\u05e9\u05e8 \u05e2\u05d5\u05dc\u05de\u05d5\u05ea",
    "Hand": "\u05d9\u05d3",                                   # יד
    "Star": "\u05db\u05d5\u05db\u05d1",                       # כוכב
    "Skywalker": "\u05d4\u05d5\u05dc\u05da \u05e9\u05de\u05d9\u05dd",
    "Wizard": "\u05e7\u05d5\u05e1\u05dd",                    # קוסם
    "Eagle": "\u05e0\u05e9\u05e8",                            # נשר
    "Warrior": "\u05dc\u05d5\u05d7\u05dd",                    # לוחם
    "Mirror": "\u05de\u05e8\u05d0\u05d4",                     # מראה
    "Storm": "\u05e1\u05e2\u05e8\u05d4",                      # סערה

    # Tzolkin tone names
    "Magnetic": "\u05de\u05d2\u05e0\u05d8\u05d9",
    "Lunar": "\u05d9\u05e8\u05d7\u05d9",
    "Electric": "\u05d7\u05e9\u05de\u05dc\u05d9",
    "Self-Existing": "\u05e2\u05e6\u05de\u05d9",
    "Overtone": "\u05e2\u05dc-\u05d8\u05d5\u05e0\u05d9",
    "Rhythmic": "\u05e8\u05d9\u05ea\u05de\u05d9",
    "Resonant": "\u05de\u05d4\u05d3\u05d4\u05d3",
    "Galactic": "\u05d2\u05dc\u05e7\u05d8\u05d9",
    "Solar": "\u05e9\u05de\u05e9\u05d9",
    "Planetary": "\u05e4\u05dc\u05e0\u05d8\u05e8\u05d9",
    "Spectral": "\u05e1\u05e4\u05e7\u05d8\u05e8\u05dc\u05d9",
    "Crystal": "\u05e7\u05e8\u05d9\u05e1\u05d8\u05dc\u05d9",
    "Cosmic": "\u05e7\u05d5\u05e1\u05de\u05d9",

    # Kabbalah sefirah meditations (Hebrew)
    "kabbalah.sefirah.0.meditation": "\u05d0\u05d9\u05df \u05e1\u05d5\u05e3 \u05d5\u05d0\u05d9\u05df \u05ea\u05d7\u05d9\u05dc\u05d4",
    "kabbalah.sefirah.1.meditation": "\u05d7\u05db\u05de\u05d4 \u05e2\u05dc\u05d9\u05d5\u05e0\u05d4",
    "kabbalah.sefirah.2.meditation": "\u05d1\u05d9\u05e0\u05d4 \u05e2\u05de\u05d5\u05e7\u05d4",
    "kabbalah.sefirah.3.meditation": "\u05d0\u05d4\u05d1\u05ea \u05d7\u05d9\u05e0\u05dd",
    "kabbalah.sefirah.4.meditation": "\u05d2\u05d1\u05d5\u05e8\u05d4 \u05de\u05de\u05d5\u05e7\u05d3\u05ea",
    "kabbalah.sefirah.5.meditation": "\u05d9\u05d5\u05e4\u05d9 \u05de\u05d0\u05d5\u05d6\u05df",
    "kabbalah.sefirah.6.meditation": "\u05e0\u05e6\u05d7\u05d5\u05df \u05de\u05ea\u05de\u05d9\u05d3",
    "kabbalah.sefirah.7.meditation": "\u05d4\u05d5\u05d3 \u05d5\u05d4\u05db\u05e8\u05ea \u05ea\u05d5\u05d3\u05d4",
    "kabbalah.sefirah.8.meditation": "\u05d9\u05e1\u05d5\u05d3 \u05d5\u05d7\u05d9\u05d1\u05d5\u05e8",
    "kabbalah.sefirah.9.meditation": "\u05de\u05dc\u05db\u05d5\u05ea \u05d5\u05e0\u05d5\u05db\u05d7\u05d5\u05ea",

    # Hindu deities and terms
    "Vishnu": "\u05d5\u05d9\u05e9\u05e0\u05d5",
    "Shiva": "\u05e9\u05d9\u05d5\u05d4",
    "Brahma": "\u05d1\u05e8\u05d4\u05de\u05d0",
    "Ganesha": "\u05d2\u05e0\u05e9\u05d4",
    "Lakshmi": "\u05dc\u05e7\u05e9\u05de\u05d9",
    "Saraswati": "\u05e1\u05e8\u05e1\u05d5\u05d5\u05d8\u05d9",
    "Durga": "\u05d3\u05d5\u05e8\u05d2\u05d4",
    "Parvati": "\u05e4\u05e8\u05d5\u05d5\u05d8\u05d9",
    "Hanuman": "\u05d4\u05e0\u05d5\u05de\u05df",
    "Krishna": "\u05e7\u05e8\u05d9\u05e9\u05e0\u05d4",
    "Rama": "\u05e8\u05d0\u05de\u05d4",
    "Indra": "\u05d0\u05d9\u05e0\u05d3\u05e8\u05d4",
    "Agni": "\u05d0\u05d2\u05e0\u05d9",
    "Varuna": "\u05d5\u05e8\u05d5\u05e0\u05d4",

    # Common qualities/states
    "Neutral": "\u05e0\u05d9\u05d8\u05e8\u05dc\u05d9",
    "Auspicious": "\u05de\u05d1\u05d5\u05e8\u05da",
    "Very Auspicious": "\u05de\u05d1\u05d5\u05e8\u05da \u05de\u05d0\u05d5\u05d3",
    "Inauspicious": "\u05dc\u05d0 \u05de\u05d1\u05d5\u05e8\u05da",
    "Hot": "\u05d7\u05dd",
    "Cool": "\u05e7\u05e8\u05d9\u05e8",
    "Rainy": "\u05d2\u05e9\u05d5\u05dd",
    "Dry": "\u05d9\u05d1\u05e9",
    "Warm": "\u05d7\u05de\u05d9\u05dd",
    "Cold": "\u05e7\u05e8",
    "Temperate": "\u05de\u05de\u05d5\u05d6\u05d2",

    # Tzolkin actions/powers
    "Birth": "\u05dc\u05d9\u05d3\u05d4",
    "Spirit": "\u05e8\u05d5\u05d7",
    "Abundance": "\u05e9\u05e4\u05e2",
    "Flowering": "\u05e4\u05e8\u05d9\u05d7\u05d4",
    "Survival": "\u05d4\u05d9\u05e9\u05e8\u05d3\u05d5\u05ea",
    "Equalize": "\u05d0\u05d9\u05d6\u05d5\u05df",
    "Accomplish": "\u05d4\u05d2\u05e9\u05de\u05d4",
    "Elegance": "\u05d0\u05dc\u05d2\u05e0\u05d8\u05d9\u05d5\u05ea",
    "Purification": "\u05d8\u05d9\u05d4\u05d5\u05e8",
    "Endlessness": "\u05d0\u05d9\u05df-\u05e1\u05d5\u05e4\u05d9\u05d5\u05ea",
    "Free Will": "\u05e8\u05e6\u05d5\u05df \u05d7\u05d5\u05e4\u05e9\u05d9",
    "Exploration": "\u05d7\u05e7\u05d9\u05e8\u05d4",
    "Timelessness": "\u05e0\u05e6\u05d7\u05d9\u05d5\u05ea",
    "Enchantment": "\u05e7\u05e1\u05dd",
    "Vision": "\u05d7\u05d6\u05d5\u05df",
    "Fearlessness": "\u05d7\u05d5\u05e1\u05e8 \u05e4\u05d7\u05d3",
    "Order": "\u05e1\u05d3\u05e8",
    "Self-Generation": "\u05d4\u05ea\u05d7\u05d3\u05e9\u05d5\u05ea \u05e2\u05e6\u05de\u05d9\u05ea",
    "Universal Fire": "\u05d0\u05e9 \u05d0\u05d5\u05e0\u05d9\u05d1\u05e8\u05e1\u05dc\u05d9\u05ea",
    "Life Force": "\u05db\u05d5\u05d7 \u05d4\u05d7\u05d9\u05d9\u05dd",

    # Tzolkin essences
    "Being": "\u05d4\u05d5\u05d5\u05d9\u05d4",
    "Breath": "\u05e0\u05e9\u05d9\u05de\u05d4",
    "Dreaming": "\u05d7\u05dc\u05d5\u05dd",
    "Awareness": "\u05de\u05d5\u05d3\u05e2\u05d5\u05ea",
    "Instinct": "\u05d9\u05e6\u05e8",
    "Death": "\u05de\u05d5\u05d5\u05ea",
    "Healing": "\u05e8\u05d9\u05e4\u05d5\u05d9",
    "Art": "\u05d0\u05de\u05e0\u05d5\u05ea",
    "Moon": "\u05d9\u05e8\u05d7",
    "Heart": "\u05dc\u05d1",
    "Magic": "\u05e7\u05e1\u05dd",
    "Intelligence": "\u05ea\u05d1\u05d5\u05e0\u05d4",
    "Prophecy": "\u05e0\u05d1\u05d5\u05d0\u05d4",
    "Receptivity": "\u05e7\u05dc\u05d9\u05d8\u05d4",
    "Reflection": "\u05d4\u05e9\u05ea\u05e7\u05e4\u05d5\u05ea",
    "Catalysis": "\u05d6\u05d9\u05e8\u05d5\u05d6",
    "Illumination": "\u05d4\u05d0\u05e8\u05d4",

    # Tzolkin actions
    "Nurtures": "\u05de\u05d8\u05e4\u05d7\u05ea",
    "Communicates": "\u05de\u05ea\u05e7\u05e9\u05e8\u05ea",
    "Dreams": "\u05d7\u05d5\u05dc\u05de\u05ea",
    "Targets": "\u05de\u05db\u05d5\u05d5\u05e0\u05ea",
    "Survives": "\u05e9\u05d5\u05e8\u05d3\u05ea",
    "Equalizes": "\u05de\u05d0\u05d6\u05e0\u05ea",
    "Knows": "\u05d9\u05d5\u05d3\u05e2\u05ea",
    "Beautifies": "\u05de\u05d9\u05d9\u05e4\u05d4",
    "Purifies": "\u05de\u05d8\u05d4\u05e8\u05ea",
    "Loves": "\u05d0\u05d5\u05d4\u05d1\u05ea",
    "Plays": "\u05de\u05e9\u05d7\u05e7\u05ea",
    "Influences": "\u05de\u05e9\u05e4\u05d9\u05e2\u05d4",
    "Explores": "\u05d7\u05d5\u05e7\u05e8\u05ea",
    "Enchants": "\u05de\u05e7\u05e1\u05d9\u05de\u05d4",
    "Creates": "\u05d9\u05d5\u05e6\u05e8\u05ea",
    "Questions": "\u05e9\u05d5\u05d0\u05dc\u05ea",
    "Reflects": "\u05de\u05e9\u05ea\u05e7\u05e4\u05ea",
    "Catalyzes": "\u05de\u05d6\u05e8\u05d6\u05ea",
    "Enlightens": "\u05de\u05d0\u05d9\u05e8\u05d4",
    "Endures": "\u05e1\u05d5\u05d1\u05dc\u05ea",

    # Chakras
    "Root": "\u05e9\u05d5\u05e8\u05e9",
    "Sacral": "\u05e1\u05e7\u05e8\u05dc",
    "Solar Plexus": "\u05de\u05e7\u05dc\u05e2\u05ea \u05d4\u05e9\u05de\u05e9",
    "Throat": "\u05d2\u05e8\u05d5\u05df",
    "Third Eye": "\u05e2\u05d9\u05df \u05e9\u05dc\u05d9\u05e9\u05d9\u05ea",

    # Egyptian deities
    "Ra": "\u05e8\u05e2",
    "Osiris": "\u05d0\u05d5\u05e1\u05d9\u05e8\u05d9\u05e1",
    "Isis": "\u05d0\u05d9\u05d6\u05d9\u05e1",
    "Horus": "\u05d4\u05d5\u05e8\u05d5\u05e1",
    "Thoth": "\u05ea\u05d5\u05ea",
    "Anubis": "\u05d0\u05e0\u05d5\u05d1\u05d9\u05e1",
    "Hathor": "\u05d4\u05d0\u05ea\u05d5\u05e8",
    "Maat": "\u05de\u05d0\u05ea",
    "Sekhmet": "\u05e1\u05d7\u05de\u05ea",
    "Ptah": "\u05e4\u05ea\u05d7",
    "Sobek": "\u05e1\u05d5\u05d1\u05e7",
    "Khonsu": "\u05d7\u05d5\u05e0\u05e1\u05d5",

    # More archetypes
    "The Alchemist": "\u05d4\u05d0\u05dc\u05db\u05d9\u05de\u05d0\u05d9",
    "The Elder": "\u05d4\u05d6\u05e7\u05df",
    "The Messenger": "\u05d4\u05e9\u05dc\u05d9\u05d7",
    "The Pioneer": "\u05d4\u05d7\u05dc\u05d5\u05e5",
    "The Rebel": "\u05d4\u05de\u05d5\u05e8\u05d3",
    "The Sage": "\u05d4\u05d7\u05db\u05dd",
    "The Seeker": "\u05d4\u05de\u05d7\u05e4\u05e9",
    "The Caretaker": "\u05d4\u05de\u05d8\u05e4\u05dc",

    # Celtic tree names
    "Birch": "\u05dc\u05d1\u05e0\u05d4",
    "Rowan": "\u05e8\u05d5\u05d5\u05df",
    "Ash": "\u05de\u05d9\u05dc\u05d4",
    "Alder": "\u05d0\u05dc\u05d3\u05e8",
    "Willow": "\u05e2\u05e8\u05d1\u05d4",
    "Hawthorn": "\u05e2\u05d5\u05d6\u05e8\u05d3",
    "Oak": "\u05d0\u05dc\u05d5\u05df",
    "Holly": "\u05d3\u05e8\u05d3\u05e8",
    "Hazel": "\u05d0\u05d2\u05d5\u05d6",
    "Vine": "\u05d2\u05e4\u05df",
    "Ivy": "\u05e7\u05d9\u05e1\u05d5\u05e1",
    "Reed": "\u05e7\u05e0\u05d4",
    "Elder": "\u05e1\u05de\u05d1\u05d5\u05e7",

    # More Hindu deities / terms
    "Surya": "\u05e1\u05d5\u05e8\u05d9\u05d4",
    "Kamadeva": "\u05e7\u05de\u05d3\u05d1\u05d4",
    "Yama": "\u05d9\u05de\u05d4",
    "Gauri": "\u05d2\u05d0\u05d5\u05e8\u05d9",
    "Nagas": "\u05e0\u05d0\u05d2\u05d5\u05ea",
    "Kartikeya": "\u05e7\u05e8\u05d8\u05d9\u05e7\u05d9\u05d4",

    # Hindu rashi names (Sanskrit zodiac in parentheses)
    "Mesha (Aries)": "\u05de\u05e9\u05d4 (\u05d8\u05dc\u05d4)",
    "Vrishabha (Taurus)": "\u05d5\u05e8\u05d9\u05e9\u05d0\u05d1\u05d4 (\u05e9\u05d5\u05e8)",
    "Mithuna (Gemini)": "\u05de\u05d9\u05ea\u05d5\u05e0\u05d4 (\u05ea\u05d0\u05d5\u05de\u05d9\u05dd)",
    "Karka (Cancer)": "\u05e7\u05e8\u05e7\u05d4 (\u05e1\u05e8\u05d8\u05df)",
    "Simha (Leo)": "\u05e1\u05d9\u05de\u05d4\u05d4 (\u05d0\u05e8\u05d9\u05d4)",
    "Kanya (Virgo)": "\u05e7\u05e0\u05d9\u05d4 (\u05d1\u05ea\u05d5\u05dc\u05d4)",
    "Tula (Libra)": "\u05d8\u05d5\u05dc\u05d4 (\u05de\u05d0\u05d6\u05e0\u05d9\u05d9\u05dd)",
    "Vrischika (Scorpio)": "\u05d5\u05e8\u05d9\u05e9\u05e6\u05f3\u05d9\u05e7\u05d4 (\u05e2\u05e7\u05e8\u05d1)",
    "Dhanus (Sagittarius)": "\u05d3\u05d4\u05e0\u05d5\u05e1 (\u05e7\u05e9\u05ea)",
    "Makara (Capricorn)": "\u05de\u05e7\u05e8\u05d4 (\u05d2\u05d3\u05d9)",
    "Kumbha (Aquarius)": "\u05e7\u05d5\u05de\u05d1\u05d4 (\u05d3\u05dc\u05d9)",
    "Meena (Pisces)": "\u05de\u05d9\u05e0\u05d4 (\u05d3\u05d2\u05d9\u05dd)",

    # More archetypes
    "The Enchantress": "\u05d4\u05e7\u05d5\u05e1\u05de\u05ea",
    "The Creator": "\u05d4\u05d9\u05d5\u05e6\u05e8",
    "The Seer": "\u05d4\u05e8\u05d5\u05d0\u05d4",
    "The Artist": "\u05d4\u05d0\u05de\u05df",
    "The Wanderer": "\u05d4\u05e0\u05d5\u05d3\u05d3",
    "The Guardian": "\u05d4\u05e9\u05d5\u05de\u05e8",
    "The Servant": "\u05d4\u05de\u05e9\u05e8\u05ea",

    # Seasons / weather
    "Harvest": "\u05e7\u05e6\u05d9\u05e8",
    "Germination": "\u05e0\u05d1\u05d9\u05d8\u05d4",
    "Late summer": "\u05e1\u05d5\u05e3 \u05e7\u05d9\u05e5",
    "Planting": "\u05e9\u05ea\u05d9\u05dc\u05d4",
    "Flowering": "\u05e4\u05e8\u05d9\u05d7\u05d4",
    "Rain": "\u05d2\u05e9\u05dd",
    "Flood": "\u05e9\u05d9\u05d8\u05e4\u05d5\u05df",
    "Flower": "\u05e4\u05e8\u05d7",
    "House": "\u05d1\u05d9\u05ea",

    # HD signals and types
    "Satisfaction": "\u05e1\u05d9\u05e4\u05d5\u05e7",
    "Frustration": "\u05ea\u05e1\u05db\u05d5\u05dc",
    "Bitterness": "\u05de\u05e8\u05d9\u05e8\u05d5\u05ea",
    "Disappointment": "\u05d0\u05db\u05d6\u05d1\u05d4",
    "Surprise": "\u05d4\u05e4\u05ea\u05e2\u05d4",
    "Manifesting Generator": "\u05de\u05e0\u05d9\u05e4\u05e1\u05d8\u05d5\u05e8 \u05d2\u05e0\u05e8\u05d8\u05d5\u05e8",
    "Emotional": "\u05e8\u05d2\u05e9\u05d9",
    "Sacral": "\u05e1\u05e7\u05e8\u05dc\u05d9",
    "Splenic": "\u05d8\u05d7\u05d5\u05dc\u05d9",
    "Self-Projected": "\u05d4\u05e7\u05e8\u05e0\u05ea \u05e2\u05e6\u05de\u05d9\u05ea",
    "Mental": "\u05de\u05e0\u05d8\u05dc\u05d9",
    "Ego": "\u05d0\u05d2\u05d5",

    # Zoroastrian Amesha Spentas
    "Ahura Mazda": "\u05d0\u05d4\u05d5\u05e8\u05d4 \u05de\u05d6\u05d3\u05d0",
    "Asha Vahishta": "\u05d0\u05e9\u05d4 \u05d5\u05d4\u05d9\u05e9\u05d8\u05d4",
    "Vohu Manah": "\u05d5\u05d5\u05d4\u05d5 \u05de\u05e0\u05d4",
    "Spenta Armaiti": "\u05e1\u05e4\u05e0\u05d8\u05d4 \u05d0\u05e8\u05de\u05d9\u05d8\u05d9",
    "Haurvatat": "\u05d4\u05d0\u05d5\u05e8\u05d5\u05d5\u05d8\u05d0\u05d8",
    "Ameretat": "\u05d0\u05de\u05e8\u05d8\u05d0\u05d8",

    # Bahai months (Arabic names stay, translate English meanings)
    "Splendour": "\u05d6\u05d5\u05d4\u05e8",
    "Glory": "\u05ea\u05e4\u05d0\u05e8\u05ea",
    "Mercy": "\u05e8\u05d7\u05de\u05d9\u05dd",
    "Words": "\u05de\u05d9\u05dc\u05d9\u05dd",
    "Perfection": "\u05e9\u05dc\u05de\u05d5\u05ea",
    "Names": "\u05e9\u05de\u05d5\u05ea",
    "Might": "\u05e2\u05d5\u05e6\u05de\u05d4",
    "Will": "\u05e8\u05e6\u05d5\u05df",
    "Questions": "\u05e9\u05d0\u05dc\u05d5\u05ea",
    "Honour": "\u05db\u05d1\u05d5\u05d3",
    "Sovereignty": "\u05e8\u05d9\u05d1\u05d5\u05e0\u05d5\u05ea",
    "Dominion": "\u05e9\u05dc\u05d8\u05d5\u05df",
    "Loftiness": "\u05e8\u05d5\u05de\u05de\u05d5\u05ea",

    # Numerology
    "Independence": "\u05e2\u05e6\u05de\u05d0\u05d5\u05ea",
    "Cooperation": "\u05e9\u05d9\u05ea\u05d5\u05e3 \u05e4\u05e2\u05d5\u05dc\u05d4",
    "Expression": "\u05d1\u05d9\u05d8\u05d5\u05d9",
    "Stability": "\u05d9\u05e6\u05d9\u05d1\u05d5\u05ea",
    "Adventure": "\u05d4\u05e8\u05e4\u05ea\u05e7\u05d4",
    "Responsibility": "\u05d0\u05d7\u05e8\u05d9\u05d5\u05ea",
    "Introspection": "\u05d4\u05ea\u05d1\u05d5\u05e0\u05e0\u05d5\u05ea",
    "Achievement": "\u05d4\u05d9\u05e9\u05d2",
    "Humanitarianism": "\u05d4\u05d5\u05de\u05e0\u05d9\u05d5\u05ea",

    # Moon phases
    "New Moon": "\u05de\u05d5\u05dc\u05d3 \u05d7\u05d3\u05e9",
    "Waxing Crescent": "\u05e1\u05d4\u05e8 \u05e2\u05d5\u05dc\u05d4",
    "First Quarter": "\u05e8\u05d1\u05e2 \u05e8\u05d0\u05e9\u05d5\u05df",
    "Waxing Gibbous": "\u05de\u05dc\u05d0 \u05e2\u05d5\u05dc\u05d4",
    "Full Moon": "\u05d9\u05e8\u05d7 \u05de\u05dc\u05d0",
    "Waning Gibbous": "\u05de\u05dc\u05d0 \u05d9\u05d5\u05e8\u05d3",
    "Last Quarter": "\u05e8\u05d1\u05e2 \u05d0\u05d7\u05e8\u05d5\u05df",
    "Waning Crescent": "\u05e1\u05d4\u05e8 \u05d9\u05d5\u05e8\u05d3",

    # More single-word terms
    "Illusion": "\u05d0\u05e9\u05dc\u05d9\u05d4",
    "Intuition": "\u05d0\u05d9\u05e0\u05d8\u05d5\u05d0\u05d9\u05e6\u05d9\u05d4",
    "Immortality": "\u05d0\u05dc\u05de\u05d5\u05d5\u05ea",
    "Nowruz": "\u05e0\u05d5\u05e8\u05d5\u05d6",
    "Yalda": "\u05d9\u05dc\u05d3\u05d0",
    "Initiation": "\u05d7\u05e0\u05d9\u05db\u05d4",
    "Ritual": "\u05d8\u05e7\u05e1",
    "Purification": "\u05d8\u05d9\u05d4\u05d5\u05e8",
    "Offering": "\u05e7\u05d5\u05e8\u05d1\u05df",
    "Contemplation": "\u05d4\u05ea\u05d1\u05d5\u05e0\u05e0\u05d5\u05ea",
    "Celebration": "\u05d7\u05d2\u05d9\u05d2\u05d4",
    "Liberation": "\u05e9\u05d7\u05e8\u05d5\u05e8",
    "Judgment": "\u05de\u05e9\u05e4\u05d8",
    "Endurance": "\u05e1\u05d9\u05d1\u05d5\u05dc\u05ea",
    "Prosperity": "\u05e9\u05d2\u05e9\u05d5\u05d2",
    "Fertility": "\u05e4\u05d5\u05e8\u05d9\u05d5\u05ea",
    "Divination": "\u05e0\u05d9\u05d7\u05d5\u05e9",
    "Revelation": "\u05d4\u05ea\u05d2\u05dc\u05d5\u05ea",
    "Transcendence": "\u05d4\u05ea\u05e2\u05dc\u05d5\u05ea",
    "Awakening": "\u05d4\u05ea\u05e2\u05d5\u05e8\u05e8\u05d5\u05ea",
    "Ancestor": "\u05d0\u05d1 \u05e7\u05d3\u05de\u05d5\u05df",
    "Ancestors": "\u05d0\u05d1\u05d5\u05ea",
    "Pilgrimage": "\u05e2\u05dc\u05d9\u05d9\u05d4 \u05dc\u05e8\u05d2\u05dc",
    "Fasting": "\u05e6\u05d5\u05dd",
    "Prayer": "\u05ea\u05e4\u05d9\u05dc\u05d4",
    "Almsgiving": "\u05e6\u05d3\u05e7\u05d4",
    "Charity": "\u05e6\u05d3\u05e7\u05d4",

    # Numerology numbers
    "One": "\u05d0\u05d7\u05d3",
    "Two": "\u05e9\u05e0\u05d9\u05d9\u05dd",
    "Three": "\u05e9\u05dc\u05d5\u05e9\u05d4",
    "Four": "\u05d0\u05e8\u05d1\u05e2\u05d4",
    "Five": "\u05d7\u05de\u05e9",
    "Six": "\u05e9\u05e9",
    "Seven": "\u05e9\u05d1\u05e2",
    "Eight": "\u05e9\u05de\u05d5\u05e0\u05d4",
    "Nine": "\u05ea\u05e9\u05e2",

    # Astronomy extended
    "Conjunction": "\u05e6\u05d9\u05de\u05d5\u05d3",
    "Opposition": "\u05e0\u05d9\u05d2\u05d5\u05d3",
    "Trine": "\u05d8\u05e8\u05d9\u05d2\u05d5\u05df",
    "Square": "\u05e8\u05d9\u05d1\u05d5\u05e2",
    "Sextile": "\u05e1\u05e7\u05e1\u05d8\u05d9\u05dc",
    "Retrograde": "\u05e8\u05d8\u05e8\u05d5\u05d2\u05e8\u05d3\u05d9",
    "Direct": "\u05d9\u05e9\u05d9\u05e8",
    "Eclipse": "\u05dc\u05d9\u05e7\u05d5\u05d9",
    "Equinox": "\u05e9\u05d5\u05d5\u05d9\u05d5\u05df",
    "Solstice": "\u05d4\u05d9\u05e4\u05d5\u05da \u05d9\u05d5\u05dd",

    # Japanese eras (Hebrew transliteration)
    "Meiji": "\u05de\u05d9\u05d9\u05d2\u05f3\u05d9",
    "Taisho": "\u05d8\u05d0\u05d9\u05e9\u05d5",
    "Showa": "\u05e9\u05d5\u05d5\u05d0",
    "Heisei": "\u05d4\u05d9\u05d9\u05e1\u05d9\u05d9",
    "Reiwa": "\u05e8\u05d9\u05d9\u05d5\u05d5\u05d0",

    # Japanese rokuyo
    "Sensho": "\u05e1\u05e0\u05e9\u05d5",
    "Tomobiki": "\u05d8\u05d5\u05de\u05d5\u05d1\u05d9\u05e7\u05d9",
    "Senbu": "\u05e1\u05e0\u05d1\u05d5",
    "Butsumetsu": "\u05d1\u05d5\u05d8\u05e1\u05d5\u05de\u05d8\u05e1\u05d5",
    "Taian": "\u05d8\u05d0\u05d9\u05d0\u05df",
    "Shakko": "\u05e9\u05d0\u05e7\u05d5",

    # Persian months (Hebrew transliteration)
    "Farvardin": "\u05e4\u05e8\u05d5\u05e8\u05d3\u05d9\u05df",
    "Ordibehesht": "\u05d0\u05d5\u05e8\u05d3\u05d9\u05d1\u05d4\u05e9\u05d8",
    "Khordad": "\u05d7\u05d5\u05e8\u05d3\u05d0\u05d3",
    "Tir": "\u05d8\u05d9\u05e8",
    "Mordad": "\u05de\u05d5\u05e8\u05d3\u05d0\u05d3",
    "Shahrivar": "\u05e9\u05d4\u05e8\u05d9\u05d5\u05e8",
    "Mehr": "\u05de\u05d4\u05e8",
    "Aban": "\u05d0\u05d1\u05d0\u05df",
    "Azar": "\u05d0\u05d6\u05e8",
    "Dey": "\u05d3\u05d9\u05d9",
    "Bahman": "\u05d1\u05d4\u05de\u05df",
    "Esfand": "\u05d0\u05e1\u05e4\u05e0\u05d3",

    # Coptic months (Hebrew transliteration)
    "Thout": "\u05ea\u05d5\u05ea",
    "Paopi": "\u05e4\u05d0\u05d5\u05e4\u05d9",
    "Hathor": "\u05d4\u05d0\u05ea\u05d5\u05e8",
    "Koiak": "\u05e7\u05d5\u05d9\u05d0\u05e7",
    "Tobi": "\u05d8\u05d5\u05d1\u05d9",
    "Meshir": "\u05de\u05e9\u05d9\u05e8",
    "Paremhat": "\u05e4\u05e8\u05de\u05d4\u05d8",
    "Parmouti": "\u05e4\u05e8\u05de\u05d5\u05d8\u05d9",
    "Pashons": "\u05e4\u05e9\u05d5\u05e0\u05e1",
    "Paoni": "\u05e4\u05d0\u05d5\u05e0\u05d9",
    "Epip": "\u05d0\u05e4\u05d9\u05e4",
    "Mesori": "\u05de\u05e1\u05d5\u05e8\u05d9",

    # Ethiopian months (Hebrew transliteration)
    "Meskerem": "\u05de\u05e1\u05e7\u05e8\u05dd",
    "Tikimt": "\u05d8\u05d9\u05e7\u05d9\u05de\u05d8",
    "Hidar": "\u05d4\u05d9\u05d3\u05e8",
    "Tahsas": "\u05d8\u05d7\u05e1\u05e1",
    "Tir": "\u05d8\u05d9\u05e8",
    "Yekatit": "\u05d9\u05e7\u05d8\u05d9\u05d8",
    "Megabit": "\u05de\u05d2\u05d1\u05d9\u05d8",
    "Miyazya": "\u05de\u05d9\u05d0\u05d6\u05d9\u05d4",
    "Ginbot": "\u05d2\u05d9\u05e0\u05d1\u05d5\u05d8",
    "Sene": "\u05e1\u05e0\u05d4",
    "Hamle": "\u05d4\u05de\u05dc\u05d4",
    "Nahase": "\u05e0\u05d7\u05e1\u05d4",

    # Buddhist terms
    "Formation": "\u05d4\u05d9\u05d5\u05d5\u05e6\u05e8\u05d5\u05ea",
    "Existence": "\u05e7\u05d9\u05d5\u05dd",
    "Destruction": "\u05d4\u05e8\u05e1",
    "Emptiness": "\u05e8\u05d9\u05e7\u05e0\u05d5\u05ea",
    "No Observance": "\u05dc\u05dc\u05d0 \u05e9\u05de\u05d9\u05e8\u05d4",
    "Dharma": "\u05d3\u05d4\u05e8\u05de\u05d4",
    "Sangha": "\u05e1\u05e0\u05d2\u05d4\u05d4",
    "Nirvana": "\u05e0\u05d9\u05e8\u05d5\u05d5\u05e0\u05d4",
    "Samsara": "\u05e1\u05de\u05e1\u05e8\u05d4",
    "Karma": "\u05e7\u05e8\u05de\u05d4",
    "Bodhi": "\u05d1\u05d5\u05d3\u05d4\u05d9",
    "Sutra": "\u05e1\u05d5\u05d8\u05e8\u05d4",

    # Geology eras + supercontinents
    "Cenozoic": "\u05e7\u05e0\u05d5\u05d6\u05d5\u05d0\u05d9\u05e7\u05d5\u05df",
    "Mesozoic": "\u05de\u05d6\u05d5\u05d6\u05d5\u05d0\u05d9\u05e7\u05d5\u05df",
    "Paleozoic": "\u05e4\u05dc\u05d0\u05d5\u05d6\u05d5\u05d0\u05d9\u05e7\u05d5\u05df",
    "Hadean": "\u05d4\u05d3\u05d0\u05d9",
    "Archean": "\u05d0\u05e8\u05db\u05d9\u05d0\u05d9",
    "Proterozoic": "\u05e4\u05e8\u05d5\u05d8\u05e8\u05d5\u05d6\u05d5\u05d0\u05d9",
    "Phanerozoic": "\u05e4\u05e0\u05e8\u05d5\u05d6\u05d5\u05d0\u05d9",
    "Vaalbara": "\u05d5\u05d0\u05dc\u05d1\u05e8\u05d4",
    "Kenorland": "\u05e7\u05e0\u05d5\u05e8\u05dc\u05e0\u05d3",
    "Columbia": "\u05e7\u05d5\u05dc\u05d5\u05de\u05d1\u05d9\u05d4",
    "Rodinia": "\u05e8\u05d5\u05d3\u05d9\u05e0\u05d9\u05d4",
    "Pannotia": "\u05e4\u05e0\u05d5\u05d8\u05d9\u05d4",
    "Pangaea": "\u05e4\u05e0\u05d2\u05d0\u05d4",
    "Pangaea Proxima": "\u05e4\u05e0\u05d2\u05d0\u05d4 \u05e4\u05e8\u05d5\u05e7\u05e1\u05d9\u05de\u05d4",

    # Common descriptive words
    "Courage": "\u05d0\u05d5\u05de\u05e5",
    "Patience": "\u05e1\u05d1\u05dc\u05e0\u05d5\u05ea",
    "Faith": "\u05d0\u05de\u05d5\u05e0\u05d4",
    "Hope": "\u05ea\u05e7\u05d5\u05d5\u05d4",
    "Gratitude": "\u05d4\u05db\u05e8\u05ea \u05ea\u05d5\u05d3\u05d4",
    "Humility": "\u05e2\u05e0\u05d5\u05d5\u05d4",
    "Forgiveness": "\u05e1\u05dc\u05d9\u05d7\u05d4",
    "Abundance": "\u05e9\u05e4\u05e2",
    "Clarity": "\u05d1\u05d4\u05d9\u05e8\u05d5\u05ea",
    "Devotion": "\u05de\u05e1\u05d9\u05e8\u05d5\u05ea",
    "Discipline": "\u05de\u05e9\u05de\u05e2\u05ea",
    "Freedom": "\u05d7\u05d9\u05e8\u05d5\u05ea",
    "Growth": "\u05e6\u05de\u05d9\u05d7\u05d4",
    "Knowledge": "\u05d3\u05e2\u05ea",
    "Mystery": "\u05de\u05d9\u05e1\u05ea\u05d5\u05e8\u05d9\u05df",
    "Protection": "\u05d4\u05d2\u05e0\u05d4",
    "Rebirth": "\u05dc\u05d9\u05d3\u05d4 \u05de\u05d7\u05d3\u05e9",
    "Sacrifice": "\u05e7\u05d5\u05e8\u05d1\u05df",
    "Surrender": "\u05db\u05e0\u05d9\u05e2\u05d4",
    "Transition": "\u05de\u05e2\u05d1\u05e8",

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

    # Hebrew month qualities/briefs/senses/tribes
    "hebrew.month.1.quality": "\u05d2\u05d0\u05d5\u05dc\u05d4",  # גאולה (Redemption)
    "hebrew.month.1.sense": "\u05d3\u05d9\u05d1\u05d5\u05e8",    # דיבור (Speech)
    "hebrew.month.1.tribe": "\u05d9\u05d4\u05d5\u05d3\u05d4",    # יהודה (Judah)
    "hebrew.month.1.brief": "\u05d7\u05d5\u05d3\u05e9 \u05d4\u05e0\u05d9\u05e1\u05d9\u05dd \u05d5\u05d4\u05e9\u05d7\u05e8\u05d5\u05e8 \u2014 \u05e4\u05e1\u05d7",  # חודש הניסים והשחרור — פסח
    "hebrew.month.2.quality": "\u05d4\u05e8\u05d4\u05d5\u05e8",  # הרהור (Reflection)
    "hebrew.month.2.sense": "\u05de\u05d7\u05e9\u05d1\u05d4",    # מחשבה (Thought)
    "hebrew.month.2.tribe": "\u05d9\u05e9\u05e9\u05db\u05e8",    # יששכר (Issachar)
    "hebrew.month.3.quality": "\u05e7\u05d1\u05dc\u05d4",        # קבלה (Receiving)
    "hebrew.month.3.sense": "\u05d4\u05dc\u05d9\u05db\u05d4",    # הליכה (Walking)
    "hebrew.month.3.tribe": "\u05d6\u05d1\u05d5\u05dc\u05d5\u05df",  # זבולון (Zebulun)
    "hebrew.month.4.quality": "\u05e8\u05d0\u05d9\u05d9\u05d4",  # ראייה (Sight)
    "hebrew.month.4.sense": "\u05e8\u05d0\u05d9\u05d9\u05d4",    # ראייה (Sight)
    "hebrew.month.4.tribe": "\u05e8\u05d0\u05d5\u05d1\u05df",    # ראובן (Reuben)
    "hebrew.month.5.quality": "\u05e9\u05de\u05d9\u05e2\u05d4",  # שמיעה (Hearing)
    "hebrew.month.5.sense": "\u05e9\u05de\u05d9\u05e2\u05d4",    # שמיעה (Hearing)
    "hebrew.month.5.tribe": "\u05e9\u05de\u05e2\u05d5\u05df",    # שמעון (Simeon)
    "hebrew.month.6.quality": "\u05ea\u05e9\u05d5\u05d1\u05d4",  # תשובה (Return)
    "hebrew.month.6.sense": "\u05e4\u05e2\u05d5\u05dc\u05d4",    # פעולה (Action)
    "hebrew.month.6.tribe": "\u05d2\u05d3",                      # גד (Gad)
    "hebrew.month.7.quality": "\u05de\u05d2\u05e2",              # מגע (Touch)
    "hebrew.month.7.sense": "\u05de\u05d2\u05e2",                # מגע (Touch)
    "hebrew.month.7.tribe": "\u05d0\u05e4\u05e8\u05d9\u05dd",    # אפרים (Ephraim)
    "hebrew.month.8.quality": "\u05e8\u05d9\u05d7",              # ריח (Smell)
    "hebrew.month.8.sense": "\u05e8\u05d9\u05d7",                # ריח (Smell)
    "hebrew.month.8.tribe": "\u05de\u05e0\u05e9\u05d4",          # מנשה (Manasseh)
    "hebrew.month.9.quality": "\u05e9\u05d9\u05e0\u05d4",        # שינה (Sleep)
    "hebrew.month.9.sense": "\u05e9\u05d9\u05e0\u05d4",          # שינה (Sleep)
    "hebrew.month.9.tribe": "\u05d1\u05e0\u05d9\u05de\u05d9\u05df",  # בנימין (Benjamin)
    "hebrew.month.10.quality": "\u05db\u05e2\u05e1",             # כעס (Anger)
    "hebrew.month.10.sense": "\u05db\u05e2\u05e1",               # כעס (Anger)
    "hebrew.month.10.tribe": "\u05d3\u05df",                     # דן (Dan)
    "hebrew.month.11.quality": "\u05d8\u05e2\u05dd",             # טעם (Taste)
    "hebrew.month.11.sense": "\u05d0\u05db\u05d9\u05dc\u05d4",   # אכילה (Eating)
    "hebrew.month.11.tribe": "\u05d0\u05e9\u05e8",               # אשר (Asher)
    "hebrew.month.12.quality": "\u05e9\u05de\u05d7\u05d4",       # שמחה (Joy)
    "hebrew.month.12.sense": "\u05e6\u05d7\u05d5\u05e7",         # צחוק (Laughter)
    "hebrew.month.12.tribe": "\u05e0\u05e4\u05ea\u05dc\u05d9",   # נפתלי (Naphtali)

    # Hebrew holiday briefs
    "hebrew.holiday.0.brief": "\u05e8\u05d0\u05e9 \u05d4\u05e9\u05e0\u05d4 \u2014 \u05d9\u05d5\u05dd \u05d4\u05d3\u05d9\u05df, \u05ea\u05e7\u05d9\u05e2\u05ea \u05d4\u05e9\u05d5\u05e4\u05e8",
    "hebrew.holiday.1.brief": "\u05d9\u05d5\u05dd \u05db\u05d9\u05e4\u05d5\u05e8 \u2014 \u05d9\u05d5\u05dd \u05d4\u05e6\u05d5\u05dd \u05d5\u05d4\u05ea\u05e9\u05d5\u05d1\u05d4",
    "hebrew.holiday.2.brief": "\u05e1\u05d5\u05db\u05d5\u05ea \u2014 \u05d7\u05d2 \u05d4\u05e7\u05e6\u05d9\u05e8, \u05e9\u05de\u05d7\u05d4 \u05d1\u05d8\u05d1\u05e2",
    "hebrew.holiday.3.brief": "\u05e4\u05e1\u05d7 \u2014 \u05d7\u05d2 \u05d4\u05d7\u05d9\u05e8\u05d5\u05ea, \u05d6\u05db\u05e8\u05d5\u05df \u05d9\u05e6\u05d9\u05d0\u05ea \u05de\u05e6\u05e8\u05d9\u05dd",
    "hebrew.holiday.4.brief": "\u05e9\u05d1\u05d5\u05e2\u05d5\u05ea \u2014 \u05de\u05ea\u05df \u05ea\u05d5\u05e8\u05d4, \u05d7\u05d2 \u05d4\u05e7\u05e6\u05d9\u05e8",
    "hebrew.holiday.5.brief": "\u05d7\u05e0\u05d5\u05db\u05d4 \u2014 \u05d7\u05d2 \u05d4\u05d0\u05d5\u05e8\u05d5\u05ea, \u05e0\u05e1 \u05d4\u05d7\u05e0\u05d5\u05db\u05d9\u05d4",
    "hebrew.holiday.6.brief": "\u05e4\u05d5\u05e8\u05d9\u05dd \u2014 \u05d7\u05d2 \u05d4\u05e9\u05de\u05d7\u05d4, \u05de\u05d2\u05d9\u05dc\u05ea \u05d0\u05e1\u05ea\u05e8",

    # Astrology sign briefs (Hebrew)
    "astrology.sign.0.brief": "\u05d4\u05e0\u05d9\u05e6\u05d5\u05e5 \u05e9\u05de\u05ea\u05d7\u05d9\u05dc \u2014 \u05d0\u05d5\u05de\u05e5 \u05d2\u05d5\u05dc\u05de\u05d9 \u05dc\u05d4\u05ea\u05e7\u05d9\u05d9\u05dd",
    "astrology.sign.1.brief": "\u05d4\u05d0\u05d3\u05de\u05d4 \u05e9\u05e1\u05d5\u05d1\u05dc\u05ea \u2014 \u05d8\u05d9\u05e4\u05d5\u05d7 \u05e1\u05d1\u05dc\u05e0\u05d9 \u05e9\u05dc \u05d9\u05d5\u05e4\u05d9",
    "astrology.sign.2.brief": "\u05d4\u05e8\u05d5\u05d7 \u05e9\u05de\u05d7\u05d1\u05e8\u05ea \u2014 \u05ea\u05e7\u05e9\u05d5\u05e8\u05ea \u05e1\u05e7\u05e8\u05e0\u05d5\u05ea \u05d0\u05d9\u05e0\u05e1\u05d5\u05e4\u05d9\u05ea",
    "astrology.sign.3.brief": "\u05d4\u05d9\u05dd \u05e9\u05de\u05d8\u05e4\u05d7 \u2014 \u05e2\u05d5\u05de\u05e7 \u05e8\u05d2\u05e9\u05d9 \u05e9\u05dc \u05d0\u05d9\u05e0\u05d8\u05d5\u05d0\u05d9\u05e6\u05d9\u05d4",
    "astrology.sign.4.brief": "\u05d4\u05d0\u05e9 \u05e9\u05d1\u05de\u05e8\u05db\u05d6 \u2014 \u05d4\u05d1\u05e2\u05ea \u05e2\u05e6\u05de\u05d9\u05ea \u05d9\u05e6\u05d9\u05e8\u05ea\u05d9\u05ea",
    "astrology.sign.5.brief": "\u05d4\u05d0\u05d3\u05de\u05d4 \u05e9\u05de\u05e0\u05ea\u05d7\u05ea \u2014 \u05e9\u05d9\u05e8\u05d5\u05ea \u05d3\u05e8\u05da \u05d3\u05d9\u05d5\u05e7",
    "astrology.sign.6.brief": "\u05d4\u05d0\u05d5\u05d5\u05d9\u05e8 \u05e9\u05de\u05d0\u05d6\u05df \u2014 \u05d4\u05e8\u05de\u05d5\u05e0\u05d9\u05d4 \u05d3\u05e8\u05da \u05d9\u05d7\u05e1\u05d9\u05dd",
    "astrology.sign.7.brief": "\u05d4\u05de\u05d9\u05dd \u05e9\u05de\u05e9\u05e0\u05d9\u05dd \u2014 \u05e2\u05d5\u05de\u05e7 \u05e8\u05d2\u05e9\u05d9 \u05e9\u05dc \u05d4\u05ea\u05d7\u05d3\u05e9\u05d5\u05ea",
    "astrology.sign.8.brief": "\u05d4\u05d0\u05e9 \u05e9\u05e9\u05d5\u05d0\u05e4\u05ea \u2014 \u05d4\u05e8\u05e4\u05ea\u05e7\u05d4 \u05d3\u05e8\u05da \u05d7\u05d5\u05e4\u05e9",
    "astrology.sign.9.brief": "\u05d4\u05d0\u05d3\u05de\u05d4 \u05e9\u05de\u05d8\u05e4\u05e1\u05ea \u2014 \u05de\u05e2\u05e9\u05d9\u05d5\u05ea \u05d3\u05e8\u05da \u05de\u05e9\u05de\u05e2\u05ea",
    "astrology.sign.10.brief": "\u05d4\u05d0\u05d5\u05d5\u05d9\u05e8 \u05e9\u05de\u05d7\u05d3\u05e9 \u2014 \u05d7\u05d6\u05d5\u05df \u05dc\u05e2\u05ea\u05d9\u05d3",
    "astrology.sign.11.brief": "\u05d4\u05de\u05d9\u05dd \u05e9\u05de\u05e8\u05d2\u05d9\u05e9\u05d9\u05dd \u2014 \u05d7\u05de\u05dc\u05d4 \u05d3\u05e8\u05da \u05d3\u05de\u05d9\u05d5\u05df",

    # Astrology sign keywords (Hebrew)
    "astrology.sign.0.keywords": "\u05d0\u05d5\u05de\u05e5, \u05d9\u05d5\u05d6\u05de\u05d4, \u05e2\u05e6\u05de\u05d0\u05d5\u05ea",
    "astrology.sign.1.keywords": "\u05d9\u05e6\u05d9\u05d1\u05d5\u05ea, \u05d7\u05d5\u05e9\u05e0\u05d9\u05d5\u05ea, \u05d4\u05ea\u05de\u05d3\u05d4",
    "astrology.sign.2.keywords": "\u05ea\u05e7\u05e9\u05d5\u05e8\u05ea, \u05d2\u05de\u05d9\u05e9\u05d5\u05ea, \u05e1\u05e7\u05e8\u05e0\u05d5\u05ea",
    "astrology.sign.3.keywords": "\u05d0\u05d9\u05e0\u05d8\u05d5\u05d0\u05d9\u05e6\u05d9\u05d4, \u05d4\u05d2\u05e0\u05d4, \u05e8\u05d2\u05e9\u05d9\u05d5\u05ea",
    "astrology.sign.4.keywords": "\u05d9\u05e6\u05d9\u05e8\u05ea\u05d9\u05d5\u05ea, \u05e0\u05d3\u05d9\u05d1\u05d5\u05ea, \u05de\u05e0\u05d4\u05d9\u05d2\u05d5\u05ea",
    "astrology.sign.5.keywords": "\u05e0\u05d9\u05ea\u05d5\u05d7, \u05e9\u05d9\u05e8\u05d5\u05ea, \u05d3\u05d9\u05d5\u05e7",
    "astrology.sign.6.keywords": "\u05d4\u05e8\u05de\u05d5\u05e0\u05d9\u05d4, \u05d3\u05d9\u05e4\u05dc\u05d5\u05de\u05d8\u05d9\u05d4, \u05e6\u05d3\u05e7",
    "astrology.sign.7.keywords": "\u05e2\u05d5\u05e6\u05de\u05d4, \u05d4\u05ea\u05d7\u05d3\u05e9\u05d5\u05ea, \u05ea\u05e9\u05d5\u05e7\u05d4",
    "astrology.sign.8.keywords": "\u05d4\u05e8\u05e4\u05ea\u05e7\u05d4, \u05d0\u05d5\u05e4\u05d8\u05d9\u05de\u05d9\u05d5\u05ea, \u05d7\u05d9\u05e4\u05d5\u05e9",
    "astrology.sign.9.keywords": "\u05de\u05e9\u05de\u05e2\u05ea, \u05e9\u05d0\u05d9\u05e4\u05d4, \u05d0\u05d7\u05e8\u05d9\u05d5\u05ea",
    "astrology.sign.10.keywords": "\u05d7\u05d3\u05e9\u05e0\u05d5\u05ea, \u05d0\u05e0\u05d5\u05e9\u05d9\u05d5\u05ea, \u05de\u05e8\u05d3\u05e0\u05d5\u05ea",
    "astrology.sign.11.keywords": "\u05d7\u05de\u05dc\u05d4, \u05d3\u05de\u05d9\u05d5\u05df, \u05d0\u05d9\u05e0\u05d8\u05d5\u05d0\u05d9\u05e6\u05d9\u05d4",

    # Astrology elements — expression field
    "astrology.element.0.name": "\u05d0\u05e9",
    "astrology.element.0.expression": "\u05d3\u05e8\u05da \u05e4\u05e2\u05d5\u05dc\u05d4, \u05d4\u05e9\u05e8\u05d0\u05d4 \u05d5\u05d4\u05ea\u05dc\u05d4\u05d1\u05d5\u05ea",
    "astrology.element.1.name": "\u05d0\u05d3\u05de\u05d4",
    "astrology.element.1.expression": "\u05d3\u05e8\u05da \u05de\u05e2\u05e9\u05d9\u05d5\u05ea, \u05d1\u05e0\u05d9\u05d9\u05d4 \u05d5\u05d7\u05d5\u05e9\u05e0\u05d9\u05d5\u05ea",
    "astrology.element.2.name": "\u05d0\u05d5\u05d5\u05d9\u05e8",
    "astrology.element.2.expression": "\u05d3\u05e8\u05da \u05de\u05d7\u05e9\u05d1\u05d4, \u05ea\u05e7\u05e9\u05d5\u05e8\u05ea \u05d5\u05e8\u05e2\u05d9\u05d5\u05e0\u05d5\u05ea",
    "astrology.element.3.name": "\u05de\u05d9\u05dd",
    "astrology.element.3.expression": "\u05d3\u05e8\u05da \u05e8\u05d2\u05e9, \u05d0\u05d9\u05e0\u05d8\u05d5\u05d0\u05d9\u05e6\u05d9\u05d4 \u05d5\u05d7\u05de\u05dc\u05d4",

    # Convergence strength names + descriptions (Hebrew)
    "convergence.strength.0.name": "\u05d9\u05d9\u05e9\u05d5\u05e8 \u05e7\u05dc",          # יישור קל (Minor Alignment)
    "convergence.strength.0.description": "\u05e9\u05ea\u05d9 \u05de\u05e2\u05e8\u05db\u05d5\u05ea \u05d7\u05d5\u05dc\u05e7\u05d5\u05ea \u05e8\u05d2\u05e2 \u05de\u05e9\u05de\u05e2\u05d5\u05ea\u05d9",
    "convergence.strength.1.name": "\u05d4\u05ea\u05db\u05e0\u05e1\u05d5\u05ea \u05de\u05e9\u05de\u05e2\u05d5\u05ea\u05d9\u05ea",
    "convergence.strength.1.description": "\u05e9\u05dc\u05d5\u05e9 \u05de\u05e2\u05e8\u05db\u05d5\u05ea \u05de\u05ea\u05d9\u05d9\u05e9\u05e8\u05d5\u05ea \u05d1\u05d5-\u05d6\u05de\u05e0\u05d9\u05ea",
    "convergence.strength.2.name": "\u05d4\u05ea\u05db\u05e0\u05e1\u05d5\u05ea \u05d2\u05d3\u05d5\u05dc\u05d4",
    "convergence.strength.2.description": "\u05d0\u05e8\u05d1\u05e2 \u05de\u05e2\u05e8\u05db\u05d5\u05ea \u05d0\u05d5 \u05d9\u05d5\u05ea\u05e8 \u05de\u05ea\u05db\u05e0\u05e1\u05d5\u05ea",
    "convergence.strength.3.name": "\u05d4\u05ea\u05db\u05e0\u05e1\u05d5\u05ea \u05e0\u05d3\u05d9\u05e8\u05d4",
    "convergence.strength.3.description": "\u05d7\u05de\u05e9 \u05de\u05e2\u05e8\u05db\u05d5\u05ea \u05d0\u05d5 \u05d9\u05d5\u05ea\u05e8 \u05d1\u05d9\u05d9\u05e9\u05d5\u05e8",

    # Convergence system names
    "convergence.system.0.system_name": "\u05d0\u05e1\u05d8\u05e8\u05d5\u05e0\u05d5\u05de\u05d9\u05d4",  # אסטרונומיה
    "convergence.system.1.system_name": "\u05d0\u05e1\u05d8\u05e8\u05d5\u05dc\u05d5\u05d2\u05d9\u05d4",  # אסטרולוגיה
    "convergence.system.2.system_name": "\u05e6\u05d5\u05dc\u05e7\u05d9\u05df",  # צולקין
    "convergence.system.3.system_name": "\u05d0\u05d9 \u05e6\u05f3\u05d9\u05e0\u05d2",  # אי צ'ינג
    "convergence.system.4.system_name": "\u05e2\u05d9\u05e6\u05d5\u05d1 \u05d0\u05e0\u05d5\u05e9\u05d9",  # עיצוב אנושי
    "convergence.system.5.system_name": "\u05e1\u05d9\u05e0\u05d9",  # סיני
    "convergence.system.6.system_name": "\u05d2\u05e8\u05d2\u05d5\u05e8\u05d9\u05d0\u05e0\u05d9",  # גרגוריאני
    "convergence.system.7.system_name": "\u05e2\u05d1\u05e8\u05d9",  # עברי
    "convergence.system.8.system_name": "\u05d0\u05e1\u05dc\u05d0\u05de\u05d9",  # אסלאמי
    "convergence.system.9.system_name": "\u05d1\u05d5\u05d3\u05d4\u05d9\u05e1\u05d8\u05d9",  # בודהיסטי
    "convergence.system.10.system_name": "\u05d4\u05d9\u05e0\u05d3\u05d5",  # הינדו
    "convergence.system.11.system_name": "\u05e7\u05d1\u05dc\u05d4",  # קבלה
    "convergence.system.12.system_name": "\u05d2\u05d9\u05d0\u05d5\u05dc\u05d5\u05d2\u05d9\u05d4",  # גיאולוגיה

    # Astrology sign shadows (Hebrew)
    "astrology.sign.0.shadow": "\u05d7\u05d5\u05e1\u05e8 \u05e1\u05d1\u05dc\u05e0\u05d5\u05ea, \u05ea\u05d5\u05e7\u05e4\u05e0\u05d5\u05ea, \u05d0\u05e0\u05d5\u05db\u05d9\u05d5\u05ea",
    "astrology.sign.1.shadow": "\u05e2\u05e7\u05e9\u05e0\u05d5\u05ea, \u05e8\u05db\u05d5\u05e9\u05e0\u05d5\u05ea, \u05d0\u05d9\u05e0\u05e8\u05e6\u05d9\u05d4",
    "astrology.sign.2.shadow": "\u05e9\u05d8\u05d7\u05d9\u05d5\u05ea, \u05d7\u05d5\u05e1\u05e8 \u05de\u05e0\u05d5\u05d7\u05d4, \u05d3\u05d5\u05d0\u05dc\u05d9\u05d5\u05ea",
    "astrology.sign.3.shadow": "\u05d4\u05d9\u05d0\u05d7\u05d6\u05d5\u05ea, \u05e9\u05d9\u05e0\u05d5\u05d9\u05d9 \u05de\u05e6\u05d1 \u05e8\u05d5\u05d7, \u05de\u05e0\u05d9\u05e4\u05d5\u05dc\u05e6\u05d9\u05d4",
    "astrology.sign.4.shadow": "\u05d2\u05d0\u05d5\u05d5\u05d4, \u05d3\u05e8\u05de\u05d4, \u05e9\u05dc\u05d8\u05e0\u05d5\u05ea",
    "astrology.sign.5.shadow": "\u05d1\u05d9\u05e7\u05d5\u05e8\u05ea\u05d9\u05d5\u05ea, \u05d7\u05e8\u05d3\u05d4, \u05e4\u05e8\u05e4\u05e7\u05e6\u05d9\u05d5\u05e0\u05d9\u05d6\u05dd",
    "astrology.sign.6.shadow": "\u05d4\u05d9\u05e1\u05d5\u05e1, \u05e8\u05e6\u05d5\u05df \u05dc\u05e8\u05e6\u05d5\u05ea, \u05d4\u05d9\u05de\u05e0\u05e2\u05d5\u05ea",
    "astrology.sign.7.shadow": "\u05d0\u05d5\u05d1\u05e1\u05e1\u05d9\u05d4, \u05e7\u05e0\u05d0\u05d4, \u05d4\u05e8\u05e1\u05e0\u05d5\u05ea",
    "astrology.sign.8.shadow": "\u05d4\u05e4\u05e8\u05d6\u05d4, \u05d3\u05d5\u05d2\u05de\u05d8\u05d9\u05d5\u05ea, \u05d7\u05d5\u05e1\u05e8 \u05de\u05e0\u05d5\u05d7\u05d4",
    "astrology.sign.9.shadow": "\u05e7\u05e8\u05d9\u05e8\u05d5\u05ea, \u05e0\u05d5\u05e7\u05e9\u05d5\u05ea, \u05d0\u05db\u05d6\u05e8\u05d9\u05d5\u05ea",
    "astrology.sign.10.shadow": "\u05e0\u05d9\u05ea\u05d5\u05e7, \u05de\u05e8\u05d3\u05e0\u05d5\u05ea, \u05d0\u05e7\u05e1\u05e6\u05e0\u05d8\u05e8\u05d9\u05d5\u05ea",
    "astrology.sign.11.shadow": "\u05d1\u05e8\u05d9\u05d7\u05d4, \u05d1\u05dc\u05d1\u05d5\u05dc, \u05e7\u05d5\u05e8\u05d1\u05e0\u05d5\u05ea \u05e2\u05e6\u05de\u05d9\u05ea",

    # Astrology modality details
    "astrology.modality.0.quality": "\u05d9\u05d6\u05de\u05d4, \u05de\u05e0\u05d4\u05d9\u05d2\u05d5\u05ea, \u05d4\u05ea\u05d7\u05dc\u05d5\u05ea \u05d7\u05d3\u05e9\u05d5\u05ea",
    "astrology.modality.0.role": "\u05de\u05ea\u05d7\u05d9\u05dc \u05d0\u05ea \u05d4\u05e2\u05d5\u05e0\u05d4 \u2014 \u05d4\u05e0\u05d9\u05e6\u05d5\u05e5 \u05e9\u05de\u05ea\u05d7\u05d9\u05dc",
    "astrology.modality.1.quality": "\u05d4\u05ea\u05de\u05d3\u05d4, \u05e0\u05d7\u05d9\u05e9\u05d5\u05ea, \u05e2\u05d5\u05de\u05e7",
    "astrology.modality.1.role": "\u05de\u05d7\u05d6\u05d9\u05e7 \u05d0\u05ea \u05d4\u05e2\u05d5\u05e0\u05d4 \u2014 \u05d4\u05db\u05d5\u05d7 \u05e9\u05e1\u05d5\u05d1\u05dc",
    "astrology.modality.2.quality": "\u05d4\u05e1\u05ea\u05d2\u05dc\u05d5\u05ea, \u05d2\u05de\u05d9\u05e9\u05d5\u05ea, \u05de\u05e2\u05d1\u05e8",
    "astrology.modality.2.role": "\u05de\u05e1\u05d9\u05d9\u05dd \u05d0\u05ea \u05d4\u05e2\u05d5\u05e0\u05d4 \u2014 \u05d4\u05d7\u05db\u05de\u05d4 \u05e9\u05de\u05e9\u05d7\u05e8\u05e8\u05ea",

    # Astrology element qualities
    "astrology.element.0.quality": "\u05e8\u05e6\u05d5\u05df, \u05e8\u05d5\u05d7, \u05db\u05d5\u05d7 \u05d9\u05e6\u05d9\u05e8\u05ea\u05d9",
    "astrology.element.1.quality": "\u05e6\u05d5\u05e8\u05d4, \u05d7\u05d5\u05de\u05e8, \u05d7\u05db\u05de\u05d4 \u05de\u05e2\u05e9\u05d9\u05ea",
    "astrology.element.2.quality": "\u05de\u05d7\u05e9\u05d1\u05d4, \u05ea\u05e7\u05e9\u05d5\u05e8\u05ea, \u05d9\u05d7\u05e1\u05d9\u05dd",
    "astrology.element.3.quality": "\u05e8\u05d2\u05e9, \u05d0\u05d9\u05e0\u05d8\u05d5\u05d0\u05d9\u05e6\u05d9\u05d4, \u05d4\u05ea\u05ea-\u05de\u05d5\u05d3\u05e2",

    # Gregorian month origins
    "gregorian.month.1.origin": "\u05d9\u05e0\u05d5\u05e1 \u2014 \u05d0\u05dc \u05d4\u05e9\u05e2\u05e8\u05d9\u05dd \u05d3\u05d5-\u05e4\u05e8\u05e6\u05d5\u05e4\u05d9, \u05d4\u05ea\u05d7\u05dc\u05d5\u05ea \u05d5\u05de\u05e2\u05d1\u05e8\u05d9\u05dd",
    "gregorian.month.2.origin": "\u05e4\u05d1\u05e8\u05d5\u05d0\u05e8\u05d9\u05d5\u05e1 \u2014 \u05d7\u05d2 \u05d4\u05d8\u05d9\u05d4\u05d5\u05e8 \u05d4\u05e8\u05d5\u05de\u05d9",
    "gregorian.month.3.origin": "\u05de\u05e8\u05e1 \u2014 \u05d0\u05dc \u05d4\u05de\u05dc\u05d7\u05de\u05d4",
    "gregorian.month.4.origin": "\u05d0\u05e4\u05e8\u05d9\u05dc\u05d9\u05e1 \u2014 \u05d0\u05dc\u05ea \u05d4\u05d0\u05d4\u05d1\u05d4 \u05d5\u05d4\u05d9\u05d5\u05e4\u05d9",
    "gregorian.month.5.origin": "\u05de\u05d0\u05d9\u05d4 \u2014 \u05d0\u05dc\u05ea \u05d4\u05e6\u05de\u05d9\u05d7\u05d4",
    "gregorian.month.6.origin": "\u05d9\u05d5\u05e0\u05d5 \u2014 \u05d0\u05dc\u05ea \u05d4\u05e0\u05d9\u05e9\u05d5\u05d0\u05d9\u05df",
    "gregorian.month.7.origin": "\u05d9\u05d5\u05dc\u05d9\u05d5\u05e1 \u05e7\u05d9\u05e1\u05e8 \u2014 \u05de\u05e0\u05d4\u05d9\u05d2 \u05e8\u05d5\u05de\u05d9",
    "gregorian.month.8.origin": "\u05d0\u05d5\u05d2\u05d5\u05e1\u05d8\u05d5\u05e1 \u2014 \u05e7\u05d9\u05e1\u05e8 \u05e8\u05d5\u05de\u05d9",
    "gregorian.month.9.origin": "\u05e1\u05e4\u05d8\u05dd \u2014 \u05d4\u05d7\u05d5\u05d3\u05e9 \u05d4\u05e9\u05d1\u05d9\u05e2\u05d9",
    "gregorian.month.10.origin": "\u05d0\u05d5\u05e7\u05d8\u05d5 \u2014 \u05d4\u05d7\u05d5\u05d3\u05e9 \u05d4\u05e9\u05de\u05d9\u05e0\u05d9",
    "gregorian.month.11.origin": "\u05e0\u05d5\u05d1\u05dd \u2014 \u05d4\u05d7\u05d5\u05d3\u05e9 \u05d4\u05ea\u05e9\u05d9\u05e2\u05d9",
    "gregorian.month.12.origin": "\u05d3\u05e6\u05dd \u2014 \u05d4\u05d7\u05d5\u05d3\u05e9 \u05d4\u05e2\u05e9\u05d9\u05e8\u05d9",

    # Gregorian day briefs
    "gregorian.day.0.brief": "\u05d9\u05d5\u05dd \u05d4\u05d9\u05e8\u05d7 \u2014 \u05d0\u05d9\u05e0\u05d8\u05d5\u05d0\u05d9\u05e6\u05d9\u05d4, \u05e8\u05d2\u05e9, \u05d4\u05d2\u05d0\u05d5\u05ea \u05e4\u05e0\u05d9\u05de\u05d9\u05d5\u05ea",
    "gregorian.day.1.brief": "\u05d9\u05d5\u05dd \u05de\u05d0\u05d3\u05d9\u05dd \u2014 \u05d0\u05d5\u05de\u05e5, \u05e4\u05e2\u05d5\u05dc\u05d4, \u05e0\u05d7\u05d9\u05e9\u05d5\u05ea \u05d4\u05dc\u05d5\u05d7\u05dd",
    "gregorian.day.2.brief": "\u05d9\u05d5\u05dd \u05db\u05d5\u05db\u05d1 \u05d7\u05de\u05d4 \u2014 \u05ea\u05e7\u05e9\u05d5\u05e8\u05ea, \u05de\u05dc\u05d0\u05db\u05d4, \u05ea\u05d7\u05d1\u05d5\u05dc\u05d4",
    "gregorian.day.3.brief": "\u05d9\u05d5\u05dd \u05e6\u05d3\u05e7 \u2014 \u05d4\u05ea\u05e4\u05e9\u05d8\u05d5\u05ea, \u05e9\u05e4\u05e2, \u05e0\u05d3\u05d9\u05d1\u05d5\u05ea",
    "gregorian.day.4.brief": "\u05d9\u05d5\u05dd \u05e0\u05d5\u05d2\u05d4 \u2014 \u05d0\u05d4\u05d1\u05d4, \u05d9\u05d5\u05e4\u05d9, \u05ea\u05e9\u05d5\u05e7\u05d4",
    "gregorian.day.5.brief": "\u05d9\u05d5\u05dd \u05e9\u05d1\u05ea\u05d0\u05d9 \u2014 \u05de\u05e9\u05de\u05e2\u05ea, \u05de\u05d1\u05e0\u05d4, \u05e9\u05d9\u05e2\u05d5\u05e8 \u05d4\u05de\u05e4\u05e7\u05d7",
    "gregorian.day.6.brief": "\u05d9\u05d5\u05dd \u05d4\u05e9\u05de\u05e9 \u2014 \u05d7\u05d9\u05d5\u05e0\u05d9\u05d5\u05ea, \u05e8\u05d9\u05d1\u05d5\u05e0\u05d5\u05ea, \u05de\u05e8\u05db\u05d6 \u05d4\u05db\u05dc",

    # Gregorian month qualities
    "gregorian.month.1.quality": "\u05d7\u05d5\u05e8\u05e3 \u05e2\u05de\u05d5\u05e7, \u05d4\u05e8\u05d4\u05d5\u05e8, \u05d4\u05ea\u05d7\u05dc\u05d5\u05ea \u05d7\u05d3\u05e9\u05d5\u05ea",
    "gregorian.month.2.quality": "\u05d7\u05d5\u05e8\u05e3 \u05de\u05d0\u05d5\u05d7\u05e8, \u05d8\u05d9\u05d4\u05d5\u05e8, \u05de\u05e2\u05d1\u05e8",
    "gregorian.month.3.quality": "\u05e9\u05d5\u05d5\u05d9\u05d5\u05df, \u05d4\u05ea\u05e2\u05d5\u05e8\u05e8\u05d5\u05ea, \u05e6\u05de\u05d9\u05d7\u05d4",
    "gregorian.month.4.quality": "\u05d0\u05d1\u05d9\u05d1, \u05e4\u05e8\u05d9\u05d7\u05d4, \u05d4\u05ea\u05d7\u05d3\u05e9\u05d5\u05ea",
    "gregorian.month.5.quality": "\u05e1\u05d5\u05e3 \u05d0\u05d1\u05d9\u05d1, \u05e9\u05e4\u05e2, \u05e6\u05de\u05d9\u05d7\u05d4",
    "gregorian.month.6.quality": "\u05ea\u05d7\u05d9\u05dc\u05ea \u05e7\u05d9\u05e5, \u05d0\u05d5\u05e8, \u05e4\u05e2\u05d9\u05dc\u05d5\u05ea",
    "gregorian.month.7.quality": "\u05dc\u05d1 \u05d4\u05e7\u05d9\u05e5, \u05d7\u05d5\u05dd, \u05d7\u05d9\u05d5\u05e0\u05d9\u05d5\u05ea",
    "gregorian.month.8.quality": "\u05e9\u05d9\u05d0 \u05d4\u05e7\u05d9\u05e5, \u05d4\u05d1\u05e9\u05dc\u05d4, \u05e9\u05e4\u05e2",
    "gregorian.month.9.quality": "\u05e9\u05d5\u05d5\u05d9\u05d5\u05df, \u05de\u05e2\u05d1\u05e8, \u05d0\u05d9\u05d6\u05d5\u05df",
    "gregorian.month.10.quality": "\u05e1\u05ea\u05d9\u05d5, \u05e9\u05d9\u05e0\u05d5\u05d9 \u05e6\u05d1\u05e2\u05d9\u05dd, \u05e2\u05d5\u05de\u05e7",
    "gregorian.month.11.quality": "\u05e1\u05d5\u05e3 \u05e1\u05ea\u05d9\u05d5, \u05d7\u05d5\u05e9\u05da \u05d5\u05d6\u05d9\u05db\u05e8\u05d5\u05df",
    "gregorian.month.12.quality": "\u05d7\u05d5\u05e8\u05e3, \u05d7\u05d5\u05e9\u05da \u05d5\u05d0\u05d5\u05e8, \u05e1\u05d9\u05d5\u05dd",

    # Gregorian month briefs
    "gregorian.month.1.brief": "\u05d7\u05d5\u05d3\u05e9 \u05d4\u05e9\u05e2\u05e8\u05d9\u05dd \u2014 \u05e4\u05e0\u05d9\u05dd \u05d0\u05d7\u05ea \u05de\u05d1\u05d9\u05d8\u05d5\u05ea \u05d0\u05d7\u05d5\u05e8\u05d4, \u05d0\u05d7\u05ea \u05e7\u05d3\u05d9\u05de\u05d4",
    "gregorian.month.2.brief": "\u05d4\u05d7\u05d5\u05d3\u05e9 \u05d4\u05e7\u05e6\u05e8 \u05d1\u05d9\u05d5\u05ea\u05e8 \u2014 \u05d8\u05d9\u05d4\u05d5\u05e8 \u05d5\u05d4\u05db\u05e0\u05d4",
    "gregorian.month.3.brief": "\u05e9\u05d5\u05d5\u05d9\u05d5\u05df \u05d4\u05d0\u05d1\u05d9\u05d1 \u2014 \u05d4\u05d0\u05d5\u05e8 \u05e9\u05d1 \u05d5\u05d4\u05d7\u05d5\u05e9\u05da \u05e9\u05d5\u05d5\u05d9\u05dd",
    "gregorian.month.4.brief": "\u05d4\u05ea\u05e2\u05d5\u05e8\u05e8\u05d5\u05ea \u05d4\u05d0\u05d1\u05d9\u05d1 \u2014 \u05e6\u05de\u05d9\u05d7\u05d4 \u05d5\u05d4\u05ea\u05d7\u05d3\u05e9\u05d5\u05ea",
    "gregorian.month.5.brief": "\u05dc\u05d1 \u05d4\u05d0\u05d1\u05d9\u05d1 \u2014 \u05e4\u05e8\u05d9\u05d7\u05d4 \u05d5\u05e9\u05e4\u05e2",
    "gregorian.month.6.brief": "\u05d4\u05d9\u05e4\u05d5\u05da \u05d9\u05d5\u05dd \u05d4\u05e7\u05d9\u05e5 \u2014 \u05d0\u05d5\u05e8 \u05d1\u05e9\u05d9\u05d0",
    "gregorian.month.7.brief": "\u05dc\u05d1 \u05d4\u05e7\u05d9\u05e5 \u2014 \u05d7\u05d5\u05dd \u05d5\u05d7\u05d9\u05d5\u05e0\u05d9\u05d5\u05ea",
    "gregorian.month.8.brief": "\u05e9\u05d9\u05d0 \u05d4\u05e7\u05d9\u05e5 \u2014 \u05d1\u05e9\u05dc\u05d5\u05ea \u05d5\u05e9\u05e4\u05e2",
    "gregorian.month.9.brief": "\u05e9\u05d5\u05d5\u05d9\u05d5\u05df \u05d4\u05e1\u05ea\u05d9\u05d5 \u2014 \u05d4\u05d0\u05d5\u05e8 \u05d5\u05d4\u05d7\u05d5\u05e9\u05da \u05de\u05ea\u05d0\u05d6\u05e0\u05d9\u05dd",
    "gregorian.month.10.brief": "\u05dc\u05d1 \u05d4\u05e1\u05ea\u05d9\u05d5 \u2014 \u05d4\u05e2\u05dc\u05d9\u05dd \u05e0\u05d5\u05e9\u05e8\u05d9\u05dd",
    "gregorian.month.11.brief": "\u05d4\u05d7\u05d5\u05e9\u05da \u05d9\u05d5\u05e8\u05d3 \u2014 \u05e9\u05e7\u05d8 \u05d5\u05d6\u05d9\u05db\u05e8\u05d5\u05df",
    "gregorian.month.12.brief": "\u05d4\u05d9\u05e4\u05d5\u05da \u05d9\u05d5\u05dd \u05d4\u05d7\u05d5\u05e8\u05e3 \u2014 \u05d7\u05d5\u05e9\u05da \u05d5\u05d0\u05d5\u05e8",

    # A11y accessibility narrative terms
    "a11y.sky.pos.0": "\u05e2\u05d5\u05dc\u05d4 \u05d1\u05de\u05d6\u05e8\u05d7",
    "a11y.sky.pos.1": "\u05e0\u05de\u05d5\u05da \u05d1\u05de\u05d6\u05e8\u05d7",
    "a11y.sky.pos.2": "\u05d2\u05d1\u05d5\u05d4 \u05d1\u05e9\u05de\u05d9\u05dd",
    "a11y.sky.pos.3": "\u05db\u05de\u05e2\u05d8 \u05de\u05e2\u05dc \u05d4\u05e8\u05d0\u05e9",
    "a11y.sky.pos.4": "\u05e0\u05de\u05d5\u05da \u05d1\u05de\u05e2\u05e8\u05d1",
    "a11y.sky.pos.5": "\u05e9\u05d5\u05e7\u05e2 \u05d1\u05de\u05e2\u05e8\u05d1",
    "a11y.sky.pos.6": "\u05de\u05ea\u05d7\u05ea \u05dc\u05d0\u05d5\u05e4\u05e7",
    "a11y.sky.bright.0": "\u05d6\u05d5\u05e8\u05d7 \u05d1\u05e2\u05d5\u05e6\u05de\u05d4",
    "a11y.sky.bright.1": "\u05e0\u05e8\u05d0\u05d4 \u05d1\u05d1\u05d9\u05e8\u05d5\u05e8",
    "a11y.sky.bright.2": "\u05e0\u05e8\u05d0\u05d4",
    "a11y.sky.bright.3": "\u05e0\u05e8\u05d0\u05d4 \u05d1\u05e7\u05d5\u05e9\u05d9",
    "a11y.sky.bright.4": "\u05dc\u05d0 \u05e0\u05e8\u05d0\u05d4 \u05d1\u05e2\u05d9\u05df \u05d1\u05dc\u05ea\u05d9 \u05de\u05d6\u05d5\u05d9\u05e0\u05ea",
    "a11y.trans.0.name": "\u05d4\u05ea\u05e7\u05e8\u05d1\u05d5\u05ea",
    "a11y.trans.1.name": "\u05d4\u05ea\u05e8\u05d7\u05e7\u05d5\u05ea",
    "a11y.trans.2.name": "\u05ea\u05e6\u05d5\u05d2\u05ea \u05d7\u05dc\u05dc",
    "a11y.trans.3.name": "\u05ea\u05e6\u05d5\u05d2\u05ea \u05db\u05d3\u05d5\u05e8 \u05d4\u05d0\u05e8\u05e5",
    "a11y.trans.4.name": "\u05db\u05e0\u05d9\u05e1\u05d4 \u05dc\u05de\u05e2\u05e8\u05db\u05ea",
    "a11y.trans.5.name": "\u05d9\u05e6\u05d9\u05d0\u05d4 \u05de\u05de\u05e2\u05e8\u05db\u05ea",
    "a11y.trans.6.name": "\u05e7\u05d3\u05d9\u05de\u05d4 \u05d1\u05d6\u05de\u05df",
    "a11y.trans.7.name": "\u05d0\u05d7\u05d5\u05e8\u05d4 \u05d1\u05d6\u05de\u05df",

    # Celtic festivals
    "celtic.festival.0.name": "\u05e1\u05d0\u05de\u05d4\u05d9\u05df",
    "celtic.festival.1.name": "\u05d9\u05d5\u05dc",
    "celtic.festival.2.name": "\u05d0\u05d9\u05de\u05d1\u05d5\u05dc\u05e7",
    "celtic.festival.3.name": "\u05d0\u05d5\u05e1\u05d8\u05e8\u05d4",
    "celtic.festival.4.name": "\u05d1\u05dc\u05d8\u05d9\u05d9\u05df",
    "celtic.festival.5.name": "\u05dc\u05d9\u05ea\u05d4",
    "celtic.festival.6.name": "\u05dc\u05d5\u05d2\u05e0\u05e1\u05d3",
    "celtic.festival.7.name": "\u05de\u05d0\u05d1\u05d5\u05df",

    # Buddhist uposatha
    "buddhist.uposatha.1.name": "\u05e9\u05de\u05d9\u05e8\u05ea \u05d9\u05e8\u05d7 \u05d7\u05d3\u05e9",
    "buddhist.uposatha.2.name": "\u05e9\u05de\u05d9\u05e8\u05ea \u05d9\u05e8\u05d7 \u05de\u05dc\u05d0",

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
