#include "hd_card.h"
#include "../systems/human_design/human_design.h"

#include <stdio.h>
#include <string.h>

void hd_fmt_gate(int gate, int line, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;
    const char *name = hd_gate_name(gate);
    const char *kw = hd_gate_keyword(gate);
    snprintf(buf, sz, "Gate %d.%d \xc2\xb7 %s \xc2\xb7 %s",
             gate, line, name, kw);
}

void hd_fmt_gate_short(int gate, int line, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;
    snprintf(buf, sz, "%d.%d", gate, line);
}

void hd_fmt_cross(int sun_gate, int sun_line,
                  int earth_gate, int earth_line,
                  char *buf, size_t sz)
{
    if (!buf || sz == 0) return;
    snprintf(buf, sz, "Sun %d.%d / Earth %d.%d",
             sun_gate, sun_line, earth_gate, earth_line);
}

void hd_fmt_keyword(int gate, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;
    const char *kw = hd_gate_keyword(gate);
    snprintf(buf, sz, "%s", kw);
}

hd_display_t hd_display_from_longitude(double sun_longitude)
{
    hd_display_t d;
    memset(&d, 0, sizeof(d));

    hd_incarnation_t ic = hd_incarnation(sun_longitude);

    hd_fmt_gate(ic.sun.gate, ic.sun.line,
                d.sun_gate, sizeof(d.sun_gate));
    hd_fmt_gate(ic.earth.gate, ic.earth.line,
                d.earth_gate, sizeof(d.earth_gate));
    hd_fmt_cross(ic.sun.gate, ic.sun.line,
                 ic.earth.gate, ic.earth.line,
                 d.cross_line, sizeof(d.cross_line));
    hd_fmt_keyword(ic.sun.gate, d.sun_keyword, sizeof(d.sun_keyword));
    hd_fmt_keyword(ic.earth.gate, d.earth_keyword, sizeof(d.earth_keyword));

    const char *sun_name = hd_gate_name(ic.sun.gate);
    const char *earth_name = hd_gate_name(ic.earth.gate);
    snprintf(d.summary, sizeof(d.summary),
             "Gate %d.%d %s \xc2\xb7 Gate %d.%d %s",
             ic.sun.gate, ic.sun.line, sun_name,
             ic.earth.gate, ic.earth.line, earth_name);

    return d;
}
