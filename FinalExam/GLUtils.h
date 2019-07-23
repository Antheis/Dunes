#pragma once

#include <GL/glew.h>
#include <glm/vec3.hpp>

#include <iostream>
#include <fstream>

constexpr uint8_t VBO_INIT_VALUE = 0;

#pragma pack(push, 4)

struct glPos2D
{
	GLfloat x;
	GLfloat y;

	friend std::ostream& operator<<(std::ostream& out, const glPos2D& pos) {
		out << "Pos(x:" << pos.x << ", y:" << pos.y << ")";
		return out;
	}
};

struct glPos3D
{
	GLfloat x;
	GLfloat y;
	GLfloat z;

	friend glPos3D operator-(const glPos3D& first, const glPos3D& second)
	{
		return { first.x - second.x, first.y - second.y, first.z - second.z };
	}

	friend std::ostream& operator<<(std::ostream& out, const glPos3D& pos) {
		out << "Pos(x:" << pos.x << ", y:" << pos.y << ", z:" << pos.z << ")";
		return out;
	}
};

struct glColor3
{
	GLubyte r;
	GLubyte g;
	GLubyte b;

	friend std::ostream& operator<<(std::ostream& out, const glColor3& col) {
		out << "Color(r:" << col.r << ", g:" << col.g << ", b:" << col.b << ")";
		return out;
	}
};

struct glColor4
{
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;

	friend std::ostream& operator<<(std::ostream& out, const glColor4& col) {
		out << "Color(r:" << col.r << ", g:" << col.g << ", b:" << col.b << ", a:" << col.a << ")";
		return out;
	}
};

struct vertexArray
{
	glPos3D pos;
	glColor3 col;

	friend std::ostream& operator<<(std::ostream& out, const vertexArray& vertex) {
		out << "Vertex(" << vertex.pos << ", " << vertex.col << ")";
		return out;
	}
};

#pragma pack(pop)

typedef GLuint VAO, VBO, IBO;