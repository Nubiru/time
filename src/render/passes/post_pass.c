/*
 * post_pass.c — Post-processing FBO pipeline render pass
 *
 * Stateful module (S1): owns GPU resource handles as module-static s_ variables.
 * Manages scene FBO, bloom extraction, Gaussian blur ping-pong, and final
 * composite with tone mapping, vignette, and optional film grain.
 */

#ifdef __EMSCRIPTEN__

#include "post_pass.h"
#include <stdio.h>
#include <GLES3/gl3.h>
#include "../shader.h"
#include "../post_process.h"
#include "../depth_visual.h"

/* --- Module-static GL handles --- */

/* Fullscreen quad VAO (shared by all post-process shader passes) */
static GLuint s_pp_quad_vao;
static GLuint s_pp_quad_vbo;

/* Scene FBO (all passes render here instead of default framebuffer) */
static GLuint s_pp_scene_fbo;
static GLuint s_pp_scene_tex;
static GLuint s_pp_scene_depth;

/* Brightness extraction FBO */
static GLuint s_pp_bright_fbo;
static GLuint s_pp_bright_tex;

/* Blur ping-pong FBOs */
static GLuint s_pp_ping_fbo;
static GLuint s_pp_ping_tex;
static GLuint s_pp_pong_fbo;
static GLuint s_pp_pong_tex;

/* God ray FBO (half resolution) */
static GLuint s_pp_godrays_fbo;
static GLuint s_pp_godrays_tex;

/* Shader programs */
static GLuint s_pp_bright_program;
static GLint  s_pp_bright_loc_scene;
static GLint  s_pp_bright_loc_threshold;

static GLuint s_pp_blur_program;
static GLint  s_pp_blur_loc_image;
static GLint  s_pp_blur_loc_horizontal;

static GLuint s_pp_composite_program;
static GLint  s_pp_comp_loc_scene;
static GLint  s_pp_comp_loc_bloom;
static GLint  s_pp_comp_loc_bloom_intensity;
static GLint  s_pp_comp_loc_exposure;
static GLint  s_pp_comp_loc_gamma;
static GLint  s_pp_comp_loc_vignette_strength;
static GLint  s_pp_comp_loc_vignette_radius;
static GLint  s_pp_comp_loc_grain_intensity;
static GLint  s_pp_comp_loc_time;
static GLint  s_pp_comp_loc_depth;
static GLint  s_pp_comp_loc_dof_focus;
static GLint  s_pp_comp_loc_dof_strength;
static GLint  s_pp_comp_loc_tint;

static GLuint s_pp_godrays_program;
static GLint  s_pp_gr_loc_frame;
static GLint  s_pp_gr_loc_sun_pos;
static GLint  s_pp_gr_loc_density;
static GLint  s_pp_gr_loc_weight;
static GLint  s_pp_gr_loc_decay;
static GLint  s_pp_gr_loc_exposure;

/* Current config */
static pp_config_t s_pp_config;
static int s_pp_width;
static int s_pp_height;
static int s_pp_active; /* 0 = passthrough (FBO failed), 1 = full pipeline */

/* --- Static scratch buffer (BSS, zero stack cost) --- */
static float s_pp_quad_data[PP_QUAD_VERTEX_COUNT * PP_QUAD_VERTEX_FLOATS];

/* --- Internal helpers --- */

static GLuint create_color_texture(int w, int h) {
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0,
                 GL_RGBA, GL_HALF_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return tex;
}

