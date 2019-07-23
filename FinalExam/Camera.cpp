#include "Camera.h"

#include <glm/gtc/quaternion.hpp>

#include <iostream>

glm::mat4 lookAt(glm::vec3 campos, glm::vec3 look, glm::vec3 up)
{
	glm::vec3 zc{ glm::normalize(campos - look) };
	glm::vec3 xc{ glm::normalize(glm::cross(up, zc)) };
	glm::vec3 yc{ glm::normalize(glm::cross(zc, xc)) };

	glm::mat4 ret;

	ret[0][0] = xc.x;
	ret[1][0] = xc.y;
	ret[2][0] = xc.z;
	ret[3][0] = glm::dot(-xc, campos);
	ret[0][1] = yc.x;
	ret[1][1] = yc.y;
	ret[2][1] = yc.z;
	ret[3][1] = glm::dot(-yc, campos);
	ret[0][2] = zc.x;
	ret[1][2] = zc.y;
	ret[2][2] = zc.z;
	ret[3][2] = glm::dot(-zc, campos);
	ret[0][3] = 0.f;
	ret[1][3] = 0.f;
	ret[2][3] = 0.f;
	ret[3][3] = 1.f;

	return ret;
}

glm::mat4 perspective(float fovy, float aspect, float near, float far)
{
	float tanHalfFOV = glm::tan(glm::radians(fovy) * 0.5f);

	return glm::mat4{
		glm::vec4{1 / (aspect * tanHalfFOV), 0.f, 0.f, 0.f},
		glm::vec4{0.f, 1 / tanHalfFOV, 0.f, 0.f},
		glm::vec4{0.f, 0.f, -((far + near) / (far - near)), -1.f},
		glm::vec4{0.f, 0.f, -((2 * far * near) / (far - near)), 0.f}
	};
}

static float DEFAULT_TRANSLATE_SPEED = 0.5;
static float DEFAULT_ZOOM_FRACTION = 2.5;
static float DEFAULT_ROTATE_SPEED = 2.0;

Camera::Camera(const glm::vec3& viewPoint, const glm::vec3& viewCenter, const glm::vec3& upVector, float fieldOfView, float aspectRatio)
	: m_viewPoint{ viewPoint },
	m_viewCenter{ viewCenter },
	m_upVector{ glm::normalize(upVector) },
	m_fieldOfView{ fieldOfView },
	m_aspectRatio{ aspectRatio },
	m_translateSpeed{ DEFAULT_TRANSLATE_SPEED },
	m_zoomFraction{ DEFAULT_ZOOM_FRACTION },
	m_rotateSpeed{ DEFAULT_ROTATE_SPEED },
	m_pers{ perspective(45.f, aspectRatio, 0.1f, 10000.f) }
{
	getFrustrumInfo();
}

void Camera::translate(float changeHoriz, float changeVert, bool inImagePlane)
{
	glm::vec3 translateVec;

	if (inImagePlane)
		translateVec = (m_imagePlaneHorizDir * (m_displayWidth * changeHoriz)) + (m_imagePlaneVertDir * (changeVert * m_displayHeight));
	else
		translateVec = (m_viewCenter - m_viewPoint) * changeVert;

	translateVec *= m_translateSpeed;

	m_viewPoint += translateVec;
	m_viewCenter += translateVec;
}

void Camera::zoom(float changeVert)
{
	float scaleFactor = powf(2.0, -changeVert * m_zoomFraction);
	m_viewPoint = m_viewCenter + (m_viewPoint - m_viewCenter) * scaleFactor;

	getFrustrumInfo();
}

glm::quat setFromAxisAngle(glm::vec3& axis, float angle)
{
	float cosAng = cosf(angle / 2.0f);
	float sinAng = sinf(angle / 2.0f);
	float norm = sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);

	glm::quat res;

	res.x = axis.x / norm;
	res.y = axis.y / norm;
	res.z = axis.z / norm;
	res.w = cosAng;

	res.x *= sinAng;
	res.y *= sinAng;
	res.z *= sinAng;

	return res;
}

