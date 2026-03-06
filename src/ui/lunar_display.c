#include "lunar_display.h"
#include "../systems/astronomy/lunar.h"
#include "../systems/astrology/zodiac.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

static const double SYNODIC_MONTH = 29.53058868;

void lunar_fmt_illumination(double illumination, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;
    double pct = illumination * 100.0;
    snprintf(buf, sz, "%.1f%%", pct);
}

void lunar_fmt_age(double age_days, double synodic_month, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;
    snprintf(buf, sz, "Day %.1f of %.1f", age_days, synodic_month);
}

void lunar_fmt_sign(int sign_index, char *buf, size_t sz)
{
    if (!buf || sz == 0) return;
    if (sign_index < 0 || sign_index > 11) {
        snprintf(buf, sz, "Moon in ?");
        return;
    }
    const char *name = zodiac_sign_name(sign_index);
    const char *sym = zodiac_sign_symbol(sign_index);
    snprintf(buf, sz, "Moon in %s %s", sym, name);
}

void lunar_fmt_countdown(const char *event_name, double days_until,
                         char *buf, size_t sz)
{
    if (!buf || sz == 0) return;
    snprintf(buf, sz, "%s in %.1fd", event_name, days_until);
}

void lunar_fmt_bar(double illumination, char *buf, size_t sz, int width)
{
    if (!buf || sz == 0) return;
    if (width <= 0 || (size_t)width >= sz) {
        buf[0] = '\0';
        return;
    }
    int filled = (int)(illumination * (double)width + 0.5);
    if (filled > width) filled = width;
    if (filled < 0) filled = 0;
    for (int i = 0; i < width; i++) {
        buf[i] = (i < filled) ? '#' : '.';
    }
    buf[width] = '\0';
}

lunar_display_t lunar_display(double jd)
{
    lunar_display_t d;
    memset(&d, 0, sizeof(d));

    lunar_info_t info = lunar_phase(jd);

    /* Phase line: "🌕 Full Moon" */
    const char *sym = lunar_phase_symbol(info.phase);
    const char *name = lunar_phase_name(info.phase);
    snprintf(d.phase_line, sizeof(d.phase_line), "%s %s", sym, name);

    /* Illumination */
    lunar_fmt_illumination(info.illumination, d.illumination,
                           sizeof(d.illumination));

    /* Age */
    lunar_fmt_age(info.age_days, SYNODIC_MONTH, d.age_line,
                  sizeof(d.age_line));

    /* Moon sign */
    int sign = lunar_zodiac_sign(jd);
    lunar_fmt_sign(sign, d.moon_sign, sizeof(d.moon_sign));

    /* Countdown to next new/full */
    double next_new_jd = lunar_next_new_moon(jd);
    double next_full_jd = lunar_next_full_moon(jd);
    lunar_fmt_countdown("New Moon", next_new_jd - jd,
                        d.next_new, sizeof(d.next_new));
    lunar_fmt_countdown("Full Moon", next_full_jd - jd,
                        d.next_full, sizeof(d.next_full));

    /* Summary: "🌕 Full Moon 98% · ♌ Leo" */
    int pct = (int)(info.illumination * 100.0 + 0.5);
    const char *sign_sym = zodiac_sign_symbol(sign);
    const char *sign_name = zodiac_sign_name(sign);
    snprintf(d.summary, sizeof(d.summary), "%s %s %d%% %s %s %s",
             sym, name, pct, "\xc2\xb7", sign_sym, sign_name);

    return d;
}
