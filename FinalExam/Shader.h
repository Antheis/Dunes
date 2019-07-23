#pragma once

#include "Camera.h"
#include "Figure.h"
#include "Light.h"
#include "Loader.h"

#include <glm/glm.hpp>

class Shader
{
public:
	Shader() : shader_{ std::make_shared<ShaderProgram>() } {}
	virtual ~Shader() = default;

	virtual void setup() = 0;
	virtual void setupUniformVar(const Figure& fig, const Camera& cam, const Light& light) = 0;

	void setupTexture(const Texture& tex) {
		if (!initTex) {
			shader_->addUniform("heightmap.id");
			shader_->addUniform("heightmap.width");
			shader_->addUniform("heightmap.height");
			initTex = true;
		}

		glUniform1f(shader_->uniform("heightmap.id"), 0);
		glUniform1f(shader_->uniform("heightmap.width"), tex.getWidth());
		glUniform1f(shader_->uniform("heightmap.height"), tex.getHeight());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex.get_id());
	}
	void deactivateTexture() { glBindTexture(GL_TEXTURE_2D, 0); }

	void use()
	{
		shader_->use();
	}

	void disable()
	{
		shader_->disable();
	}

	GLuint uniform(std::string name)
	{
		return shader_->uniform(name);
	}

	bool depth = false;

protected:
	std::shared_ptr<ShaderProgram> shader_;
	bool initTex = false;
};