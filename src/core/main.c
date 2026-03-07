#include <stdio.h>
#include <math.h>
#include "app_state.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES3/gl3.h>
#include "../render/gl_init.h"
#include "../render/shader.h"
#include "../render/camera.h"
#include "../render/mesh.h"
#include "../math/mat4.h"
#include "../systems/astronomy/orbit.h"
#include "../systems/astronomy/planets.h"
#include "../systems/gregorian/gregorian.h"
#include "../systems/astrology/zodiac.h"
#include "../systems/tzolkin/tzolkin.h"
#include "../systems/chinese/chinese.h"
#include "../systems/astrology/observer.h"
#include "../systems/astrology/aspects.h"
#include "../systems/astrology/houses.h"
#include "../render/camera_scale.h"
#include "../render/ring_geometry.h"
#include "../render/color_palette.h"
#include "../render/aspect_lines.h"
#include "../render/cusp_lines.h"
#include "../render/billboard.h"
#include "../render/star_field.h"
#include "../render/planet_pack.h"
#include "../systems/astronomy/planet_data.h"
#include "../ui/zodiac_glyphs.h"
#include "../math/sidereal.h"
#include "../math/ecliptic.h"
#endif

#define PI 3.14159265358979323846
#define DEG_TO_RAD (PI / 180.0)

#ifdef __EMSCRIPTEN__

/* Single mutable state — the only global in the entire program. */
static app_state_t g_state;

static const char *vert_source =
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

static const char *frag_source =
    "#version 300 es\n"
    "precision mediump float;\n"
    "in vec3 v_normal;\n"
    "in vec3 v_position;\n"
    "uniform vec3 u_color;\n"
    "uniform vec3 u_light_dir;\n"
    "uniform float u_emissive;\n"  /* 1.0 = self-luminous (Sun), 0.0 = diffuse-lit */
    "uniform float u_opacity;\n"
    "out vec4 frag_color;\n"
    "void main() {\n"
    "    vec3 n = normalize(v_normal);\n"
    "    float diff = max(dot(n, u_light_dir), 0.0);\n"
    "    float ambient = 0.15;\n"
    "    vec3 lit = u_color * mix(ambient + diff * 0.85, 1.0, u_emissive);\n"
    /* Soft glow at edges for emissive objects */
    "    float rim = 1.0 - max(dot(n, normalize(-v_position)), 0.0);\n"
    "    lit += u_color * u_emissive * rim * 0.3;\n"
    "    frag_color = vec4(lit, u_opacity);\n"
    "}\n";

/* (Orbit trail shaders now provided by planet_pack module) */

/* --- Zodiac ring shaders (vertex-colored, no lighting) --- */

static const char *ring_vert_source =
    "#version 300 es\n"
    "layout(location = 0) in vec3 a_position;\n"
    "layout(location = 1) in vec3 a_color;\n"
    "uniform mat4 u_view;\n"
    "uniform mat4 u_proj;\n"
    "out vec3 v_color;\n"
    "void main() {\n"
    "    gl_Position = u_proj * u_view * vec4(a_position, 1.0);\n"
    "    v_color = a_color;\n"
    "}\n";

static const char *ring_frag_source =
    "#version 300 es\n"
    "precision mediump float;\n"
    "in vec3 v_color;\n"
    "uniform float u_opacity;\n"
    "out vec4 frag_color;\n"
    "void main() {\n"
    "    frag_color = vec4(v_color, u_opacity);\n"
    "}\n";

/* --- Aspect line shaders (per-vertex RGBA, for chord lines inside ring) --- */

static const char *line_vert_source =
    "#version 300 es\n"
    "layout(location = 0) in vec3 a_position;\n"
    "layout(location = 1) in vec4 a_color;\n"
    "uniform mat4 u_view;\n"
    "uniform mat4 u_proj;\n"
    "uniform float u_opacity;\n"
    "out vec4 v_color;\n"
    "void main() {\n"
    "    gl_Position = u_proj * u_view * vec4(a_position, 1.0);\n"
    "    v_color = vec4(a_color.rgb, a_color.a * u_opacity);\n"
    "}\n";

static const char *line_frag_source =
    "#version 300 es\n"
    "precision mediump float;\n"
    "in vec4 v_color;\n"
    "out vec4 frag_color;\n"
    "void main() {\n"
    "    frag_color = v_color;\n"
    "}\n";

/* --- Zodiac glyph shaders (textured billboards for sign labels on ring) --- */

static const char *glyph_vert_source =
    "#version 300 es\n"
    "layout(location = 0) in vec3 a_position;\n"
    "layout(location = 1) in vec2 a_uv;\n"
    "layout(location = 2) in vec3 a_color;\n"
    "uniform mat4 u_view;\n"
    "uniform mat4 u_proj;\n"
    "out vec2 v_uv;\n"
    "out vec3 v_color;\n"
    "void main() {\n"
    "    gl_Position = u_proj * u_view * vec4(a_position, 1.0);\n"
    "    v_uv = a_uv;\n"
    "    v_color = a_color;\n"
    "}\n";

static const char *glyph_frag_source =
    "#version 300 es\n"
    "precision mediump float;\n"
    "in vec2 v_uv;\n"
    "in vec3 v_color;\n"
    "uniform sampler2D u_texture;\n"
    "uniform float u_opacity;\n"
    "out vec4 frag_color;\n"
    "void main() {\n"
    "    float a = texture(u_texture, v_uv).r;\n"
    "    if (a < 0.5) discard;\n"
    "    frag_color = vec4(v_color, u_opacity);\n"
    "}\n";

#define GLYPH_ATLAS_W 96   /* 12 signs × 8 pixels wide */
#define GLYPH_ATLAS_H 12   /* 12 pixels tall */
#define SIGN_LABEL_SIZE 0.4f
#define SIGN_LABEL_HEIGHT 0.3f  /* float above the ring plane */

/* Ring placement: between Mars (sqrt(1.52)*3 ≈ 3.7) and Jupiter (sqrt(5.2)*3 ≈ 6.8) */
#define RING_INNER_RADIUS 4.2f
#define RING_OUTER_RADIUS 4.8f
#define RING_MID_RADIUS   ((RING_INNER_RADIUS + RING_OUTER_RADIUS) / 2.0f)
#define RING_SEGMENTS_PER_SIGN 8

