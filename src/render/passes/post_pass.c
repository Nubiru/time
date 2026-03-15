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

    /* Scene FBO (with depth buffer) */
    s_pp_scene_tex = create_color_texture(width, height);
    glGenRenderbuffers(1, &s_pp_scene_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, s_pp_scene_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

    glGenFramebuffers(1, &s_pp_scene_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, s_pp_scene_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, s_pp_scene_tex, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, s_pp_scene_depth);

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

    s_pp_active = 1; /* everything succeeded — full pipeline */
    printf("Post-process: bloom + tonemap + vignette, %dx%d, shaders compiled\n",
           width, height);
    return 0;
}

void post_pass_begin(const render_frame_t *frame) {
    (void)frame;
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

    /* Step 3: Composite to default framebuffer */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, s_pp_width, s_pp_height);

    glUseProgram(s_pp_composite_program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, s_pp_scene_tex);
    glUniform1i(s_pp_comp_loc_scene, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, s_pp_pong_tex);
    glUniform1i(s_pp_comp_loc_bloom, 1);

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

    draw_quad();

    glEnable(GL_DEPTH_TEST);
}

void post_pass_resize(int width, int height) {
    s_pp_width = width;
    s_pp_height = height;

    /* Resize scene FBO */
    glBindTexture(GL_TEXTURE_2D, s_pp_scene_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0,
                 GL_RGBA, GL_HALF_FLOAT, NULL);
    glBindRenderbuffer(GL_RENDERBUFFER, s_pp_scene_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

    /* Resize bloom FBOs (half res) */
    int bw = width / 2, bh = height / 2;
    if (bw < 1) bw = 1;
    if (bh < 1) bh = 1;

    GLuint textures[] = { s_pp_bright_tex, s_pp_ping_tex, s_pp_pong_tex };
    for (int i = 0; i < 3; i++) {
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, bw, bh, 0,
                     GL_RGBA, GL_HALF_FLOAT, NULL);
    }
}

void post_pass_destroy(void) {
    glDeleteProgram(s_pp_bright_program);
    glDeleteProgram(s_pp_blur_program);
    glDeleteProgram(s_pp_composite_program);

    glDeleteFramebuffers(1, &s_pp_scene_fbo);
    glDeleteTextures(1, &s_pp_scene_tex);
    glDeleteRenderbuffers(1, &s_pp_scene_depth);

    glDeleteFramebuffers(1, &s_pp_bright_fbo);
    glDeleteTextures(1, &s_pp_bright_tex);

    glDeleteFramebuffers(1, &s_pp_ping_fbo);
    glDeleteTextures(1, &s_pp_ping_tex);
    glDeleteFramebuffers(1, &s_pp_pong_fbo);
    glDeleteTextures(1, &s_pp_pong_tex);

    glDeleteBuffers(1, &s_pp_quad_vbo);
    glDeleteVertexArrays(1, &s_pp_quad_vao);
}

#endif /* __EMSCRIPTEN__ */
