#include "GameObject.h"

using namespace DevSlem::D2DEngine;

void DevSlem::D2DEngine::GameObject::OnActiveSet(bool isActive) { }

DevSlem::D2DEngine::GameObject::GameObject()
{
	isActive = true;
}

DevSlem::D2DEngine::GameObject::~GameObject() { }

void DevSlem::D2DEngine::GameObject::SetActive(bool value)
{
	isActive = value;
	OnActiveSet(value);
}

bool DevSlem::D2DEngine::GameObject::IsActive() const
{
	return isActive;
}

Transform* DevSlem::D2DEngine::GameObject::Transform()
{
	return &transform;
}

std::wstring_view DevSlem::D2DEngine::GameObject::Name()
{
	return name;
}

void DevSlem::D2DEngine::GameObject::SetName(std::wstring_view value)
{
	name = value;
}

void DevSlem::D2DEngine::GameObject::CreateDeviceIndependentResources() { }
void DevSlem::D2DEngine::GameObject::ReleaseDeviceIndependentResources() { }
void DevSlem::D2DEngine::GameObject::CreateDeviceResources() { }
void DevSlem::D2DEngine::GameObject::ReleaseDeviceResources() { }
