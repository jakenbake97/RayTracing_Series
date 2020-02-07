#pragma once

#ifndef AABB_H
#define AABB_H
#include "Ray.h"

inline float FfMin(float a, float b) { return a < b ? a : b; }
inline float FfMax(float a, float b) { return a > b ? a : b; }

class AABB
{
public:
	AABB() = default;
	AABB(const Vec3& a, const Vec3& b) {min = a; max = b;}

	Vec3 Min() const {return min;}
	Vec3 Max() const {return max;}

	inline bool Hit(const Ray& r, float tMin, float tMax) const
	{
		for (int a = 0; a < 3; a++)
		{
			float invD = 1.0f / r.Direction()[a];
			float t0 = (Min()[a] - r.Origin()[a]) * invD;
			float t1 = (Max()[a] - r.Origin()[a]) * invD;

			if(invD < 0.0f)
				std::swap(t0, t1);
			tMin = t0 > tMin ? t0 : tMin;
			tMax = t1 < tMax ? t1 : tMax;
			if (tMax <= tMin)
				return false;
		}
		return true;
	}

	Vec3 min;
	Vec3 max;
};

AABB SurroundingBox(AABB box0, AABB box1)
{
	Vec3 small( FfMin(box0.Min().X(), box1.Min().X()),
                FfMin(box0.Min().Y(), box1.Min().Y()),
                FfMin(box0.Min().Z(), box1.Min().Z()));
    Vec3 big  ( FfMax(box0.Max().X(), box1.Max().X()),
                FfMax(box0.Max().Y(), box1.Max().Y()),
                FfMax(box0.Max().Z(), box1.Max().Z()));
    return AABB(small,big);
}

#endif
