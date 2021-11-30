#pragma once
#include "Vectors.h"
#include <math.h>

float distance(Vec3 first, Vec3 second)
{
	return length(first - second);
}

float length(const Vec3& vec)
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}