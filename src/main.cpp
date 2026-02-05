/* (C) 2026 madoodia.com */

#include "Global.h"
#include "Math.h"
#include "Utils.cpp"

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
	materials[4].refColor = V3f(0.5f, 0.7f, 0.921f);
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

	u32 coreCount = GetActiveProcessorCount(0);
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

	// fencing?
	// LockedAdd(&workQueue.nextWorkOrderIndex, 0);
#if 1
	for (u32 coreIndex = 1;
		 coreIndex < coreCount;
		 ++coreIndex)
	{
		CreateWorkThread(&workQueue);
	}
#endif
	while (workQueue.tilesRenderedCount < totalTilesCount)
	{
		if (RenderTile(&workQueue))
		{
			printf("Progress: %d%%\r", (u32)(100.0f * workQueue.tilesRenderedCount / workQueue.workOrderCount));
		}
		fflush(stdout);
	}

	clock_t endTime = clock();
	clock_t timeElapsed = endTime - startTime;
	printf("Render Time: %dms\n", timeElapsed);
	printf("Total Bounces: %llu\n", workQueue.bouncesComputed);
	printf("Performance: %fms/bounces\n", ((f64)timeElapsed / (f64)workQueue.bouncesComputed));

	WriteImage(image, "output/FinalRender.bmp");

	return 0;
}
