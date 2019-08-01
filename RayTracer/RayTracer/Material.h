#pragma once
#ifndef MATERIALH
#define MATERIALH

#include "Ray.h"
#include "Hitable.h"

Vec3 randomInUnitSphere() {
	Vec3 p;
	do {
		p = 2.0 * Vec3(rand() / (RAND_MAX + 1.0), rand() / (RAND_MAX + 1.0), rand() / (RAND_MAX + 1.0)) - Vec3(1, 1, 1);
	} while (p.squaredLength() >= 1.0);
	return p;
}

Vec3 reflect(const Vec3& v, const Vec3& n) {
	return v - 2 * dot(v, n) * n;
}

class Material {
public:
	virtual bool scatter(const ray& rIn, const hitRecord& rec, Vec3& attenuation, ray& scattered) const = 0;
};

class Lambertian : public Material {
public:
	Lambertian(const Vec3& a) : albedo(a) {}
	virtual bool scatter(const ray& rIn, const hitRecord& rec, Vec3& attenuation, ray& scattered) const {
		Vec3 target = rec.p + rec.normal + randomInUnitSphere();
		scattered = ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}

	Vec3 albedo;
};

class Metal : public Material {
public:
	Metal(const Vec3& a) : albedo(a) {}
	virtual bool scatter(const ray& rIn, const hitRecord& rec, Vec3& attenuation, ray& scattered) const {
		Vec3 reflected = reflect(unitVector(rIn.direction()), rec.normal);
		scattered = ray(rec.p, reflected);
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
	Vec3 albedo;
};

#endif