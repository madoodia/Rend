/* (C) 2026 madoodia.com */

#pragma once

#include <stdint.h>
#include "global.h"

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
		return (uint64_t)freq_mhz * 1000000; // MHz to Hz
	}
	return 0;
}

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
