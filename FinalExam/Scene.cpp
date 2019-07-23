#include "Scene.h"

#include "Floor.h"
#include <random>
#include <chrono>

Scene::Scene()
	: light_{ {0, 200, 500}, {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8}, {0.8, 0.8, 0.8} }
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	int size = std::mt19937{seed}() % 200 + 200;
	auto& fig = figs_.emplace_back(std::make_unique<Floor>(size));
	fig->glScale(2.f, 2.2f, 2.f);
}

void Scene::draw(Camera& cam, Shader& shader)
{
	glClearColor(0.07f, 0.87f, 1.f, 0);  //background color setting
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //clear buffers

	for (auto& fig : figs_)
	{
		shader.setupUniformVar(*fig, cam, light_);
		fig->draw();
	}
}