#include "Component.h"

using namespace DevSlem::D2DEngine;

DevSlem::D2DEngine::Component::Component(DevSlem::D2DEngine::GameObject* gameObject)
{
	enabled = true;
	this->gameObject = gameObject;
}

DevSlem::D2DEngine::Component::~Component() { }

void DevSlem::D2DEngine::Component::OnEnabledSet(bool enabled) { }

void DevSlem::D2DEngine::Component::SetEnabled(bool value)
{
	enabled = value;
	OnEnabledSet(value);
}

bool DevSlem::D2DEngine::Component::Enabled() const
{
	return enabled;
}

GameObject* DevSlem::D2DEngine::Component::GameObject() const
{
	return gameObject;
}

Transform* DevSlem::D2DEngine::Component::Transform() const
{
	return gameObject->Transform();
}
