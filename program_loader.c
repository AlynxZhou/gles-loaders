#include <stdio.h>
#include <stdlib.h>
#include <GLES3/gl3.h>
#include "shader_loader.h"

GLuint load_program(const char *const vshader_path, const char *const fshader_path)
{
	GLint linked = 0;
	GLuint vshader = 0;
	GLuint fshader = 0;
	GLuint program = 0;
	vshader = load_shader(GL_VERTEX_SHADER, vshader_path);
	fshader = load_shader(GL_FRAGMENT_SHADER, fshader_path);
	if (!vshader || !fshader)
		return 0;
	program = glCreateProgram();
	if (!program) {
		fprintf(stderr, "Program Create Error.\n");
		return 0;
	}
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);
	glLinkProgram(program);
	free_shader(vshader);
	free_shader(fshader);
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {
		GLint info_len = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_len);
		if (info_len) {
			char *info = (char *)malloc(info_len * sizeof(char));
			glGetProgramInfoLog(program, info_len, NULL, info);
			fprintf(stderr, "Link Error: %s\n", info);
			free(info);
		}
		glDeleteProgram(program);
		return 0;
        }
	return program;
}

void free_program(GLuint program)
{
	glDeleteProgram(program);
}
