#pragma once
#ifndef MATERIALH
#define MATERIALH

#include "Ray.h"
#include "Hitable.h"
#include "Texture.h"

Vec3 RandomInUnitSphere() {
	Vec3 p;
	do {
		p = 2.0 * Vec3(rand() / (RAND_MAX + 1.0), rand() / (RAND_MAX + 1.0), rand() / (RAND_MAX + 1.0)) - Vec3(1, 1, 1);
	} while (p.SquaredLength() >= 1.0);
	return p;
}

inline Vec3 Reflect(const Vec3& v, const Vec3& n) {
	return v - 2 * Dot(v, n) * n;
}

//The base class for the appearance of all objects in the scene
class Material {
public:
	virtual bool Scatter(const Ray& rIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const = 0;
};

bool Refract(const Vec3& v, const Vec3& n, const float niOverNt, Vec3& refracted) {
	const Vec3 uv = UnitVector(v);
	const float dt = Dot(uv, n);
	const float discriminant = 1.0 - niOverNt * niOverNt * (1 - dt * dt);
	if (discriminant > 0)
	{
		refracted = niOverNt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else
		return false;
}

float Schlick(const float cosine, const float refIdx) {
	float r0 = (1 - refIdx) / (1 + refIdx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

// Controls the appearance of diffuse objects using the lambertian lighting model
class Lambertian : public Material {
public:
	Lambertian(Texture* a) : albedo(a) {}

	virtual bool Scatter(const Ray& rIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const
	{
		Vec3 target = rec.p + rec.normal + RandomInUnitSphere();
		scattered = Ray(rec.p, target - rec.p);
		attenuation = albedo->Value(rec.u, rec.v, rec.p);
		return true;
	}

	Texture* albedo;
};

// Controls the appearance of reflective objects
class Metal : public Material {
public:
	Metal(const Vec3& a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }

	bool Scatter(const Ray& rIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const override
	{
		const Vec3 reflected = Reflect(UnitVector(rIn.Direction()), rec.normal);
		scattered = Ray(rec.p, reflected + fuzz * RandomInUnitSphere());
		attenuation = albedo;
		return (Dot(scattered.Direction(), rec.normal) > 0);
	}
	Vec3 albedo;
	float fuzz;
};

// used for transparent materials such as the glass orbs
class Dielectric : public Material {
public:
	Dielectric(const float ri) : refIdx(ri) {}

	bool Scatter(const Ray& rIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const override
	{
		Vec3 outwardNormal;
		const Vec3 reflected = Reflect(rIn.Direction(), rec.normal);
		float niOverNt;
		attenuation = Vec3(1.0, 1.0, 1.0);
		Vec3 refracted;
		float reflectProb;
		float cosine;
		if (Dot(rIn.Direction(), rec.normal) > 0) {
			outwardNormal = -rec.normal;
			niOverNt = refIdx;
			//cosine = refIdx * dot(rIn.direction(), rec.normal) / rIn.direction().length();
			cosine = Dot(rIn.Direction(), rec.normal) / rIn.Direction().Length();
			cosine = sqrt(1 - refIdx * refIdx * (1 - cosine * cosine));
		}
		else {
			outwardNormal = rec.normal;
			niOverNt = 1.0 / refIdx;
			cosine = -Dot(rIn.Direction(), rec.normal) / rIn.Direction().Length();
		}

		if (Refract(rIn.Direction(), outwardNormal, niOverNt, refracted)) {
			reflectProb = Schlick(cosine, refIdx);
		}
		else {
			reflectProb = 1.0;
		}

		if (rand() / (RAND_MAX + 1.0) < reflectProb) {
			scattered = Ray(rec.p, reflected);
		}
		else {
			scattered = Ray(rec.p, refracted);
			return true;
		}
		return false;
	}
	float refIdx;
};

#endif