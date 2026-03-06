#ifndef TIME_COLOR_H
#define TIME_COLOR_H

typedef struct {
    float r, g, b;    /* 0.0 - 1.0 */
} color_rgb_t;

typedef struct {
    float h, s, l;    /* h: 0-360 degrees, s: 0-1, l: 0-1 */
} color_hsl_t;

typedef struct {
    float r, g, b, a; /* 0.0 - 1.0 */
} color_rgba_t;

/* Conversion */
color_rgb_t  color_hsl_to_rgb(color_hsl_t hsl);
color_hsl_t  color_rgb_to_hsl(color_rgb_t rgb);

/* Construction */
color_rgb_t  color_rgb(float r, float g, float b);
color_hsl_t  color_hsl(float h, float s, float l);
color_rgba_t color_rgba(float r, float g, float b, float a);
color_rgb_t  color_from_hex(unsigned int hex);

/* Interpolation */
color_rgb_t  color_lerp(color_rgb_t a, color_rgb_t b, float t);
color_hsl_t  color_lerp_hsl(color_hsl_t a, color_hsl_t b, float t);

/* Modification */
color_hsl_t  color_rotate_hue(color_hsl_t c, float degrees);
color_hsl_t  color_saturate(color_hsl_t c, float amount);
color_hsl_t  color_lighten(color_hsl_t c, float amount);
color_rgb_t  color_multiply(color_rgb_t a, color_rgb_t b);

/* Palette generation */
color_hsl_t  color_complementary(color_hsl_t c);
void         color_triadic(color_hsl_t c, color_hsl_t out[3]);
void         color_analogous(color_hsl_t c, float spread, color_hsl_t out[3]);

/* Utility */
float        color_luminance(color_rgb_t c);

#endif
