#include "vector2.h"

#include <math.h>

#include "mathx.h"
#include "vector3.h"

Vector2::Vector2(float _x, float _y)
{
	Set(_x, _y);
}

Vector2::~Vector2()
{
}

float& Vector2::operator[](int i)
{
	return (&x)[i];
}

const float& Vector2::operator[](int i) const
{
	return (&x)[i]; 
}
	
Vector2 Vector2::GetNormalized() const
{
	float s = 1 / GetLength();

	return *this * s;
}

float Vector2::GetLength() const
{
	return sqrt(GetSquaredLength());
}

float Vector2::GetSquaredLength() const
{
	return x*x + y*y;
}

void Vector2::Set(float _x, float _y)
{
	x = _x;
	y = _y;
}

void Vector2::Normalize()
{
	float s = 1 / GetLength();

	*this = *this * s;
}

Vector2 Vector2::operator-() const
{
	return Vector2(-x, -y);
}

Vector2 Vector2::operator+(const Vector2 &v) const
{
	return Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::operator-(const Vector2 &v) const
{
	return Vector2(x - v.x, y - v.y);
}

Vector2 Vector2::operator*(const Vector2 &v) const
{
	return Vector2(x * v.x, y * v.y);
}

Vector2 Vector2::operator/(const Vector2 &v) const
{
	return Vector2(x / v.x, y / v.y);
}

Vector2& Vector2::operator+=(const Vector2 &v)
{
	x += v.x; 
	y += v.y;
	
	return *this;
}

Vector2& Vector2::operator-=(const Vector2 &v)
{
	x -= v.x; 
	y -= v.y;

	return *this;
}

Vector2& Vector2::operator*=(const Vector2 &v)
{
	x *= v.x; 
	y *= v.y;
	
	return *this;
}

Vector2& Vector2::operator/=(const Vector2 &v)
{
	x /= v.x; 
	y /= v.y;

	return *this;
}

Vector2 Vector2::operator+(float s) const
{
	return Vector2(x + s, y + s);
}

Vector2 Vector2::operator-(float s) const
{
	return Vector2(x - s, y - s);
}

Vector2 Vector2::operator*(float s) const
{
	return Vector2(x * s, y * s);
}

Vector2 Vector2::operator/(float s) const
{
	return Vector2(x / s, y / s);
}

Vector2& Vector2::operator+=(float s)
{
	x += s; 
	y += s;
	
	return *this;
}

Vector2& Vector2::operator-=(float s)
{
	x -= s; 
	y -= s; 

	return *this;
}

Vector2& Vector2::operator*=(float s)
{
	x *= s; 
	y *= s; 

	return *this;
}

Vector2& Vector2::operator/=(float s)
{
	x /= s; 
	y /= s; 
	
	return *this;
}

Vector2 operator+(float s, const Vector2& v)
{
	return v + s;
}

Vector2 operator-(float s, const Vector2& v)
{
	return -v + s;
}

Vector2 operator*(float s, const Vector2& v)
{
	return v * s;
}

float Vector2::Angle(const Vector2 &from, const Vector2 &to)
{
	return acos(Dot(from, to) / (from.GetLength() * to.GetLength())) * Mathx::rad2deg;
}

float Vector2::Cross(const Vector2 &a, const Vector2 &b)
{
	return a.x * b.y - a.y * b.x;
}

float Vector2::Distance(const Vector2 &a, const Vector2 &b)
{
	return (a - b).GetLength();
}

float Vector2::Dot(const Vector2 &a, const Vector2 &b)
{
	return a.x * b.x + a.y * b.y;
}

std::ostream& operator<<(std::ostream& os, const Vector2& v)
{
	os << "(" << v.x << ", " << v.y << ")";

	return os;
}

Vector2::operator Vector3() const
{
	return Vector3(x, y, 0);
}

const Vector2 Vector2::zero		= Vector2(0,0);
const Vector2 Vector2::one		= Vector2(1,1);
const Vector2 Vector2::left		= Vector2(1,0);
const Vector2 Vector2::up		= Vector2(0,1);
const Vector2 Vector2::right	= Vector2(-1,0);
const Vector2 Vector2::down		= Vector2(0,-1);