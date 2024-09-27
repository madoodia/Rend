// --------------------- //
// (C) 2024 madoodia.com //
// --------------------- //

#include <iostream>
#include <stdio.h>

int main(int, char**)
{

	int width = 256;
	int height = 256;

	std::cout << "P3\n" << width << ' ' << height << "\n255\n";

	for (int i = 0; i < height; i++)
	{
		std::clog << "\rScanlines remaining: " << (height - i) << ' ' << std::flush;
		for (int j = 0; j < width; j++)
		{
			float r = float(j) / (width - 1);
			float g = float(i) / (height - 1);
			float b = 0.25;

			int ir = int(255.999 * r);
			int ig = int(255.999 * g);
			int ib = int(255.999 * b);

			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}

	return 0;
}
