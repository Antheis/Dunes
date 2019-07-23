#version 430

in vec3 Position;
in vec3 Normal;
in float Noise;

struct LightInfo
{
	vec3 Position;
	vec3 Intensity;
};
uniform LightInfo Light;

struct CameraInfo
{
	vec3 Position;
	vec3 ViewDirection;
};
uniform CameraInfo Camera;

out vec4 FragColor;

float mod289(float x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
vec4 mod289(vec4 x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
vec4 perm(vec4 x){return mod289(((x * 34.0) + 1.0) * x);}

float noise(vec3 p){
    vec3 a = floor(p);
    vec3 d = p - a;
    d = d * d * (3.0 - 2.0 * d);

    vec4 b = a.xxyy + vec4(0.0, 1.0, 0.0, 1.0);
    vec4 k1 = perm(b.xyxy);
    vec4 k2 = perm(k1.xyxy + b.zzww);

    vec4 c = k2 + a.zzzz;
    vec4 k3 = perm(c);
    vec4 k4 = perm(c + 1.0);

    vec4 o1 = fract(k3 * (1.0 / 41.0));
    vec4 o2 = fract(k4 * (1.0 / 41.0));

    vec4 o3 = o2 * d.z + o1 * (1.0 - d.z);
    vec2 o4 = o3.yw * d.x + o3.xz * (1.0 - d.x);

    return o4.y * d.y + o4.x * (1.0 - d.y);
}

vec3 high_end_col = vec3(0.877, 0.82, 0.155);
vec3 low_end_color = vec3(0.678, 0.545, 0.329);//darkest dune's color

vec3 Lambert(vec3 position, vec3 norm)
{
  vec3 nrmN = normalize(-norm);
  nrmN.y *= 0.3;
  vec3 nrmL = normalize(position);
  float result = 2 * dot(nrmN, nrmL);

  return mix(low_end_color, high_end_col, Noise) * max(result, 0.0);
}

float ocean_specular(vec3 position, vec3 normal)
{
	vec3 lightVec = normalize(position - Light.Position);
	vec3 reflection = reflect(lightVec, normal);

	float specular = dot(normalize(reflection), -normalize(Camera.ViewDirection));

	float specularShininess = 10.0f;

	if(specular > 0.0f)
		specular = pow(specular, specularShininess);

	return specular * 0.8f;
}

float glitter_specular(vec3 position, float specular)
{
	vec3 fp = fract(0.7 * position + 3 * noise(position * 0.04).r + 0.1 * Camera.ViewDirection);
	fp *= (1 - fp);
	float glitter = 1 - (fp.x + fp.y + fp.z);
	return glitter * pow(specular, 7);
}

vec3 calc_color()
{
	vec3 color = Lambert(Position, Normal);
	color += glitter_specular(Position, ocean_specular(Position, Normal));

	return color;
}

void main()
{
   FragColor = vec4(calc_color(), 1.0);
}
