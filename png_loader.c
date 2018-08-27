#include <png.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <GLES3/gl3.h>

GLuint load_png_texture(const char *const png_path)
{
	unsigned char header[8];
	GLuint texture = 0;
	png_uint_32 width = 0;
	png_uint_32 height = 0;
	png_byte color_type;
	// png_byte bit_depth;
	png_struct *png_ptr = NULL;
	png_info *info_ptr = NULL;
	// int number_of_passes = 0;
	png_byte **row_ptrs = NULL;
	int pos = 0;
	GLubyte *rgba = NULL;
	FILE *fp = NULL;
	if (!(fp = fopen(png_path, "rb"))) {
		fprintf(stderr, "Open Error: Cannot find %s.\n", png_path);
		return 0;
	}
	fread(header, 1, sizeof(header), fp);
	if (png_sig_cmp(header, 0, 8)) {
		fprintf(stderr, "Format Error: Not a valid PNG file.\n");
		fclose(fp);
		return 0;
	}
	// 后三个是绑定错误以及警告的回调函数，这里设置为空。
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		fprintf(stderr, "Create Struct Error.\n");
		fclose(fp);
		return 0;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, (png_info **)NULL, (png_info **)NULL);
		fclose(fp);
		return 0;
	}
	if (setjmp(png_jmpbuf(png_ptr))) {
	        png_destroy_read_struct(&png_ptr, &info_ptr, (png_info **)NULL);
	        fclose(fp);
	        return 0;
	}
	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);
	png_read_info(png_ptr, info_ptr);
	width = png_get_image_width(png_ptr, info_ptr);
        height = png_get_image_height(png_ptr, info_ptr);
        color_type = png_get_color_type(png_ptr, info_ptr);
	if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
	        png_set_swap_alpha(png_ptr);
	// bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	// number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);
	row_ptrs = (png_byte **)malloc(sizeof(png_byte *) * height);
	if (!row_ptrs) {
		fprintf(stderr, "Malloc Error.\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_info **)NULL);
		fclose(fp);
		return 0;
	}
	rgba = (GLubyte *)malloc(width * height * 4);
	if (!rgba) {
		fprintf(stderr, "Malloc Error.\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_info **)NULL);
		fclose(fp);
		return 0;
	}
	for (size_t i = 0; i < height; ++i)
	        row_ptrs[i] = NULL;
	for (size_t i = 0; i < height; ++i)
		row_ptrs[i] = (png_byte *)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
	png_read_image(png_ptr, row_ptrs);
	pos = width * height * 4 - 4 * width;
	for(int row = height - 1; row >= 0; --row) {
		for(int col = 0; col < (4 * width); col += 4) {
			rgba[pos++] = row_ptrs[row][col + 1];    // red
			rgba[pos++] = row_ptrs[row][col + 2];    // green
			rgba[pos++] = row_ptrs[row][col + 3];    // blue
			rgba[pos++] = row_ptrs[row][col];        // alpha
		}
		pos -= width * 4 * 2;
	}
	fclose(fp);
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_info **)NULL);
	for (size_t i = 0; i < height; ++i)
		if (row_ptrs[i])
			png_free(png_ptr, row_ptrs[i]);
	if (row_ptrs)
		free(row_ptrs);
	if (rgba)
		free(rgba);
	return texture;
}

void free_png_texture(GLuint texture)
{
	glDeleteTextures(1, &texture);
}
