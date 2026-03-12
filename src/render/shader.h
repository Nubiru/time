#ifndef TIME_SHADER_H
#define TIME_SHADER_H

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>

GLuint shader_create_program(const char *vert_source, const char *frag_source);
#endif

#endif