void makeOrthogonalTo(glm::vec3& vec1, glm::vec3& vec2)
{
	float length = glm::length(vec2);

	if (length == 0)
	{
		std::cout << "Warning: called MathVec3D::makeOrthogonalTo with zero vector as the argument.  Doing nothing." << std::endl;
		return;
	}
	float scale = (vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z) / (length * length);
	vec1.x -= scale * vec2.x;
	vec1.y -= scale * vec2.y;
	vec1.z -= scale * vec2.z;
}

void Camera::rotate(float changeHoriz, float changeVert)
{
	float horizRotAngle = m_rotateSpeed * changeVert;
	float vertRotAngle = -m_rotateSpeed * changeHoriz;

	glm::quat horizRot;
	horizRot = setFromAxisAngle(m_imagePlaneHorizDir, horizRotAngle);

	glm::quat vertRot;
	vertRot = setFromAxisAngle(m_imagePlaneVertDir, vertRotAngle);

	glm::quat totalRot = horizRot * vertRot;

	glm::vec3 viewVec = m_viewPoint - m_viewCenter;
	viewVec = totalRot * viewVec;

	m_viewPoint = m_viewCenter + viewVec;

	getFrustrumInfo();
}

void Camera::centerAt(const  glm::vec3& pos)
{
	m_viewPoint += (pos - m_viewCenter);
	m_viewCenter = pos;
	getFrustrumInfo();
}

void Camera::lookFrom(const glm::vec3& pos)
{
	m_viewPoint = pos;
	getFrustrumInfo();
}

glm::vec3 Camera::getViewPoint() const
{
	return (m_viewPoint);
}

glm::vec3 Camera::getViewCenter() const
{
	return (m_viewCenter);
}

glm::vec3 Camera::getUpVector() const
{
	return (m_upVector);
}

float Camera::getFieldOfView() const
{
	return (m_fieldOfView);
}

float Camera::getAspectRatio() const
{
	return (m_aspectRatio);
}

glm::vec3 Camera::getViewDir() const
{
	return (m_viewDir);
}

glm::vec3 Camera::getImagePlaneHorizDir() const
{
	return (m_imagePlaneHorizDir);
}

glm::vec3 Camera::getImagePlaneVertDir() const
{
	return (m_imagePlaneVertDir);
}

void Camera::setAspectRatio(float aspectRatio)
{
	if (m_aspectRatio == aspectRatio)
		return;
	m_aspectRatio = aspectRatio;
	getFrustrumInfo();
}

void Camera::setFieldOfView(float fieldOfView)
{
	if (m_fieldOfView == fieldOfView)
		return;
	m_fieldOfView = fieldOfView;
	getFrustrumInfo();
}

void Camera::setTranslateSpeed(float translateSpeed)
{
	m_translateSpeed = translateSpeed;
}

void Camera::setZoomFraction(float zoomFraction)
{
	m_zoomFraction = zoomFraction;
}

void Camera::setRotateSpeed(float rotateSpeed)
{
	m_rotateSpeed = rotateSpeed;
}

void Camera::getFrustrumInfo()
{
	// Get the viewing direction
	m_viewDir = m_viewCenter - m_viewPoint;
	m_viewDir = glm::normalize(m_viewDir);

	// Get the vertical image-plane direction (the projection of the up vector into the view plane)
	m_imagePlaneVertDir = m_upVector;
	makeOrthogonalTo(m_imagePlaneVertDir, m_viewDir);
	m_imagePlaneVertDir = glm::normalize(m_imagePlaneVertDir);

	// Get the horizontal image-plane direction
	m_imagePlaneHorizDir = glm::cross(m_viewDir, m_imagePlaneVertDir);
	m_imagePlaneHorizDir = glm::normalize(m_imagePlaneHorizDir);

	// Get the view plane width and height at the view center.
	m_displayHeight = 2.f * glm::length(m_viewCenter - m_viewPoint) * tan(0.5f * m_fieldOfView);
	m_displayWidth = m_displayHeight * m_aspectRatio;
}

glm::mat4 Camera::updateLookAt() const
{
	return lookAt(m_viewPoint, m_viewCenter, m_upVector);
}

glm::mat4 Camera::getPerspective() const
{
	return m_pers;
}