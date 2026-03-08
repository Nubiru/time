/*
 * mesh_shader.c — Shared lit 3D mesh shader source strings
 *
 * Pure module: no GL calls, no malloc, no globals.
 */

#include "mesh_shader.h"

static const char *s_vert_source =
    "#version 300 es\n"
    "layout(location = 0) in vec3 a_position;\n"
    "layout(location = 1) in vec3 a_normal;\n"
    "uniform mat4 u_model;\n"
    "uniform mat4 u_view;\n"
    "uniform mat4 u_proj;\n"
    "out vec3 v_normal;\n"
    "out vec3 v_position;\n"
    "void main() {\n"
    "    vec4 world_pos = u_model * vec4(a_position, 1.0);\n"
    "    v_position = world_pos.xyz;\n"
    "    v_normal = mat3(u_model) * a_normal;\n"
    "    gl_Position = u_proj * u_view * world_pos;\n"
    "}\n";

static const char *s_frag_source =
    "#version 300 es\n"
    "precision mediump float;\n"
    "in vec3 v_normal;\n"
    "in vec3 v_position;\n"
    "uniform vec3 u_color;\n"
    "uniform vec3 u_light_dir;\n"
    "uniform float u_emissive;\n"
    "uniform float u_opacity;\n"
    "out vec4 frag_color;\n"
    "void main() {\n"
    "    vec3 n = normalize(v_normal);\n"
    "    float diff = max(dot(n, u_light_dir), 0.0);\n"
    "    float ambient = 0.15;\n"
    "    vec3 lit = u_color * mix(ambient + diff * 0.85, 1.0, u_emissive);\n"
    "    float rim = 1.0 - max(dot(n, normalize(-v_position)), 0.0);\n"
    "    lit += u_color * u_emissive * rim * 0.3;\n"
    "    frag_color = vec4(lit, u_opacity);\n"
    "}\n";

const char *mesh_shader_vert_source(void) { return s_vert_source; }
const char *mesh_shader_frag_source(void) { return s_frag_source; }
