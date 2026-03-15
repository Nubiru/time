/*
 * sun_shader.c — Procedural animated Sun shader source strings
 *
 * The Sun is rendered as a camera-facing billboard quad. The fragment shader
 * performs ray-sphere intersection against a unit sphere, then samples
 * layered animated simplex noise for the plasma surface. Corona rays and
 * limb darkening extend beyond the sphere edge.
 *
 * Brand colors: Solar gold {1.0, 0.85, 0.55} core, white-hot center,
 * deep orange-red at limb. All derived from the project palette.
 *
 * Pure module: no GL calls, no malloc, no globals.
 */

#include "sun_shader.h"
#include "noise_shader.h"
#include "shader_builder.h"

/* --- Vertex shader: camera-facing billboard --- */
static const char *s_vert_source =
    "#version 300 es\n"
    "layout(location = 0) in vec2 a_quad;\n"  /* -1..1 quad corners */
    "uniform mat4 u_view;\n"
    "uniform mat4 u_proj;\n"
    "uniform vec3 u_sun_pos;\n"
    "uniform float u_size;\n"
    "out vec2 v_uv;\n"
    "void main() {\n"
    "    v_uv = a_quad;\n"
    "    vec3 cam_right = vec3(u_view[0][0], u_view[1][0], u_view[2][0]);\n"
    "    vec3 cam_up    = vec3(u_view[0][1], u_view[1][1], u_view[2][1]);\n"
    "    vec3 world_pos = u_sun_pos\n"
    "        + cam_right * a_quad.x * u_size\n"
    "        + cam_up    * a_quad.y * u_size;\n"
    "    gl_Position = u_proj * u_view * vec4(world_pos, 1.0);\n"
    "}\n";

/* --- Fragment shader: procedural plasma sun ---
 *
 * Approach:
 * 1. Ray-sphere intersection against unit disk in UV space
 * 2. Surface normal from sphere math
 * 3. Layered animated fbm noise for plasma turbulence
 * 4. Limb darkening (physical: I ~ cos(theta)^0.6)
 * 5. Corona: soft exponential glow beyond sphere edge
 * 6. Color ramp: white-hot center -> solar gold -> deep orange at limb
 */
static const char *s_frag_preamble =
    "#version 300 es\n"
    "precision highp float;\n"
    "in vec2 v_uv;\n"
    "uniform float u_time;\n"
    "out vec4 frag_color;\n"
    "\n";

/* The noise functions get inserted between preamble and main body */

