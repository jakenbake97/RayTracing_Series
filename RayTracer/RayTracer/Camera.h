#pragma once
#ifndef CAMERAH
#define CAMERAH

#include "Ray.h"

class Camera {
public:
	Camera() {
		lowerLeftCorner = Vec3(-2.0, -1.0, -1.0);
		horizontal = Vec3(4.0, 0.0, 0.0);
		vertical = Vec3(0.0, 2.0, 0.0);
		origin = Vec3(0.0, 0.0, 0.0);
	}
	ray getRay(float u, float v) { return ray(origin, lowerLeftCorner + u * horizontal + v * vertical - origin); }

	Vec3 origin;
	Vec3 lowerLeftCorner;
	Vec3 horizontal;
	Vec3 vertical;
};

#endif // !CAMERAH