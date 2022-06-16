#pragma once

#include "Bounds.h"

namespace DevSlem::D2DEngine
{
	class BoundsSetter abstract
	{
	protected:
		// Field
		Vector2 offset;
		Vector2 size;

	protected:
		// Method
		virtual Vector2 Position() const = 0;

	public:
		// Getter
		Vector2 Offset() const;
		Vector2 Size() const;
		DevSlem::D2DEngine::Bounds Bounds() const;

		// Setter
		void SetOffset(const Vector2& offset);
		void SetSize(const Vector2& size);
		DevSlem::D2DEngine::Bounds SizeBounds();
	};
}
