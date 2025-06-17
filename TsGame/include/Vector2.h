#pragma once
#include <math.h>

class Vector2
{
public:
	double x = 0;
	double y = 0;
public:
	Vector2() = default;
	~Vector2() = default;

	Vector2(double x, double y) : x(x), y(y) {}

	Vector2 operator+(const Vector2& rhs) const {
		return Vector2(x + rhs.x, y + rhs.y);
	}

	Vector2 operator-(const Vector2& rhs) const {
		return Vector2(x - rhs.x, y - rhs.y);
	}

	Vector2 operator+=(const Vector2& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	Vector2 operator-=(const Vector2& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	Vector2 operator*(double rhs) const {
		return Vector2(x * rhs, y * rhs);
	}

	Vector2 operator/(double rhs) const {
		return Vector2(x / rhs, y / rhs);
	}

	Vector2 operator*=(double rhs) {
		x *= rhs;
		y *= rhs;
		return *this;
	}

	Vector2 operator/=(double rhs) {
		x /= rhs;
		y /= rhs;
		return *this;
	}

	double operator*(const Vector2& rhs) const {
		return x * rhs.x + y * rhs.y;
	}

	Vector2 operator/(const Vector2& rhs) const {
		return Vector2(x / rhs.x, y / rhs.y);
	}

	Vector2 operator*=(const Vector2& rhs) {
		x *= rhs.x;
		y *= rhs.y;
		return *this;
	}

	Vector2 operator/=(const Vector2& rhs) {
		x /= rhs.x;
		y /= rhs.y;
		return *this;
	}

	bool operator==(const Vector2& rhs) const {
		return x == rhs.x && y == rhs.y;
	}

	bool operator!=(const Vector2& rhs) const {
		return x != rhs.x || y != rhs.y;
	}

	bool operator>(const Vector2& rhs) const {
		return length() > rhs.length();
	}
	
	bool operator<(const Vector2& rhs) const {
		return length() < rhs.length();
	}

	double length() const {
		return sqrt(x * x + y * y);
	}

	Vector2 normalize() const {
		double len = length();
		if (len == 0) return Vector2(0, 0);

		return Vector2(x / len, y / len);
	}

	bool approx_zero() const {
		return length() < 0.00001;
	}
};

