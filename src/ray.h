/* (C) 2026 madoodia.com */

#pragma once

#include "global.h"
#include "math.h"

struct Material
{
	V3 emitColor;
	V3 refColor;
};

struct Plane
{
	V3 normal;
	f32 distance;
	u32 matIndex;
};

struct Sphere
{
	V3 center;
	f32 radius;
	u32 matIndex;
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

	f32 tolerance = 0.0001f;
	f32 minHitDist = 0.001f;

	V3 result = {};
	V3 attenuation = V3f(1.0f, 1.0f, 1.0f);
	for (u32 rayCount = 0;
		 rayCount < 8;
		 ++rayCount)
	{
		f32 hitDistance = F32MAX;
		u32 hitMatIndex = 0;
		V3 nextOrigin = {};
		V3 nextNormal = {};

		// Planes
		for (u32 index = 0;
			 index < world->planeCount;
			 ++index)
		{
			Plane plane = world->planes[index];

			f32 denom = Dot(plane.normal, rayDirection);
			if ((denom < -tolerance) > (denom > tolerance))
			{
				f32 t = (-plane.distance - Dot(plane.normal, rayOrigin)) / denom;
				if ((t < hitDistance) && (t > minHitDist))
				{
					hitDistance = t;
					hitMatIndex = plane.matIndex;

					nextOrigin = t * rayDirection;
					nextNormal = plane.normal;
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

			f32 denom = 2.0f * a;
			f32 root = SquareRoot(b * b - 4.0f * a * c);
			if (root > tolerance)
			{
				f32 tp = (-b + root) / denom;
				f32 tn = (-b - root) / denom;

				f32 t = tp;
				if ((tn > minHitDist) && (tn < tp))
					t = tn;
				if ((t > minHitDist) && (t < hitDistance))
				{
					hitDistance = t;
					hitMatIndex = sphere.matIndex;

					nextOrigin = t * rayDirection;
					nextNormal = NOZ(nextOrigin - sphere.center);
				}
			}
		}

		if (hitMatIndex)
		{
			Material hitMat = world->materials[hitMatIndex];
			result += Hadamard(attenuation, hitMat.emitColor);
			attenuation = Hadamard(attenuation, hitMat.refColor);

			rayOrigin = nextOrigin;
			rayDirection = nextNormal;
		}
		else
		{
			Material hitMat = world->materials[hitMatIndex];
			result += Hadamard(attenuation, hitMat.emitColor);
			break;
		}
	}
	return result;
}
