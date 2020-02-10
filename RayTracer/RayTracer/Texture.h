#pragma once
#include "Vec3.h"

class Texture
{
public:
	virtual Vec3 Value(float u, float v, const Vec3& p) const = 0;
};

class ConstantTexture : public Texture
{
public:
	ConstantTexture() {}
	ConstantTexture(Vec3 c) : color(c) {}
	virtual Vec3 Value(float u, float v, const Vec3& p) const { return color;}
	Vec3 color;
};

class CheckerTexture : public Texture
{
public:
	CheckerTexture() {}
	CheckerTexture(Texture* t0, Texture* t1) : even(t0), odd(t1) {}
	virtual Vec3 Value(float u, float v, const Vec3& p) const
	{
		float sines = sin(10 * p.X()) * sin(10 * p.Y()) * sin(10 * p.Z());
		if (sines < 0)
			return odd->Value(u, v, p);
		else
			return even->Value(u, v, p);
	}
	Texture* odd;
	Texture* even;
};
