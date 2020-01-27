#pragma once
#ifndef RAY_H
#define RAY_H
#include "Vec3.h"

// A simple class for defining each ray that is projected from the camera
class Ray 
{
public:
	Ray() = default;
	Ray(const Vec3& a, const Vec3& b) { vecA = a; vecB = b; }
	Vec3 Origin() const { return vecA; }
	Vec3 Direction() const { return vecB; }
	Vec3 PointAtParameter(const float t) const { return vecA + t * vecB; }

	Vec3 vecA;
	Vec3 vecB;
};
#endif // !RAY_H

