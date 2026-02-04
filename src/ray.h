/* (C) 2026 madoodia.com */

#pragma once

#include "global.h"
#include "math.h"

struct Material
{
	f32 scatter;
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

	u64 bouncesComputed;
	u32 tilesRenderedCount;
};

internal V3
RayCast(World* world, V3 rayOrigin, V3 rayDirection, f32 contribution)
{
	f32 tolerance = 0.0001f;
	f32 minHitDist = 0.001f;
	u64 bounceComputed = 0;

	V3 sample = {};
	V3 attenuation = V3f(1.0f, 1.0f, 1.0f);
	u32 bouncesCount = 32; // TODO: Solve the problem of one bouncesCount that render Objects Black
	for (u32 bounceIndex = 0;
		 bounceIndex < bouncesCount;
		 ++bounceIndex)
	{
		++bounceComputed;
		f32 hitDistance = F32MAX;
		u32 hitMatIndex = 0;
		V3 nextNormal = {};

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
				if ((t < hitDistance) && (t > minHitDist))
				{
					hitDistance = t;
					hitMatIndex = plane.matIndex;

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

					nextNormal = NOZ(t * rayDirection + sphereToOrigin);
				}
			}
		}

		if (hitMatIndex)
		{
			Material hitMat = world->materials[hitMatIndex];
			sample += Hadamard(attenuation, hitMat.emitColor);
			f32 cosAtten = 1.0f; // Dot(-rayDirection, nextNormal);
			if (cosAtten < 0.0f)
				cosAtten = 0.0f;
			attenuation = Hadamard(attenuation, cosAtten * hitMat.refColor);

			rayOrigin += hitDistance * rayDirection;
			V3 mainBounce = rayDirection - (Dot(rayDirection, nextNormal) * 2.0f * nextNormal);
			V3 otherBounce = NOZ(nextNormal + V3f(RandomBiF32(), RandomBiF32(), RandomBiF32()));
			rayDirection = NOZ(Lerp(mainBounce, otherBounce, hitMat.scatter));
		}
		else
		{
			Material hitMat = world->materials[hitMatIndex];
			sample += Hadamard(attenuation, hitMat.emitColor);
			break;
		}
	}

	world->bouncesComputed += bounceComputed;

	return sample;
}
