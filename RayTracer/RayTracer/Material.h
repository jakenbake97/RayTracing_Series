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

bool refract(const Vec3& v, const Vec3& n, float niOverNt, Vec3& refracted) {
	Vec3 uv = unitVector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0 - niOverNt * niOverNt * (1 - dt * dt);
	if (discriminant > 0)
	{
		refracted = niOverNt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else
		return false;
}

float schlick(float cosine, float refIdx) {
	float r0 = (1 - refIdx) / (1 + refIdx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

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
	Metal(const Vec3& a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
	virtual bool scatter(const ray& rIn, const hitRecord& rec, Vec3& attenuation, ray& scattered) const {
		Vec3 reflected = reflect(unitVector(rIn.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * randomInUnitSphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
	Vec3 albedo;
	float fuzz;
};

class Dielectric : public Material {
public:
	Dielectric(float ri) : refIdx(ri) {}
	virtual bool scatter(const ray& rIn, const hitRecord& rec, Vec3& attenuation, ray& scattered) const {
		Vec3 outwardNormal;
		Vec3 reflected = reflect(rIn.direction(), rec.normal);
		float niOverNt;
		attenuation = Vec3(1.0, 1.0, 1.0);
		Vec3 refracted;
		float reflectProb;
		float cosine;
		if (dot(rIn.direction(), rec.normal) > 0) {
			outwardNormal = -rec.normal;
			niOverNt = refIdx;
			//cosine = refIdx * dot(rIn.direction(), rec.normal) / rIn.direction().length();
			cosine = dot(rIn.direction(), rec.normal) / rIn.direction().length();
			cosine = sqrt(1 - refIdx * refIdx * (1 - cosine * cosine));
		}
		else {
			outwardNormal = rec.normal;
			niOverNt = 1.0 / refIdx;
			cosine = -dot(rIn.direction(), rec.normal) / rIn.direction().length();
		}

		if (refract(rIn.direction(), outwardNormal, niOverNt, refracted)) {
			reflectProb = schlick(cosine, refIdx);
		}
		else {
			reflectProb = 1.0;
		}

		if (rand()/(RAND_MAX + 1.0) < reflectProb) {
			scattered = ray(rec.p, reflected);
		}
		else {
			scattered = ray(rec.p, refracted);
			return true;
		}
	}
	float refIdx;
};

#endif