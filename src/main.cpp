/* (C) 2026 madoodia.com */

#include "ray.h"
#include "utils.h"
#include "global.h"

int main(int, char**)
{
	Material materials[6] = {};
	materials[0].emitColor = V3f(0.2f, 0.4f, 0.6f);
	materials[1].roughness = 0.5f;
	materials[1].refColor = V3f(0.4f, 0.4f, 0.4f);
	materials[2].roughness = 1.0f;
	materials[2].refColor = V3f(0.921f, 0.784f, 0.467f);
	materials[3].roughness = 1.0f;
	materials[3].emitColor = V3f(1.0f, 0.0f, 0.0f);
	materials[4].roughness = 0.1f;
	materials[4].refColor = V3f(0.64f, 0.7f, 0.921f);
	materials[5].roughness = 1.0f;
	materials[5].refColor = V3f(0.95f, 0.95f, 0.95f);

	Plane planes[1] = {};
	planes[0].normal = V3f(0.0f, 0.0f, 1.0f);
	planes[0].distance = 0.0f;
	planes[0].matIndex = 1;

	Sphere spheres[4] = {};
	spheres[0].center = V3f(0.0f, 0.0f, 0.0f);
	spheres[0].radius = 1.0f;
	spheres[0].matIndex = 2;
	spheres[1].center = V3f(2.5f, -1.5f, 0.0f);
	spheres[1].radius = 1.0f;
	spheres[1].matIndex = 3;
	spheres[2].center = V3f(-2.0f, .5f, 2.0f);
	spheres[2].radius = 1.0f;
	spheres[2].matIndex = 4;
	spheres[3].center = V3f(-3.0f, 3.0f, 1.0f);
	spheres[3].radius = 2.0f;
	spheres[3].matIndex = 5;

	World world = {};
	world.materials = materials;
	world.materialCount = ARRAY_COUNT(materials);
	world.planes = planes;
	world.planeCount = ARRAY_COUNT(planes);
	world.spheres = spheres;
	world.sphereCount = ARRAY_COUNT(spheres);
	ImageBuffer image = AllocateImage(1920, 1080);

	V3 cameraPosition = V3f(0.0f, -10.0f, 1.0f);
	V3 cameraZ = NOZ(cameraPosition);
	V3 cameraX = NOZ(Cross(V3f(0.0f, 0.0f, 1.0f), cameraZ));
	V3 cameraY = NOZ(Cross(cameraZ, cameraX));
	// f32 Fov = 90.0f;

	f32 filmDistance = 1.0f;
	f32 filmWidth = 1.0f;
	f32 filmHeight = 1.0f;
	if (image.width > image.height)
		filmHeight = filmWidth * (f32)image.height / (f32)image.width;
	else if (image.height > image.width)
		filmWidth = filmHeight * (f32)image.width / (f32)image.height;
	f32 halfFilmWidth = filmWidth * 0.5f;
	f32 halfFilmHeight = filmHeight * 0.5f;
	V3 filmCenter = cameraPosition - cameraZ * filmDistance;
	f32 halfPixW = 0.5f / image.width;
	f32 halfPixH = 0.5f / image.height;

	u32 raysPerPixel = 16;
	u32* finalOutput = image.pixels;

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

				V3 color = {};
				f32 contribution = 1.0f / (f32)raysPerPixel;
				for (u32 rayIndex = 0;
					 rayIndex < raysPerPixel;
					 ++rayIndex)
				{
					f32 offX = filmX + halfPixW * RandomBiF32();
					f32 offY = filmY + halfPixH * RandomBiF32();
					V3 filmPoint = filmCenter + offX * halfFilmWidth * cameraX + offY * halfFilmHeight * cameraY;

					V3 rayOrigin = cameraPosition;
					V3 rayDirection = NOZ(filmPoint - cameraPosition);
					color += contribution * RayCast(&world, rayOrigin, rayDirection);
				}
				V4 bmpColor = {
					255.0f * LinearToRGB(color.r),
					255.0f * LinearToRGB(color.g),
					255.0f * LinearToRGB(color.b),
					255.0f};

				u32 bmpValue = BGRAPack4x8(bmpColor);

				*finalOutput++ = bmpValue;
			}
		}

		WriteImage(image, "output/final_render.bmp");
	}

	return 0;
}
