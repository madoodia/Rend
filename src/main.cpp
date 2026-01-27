// --------------------- //
// (C) 2026 madoodia.com //
// --------------------- //

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <Windows.h>
#include <string>

#define internal static

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef char s8;
typedef short s16;
typedef int s32;

typedef float f32;
typedef double f64;

#pragma pack(push, 1)
struct BitmapHeader
{
	u16 fileType;
	u32 fileSize;
	u16 reserved1;
	u16 reserved2;
	u32 bitmapOffset;
	u32 size;
	s32 width;
	s32 height;
	u16 planes;
	u16 bitsPerPixel;
	u32 compression;
	u32 sizeOfBitmap;
	s32 horzResolution;
	s32 vertResolution;
	u32 colorUsed;
	u32 colorImportant;
};
#pragma pack(pop)

internal uint64_t
GetCPUFrequencyHz()
{
	DWORD freq_mhz = 0;
	DWORD size = sizeof(freq_mhz);
	LONG result = RegGetValueA(HKEY_LOCAL_MACHINE,
							   "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
							   "~MHz", RRF_RT_REG_DWORD, NULL, &freq_mhz, &size);

	if (result == ERROR_SUCCESS)
	{
		return (u64)freq_mhz * 1000000; // MHz to Hz
	}
	return 0;
}

class TimeStamp
{
public:
	TimeStamp()
	{
		m_start = ReadTimeStampCounter();
		m_cpu_hz = GetCPUFrequencyHz();
	}
	~TimeStamp()
	{
		m_end = ReadTimeStampCounter();

		double elapsed_ms = (double)(m_end - m_start) / m_cpu_hz * 1000.0;
		printf("-------------------------------\n");
		// printf("Program Start: %llu\n", m_start);
		// printf("Program End: %llu\n", m_end);
		printf("Program Elapsed Time: %.2f ms\n", elapsed_ms);

		// printf("CPU Cycles Elapsed: %llu\n", (u64)(m_end - m_start));
		// printf("CPU Frequency: %llu Hz\n", m_cpu_hz);
		printf("-------------------------------\n");
	}

private:
	u64 m_start;
	u64 m_end;
	u64 m_cpu_hz;
};

struct ImageBuffer
{
	u32 width;
	u32 height;
	u32* pixels;
};

internal u32
GetTotalPixelSize(ImageBuffer image)
{
	return image.width * image.height * sizeof(u32);
}

internal ImageBuffer
AllocateImage(u32 width, u32 height)
{
	ImageBuffer image = {};
	image.width = width;
	image.height = height;

	u32 outputSize = GetTotalPixelSize(image);
	image.pixels = (u32*)malloc(outputSize);

	return image;
}

internal void
WriteImage(ImageBuffer image, const char* filename)
{
	u32 outputSize = GetTotalPixelSize(image);
	BitmapHeader bmpHeader = {};
	bmpHeader.fileType = 0x4D42; // 'BMP'
	bmpHeader.fileSize = sizeof(bmpHeader) + outputSize;
	bmpHeader.reserved1 = 0;
	bmpHeader.reserved2 = 0;
	bmpHeader.bitmapOffset = sizeof(bmpHeader);
	bmpHeader.size = sizeof(bmpHeader) - 14;
	bmpHeader.width = image.width;
	bmpHeader.height = image.height;
	bmpHeader.planes = 1;
	bmpHeader.bitsPerPixel = 32;
	bmpHeader.compression = 0;
	bmpHeader.sizeOfBitmap = outputSize;
	bmpHeader.horzResolution = 0;
	bmpHeader.vertResolution = 0;
	bmpHeader.colorUsed = 0;
	bmpHeader.colorImportant = 0;

	FILE* outputFile = fopen(filename, "wb");

	if (outputFile)
	{
		fwrite(&bmpHeader, sizeof(bmpHeader), 1, outputFile);
		fwrite(image.pixels, outputSize, 1, outputFile);
		fclose(outputFile);
	}
	else
	{
		fprintf(stderr, "[Error]: Could not open and write data to output file '%s'.\n", filename);
	}
}

int main(int, char**)
{
	ImageBuffer image = AllocateImage(1280, 720);

	u32* finalOutput = image.pixels;

	{
		TimeStamp timer;
		for (u32 Y = 0;
			 Y < image.height;
			 ++Y)
		{
			for (u32 X = 0;
				 X < image.width;
				 ++X)
			{
				*finalOutput++ = (Y < image.height/3) ? 0xFFAA0000 : ((Y < 2*image.height/3) ? 0xFFFFFFFF : 0xFF00AA00);
			}
		}

		WriteImage(image, "images/output.bmp");
	}

	return 0;
}
