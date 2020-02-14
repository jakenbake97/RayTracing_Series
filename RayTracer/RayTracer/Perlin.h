#pragma once
#include "Vec3.h"
#include "Random.h"

inline float PerlinInterp(Vec3 c[2][2][2], float u, float v, float w)
{
	float uu = u * u * (3 - 2 * u);
	float vv = v * v * (3 - 2 * v);
	float ww = w * w * (3 - 2 * w);
	float accum = 0;
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
			{
				Vec3 weightV(u - i, v - j, w - k);
				accum += (i * uu + (1 - i) * (1 - uu)) * (j * vv + (1 - j) * (1 - vv)) * (k * ww + (1 - k) * (1 - ww)) * Dot(c[i][j][k], weightV);
			}
	return accum;
}

class Perlin
{
public:
	float noise(const Vec3& p) const
	{
		float u = p.X() - floor(p.X());
		float v = p.Y() - floor(p.Y());
		float w = p.Z() - floor(p.Z());
		u = u * u * (3 - 2 * u);
		v = v * v * (3 - 2 * v);
		w = w * w * (3 - 2 * w);
		int i = floor(p.X());
		int j = floor(p.Y());
		int k = floor(p.Z());
		Vec3 c[2][2][2];

		for (int di = 0; di < 2; di++)
			for (int dj = 0; dj < 2; dj++)
				for (int dk = 0; dk < 2; dk++)
					c[di][dj][dk] = ranVec[permX[(i + di) & 255] ^ permY[(j + dj) & 255] ^ permZ[(k + dk) & 255]];
		return  PerlinInterp(c, u, v, w);
	}

	float Turb(const Vec3& p, int depth = 7) const
	{
		float accum = 0;
		Vec3 tempP = p;
		float weight = 1.0f;
		for (int i = 0; i < depth; i++)
		{
			accum += weight * noise(tempP);
			weight *= 0.5f;
			tempP *= 2;
		}
		return fabs(accum);
	}

	static Vec3* ranVec;
	static int* permX;
	static int* permY;
	static int* permZ;
};

static Vec3* PerlinGenerate()
{
	Vec3* p = new Vec3[256];
	for (int i = 0; i < 256; ++i)
		p[i] = UnitVector(Vec3(-1 + 2 * DRand(), -1 + 2 * DRand(), -1 + 2 * DRand()));
	return p;
}

void Permute(int* p, int n)
{
	for (int i = n - 1; i > 0; i--)
	{
		int target = int(DRand() * (i + 1));
		int tmp = p[i];
		p[i] = p[target];
		p[target] = tmp;
	}
}

static int* PerlinGeneratePerm()
{
	int* p = new int[256];
	for (int i = 0; i < 256; i++)
		p[i] = i;
	Permute(p, 256);
	return p;
}

Vec3* Perlin::ranVec = PerlinGenerate();
int* Perlin::permX = PerlinGeneratePerm();
int* Perlin::permY = PerlinGeneratePerm();
int* Perlin::permZ = PerlinGeneratePerm();