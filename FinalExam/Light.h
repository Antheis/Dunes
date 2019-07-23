#pragma once

#include <glm/glm.hpp>

class Light
{
public:
	Light(glm::vec3 position,
		glm::vec3 ambient,
		glm::vec3 diffuse,
		glm::vec3 specular);

	void setPosition(const glm::vec3& position);
	void setAmbient(const glm::vec3& ambient);
	void setDiffuse(const glm::vec3& diffuse);
	void setSpecular(const glm::vec3& specular);

	const glm::vec3& getPosition() const;
	const glm::vec3& getAmbient() const;
	const glm::vec3& getDiffuse() const;
	const glm::vec3& getSpecular() const;

private:
	glm::vec3 pos_;
	glm::vec3 ambient_;
	glm::vec3 diffuse_;
	glm::vec3 specular_;
};