/* (C) 2026 madoodia.com */

#include <time.h>

#include "ray.h"
#include "utils.h"
#include "global.h"

internal void
RenderTile(WorkQueue* workQueue)
{
	u64 workOrderIndex = workQueue->nextWorkOrderIndex++;
	if (workOrderIndex >= workQueue->workOrderCount)
	{
		return;
	}

	WorkOrder* workOrder = workQueue->workOrders + workOrderIndex;
	World* world = workOrder->world;
	ImageBuffer image = workOrder->image;
	u32 minX = workOrder->minX;
	u32 minY = workOrder->minY;
	u32 maxX = workOrder->maxX;
	u32 maxY = workOrder->maxY;

	V3 cameraPosition = V3f(0.0f, -10.0f, 1.2f);
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
	f32 contribution = 1.0f / (f32)raysPerPixel;

	for (u32 y = minY;
		 y < maxY;
		 ++y)
	{
		u32* finalOutput = GetPixelPointer(image, minX, y);
		f32 filmY = -1.0f + 2.0f * ((f32)y / (f32)image.height);
		for (u32 x = minX;
			 x < maxX;
			 ++x)
		{
			V3 sample = {};
			V3 finalColor = {};

			f32 filmX = -1.0f + 2.0f * ((f32)x / (f32)image.width);

			for (u32 rayIndex = 0;
				 rayIndex < raysPerPixel;
				 ++rayIndex)
			{
				f32 offX = filmX + halfPixW * RandomBiF32();
				f32 offY = filmY + halfPixH * RandomBiF32();
				V3 filmPoint = filmCenter + offX * halfFilmWidth * cameraX + offY * halfFilmHeight * cameraY;

				V3 rayOrigin = cameraPosition;
				V3 rayDirection = NOZ(filmPoint - cameraPosition);
				sample += RayCast(workQueue, world, rayOrigin, rayDirection, contribution);
			}
			finalColor += contribution * sample;
			V4 bmpColor = {
				255.0f * LinearToRGB(finalColor.r),
				255.0f * LinearToRGB(finalColor.g),
				255.0f * LinearToRGB(finalColor.b),
				255.0f};

			u32 bmpValue = BGRAPack4x8(bmpColor);
			*finalOutput++ = bmpValue;
		}
	}
	++workQueue->tilesRenderedCount;
}

int main(int, char**)
{
	Material materials[6] = {};
	materials[0].emitColor = V3f(0.2f, 0.4f, 0.6f);
	materials[1].scatter = 1.0f;
	materials[1].refColor = V3f(0.4f, 0.4f, 0.4f);
	materials[2].scatter = 1.0f;
	materials[2].refColor = V3f(0.921f, 0.784f, 0.467f);
	materials[3].scatter = 1.0f;
	materials[3].emitColor = V3f(2.2f, 0.0f, 0.0f);
	materials[4].scatter = .25f;
	materials[4].refColor = V3f(0.64f, 0.7f, 0.921f);
	materials[5].scatter = .1f;
	materials[5].refColor = V3f(0.7f, 0.21f, 0.75f);

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
	spheres[2].center = V3f(-1.0f, -0.5f, 2.0f);
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
	ImageBuffer image = AllocateImage(1280, 720);

	clock_t startTime = clock();

	u32 coreCount = 8;
	u32 tileWidth = image.width / coreCount;
	u32 tileHeight = tileWidth;
	u32 tileCountX = (image.width + tileWidth - 1) / tileWidth;
	u32 tileCountY = (image.height + tileHeight - 1) / tileHeight;
	u32 totalTilesCount = tileCountX * tileCountY;

	printf("Configurations: %d Cores with %d of %dx%d (%dk) tiles.\n", coreCount, totalTilesCount, tileWidth, tileHeight, (tileWidth * tileHeight) * 4 / 1024);

	WorkQueue workQueue = {};
	workQueue.workOrders = (WorkOrder*)malloc(sizeof(WorkOrder) * totalTilesCount);

	for (u32 tileY = 0;
		 tileY < tileCountY;
		 ++tileY)
	{
		u32 minY = tileY * tileHeight;
		u32 maxY = minY + tileHeight;
		if (maxY > image.height)
			maxY = image.height;
		for (u32 tileX = 0;
			 tileX < tileCountX;
			 ++tileX)
		{
			u32 minX = tileX * tileWidth;
			u32 maxX = minX + tileWidth;
			if (maxX > image.width)
				maxX = image.width;

			WorkOrder* workOrder = workQueue.workOrders + workQueue.workOrderCount++;
			// workQueue.workOrders[workQueue.workOrderCount - 1] = *workOrders;
			Assert(workQueue.workOrderCount <= totalTilesCount);
			workOrder->world = &world;
			workOrder->image = image;
			workOrder->minX = minX;
			workOrder->minY = minY;
			workOrder->maxX = maxX;
			workOrder->maxY = maxY;
		}
	}
	Assert(workQueue.workOrderCount == totalTilesCount);

	while (workQueue.tilesRenderedCount < totalTilesCount)
	{
		RenderTile(&workQueue);
		printf("Progress: %.2f%%\r", 100.0f * (f32)workQueue.tilesRenderedCount / (f32)workQueue.workOrderCount);
		fflush(stdout);
	}

	clock_t endTime = clock();
	clock_t timeElapsed = endTime - startTime;
	printf("Render Time: %dms\n", timeElapsed);
	printf("Total Bounces: %llu\n", workQueue.bouncesComputed);
	printf("Performance: %fms/bounces\n", ((f64)timeElapsed / (f64)workQueue.bouncesComputed));

	WriteImage(image, "output/final_render.bmp");

	return 0;
}
