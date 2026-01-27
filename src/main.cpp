/* (C) 2026 madoodia.com */

#include "ray.h"
#include "utils.h"

int main(int, char**)
{
	V3 cameraPosition = V3f(0.0f, 10.0f, 1.0f);
	V3 cameraZ = NOZ(V3f(0.0f, 0.0f, 0.0f) - cameraPosition);
	V3 cameraX = NOZ(Cross(V3f(0.0f, 1.0f, 0.0f), cameraZ));
	V3 cameraY = NOZ(Cross(cameraZ, cameraX));
	// f32 Fov = 90.0f;

	Material materials[2] = {};
	materials[0].color = V3f(0.5f, 0.5f, 0.5f);
	materials[1].color = V3f(1.0f, 0.0f, 0.0f);

	Plane ground = {};
	ground.normal = V3f(0.0f, 1.0f, 0.0f);
	ground.distance = 0.0f;
	ground.MaterialIndex = 1;

	World world = {};
	world.materials = materials;
	world.materialCount = 2;
	world.planes = &ground;
	world.planeCount = 1;
	world.spheres = nullptr;
	world.sphereCount = 0;

	ImageBuffer image = AllocateImage(1280, 720);

	u32* finalOutput = image.pixels;

	f32 filmDistance = 1.0f;
	f32 filmWidth = 1.0f;
	f32 filmHeight = 1.0f;
	f32 halfFilmWidth = filmWidth * 0.5f;
	f32 halfFilmHeight = filmHeight * 0.5f;
	V3 filmCenter = cameraPosition - cameraZ * filmDistance;

	{
		TimeStamp timer("Rendering");
		for (u32 y = 0;
			 y < image.height;
			 ++y)
		{
			f32 filmY = -1.0f + 2.0f * (f32)y / (f32)image.height;
			for (u32 x = 0;
				 x < image.width;
				 ++x)
			{
				f32 filmX = -1.0f + 2.0f * (f32)x / (f32)image.width;
				V3 filmPoint = filmCenter + halfFilmWidth * cameraX * filmX + halfFilmHeight * cameraY * filmY;

				V3 rayOrigin = cameraPosition;
				V3 rayDirection = NOZ(filmPoint - cameraPosition);
				V3 color = RayCast(&world, rayOrigin, rayDirection);

				V4 bmpColor = V4f(255.0f * color, 255.0f);
				u32 bmpValue = BGRAPack4x8(bmpColor);

				*finalOutput++ = bmpValue;
			}
		}

		WriteImage(image, "output/final_render.bmp");
	}
	{
		TimeStamp Timer("Summation 1");
		int sum = 0;
		for (int i = 0; i < 1000000; i++)
		{
			sum += i;
		}
	}
	{
		TimeStamp Timer("Summation 2");
		int sum = 0;
		for (int i = 0; i < 1000000; i++)
		{
			sum += i;
		}
		printf("Sum: %d\n", sum);
	}

	return 0;
}
