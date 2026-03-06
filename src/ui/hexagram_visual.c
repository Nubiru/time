#include "hexagram_visual.h"

#include <string.h>
#include <stdio.h>

static const char *TRIGRAM_ELEMENTS[8] = {
    "Heaven",   /* 0: Qian */
    "Lake",     /* 1: Dui  */
    "Fire",     /* 2: Li   */
    "Thunder",  /* 3: Zhen */
    "Wind",     /* 4: Xun  */
    "Water",    /* 5: Kan  */
    "Mountain", /* 6: Gen  */
    "Earth"     /* 7: Kun  */
};

/* Unicode hexagram symbols U+4DC0..U+4DFF in King Wen order (1-64).
 * Each codepoint is 3 bytes in UTF-8: 0xE4 0xB7 0x80 + offset. */
static const char HEXAGRAM_UNICODE[64][4] = {
    "\xe4\xb7\x80", "\xe4\xb7\x81", "\xe4\xb7\x82", "\xe4\xb7\x83",
    "\xe4\xb7\x84", "\xe4\xb7\x85", "\xe4\xb7\x86", "\xe4\xb7\x87",
    "\xe4\xb7\x88", "\xe4\xb7\x89", "\xe4\xb7\x8a", "\xe4\xb7\x8b",
    "\xe4\xb7\x8c", "\xe4\xb7\x8d", "\xe4\xb7\x8e", "\xe4\xb7\x8f",
    "\xe4\xb7\x90", "\xe4\xb7\x91", "\xe4\xb7\x92", "\xe4\xb7\x93",
    "\xe4\xb7\x94", "\xe4\xb7\x95", "\xe4\xb7\x96", "\xe4\xb7\x97",
    "\xe4\xb7\x98", "\xe4\xb7\x99", "\xe4\xb7\x9a", "\xe4\xb7\x9b",
    "\xe4\xb7\x9c", "\xe4\xb7\x9d", "\xe4\xb7\x9e", "\xe4\xb7\x9f",
    "\xe4\xb7\xa0", "\xe4\xb7\xa1", "\xe4\xb7\xa2", "\xe4\xb7\xa3",
    "\xe4\xb7\xa4", "\xe4\xb7\xa5", "\xe4\xb7\xa6", "\xe4\xb7\xa7",
    "\xe4\xb7\xa8", "\xe4\xb7\xa9", "\xe4\xb7\xaa", "\xe4\xb7\xab",
    "\xe4\xb7\xac", "\xe4\xb7\xad", "\xe4\xb7\xae", "\xe4\xb7\xaf",
    "\xe4\xb7\xb0", "\xe4\xb7\xb1", "\xe4\xb7\xb2", "\xe4\xb7\xb3",
    "\xe4\xb7\xb4", "\xe4\xb7\xb5", "\xe4\xb7\xb6", "\xe4\xb7\xb7",
    "\xe4\xb7\xb8", "\xe4\xb7\xb9", "\xe4\xb7\xba", "\xe4\xb7\xbb",
    "\xe4\xb7\xbc", "\xe4\xb7\xbd", "\xe4\xb7\xbe", "\xe4\xb7\xbf",
};

int hexagram_nuclear(int king_wen)
{
    if (king_wen < 1 || king_wen > 64) return -1;

    hexagram_t h = iching_hexagram(king_wen);

    /* Nuclear lower trigram: lines[1], lines[2], lines[3] */
    /* Nuclear upper trigram: lines[2], lines[3], lines[4] */
    int nuclear_lines[6];
    nuclear_lines[0] = h.lines[1];
    nuclear_lines[1] = h.lines[2];
    nuclear_lines[2] = h.lines[3];
    nuclear_lines[3] = h.lines[2];
    nuclear_lines[4] = h.lines[3];
    nuclear_lines[5] = h.lines[4];

    return iching_lines_to_king_wen(nuclear_lines);
}

int hexagram_complement(int king_wen)
{
    if (king_wen < 1 || king_wen > 64) return -1;

    hexagram_t h = iching_hexagram(king_wen);
    int inv[6];
    for (int i = 0; i < 6; i++) {
        inv[i] = h.lines[i] ? 0 : 1;
    }
    return iching_lines_to_king_wen(inv);
}

int hexagram_reverse(int king_wen)
{
    if (king_wen < 1 || king_wen > 64) return -1;

    hexagram_t h = iching_hexagram(king_wen);
    int rev[6];
    for (int i = 0; i < 6; i++) {
        rev[i] = h.lines[5 - i];
    }
    return iching_lines_to_king_wen(rev);
}

hexagram_art_t hexagram_ascii(int king_wen)
{
    hexagram_art_t art;
    memset(&art, 0, sizeof(art));

    if (king_wen < 1 || king_wen > 64) return art;

    hexagram_t h = iching_hexagram(king_wen);

    /* art.lines[0] = top = line 6, art.lines[5] = bottom = line 1 */
    for (int i = 0; i < 6; i++) {
        int line_idx = 5 - i; /* map art row 0 -> hexagram line 5 (top) */
        if (h.lines[line_idx]) {
            /* Yang: solid line */
            snprintf(art.lines[i], HEX_ART_LINE_MAX, "---------");
        } else {
            /* Yin: broken line */
            snprintf(art.lines[i], HEX_ART_LINE_MAX, "---   ---");
        }
    }
    return art;
}

char *hexagram_summary(int king_wen, char *buf, int max)
{
    if (!buf || max <= 0) return buf;

    if (king_wen < 1 || king_wen > 64) {
        snprintf(buf, (unsigned long)max, "Invalid hexagram (%d)", king_wen);
        return buf;
    }

    hexagram_t h = iching_hexagram(king_wen);
    const char *name = iching_hexagram_name(king_wen);
    const char *lower = trigram_element(h.lower_trigram);
    const char *upper = trigram_element(h.upper_trigram);

    snprintf(buf, (unsigned long)max, "%d. %s — %s/%s", king_wen, name, upper, lower);
    return buf;
}

const char *trigram_element(int trigram)
{
    if (trigram < 0 || trigram > 7) return "?";
    return TRIGRAM_ELEMENTS[trigram];
}

int hexagram_is_complement(int kw_a, int kw_b)
{
    if (kw_a < 1 || kw_a > 64 || kw_b < 1 || kw_b > 64) return 0;
    return hexagram_complement(kw_a) == kw_b;
}

const char *hexagram_unicode(int king_wen)
{
    if (king_wen < 1 || king_wen > 64) return "?";
    return HEXAGRAM_UNICODE[king_wen - 1];
}
