/* (C) 2026 madoodia.com */

#pragma once

#include <time.h>

#include "global.h"
#include "math.h"

struct Material
{
	f32 scatter;
	V3 emitColor;
	V3 refColor;
};

struct Plane
{
	V3 normal;
	f32 distance;
	u32 matIndex;
};

struct Sphere
{
	V3 center;
	f32 radius;
	u32 matIndex;
};

struct World
{
	Material* materials;
	u32 materialCount;

	Plane* planes;
	u32 planeCount;

	Sphere* spheres;
	u32 sphereCount;
};

struct WorkOrder
{
	World* world;
	ImageBuffer image;
	u32 minX;
	u32 minY;
	u32 maxX;
	u32 maxY;
};

struct WorkQueue
{
	u32 workOrderCount;
	WorkOrder* workOrders;

	volatile u64 nextWorkOrderIndex;
	volatile u64 bouncesComputed;
	volatile u64 tilesRenderedCount;
};

u32 GetTotalPixelSize(ImageBuffer image);
ImageBuffer AllocateImage(u32 width, u32 height);
void WriteImage(ImageBuffer image, const char* filename);
f32 LinearToRGB(f32 linear);
u32* GetPixelPointer(ImageBuffer image, u32 x, u32 y);
u64 LockedAdd(volatile u64* value, u64 addend);
V3 RayCast(WorkQueue* workQueue, World* world, V3 rayOrigin, V3 rayDirection, f32 contribution);
b32x RenderTile(WorkQueue* workQueue);
uint64_t GetCPUFrequencyHz();
DWORD WINAPI WorkerThread(void* lpParameter);
DWORD WINAPI CreateWorkThread(void* parameter);

class TimeStamp
{
public:
	explicit TimeStamp(const char* msg)
	{
		m_start = ReadTimeStampCounter();
		m_cpu_hz = GetCPUFrequencyHz();
		m_msg = msg;
	}
	~TimeStamp()
	{
		m_end = ReadTimeStampCounter();

		double elapsed_ms = (double)(m_end - m_start) / m_cpu_hz * 1000.0;
		printf("-------------------------------\n");
		// printf("Program Start: %llu\n", m_start);
		// printf("Program End: %llu\n", m_end);
		printf("%s >>> Program Elapsed Time: %.2f ms\n", m_msg, elapsed_ms);

		// printf("CPU Cycles Elapsed: %llu\n", (u64)(m_end - m_start));
		// printf("CPU Frequency: %llu Hz\n", m_cpu_hz);
		printf("-------------------------------\n");
	}

private:
	u64 m_start;
	u64 m_end;
	u64 m_cpu_hz;
	const char* m_msg;
};
