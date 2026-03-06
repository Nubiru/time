#include "color.h"

#include <math.h>

static float clampf(float x, float lo, float hi)
{
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

static float hue_to_rgb(float p, float q, float t)
{
    if (t < 0.0f) t += 1.0f;
    if (t > 1.0f) t -= 1.0f;
    if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
    if (t < 0.5f) return q;
    if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
    return p;
}

/* --- Conversion --- */

color_rgb_t color_hsl_to_rgb(color_hsl_t hsl)
{
    color_rgb_t rgb;
    if (hsl.s == 0.0f) {
        rgb.r = rgb.g = rgb.b = hsl.l;
    } else {
        float q = hsl.l < 0.5f
            ? hsl.l * (1.0f + hsl.s)
            : hsl.l + hsl.s - hsl.l * hsl.s;
        float p = 2.0f * hsl.l - q;
        float h = hsl.h / 360.0f;
        rgb.r = hue_to_rgb(p, q, h + 1.0f / 3.0f);
        rgb.g = hue_to_rgb(p, q, h);
        rgb.b = hue_to_rgb(p, q, h - 1.0f / 3.0f);
    }
    return rgb;
}

color_hsl_t color_rgb_to_hsl(color_rgb_t rgb)
{
    color_hsl_t hsl;
    float mx = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b)
                              : (rgb.g > rgb.b ? rgb.g : rgb.b);
    float mn = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b)
                              : (rgb.g < rgb.b ? rgb.g : rgb.b);

    hsl.l = (mx + mn) * 0.5f;

    if (mx == mn) {
        hsl.h = 0.0f;
        hsl.s = 0.0f;
    } else {
        float d = mx - mn;
        hsl.s = hsl.l > 0.5f ? d / (2.0f - mx - mn) : d / (mx + mn);

        if (mx == rgb.r)
            hsl.h = (rgb.g - rgb.b) / d + (rgb.g < rgb.b ? 6.0f : 0.0f);
        else if (mx == rgb.g)
            hsl.h = (rgb.b - rgb.r) / d + 2.0f;
        else
            hsl.h = (rgb.r - rgb.g) / d + 4.0f;

        hsl.h *= 60.0f;
    }
    return hsl;
}

/* --- Construction --- */

color_rgb_t color_rgb(float r, float g, float b)
{
    color_rgb_t c = {r, g, b};
    return c;
}

color_hsl_t color_hsl(float h, float s, float l)
{
    color_hsl_t c = {h, s, l};
    return c;
}

color_rgba_t color_rgba(float r, float g, float b, float a)
{
    color_rgba_t c = {r, g, b, a};
    return c;
}

color_rgb_t color_from_hex(unsigned int hex)
{
    color_rgb_t c;
    c.r = (float)((hex >> 16) & 0xFF) / 255.0f;
    c.g = (float)((hex >> 8) & 0xFF) / 255.0f;
    c.b = (float)(hex & 0xFF) / 255.0f;
    return c;
}

/* --- Interpolation --- */

color_rgb_t color_lerp(color_rgb_t a, color_rgb_t b, float t)
{
    color_rgb_t c;
    c.r = a.r + (b.r - a.r) * t;
    c.g = a.g + (b.g - a.g) * t;
    c.b = a.b + (b.b - a.b) * t;
    return c;
}

color_hsl_t color_lerp_hsl(color_hsl_t a, color_hsl_t b, float t)
{
    color_hsl_t c;

    /* Shortest-path hue interpolation */
    float diff = b.h - a.h;
    if (diff > 180.0f) diff -= 360.0f;
    if (diff < -180.0f) diff += 360.0f;
    c.h = a.h + diff * t;
    if (c.h < 0.0f) c.h += 360.0f;
    if (c.h >= 360.0f) c.h -= 360.0f;

    c.s = a.s + (b.s - a.s) * t;
    c.l = a.l + (b.l - a.l) * t;
    return c;
}

/* --- Modification --- */

color_hsl_t color_rotate_hue(color_hsl_t c, float degrees)
{
    c.h = fmodf(c.h + degrees, 360.0f);
    if (c.h < 0.0f) c.h += 360.0f;
    return c;
}

color_hsl_t color_saturate(color_hsl_t c, float amount)
{
    c.s = clampf(c.s + amount, 0.0f, 1.0f);
    return c;
}

color_hsl_t color_lighten(color_hsl_t c, float amount)
{
    c.l = clampf(c.l + amount, 0.0f, 1.0f);
    return c;
}

color_rgb_t color_multiply(color_rgb_t a, color_rgb_t b)
{
    color_rgb_t c;
    c.r = a.r * b.r;
    c.g = a.g * b.g;
    c.b = a.b * b.b;
    return c;
}

/* --- Palette generation --- */

color_hsl_t color_complementary(color_hsl_t c)
{
    return color_rotate_hue(c, 180.0f);
}

void color_triadic(color_hsl_t c, color_hsl_t out[3])
{
    out[0] = c;
    out[1] = color_rotate_hue(c, 120.0f);
    out[2] = color_rotate_hue(c, 240.0f);
}

void color_analogous(color_hsl_t c, float spread, color_hsl_t out[3])
{
    out[0] = color_rotate_hue(c, -spread);
    out[1] = c;
    out[2] = color_rotate_hue(c, spread);
}

/* --- Utility --- */

float color_luminance(color_rgb_t c)
{
    return 0.2126f * c.r + 0.7152f * c.g + 0.0722f * c.b;
}
