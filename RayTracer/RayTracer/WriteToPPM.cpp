#include <iostream>
#include <fstream>
#include <float.h>
#include <string>

#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"
#include "Material.h"
#include "Timer.h"

using namespace std;

double DRand()
{
	return rand() / (RAND_MAX + 1.0);
}

Vec3 Color(const Ray& r, Hitable* world, const int depth) {
	HitRecord rec;
	if (world->Hit(r, 0.001, FLT_MAX, rec)) {
		Ray scattered;
		Vec3 attenuation;
		if (depth < 50 && rec.matPtr->Scatter(r, rec, attenuation, scattered)) {
			return attenuation * Color(scattered, world, depth + 1);
		}
		else {
			return Vec3{ 0, 0, 0 };
		}
	}
	else {
		const Vec3 unitDirection = UnitVector(r.Direction());
		const float t = 0.5 * (unitDirection.Y() + 1.0);
		return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
	}
}

Hitable* RandomScene() {
	const int n = 50000;
	const auto list = new Hitable * [n + 1];
	list[0] = new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(Vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -10; a < 10; a++) {
		for (int b = -10; b < 10; b++) {
			const float chooseMat = DRand();
			Vec3 center(a + 0.9 * DRand(), 0.2, b + 0.9 * DRand());
			if ((center - Vec3(4, 0.2, 0)).Length() > 0.9) {
				if (chooseMat < 0.8) { // diffuse
					list[i++] = new MovingSphere(center, center + Vec3(0, 0.5 * DRand(), 0), 0.0, 1.0, 0.2, 
						new Lambertian(Vec3(DRand() * DRand(),DRand() * DRand(), DRand() * DRand())));
				}
				else if (chooseMat < 0.95) { // metal
					list[i++] = new Sphere(center, 0.2,
						new Metal(Vec3(0.5 * (1 + DRand()), 0.5 * (1 + DRand()),
							0.5 * (1 + DRand())), 0.5 * DRand()));
				}
				else { //glass
					list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new Sphere(Vec3(0, 1, 0), 1.0, new Dielectric(1.5));
	list[i++] = new Sphere(Vec3(-4, 1, 0), 1.0, new Lambertian(Vec3(0.4, 0.2, 0.1)));
	list[i++] = new Sphere(Vec3(4, 1, 0), 1.0, new Metal(Vec3(0.7, 0.6, 0.5), 0.0));

	return new HitableList(list, i);
}

int main() {

	int nx = 853;
	int ny = 480;
	int ns = 32;

	Hitable* list[5];
	float R = cos(M_PI / 4);
	list[0] = new Sphere(Vec3(0, 0, -1), 0.5, new Lambertian(Vec3(0.1, 0.2, 0.5)));
	list[1] = new Sphere(Vec3(0, -100.5, -1), 100, new Lambertian(Vec3(0.8, 0.8, 0.0)));
	list[2] = new Sphere(Vec3(1, 0, -1), 0.5, new Metal(Vec3(0.8, 0.6, 0.2), 0.0));
	list[3] = new Sphere(Vec3(-1, 0, -1), 0.5, new Dielectric(1.5));
	list[4] = new Sphere(Vec3(-1, 0, -1), -0.45, new Dielectric(1.5));
	Hitable* world = new HitableList(list, 5);
	world = RandomScene();

	const Vec3 lookAt(0, 0, 0);
	const float distToFocus = 10.0;
	const float aperture = 0.0;
	const float radian = 7.5f * 0.0174532925;

	ofstream output;
	Timer timer;
	const Vec3 lookFrom(13.0f, 2.0f, 13.0f);
	string fileName = "output.ppm";
	output.open(fileName);
	output << "P3\n" << nx << " " << ny << "\n255\n";
	Camera cam(lookFrom, lookAt, Vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, distToFocus, 0.0, 1.0);

	for (int i = ny - 1; i >= 0; i--) {
		for (int j = 0; j < nx; j++) {
			Vec3 col(0, 0, 0);
			for (int k = 0; k < ns; k++)
			{
				float u = float(j + DRand()) / float(nx);
				float v = float(i + DRand()) / float(ny);
				Ray r = cam.GetRay(u, v);
				Vec3 p = r.PointAtParameter(2.0);
				col += Color(r, world, 0);
			}
			col /= float(ns);
			col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			output << ir << " " << ig << " " << ib << "\n";
		}
	}
	cout << "File: complete in " << timer.GetDuration() << "\n";
	output.close();
	return 0;
}