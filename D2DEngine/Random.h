#pragma once

#include <random>
#include "Vector2.h"

#define PI           3.14159265358979323846

namespace DevSlem::D2DEngine
{
	class Random
	{
	private:
		// Field
		static std::mt19937 gen;
		static std::uniform_real_distribution<float> dist;

		// Constructor
		Random();

		// Method
		static std::random_device::result_type InstantiateRandomDevice();

	public:
		// Getter
		static float Range();
		static float Range(float max);
		static float Range(float min, float max);
		static Vector2 Direction();
	};
}