/* sqrt scale: compresses outer planets while keeping inner planets visible.
 * Mercury ~1.9, Earth ~3.0, Jupiter ~6.8, Neptune ~16.4 scene units. */
static const float ORBIT_SCALE = 3.0f;

/* Planet pack uses linear AU scaling. We post-process to sqrt for visual compression. */
static void sqrt_scale_planet_vertices(float *verts, int count) {
    for (int i = 0; i < count; i++) {
        int base = i * PP_PLANET_VERTEX_FLOATS;
        float x = verts[base + 0];
        float y = verts[base + 1];
        float z = verts[base + 2];
        /* Convert from linear to sqrt distance scaling */
        float r_linear = sqrtf(x * x + y * y + z * z);
        if (r_linear > 0.001f) {
            float r_sqrt = sqrtf(r_linear) * ORBIT_SCALE;
            float scale = r_sqrt / r_linear;
            verts[base + 0] = x * scale;
            verts[base + 1] = y * scale;
            verts[base + 2] = z * scale;
        }
    }
}

/* Same sqrt scaling for trail vertices (7 floats per vertex: pos3 + color4) */
static void sqrt_scale_trail_vertices(float *verts, int count) {
    for (int i = 0; i < count; i++) {
        int base = i * PP_TRAIL_VERTEX_FLOATS;
        float x = verts[base + 0];
        float y = verts[base + 1];
        float z = verts[base + 2];
        float r_linear = sqrtf(x * x + y * y + z * z);
        if (r_linear > 0.001f) {
            float r_sqrt = sqrtf(r_linear) * ORBIT_SCALE;
            float scale = r_sqrt / r_linear;
            verts[base + 0] = x * scale;
            verts[base + 1] = y * scale;
            verts[base + 2] = z * scale;
        }
    }
}

/* --- Input callbacks (only places besides main_loop that mutate g_state) --- */

static EM_BOOL on_mouse_down(int type, const EmscriptenMouseEvent *e, void *data) {
    (void)type; (void)data;
    g_state.mouse_down = 1;
    g_state.mouse_x = e->clientX;
    g_state.mouse_y = e->clientY;
    return EM_TRUE;
}

static EM_BOOL on_mouse_up(int type, const EmscriptenMouseEvent *e, void *data) {
    (void)type; (void)e; (void)data;
    g_state.mouse_down = 0;
    return EM_TRUE;
}

static EM_BOOL on_mouse_move(int type, const EmscriptenMouseEvent *e, void *data) {
    (void)type; (void)data;
    if (!g_state.mouse_down) return EM_FALSE;

    double dx = e->clientX - g_state.mouse_x;
    double dy = e->clientY - g_state.mouse_y;
    g_state.mouse_x = e->clientX;
    g_state.mouse_y = e->clientY;

    camera_rotate(&g_state.camera, (float)(-dx * 0.005), (float)(-dy * 0.005));
    return EM_TRUE;
}

static EM_BOOL on_wheel(int type, const EmscriptenWheelEvent *e, void *data) {
    (void)type; (void)data;
    camera_zoom(&g_state.camera, (float)(e->deltaY * 0.01));
    return EM_TRUE;
}

/* Time speed presets (days per real second) */
static const double TIME_SPEEDS[] = {
    0.0,         /* 0: paused */
    1.0,         /* 1: real-time (1 day/day) */
    60.0,        /* 2: 1 minute/sec */
    3600.0,      /* 3: 1 hour/sec */
    86400.0,     /* 4: 1 day/sec */
    864000.0     /* 5: 10 days/sec */
};

static EM_BOOL on_key_down(int type, const EmscriptenKeyboardEvent *e, void *data) {
    (void)type; (void)data;

    if (e->key[0] == ' ' && e->key[1] == '\0') {
        /* Space: toggle pause */
        if (g_state.time_speed == 0.0)
            g_state.time_speed = 1.0;
        else
            g_state.time_speed = 0.0;
        return EM_TRUE;
    }

    if (e->key[0] >= '0' && e->key[0] <= '5' && e->key[1] == '\0') {
        int idx = e->key[0] - '0';
        double sign = (g_state.time_speed < 0.0) ? -1.0 : 1.0;
        g_state.time_speed = sign * TIME_SPEEDS[idx];
        return EM_TRUE;
    }

    if (e->key[0] == '-' && e->key[1] == '\0') {
        /* Minus: reverse time direction */
        g_state.time_speed = -g_state.time_speed;
        return EM_TRUE;
    }

    if ((e->key[0] == 't' || e->key[0] == 'T') && e->key[1] == '\0') {
        g_state.show_trails = !g_state.show_trails;
        return EM_TRUE;
    }

    if ((e->key[0] == 'h' || e->key[0] == 'H') && e->key[1] == '\0') {
        g_state.show_hud = !g_state.show_hud;
        EM_ASM({
            var hud = document.getElementById('time-hud');
            if (hud) hud.style.display = $0 ? 'block' : 'none';
        }, g_state.show_hud);
        return EM_TRUE;
    }

    /* Shift + 0-6: jump to scale level (check code field, since
     * shift+digit produces symbols in key field on most layouts) */
    if (e->shiftKey) {
        int target = -1;
        if (e->code[0] == 'D' && e->code[1] == 'i' && e->code[2] == 'g' &&
            e->code[3] == 'i' && e->code[4] == 't' &&
            e->code[5] >= '0' && e->code[5] <= '6' && e->code[6] == '\0') {
            target = e->code[5] - '0';
        }
        if (target >= 0 && target <= 6) {
            g_state.scale_transition = scale_transition_create(
                g_state.camera.log_zoom, (scale_id_t)target, 1.2f);
            return EM_TRUE;
        }
    }

    return EM_FALSE;
}

/* Planet colors for zodiac ring markers (from planet_pack data) */
static void planet_pack_color(int planet_idx, float *r, float *g, float *b) {
    planet_color_t c = planet_data_color(planet_idx);
    *r = c.r; *g = c.g; *b = c.b;
}

/* --- HUD: push time data to DOM overlay --- */

