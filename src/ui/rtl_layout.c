#include "rtl_layout.h"

#include <stddef.h>

/* Decode a single UTF-8 codepoint from a byte string.
 * Sets *bytes_read to number of bytes consumed. */
static uint32_t decode_utf8(const char *s, int *bytes_read)
{
    unsigned char c = (unsigned char)s[0];
    if (c < 0x80) {
        *bytes_read = 1;
        return c;
    }
    if ((c & 0xE0) == 0xC0) {
        *bytes_read = 2;
        return ((uint32_t)(c & 0x1F) << 6) |
               (uint32_t)((unsigned char)s[1] & 0x3F);
    }
    if ((c & 0xF0) == 0xE0) {
        *bytes_read = 3;
        return ((uint32_t)(c & 0x0F) << 12) |
               ((uint32_t)((unsigned char)s[1] & 0x3F) << 6) |
               (uint32_t)((unsigned char)s[2] & 0x3F);
    }
    if ((c & 0xF8) == 0xF0) {
        *bytes_read = 4;
        return ((uint32_t)(c & 0x07) << 18) |
               ((uint32_t)((unsigned char)s[1] & 0x3F) << 12) |
               ((uint32_t)((unsigned char)s[2] & 0x3F) << 6) |
               (uint32_t)((unsigned char)s[3] & 0x3F);
    }
    *bytes_read = 1;
    return 0xFFFD; /* replacement character */
}

/* Check if codepoint is whitespace or neutral (digits, punctuation, spaces). */
static bool is_neutral(uint32_t cp)
{
    /* ASCII whitespace */
    if (cp == ' ' || cp == '\t' || cp == '\n' || cp == '\r') return true;
    /* ASCII digits */
    if (cp >= '0' && cp <= '9') return true;
    /* ASCII punctuation / symbols (not letters) */
    if (cp >= 0x21 && cp <= 0x2F) return true;  /* ! " # $ % & ' ( ) * + , - . / */
    if (cp >= 0x3A && cp <= 0x40) return true;  /* : ; < = > ? @ */
    if (cp >= 0x5B && cp <= 0x60) return true;  /* [ \ ] ^ _ ` */
    if (cp >= 0x7B && cp <= 0x7E) return true;  /* { | } ~ */
    return false;
}

bool rtl_is_rtl_codepoint(uint32_t codepoint)
{
    /* Arabic: U+0600-U+06FF */
    if (codepoint >= 0x0600 && codepoint <= 0x06FF) return true;
    /* Hebrew: U+0590-U+05FF */
    if (codepoint >= 0x0590 && codepoint <= 0x05FF) return true;
    /* Syriac: U+0700-U+074F */
    if (codepoint >= 0x0700 && codepoint <= 0x074F) return true;
    /* Arabic Supplement: U+0750-U+077F */
    if (codepoint >= 0x0750 && codepoint <= 0x077F) return true;
    /* Thaana: U+0780-U+07BF */
    if (codepoint >= 0x0780 && codepoint <= 0x07BF) return true;
    /* Arabic Extended-A: U+08A0-U+08FF */
    if (codepoint >= 0x08A0 && codepoint <= 0x08FF) return true;
    /* RTL marks */
    if (codepoint == 0x200F) return true; /* RLM */
    if (codepoint == 0x202B) return true; /* RLE */
    if (codepoint == 0x202E) return true; /* RLO */
    /* Hebrew Presentation Forms: U+FB1D-U+FB4F */
    if (codepoint >= 0xFB1D && codepoint <= 0xFB4F) return true;
    /* Arabic Presentation Forms-A: U+FB50-U+FDFF */
    if (codepoint >= 0xFB50 && codepoint <= 0xFDFF) return true;
    /* Arabic Presentation Forms-B: U+FE70-U+FEFF */
    if (codepoint >= 0xFE70 && codepoint <= 0xFEFF) return true;

    return false;
}

