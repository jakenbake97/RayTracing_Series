#pragma once
#ifndef CAMERAH
#define CAMERAH

#include "Ray.h"

class Camera {
public:
	Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 vUp, float vFov, float aspect) { // vFov is top to bottom in degrees
		Vec3 u, v, w;
		float theta = vFov * M_PI / 180;
		float halfHeight = tan(theta / 2);
		float halfWidth = aspect * halfHeight;
		origin = lookFrom;
		w = unitVector(lookFrom - lookAt);
		u = unitVector(cross(vUp, w));
		v = cross(w, u);
		lowerLeftCorner = Vec3(-halfWidth, -halfHeight, -1.0);
		lowerLeftCorner = origin - halfWidth * u - halfHeight * v - w;
		horizontal = 2 * halfWidth * u;
		vertical = 2 * halfHeight * v;
	}
	ray getRay(float s, float t) { return ray(origin, lowerLeftCorner + s * horizontal + t * vertical - origin); }

	Vec3 origin;
	Vec3 lowerLeftCorner;
	Vec3 horizontal;
	Vec3 vertical;
};

#endif // !CAMERAH
