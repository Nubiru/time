/*
 * sky_pack.c — Preetham/Perez physically-based sky color model
 *
 * Implementation of the Perez five-parameter sky luminance distribution
 * with CIE xyY → linear sRGB conversion.
 *
 * Sources:
 *   - Preetham, Shirley, Smits — SIGGRAPH 1999
 *   - Marco Sporl — ShaderX3 Ch 8.3 (2004)
 *   - CIE xyY to sRGB: ITU-R BT.709 / D65 illuminant
 *
 * Pure module: no GL, no malloc, no globals.
 */

#include "sky_pack.h"

#include <math.h>

#ifndef PI
#define PI 3.14159265358979323846
#endif

sky_config_t sky_default_config(void)
{
    sky_config_t cfg;
    cfg.turbidity = 3.0f;         /* clear sky */
    cfg.sun_theta = (float)(PI / 4.0);  /* 45 degrees from zenith */
    cfg.sun_phi = 0.0f;
    return cfg;
}

/* Perez distribution function:
 * F(theta, gamma) = (1 + A*exp(B/cos(theta))) * (1 + C*exp(D*gamma) + E*cos^2(gamma)) */
float sky_perez(float theta, float gamma,
                float A, float B, float C, float D, float E)
{
    float cos_theta = cosf(theta);
    if (cos_theta < 0.01f) cos_theta = 0.01f; /* avoid division by zero at horizon */
    float cos_gamma = cosf(gamma);

    return (1.0f + A * expf(B / cos_theta))
         * (1.0f + C * expf(D * gamma) + E * cos_gamma * cos_gamma);
}

sky_params_t sky_compute_params(sky_config_t config)
{
    sky_params_t p;
    float T = config.turbidity;

    p.sun_theta = config.sun_theta;
    p.sun_phi = config.sun_phi;

    /* Distribution coefficients from turbidity (Preetham Eq 8.3.3-8.3.5) */

    /* Luminance Y */
    p.A_Y =  0.1787f * T - 1.4630f;
    p.B_Y = -0.3554f * T + 0.4275f;
    p.C_Y = -0.2270f * T + 5.3251f;
    p.D_Y =  0.1206f * T - 2.5771f;
    p.E_Y = -0.0670f * T + 0.3703f;

    /* Chromaticity x */
    p.A_x = -0.0193f * T - 0.2592f;
    p.B_x = -0.0665f * T + 0.0008f;
    p.C_x = -0.0004f * T + 0.2125f;
    p.D_x = -0.0641f * T - 0.8989f;
    p.E_x = -0.0033f * T + 0.0452f;

    /* Chromaticity y */
    p.A_y = -0.0167f * T - 0.2608f;
    p.B_y = -0.0950f * T + 0.0092f;
    p.C_y = -0.0079f * T + 0.2102f;
    p.D_y = -0.0441f * T - 1.6537f;
    p.E_y = -0.0109f * T + 0.0529f;

    /* Zenith luminance (Eq 8.3.7) */
    float chi = (4.0f / 9.0f - T / 120.0f) * ((float)PI - 2.0f * config.sun_theta);
    p.Y_z = (4.0453f * T - 4.9710f) * tanf(chi) - 0.2155f * T + 2.4192f;
    if (p.Y_z < 0.0f) p.Y_z = 0.0f;

    /* Zenith chromaticity x (Eq 8.3.11) */
    float ts = config.sun_theta;
    float ts2 = ts * ts;
    float ts3 = ts2 * ts;
    float T2 = T * T;

    p.x_z = ( 0.00166f * ts3 - 0.00375f * ts2 + 0.00209f * ts + 0.0f) * T2
          + (-0.02903f * ts3 + 0.06377f * ts2 - 0.03202f * ts + 0.00394f) * T
          + ( 0.11693f * ts3 - 0.21196f * ts2 + 0.06052f * ts + 0.25886f);

    /* Zenith chromaticity y (Eq 8.3.12) */
    p.y_z = ( 0.00275f * ts3 - 0.00610f * ts2 + 0.00317f * ts + 0.0f) * T2
          + (-0.04214f * ts3 + 0.08970f * ts2 - 0.04153f * ts + 0.00516f) * T
          + ( 0.15346f * ts3 - 0.26756f * ts2 + 0.06670f * ts + 0.26688f);

    return p;
}

