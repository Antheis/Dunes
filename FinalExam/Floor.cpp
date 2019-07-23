#include "Floor.h"

#include "PerlinNoise.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>

Floor::Floor(int size)
	: Figure("desert_height_map.png", Material{
			{ 0.2125f, 0.1275f, 0.054f },
			{ 0.714f, 0.4284f, 0.18144f },
			{ 0.393548f, 0.271906f, 0.166721f },
			.5f
		}), size_{ size }
{
	std::srand(NULL);
	setup();
}

Floor::~Floor()
{
	glBindVertexArray(0);
}

void Floor::setup()
{
	glGenVertexArrays(1, &handler_);
	glBindVertexArray(handler_);

	float max = size_ / 2.f;
	float min = -max;

	std::cout << "Please input a maximum height for the dunes: ";
	float height;
	std::cin >> height;
	height = abs(height);

	float offset = 10.f;

	int nb_pts = size_ * static_cast<int>(offset);

	PerlinNoise noise{};

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	float scale = std::mt19937{ seed }() / static_cast<float>(std::mt19937::max()) + 2; //between 2 and 3 makes pretty dunes

	std::cout << "Generating desert of size " << size_ << " with a noise scale of " << scale << std::endl;

	glPos3D* vertices_ = new glPos3D[nb_pts * nb_pts];
	float* noise_values_ = new float[nb_pts * nb_pts];

	for (int y = 0; y < nb_pts; ++y)
	{
		for (int x = 0; x < nb_pts; ++x)
		{
			float bump = tex_->getPixel(x / static_cast<float>(nb_pts) * tex_->getWidth(),
				y / static_cast<float>(nb_pts) * tex_->getHeight()) / 255.f;

			float xCoord = min + (x / offset);
			float yCoord = min + (y / offset);

			float noise_bump = noise((xCoord / static_cast<float>(size_)) * scale * 15, 1, (yCoord / static_cast<float>(size_)) * scale * 15) / scale * -15;

			float noiseValue = noise((xCoord / static_cast<float>(size_)) * scale, 0, (yCoord / static_cast<float>(size_)) * scale) / scale;
			vertices_[y * nb_pts + x] =
				glPos3D{ xCoord,
				static_cast<GLfloat>(noiseValue) * height + bump * 1.8f + noise_bump * 0.025f,
				yCoord }
			;

			noise_values_[y * nb_pts + x] = (std::rand() % 256) / 255.f;
		}
	}

	glGenBuffers(1, &vbo_vertices_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glPos3D) * nb_pts * nb_pts, vertices_, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,   //attr number = 0
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		0);
	glEnableVertexAttribArray(0);  //attr number = 0

	glGenBuffers(1, &vbo_noise_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_noise_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * nb_pts * nb_pts, noise_values_, GL_STATIC_DRAW);
	glVertexAttribPointer(
		2,   //attr number = 2
		1,
		GL_FLOAT,
		GL_FALSE,
		0,
		0);
	glEnableVertexAttribArray(2);  //attr number = 2

	glPos3D* normals_ = new glPos3D[nb_pts * nb_pts];
	GLuint* indexes_ = new GLuint[(nb_pts - 1) * (nb_pts - 1) * 6];
	int idx = 0;
	for (int y = 0; y < nb_pts; ++y)
		for (int x = 0; x < nb_pts; ++x)
		{
			glPos3D vec1 = vertices_[(nb_pts * (!y ? (y + 1) : (y - 1)) + x)] - vertices_[(nb_pts * y + x)];
			glPos3D vec2 = vertices_[(nb_pts * y + (!x ? (x + 1) : (x - 1)))] - vertices_[(nb_pts * y + x)];

			normals_[y * nb_pts + x] = glPos3D{
				vec1.y * vec2.z - vec1.z * vec2.y,
				vec1.z * vec2.x - vec1.x * vec2.z,
				vec1.x * vec2.y - vec1.y * vec2.x
			};

			if (!y || !x) continue;
			indexes_[idx] = ((nb_pts * y + x));
			indexes_[idx + 1] = ((nb_pts * y + (x - 1)));
			indexes_[idx + 2] = ((nb_pts * (y - 1) + (x - 1)));
			indexes_[idx + 3] = ((nb_pts * (y - 1) + (x - 1)));
			indexes_[idx + 4] = ((nb_pts * (y - 1) + x));
			indexes_[idx + 5] = ((nb_pts * y + x));
			idx += 6;
		}

	glGenBuffers(1, &vbo_normals_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normals_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glPos3D) * nb_pts * nb_pts, normals_, GL_STATIC_DRAW);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &ibo_indexes_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_indexes_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * idx, indexes_, GL_STATIC_DRAW);

	delete[] vertices_;
	delete[] noise_values_;
	delete[] normals_;
	delete[] indexes_;
}

void Floor::draw()
{
	glBindVertexArray(handler_);

	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLuint), GL_UNSIGNED_INT, 0);
}