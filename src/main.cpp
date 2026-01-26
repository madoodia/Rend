// --------------------- //
// (C) 2026 madoodia.com //
// --------------------- //

#include <stdio.h>
#include <stdlib.h>

// timer rdtsc
#include <chrono>

std::chrono::steady_clock::time_point GetTimeStamp()
{
	return std::chrono::steady_clock::now();
}

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef char s8;
typedef short s16;
typedef int s32;

typedef float f32;
typedef double f64;

int main(int, char**)
{
	u32 Width = 5;
	u32 Height = 5;

	u32* ptr = nullptr;

	u32 OutputSize = sizeof(u32) * Width * Height;
	u32* OutputBuffer = (u32*)malloc(OutputSize);

	u32* FinalOutput = OutputBuffer;

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

	return (0);
}