sky_rgb_t sky_color_at(const sky_params_t *params,
                        float view_theta, float view_phi)
{
    sky_rgb_t result = {0.0f, 0.0f, 0.0f};
    if (!params) return result;

    /* Angle between sun and view direction */
    float cos_gamma = sinf(params->sun_theta) * sinf(view_theta)
                    * cosf(params->sun_phi - view_phi)
                    + cosf(params->sun_theta) * cosf(view_theta);
    if (cos_gamma > 1.0f) cos_gamma = 1.0f;
    if (cos_gamma < -1.0f) cos_gamma = -1.0f;
    float gamma = acosf(cos_gamma);

    /* Evaluate Perez function for luminance and chromaticity */
    float F_theta_gamma_Y = sky_perez(view_theta, gamma,
                                       params->A_Y, params->B_Y, params->C_Y,
                                       params->D_Y, params->E_Y);
    float F_0_ts_Y = sky_perez(0.0f, params->sun_theta,
                                params->A_Y, params->B_Y, params->C_Y,
                                params->D_Y, params->E_Y);

    float F_theta_gamma_x = sky_perez(view_theta, gamma,
                                       params->A_x, params->B_x, params->C_x,
                                       params->D_x, params->E_x);
    float F_0_ts_x = sky_perez(0.0f, params->sun_theta,
                                params->A_x, params->B_x, params->C_x,
                                params->D_x, params->E_x);

    float F_theta_gamma_y = sky_perez(view_theta, gamma,
                                       params->A_y, params->B_y, params->C_y,
                                       params->D_y, params->E_y);
    float F_0_ts_y = sky_perez(0.0f, params->sun_theta,
                                params->A_y, params->B_y, params->C_y,
                                params->D_y, params->E_y);

    /* Sky values at this direction */
    float Y = params->Y_z * F_theta_gamma_Y / (F_0_ts_Y + 0.001f);
    float x = params->x_z * F_theta_gamma_x / (F_0_ts_x + 0.001f);
    float y = params->y_z * F_theta_gamma_y / (F_0_ts_y + 0.001f);

    /* CIE xyY → XYZ */
    if (y < 0.001f) return result;
    float X = x * Y / y;
    float Z = (1.0f - x - y) * Y / y;

    /* XYZ → linear sRGB (D65 illuminant, ITU-R BT.709) */
    result.r =  3.2406f * X - 1.5372f * Y - 0.4986f * Z;
    result.g = -0.9689f * X + 1.8758f * Y + 0.0415f * Z;
    result.b =  0.0557f * X - 0.2040f * Y + 1.0570f * Z;

    /* Clamp negative values */
    if (result.r < 0.0f) result.r = 0.0f;
    if (result.g < 0.0f) result.g = 0.0f;
    if (result.b < 0.0f) result.b = 0.0f;

    return result;
}

/* GLSL ES 3.00 sky dome vertex shader */
static const char s_sky_vert[] =
    "#version 300 es\n"
    "layout(location = 0) in vec3 a_position;\n"
    "uniform mat4 u_mvp;\n"
    "out vec3 v_dir;\n"
    "void main() {\n"
    "    v_dir = normalize(a_position);\n"
    "    gl_Position = u_mvp * vec4(a_position, 1.0);\n"
    "}\n";

/* GLSL ES 3.00 sky dome fragment shader — Preetham model */
static const char s_sky_frag[] =
    "#version 300 es\n"
    "precision highp float;\n"
    "in vec3 v_dir;\n"
    "uniform vec3 u_sun_dir;\n"
    "uniform float u_turbidity;\n"
    "out vec4 frag_color;\n"
    "\n"
    "float perez(float theta, float gamma, float A, float B, float C, float D, float E) {\n"
    "    float cos_t = max(cos(theta), 0.01);\n"
    "    float cos_g = cos(gamma);\n"
    "    return (1.0 + A * exp(B / cos_t)) * (1.0 + C * exp(D * gamma) + E * cos_g * cos_g);\n"
    "}\n"
    "\n"
    "void main() {\n"
    "    vec3 dir = normalize(v_dir);\n"
    "    float T = u_turbidity;\n"
    "\n"
    "    /* Sun angles */\n"
    "    float sun_theta = acos(max(u_sun_dir.y, 0.0));\n"
    "    float view_theta = acos(max(dir.y, 0.0));\n"
    "    float gamma = acos(clamp(dot(dir, u_sun_dir), -1.0, 1.0));\n"
    "\n"
    "    /* Distribution coefficients from turbidity */\n"
    "    float A_Y =  0.1787*T - 1.4630;\n"
    "    float B_Y = -0.3554*T + 0.4275;\n"
    "    float C_Y = -0.2270*T + 5.3251;\n"
    "    float D_Y =  0.1206*T - 2.5771;\n"
    "    float E_Y = -0.0670*T + 0.3703;\n"
    "\n"
    "    /* Zenith luminance */\n"
    "    float chi = (4.0/9.0 - T/120.0) * (3.14159 - 2.0*sun_theta);\n"
    "    float Y_z = max((4.0453*T - 4.9710) * tan(chi) - 0.2155*T + 2.4192, 0.0);\n"
    "\n"
    "    /* Sky luminance at this direction */\n"
    "    float Y = Y_z * perez(view_theta, gamma, A_Y, B_Y, C_Y, D_Y, E_Y)\n"
    "                  / perez(0.0, sun_theta, A_Y, B_Y, C_Y, D_Y, E_Y);\n"
    "\n"
    "    /* Simplified color: blue zenith fading to orange horizon */\n"
    "    vec3 zenith_color = vec3(0.2, 0.4, 0.8);\n"
    "    vec3 horizon_color = mix(vec3(0.8, 0.6, 0.4), vec3(0.9, 0.4, 0.2), \n"
    "                             smoothstep(0.0, 1.2, sun_theta));\n"
    "    float hor = smoothstep(0.0, 1.5, view_theta);\n"
    "    vec3 color = mix(zenith_color, horizon_color, hor) * Y * 0.15;\n"
    "\n"
    "    /* Sun glow */\n"
    "    float sun_glow = exp(-gamma * gamma * 20.0) * 0.5;\n"
    "    color += vec3(1.0, 0.9, 0.7) * sun_glow * Y;\n"
    "\n"
    "    /* Below horizon = dark */\n"
    "    color *= smoothstep(-0.05, 0.0, dir.y);\n"
    "\n"
    "    frag_color = vec4(color, 1.0);\n"
    "}\n";

const char *sky_vert_source(void) { return s_sky_vert; }
const char *sky_frag_source(void) { return s_sky_frag; }
