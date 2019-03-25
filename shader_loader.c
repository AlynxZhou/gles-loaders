#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GLES3/gl3.h>

#define LINE_LENGTH 512

GLuint make_shader(GLenum type, const char *const shader_src)
{
	GLuint shader = 0;
	GLint compiled = 0;
	if (!(shader = glCreateShader(type))) {
		fprintf(stderr, "Shader Create Error.\n");
		return 0;
	}
	glShaderSource(shader, 1, &shader_src, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLint info_len = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_len);
		if (info_len) {
			char *info = malloc(info_len * sizeof(*info));
			if (!info) {
				fprintf(stderr, "Malloc Error.\n");
				glDeleteShader(shader);
				return 0;
			}
			glGetShaderInfoLog(shader, info_len, NULL, info);
			fprintf(stderr, "Compile Error: %s\n", info);
			free(info);
		}
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

void free_shader(GLuint shader)
{
	glDeleteShader(shader);
}

GLuint load_shader(GLenum type, const char *const shader_path)
{
	GLuint result = 0;
	FILE *fp = NULL;
	size_t file_length = 0;
	char *file_content = NULL;
	char temp_line[LINE_LENGTH];
	fp = fopen(shader_path, "r");
	if (fp == NULL) {
		fprintf(stderr, "Open file %s failed.\n", shader_path);
		return 0;
	}
	fseek(fp, 0l, SEEK_END);
	file_length = ftell(fp) + 1;
	rewind(fp);
	file_content = malloc(file_length * sizeof(*file_content));
	if (fp == NULL) {
		fprintf(stderr, "Malloc failed.\n");
		fclose(fp);
		return 0;
	}
	file_content[0] = '\0';
	while (fgets(temp_line, LINE_LENGTH, fp))
		strncat(file_content, temp_line, LINE_LENGTH);
	fclose(fp);
	result = make_shader(type, file_content);
	free(file_content);
	return result;
}
