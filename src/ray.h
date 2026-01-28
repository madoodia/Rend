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
	V3 result = world->materials[0].color; // Background color

	f32 hitDistance = F32MAX;
	f32 tolerance = 0.0001f;

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
			if ((t < hitDistance) && (t > 0.0f))
			{
				hitDistance = t;
				result = world->materials[plane.MaterialIndex].color;
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

		f32 discriminant = b * b - 4.0f * a * c;
		if (discriminant >= tolerance)
		{
			f32 sqrtDiscriminant = SquareRoot(discriminant);
			f32 denom = 2.0f * a;
			f32 t0 = (-b + sqrtDiscriminant) / denom;
			f32 t1 = (-b - sqrtDiscriminant) / denom;

			f32 t = (t0 < t1) ? t0 : t1;
			if ((t < hitDistance) && (t > 0.0f))
			{
				hitDistance = t;
				result = world->materials[sphere.MaterialIndex].color;
			}
		}
	}

	return result;
}
