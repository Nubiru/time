#include "iching_card.h"
#include "hexagram_visual.h"
#include "../systems/iching/iching.h"

#include <stdio.h>
#include <string.h>

void iching_fmt_title(int king_wen, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;
    const char *name = iching_hexagram_name(king_wen);
    snprintf(buf, sz, "Hexagram %d \xc2\xb7 %s", king_wen, name);
}

void iching_fmt_trigrams(int lower, int upper, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;
    const char *lower_sym = iching_trigram_symbol(lower);
    const char *upper_sym = iching_trigram_symbol(upper);
    const char *lower_elem = trigram_element(lower);
    const char *upper_elem = trigram_element(upper);
    snprintf(buf, sz, "%s %s / %s %s",
             upper_sym, upper_elem, lower_sym, lower_elem);
}

static iching_display_t build_display(hexagram_t hex)
{
    iching_display_t d;
    memset(&d, 0, sizeof(d));

    d.king_wen = hex.king_wen;

    iching_fmt_title(hex.king_wen, d.title, sizeof(d.title));
    iching_fmt_trigrams(hex.lower_trigram, hex.upper_trigram,
                        d.trigrams, sizeof(d.trigrams));

    /* Nuclear hexagram */
    int nuc = hexagram_nuclear(hex.king_wen);
    if (nuc > 0) {
        const char *nuc_name = iching_hexagram_name(nuc);
        snprintf(d.nuclear_line, sizeof(d.nuclear_line),
                 "Nuclear: %d \xc2\xb7 %s", nuc, nuc_name);
    } else {
        snprintf(d.nuclear_line, sizeof(d.nuclear_line), "Nuclear: ?");
    }

    /* Unicode symbol */
    const char *uni = hexagram_unicode(hex.king_wen);
    snprintf(d.unicode_sym, sizeof(d.unicode_sym), "%s", uni);

    /* ASCII art — hexagram_ascii returns [0]=top(line6) [5]=bottom(line1) */
    hexagram_art_t art = hexagram_ascii(hex.king_wen);
    for (int i = 0; i < ICHING_ART_LINES; i++) {
        snprintf(d.ascii_art[i], ICHING_ART_MAX, "%s", art.lines[i]);
    }

    /* Summary: "☷ 42 Yi · Wind/Thunder" */
    const char *upper_elem = trigram_element(hex.upper_trigram);
    const char *lower_elem = trigram_element(hex.lower_trigram);
    const char *name = iching_hexagram_name(hex.king_wen);
    snprintf(d.summary, sizeof(d.summary), "%s %d %s \xc2\xb7 %s/%s",
             d.unicode_sym, hex.king_wen, name, upper_elem, lower_elem);

    return d;
}

iching_display_t iching_display_from_jd(double jd)
{
    hexagram_t hex = iching_from_jd(jd);
    return build_display(hex);
}

iching_display_t iching_display_from_number(int king_wen)
{
    hexagram_t hex = iching_hexagram(king_wen);
    return build_display(hex);
}
