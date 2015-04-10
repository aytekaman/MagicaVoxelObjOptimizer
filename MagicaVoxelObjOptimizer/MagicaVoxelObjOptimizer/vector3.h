#pragma once

#include <iostream>

struct Vector2;

struct Vector3
{
	Vector3(float x = 0, float y = 0, float z = 0);
	~Vector3();

	float& operator[](int i);
	const float& operator[](int i) const; 
	
	Vector3 GetNormalized() const;
	float GetLength() const;
	float GetSquaredLength() const;

	void Set(float x, float y, float z);

	void Normalize();

	Vector3 operator-() const;

	Vector3 operator+(const Vector3 &v) const;
	Vector3 operator-(const Vector3 &v) const;
	Vector3 operator*(const Vector3 &v) const;
	Vector3 operator/(const Vector3 &v) const;

	Vector3& operator+=(const Vector3 &v);
	Vector3& operator-=(const Vector3 &v);
	Vector3& operator*=(const Vector3 &v);
	Vector3& operator/=(const Vector3 &v);

	Vector3 operator+(float s) const;
	Vector3 operator-(float s) const;
	Vector3 operator*(float s) const;
	Vector3 operator/(float s) const;

	Vector3& operator+=(float s);
	Vector3& operator-=(float s);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);

	friend Vector3 operator+(float s, const Vector3& v);
	friend Vector3 operator-(float s, const Vector3& v);
	friend Vector3 operator*(float s, const Vector3& v);

	static float	Angle(const Vector3 &from, const Vector3 &to);
	static Vector3	Cross(const Vector3 &a, const Vector3 &b);
	static float	Distance(const Vector3 &a, const Vector3 &b);
	static float	Dot(const Vector3 &a, const Vector3 &b);

	friend std::ostream& operator<<(std::ostream& os, const Vector3& v);
	operator Vector2() const;

	float x, y, z;

	const static Vector3 zero, one, left, up, forward, right, down, back;
};