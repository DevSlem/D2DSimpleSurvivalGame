#pragma once
#include <d2d1.h>
#include "Rigidbody.h"
#include "Updater.h"
#include "DebugUtility.h"

namespace DevSlem::D2DEngine
{
	enum class DrawingMode
	{
		Draw,
		Fill
	};

	class Renderer abstract : public Component, public IUpdate, public BoundsSetter
	{
	/* === Static Members === */
	private:
		// Field
		static ID2D1RenderTarget* renderTarget;
		static ID2D1SolidColorBrush* solidBrush;

	protected:
		// Getter
		static ID2D1SolidColorBrush* SolidBrush();

	public:
		// Getter
		static ID2D1RenderTarget* RenderTarget();

		// Setter
		static void SetRenderTarget(ID2D1RenderTarget* value);
		static void SetSolidBrush(ID2D1SolidColorBrush* brush);

	/* === Instance Members === */
	private:
		// Field
#if DEBUG_RENDERING
		bool boundsRendered;
#endif

		// Method
		void SetTransform();
		void DrawBounds();

	protected:
		// Method
		virtual Vector2 Position() const override;
		virtual D2D1::ColorF BoundsRenderingColor();
		virtual void Render() = 0;

	public:
		// Constructor
		Renderer(DevSlem::D2DEngine::GameObject* gameObject);

		// Getter
		bool BoundsRendered() const;

		// Setter
		void SetBoundsRendered(bool value);

		// Method
		virtual void Update(float deltaTime) override;
	};

	class BitmapRenderer : public Renderer
	{
	private:
		ID2D1Bitmap* bitmap;

	protected:
		// Method
		virtual void Render() override;

	public:
		// Constructor
		BitmapRenderer(DevSlem::D2DEngine::GameObject* gameObject);

		// Setter
		void SetBitmap(ID2D1Bitmap* bitmap);
	};

	class GeometryRenderer : public Renderer
	{
	private:
		// Field
		ID2D1Geometry* geometry;
		ID2D1Brush* geometryBrush;
		float strokeWidth;
		DrawingMode drawingMode;

	protected:
		// Method
		virtual void Render() override;

	public:
		// Constructor
		GeometryRenderer(DevSlem::D2DEngine::GameObject* gameObject);

		// Setter
		void SetGeometry(ID2D1Geometry* geometry);
		void SetGeometryBrush(ID2D1Brush* geometryBrush);
		void SetStrokeWidth(float value);
		void SetDrawingMode(DrawingMode value);
		//void SetStrokeStyle(ID2D1StrokeStyle* strokeStyle);
	};
}
