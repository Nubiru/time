#ifndef TIME_RTL_LAYOUT_H
#define TIME_RTL_LAYOUT_H

/* RTL Layout Support — right-to-left script detection and layout mirroring.
 * Pure module: no allocation, no globals, no side effects.
 * Identifies RTL Unicode ranges (Arabic, Hebrew, Syriac, Thaana),
 * determines which UI elements should mirror in RTL mode,
 * and provides bidirectional text direction detection. */

#include <stdbool.h>
#include <stdint.h>

/* Script direction */
typedef enum {
    RTL_DIR_LTR = 0,      /* Left-to-right */
    RTL_DIR_RTL,           /* Right-to-left */
    RTL_DIR_NEUTRAL,       /* Numbers, punctuation */
    RTL_DIR_COUNT
} rtl_direction_t;

/* UI element types for mirroring decisions */
typedef enum {
    RTL_ELEM_TEXT = 0,       /* Regular text — mirrors */
    RTL_ELEM_MENU,           /* Menu/navigation — mirrors */
    RTL_ELEM_CARD,           /* Info card — mirrors layout */
    RTL_ELEM_HUD,            /* HUD overlay — mirrors position */
    RTL_ELEM_SKY,            /* Sky/celestial view — NEVER mirrors */
    RTL_ELEM_ZODIAC,         /* Zodiac wheel — NEVER mirrors (astronomical) */
    RTL_ELEM_CHART,          /* Charts/graphs — NEVER mirrors */
    RTL_ELEM_CLOCK,          /* Clock face — NEVER mirrors */
    RTL_ELEM_COUNT
} rtl_element_t;

/* Script identifier */
typedef enum {
    RTL_SCRIPT_LATIN = 0,
    RTL_SCRIPT_ARABIC,
    RTL_SCRIPT_HEBREW,
    RTL_SCRIPT_PERSIAN,      /* Uses Arabic script with extensions */
    RTL_SCRIPT_URDU,         /* Uses Arabic script (Nastaliq) */
    RTL_SCRIPT_SYRIAC,
    RTL_SCRIPT_THAANA,       /* Maldivian */
    RTL_SCRIPT_DEVANAGARI,   /* Hindi — LTR */
    RTL_SCRIPT_CJK,          /* Chinese/Japanese/Korean — LTR (vert possible) */
    RTL_SCRIPT_THAI,         /* Thai — LTR */
    RTL_SCRIPT_GEEZ,         /* Ethiopic — LTR */
    RTL_SCRIPT_UNKNOWN,
    RTL_SCRIPT_COUNT
} rtl_script_t;

/* Check if a Unicode codepoint is in an RTL range. */
bool rtl_is_rtl_codepoint(uint32_t codepoint);

/* Get direction for a Unicode codepoint. */
rtl_direction_t rtl_codepoint_direction(uint32_t codepoint);

/* Identify script from a Unicode codepoint. */
rtl_script_t rtl_identify_script(uint32_t codepoint);

/* Check if a script is RTL. */
bool rtl_is_rtl_script(rtl_script_t script);

/* Should this UI element be mirrored in RTL mode? */
bool rtl_should_mirror(rtl_element_t element);

/* Script name string. Returns static string. */
const char *rtl_script_name(rtl_script_t script);

/* Direction name string. Returns static string. */
const char *rtl_direction_name(rtl_direction_t direction);

/* Element type name string. Returns static string. */
const char *rtl_element_name(rtl_element_t element);

/* Mirror an x-coordinate for RTL layout.
 * x: original position, element_width: width of element,
 * container_width: total container width.
 * Returns mirrored x = container_width - x - element_width. */
double rtl_mirror_x(double x, double element_width, double container_width);

/* Check if a UTF-8 string starts with an RTL codepoint.
 * Skips whitespace and digits (neutral characters) to find first
 * strong directional codepoint. Returns false for NULL/empty. */
bool rtl_starts_rtl(const char *utf8_str);

/* Detect dominant direction of a UTF-8 string.
 * Counts RTL vs LTR codepoints. Returns NEUTRAL if no strong chars. */
rtl_direction_t rtl_dominant_direction(const char *utf8_str);

/* Number of scripts (equals RTL_SCRIPT_COUNT). */
int rtl_script_count(void);

/* Number of element types (equals RTL_ELEM_COUNT). */
int rtl_element_count(void);

#endif /* TIME_RTL_LAYOUT_H */
