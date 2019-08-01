#pragma once
#ifndef HITABLEH
#define HITABLEH

#include "Ray.h"

class Material;

struct hitRecord {
	float t;
	Vec3 p;
	Vec3 normal;
	Material *matPtr;
};

class Hitable {
public:
	virtual bool hit(const ray& r, float tMin, float tMax, hitRecord& rec) const = 0;
};
#endif // !HITABLEH
