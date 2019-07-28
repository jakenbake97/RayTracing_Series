#include <iostream>
#include <fstream>
#include "Vec3.h"

using namespace std;

int main() {
	ofstream output;
	output.open("output.ppm");
	int nx = 200;
	int ny = 100;
	output << "P3\n" << nx << " " << ny << "\n255\n";

	for (int i = ny - 1; i >= 0; i--) {
		for (int j = 0; j < nx; j++) {
			Vec3 col(float(j) / float(nx), float(i) / float(ny), 0.2);
			int ir = int(255.99*col.r());
			int ig = int(255.99*col.g());
			int ib = int(255.99*col.b());

			output << ir << " " << ig << " " << ib << "\n";
		}
	}
	output.close();
	return 0;
}