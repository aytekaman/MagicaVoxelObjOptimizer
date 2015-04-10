#include "vector3.h"

#include <math.h>

#include "mathx.h"
#include "vector2.h"

Vector3::Vector3(float _x, float _y, float _z)
{
	Set(_x, _y, _z);
}

Vector3::~Vector3()
{
}

float& Vector3::operator[](int i)
{
	return (&x)[i];
}

const float& Vector3::operator[](int i) const
{
	return (&x)[i];
}
	
Vector3 Vector3::GetNormalized() const
{
	float s = 1 / GetLength();

	return *this * s;
}

float Vector3::GetLength() const
{
	return sqrt(GetSquaredLength());
}

float Vector3::GetSquaredLength() const
{
	return x*x + y*y + z*z;
}

void Vector3::Set(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

void Vector3::Normalize()
{
	float s = 1 / GetLength();

	*this = *this * s;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator+(const Vector3 &v) const
{
	return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3 Vector3::operator-(const Vector3 &v) const
{
	return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3 Vector3::operator*(const Vector3 &v) const
{
	return Vector3(x * v.x, y * v.y, z * v.z);
}

Vector3 Vector3::operator/(const Vector3 &v) const
{
	return Vector3(x / v.x, y / v.y, z / v.z);
}

Vector3& Vector3::operator+=(const Vector3 &v)
{
	x += v.x; 
	y += v.y; 
	z += v.z; 
	
	return *this;
}

Vector3& Vector3::operator-=(const Vector3 &v)
{
	x -= v.x; 
	y -= v.y; 
	z -= v.z; 
	
	return *this;
}

Vector3& Vector3::operator*=(const Vector3 &v)
{
	x *= v.x; 
	y *= v.y; 
	z *= v.z; 
	
	return *this;
}

Vector3& Vector3::operator/=(const Vector3 &v)
{
	x /= v.x; 
	y /= v.y; 
	z /= v.z; 
	
	return *this;
}

Vector3 Vector3::operator+(float s) const
{
	return Vector3(x + s, y + s, z + s);
}

Vector3 Vector3::operator-(float s) const
{
	return Vector3(x - s, y - s, z - s);
}

Vector3 Vector3::operator*(float s) const
{
	return Vector3(x * s, y * s, z * s);
}

Vector3 Vector3::operator/(float s) const
{
	return Vector3(x / s, y / s, z / s);
}

Vector3& Vector3::operator+=(float s)
{
	x += s; 
	y += s; 
	z += s; 
	
	return *this;
}

Vector3& Vector3::operator-=(float s)
{
	x -= s; 
	y -= s; 
	z -= s; 
	
	return *this;
}

Vector3& Vector3::operator*=(float s)
{
	x *= s; 
	y *= s; 
	z *= s; 
	
	return *this;
}

Vector3& Vector3::operator/=(float s)
{
	x /= s; 
	y /= s; 
	z /= s; 
	
	return *this;
}

Vector3 operator+(float s, const Vector3& v)
{
	return v + s;
}

Vector3 operator-(float s, const Vector3& v)
{
	return -v + s;
}

Vector3 operator*(float s, const Vector3& v)
{
	return v * s;
}

float Vector3::Angle(const Vector3 &from, const Vector3 &to)
{
	return acos(Dot(from, to) / (from.GetLength() * to.GetLength())) * Mathx::rad2deg;
}

Vector3	Vector3::Cross(const Vector3 &a, const Vector3 &b)
{
	return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

float Vector3::Distance(const Vector3 &a, const Vector3 &b)
{
	return (a - b).GetLength();
}

float Vector3::Dot(const Vector3 &a, const Vector3 &b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

std::ostream& operator<<(std::ostream& os, const Vector3& v)
{
	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";

	return os;
}

Vector3::operator Vector2() const
{
	return Vector2(x, y);
}

const Vector3 Vector3::zero		= Vector3(0,0,0);
const Vector3 Vector3::one		= Vector3(1,1,1);
const Vector3 Vector3::left		= Vector3(1,0,0);
const Vector3 Vector3::up		= Vector3(0,1,0);
const Vector3 Vector3::forward	= Vector3(0,0,1);
const Vector3 Vector3::right	= Vector3(-1,0,0);
const Vector3 Vector3::down		= Vector3(0,-1,0);
const Vector3 Vector3::back		= Vector3(0,0,-1);