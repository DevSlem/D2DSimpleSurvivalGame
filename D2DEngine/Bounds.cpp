#include "Bounds.h"
#include "Utility.h"

using namespace DevSlem::D2DEngine;

DevSlem::D2DEngine::Bounds::Bounds() { }

DevSlem::D2DEngine::Bounds::Bounds(const Vector2& center, const Vector2& extents) : 
	center(center), extents(extents.Abs()) { }

DevSlem::D2DEngine::Bounds::Bounds(const D2D1_RECT_F& rect) : 
	center(DevSlem::D2DEngine::GetCenterOfRect(rect)), 
	extents((rect.right - rect.left) / 2.0f, (rect.bottom - rect.top) / 2.0f) { }

bool DevSlem::D2DEngine::Bounds::Contains(Vector2 position)
{
	auto rect = (D2D1_RECT_F)(*this);
	return position.x >= rect.left && position.y >= rect.top && position.x <= rect.right && position.y <= rect.bottom;
}

bool DevSlem::D2DEngine::Bounds::Intersects(const Bounds& bounds)
{
	auto distances = (this->center - bounds.center).Abs();
	auto extents_sum = this->extents + bounds.extents;
	return distances.x <= extents_sum.x && distances.y <= extents_sum.y;
}

Vector2 DevSlem::D2DEngine::Bounds::Overlaps(const Bounds& bounds)
{
	auto distances = (this->center - bounds.center).Abs();
	auto extents_sum = this->extents + bounds.extents;
	return extents_sum - distances;
}

Vector2 DevSlem::D2DEngine::Bounds::Min() const
{
	return center - extents;
}

Vector2 DevSlem::D2DEngine::Bounds::Max() const
{
	return center + extents;
}

DevSlem::D2DEngine::Bounds::operator D2D1_RECT_F() const noexcept
{
	D2D1_RECT_F rect;
	rect.left = center.x - extents.x;
	rect.top = center.y - extents.y;
	rect.right = center.x + extents.x;
	rect.bottom = center.y + extents.y;
	return rect;
}
