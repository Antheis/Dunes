#include "Window.h"

#include "DesertShader.h"

Window::Window(uint16_t width, uint16_t height)
	: width_{ width }, height_{ height },
	cam_{ {0, 20, -50}, {0, 0, 0}, {0, 1, 0}, 60.f, static_cast<float>(width / height) },
	scene_{}
{
	shader_ = std::make_unique<DesertShader>();
}

void Window::tick()
{
	shader_->use();
	scene_.draw(cam_, *shader_);
	shader_->disable();
}

void Window::translate(float changeHoriz, float changeVert, bool inImagePlane)
{
	cam_.translate(changeHoriz, changeVert, inImagePlane);
}

void Window::zoom(float changeVert)
{
	cam_.zoom(changeVert);
}

void Window::rotate(float changeHoriz, float changeVert)
{
	cam_.rotate(changeHoriz, changeVert);
}