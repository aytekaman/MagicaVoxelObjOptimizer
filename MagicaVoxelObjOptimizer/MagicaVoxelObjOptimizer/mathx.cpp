#include "mathx.h"

float Mathx::Clamp(float value, float min, float max)
{
	if(value < min)
		return min;

	if(value > max)
		return max;

	return value;
}

int Mathx::Clamp(int value, int min, int max)
{
	if(value < min)
		return min;

	if(value > max)
		return max;

	return value;
}

float Mathx::InverseLerp(float from, float to, float value)
{
	return (value - from) / (to - from);
}

float Mathx::Lerp(float from, float to, float t)
{
	return from + t * (to - from);
}

float Mathx::Max(float a, float b)
{
	return a > b ? a : b;
}

int Mathx::Max(int a, int b)
{
	return a > b ? a : b;
}

float Mathx::Min(float a, float b)
{
	return a < b ? a : b;
}

int Mathx::Min(int a, int b)
{
	return a < b ? a : b;
}

float Mathx::PingPong(float t, float length)
{
	float lengthDoubled = 2 * length;

	while(t > lengthDoubled)
		t -= lengthDoubled;

	while(t < 0)
		t += lengthDoubled;

	if(t < length)
		return t;
	else
		return lengthDoubled - t;
}

const float Mathx::pi = 3.1415926535897932f;
const float Mathx::deg2rad = pi / 180;
const float Mathx::rad2deg = 180 / pi;
const float Mathx::golden = 1.61803398875f;