rtl_direction_t rtl_codepoint_direction(uint32_t codepoint)
{
    if (is_neutral(codepoint)) return RTL_DIR_NEUTRAL;
    if (rtl_is_rtl_codepoint(codepoint)) return RTL_DIR_RTL;
    /* Any non-neutral, non-RTL codepoint with a value > 0 is LTR */
    if (codepoint > 0) return RTL_DIR_LTR;
    return RTL_DIR_NEUTRAL;
}

rtl_script_t rtl_identify_script(uint32_t codepoint)
{
    /* Latin: A-Z, a-z, Latin Extended */
    if ((codepoint >= 'A' && codepoint <= 'Z') ||
        (codepoint >= 'a' && codepoint <= 'z') ||
        (codepoint >= 0x00C0 && codepoint <= 0x024F)) {
        return RTL_SCRIPT_LATIN;
    }

    /* Hebrew: U+0590-U+05FF */
    if (codepoint >= 0x0590 && codepoint <= 0x05FF) return RTL_SCRIPT_HEBREW;
    /* Hebrew Presentation Forms: U+FB1D-U+FB4F */
    if (codepoint >= 0xFB1D && codepoint <= 0xFB4F) return RTL_SCRIPT_HEBREW;

    /* Arabic: U+0600-U+06FF */
    if (codepoint >= 0x0600 && codepoint <= 0x06FF) return RTL_SCRIPT_ARABIC;
    /* Arabic Supplement: U+0750-U+077F */
    if (codepoint >= 0x0750 && codepoint <= 0x077F) return RTL_SCRIPT_ARABIC;
    /* Arabic Extended-A: U+08A0-U+08FF */
    if (codepoint >= 0x08A0 && codepoint <= 0x08FF) return RTL_SCRIPT_ARABIC;
    /* Arabic Presentation Forms-A: U+FB50-U+FDFF */
    if (codepoint >= 0xFB50 && codepoint <= 0xFDFF) return RTL_SCRIPT_ARABIC;
    /* Arabic Presentation Forms-B: U+FE70-U+FEFF */
    if (codepoint >= 0xFE70 && codepoint <= 0xFEFF) return RTL_SCRIPT_ARABIC;

    /* Syriac: U+0700-U+074F */
    if (codepoint >= 0x0700 && codepoint <= 0x074F) return RTL_SCRIPT_SYRIAC;

    /* Thaana: U+0780-U+07BF */
    if (codepoint >= 0x0780 && codepoint <= 0x07BF) return RTL_SCRIPT_THAANA;

    /* Devanagari: U+0900-U+097F */
    if (codepoint >= 0x0900 && codepoint <= 0x097F) return RTL_SCRIPT_DEVANAGARI;

    /* Thai: U+0E00-U+0E7F */
    if (codepoint >= 0x0E00 && codepoint <= 0x0E7F) return RTL_SCRIPT_THAI;

    /* Ge'ez (Ethiopic): U+1200-U+137F */
    if (codepoint >= 0x1200 && codepoint <= 0x137F) return RTL_SCRIPT_GEEZ;

    /* CJK Unified Ideographs: U+4E00-U+9FFF */
    if (codepoint >= 0x4E00 && codepoint <= 0x9FFF) return RTL_SCRIPT_CJK;
    /* CJK Extension A: U+3400-U+4DBF */
    if (codepoint >= 0x3400 && codepoint <= 0x4DBF) return RTL_SCRIPT_CJK;
    /* Hiragana: U+3040-U+309F, Katakana: U+30A0-U+30FF */
    if (codepoint >= 0x3040 && codepoint <= 0x30FF) return RTL_SCRIPT_CJK;
    /* Hangul: U+AC00-U+D7AF */
    if (codepoint >= 0xAC00 && codepoint <= 0xD7AF) return RTL_SCRIPT_CJK;

    return RTL_SCRIPT_UNKNOWN;
}

bool rtl_is_rtl_script(rtl_script_t script)
{
    switch (script) {
    case RTL_SCRIPT_ARABIC:
    case RTL_SCRIPT_HEBREW:
    case RTL_SCRIPT_PERSIAN:
    case RTL_SCRIPT_URDU:
    case RTL_SCRIPT_SYRIAC:
    case RTL_SCRIPT_THAANA:
        return true;
    default:
        return false;
    }
}

