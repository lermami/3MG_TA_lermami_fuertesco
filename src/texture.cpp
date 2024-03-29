#include "texture.hpp"
#include "..\deps\stb_image\stbi_image.h"

#include <GL/glew.h>

Texture::Texture(TextureTarget target, TextureFormat format, TextureType type) {
	target_ = target;
	format_ = format;
	type_ = type;

	wrap_s_format_ = TextureWrap::kRepeat;
	wrap_t_format_ = TextureWrap::kRepeat;
	min_filter_format_ = TextureFiltering::kLinear;
	mag_filter_format_ = TextureFiltering::kLinear;
}

Texture::~Texture() {

}

unsigned Texture::LoadTexture(const char* path) {
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(1);

	unsigned char* tex_src = stbi_load(path, &width, &height, &nrChannels, 0);

	GLenum target = GL_NONE;
	switch (target_) {
		case TextureTarget::kTexture_1D: target = GL_TEXTURE_1D; break;
		case TextureTarget::kTexture_2D: target = GL_TEXTURE_2D; break;
		case TextureTarget::kTexture_3D: target = GL_TEXTURE_3D; break;
	}

	GLenum format = GL_NONE;
	switch (format_) {
		case TextureFormat::kRGB: format = GL_RGB; break;
		case TextureFormat::kRGBA: format = GL_RGBA; break;
		case TextureFormat::kDepthComponent: format = GL_DEPTH_COMPONENT; break;
	}

	GLenum wrap_s = GL_NONE;
	switch (wrap_s_format_) {
		case TextureWrap::kClampToBorder: wrap_s = GL_CLAMP_TO_BORDER; break;
		case TextureWrap::kClampToEdge: wrap_s = GL_CLAMP_TO_EDGE; break;
		case TextureWrap::kMirroredRepeat: wrap_s = GL_MIRRORED_REPEAT; break;
		case TextureWrap::kRepeat: wrap_s = GL_REPEAT; break;
	}

	GLenum wrap_t = GL_NONE;
	switch (wrap_t_format_) {
		case TextureWrap::kClampToBorder: wrap_t = GL_CLAMP_TO_BORDER; break;
		case TextureWrap::kClampToEdge: wrap_t = GL_CLAMP_TO_EDGE; break;
		case TextureWrap::kMirroredRepeat: wrap_t = GL_MIRRORED_REPEAT; break;
		case TextureWrap::kRepeat: wrap_t = GL_REPEAT; break;
	}

	GLenum min = GL_NONE; 
	switch (min_filter_format_)
	{
		case TextureFiltering::kLinear: min = GL_LINEAR; break;
		case TextureFiltering::kNearest: min = GL_NEAREST; break;
	}
	
	GLenum mag = GL_NONE;
	switch (mag_filter_format_)
	{
		case TextureFiltering::kLinear: mag = GL_LINEAR; break;
		case TextureFiltering::kNearest: mag = GL_NEAREST; break;
	}

	GLenum type = GL_NONE;
	switch (type_)
	{
	case TextureType::kUnsignedByte: type = GL_UNSIGNED_BYTE; break;
	case TextureType::kFloat: type = GL_FLOAT; break;
	}

	glGenTextures(1, &handle_);
	glBindTexture(target, handle_);

	glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap_t);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag);
	glTexImage2D(target, 0, format, width, height, 0, format, type, tex_src);
	glGenerateMipmap(target);
	glBindTexture(target, 0);

	stbi_image_free(tex_src);

	return handle_;
}

unsigned Texture::LoadTexture(int w, int h) {
	GLenum target = GL_NONE;
	switch (target_) {
	case TextureTarget::kTexture_1D: target = GL_TEXTURE_1D; break;
	case TextureTarget::kTexture_2D: target = GL_TEXTURE_2D; break;
	case TextureTarget::kTexture_3D: target = GL_TEXTURE_3D; break;
	}

	GLenum format = GL_NONE;
	switch (format_) {
	case TextureFormat::kRGB: format = GL_RGB; break;
	case TextureFormat::kRGBA: format = GL_RGBA; break;
	case TextureFormat::kDepthComponent: format = GL_DEPTH_COMPONENT; break;
	}

	GLenum wrap_s = GL_NONE;
	switch (wrap_s_format_) {
	case TextureWrap::kClampToBorder: wrap_s = GL_CLAMP_TO_BORDER; break;
	case TextureWrap::kClampToEdge: wrap_s = GL_CLAMP_TO_EDGE; break;
	case TextureWrap::kMirroredRepeat: wrap_s = GL_MIRRORED_REPEAT; break;
	case TextureWrap::kRepeat: wrap_s = GL_REPEAT; break;
	}

	GLenum wrap_t = GL_NONE;
	switch (wrap_t_format_) {
	case TextureWrap::kClampToBorder: wrap_t = GL_CLAMP_TO_BORDER; break;
	case TextureWrap::kClampToEdge: wrap_t = GL_CLAMP_TO_EDGE; break;
	case TextureWrap::kMirroredRepeat: wrap_t = GL_MIRRORED_REPEAT; break;
	case TextureWrap::kRepeat: wrap_t = GL_REPEAT; break;
	}

	GLenum min = GL_NONE;
	switch (min_filter_format_)
	{
	case TextureFiltering::kLinear: min = GL_LINEAR; break;
	case TextureFiltering::kNearest: min = GL_NEAREST; break;
	}

	GLenum mag = GL_NONE;
	switch (mag_filter_format_)
	{
	case TextureFiltering::kLinear: mag = GL_LINEAR; break;
	case TextureFiltering::kNearest: mag = GL_NEAREST; break;
	}

	GLenum type = GL_NONE;
	switch (type_)
	{
		case TextureType::kUnsignedByte: type = GL_UNSIGNED_BYTE; break;
		case TextureType::kFloat: type = GL_FLOAT; break;
	}

	glGenTextures(1, &handle_);
	glBindTexture(target, handle_);

	glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap_t);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag);
	glTexImage2D(target, 0, format, w, h, 0, format, type, NULL);
	glGenerateMipmap(target);
	glBindTexture(target, 0);

	return handle_;
}


void Texture::set_wrap_s(TextureWrap param) {
	wrap_s_format_ = param;
}

void Texture::set_wrap_t(TextureWrap param) {
	wrap_t_format_ = param;
}

void Texture::set_min_filter(TextureFiltering param) {
	min_filter_format_ = param;
}

void Texture::set_mag_filter(TextureFiltering param) {
	mag_filter_format_ = param;
}