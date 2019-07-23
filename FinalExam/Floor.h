#pragma once

#include "GLUtils.h"

#include "Figure.h"

#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class Floor : public Figure
{
public:
	Floor(int size);
	~Floor();
	Floor(const Floor&) = default;

	void draw() override;

private:
	void setup() override;

	int size_;

	VBO vbo_vertices_;
	VBO vbo_normals_;
	VBO vbo_noise_;

	IBO ibo_indexes_;
};