#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stack>

class Model
{
public:
	Model();

	void glTranslate(float x, float y, float z);
	void glRotate(float degree, float x, float y, float z);
	void glScale(float x, float y, float z);

	const glm::mat4& getMatrix() const;

	void pushMatrix();
	void popMatrix();

private:
	std::stack<glm::mat4> model_matrix_;
};
