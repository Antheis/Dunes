#include "DesertShader.h"

#include <glm/gtc/type_ptr.hpp>

DesertShader::DesertShader()
	: Shader{}
{
	setup();
}

void DesertShader::setup()
{
	shader_->initFromFiles("Desert.vert", "Desert.frag");

	shader_->addUniform("NormalMatrix");
	shader_->addUniform("ModelView");
	shader_->addUniform("MVP"); //Projection*View*Model : mat4

	shader_->addUniform("Light.Position");
	shader_->addUniform("Light.Intensity");

	shader_->addUniform("Camera.Position");
	shader_->addUniform("Camera.ViewDirection");
}

void DesertShader::setupUniformVar(const Figure& fig, const Camera& cam, const Light& light)
{
	glm::mat4 modelView = cam.updateLookAt() * fig.getModel();
	glm::mat4 mvp = cam.getPerspective() * modelView;

	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelView)));

	glUniformMatrix3fv(shader_->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
	glUniformMatrix4fv(shader_->uniform("ModelView"), 1, GL_FALSE, glm::value_ptr(modelView));
	glUniformMatrix4fv(shader_->uniform("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));

	glUniform3fv(shader_->uniform("Light.Position"), 1, glm::value_ptr(light.getPosition()));
	glUniform3fv(shader_->uniform("Light.Intensity"), 1, glm::value_ptr(light.getAmbient()));

	glUniform3fv(shader_->uniform("Camera.Position"), 1, glm::value_ptr(cam.getViewPoint()));
	glUniform3fv(shader_->uniform("Camera.ViewDirection"), 1, glm::value_ptr(cam.getViewDir()));
}