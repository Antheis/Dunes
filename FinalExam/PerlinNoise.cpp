#include "PerlinNoise.h"

#include <algorithm>
#include <chrono>
#include <numeric>
#include <random>

PerlinNoise::PerlinNoise()
{
	//Generating ramdom permutation
	std::iota(new_permutation.begin(), new_permutation.end(), 0);

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(new_permutation.begin(), new_permutation.end(), std::mt19937{ seed });

	for (int i = 0; i < new_permutation.size(); ++i)
	{
		//pts[i] = permutation[i];
		pts[i] = new_permutation[i];
		pts[256 + i] = pts[i];
	}
}

double PerlinNoise::noise(float x, float y, float z)
{
	int X = static_cast<int>(floor(x)) & 255;
	int Y = static_cast<int>(floor(y)) & 255;
	int Z = static_cast<int>(floor(z)) & 255;
	x -= floor(x);
	y -= floor(y);
	z -= floor(z);
	double u = fade(x);
	double v = fade(y);
	double w = fade(z);
	int A = pts[X] + Y, AA = pts[A] + Z, AB = pts[A + 1] + Z,
		B = pts[X + 1] + Y, BA = pts[B] + Z, BB = pts[B + 1] + Z;

	return lerp(w, lerp(v, lerp(u, grad(pts[AA], x, y, z),
		grad(pts[BA], x - 1, y, z)),
		lerp(u, grad(pts[AB], x, y - 1, z),
			grad(pts[BB], x - 1, y - 1, z))),
		lerp(v, lerp(u, grad(pts[AA + 1], x, y, z - 1),
			grad(pts[BA + 1], x - 1, y, z - 1)),
			lerp(u, grad(pts[AB + 1], x, y - 1, z - 1),
				grad(pts[BB + 1], x - 1, y - 1, z - 1))));
}

inline double PerlinNoise::fade(double t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

inline double PerlinNoise::lerp(double t, double a, double b)
{
	return a + t * (b - a);
}

inline double PerlinNoise::grad(int hash, double x, double y, double z)
{
	int h = hash & 15;
	double u = h < 8 ? x : y;
	double v = h < 4 ? y : h == 12 || h == 14 ? x : z;

	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}
