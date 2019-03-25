#ifndef __PNG_LOADER_H__
#define __PNG_LOADER_H__

#include <GLES3/gl3.h>

GLuint load_png_texture(const char *const png_path);
void free_png_texture(GLuint texture);

#endif
