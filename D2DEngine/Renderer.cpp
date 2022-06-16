#include "Renderer.h"

using namespace DevSlem::D2DEngine;

ID2D1RenderTarget* Renderer::renderTarget = nullptr;
ID2D1SolidColorBrush* Renderer::solidBrush = nullptr;

void Renderer::DrawBounds()
{
	if (!BoundsRendered() || solidBrush == nullptr)
		return;

	solidBrush->SetColor(BoundsRenderingColor());
	renderTarget->DrawRectangle((D2D1_RECT_F)this->SizeBounds(), solidBrush);
}

void Renderer::SetTransform()
{
	auto transform = Transform();
	if (transform == nullptr)
		return;

	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	auto translate = D2D1::Matrix3x2F::Translation(transform->position.x + offset.x, transform->position.y + offset.y);
	auto rotation = D2D1::Matrix3x2F::Rotation(transform->rotation);
	auto scale = D2D1::Matrix3x2F::Scale(transform->scale.x, transform->scale.y);
	renderTarget->SetTransform(scale * rotation * translate);
}

ID2D1SolidColorBrush* DevSlem::D2DEngine::Renderer::SolidBrush()
{
	return solidBrush;
}

Vector2 DevSlem::D2DEngine::Renderer::Position() const
{
	return Transform()->position;
}

D2D1::ColorF DevSlem::D2DEngine::Renderer::BoundsRenderingColor()
{
	return D2D1::ColorF(D2D1::ColorF::Blue);
}

ID2D1RenderTarget* DevSlem::D2DEngine::Renderer::RenderTarget()
{
	return renderTarget;
}

void DevSlem::D2DEngine::Renderer::SetRenderTarget(ID2D1RenderTarget* value)
{
	renderTarget = value;
}

void DevSlem::D2DEngine::Renderer::SetSolidBrush(ID2D1SolidColorBrush* brush)
{
	solidBrush = brush;
}

Renderer::Renderer(DevSlem::D2DEngine::GameObject* gameObject) : Component(gameObject)
{
	renderTarget = nullptr;
#if DEBUG_RENDERING
	boundsRendered = false;
#endif
}

void Renderer::Update(float deltaTime)
{
	if (!Enabled())
		return;

	if (renderTarget == nullptr)
		return;

	SetTransform();
	DrawBounds();
	Render();
}

void DevSlem::D2DEngine::Renderer::SetBoundsRendered(bool value)
{
#if DEBUG_RENDERING
	this->boundsRendered = value;
#endif
}

bool DevSlem::D2DEngine::Renderer::BoundsRendered() const
{
#if DEBUG_RENDERING
	return boundsRendered;
#else
	return false;
#endif
}

BitmapRenderer::BitmapRenderer(DevSlem::D2DEngine::GameObject* gameObject) : Renderer(gameObject)
{
	bitmap = nullptr;
}

void DevSlem::D2DEngine::BitmapRenderer::SetBitmap(ID2D1Bitmap* bitmap)
{
	this->bitmap = bitmap;
	if (size == Vector2::Zero())
	{
		auto temp = bitmap->GetPixelSize();
		size = Vector2((float)temp.width, (float)temp.height);
	}
}

void BitmapRenderer::Render()
{
	if (bitmap == nullptr)
		return;

	RenderTarget()->DrawBitmap(bitmap, this->SizeBounds());
}

GeometryRenderer::GeometryRenderer(DevSlem::D2DEngine::GameObject* gameObject) : Renderer(gameObject)
{
	geometry = nullptr;
	geometryBrush = nullptr;
	strokeWidth = 1;
	drawingMode = DrawingMode::Draw;
	//strokeStyle = nullptr;
}

void DevSlem::D2DEngine::GeometryRenderer::SetGeometry(ID2D1Geometry* geometry)
{
	this->geometry = geometry;
}

void DevSlem::D2DEngine::GeometryRenderer::SetGeometryBrush(ID2D1Brush* geometryBrush)
{
	this->geometryBrush = geometryBrush;
}

void DevSlem::D2DEngine::GeometryRenderer::SetStrokeWidth(float value)
{
	strokeWidth = value;
}

void DevSlem::D2DEngine::GeometryRenderer::SetDrawingMode(DrawingMode value)
{
	drawingMode = value;
}

void GeometryRenderer::Render()
{
	if (geometry == nullptr || geometryBrush == nullptr)
		return;

	switch (drawingMode)
	{
	case DrawingMode::Draw:
		RenderTarget()->DrawGeometry(geometry, geometryBrush, strokeWidth);
		break;
	case DrawingMode::Fill:
		RenderTarget()->FillGeometry(geometry, geometryBrush);
		break;
	default:
		break;
	}
}