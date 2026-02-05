/* (C) 2026 madoodia.com */

#include "utils.h"

u32 GetTotalPixelSize(ImageBuffer image)
{
	return (u32)image.width * ((u32)image.height * sizeof(u32));
}

ImageBuffer AllocateImage(u32 width, u32 height)
{
	ImageBuffer image = {};
	image.width = width;
	image.height = height;

	u32 pixelSize = GetTotalPixelSize(image);
	image.pixels = (u32*)malloc(pixelSize);

	return image;
}

void WriteImage(ImageBuffer image, const char* filename)
{
	u32 pixelSize = GetTotalPixelSize(image);
	BitmapHeader bmpHeader = {};
	bmpHeader.fileType = 0x4D42; // 'BMP'
	bmpHeader.fileSize = sizeof(bmpHeader) + pixelSize;
	bmpHeader.reserved1 = 0;
	bmpHeader.reserved2 = 0;
	bmpHeader.bitmapOffset = sizeof(bmpHeader);
	bmpHeader.size = sizeof(bmpHeader) - 14;
	bmpHeader.width = (u32)image.width;
	bmpHeader.height = (u32)image.height;
	bmpHeader.planes = 1;
	bmpHeader.bitsPerPixel = 32;
	bmpHeader.compression = 0;
	bmpHeader.sizeOfBitmap = pixelSize;
	bmpHeader.horzResolution = 0;
	bmpHeader.vertResolution = 0;
	bmpHeader.colorUsed = 0;
	bmpHeader.colorImportant = 0;

	FILE* outputFile = fopen(filename, "wb");

	if (outputFile)
	{
		fwrite(&bmpHeader, sizeof(bmpHeader), 1, outputFile);
		fwrite(image.pixels, pixelSize, 1, outputFile);
		fclose(outputFile);
		printf("Image written to '%s'\n", filename);
	}
	else
	{
		fprintf(stderr, "[Error]: Could not open and write data to output file '%s'.\n", filename);
	}
}

f32 LinearToRGB(f32 linear)
{

	if (linear <= 0.0f)
	{
		linear = 0.0f;
	}
	else if (linear >= 1.0f)
	{
		linear = 1.0f;
	}
	f32 srgb = 12.92f * linear;
	if (linear > 0.0031308f)
		srgb = 1.055f * Power(linear, 1.0f / 2.4f) - 0.055f;
	return srgb;
}

u32* GetPixelPointer(ImageBuffer image, u32 x, u32 y)
{
	u32* result = image.pixels + y * image.width + x;
	return result;
}

u64 LockedAdd(volatile u64* value, u64 addend)
{
	u64 oldValue = InterlockedExchangeAdd64((volatile LONG64*)value, (LONGLONG)addend);
	return (u64)oldValue;
}

V3 RayCast(WorkQueue* workQueue, World* world, V3 rayOrigin, V3 rayDirection, f32 contribution)
{
	f32 tolerance = 0.0001f;
	f32 minHitDist = 0.001f;
	u64 bounceComputed = 0;

	V3 sample = {};
	V3 attenuation = V3f(1.0f, 1.0f, 1.0f);
	u32 bouncesCount = 32; // TODO: Solve the problem of one bouncesCount that render Objects Black
	for (u32 bounceIndex = 0;
		 bounceIndex < bouncesCount;
		 ++bounceIndex)
	{
		++bounceComputed;
		f32 hitDistance = F32MAX;
		u32 hitMatIndex = 0;
		V3 nextNormal = {};

		// Planes
		for (u32 index = 0;
			 index < world->planeCount;
			 ++index)
		{
			Plane plane = world->planes[index];

			f32 denom = Dot(plane.normal, rayDirection);
			if ((denom < -tolerance) || (denom > tolerance))
			{
				f32 t = (-plane.distance - Dot(plane.normal, rayOrigin)) / denom;
				if ((t < hitDistance) && (t > minHitDist))
				{
					hitDistance = t;
					hitMatIndex = plane.matIndex;

					nextNormal = plane.normal;
				}
			}
		}

		// Spheres
		for (u32 index = 0;
			 index < world->sphereCount;
			 ++index)
		{
			Sphere sphere = world->spheres[index];

			V3 sphereToOrigin = rayOrigin - sphere.center;
			f32 a = Dot(rayDirection, rayDirection);
			f32 b = 2.0f * Dot(rayDirection, sphereToOrigin);
			f32 c = Dot(sphereToOrigin, sphereToOrigin) - (sphere.radius * sphere.radius);

			f32 denom = 2.0f * a;
			f32 root = SquareRoot(b * b - 4.0f * a * c);
			if (root > tolerance)
			{
				f32 tp = (-b + root) / denom;
				f32 tn = (-b - root) / denom;

				f32 t = tp;
				if ((tn > minHitDist) && (tn < tp))
					t = tn;
				if ((t > minHitDist) && (t < hitDistance))
				{
					hitDistance = t;
					hitMatIndex = sphere.matIndex;

					nextNormal = NOZ(t * rayDirection + sphereToOrigin);
				}
			}
		}

		if (hitMatIndex)
		{
			Material hitMat = world->materials[hitMatIndex];
			sample += Hadamard(attenuation, hitMat.emitColor);
			f32 cosAtten = 1.0f; // Dot(-rayDirection, nextNormal);
			if (cosAtten < 0.0f)
				cosAtten = 0.0f;
			attenuation = Hadamard(attenuation, cosAtten * hitMat.refColor);

			rayOrigin += hitDistance * rayDirection;
			V3 mainBounce = rayDirection - (Dot(rayDirection, nextNormal) * 2.0f * nextNormal);
			V3 otherBounce = NOZ(nextNormal + V3f(RandomBiF32(), RandomBiF32(), RandomBiF32()));
			rayDirection = NOZ(Lerp(mainBounce, otherBounce, hitMat.scatter));
		}
		else
		{
			Material hitMat = world->materials[hitMatIndex];
			sample += Hadamard(attenuation, hitMat.emitColor);
			break;
		}
	}

	LockedAdd(&workQueue->bouncesComputed, bounceComputed);

	return sample;
}

b32x RenderTile(WorkQueue* workQueue)
{
	u64 workOrderIndex = LockedAdd(&workQueue->nextWorkOrderIndex, 1);
	if (workOrderIndex >= workQueue->workOrderCount)
	{
		return false;
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
	LockedAdd(&workQueue->tilesRenderedCount, 1);
	return true;
}

uint64_t GetCPUFrequencyHz()
{
	DWORD freq_mhz = 0;
	DWORD size = sizeof(freq_mhz);
	LONG result = RegGetValueA(HKEY_LOCAL_MACHINE,
							   "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
							   "~MHz", RRF_RT_REG_DWORD, NULL, &freq_mhz, &size);

	if (result == ERROR_SUCCESS)
	{
		return (uint64_t)freq_mhz * 1000000; // MHz to Hz
	}
	return 0;
}

DWORD WINAPI WorkerThread(void* lpParameter)
{
	WorkQueue* workQueue = (WorkQueue*)lpParameter;

	while (true)
	{
		RenderTile(workQueue);
	}
	return 0;
}

DWORD WINAPI CreateWorkThread(void* parameter)
{
	DWORD threadId;
	HANDLE threadHandle = CreateThread(0, 0, WorkerThread, parameter, 0, &threadId);
	CloseHandle(threadHandle);
	return threadId;
}
