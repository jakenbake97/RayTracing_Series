#include <iostream>
#include <fstream>
#include "Sphere.h"
#include "HitableList.h"
#include <float.h>
#include "Camera.h"
#include "Material.h"

using namespace std;

Vec3 color(const ray& r, Hitable *world, int depth) {
	hitRecord rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		ray scattered;
		Vec3 attenuation;
		if (depth < 50 && rec.matPtr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return Vec3(0, 0, 0);
		}
	}
	else {
		Vec3 unitDirection = unitVector(r.direction());
		float t = 0.5 * (unitDirection.y() + 1.0);
		return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
	}
}

int main() {
	ofstream output;
	output.open("output.ppm");
	int nx = 200;
	int ny = 100;
	int ns = 100;
	output << "P3\n" << nx << " " << ny << "\n255\n";

	Camera cam(Vec3(-2, 2, 1), Vec3(0, 0, -1), Vec3(0, 1, 0), 45, float(nx) / float(ny));
	Hitable *list[5];
	float R = cos(M_PI / 4);
	list[0] = new Sphere(Vec3(0, 0, -1), 0.5, new Lambertian(Vec3(0.1, 0.2, 0.5)));
	list[1] = new Sphere(Vec3(0, -100.5, -1), 100, new Lambertian(Vec3(0.8, 0.8, 0.0)));
	list[2] = new Sphere(Vec3(1, 0, -1), 0.5, new Metal(Vec3(0.8, 0.6, 0.2), 0.0));
	list[3] = new Sphere(Vec3(-1, 0, -1), 0.5, new Dielectric(1.5));
	list[4] = new Sphere(Vec3(-1, 0, -1), -0.45, new Dielectric(1.5));
	Hitable *world = new HitableList(list, 5);
	

	for (int i = ny - 1; i >= 0; i--) {
		for (int j = 0; j < nx; j++) {
			Vec3 col(0, 0, 0);
			for (int k = 0; k < ns; k++)
			{
				float u = float(j + (rand() / (RAND_MAX + 1.0)))/ float(nx);
				float v = float(i + (rand() / (RAND_MAX + 1.0)))/ float(ny);
				ray r = cam.getRay(u, v);
				Vec3 p = r.point_at_parameter(2.0);
				col += color(r, world, 0);
			}
			col /= float(ns);
			col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			output << ir << " " << ig << " " << ib << "\n";
		}
	}
	output.close();
	return 0;
}