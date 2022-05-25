#pragma once
#include <cmath>

struct Vector2
{
	float x;
	float y;

	/* Constructors*/
	Vector2()
	{
		x = 0;
		y = 0;
	}

	Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	/**********************
	 * Operator Overloads *
	 **********************/
	Vector2 operator+(Vector2& other) { return Vector2(x + other.x, y + other.y); }
	Vector2 operator-(const Vector2& other) { return Vector2(x - other.x, y - other.y); }
	Vector2 operator*(float scale) { return Vector2(x * scale, y * scale); }
	Vector2 operator*(const Vector2& other) { return Vector2(x * other.x, y * other.y); }
	void operator/=(float value) { x /= value; y /= value; }

	Vector2 operator-()
	{
		return Vector2(-x, -y);
	}

	bool operator==(const Vector2& other) const {
		return x == other.x && y == other.y;
	}

	bool operator!=(const Vector2& other) const {
		return !(*this == other);
	}

	/************
	* Functions *
	*************/
	float GetMagnitude()
	{
		return sqrt(x * x + y * y);
	}

	void Normalise()
	{
		float magnitude = GetMagnitude();
		if (magnitude != 0)
		{
			x /= magnitude;
			y /= magnitude;
		}
	}

	Vector2 GetNormalised()
	{
		Vector2 result(x, y);
		float magnitude = GetMagnitude();

		if (magnitude != 0)
		{
			result /= magnitude;
		}

		return result;
	}

	Vector2 GetRightAngle()
	{
		return Vector2(-y, x);
	}

	float Dot(Vector2 other)
	{
		return (x * other.x) + (y * other.y);
	}

	float Distance(Vector2 other)
	{
		return (other - *this).GetMagnitude();
	}

	/********************
	 * static functions *
	 ********************/
	static float Dot(Vector2 lhs, Vector2 rhs) // dot product
	{
		return (lhs.x * rhs.x) + (lhs.y * rhs.y);
	}

	// Angle between 2 vectors
	static float Angle(Vector2 lhs, Vector2 rhs)
	{
		lhs.Normalise();
		rhs.Normalise();
		float fDot = Dot(lhs, rhs);

		// Get angle
		float angle = acos(fDot);

		// check if angle is clockwise or anticlockwise 
		Vector2 rightAngle = lhs.GetRightAngle();

		float fRightDot = rhs.Dot(rightAngle);
		if (fRightDot < 0)
			angle = angle + -1.0f;

		return angle;
	}

	// unit vectors
	static Vector2 Zero() { return Vector2(); }
	static Vector2 One() { return Vector2(1, 1); }
	static Vector2 Up() { return Vector2(0, 1); }
	static Vector2 Right() { return Vector2(1, 0); }
	static Vector2 Down() { return Vector2(0, -1); }
	static Vector2 Left() { return Vector2(-1, 0); }

};

struct Vector3
{
	float x;
	float y;
	float z;

	Vector3()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	/**********************
	* Operator Overloads *
	**********************/
	Vector3 operator + (Vector3& other) {
		return Vector3(x + other.x, y + other.y, z + other.z);
	}
	Vector3 operator-(const Vector3& other)
	{
		return Vector3(x - other.x, y - other.y, z + other.z);
	}
	Vector3 operator*(float scale)
	{
		return Vector3(x * scale, y * scale, z * scale);
	}
	Vector3 operator*(const Vector3& other)
	{
		return Vector3(x * other.x, y * other.y, other.z);
	}
	Vector3 operator/(int value)
	{
		return Vector3(x / value, y / value, z / value);
	}

	void operator/=(float value)
	{
		x /= value; y /= value;
	}

	Vector3 operator-()
	{
		return Vector3(-x, -y, -z);
	}

	bool operator==(const Vector3& other) const {
		return x == other.x && y == other.y;
	}

	bool operator!=(const Vector3& other) const {
		return !(*this == other);
	}

	/************
	* Functions *
	*************/
	float GetMagnitude()
	{
		return sqrt(x * x + y * y + z * z);
	}

	void Normalise()
	{
		float magnitude = GetMagnitude();
		if (magnitude != 0)
		{
			x /= magnitude;
			y /= magnitude;
			z /= magnitude;
		}
	}

	Vector3 GetNormalised()
	{
		Vector3 result(x, y, z);
		float magnitude = GetMagnitude();

		if (magnitude != 0)
		{
			result /= magnitude;
		}

		return result;
	}

	float Dot(Vector3 other)
	{
		return (x * other.x) + (y * other.y) + (z * other.z);
	}

	Vector3 Cross(Vector3 other)
	{
		Vector3 result;
		result.x = (y * other.z) - (z * other.y);
		result.y = (z * other.x) - (x * other.z);
		result.z = (x * other.y) - (y * other.x);

		return result;
	}

	float Distance(Vector3 other)
	{
		return (other - *this).GetMagnitude();
	}

	/********************
	 * Static functions *
	 ********************/
	static float Dot(Vector3 lhs, Vector3 rhs) // dot product
	{
		return (lhs.x * rhs.x) + (lhs.y * rhs.y);
	}

	// unit vectors
	static Vector3 Zero() { return Vector3(); }
	static Vector3 One() { return Vector3(1, 1, 1); }
	static Vector3 Up() { return Vector3(0, 1, 0); }
	static Vector3 Down() { return Vector3(0, -1, 0); }
	static Vector3 Left() { return Vector3(-1, 0, 0); }
	static Vector3 Right() { return Vector3(1, 0, 0); }
	static Vector3 Forward() { return Vector3(0, 0, 1); }
	static Vector3 Back() { return Vector3(0, 0, -1); }
};