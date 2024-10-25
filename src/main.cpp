// --------------------- //
// (C) 2024 madoodia.com //
// --------------------- //

#include "Eigen/Dense"

#include <iostream>
#include <stdio.h>

int main(int, char**)
{
	int Width = 512;
	int Height = 512;

	std::cout << "P3\n" << Width << ' ' << Height << "\n255\n";

	for (int i = 0; i < Height; i++)
	{
		std::clog << "\rScanlines remaining: " << (Height - i) << ' ' << std::flush;
		for (int j = 0; j < Width; j++)
		{
			float r = float(j) / (Width - 1);
			float g = float(i) / (Height - 1);
			float b = 0.0;

			int ir = int(255.999 * r);
			int ig = int(255.999 * g);
			int ib = int(255.999 * b);

			std::cout << ir << " " << ig << " " << ib <<  "\n";
		}
	}

	return 0;
}

// Based on DOD Paradigm
// TODO: Test Writing to disk with buffer when you have a struct of colors
// TODO: Implement Vector3 struct and implement related functions in DOD style
// TODO: Implement Ray struct and implement related functions in DOD style
// TODO: Consider Single Thread and Multi Threaded Ray Tracing thrughout the project

