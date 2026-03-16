#ifdef __EMSCRIPTEN__

#include "gl_init.h"

#include <GLES3/gl3.h>
#include <emscripten/html5.h>
#include <stdio.h>

int gl_init(void) {
    EmscriptenWebGLContextAttributes attrs;
    emscripten_webgl_init_context_attributes(&attrs);
    attrs.majorVersion = 2;
    attrs.minorVersion = 0;
    attrs.alpha = 0;
    attrs.depth = 1;
    attrs.antialias = 1;

    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx =
        emscripten_webgl_create_context("#canvas", &attrs);
    if (ctx <= 0) {
        printf("Failed to create WebGL2 context: %d\n", (int)ctx);
        return -1;
    }

    emscripten_webgl_make_context_current(ctx);

    /* Match drawing buffer to CSS layout size */
    double css_w, css_h;
    emscripten_get_element_css_size("#canvas", &css_w, &css_h);
    emscripten_set_canvas_element_size("#canvas", (int)css_w, (int)css_h);
    glViewport(0, 0, (int)css_w, (int)css_h);

    /* Background: space-black #060709 — blue-tinted, alive (color_theory.h) */
    glClearColor(0.024f, 0.027f, 0.036f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    printf("WebGL2 context created: %dx%d\n", (int)css_w, (int)css_h);
    return 0;
}

#endif /* __EMSCRIPTEN__ */
