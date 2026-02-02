/* (C) 2026 madoodia.com */

#pragma once

#include "global.h"

union V2
{
	struct
	{
		f32 x, y;
	};
	struct
	{
		f32 u, v;
	};
	f32 E[2];
};

union V3
{
	struct
	{
		f32 x, y, z;
	};
	struct
	{
		f32 r, g, b;
	};
	struct
	{
		f32 u, v, w;
	};
	struct
	{
		V2 xy;
		f32 _z;
	};
	struct
	{
		f32 _x;
		V2 yz;
	};
	struct
	{
		V2 uv;
		f32 _w;
	};
	struct
	{
		f32 _u;
		V2 vw;
	};
	f32 E[3];
};

union V4
{
	struct
	{
		union
		{
			V3 xyz;
			struct
			{
				f32 x, y, z;
			};
		};
		f32 w;
	};
	struct
	{
		union
		{
			V3 rgb;
			struct
			{
				f32 r, g, b;
			};
			f32 a;
		};
	};
	struct
	{
		V3 xyz;
		f32 _w;
	};
	struct
	{
		V2 xy;
		f32 _z;
		f32 _w;
	};
	struct
	{
		f32 _x;
		V2 yz;
		f32 _w;
	};
	struct
	{
		f32 _x;
		f32 _y;
		V2 zw;
	};

	f32 E[4];
};

inline V3
operator*(f32 a, V3 b)
{
	V3 result;
	result.x = a * b.x;
	result.y = a * b.y;
	result.z = a * b.z;
	return result;
}

inline V3
operator*(V3 a, f32 b)
{
	V3 result;
	result.x = a.x * b;
	result.y = a.y * b;
	result.z = a.z * b;
	return result;
}

inline V3
operator*(V3 a, V3 b)
{
	V3 result;
	result.x = a.x * b.x;
	result.y = a.y * b.y;
	result.z = a.z * b.z;
	return result;
}

inline V3
operator-(V3 a, V3 b)
{
	V3 result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	return result;
}

inline V2
operator+=(V2 a, V2 b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

inline V3
operator+(const V3 a, const V3 b)
{
	V3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}

inline V3&
operator+=(V3& a, V3 b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return a;
}

inline V2
V2f(f32 x, f32 y)
{
	V2 result;
	result.x = x;
	result.y = y;
	return result;
}
inline V3
V3f(f32 x, f32 y, f32 z)
{
	V3 result;
	result.x = x;
	result.y = y;
	result.z = z;
	return result;
}
inline V3
V3f(u32 x, u32 y, u32 z)
{
	V3 result;
	result.x = (f32)x;
	result.y = (f32)y;
	result.z = (f32)z;
	return result;
}
inline V4
V4f(f32 x, f32 y, f32 z, f32 w)
{
	V4 result;
	result.x = x;
	result.y = y;
	result.z = z;
	result.w = w;
	return result;
}

inline V4
V4f(V3 v, f32 w)
{
	V4 result;
	result.x = v.x;
	result.y = v.y;
	result.z = v.z;
	result.w = w;
	return result;
}

inline V2
Hadamard(V2 A, V2 B)
{
	V2 result = {A.x * B.x,
				 A.y * B.y};
	return result;
}

inline V3
Hadamard(V3 A, V3 B)
{
	V3 result = {A.x * B.x,
				 A.y * B.y,
				 A.z * B.z};
	return result;
}

inline f32
Square(f32 value)
{
	f32 result = value * value;
	return result;
}

inline f32
SquareRoot(f32 value)
{
	f32 result = (f32)sqrt(value);
	return result;
}

inline f32
Dot(V2 a, V2 b)
{
	f32 result = a.x * b.x + a.y * b.y;
	return result;
}

inline f32
Dot(V3 a, V3 b)
{
	f32 result = a.x * b.x + a.y * b.y + a.z * b.z;
	return result;
}

inline f32
LengthSqr(V3 v)
{
	f32 length = Dot(v, v);
	return length;
}

inline f32
Length(V3 v)
{
	f32 length = SquareRoot(LengthSqr(v));
	return length;
}

inline V3
Cross(V3 a, V3 b)
{
	V3 result;
	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
	return result;
}

inline V3
Normalize(V3 v)
{
	return v * (1.0f / Length(v));
}

inline V3
NOZ(V3 v)
{
	V3 result = {};

	f32 lenSqr = LengthSqr(v);
	if (lenSqr > Square(0.0001f))
	{
		f32 invLen = 1.0f / SquareRoot(lenSqr);
		result = v * invLen;
	}

	return result;
}

inline f32
Distance(V3 a, V3 b)
{
	return Length(b - a);
}

inline u32
RoundF32ToU32(f32 value)
{
	return (u32)(value + 0.5f);
}

inline u32
RGBAPack4x8(V4 unpacked)
{
	u32 result = ((RoundF32ToU32)(unpacked.a) << 24) |
				 ((RoundF32ToU32)(unpacked.b) << 16) |
				 ((RoundF32ToU32)(unpacked.g) << 8) |
				 ((RoundF32ToU32)(unpacked.r) << 0);
	return result;
}

inline V4
RGBAUnpack4x8(u32 packed)
{
	V4 result;
	result.r = (f32)((packed >> 0) & 0xFF);
	result.g = (f32)((packed >> 8) & 0xFF);
	result.b = (f32)((packed >> 16) & 0xFF);
	result.a = (f32)((packed >> 24) & 0xFF);
	return result;
}

inline u32
BGRAPack4x8(V4 unpacked)
{
	u32 result = ((RoundF32ToU32)(unpacked.a) << 24) |
				 ((RoundF32ToU32)(unpacked.r) << 16) |
				 ((RoundF32ToU32)(unpacked.g) << 8) |
				 ((RoundF32ToU32)(unpacked.b) << 0);
	return result;
}

inline V4
BGRAUnpack4x8(u32 packed)
{
	V4 result;
	result.b = (f32)((packed >> 0) & 0xFF);
	result.g = (f32)((packed >> 8) & 0xFF);
	result.r = (f32)((packed >> 16) & 0xFF);
	result.a = (f32)((packed >> 24) & 0xFF);
	return result;
}
