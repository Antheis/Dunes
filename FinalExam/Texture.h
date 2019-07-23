#pragma once

#include "GLUtils.h"

#include <string>
#include <vector>

#include "stb_image.h"

#include <GL/glew.h>

class Texture
{
public:
	Texture() = default;
	Texture(const std::string& filename);
	~Texture() = default;

	void load_image(const std::string& filename);
	void load_texture(const std::string& filename);

	GLuint get_id() const { return tex_2d; }
	int getWidth() const { return width; }
	int getHeight() const { return height; }
	const std::vector<unsigned char> getPixels() const { return pixels_; };
	unsigned char getPixel(int x, int y) const {
		return pixels_[width * y + x];
	}

private:
	GLuint tex_2d;
	int width;
	int height;
	int channel;
	std::vector<unsigned char> pixels_;
};