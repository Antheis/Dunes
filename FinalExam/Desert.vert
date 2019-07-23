#version 430

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in float noise;

uniform mat3 NormalMatrix;
uniform mat4 ModelView;
uniform mat4 MVP;

out vec3 Position;
out vec3 Normal;
out float Noise;

void main(void)
{
	Normal = normalize(NormalMatrix * normalize(normal));
	Position = (ModelView * vec4(position, 1.0)).xyz;

	gl_Position = MVP * vec4(position, 1.0);
}