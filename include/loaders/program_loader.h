#ifndef __PROGRAM_LOADER_H__
#define __PROGRAM_LOADER_H__

#include <GLES3/gl3.h>

GLuint load_program(const char *const vshader_path,
		    const char *const fshader_path);
void free_program(GLuint program);

#endif
