#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>
#include <ostream>

template <typename Real = double>
struct Vector2
{
	Real x;
	Real y;

	Vector2() : x(0), y(0) {}
	Vector2(const Vector2& v) : x(v.x), y(v.y) {}
	Vector2(Real x, Real y) : x(x), y(y) {}

	Vector2& operator+=(const Vector2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	Vector2& operator-=(const Vector2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	Vector2& operator*=(Real f)
	{
		x *= f;
		y *= f;
		return *this;
	}

	Vector2& normalize()
	{
		return *this *= 1.0 / norm();
	}

	double dot(const Vector2& v) const
	{
		return x * v.x + y * v.y;
	}

	double sqNorm() const
	{
		return x * x + y * y;
	}

	double norm() const
	{
		return sqrt(x * x + y * y);
	}

	friend std::ostream& operator<<(std::ostream& os, const Vector2& v)
	{
		os << v.x << " " << v.y;
		return os;
	}

	friend std::istream& operator>>(std::istream& is, Vector2& v)
	{
		is >> v.x >> v.y;
		return is;
	}
};

#endif
