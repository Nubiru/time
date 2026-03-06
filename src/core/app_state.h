#ifndef TIME_APP_STATE_H
#define TIME_APP_STATE_H

#include "../math/vec3.h"
#include "../math/mat4.h"
#include "../render/camera.h"
#include "../render/camera_scale.h"
#include "../render/render_layers.h"

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#include "../render/mesh.h"
#endif

/* All mutable application state in one place.
 * Pure functions never touch this — they receive data and return data.
 * Only main.c and input handlers mutate this struct. */
typedef struct {
    /* Time */
    double simulation_jd;    /* current Julian Day in simulation */
    double time_speed;       /* days per real second (1.0 = real-time) */

    /* Camera */
    camera_t camera;
    scale_transition_t scale_transition;

    /* Mouse */
    int mouse_down;
    double mouse_x;
    double mouse_y;

    /* Observer location */
    double observer_lat;     /* degrees, positive north */
    double observer_lon;     /* degrees, positive east */

    /* Render layers */
    layer_config_t layer_configs[LAYER_COUNT];
    layer_state_t layer_state;

    /* Visual toggles */
    int show_trails;       /* orbit trail lines */
    int show_hud;          /* time HUD overlay */

#ifdef __EMSCRIPTEN__
    /* GPU resources (created once, reused) */
    GLuint program;
    GLint loc_model;
    GLint loc_view;
    GLint loc_proj;
    GLint loc_color;
    GLint loc_light_dir;
    GLint loc_emissive;

    /* Trail shader (gl_VertexID based, no vertex buffers) */
    GLuint trail_program;
    GLint trail_loc_view;
    GLint trail_loc_proj;
    GLint trail_loc_color;
    GLint trail_loc_a;        /* semi-major axis */
    GLint trail_loc_e;        /* eccentricity */
    GLint trail_loc_i;        /* inclination (rad) */
    GLint trail_loc_omega_n;  /* longitude of ascending node (rad) */
    GLint trail_loc_omega;    /* argument of perihelion (rad) */
    GLuint trail_vao;         /* empty VAO for gl_VertexID drawing */

    GLint loc_opacity;        /* alpha for planet/marker fading */

    /* Zodiac ring shader (vertex-colored ring on ecliptic plane) */
    GLuint ring_program;
    GLint ring_loc_view;
    GLint ring_loc_proj;
    GLint ring_loc_opacity;
    GLuint ring_vao;
    GLuint ring_vbo_pos;
    GLuint ring_vbo_color;
    GLuint ring_ebo;
    int ring_index_count;

    /* Aspect line shader (per-vertex RGBA, GL_LINES) */
    GLuint line_program;
    GLint line_loc_view;
    GLint line_loc_proj;
    GLint line_loc_opacity;
    GLuint line_vao;
    GLuint line_vbo;

    /* Zodiac glyph shader (textured billboards for sign labels) */
    GLuint glyph_program;
    GLint glyph_loc_view;
    GLint glyph_loc_proj;
    GLint glyph_loc_opacity;
    GLuint glyph_texture;
    GLuint glyph_vao;
    GLuint glyph_vbo;
    GLuint glyph_ebo;

    /* Meshes */
    mesh_t sun_mesh;
    mesh_t planet_mesh;

    /* Timing */
    double prev_time_ms;
#endif
} app_state_t;

/* Initialize with sensible defaults.
 * Does NOT create GPU resources — call after GL init. */
app_state_t app_state_create(float aspect_ratio);

#endif
