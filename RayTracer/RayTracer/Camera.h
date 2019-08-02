#pragma once
#ifndef CAMERAH
#define CAMERAH

#include "Ray.h"

Vec3 randomInUnitDisk() {
	Vec3 p;
	do {
		p = 2.0 * Vec3(rand() / (RAND_MAX + 1.0), rand() / (RAND_MAX + 1.0), 0) - Vec3(1, 1, 0);
	} while (dot(p, p) >= 1.0);
	return p;
}

class Camera {
public:
	Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 vUp, float vFov, float aspect, float aperture, float focusDist) { // vFov is top to bottom in degrees
		lensRadius = aperture / 2;
		float theta = vFov * M_PI / 180;
		float halfHeight = tan(theta / 2);
		float halfWidth = aspect * halfHeight;
		origin = lookFrom;
		w = unitVector(lookFrom - lookAt);
		u = unitVector(cross(vUp, w));
		v = cross(w, u);
		lowerLeftCorner = origin - halfWidth * focusDist * u - halfHeight * focusDist * v - focusDist * w;
		horizontal = 2 * halfWidth * focusDist * u;
		vertical = 2 * halfHeight * focusDist * v;
	}
	ray getRay(float s, float t) { 
		Vec3 rd = lensRadius * randomInUnitDisk();
		Vec3 offset = u * rd.x() + v * rd.y();
		return ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset);
	}

	Vec3 origin;
	Vec3 lowerLeftCorner;
	Vec3 horizontal;
	Vec3 vertical;
	Vec3 u, v, w;
	float lensRadius;
};

#endif // !CAMERAH
