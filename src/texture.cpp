#include "texture.hpp"
#include "..\deps\stb_image\stbi_image.h"

#include <GL/glew.h>

Texture::Texture(unsigned handle) : handle_ { handle }{
}

Texture::~Texture() {
	if (destroy_) {
		glDeleteTextures(1, &handle_);
	}
}

Texture::Texture(Texture&& o) : handle_ { o.handle_ }{
	o.destroy_ = false;
}

Texture& Texture::operator=(Texture&& o) : handle_{ o.handle_ } {
	o.destroy_ = false;
	return *this;
}

Texture Texture::LoadTexture(TextureInfo info, const char* path) {
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(1);

	unsigned char* tex_src = stbi_load(path, &width, &height, &nrChannels, 0);

	GLenum target = GL_NONE;
	switch (info.target_) {
		case TextureTarget::kTexture_1D: target = GL_TEXTURE_1D; break;
		case TextureTarget::kTexture_2D: target = GL_TEXTURE_2D; break;
		case TextureTarget::kTexture_3D: target = GL_TEXTURE_3D; break;
	}

	GLenum format = GL_NONE;
	switch (info.format_) {
		case TextureFormat::kRGB: format = GL_RGB; break;
		case TextureFormat::kRGBA: format = GL_RGBA; break;
		case TextureFormat::kDepthComponent: format = GL_DEPTH_COMPONENT; break;
	}

	GLenum wrap_s = GL_NONE;
	switch (info.wrap_s_format_) {
		case TextureWrap::kClampToBorder: wrap_s = GL_CLAMP_TO_BORDER; break;
		case TextureWrap::kClampToEdge: wrap_s = GL_CLAMP_TO_EDGE; break;
		case TextureWrap::kMirroredRepeat: wrap_s = GL_MIRRORED_REPEAT; break;
		case TextureWrap::kRepeat: wrap_s = GL_REPEAT; break;
	}

	GLenum wrap_t = GL_NONE;
	switch (info.wrap_t_format_) {
		case TextureWrap::kClampToBorder: wrap_t = GL_CLAMP_TO_BORDER; break;
		case TextureWrap::kClampToEdge: wrap_t = GL_CLAMP_TO_EDGE; break;
		case TextureWrap::kMirroredRepeat: wrap_t = GL_MIRRORED_REPEAT; break;
		case TextureWrap::kRepeat: wrap_t = GL_REPEAT; break;
	}

	GLenum min = GL_NONE; 
	switch (info.min_filter_format_)
	{
		case TextureFiltering::kLinear: min = GL_LINEAR; break;
		case TextureFiltering::kNearest: min = GL_NEAREST; break;
	}
	
	GLenum mag = GL_NONE;
	switch (info.mag_filter_format_)
	{
		case TextureFiltering::kLinear: mag = GL_LINEAR; break;
		case TextureFiltering::kNearest: mag = GL_NEAREST; break;
	}

	GLenum type = GL_NONE;
	switch (info.type_)
	{
	case TextureType::kUnsignedByte: type = GL_UNSIGNED_BYTE; break;
	case TextureType::kFloat: type = GL_FLOAT; break;
	}

	unsigned handle = 0;

	glGenTextures(1, &handle);
	glBindTexture(target, handle);

	glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap_t);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag);
	glTexImage2D(target, 0, format, width, height, 0, format, type, tex_src);
	glGenerateMipmap(target);
	glBindTexture(target, 0);

	stbi_image_free(tex_src);

	return Texture(handle);
}

Texture Texture::LoadTexture(TextureInfo info, int w, int h) {
	GLenum target = GL_NONE;
	switch (info.target_) {
	case TextureTarget::kTexture_1D: target = GL_TEXTURE_1D; break;
	case TextureTarget::kTexture_2D: target = GL_TEXTURE_2D; break;
	case TextureTarget::kTexture_3D: target = GL_TEXTURE_3D; break;
	}

	GLenum format = GL_NONE;
	switch (info.format_) {
	case TextureFormat::kRGB: format = GL_RGB; break;
	case TextureFormat::kRGBA: format = GL_RGBA; break;
	case TextureFormat::kDepthComponent: format = GL_DEPTH_COMPONENT; break;
	}

	GLenum wrap_s = GL_NONE;
	switch (info.wrap_s_format_) {
	case TextureWrap::kClampToBorder: wrap_s = GL_CLAMP_TO_BORDER; break;
	case TextureWrap::kClampToEdge: wrap_s = GL_CLAMP_TO_EDGE; break;
	case TextureWrap::kMirroredRepeat: wrap_s = GL_MIRRORED_REPEAT; break;
	case TextureWrap::kRepeat: wrap_s = GL_REPEAT; break;
	}

	GLenum wrap_t = GL_NONE;
	switch (info.wrap_t_format_) {
	case TextureWrap::kClampToBorder: wrap_t = GL_CLAMP_TO_BORDER; break;
	case TextureWrap::kClampToEdge: wrap_t = GL_CLAMP_TO_EDGE; break;
	case TextureWrap::kMirroredRepeat: wrap_t = GL_MIRRORED_REPEAT; break;
	case TextureWrap::kRepeat: wrap_t = GL_REPEAT; break;
	}

	GLenum min = GL_NONE;
	switch (info.min_filter_format_)
	{
	case TextureFiltering::kLinear: min = GL_LINEAR; break;
	case TextureFiltering::kNearest: min = GL_NEAREST; break;
	}

	GLenum mag = GL_NONE;
	switch (info.mag_filter_format_)
	{
	case TextureFiltering::kLinear: mag = GL_LINEAR; break;
	case TextureFiltering::kNearest: mag = GL_NEAREST; break;
	}

	GLenum type = GL_NONE;
	switch (info.type_)
	{
		case TextureType::kUnsignedByte: type = GL_UNSIGNED_BYTE; break;
		case TextureType::kFloat: type = GL_FLOAT; break;
	}

	unsigned handle = 0;

	glGenTextures(1, &handle);
	glBindTexture(target, handle);

	glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap_t);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag);
	glTexImage2D(target, 0, format, w, h, 0, format, type, NULL);
	glGenerateMipmap(target);
	glBindTexture(target, 0);

	return Texture(handle);
}

unsigned Texture::get()
{
	return handle_;
}


TextureInfo::TextureInfo(TextureTarget target, TextureFormat format, TextureType type, TextureWrap wrap_s, TextureWrap wrap_t, TextureFiltering min_filter, TextureFiltering mag_filter)
	: type_{ type }, target_{ target }, format_{ format }, wrap_s_format_{ wrap_s }, wrap_t_format_{ wrap_t }, min_filter_format_{ min_filter }, mag_filter_format_{ mag_filter } 
{
}
