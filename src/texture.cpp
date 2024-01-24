#include "texture.hpp"
#include "..\deps\stb_image\stbi_image.h"

#include <GL/glew.h>

Texture::Texture(TextureType type, TextureFormat format) {
	type_ = type;
	format_ = format;

	wrap_s_format_ = TextureParameters::kRepeat;
	wrap_t_format_ = TextureParameters::kRepeat;
	min_filter_format_ = TextureParameters::kLinear;
	mag_filter_format_ = TextureParameters::kLinear;
}

Texture::~Texture() {

}

unsigned Texture::LoadTexture(const char* path) {
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(1);
	unsigned char* tex_src = stbi_load(path, &width, &height, &nrChannels, 0);

	GLenum type = (GLenum)type_;
	GLenum format = (GLenum)format_;
	GLenum wrap_s = (GLenum)wrap_s_format_;
	GLenum wrap_t = (GLenum)wrap_t_format_;
	GLenum min = (GLenum)min_filter_format_;
	GLenum mag = (GLenum)mag_filter_format_;

	glGenTextures(1, &handle_);
	glBindTexture(type, handle_);

	glTexParameteri(type, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, wrap_t);
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, mag);
	glTexImage2D(type, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, tex_src);
	glGenerateMipmap(type);

	stbi_image_free(tex_src);

	return handle_;
}

void Texture::set_wrap_s(TextureParameters param) {
	wrap_s_format_ = param;
}

void Texture::set_wrap_t(TextureParameters param) {
	wrap_t_format_ = param;
}

void Texture::set_min_filter(TextureParameters param) {
	min_filter_format_ = param;
}

void Texture::set_mag_filter(TextureParameters param) {
	mag_filter_format_ = param;
}