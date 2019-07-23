#pragma once

#include "GLUtils.h"
#include "Model.h"
#include "Texture.h"

#include <optional>

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

class Figure
{
public:
	Figure(const Material& mat = {
			{ 0.19225f, 0.19225f, 0.19225f },
			{ 0.50754f, 0.50754f, 0.50754f },
			{ 0.508273f, 0.508273f, 0.508273f },
			51.2f
		}) : mat_{ mat } {}
	Figure(const std::string& filename, const Material& mat) : mat_{ mat } {
		tex_.emplace(Texture{ filename });
	}
	virtual ~Figure() = default;
	Figure(const Figure&) = delete;

	const glm::mat4& getModel() const { return model_.getMatrix(); }
	void pushModelMatrix() { model_.pushMatrix(); }
	void popModelMatrix() { model_.popMatrix(); }
	void glTranslate(float x, float y, float z) { model_.glTranslate(x, y, z); }
	void glRotate(float degree, float x, float y, float z) { model_.glRotate(degree, x, y, z); }
	void glScale(float x, float y, float z) { model_.glScale(x, y, z); }

	const Material& getMaterial() const { return mat_; }

	bool hasTexture() const { return tex_.has_value(); }
	const Texture& getTexture() const { return tex_.value(); }

	virtual void draw() = 0;

protected:
	virtual void setup() = 0;

	VAO handler_;

	Model model_;
	Material mat_;

	std::optional<Texture> tex_;
};