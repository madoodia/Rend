/* (C) 2026 madoodia.com */

#pragma once

#include "global.h"
#include "math.h"

struct Material
{
	V3 color;
};

struct Plane
{
	V3 normal;
	f32 distance;
	u32 MaterialIndex;
};

struct Sphere
{
	V3 center;
	f32 radius;
	u32 MaterialIndex;
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

internal V3
RayCast(World* world, V3 rayOrigin, V3 rayDirection)
{
	V3 resultColor = world->materials[0].color; // Background color
	return resultColor;
}
