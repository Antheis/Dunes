#pragma once

#include "Camera.h"
#include "Scene.h"
#include "Shader.h"

#include <unordered_map>

class Window
{
public:
	Window(uint16_t width, uint16_t height);

	void tick();

	void translate(float changeHoriz, float changeVert, bool inImagePlane);
	void zoom(float changeVert);
	void rotate(float changeHoriz, float changeVert);

private:
	uint16_t width_;
	uint16_t height_;

	Scene scene_;
	Camera cam_;
	std::unique_ptr<Shader> shader_;
};