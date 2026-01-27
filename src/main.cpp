// --------------------- //
// (C) 2026 madoodia.com //
// --------------------- //

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <Windows.h>

// timer rdtsc
#include <chrono>
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

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef char s8;
typedef short s16;
typedef int s32;

typedef float f32;
typedef double f64;

int main(int, char**)
{
	u32 Width = 4096;
	u32 Height = 4096;

	u32 OutputSize = sizeof(u32) * Width * Height;
	u32* OutputBuffer = (u32*)malloc(OutputSize);

	u32* FinalOutput = OutputBuffer;

	u64 start = ReadTimeStampCounter();
	u64 cpu_hz = GetCPUFrequencyHz();

	for (u32 Y = 0;
		 Y < Height;
		 ++Y)
	{
		for (u32 X = 0;
			 X < Width;
			 ++X)
		{
			*FinalOutput++ = (Y < 32) ? 0x0000FFFF : 0xFF0000FF;
		}
	}

	u64 end = ReadTimeStampCounter();

	double elapsed_ms = (double)(end - start) / cpu_hz * 1000.0;
	printf("---------------------------\n");
	printf("Program Start: %llu\n", start);
	printf("Program End: %llu\n", end);
	printf("Program Elapsed Time: %.2f ms\n\n", elapsed_ms);

	printf("CPU Cycles Elapsed: %llu\n", (u64)(end - start));
	printf("CPU Frequency: %llu Hz\n", cpu_hz);

	printf("---------------------------\n");

	free(OutputBuffer);
}
