#include "Light.h"

Light::Light(glm::vec3 position,
	glm::vec3 ambient,
	glm::vec3 diffuse,
	glm::vec3 specular)
	: pos_{ position },
	ambient_{ ambient },
	diffuse_{ diffuse },
	specular_{ specular }
{}

inline void Light::setPosition(const glm::vec3& position)
{
	pos_ = position;
}

inline void Light::setAmbient(const glm::vec3& ambient)
{
	ambient_ = ambient;
}

inline void Light::setDiffuse(const glm::vec3& diffuse)
{
	diffuse_ = diffuse;
}

inline void Light::setSpecular(const glm::vec3& specular)
{
	specular_ = specular;
}

const glm::vec3& Light::getPosition() const
{
	return pos_;
}

const glm::vec3& Light::getAmbient() const
{
	return ambient_;
}

const glm::vec3& Light::getDiffuse() const
{
	return diffuse_;
}

const glm::vec3& Light::getSpecular() const
{
	return specular_;
}