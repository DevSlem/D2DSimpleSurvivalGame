#pragma once
#include "Utility.h"

namespace DevSlem::D2DEngine
{
	class Transform
	{
	public:
		Vector2 position;
		Vector2 scale;
		float rotation;

		Transform() : scale(1, 1), rotation(0) {}
	};

}
