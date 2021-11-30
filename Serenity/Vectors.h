#pragma once

class Vec3
{
public:
	float x, y, z;

	Vec3() { x = .0f; y = .0f; z = .0f; }
	Vec3(float x, float y, float z) : x{ x }, y{ y }, z{ z } {}
	Vec3 operator+(const Vec3& other) { return { x + other.x, y + other.y, z + other.z }; }
	Vec3 operator*(const Vec3& other) { return { x * other.x, y * other.y, z * other.z }; }
	Vec3 operator-(const Vec3& other) { return { x - other.x, y - other.y, z - other.z }; }
	Vec3 operator/(const Vec3& other) { return { x / other.x, y / other.y, z / other.z }; }
	Vec3& operator*=(const Vec3& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
	Vec3& operator-=(const Vec3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
	Vec3& operator/=(const Vec3& other) { x /= other.x; y /= other.y; z /= other.z; return *this; }
	Vec3& operator+=(const Vec3& other) { x += other.x; y += other.y; z += other.z; return *this; }
};

float distance(Vec3 first, Vec3 second);

float length(const Vec3& vec);

struct Vec2{
	float x, y;
};


