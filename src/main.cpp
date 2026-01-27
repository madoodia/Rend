// --------------------- //
// (C) 2026 madoodia.com //
// --------------------- //

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <Windows.h>
#include <string>

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
	u16 FileType;
	u32 FileSize;
	u16 Reserved1;
	u16 Reserved2;
	u32 BitmapOffset;
	u32 Size;
	s32 Width;
	s32 Height;
	u16 Planes;
	u16 BitsPerPixel;
	u32 Compression;
	u32 SizeOfBitmap;
	s32 HorzResolution;
	s32 VertResolution;
	u32 ColorUsed;
	u32 ColorImportant;
};
#pragma pack(pop)

uint64_t GetCPUFrequencyHz();
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

uint64_t GetCPUFrequencyHz()
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

int main(int, char**)
{
	u32 Width = 1280;
	u32 Height = 780;

	u32 OutputSize = sizeof(u32) * Width * Height;
	u32* OutputBuffer = (u32*)malloc(OutputSize);

	u32* FinalOutput = OutputBuffer;

	{
		TimeStamp timer;
		for (u32 Y = 0;
			 Y < Height;
			 ++Y)
		{
			for (u32 X = 0;
				 X < Width;
				 ++X)
			{
				*FinalOutput++ = (Y < 32) ? 0xFFFF0000 : 0xFF0000FF;
			}
		}

		BitmapHeader BMPHeader = {};
		BMPHeader.FileType = 0x4D42; // 'BMP'
		BMPHeader.FileSize = sizeof(BMPHeader) + OutputSize;
		BMPHeader.Reserved1 = 0;
		BMPHeader.Reserved2 = 0;
		BMPHeader.BitmapOffset = sizeof(BMPHeader);
		BMPHeader.Size = sizeof(BMPHeader) - 14;
		BMPHeader.Width = Width;
		BMPHeader.Height = Height;
		BMPHeader.Planes = 1;
		BMPHeader.BitsPerPixel = 32;
		BMPHeader.Compression = 0;
		BMPHeader.SizeOfBitmap = OutputSize;
		BMPHeader.HorzResolution = 0;
		BMPHeader.VertResolution = 0;
		BMPHeader.ColorUsed = 0;
		BMPHeader.ColorImportant = 0;

		char* OutputFilename = "images/output.bmp";
		FILE* OutputFile = fopen(OutputFilename, "wb");

		if (OutputFile)
		{
			fwrite(&BMPHeader, sizeof(BMPHeader), 1, OutputFile);
			fwrite(OutputBuffer, OutputSize, 1, OutputFile);
			fclose(OutputFile);
		}
		else
		{
			fprintf(stderr, "[Error]: Could not open and write data to output file '%s'.\n", OutputFilename);
		}
	}

	free(OutputBuffer);

	return 0;
}