static void degrees_to_hms_buf(double deg, char *buf, size_t sz) {
    deg = fmod(deg, 360.0);
    if (deg < 0.0) deg += 360.0;
    double hours = deg / 15.0;
    int h = (int)hours;
    int m = (int)((hours - h) * 60.0);
    int s = (int)((hours - h - m / 60.0) * 3600.0 + 0.5);
    if (s >= 60) { s -= 60; m++; }
    if (m >= 60) { m -= 60; h++; }
    if (h >= 24) h -= 24;
    snprintf(buf, sz, "%02dh %02dm %02ds", h, m, s);
}

static void update_hud(double jd, double time_speed,
                       double obs_lat, double obs_lon,
                       float log_zoom) {
    /* Gregorian */
    char date[16], time_str[16];
    gregorian_format_date(jd, date, sizeof(date));
    gregorian_format_time(jd, time_str, sizeof(time_str));
    int dow = gregorian_day_of_week(jd);
    const char *day_name = gregorian_day_name(dow);

    /* JD */
    char jd_str[24];
    snprintf(jd_str, sizeof(jd_str), "%.4f", jd);

    /* Sidereal — GMST and LST */
    double jd_0h = floor(jd - 0.5) + 0.5;
    double gmst = gmst_degrees(jd_0h);
    char gmst_str[16];
    degrees_to_hms_buf(gmst, gmst_str, sizeof(gmst_str));

    double lst = lst_degrees(gmst, obs_lon);
    char lst_str[16];
    degrees_to_hms_buf(lst, lst_str, sizeof(lst_str));

    /* Ascendant */
    double obliquity = mean_obliquity(jd);
    double asc = ascendant_longitude(lst, obliquity, obs_lat);
    int asc_sign = zodiac_sign(asc);
    double asc_deg = zodiac_degree(asc);
    char asc_str[32];
    snprintf(asc_str, sizeof(asc_str), "ASC %s %s %.0f",
             zodiac_sign_symbol(asc_sign), zodiac_sign_name(asc_sign), asc_deg);

    /* Houses (Whole Sign) */
    house_system_t houses = houses_whole_sign(asc);
    int mc_sign = houses.signs[9]; /* 10th house = MC sign */
    char house_str[48];
    snprintf(house_str, sizeof(house_str), "MC %s %s",
             zodiac_sign_symbol(mc_sign), zodiac_sign_name(mc_sign));

    /* Tzolkin */
    tzolkin_day_t kin = tzolkin_from_jd(jd);
    char tzolkin_str[64];
    snprintf(tzolkin_str, sizeof(tzolkin_str), "%d %s (Kin %d)",
             kin.tone, tzolkin_seal_name(kin.seal), kin.kin);

    /* Chinese calendar */
    chinese_year_t cy = chinese_year_from_jd(jd);
    char chinese_str[64];
    snprintf(chinese_str, sizeof(chinese_str), "%s %s %s %s",
             chinese_animal_symbol(cy.animal),
             chinese_element_name(cy.element),
             chinese_animal_name(cy.animal),
             chinese_polarity_name(cy.polarity));

    /* Sun sign (geocentric = Earth heliocentric + 180) */
    solar_system_t sys = planets_at(jd);
    double earth_lon = sys.positions[PLANET_EARTH].longitude;
    double sun_lon = fmod(earth_lon + 180.0, 360.0);
    int sign = zodiac_sign(sun_lon);
    double deg_in_sign = zodiac_degree(sun_lon);
    char sun_str[32];
    snprintf(sun_str, sizeof(sun_str), "%s %s %.1fd",
             zodiac_sign_symbol(sign), zodiac_sign_name(sign), deg_in_sign);

    /* Aspects — approximate geocentric longitudes */
    double geo_lons[8];
    for (int p = 0; p < 8; p++) {
        if (p == PLANET_EARTH) {
            geo_lons[p] = sun_lon; /* Sun's geocentric position */
        } else {
            /* Simplified geocentric: helio longitude - Earth longitude + 180
             * (proper calculation needs full coordinate transform, this is approximate) */
            double gl = fmod(sys.positions[p].longitude - earth_lon + 180.0, 360.0);
            if (gl < 0.0) gl += 360.0;
            geo_lons[p] = gl;
        }
    }
    aspect_list_t aspects = aspects_find_all(geo_lons, 8.0);
    char aspect_str[128];
    aspect_str[0] = '\0';
    int apos = 0;
    int max_show = 3; /* show top 3 tightest aspects */
    for (int a = 0; a < aspects.count && a < max_show; a++) {
        aspect_t asp = aspects.aspects[a];
        const char *pa = planet_symbol(asp.planet_a);
        const char *pb = planet_symbol(asp.planet_b);
        const char *sym = aspect_symbol(asp.type);
        int written = snprintf(aspect_str + apos, sizeof(aspect_str) - (size_t)apos,
                               "%s%s%s%s", a > 0 ? " " : "", pa, sym, pb);
        if (written > 0) apos += written;
    }
    if (aspects.count == 0) {
        snprintf(aspect_str, sizeof(aspect_str), "no major aspects");
    }

    /* Speed */
    char speed_str[24];
    if (time_speed == 0.0)
        snprintf(speed_str, sizeof(speed_str), "PAUSED");
    else
        snprintf(speed_str, sizeof(speed_str), "%.0fx", time_speed);

    /* Scale level */
    scale_id_t current_scale = scale_from_log_zoom(log_zoom);
    const char *scale_str = scale_name(current_scale);

    /* Build HTML and push to DOM */
    char html[1024];
    snprintf(html, sizeof(html),
        "%s %s %s<br>"
        "JD %s<br>"
        "GMST %s &middot; LST %s<br>"
        "%s &middot; %s<br>"
        "%s<br>"
        "%s &middot; %s<br>"
        "Aspects: %s<br>"
        "%s &middot; Speed: %s",
        date, time_str, day_name,
        jd_str,
        gmst_str, lst_str,
        asc_str, house_str,
        tzolkin_str,
        sun_str, chinese_str,
        aspect_str,
        scale_str, speed_str);

    EM_ASM({
        var hud = document.getElementById('time-hud');
        if (hud) hud.innerHTML = UTF8ToString($0);
    }, html);
}

#endif /* __EMSCRIPTEN__ */