static void create_fbo(GLuint *fbo, GLuint *tex, int w, int h) {
    *tex = create_color_texture(w, h);
    glGenFramebuffers(1, fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, *tex, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

static void draw_quad(void) {
    glBindVertexArray(s_pp_quad_vao);
    glDrawArrays(GL_TRIANGLES, 0, PP_QUAD_VERTEX_COUNT);
    glBindVertexArray(0);
}

/* --- Public API --- */

int post_pass_init(int width, int height) {
    s_pp_config = pp_default_config();
    s_pp_width = width;
    s_pp_height = height;
    s_pp_active = 0; /* assume passthrough until FBOs confirmed working */

    /* Setup fullscreen quad */
    pp_pack_quad(s_pp_quad_data);

    glGenVertexArrays(1, &s_pp_quad_vao);
    glBindVertexArray(s_pp_quad_vao);

    glGenBuffers(1, &s_pp_quad_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, s_pp_quad_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(PP_QUAD_VERTEX_COUNT * PP_QUAD_VERTEX_FLOATS * (int)sizeof(float)),
                 s_pp_quad_data, GL_STATIC_DRAW);

    /* position(vec2) at loc 0, texcoord(vec2) at loc 1 */
    int stride = PP_QUAD_VERTEX_FLOATS * (int)sizeof(float);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)8);

    glBindVertexArray(0);

    /* Scene FBO (with depth texture — sampleable for DOF) */
    s_pp_scene_tex = create_color_texture(width, height);

    /* Depth as texture (not renderbuffer) so composite shader can sample it */
    glGenTextures(1, &s_pp_scene_depth);
    glBindTexture(GL_TEXTURE_2D, s_pp_scene_depth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0,
                 GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenFramebuffers(1, &s_pp_scene_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, s_pp_scene_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, s_pp_scene_tex, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D, s_pp_scene_depth, 0);

    GLenum fbo_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (fbo_status != GL_FRAMEBUFFER_COMPLETE) {
        printf("Scene FBO incomplete: 0x%x — falling back to direct rendering\n",
               (unsigned)fbo_status);
        return 0; /* not a fatal error — passthrough mode */
    }

    /* Bloom FBOs (half resolution for performance) */
    int bw = width / 2, bh = height / 2;
    if (bw < 1) bw = 1;
    if (bh < 1) bh = 1;
    create_fbo(&s_pp_bright_fbo, &s_pp_bright_tex, bw, bh);
    create_fbo(&s_pp_ping_fbo, &s_pp_ping_tex, bw, bh);
    create_fbo(&s_pp_pong_fbo, &s_pp_pong_tex, bw, bh);
    create_fbo(&s_pp_godrays_fbo, &s_pp_godrays_tex, bw, bh);

    /* Compile shaders */
    const char *quad_vert = pp_quad_vert_source();

    s_pp_bright_program = shader_create_program(
        quad_vert, pp_bright_extract_frag_source());
    if (s_pp_bright_program == 0) {
        printf("Bloom shader failed — falling back to direct rendering\n");
        return 0;
    }
    s_pp_bright_loc_scene = glGetUniformLocation(s_pp_bright_program, "u_scene");
    s_pp_bright_loc_threshold = glGetUniformLocation(s_pp_bright_program, "u_threshold");

    s_pp_blur_program = shader_create_program(
        quad_vert, pp_blur_frag_source());
    if (s_pp_blur_program == 0) {
        printf("Blur shader failed — falling back to direct rendering\n");
        return 0;
    }
    s_pp_blur_loc_image = glGetUniformLocation(s_pp_blur_program, "u_image");
    s_pp_blur_loc_horizontal = glGetUniformLocation(s_pp_blur_program, "u_horizontal");

    s_pp_composite_program = shader_create_program(
        quad_vert, pp_composite_frag_source());
    if (s_pp_composite_program == 0) {
        printf("Composite shader failed — falling back to direct rendering\n");
        return 0;
    }
    s_pp_comp_loc_scene = glGetUniformLocation(s_pp_composite_program, "u_scene");
    s_pp_comp_loc_bloom = glGetUniformLocation(s_pp_composite_program, "u_bloom");
    s_pp_comp_loc_bloom_intensity = glGetUniformLocation(s_pp_composite_program, "u_bloom_intensity");
    s_pp_comp_loc_exposure = glGetUniformLocation(s_pp_composite_program, "u_exposure");
    s_pp_comp_loc_gamma = glGetUniformLocation(s_pp_composite_program, "u_gamma");
    s_pp_comp_loc_vignette_strength = glGetUniformLocation(s_pp_composite_program, "u_vignette_strength");
    s_pp_comp_loc_vignette_radius = glGetUniformLocation(s_pp_composite_program, "u_vignette_radius");
    s_pp_comp_loc_grain_intensity = glGetUniformLocation(s_pp_composite_program, "u_grain_intensity");
    s_pp_comp_loc_time = glGetUniformLocation(s_pp_composite_program, "u_time");
    s_pp_comp_loc_depth = glGetUniformLocation(s_pp_composite_program, "u_depth");
    s_pp_comp_loc_dof_focus = glGetUniformLocation(s_pp_composite_program, "u_dof_focus");
    s_pp_comp_loc_dof_strength = glGetUniformLocation(s_pp_composite_program, "u_dof_strength");
    s_pp_comp_loc_tint = glGetUniformLocation(s_pp_composite_program, "u_tint");

    /* God ray radial blur shader (GPU Gems 3, Ch 13) */
    s_pp_godrays_program = shader_create_program(
        quad_vert, pp_godrays_frag_source());
    if (s_pp_godrays_program == 0) {
        printf("God rays shader failed — disabling god rays\n");
        s_pp_config.godrays_enabled = 0;
    } else {
        s_pp_gr_loc_frame    = glGetUniformLocation(s_pp_godrays_program, "u_frame");
        s_pp_gr_loc_sun_pos  = glGetUniformLocation(s_pp_godrays_program, "u_sun_screen_pos");
        s_pp_gr_loc_density  = glGetUniformLocation(s_pp_godrays_program, "u_density");
        s_pp_gr_loc_weight   = glGetUniformLocation(s_pp_godrays_program, "u_weight");
        s_pp_gr_loc_decay    = glGetUniformLocation(s_pp_godrays_program, "u_decay");
        s_pp_gr_loc_exposure = glGetUniformLocation(s_pp_godrays_program, "u_exposure");
    }

    s_pp_active = 1;
    printf("Post-process: bloom + tonemap + vignette + godrays, %dx%d\n",
           width, height);
    return 0;
}

void post_pass_begin(const render_frame_t *frame) {
    (void)frame;
    /* Cosmos space-black — hardcoded per visual design Principle 1.
     * Theme-dependent clear color caused Dawn flash bug. */
    glClearColor(0.024f, 0.027f, 0.036f, 1.0f);

    if (s_pp_active) {
        glBindFramebuffer(GL_FRAMEBUFFER, s_pp_scene_fbo);
    }
    glViewport(0, 0, s_pp_width, s_pp_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void post_pass_end(const render_frame_t *frame) {
    if (!s_pp_active) {
        /* Passthrough mode — passes already rendered to default framebuffer */
        (void)frame;
        return;
    }

    int bw = s_pp_width / 2, bh = s_pp_height / 2;
    if (bw < 1) bw = 1;
    if (bh < 1) bh = 1;

    glDisable(GL_DEPTH_TEST);

    /* Step 1: Extract bright pixels */
    if (s_pp_config.bloom_enabled) {
        glBindFramebuffer(GL_FRAMEBUFFER, s_pp_bright_fbo);
        glViewport(0, 0, bw, bh);

        glUseProgram(s_pp_bright_program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, s_pp_scene_tex);
        glUniform1i(s_pp_bright_loc_scene, 0);
        glUniform1f(s_pp_bright_loc_threshold, s_pp_config.bloom_threshold);
        draw_quad();

        /* Step 2: Gaussian blur ping-pong */
        glUseProgram(s_pp_blur_program);
        glUniform1i(s_pp_blur_loc_image, 0);

        for (int i = 0; i < s_pp_config.bloom_passes; i++) {
            /* Horizontal pass: bright/pong -> ping */
            glBindFramebuffer(GL_FRAMEBUFFER, s_pp_ping_fbo);
            glBindTexture(GL_TEXTURE_2D,
                          (i == 0) ? s_pp_bright_tex : s_pp_pong_tex);
            glUniform1i(s_pp_blur_loc_horizontal, 1);
            draw_quad();

            /* Vertical pass: ping -> pong */
            glBindFramebuffer(GL_FRAMEBUFFER, s_pp_pong_fbo);
            glBindTexture(GL_TEXTURE_2D, s_pp_ping_tex);
            glUniform1i(s_pp_blur_loc_horizontal, 0);
            draw_quad();
        }
    }

    /* Step 3: God rays — radial blur from Sun screen position */
    if (s_pp_config.godrays_enabled && s_pp_godrays_program != 0) {
        glBindFramebuffer(GL_FRAMEBUFFER, s_pp_godrays_fbo);
        glViewport(0, 0, bw, bh);

        glUseProgram(s_pp_godrays_program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, s_pp_bright_tex);
        glUniform1i(s_pp_gr_loc_frame, 0);

        /* Sun at world origin (0,0,0). Project: clip = VP * (0,0,0,1) = col 3. */
        float sun_w = frame->view_proj.m[15];
        float sun_uv_x = 0.5f, sun_uv_y = 0.5f;
        if (sun_w > 0.001f) {
            sun_uv_x = (frame->view_proj.m[12] / sun_w) * 0.5f + 0.5f;
            sun_uv_y = (frame->view_proj.m[13] / sun_w) * 0.5f + 0.5f;
        }
        glUniform2f(s_pp_gr_loc_sun_pos, sun_uv_x, sun_uv_y);
        glUniform1f(s_pp_gr_loc_density, s_pp_config.godrays_density);
        glUniform1f(s_pp_gr_loc_weight, s_pp_config.godrays_weight);
        glUniform1f(s_pp_gr_loc_decay, s_pp_config.godrays_decay);
        glUniform1f(s_pp_gr_loc_exposure, s_pp_config.godrays_exposure);
        draw_quad();
    }

    /* Step 4: Composite to default framebuffer */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, s_pp_width, s_pp_height);

    glUseProgram(s_pp_composite_program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, s_pp_scene_tex);
    glUniform1i(s_pp_comp_loc_scene, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, s_pp_pong_tex);
    glUniform1i(s_pp_comp_loc_bloom, 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, s_pp_scene_depth);
    glUniform1i(s_pp_comp_loc_depth, 2);

    glUniform1f(s_pp_comp_loc_dof_focus,
                s_pp_config.dof_enabled ? s_pp_config.dof_focus_depth : 0.0f);
    glUniform1f(s_pp_comp_loc_dof_strength,
                s_pp_config.dof_enabled ? s_pp_config.dof_strength : 0.0f);

    glUniform1f(s_pp_comp_loc_bloom_intensity,
                s_pp_config.bloom_enabled ? s_pp_config.bloom_intensity : 0.0f);
    glUniform1f(s_pp_comp_loc_exposure, s_pp_config.exposure);
    glUniform1f(s_pp_comp_loc_gamma, s_pp_config.gamma);
    glUniform1f(s_pp_comp_loc_vignette_strength,
                s_pp_config.vignette_enabled ? s_pp_config.vignette_strength : 0.0f);
    glUniform1f(s_pp_comp_loc_vignette_radius, s_pp_config.vignette_radius);
    glUniform1f(s_pp_comp_loc_grain_intensity,
                s_pp_config.grain_enabled ? s_pp_config.grain_intensity : 0.0f);
    glUniform1f(s_pp_comp_loc_time, frame->time_sec);

    /* Depth tier background tint — subtle color wash per navigation layer */
    {
        dv_tier_t tier = dv_tier_from_zoom(frame->log_zoom);
        dv_tint_t tint = dv_tier_tint(tier);
        float blend = dv_tier_blend(frame->log_zoom, tier);
        glUniform4f(s_pp_comp_loc_tint,
                    tint.r, tint.g, tint.b, tint.a * blend);
    }

    draw_quad();

    /* Step 5: Additive god ray overlay */
    if (s_pp_config.godrays_enabled && s_pp_godrays_program != 0) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE); /* additive */

        glUseProgram(s_pp_godrays_program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, s_pp_godrays_tex);
        glUniform1i(s_pp_gr_loc_frame, 0);
        glUniform2f(s_pp_gr_loc_sun_pos, 0.5f, 0.5f);
        glUniform1f(s_pp_gr_loc_density, 0.0f);
        glUniform1f(s_pp_gr_loc_weight, 0.0f);
        glUniform1f(s_pp_gr_loc_decay, 1.0f);
        glUniform1f(s_pp_gr_loc_exposure, 1.0f);
        draw_quad();

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_BLEND);
    }

    glEnable(GL_DEPTH_TEST);
}

