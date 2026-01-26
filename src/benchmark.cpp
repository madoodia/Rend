#include <stdio.h>

const static u32 SIZE = 1 << 20; // 67 million

struct Vertex // AOS
{
	f32 Position;
	f32 Color;
};

struct Point2 // SOA
{
	f32 x[SIZE];
	f32 y[SIZE];
};

int main()
{
	// Allocation Memory
	printf("Allocating memory for %u elements each...\n\n", SIZE);
	std::chrono::steady_clock::time_point StartTime = GetTimeStamp();
	Vertex* Vertices = new Vertex[SIZE]();
	std::chrono::steady_clock::time_point EndTime = GetTimeStamp();

	printf("Time taken to initialize Vertices: %lld nanoseconds\n",
		   std::chrono::duration_cast<std::chrono::nanoseconds>(EndTime - StartTime).count());

	StartTime = GetTimeStamp();
	Point2* Points = new Point2();
	EndTime = GetTimeStamp();

	printf("Time taken to initialize Point2: %lld nanoseconds\n",
		   std::chrono::duration_cast<std::chrono::nanoseconds>(EndTime - StartTime).count());

	// Setting Values
	printf("\nSetting values...\n");
	StartTime = GetTimeStamp();
	for (u32 i = 0; i < SIZE; ++i)
	{
		Vertices[i].Position = 1.0f;
		Vertices[i].Color = 1.0f;
	}
	EndTime = GetTimeStamp();

	printf("\nTime taken to set values in Vertices: %lld nanoseconds\n",
		   std::chrono::duration_cast<std::chrono::nanoseconds>(EndTime - StartTime).count());

	StartTime = GetTimeStamp();
	for (u32 i = 0; i < SIZE; ++i)
	{
		Points->x[i] = 1.0f;
		Points->y[i] = 1.0f;
	}
	EndTime = GetTimeStamp();
	printf("Time taken to set values in Points: %lld nanoseconds\n",
		   std::chrono::duration_cast<std::chrono::nanoseconds>(EndTime - StartTime).count());

	f32 result = 0.0f;
	// Computation
	printf("\nPerforming computations...\n");
	StartTime = GetTimeStamp();
	// AOS: Uses both fields - natural fit
	for (u32 i = 0; i < SIZE; ++i)
		result += Vertices[i].Position * 2.0f;
	EndTime = GetTimeStamp();
	printf("\nTime taken for computation on Vertices: %lld nanoseconds\n",
		   std::chrono::duration_cast<std::chrono::nanoseconds>(EndTime - StartTime).count());

	StartTime = GetTimeStamp();
	// SOA: Uses both fields - natural fit
	for (u32 i = 0; i < SIZE; ++i)
		result += Points->x[i] * 2.0f;
	EndTime = GetTimeStamp();
	printf("Time taken for computation on Points: %lld nanoseconds\n",
		   std::chrono::duration_cast<std::chrono::nanoseconds>(EndTime - StartTime).count());

	// std::getchar();

	delete[] Vertices;
	delete Points;
}
