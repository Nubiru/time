#ifdef __EMSCRIPTEN__

#include "shader.h"

#include <GLES3/gl3.h>
#include <stdio.h>

GLuint shader_compile(GLenum type, const char *source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, sizeof(log), NULL, log);
        printf("Shader compile error: %s\n", log);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint shader_create_program(const char *vert_source, const char *frag_source) {
    GLuint vert = shader_compile(GL_VERTEX_SHADER, vert_source);
    if (vert == 0) return 0;

    GLuint frag = shader_compile(GL_FRAGMENT_SHADER, frag_source);
    if (frag == 0) {
        glDeleteShader(vert);
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(program, sizeof(log), NULL, log);
        printf("Program link error: %s\n", log);
        glDeleteProgram(program);
        glDeleteShader(vert);
        glDeleteShader(frag);
        return 0;
    }

    glDeleteShader(vert);
    glDeleteShader(frag);

    return program;
}

#endif /* __EMSCRIPTEN__ */
