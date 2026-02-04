/* (C) 2026 madoodia.com */

#pragma once

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdint.h>
#include <stddef.h>

#define internal static
#define inline static
#define global static

#define U16MAX 0xFFFF
#define U32MAX 0xFFFFFFFF
#define F32MAX FLT_MAX
#define F32MAX FLT_MAX
#define F32MIN -FLT_MAX

#define PI 3.14159265359f
#define TAU32 6.28318530718f
#define INV_PI 0.31830988618f
#define HALF_PI 1.57079632679f

#define ARRAY_COUNT(arr) (sizeof(arr) / sizeof((arr)[0]))

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef s32 b32;
typedef s32 b32x;

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
	u32 width;
	u32 height;
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
