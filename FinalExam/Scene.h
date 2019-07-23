#pragma once

#include "Camera.h"
#include "Figure.h"
#include "Light.h"
#include "Shader.h"

#include <vector>
#include <unordered_map>

class Scene
{
public:
	Scene();

	void draw(Camera& cam, Shader& shader);

private:
	std::vector<std::unique_ptr<Figure>> figs_;
	Light light_;
};