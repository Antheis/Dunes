#include "Model.h"

Model::Model()
	: model_matrix_{ {glm::mat4{ 1.f }} }
{}

void Model::glTranslate(float x, float y, float z)
{
	glm::mat4 transpose{ glm::translate(glm::mat4{1.f}, glm::vec3{x, y, z}) };
	model_matrix_.top() *= transpose;
}

void Model::glRotate(float degree, float x, float y, float z)
{
	glm::mat4 rot{ glm::rotate(glm::mat4{1.f}, glm::radians(degree), glm::vec3{x, y, z}) };
	model_matrix_.top() *= rot;
}

void Model::glScale(float x, float y, float z)
{
	glm::mat4 scale{ glm::scale(glm::mat4{1.f}, glm::vec3{x, y, z}) };
	model_matrix_.top() *= scale;
}


const glm::mat4& Model::getMatrix() const
{
	return model_matrix_.top();
}

inline void Model::pushMatrix()
{
	model_matrix_.push({ glm::mat4{ 1.f } });
}

inline void Model::popMatrix()
{
	model_matrix_.pop();
}