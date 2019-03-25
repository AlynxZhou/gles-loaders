#ifndef __SHADER_H__
#define __SHADER_H__

#include <GLES3/gl3.h>

GLuint make_shader(GLenum type, const char *const shader_src);
void free_shader(GLuint shader);
GLuint load_shader(GLenum type, const char *const shader_path);

#endif
