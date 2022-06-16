#pragma once
#include "D2DEngine.h"

namespace DevSlem::D2DSimpleSurvivalGame
{
	class Input
	{
	private:
		static float horizontal;
		static float vertical;
		static bool space;
		static DevSlem::D2DEngine::Vector2 mousePosition;
		static bool mouseLeftDown;

		Input() {}

	public:
		static void SetHorizontal(float value) { horizontal = value; }
		static void SetVertical(float value) { vertical = value; }
		static void SetSpace(bool value) { space = value; }
		static void SetMousePosition(const DevSlem::D2DEngine::Vector2& position) { mousePosition = position; }
		static void SetMouseLeftDown(bool value) { mouseLeftDown = value; }

		static float Horizontal() { return horizontal; }
		static float Vertical() { return vertical; }
		static bool Space() { return space; }
		static DevSlem::D2DEngine::Vector2 MousePosition() { return mousePosition; }
		static bool MouseLeftDown() { return mouseLeftDown; }

		static void Reset()
		{
			mouseLeftDown = false;
		}
	};

	inline float Input::horizontal = 0;
	inline float Input::vertical = 0;
	inline bool Input::space = false;
	inline DevSlem::D2DEngine::Vector2 Input::mousePosition;
	inline bool Input::mouseLeftDown = false;
}
