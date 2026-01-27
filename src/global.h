#pragma once

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
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

struct ImageBuffer
{
	u32 width;
	u32 height;
	u32* pixels;
};
