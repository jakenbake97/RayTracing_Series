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
	bool BoundingBox(float t0, float t1, AABB& box) const override;
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

bool Sphere::BoundingBox(float t0, float t1, AABB& box) const
{
	box = AABB(center - Vec3(radius, radius, radius), center + Vec3(radius, radius, radius));
	return true;
}

class MovingSphere: public Hitable
{
public:
	MovingSphere(){}
	MovingSphere(Vec3 cen0, Vec3 cen1, float t0, float t1, float r, Material* m)
		: center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), matPtr(m) {};
	virtual bool Hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;
	virtual bool BoundingBox(float t0, float t1, AABB& box)const;
	Vec3 Center(float time) const;
	Vec3 center0, center1;
	float time0, time1;
	float radius;
	Material* matPtr;
};

Vec3 MovingSphere::Center(float time) const
{
	return center0 + (time - time0) / (time1 - time0) * (center1 - center0);
}

bool MovingSphere::Hit(const Ray& r, const float tMin, const float tMax, HitRecord& rec) const {
	const Vec3 oc = r.Origin() - Center(r.Time());
	const float a = Dot(r.Direction(), r.Direction());
	const float b = Dot(oc, r.Direction());
	const float c = Dot(oc, oc) - radius * radius;
	const float discriminant = b * b - a * c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(b * b - a * c)) / a;
		if (temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.p = r.PointAtParameter(rec.t);
			rec.normal = (rec.p - Center(r.Time())) / radius;
			rec.matPtr = matPtr;
			return true;
		}
		temp = (-b + sqrt(b * b - a * c)) / a;
		if (temp < tMax && temp > tMin)
		{
			rec.t = temp;
			rec.p = r.PointAtParameter(rec.t);
			rec.normal = (rec.p - Center(r.Time())) / radius;
			rec.matPtr = matPtr;
			return true;
		}
	}
	return false;
}

bool MovingSphere::BoundingBox(float t0, float t1, AABB& box) const
{
        AABB box0(Center(t0) - Vec3(radius, radius, radius), Center(t0) + Vec3(radius, radius, radius));
        AABB box1(Center(t1) - Vec3(radius, radius, radius), Center(t1) + Vec3(radius, radius, radius));
        box = SurroundingBox(box0, box1);
        return true;
}
#endif // !SPHERE_H