static const char *s_frag_body =
    "\n"
    /* Color palette — brand solar gold family */
    "const vec3 SUN_WHITE  = vec3(1.0, 0.98, 0.92);\n"
    "const vec3 SUN_GOLD   = vec3(1.0, 0.85, 0.55);\n"
    "const vec3 SUN_ORANGE = vec3(1.0, 0.55, 0.15);\n"
    "const vec3 SUN_RED    = vec3(0.85, 0.20, 0.05);\n"
    "const vec3 CORONA     = vec3(1.0, 0.75, 0.35);\n"
    "\n"
    "void main() {\n"
    "    float dist = length(v_uv);\n"
    "\n"
    "    /* --- Corona glow (beyond the sphere edge) --- */\n"
    "    if (dist > 1.0) {\n"
    "        float corona_dist = dist - 1.0;\n"
    "        /* Layered corona: fast falloff + slow halo */\n"
    "        float corona = 0.3 * exp(-corona_dist * 4.0)\n"
    "                      + 0.15 * exp(-corona_dist * 1.5);\n"
    "\n"
    "        /* Animated corona rays using angular noise */\n"
    "        float angle = atan(v_uv.y, v_uv.x);\n"
    "        float ray_noise = snoise2(vec2(angle * 3.0, u_time * 0.3));\n"
    "        ray_noise += 0.5 * snoise2(vec2(angle * 7.0, u_time * 0.5 + 10.0));\n"
    "        corona *= 1.0 + ray_noise * 0.4;\n"
    "\n"
    "        /* Color: golden fading to transparent */\n"
    "        vec3 corona_color = mix(CORONA, SUN_GOLD, corona_dist * 2.0);\n"
    "        frag_color = vec4(corona_color, corona * smoothstep(2.0, 1.0, dist));\n"
    "        return;\n"
    "    }\n"
    "\n"
    "    /* --- Sphere surface --- */\n"
    "    /* Reconstruct 3D point on unit sphere from 2D UV */\n"
    "    float z = sqrt(1.0 - dist * dist);\n"
    "    vec3 sphere_pos = vec3(v_uv, z);\n"
    "\n"
    "    /* Limb darkening: cos(theta) where theta = angle from center */\n"
    "    float limb = pow(z, 0.6);\n"
    "\n"
    "    /* --- Animated plasma turbulence --- */\n"
    "    /* Multiple noise layers at different scales and speeds */\n"
    "    float t = u_time * 0.08;\n"
    "    vec3 p1 = sphere_pos * 3.0 + vec3(0.0, 0.0, t);\n"
    "    vec3 p2 = sphere_pos * 6.0 + vec3(t * 0.7, 0.0, 0.0);\n"
    "    vec3 p3 = sphere_pos * 12.0 + vec3(0.0, t * 1.3, 0.0);\n"
    "\n"
    "    /* Large-scale convection cells */\n"
    "    float n1 = fbm3(p1) * 0.5 + 0.5;\n"
    "    /* Medium granulation */\n"
    "    float n2 = fbm3(p2) * 0.5 + 0.5;\n"
    "    /* Fine surface detail */\n"
    "    float n3 = snoise3(p3) * 0.5 + 0.5;\n"
    "\n"
    "    /* Combine: large structure + granulation + fine detail */\n"
    "    float plasma = n1 * 0.5 + n2 * 0.35 + n3 * 0.15;\n"
    "\n"
    "    /* Sunspot-like dark patches (rare, large) */\n"
    "    float spots = snoise3(sphere_pos * 2.0 + vec3(t * 0.02));\n"
    "    spots = smoothstep(0.55, 0.65, spots) * 0.3;\n"
    "\n"
    "    /* --- Color mapping --- */\n"
    "    /* Plasma intensity drives color ramp: hot (white) -> gold -> orange -> red */\n"
    "    float intensity = plasma * limb - spots;\n"
    "    intensity = clamp(intensity, 0.0, 1.0);\n"
    "\n"
    "    vec3 color;\n"
    "    if (intensity > 0.7) {\n"
    "        color = mix(SUN_GOLD, SUN_WHITE, (intensity - 0.7) / 0.3);\n"
    "    } else if (intensity > 0.4) {\n"
    "        color = mix(SUN_ORANGE, SUN_GOLD, (intensity - 0.4) / 0.3);\n"
    "    } else {\n"
    "        color = mix(SUN_RED, SUN_ORANGE, intensity / 0.4);\n"
    "    }\n"
    "\n"
    "    /* Brighten center, darken edges */\n"
    "    color *= 0.7 + limb * 0.5;\n"
    "\n"
    "    /* Slight edge glow: thin bright ring at the very limb */\n"
    "    float edge_glow = smoothstep(0.92, 0.98, dist) * (1.0 - smoothstep(0.98, 1.0, dist));\n"
    "    color += SUN_WHITE * edge_glow * 0.3;\n"
    "\n"
    "    /* Soft anti-aliased edge */\n"
    "    float alpha = smoothstep(1.0, 0.985, dist);\n"
    "\n"
    "    frag_color = vec4(color, alpha);\n"
    "}\n";

/* Concatenated sources: preamble + noise lib + body */
static shader_src_t s_frag_src;
static int s_frag_built = 0;

static const char *build_frag_source(void) {
    if (!s_frag_built) {
        shader_src_init(&s_frag_src);
        shader_src_append(&s_frag_src, s_frag_preamble);
        shader_src_append(&s_frag_src, noise_shader_source());
        shader_src_append(&s_frag_src, s_frag_body);
        s_frag_built = 1;
    }
    return shader_src_get(&s_frag_src);
}

const char *sun_shader_vert_source(void) { return s_vert_source; }
const char *sun_shader_frag_source(void) { return build_frag_source(); }