void post_pass_resize(int width, int height) {
    s_pp_width = width;
    s_pp_height = height;

    /* Resize scene FBO (color + depth textures) */
    glBindTexture(GL_TEXTURE_2D, s_pp_scene_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0,
                 GL_RGBA, GL_HALF_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, s_pp_scene_depth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0,
                 GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);

    /* Resize bloom FBOs (half res) */
    int bw = width / 2, bh = height / 2;
    if (bw < 1) bw = 1;
    if (bh < 1) bh = 1;

    GLuint textures[] = { s_pp_bright_tex, s_pp_ping_tex, s_pp_pong_tex, s_pp_godrays_tex };
    for (int i = 0; i < 4; i++) {
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, bw, bh, 0,
                     GL_RGBA, GL_HALF_FLOAT, NULL);
    }
}

void post_pass_destroy(void) {
    glDeleteProgram(s_pp_bright_program);
    glDeleteProgram(s_pp_blur_program);
    glDeleteProgram(s_pp_composite_program);
    if (s_pp_godrays_program) glDeleteProgram(s_pp_godrays_program);

    glDeleteFramebuffers(1, &s_pp_scene_fbo);
    glDeleteTextures(1, &s_pp_scene_tex);
    glDeleteTextures(1, &s_pp_scene_depth);

    glDeleteFramebuffers(1, &s_pp_bright_fbo);
    glDeleteTextures(1, &s_pp_bright_tex);

    glDeleteFramebuffers(1, &s_pp_ping_fbo);
    glDeleteTextures(1, &s_pp_ping_tex);
    glDeleteFramebuffers(1, &s_pp_pong_fbo);
    glDeleteTextures(1, &s_pp_pong_tex);

    glDeleteFramebuffers(1, &s_pp_godrays_fbo);
    glDeleteTextures(1, &s_pp_godrays_tex);

    glDeleteBuffers(1, &s_pp_quad_vbo);
    glDeleteVertexArrays(1, &s_pp_quad_vao);
}

#endif /* __EMSCRIPTEN__ */