bool rtl_should_mirror(rtl_element_t element)
{
    switch (element) {
    case RTL_ELEM_TEXT:
    case RTL_ELEM_MENU:
    case RTL_ELEM_CARD:
    case RTL_ELEM_HUD:
        return true;
    case RTL_ELEM_SKY:
    case RTL_ELEM_ZODIAC:
    case RTL_ELEM_CHART:
    case RTL_ELEM_CLOCK:
    default:
        return false;
    }
}

const char *rtl_script_name(rtl_script_t script)
{
    switch (script) {
    case RTL_SCRIPT_LATIN:      return "Latin";
    case RTL_SCRIPT_ARABIC:     return "Arabic";
    case RTL_SCRIPT_HEBREW:     return "Hebrew";
    case RTL_SCRIPT_PERSIAN:    return "Persian";
    case RTL_SCRIPT_URDU:       return "Urdu";
    case RTL_SCRIPT_SYRIAC:     return "Syriac";
    case RTL_SCRIPT_THAANA:     return "Thaana";
    case RTL_SCRIPT_DEVANAGARI: return "Devanagari";
    case RTL_SCRIPT_CJK:        return "CJK";
    case RTL_SCRIPT_THAI:       return "Thai";
    case RTL_SCRIPT_GEEZ:       return "Ge'ez";
    case RTL_SCRIPT_UNKNOWN:    return "Unknown";
    default:                    return "Unknown";
    }
}

const char *rtl_direction_name(rtl_direction_t direction)
{
    switch (direction) {
    case RTL_DIR_LTR:     return "LTR";
    case RTL_DIR_RTL:     return "RTL";
    case RTL_DIR_NEUTRAL: return "Neutral";
    default:              return "Unknown";
    }
}

const char *rtl_element_name(rtl_element_t element)
{
    switch (element) {
    case RTL_ELEM_TEXT:   return "Text";
    case RTL_ELEM_MENU:   return "Menu";
    case RTL_ELEM_CARD:   return "Card";
    case RTL_ELEM_HUD:    return "HUD";
    case RTL_ELEM_SKY:    return "Sky";
    case RTL_ELEM_ZODIAC: return "Zodiac";
    case RTL_ELEM_CHART:  return "Chart";
    case RTL_ELEM_CLOCK:  return "Clock";
    default:              return "Unknown";
    }
}

double rtl_mirror_x(double x, double element_width, double container_width)
{
    return container_width - x - element_width;
}

bool rtl_starts_rtl(const char *utf8_str)
{
    if (utf8_str == NULL || utf8_str[0] == '\0') return false;

    const char *p = utf8_str;
    while (*p != '\0') {
        int bytes = 0;
        uint32_t cp = decode_utf8(p, &bytes);
        if (bytes == 0) break;

        /* Skip neutral characters to find first strong directional char */
        if (!is_neutral(cp)) {
            return rtl_is_rtl_codepoint(cp);
        }
        p += bytes;
    }
    return false;
}

rtl_direction_t rtl_dominant_direction(const char *utf8_str)
{
    if (utf8_str == NULL || utf8_str[0] == '\0') return RTL_DIR_NEUTRAL;

    int rtl_count = 0;
    int ltr_count = 0;
    const char *p = utf8_str;

    while (*p != '\0') {
        int bytes = 0;
        uint32_t cp = decode_utf8(p, &bytes);
        if (bytes == 0) break;

        rtl_direction_t dir = rtl_codepoint_direction(cp);
        if (dir == RTL_DIR_RTL) {
            rtl_count++;
        } else if (dir == RTL_DIR_LTR) {
            ltr_count++;
        }
        p += bytes;
    }

    if (rtl_count == 0 && ltr_count == 0) return RTL_DIR_NEUTRAL;
    if (rtl_count > ltr_count) return RTL_DIR_RTL;
    return RTL_DIR_LTR;
}

int rtl_script_count(void)
{
    return RTL_SCRIPT_COUNT;
}

int rtl_element_count(void)
{
    return RTL_ELEM_COUNT;
}
