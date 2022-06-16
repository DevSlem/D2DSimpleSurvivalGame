#pragma once

#include <d2d1.h>
#include "Vector2.h"

namespace DevSlem::D2DEngine
{
	struct Bounds
	{
	public:
		// Field
		Vector2 center;
		Vector2 extents;

		// Constructor
		Bounds();
		Bounds(const Vector2& center, const Vector2& extents);
		Bounds(const D2D1_RECT_F& rect);

		// Getter
		Vector2 Min() const;
		Vector2 Max() const;

		// Method
		bool Contains(Vector2 position);
		bool Intersects(const Bounds& bounds);
		Vector2 Overlaps(const Bounds& bounds);

		// Operator
		operator D2D1_RECT_F() const noexcept;
	};
}