#pragma once

#include "Shader.h"

class DesertShader : public Shader
{
public:
	DesertShader();
	~DesertShader() = default;

	void setup() override;
	void setupUniformVar(const Figure& fig, const Camera& cam, const Light& light) override;
};