#include <iostream>
#include <fstream>
#include "Sphere.h"
#include "HitableList.h"
#include <float.h>
#include "Camera.h"

using namespace std;

Vec3 color(const ray& r, Hitable *world) {
	hitRecord rec;
	if (world->hit(r, 0.0, FLT_MAX, rec)) {
		return 0.5 * Vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
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

	Hitable *list[2];
	list[0] = new Sphere(Vec3(0, 0, -1), 0.5);
	list[1] = new Sphere(Vec3(0, -100.5, -1), 100);
	Hitable *world = new HitableList(list, 2);
	Camera cam;

	for (int i = ny - 1; i >= 0; i--) {
		for (int j = 0; j < nx; j++) {
			Vec3 col(0, 0, 0);
			for (int k = 0; k < ns; k++)
			{
				float u = float(j + (rand() / (RAND_MAX + 1.0)))/ float(nx);
				float v = float(i + (rand() / (RAND_MAX + 1.0)))/ float(ny);
				ray r = cam.getRay(u, v);
				Vec3 p = r.point_at_parameter(2.0);
				col += color(r, world);
			}
			col /= float(ns);
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			output << ir << " " << ig << " " << ib << "\n";
		}
	}
	output.close();
	return 0;
}