/* (C) 2026 madoodia.com */

#include "ray.h"
#include "utils.h"
#include "global.h"

int main(int, char**)
{

	V3 cameraPosition = V3f(0.0f, 8.0f, -1.0f);
	V3 cameraZ = NOZ(cameraPosition);
	V3 cameraX = NOZ(Cross(cameraZ, V3f(0.0f, 0.0f, 1.0f)));
	V3 cameraY = NOZ(Cross(cameraZ, cameraX));
	// f32 Fov = 90.0f;

	Material materials[3] = {};
	materials[0].color = V3f(0.2f, 0.4f, 0.6f);
	materials[1].color = V3f(0.3f, 0.25f, 0.2f);
	materials[2].color = V3f(0.0f, 0.0f, 0.5f);

	Plane ground = {};
	ground.normal = V3f(0.0f, 0.0f, 1.0f);
	ground.distance = 0.0f;
	ground.MaterialIndex = 1;

	Sphere sphere = {};
	sphere.center = V3f(0.0f, 0.0f,-1.0f);
	sphere.radius = 1.0f;
	sphere.MaterialIndex = 2;

	World world = {};
	world.materials = materials;
	world.materialCount = 3;
	world.planes = &ground;
	world.planeCount = 1;
	world.spheres = &sphere;
	world.sphereCount = 1;

	ImageBuffer image = AllocateImage(1920, 1080);

	u32* finalOutput = image.pixels;
	f32 filmDistance = 1.0f;
	f32 aspectRatio = (f32)image.width / (f32)image.height;
	f32 filmWidth = 1.0f;
	f32 filmHeight = 1.0f;
	filmWidth = filmHeight * aspectRatio;
	filmHeight = filmWidth / aspectRatio;
	f32 halfFilmWidth = filmWidth * 0.5f;
	f32 halfFilmHeight = filmHeight * 0.5f;
	V3 filmCenter = cameraPosition - cameraZ * filmDistance;

	{
		TimeStamp timer("Rendering");
		for (u32 y = 0;
			 y < image.height;
			 ++y)
		{
			f32 filmY = -1.0f + 2.0f * ((f32)y / (f32)image.height);
			for (u32 x = 0;
				 x < image.width;
				 ++x)
			{
				f32 filmX = -1.0f + 2.0f * ((f32)x / (f32)image.width);
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

	return 0;
}
