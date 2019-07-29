#include <iostream>
#include <fstream>
#include "Vec3.h"
#include "Ray.h"

using namespace std;

bool hitSphere(const Vec3& center, float radius, const ray& r) {
	Vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0 * dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	return (discriminant > 0);
}

Vec3 color(const ray& r) {
	if (hitSphere(Vec3(0, 0, -1), 0.5, r))
		return Vec3(1, 0, 0);
	Vec3 unitDirection = unitVector(r.direction());
	float t = 0.5*(unitDirection.y() + 1.0);
	return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
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

	for (int i = ny - 1; i >= 0; i--) {
		for (int j = 0; j < nx; j++) {
			float u = float(j) / float(nx);
			float v = float(i) / float(ny);
			ray r(origin, lowerLeftCorner + u * horizontal + v * vertical);
			Vec3 col = color(r);
			int ir = int(255.99*col.r());
			int ig = int(255.99*col.g());
			int ib = int(255.99*col.b());

			output << ir << " " << ig << " " << ib << "\n";
		}
	}
	output.close();
	return 0;
}