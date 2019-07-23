#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"

Texture::Texture(const std::string& filename)
{
	load_image(filename);
}

void Texture::load_image(const std::string& filename)
{
	int channelCount = 1;
	unsigned char* image = stbi_load(filename.c_str(), &width, &height, &channel, channelCount);

	int max_size = width * height * channelCount;
	pixels_.resize(max_size);
	for (int i = 0; i < max_size; ++i)
		pixels_[i] = image[i];

	stbi_image_free(image);
}

void Texture::load_texture(const std::string& filename)
{
	glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &tex_2d);  //tex_2d is a member variable
	glBindTexture(GL_TEXTURE_2D, tex_2d);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	GLenum format = GL_RED;
	if (channel == 3)
		format = GL_RGB;
	else if (channel == 4)
		format = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels_.data());
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}