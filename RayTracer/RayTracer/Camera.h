#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"

// returns a random position in the Unit circle (or disk) around the origin
inline Vec3 RandomInUnitDisk() {
	Vec3 p;
	do {
		p = 2.0 * Vec3(rand() / (RAND_MAX + 1.0), rand() / (RAND_MAX + 1.0), 0) - Vec3(1, 1, 0);
	} while (Dot(p, p) >= 1.0);
	return p;
}

class Camera {
public:
	Camera(const Vec3 lookFrom, const Vec3 lookAt, const Vec3 vUp, const float vFov, const float aspect, const float aperture, const float focusDist) { // vFov is top to bottom in degrees
		lensRadius = aperture / 2;
		const float theta = vFov * M_PI / 180;
		const float halfHeight = tan(theta / 2);
		const float halfWidth = aspect * halfHeight;
		origin = lookFrom;
		w = UnitVector(lookFrom - lookAt);
		u = UnitVector(Cross(vUp, w));
		v = Cross(w, u);
		lowerLeftCorner = origin - halfWidth * focusDist * u - halfHeight * focusDist * v - focusDist * w;
		horizontal = 2 * halfWidth * focusDist * u;
		vertical = 2 * halfHeight * focusDist * v;
	}
	Ray GetRay(const float s, const float t) const
	{
		const Vec3 rd = lensRadius * RandomInUnitDisk();
		const Vec3 offset = u * rd.X() + v * rd.Y();
		return Ray{ origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset };
	}

	Vec3 origin;
	Vec3 lowerLeftCorner;
	Vec3 horizontal;
	Vec3 vertical;
	Vec3 u, v, w;
	float lensRadius;
};

#endif // !CAMERA_H
