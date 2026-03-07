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

    /* Star field (point sprites + constellation lines) */
    GLuint star_program;
    GLint star_loc_mvp;
    GLint star_loc_scale;
    GLuint star_vao;
    GLuint star_vbo;
    int star_count;

    GLuint cline_program;
    GLint cline_loc_mvp;
    GLuint cline_vao;
    GLuint cline_vbo;
    int cline_vertex_count;

    /* Planet pack (point sprites with atmosphere glow) */
    GLuint pp_program;
    GLint pp_loc_mvp;
    GLint pp_loc_scale;
    GLuint pp_vao;
    GLuint pp_vbo;

    /* Planet pack trails (per-vertex alpha fade lines) */
    GLuint pp_trail_program;
    GLint pp_trail_loc_mvp;
    GLuint pp_trail_vao;
    GLuint pp_trail_vbo;

    /* Meshes */
    mesh_t sun_mesh;
    mesh_t planet_mesh;  /* still used for zodiac ring markers */

    /* Timing */
    double prev_time_ms;
#endif
} app_state_t;

/* Initialize with sensible defaults.
 * Does NOT create GPU resources — call after GL init. */
app_state_t app_state_create(float aspect_ratio);

#endif
