#pragma once

class Mathx
{
public:
	static float	Clamp(float value, float min, float max);
	static int		Clamp(int value, int min, int max);
	static float	InverseLerp(float from, float to, float value);
	static float	Lerp(float from, float to, float t);
	static float	Max(float a, float b);
	static int		Max(int a, int b);
	static float	Min(float a, float b);
	static int		Min(int a, int b);
	static float	PingPong(float t, float length);

	const static float deg2rad;
	const static float rad2deg;
	const static float pi;
	const static float golden;
};