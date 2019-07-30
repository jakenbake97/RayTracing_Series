#include <iostream>
#include <fstream>
#include "Sphere.h"
#include "HitableList.h"
#include <float.h>

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
	output << "P3\n" << nx << " " << ny << "\n255\n";
	Vec3 lowerLeftCorner(-2.0, -1.0, -1.0);
	Vec3 horizontal(4.0, 0.0, 0.0);
	Vec3 vertical(0.0, 2.0, 0.0);
	Vec3 origin(0.0, 0.0, 0.0);
	Hitable *list[2];
	list[0] = new Sphere(Vec3(0, 0, -1), 0.5);
	list[1] = new Sphere(Vec3(0, -100.5, -1), 100);
	Hitable *world = new HitableList(list, 2);

	for (int i = ny - 1; i >= 0; i--) {
		for (int j = 0; j < nx; j++) {
			float u = float(j) / float(nx);
			float v = float(i) / float(ny);
			ray r(origin, lowerLeftCorner + u * horizontal + v * vertical);

			Vec3 p = r.point_at_parameter(2.0);
			Vec3 col = color(r, world);
			int ir = int(255.99*col.r());
			int ig = int(255.99*col.g());
			int ib = int(255.99*col.b());

			output << ir << " " << ig << " " << ib << "\n";
		}
	}
	output.close();
	return 0;
}