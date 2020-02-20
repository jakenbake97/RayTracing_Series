#pragma once
#ifndef HITABLE_H
#define HITABLE_H

#include "Ray.h"
#include "AABB.h"


class Material;

void GetSphereUV(const Vec3& p, float& u, float& v)
{
	float phi = atan2(p.Z(), p.X());
	float theta = asin(p.Y());
	u = 1 - (phi + M_PI) / (2 * M_PI);
	v = (theta + M_PI / 2) / M_PI;
}

struct HitRecord {
	float t;
	float u;
	float v;
	Vec3 p;
	Vec3 normal;
	Material* matPtr;
};

class Hitable {
public:
	virtual bool Hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const = 0;
	virtual bool BoundingBox(float t0, float t1, AABB& box) const = 0;
};
#endif // !HITABLE_H
