#pragma once

#include <iostream>

struct Vector3;

struct Vector2
{
	Vector2(float x = 0, float y = 0);
	~Vector2();

	float& operator[](int i);
	const float& operator[](int i) const;
	
	Vector2 GetNormalized() const;
	float GetLength() const;
	float GetSquaredLength() const;

	void Set(float x, float y);

	void Normalize();

	Vector2 operator-() const;

	Vector2 operator+(const Vector2 &v) const;
	Vector2 operator-(const Vector2 &v) const;
	Vector2 operator*(const Vector2 &v) const;
	Vector2 operator/(const Vector2 &v) const;

	Vector2& operator+=(const Vector2 &v);
	Vector2& operator-=(const Vector2 &v);
	Vector2& operator*=(const Vector2 &v);
	Vector2& operator/=(const Vector2 &v);

	Vector2 operator+(float s) const;
	Vector2 operator-(float s) const;
	Vector2 operator*(float s) const;
	Vector2 operator/(float s) const;

	Vector2& operator+=(float s);
	Vector2& operator-=(float s);
	Vector2& operator*=(float s);
	Vector2& operator/=(float s);

	friend Vector2 operator+(float s, const Vector2& v);
	friend Vector2 operator-(float s, const Vector2& v);
	friend Vector2 operator*(float s, const Vector2& v);

	static float Angle(const Vector2 &from, const Vector2 &to);
	static float Cross(const Vector2 &a, const Vector2 &b);
	static float Distance(const Vector2 &a, const Vector2 &b);
	static float Dot(const Vector2 &a, const Vector2 &b);

	friend std::ostream& operator<<(std::ostream& os, const Vector2& v);
	operator Vector3() const;

	float x, y;

	const static Vector2 zero, one, left, up, right, down;
};