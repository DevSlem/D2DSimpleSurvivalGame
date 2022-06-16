#include "Random.h"

using namespace DevSlem::D2DEngine;

std::mt19937 Random::gen(Random::InstantiateRandomDevice());
std::uniform_real_distribution<float> Random::dist(0, 1);

DevSlem::D2DEngine::Random::Random() { }

std::random_device::result_type DevSlem::D2DEngine::Random::InstantiateRandomDevice()
{
	return std::random_device()();
}

float DevSlem::D2DEngine::Random::Range()
{
	return dist(gen);
}

float DevSlem::D2DEngine::Random::Range(float max)
{
	return dist(gen) * max;
}

float DevSlem::D2DEngine::Random::Range(float min, float max)
{
	return dist(gen) * (max - min) + min;
}

Vector2 DevSlem::D2DEngine::Random::Direction()
{
	float theta = Range(0.0f, (float)(2.0f * PI));
	return Vector2(std::cos(theta), std::sin(theta));
}
