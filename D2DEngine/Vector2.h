#pragma once

#include <d2d1.h>

namespace DevSlem::D2DEngine
{
	struct Vector2
	{
		/* === Static Members === */
	public:
		// Getter
		static Vector2 Zero();
		static Vector2 One();
		static Vector2 Left();
		static Vector2 Right();
		static Vector2 Up();
		static Vector2 Down();

		// Method
		static float Distance(const Vector2& a, const Vector2& b) noexcept;
		static float Dot(const Vector2& a, const Vector2& b) noexcept;
		static Vector2 Lerp(const Vector2& start, const Vector2& end, float t) noexcept;

		/* === Instnace Members === */
	public:
		// Field
		float x;
		float y;

		// Constructor
		Vector2();
		Vector2(float x, float y);
		Vector2(const D2D1_POINT_2F& point);
		Vector2(const D2D1_SIZE_F& size);

		// Getter
		float Magnitude() const;
		Vector2 Normalized() const;
		Vector2 Abs() const;
		Vector2 Direction(const Vector2& destination) const;

		// Setter
		void Set(float x, float y);

		// Operator
		Vector2 operator+(const Vector2& v) const noexcept;
		Vector2 operator-(const Vector2& v) const noexcept;
		Vector2 operator*(const Vector2& v) const noexcept;
		Vector2 operator*(float value) const noexcept;
		Vector2 operator/(const Vector2& v) const noexcept;
		Vector2 operator/(float value) const noexcept;
		Vector2 operator+() const noexcept;
		Vector2 operator-() const noexcept;
		bool operator==(const Vector2& v) const noexcept;
		bool operator!=(const Vector2& v) const noexcept;
		Vector2& operator+=(const Vector2& v) noexcept;
		Vector2& operator-=(const Vector2& v) noexcept;
		Vector2& operator*=(const Vector2& v) noexcept;
		Vector2& operator*=(float value) noexcept;
		Vector2& operator/=(const Vector2& v) noexcept;
		Vector2& operator/=(float value) noexcept;

		operator D2D1_POINT_2F() const noexcept;
		operator D2D1_SIZE_F() const noexcept;
	};

	// Vector2 Operator
	Vector2 operator*(float value, const Vector2& v) noexcept;
	Vector2 operator/(float value, const Vector2& v) noexcept;
}
