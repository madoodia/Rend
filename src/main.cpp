/* (C) 2026 madoodia.com */

#include "ray.h"
#include "utils.h"
#include "global.h"

int main(int, char**)
{
	Material materials[5] = {};
	materials[0].emitColor = V3f(0.2f, 0.4f, 0.6f);
	materials[1].roughness = 0.35f;
	materials[1].refColor = V3f(0.7f, 0.7f, 0.7f);
	materials[2].roughness = 1.0f;
	materials[2].refColor = V3f(0.921f, 0.784f, 0.467f);
	materials[3].roughness = 1.0f;
	materials[3].emitColor = V3f(1.0f, 0.0f, 0.0f);
	materials[4].roughness = 0.1f;
	materials[4].refColor = V3f(0.64f, 0.7f, 0.921f);

	Plane planes[1] = {};
	planes[0].normal = V3f(0.0f, 0.0f, 1.0f);
	planes[0].distance = 0.0f;
	planes[0].matIndex = 1;

	Sphere spheres[3] = {};
	spheres[0].center = V3f(0.0f, 0.0f, 0.0f);
	spheres[0].radius = 1.0f;
	spheres[0].matIndex = 2;
	spheres[1].center = V3f(2.5f, -1.5f, 0.0f);
	spheres[1].radius = 1.0f;
	spheres[1].matIndex = 3;
	spheres[2].center = V3f(-2.0f, .5f, 2.0f);
	spheres[2].radius = 1.0f;
	spheres[2].matIndex = 4;

	World world = {};
	world.materials = materials;
	world.materialCount = ARRAY_COUNT(materials);
	world.planes = planes;
	world.planeCount = ARRAY_COUNT(planes);
	world.spheres = spheres;
	world.sphereCount = ARRAY_COUNT(spheres);
	ImageBuffer image = AllocateImage(1920, 1080);

	V3 cameraPosition = V3f(0.0f, -5.0f, 0.9f);
	V3 cameraZ = NOZ(cameraPosition);
	V3 cameraX = NOZ(Cross(V3f(0.0f, 0.0f, 1.0f), cameraZ));
	V3 cameraY = NOZ(Cross(cameraZ, cameraX));
	// f32 Fov = 90.0f;

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

		u32 rayPerPixel = 16;
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
				V3 color = {};
				f32 contribution = 1.0f / (f32)rayPerPixel;
				for (u32 rayIndex = 0;
					 rayIndex < rayPerPixel;
					 ++rayIndex)
				{
					V3 filmPoint = filmCenter + halfFilmWidth * cameraX * filmX + halfFilmHeight * cameraY * filmY;

					V3 rayOrigin = cameraPosition;
					V3 rayDirection = NOZ(filmPoint - cameraPosition);
					color += contribution * RayCast(&world, rayOrigin, rayDirection);
				}
				V4 bmpColor = V4f(255.0f * color, 255.0f);
				u32 bmpValue = BGRAPack4x8(bmpColor);

				*finalOutput++ = bmpValue;
			}
		}

		WriteImage(image, "output/final_render.bmp");
	}

	return 0;
}