void main_loop(void) {
#ifdef __EMSCRIPTEN__
    /* --- Time advancement --- */
    double now_ms = emscripten_get_now();
    double dt_sec = (now_ms - g_state.prev_time_ms) / 1000.0;
    g_state.prev_time_ms = now_ms;
    g_state.simulation_jd += g_state.time_speed * dt_sec;

    /* Advance scale transition (smooth camera zoom between levels) */
    if (g_state.scale_transition.active) {
        g_state.scale_transition = scale_transition_tick(
            g_state.scale_transition, (float)dt_sec);
        float new_log_zoom = scale_transition_value(g_state.scale_transition);
        g_state.camera.log_zoom = new_log_zoom;
        g_state.camera.distance = expf(new_log_zoom);
    }

    /* --- Compute layer visibility from current zoom --- */
    g_state.layer_state = layers_compute(g_state.layer_configs,
                                         g_state.camera.log_zoom);

    /* --- Clear --- */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(g_state.program);

    /* --- Camera matrices (pure computation from state) --- */
    mat4_t view = camera_view_matrix(&g_state.camera);
    mat4_t proj = camera_projection_matrix(&g_state.camera);
    glUniformMatrix4fv(g_state.loc_view, 1, GL_FALSE, view.m);
    glUniformMatrix4fv(g_state.loc_proj, 1, GL_FALSE, proj.m);

    /* --- Star field (background, drawn first, behind everything) --- */
    if (layer_is_visible(g_state.layer_state, LAYER_STARS)) {
        float star_opacity = g_state.layer_state.opacity[LAYER_STARS];
        mat4_t star_vp = mat4_multiply(proj, view);

        /* Stars: point sprites with additive blending */
        glUseProgram(g_state.star_program);
        glUniformMatrix4fv(g_state.star_loc_mvp, 1, GL_FALSE, star_vp.m);
        glUniform1f(g_state.star_loc_scale, star_opacity);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE); /* additive blending for stars */
        glDepthMask(GL_FALSE); /* don't write to depth — stars are infinitely far */

        glBindVertexArray(g_state.star_vao);
        glDrawArrays(GL_POINTS, 0, g_state.star_count);
        glBindVertexArray(0);

        /* Constellation lines */
        glUseProgram(g_state.cline_program);
        glUniformMatrix4fv(g_state.cline_loc_mvp, 1, GL_FALSE, star_vp.m);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); /* normal blend for lines */

        glBindVertexArray(g_state.cline_vao);
        glDrawArrays(GL_LINES, 0, g_state.cline_vertex_count);
        glBindVertexArray(0);

        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }

    /* --- Sun + Planets (only when LAYER_PLANETS visible) --- */
    if (layer_is_visible(g_state.layer_state, LAYER_PLANETS)) {
        glUniform1f(g_state.loc_opacity, 1.0f);
        /* Sun: emissive yellow sphere at origin */
        glUniform1f(g_state.loc_emissive, 1.0f);
        mat4_t sun_model = mat4_scale(0.4f, 0.4f, 0.4f);
        glUniformMatrix4fv(g_state.loc_model, 1, GL_FALSE, sun_model.m);
        glUniform3f(g_state.loc_color, 1.0f, 0.85f, 0.2f);
        glUniform3f(g_state.loc_light_dir, 0.0f, 0.0f, 1.0f);
        mesh_draw(&g_state.sun_mesh);

        /* Planets: packed point sprites with atmosphere glow */
        {
            pp_planet_data_t pdata = pp_pack_planets(g_state.simulation_jd,
                                                      1.0f, 150.0f);
            sqrt_scale_planet_vertices(pdata.vertices, pdata.planet_count);

            mat4_t pp_vp = mat4_multiply(proj, view);
            glUseProgram(g_state.pp_program);
            glUniformMatrix4fv(g_state.pp_loc_mvp, 1, GL_FALSE, pp_vp.m);
            glUniform1f(g_state.pp_loc_scale, 800.0f);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBindVertexArray(g_state.pp_vao);
            glBindBuffer(GL_ARRAY_BUFFER, g_state.pp_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0,
                            (GLsizeiptr)pp_planet_vertex_bytes(&pdata),
                            pdata.vertices);
            glDrawArrays(GL_POINTS, 0, pdata.planet_count);

            glBindVertexArray(0);
            glDisable(GL_BLEND);
        }
    }

    /* --- Orbit trails (only when LAYER_ORBITS visible) --- */
    if (g_state.show_trails &&
        layer_is_visible(g_state.layer_state, LAYER_ORBITS)) {
        /* Pack trail data with sqrt distance scaling */
        pp_trail_data_t tdata = pp_pack_trails(g_state.simulation_jd,
                                                1.0f, 180);
        sqrt_scale_trail_vertices(tdata.vertices, tdata.total_vertex_count);

        mat4_t trail_vp = mat4_multiply(proj, view);
        glUseProgram(g_state.pp_trail_program);
        glUniformMatrix4fv(g_state.pp_trail_loc_mvp, 1, GL_FALSE, trail_vp.m);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBindVertexArray(g_state.pp_trail_vao);
        glBindBuffer(GL_ARRAY_BUFFER, g_state.pp_trail_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0,
                        (GLsizeiptr)pp_trail_vertex_bytes(&tdata),
                        tdata.vertices);

        /* Draw each planet's trail as a separate line strip */
        for (int i = 0; i < PP_PLANET_COUNT; i++) {
            glDrawArrays(GL_LINE_STRIP,
                         tdata.trail_offsets[i],
                         tdata.trail_counts[i]);
        }

        glBindVertexArray(0);
        glDisable(GL_BLEND);
    }

    /* --- Zodiac ring (only when LAYER_ZODIAC_RING visible) --- */
    if (layer_is_visible(g_state.layer_state, LAYER_ZODIAC_RING)) {
        glUseProgram(g_state.ring_program);
        glUniformMatrix4fv(g_state.ring_loc_view, 1, GL_FALSE, view.m);
        glUniformMatrix4fv(g_state.ring_loc_proj, 1, GL_FALSE, proj.m);

        float ring_opacity = g_state.layer_state.opacity[LAYER_ZODIAC_RING];
        glUniform1f(g_state.ring_loc_opacity, ring_opacity);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBindVertexArray(g_state.ring_vao);
        glDrawElements(GL_TRIANGLES, g_state.ring_index_count,
                        GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glDisable(GL_BLEND);
    }

    /* --- Planet markers + aspect lines on zodiac ring --- */
    if (layer_is_visible(g_state.layer_state, LAYER_ZODIAC_RING)) {
        float ring_opacity = g_state.layer_state.opacity[LAYER_ZODIAC_RING];

        /* Compute geocentric planet longitudes + ring positions */
        solar_system_t sys = planets_at(g_state.simulation_jd);
        double earth_lon = sys.positions[PLANET_EARTH].longitude;
        double sun_geo_lon = fmod(earth_lon + 180.0, 360.0);
        double geo_lons[8];
        float ring_pos[8 * 3];
        for (int p = 0; p < 8; p++) {
            double gl;
            if (p == PLANET_EARTH)
                gl = sun_geo_lon;
            else {
                gl = fmod(sys.positions[p].longitude - earth_lon + 180.0, 360.0);
                if (gl < 0.0) gl += 360.0;
            }
            geo_lons[p] = gl;
            float lon_rad = (float)(gl * DEG_TO_RAD);
            ring_pos[p * 3 + 0] = RING_MID_RADIUS * cosf(lon_rad);
            ring_pos[p * 3 + 1] = 0.0f;
            ring_pos[p * 3 + 2] = RING_MID_RADIUS * sinf(lon_rad);
        }

        /* Render planet markers as small spheres on the ring */
        glUseProgram(g_state.program);
        glUniformMatrix4fv(g_state.loc_view, 1, GL_FALSE, view.m);
        glUniformMatrix4fv(g_state.loc_proj, 1, GL_FALSE, proj.m);
        glUniform1f(g_state.loc_emissive, 0.7f);
        glUniform1f(g_state.loc_opacity, ring_opacity);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for (int p = 0; p < 8; p++) {
            float mx = ring_pos[p * 3 + 0];
            float my = ring_pos[p * 3 + 1];
            float mz = ring_pos[p * 3 + 2];

            mat4_t translate = mat4_translate(mx, my, mz);
            mat4_t scale = mat4_scale(0.12f, 0.12f, 0.12f);
            mat4_t model = mat4_multiply(translate, scale);

            glUniformMatrix4fv(g_state.loc_model, 1, GL_FALSE, model.m);

            /* Earth index = Sun in geocentric view — use Sun color */
            if (p == PLANET_EARTH) {
                glUniform3f(g_state.loc_color, 1.0f, 0.85f, 0.2f);
            } else {
                float pr, pg, pb;
                planet_pack_color(p, &pr, &pg, &pb);
                glUniform3f(g_state.loc_color, pr, pg, pb);
            }
            glUniform3f(g_state.loc_light_dir, 0.0f, 1.0f, 0.0f);

            mesh_draw(&g_state.planet_mesh);
        }

        glDisable(GL_BLEND);

        /* Render aspect lines as colored chords inside the ring */
        aspect_line_set_t alines = aspect_lines_generate(
            ring_pos, geo_lons, 8, 8.0);

        if (alines.count > 0) {
            float line_verts[MAX_ASPECT_LINES * 14];
            for (int i = 0; i < alines.count; i++) {
                aspect_line_t *al = &alines.lines[i];
                int base = i * 14;
                line_verts[base + 0]  = al->x1;
                line_verts[base + 1]  = al->y1;
                line_verts[base + 2]  = al->z1;
                line_verts[base + 3]  = al->r;
                line_verts[base + 4]  = al->g;
                line_verts[base + 5]  = al->b;
                line_verts[base + 6]  = al->opacity;
                line_verts[base + 7]  = al->x2;
                line_verts[base + 8]  = al->y2;
                line_verts[base + 9]  = al->z2;
                line_verts[base + 10] = al->r;
                line_verts[base + 11] = al->g;
                line_verts[base + 12] = al->b;
                line_verts[base + 13] = al->opacity;
            }

            glUseProgram(g_state.line_program);
            glUniformMatrix4fv(g_state.line_loc_view, 1, GL_FALSE, view.m);
            glUniformMatrix4fv(g_state.line_loc_proj, 1, GL_FALSE, proj.m);
            glUniform1f(g_state.line_loc_opacity, ring_opacity);

            glBindVertexArray(g_state.line_vao);
            glBindBuffer(GL_ARRAY_BUFFER, g_state.line_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0,
                            (GLsizeiptr)(alines.count * 14 * (int)sizeof(float)),
                            line_verts);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDrawArrays(GL_LINES, 0, alines.count * 2);
            glDisable(GL_BLEND);

            glBindVertexArray(0);
        }

        /* Render house cusp lines as radial lines across the ring */
        {
            double jd_0h = floor(g_state.simulation_jd - 0.5) + 0.5;
            double gmst = gmst_degrees(jd_0h);
            double lst = lst_degrees(gmst, g_state.observer_lon);
            double obliquity = mean_obliquity(g_state.simulation_jd);
            double asc = ascendant_longitude(lst, obliquity, g_state.observer_lat);
            house_system_t houses = houses_whole_sign(asc);

            cusp_line_set_t cusps = cusp_lines_generate(
                houses.cusps, RING_INNER_RADIUS, RING_OUTER_RADIUS);

            if (cusps.count > 0) {
                float cusp_verts[MAX_CUSP_LINES * 14];
                for (int i = 0; i < cusps.count; i++) {
                    cusp_line_t *cl = &cusps.lines[i];
                    int base = i * 14;
                    float alpha = cl->is_angular ? 0.8f : 0.35f;
                    float gray = cl->is_angular ? 1.0f : 0.6f;
                    cusp_verts[base + 0]  = cl->x1;
                    cusp_verts[base + 1]  = cl->y1;
                    cusp_verts[base + 2]  = cl->z1;
                    cusp_verts[base + 3]  = gray;
                    cusp_verts[base + 4]  = gray;
                    cusp_verts[base + 5]  = gray;
                    cusp_verts[base + 6]  = alpha;
                    cusp_verts[base + 7]  = cl->x2;
                    cusp_verts[base + 8]  = cl->y2;
                    cusp_verts[base + 9]  = cl->z2;
                    cusp_verts[base + 10] = gray;
                    cusp_verts[base + 11] = gray;
                    cusp_verts[base + 12] = gray;
                    cusp_verts[base + 13] = alpha;
                }

                glUseProgram(g_state.line_program);
                glUniformMatrix4fv(g_state.line_loc_view, 1, GL_FALSE, view.m);
                glUniformMatrix4fv(g_state.line_loc_proj, 1, GL_FALSE, proj.m);
                glUniform1f(g_state.line_loc_opacity, ring_opacity);

                glBindVertexArray(g_state.line_vao);
                glBindBuffer(GL_ARRAY_BUFFER, g_state.line_vbo);
                glBufferSubData(GL_ARRAY_BUFFER, 0,
                                (GLsizeiptr)(cusps.count * 14 * (int)sizeof(float)),
                                cusp_verts);

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glDrawArrays(GL_LINES, 0, cusps.count * 2);
                glDisable(GL_BLEND);

                glBindVertexArray(0);
            }
        }

        /* Render zodiac sign labels as billboarded glyph quads */
        {
            vec3_t cam_right, cam_up;
            billboard_camera_vectors(&view, &cam_right, &cam_up);

            /* Glyph aspect ratio: 8:12 = 2:3 */
            float gw = SIGN_LABEL_SIZE;
            float gh = gw * 1.5f;

            float glyph_verts[12 * 4 * 8]; /* 12 signs × 4 verts × 8 floats */
            for (int s = 0; s < 12; s++) {
                float angle_deg = (float)(s * 30 + 15);
                float angle_rad = angle_deg * (float)DEG_TO_RAD;
                vec3_t center = {
                    RING_MID_RADIUS * cosf(angle_rad),
                    SIGN_LABEL_HEIGHT,
                    RING_MID_RADIUS * sinf(angle_rad)
                };

                billboard_quad_t quad = billboard_create(
                    center, cam_right, cam_up, gw, gh);

                /* UV region for sign s in atlas: u=[s/12, (s+1)/12], v=[0,1] */
                float u0 = (float)s / 12.0f;
                float u1 = (float)(s + 1) / 12.0f;

                /* Sign color from palette */
                color_rgb_t sc = color_zodiac_sign(s);

                /* 4 vertices: billboard positions + remapped UVs + color */
                int base = s * 32; /* 4 verts × 8 floats */
                for (int v = 0; v < 4; v++) {
                    int vi = base + v * 8;
                    glyph_verts[vi + 0] = quad.positions[v * 3 + 0];
                    glyph_verts[vi + 1] = quad.positions[v * 3 + 1];
                    glyph_verts[vi + 2] = quad.positions[v * 3 + 2];
                    /* Remap UV: billboard gives (0,0),(1,0),(1,1),(0,1) */
                    float bu = quad.uvs[v * 2 + 0];
                    float bv = quad.uvs[v * 2 + 1];
                    glyph_verts[vi + 3] = u0 + bu * (u1 - u0);
                    glyph_verts[vi + 4] = bv;
                    glyph_verts[vi + 5] = sc.r;
                    glyph_verts[vi + 6] = sc.g;
                    glyph_verts[vi + 7] = sc.b;
                }
            }

            glUseProgram(g_state.glyph_program);
            glUniformMatrix4fv(g_state.glyph_loc_view, 1, GL_FALSE, view.m);
            glUniformMatrix4fv(g_state.glyph_loc_proj, 1, GL_FALSE, proj.m);
            glUniform1f(g_state.glyph_loc_opacity, ring_opacity);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, g_state.glyph_texture);

            glBindVertexArray(g_state.glyph_vao);
            glBindBuffer(GL_ARRAY_BUFFER, g_state.glyph_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0,
                            (GLsizeiptr)(12 * 32 * (int)sizeof(float)),
                            glyph_verts);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDisable(GL_DEPTH_TEST);
            glDrawElements(GL_TRIANGLES, 72, GL_UNSIGNED_INT, 0);
            glEnable(GL_DEPTH_TEST);
            glDisable(GL_BLEND);

            glBindVertexArray(0);
        }
    }

    /* --- Time HUD overlay (only when LAYER_HUD visible) --- */
    if (g_state.show_hud &&
        layer_is_visible(g_state.layer_state, LAYER_HUD))
        update_hud(g_state.simulation_jd, g_state.time_speed,
                   g_state.observer_lat, g_state.observer_lon,
                   g_state.camera.log_zoom);
