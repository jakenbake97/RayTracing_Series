#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "Hitable.h"

// A hitable object in the shape of a sphere
class Sphere : public Hitable {
public:
	Sphere() = default;
	Sphere(const Vec3 cen, float r, Material* m) : center(cen), radius(r), matPtr(m) {};
	bool Hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const override;
	Vec3 center;
	float radius;
	Material* matPtr;
};

// Check to see if the sphere was hit
bool Sphere::Hit(const Ray& r, const float tMin, const float tMax, HitRecord& rec) const {
	const Vec3 oc = r.Origin() - center;
	const float a = Dot(r.Direction(), r.Direction());
	const float b = Dot(oc, r.Direction());
	const float c = Dot(oc, oc) - radius * radius;
	const float discriminant = b * b - a * c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(b * b - a * c)) / a;
		if (temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.p = r.PointAtParameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.matPtr = matPtr;
			return true;
		}
		temp = (-b + sqrt(b * b - a * c)) / a;
		if (temp < tMax && temp > tMin)
		{
			rec.t = temp;
			rec.p = r.PointAtParameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.matPtr = matPtr;
			return true;
		}
	}
	return false;
}
#endif // !SPHERE_H
