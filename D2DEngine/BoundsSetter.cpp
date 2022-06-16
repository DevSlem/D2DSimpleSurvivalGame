#include "BoundsSetter.h"

using namespace DevSlem::D2DEngine;

Vector2 DevSlem::D2DEngine::BoundsSetter::Offset() const
{
	return offset;
}

Vector2 DevSlem::D2DEngine::BoundsSetter::Size() const
{
	return size;
}

void DevSlem::D2DEngine::BoundsSetter::SetOffset(const Vector2& offset)
{
	this->offset = offset;
}

void DevSlem::D2DEngine::BoundsSetter::SetSize(const Vector2& size)
{
	this->size = size.Abs();
}

DevSlem::D2DEngine::Bounds DevSlem::D2DEngine::BoundsSetter::Bounds() const
{
	return DevSlem::D2DEngine::Bounds(Position() + offset, size / 2.0f);
}

DevSlem::D2DEngine::Bounds DevSlem::D2DEngine::BoundsSetter::SizeBounds()
{
	return DevSlem::D2DEngine::Bounds(Vector2::Zero(), size / 2.0f);
}