#endif
}

int main(void) {
    printf("Time - A Visual Study\n");

#ifdef __EMSCRIPTEN__
    if (gl_init() != 0) {
        printf("Failed to initialize WebGL\n");
        return 1;
    }

    /* Create state */
    double css_w, css_h;
    emscripten_get_element_css_size("#canvas", &css_w, &css_h);
    g_state = app_state_create((float)(css_w / css_h));

    /* Create shader program */
    g_state.program = shader_create_program(vert_source, frag_source);
    if (g_state.program == 0) {
        printf("Failed to create shader program\n");
        return 1;
    }

    /* Cache uniform locations */
    g_state.loc_model = glGetUniformLocation(g_state.program, "u_model");
    g_state.loc_view = glGetUniformLocation(g_state.program, "u_view");
    g_state.loc_proj = glGetUniformLocation(g_state.program, "u_proj");
    g_state.loc_color = glGetUniformLocation(g_state.program, "u_color");
    g_state.loc_light_dir = glGetUniformLocation(g_state.program, "u_light_dir");
    g_state.loc_emissive = glGetUniformLocation(g_state.program, "u_emissive");
    g_state.loc_opacity = glGetUniformLocation(g_state.program, "u_opacity");

    /* Create meshes: Sun sphere + small sphere for zodiac ring markers */
    g_state.sun_mesh = mesh_create_sphere(16, 32);
    g_state.planet_mesh = mesh_create_sphere(12, 24);

    /* --- Planet pack shader setup (point sprites with atmosphere glow) --- */
    g_state.pp_program = shader_create_program(
        pp_planet_vert_source(), pp_planet_frag_source());
    if (g_state.pp_program == 0) {
        printf("Failed to create planet pack shader\n");
        return 1;
    }
    g_state.pp_loc_mvp = glGetUniformLocation(g_state.pp_program, "u_mvp");
    g_state.pp_loc_scale = glGetUniformLocation(g_state.pp_program, "u_scale_factor");

    /* Pre-allocate planet VBO: 8 planets × 8 floats */
    glGenVertexArrays(1, &g_state.pp_vao);
    glBindVertexArray(g_state.pp_vao);

    glGenBuffers(1, &g_state.pp_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, g_state.pp_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(PP_PLANET_COUNT * PP_PLANET_VERTEX_FLOATS * (int)sizeof(float)),
                 NULL, GL_DYNAMIC_DRAW);

    /* Interleaved: pos(3) + color(3) + radius(1) + atmo(1) = 8 floats = 32 bytes */
    glEnableVertexAttribArray(0); /* a_position */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, (void*)0);
    glEnableVertexAttribArray(1); /* a_color */
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 32, (void*)12);
    glEnableVertexAttribArray(2); /* a_radius */
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 32, (void*)24);
    glEnableVertexAttribArray(3); /* a_atmo */
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 32, (void*)28);

    glBindVertexArray(0);

    /* --- Planet pack trail shader setup (alpha-fading line strips) --- */
    g_state.pp_trail_program = shader_create_program(
        pp_trail_vert_source(), pp_trail_frag_source());
    if (g_state.pp_trail_program == 0) {
        printf("Failed to create planet trail shader\n");
        return 1;
    }
    g_state.pp_trail_loc_mvp = glGetUniformLocation(g_state.pp_trail_program, "u_mvp");

    /* Pre-allocate trail VBO: 8 planets × 180 samples × 7 floats */
    glGenVertexArrays(1, &g_state.pp_trail_vao);
    glBindVertexArray(g_state.pp_trail_vao);

    glGenBuffers(1, &g_state.pp_trail_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, g_state.pp_trail_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(PP_PLANET_COUNT * 180 * PP_TRAIL_VERTEX_FLOATS * (int)sizeof(float)),
                 NULL, GL_DYNAMIC_DRAW);

    /* Interleaved: pos(3) + color(4) = 7 floats = 28 bytes */
    glEnableVertexAttribArray(0); /* a_position */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 28, (void*)0);
    glEnableVertexAttribArray(1); /* a_color */
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 28, (void*)12);

    glBindVertexArray(0);

    printf("Planet pack: 8 planets + trails, shaders compiled\n");

    /* --- Zodiac ring setup --- */
    g_state.ring_program = shader_create_program(ring_vert_source, ring_frag_source);
    if (g_state.ring_program == 0) {
        printf("Failed to create ring shader\n");
        return 1;
    }
    g_state.ring_loc_view = glGetUniformLocation(g_state.ring_program, "u_view");
    g_state.ring_loc_proj = glGetUniformLocation(g_state.ring_program, "u_proj");
    g_state.ring_loc_opacity = glGetUniformLocation(g_state.ring_program, "u_opacity");

    /* Generate ring mesh data on stack */
    {
        ring_mesh_info_t info = ring_mesh_size(RING_SEGMENTS_PER_SIGN);
        float positions[info.vertex_count * 3];
        float colors[info.vertex_count * 3];
        unsigned int indices[info.index_count];

        ring_generate_positions(positions, RING_INNER_RADIUS, RING_OUTER_RADIUS,
                                RING_SEGMENTS_PER_SIGN);
        ring_generate_indices(indices, RING_SEGMENTS_PER_SIGN);

        /* Color each vertex by its zodiac sign */
        for (int v = 0; v < info.vertex_count; v++) {
            int sign = ring_vertex_sign(v, RING_SEGMENTS_PER_SIGN);
            color_rgb_t c = color_zodiac_sign(sign);
            colors[v * 3 + 0] = c.r;
            colors[v * 3 + 1] = c.g;
            colors[v * 3 + 2] = c.b;
        }

        g_state.ring_index_count = info.index_count;

        /* Upload to GPU */
        glGenVertexArrays(1, &g_state.ring_vao);
        glBindVertexArray(g_state.ring_vao);

        glGenBuffers(1, &g_state.ring_vbo_pos);
        glBindBuffer(GL_ARRAY_BUFFER, g_state.ring_vbo_pos);
        glBufferData(GL_ARRAY_BUFFER,
                     (GLsizeiptr)(info.vertex_count * 3 * (int)sizeof(float)),
                     positions, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glGenBuffers(1, &g_state.ring_vbo_color);
        glBindBuffer(GL_ARRAY_BUFFER, g_state.ring_vbo_color);
        glBufferData(GL_ARRAY_BUFFER,
                     (GLsizeiptr)(info.vertex_count * 3 * (int)sizeof(float)),
                     colors, GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glGenBuffers(1, &g_state.ring_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_state.ring_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     (GLsizeiptr)(info.index_count * (int)sizeof(unsigned int)),
                     indices, GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    /* --- Aspect line shader setup --- */
    g_state.line_program = shader_create_program(line_vert_source, line_frag_source);
    if (g_state.line_program == 0) {
        printf("Failed to create line shader\n");
        return 1;
    }
    g_state.line_loc_view = glGetUniformLocation(g_state.line_program, "u_view");
    g_state.line_loc_proj = glGetUniformLocation(g_state.line_program, "u_proj");
    g_state.line_loc_opacity = glGetUniformLocation(g_state.line_program, "u_opacity");

    /* Pre-allocate line VBO (dynamic, updated each frame) */
    glGenVertexArrays(1, &g_state.line_vao);
    glBindVertexArray(g_state.line_vao);

    glGenBuffers(1, &g_state.line_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, g_state.line_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(MAX_ASPECT_LINES * 14 * (int)sizeof(float)),
                 NULL, GL_DYNAMIC_DRAW);

    /* Interleaved: pos(3) + color(4) = 7 floats = 28 bytes per vertex */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 28, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 28, (void*)12);

    glBindVertexArray(0);

    /* --- Zodiac glyph texture + shader setup --- */
    g_state.glyph_program = shader_create_program(glyph_vert_source, glyph_frag_source);
    if (g_state.glyph_program == 0) {
        printf("Failed to create glyph shader\n");
        return 1;
    }
    g_state.glyph_loc_view = glGetUniformLocation(g_state.glyph_program, "u_view");
    g_state.glyph_loc_proj = glGetUniformLocation(g_state.glyph_program, "u_proj");
    g_state.glyph_loc_opacity = glGetUniformLocation(g_state.glyph_program, "u_opacity");

    /* Pack 12 zodiac bitmaps into a 96×12 R8 texture atlas */
    {
        unsigned char atlas[GLYPH_ATLAS_W * GLYPH_ATLAS_H];
        for (int i = 0; i < GLYPH_ATLAS_W * GLYPH_ATLAS_H; i++)
            atlas[i] = 0;
        for (int s = 0; s < 12; s++) {
            const unsigned char *glyph = zodiac_glyph(s);
            if (!glyph) continue;
            for (int y = 0; y < GLYPH_HEIGHT; y++) {
                for (int x = 0; x < GLYPH_WIDTH; x++) {
                    atlas[y * GLYPH_ATLAS_W + s * GLYPH_WIDTH + x] =
                        glyph[y * GLYPH_WIDTH + x] * 255;
                }
            }
        }
        glGenTextures(1, &g_state.glyph_texture);
        glBindTexture(GL_TEXTURE_2D, g_state.glyph_texture);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, GLYPH_ATLAS_W, GLYPH_ATLAS_H,
                     0, GL_RED, GL_UNSIGNED_BYTE, atlas);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    /* Pre-allocate glyph billboard VBO: 12 signs × 4 verts × 8 floats = 384 */
    glGenVertexArrays(1, &g_state.glyph_vao);
    glBindVertexArray(g_state.glyph_vao);

    glGenBuffers(1, &g_state.glyph_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, g_state.glyph_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(12 * 4 * 8 * (int)sizeof(float)),
                 NULL, GL_DYNAMIC_DRAW);

    /* Interleaved: pos(3) + uv(2) + color(3) = 8 floats = 32 bytes per vertex */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 32, (void*)12);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 32, (void*)20);

    /* Static index buffer: 12 signs × 6 indices (2 triangles each) */
    {
        unsigned int glyph_indices[72];
        for (int s = 0; s < 12; s++) {
            int base_v = s * 4;
            int base_i = s * 6;
            glyph_indices[base_i + 0] = (unsigned int)base_v + 0;
            glyph_indices[base_i + 1] = (unsigned int)base_v + 1;
            glyph_indices[base_i + 2] = (unsigned int)base_v + 2;
            glyph_indices[base_i + 3] = (unsigned int)base_v + 0;
            glyph_indices[base_i + 4] = (unsigned int)base_v + 2;
            glyph_indices[base_i + 5] = (unsigned int)base_v + 3;
        }
        glGenBuffers(1, &g_state.glyph_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_state.glyph_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     (GLsizeiptr)(72 * (int)sizeof(unsigned int)),
                     glyph_indices, GL_STATIC_DRAW);
    }

    glBindVertexArray(0);

    /* --- Star field setup --- */
    {
        /* Compile star shaders from star_field module */
        g_state.star_program = shader_create_program(
            star_field_vert_source(), star_field_frag_source());
        if (g_state.star_program == 0) {
            printf("Failed to create star shader\n");
            return 1;
        }
        g_state.star_loc_mvp = glGetUniformLocation(g_state.star_program, "u_mvp");
        g_state.star_loc_scale = glGetUniformLocation(g_state.star_program, "u_scale_factor");

        /* Pack star vertex data on stack */
        float star_verts[600 * STAR_VERTEX_FLOATS]; /* ~595 stars max */
        g_state.star_count = star_field_pack(star_verts, 600, 8.0f, 100.0f);

        /* Upload to GPU */
        glGenVertexArrays(1, &g_state.star_vao);
        glBindVertexArray(g_state.star_vao);

        glGenBuffers(1, &g_state.star_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, g_state.star_vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     (GLsizeiptr)(g_state.star_count * STAR_VERTEX_STRIDE),
                     star_verts, GL_STATIC_DRAW);

        /* Interleaved: pos(3) + color(3) + size(1) = 7 floats = 28 bytes */
        glEnableVertexAttribArray(0); /* a_position */
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STAR_VERTEX_STRIDE, (void*)0);
        glEnableVertexAttribArray(1); /* a_color */
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, STAR_VERTEX_STRIDE, (void*)12);
        glEnableVertexAttribArray(2); /* a_size */
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, STAR_VERTEX_STRIDE, (void*)24);

        glBindVertexArray(0);

        printf("Stars: %d packed, shaders compiled\n", g_state.star_count);
    }

    /* --- Constellation line setup --- */
    {
        g_state.cline_program = shader_create_program(
            constellation_line_vert_source(), constellation_line_frag_source());
        if (g_state.cline_program == 0) {
            printf("Failed to create constellation shader\n");
            return 1;
        }
        g_state.cline_loc_mvp = glGetUniformLocation(g_state.cline_program, "u_mvp");

        /* Pack constellation line vertices on stack */
        float cline_verts[500 * 2 * CLINE_VERTEX_FLOATS]; /* ~300 lines max */
        int line_count = constellation_lines_pack(cline_verts, 500, 0.25f, 0.4f, 100.0f);
        g_state.cline_vertex_count = line_count * 2;

        /* Upload to GPU */
        glGenVertexArrays(1, &g_state.cline_vao);
        glBindVertexArray(g_state.cline_vao);

        glGenBuffers(1, &g_state.cline_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, g_state.cline_vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     (GLsizeiptr)(g_state.cline_vertex_count * CLINE_VERTEX_STRIDE),
                     cline_verts, GL_STATIC_DRAW);

        /* Interleaved: pos(3) + color(4) = 7 floats = 28 bytes */
        glEnableVertexAttribArray(0); /* a_position */
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, CLINE_VERTEX_STRIDE, (void*)0);
        glEnableVertexAttribArray(1); /* a_color */
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, CLINE_VERTEX_STRIDE, (void*)12);

        glBindVertexArray(0);

        printf("Constellations: %d lines packed, shaders compiled\n", line_count);
    }

    /* Initialize timing */
    g_state.prev_time_ms = emscripten_get_now();

    /* Register input handlers */
    emscripten_set_mousedown_callback("#canvas", NULL, EM_TRUE, on_mouse_down);
    emscripten_set_mouseup_callback("#canvas", NULL, EM_TRUE, on_mouse_up);
    emscripten_set_mousemove_callback("#canvas", NULL, EM_TRUE, on_mouse_move);
    emscripten_set_wheel_callback("#canvas", NULL, EM_TRUE, on_wheel);
    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, NULL, EM_TRUE, on_key_down);

    printf("Controls: Space=pause, 1-5=speed, -=reverse, T=trails, H=hud, Shift+0-6=scale\n");

    emscripten_set_main_loop(main_loop, 0, 1);
#endif

    return 0;
}
