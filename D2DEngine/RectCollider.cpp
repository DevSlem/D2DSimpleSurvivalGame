#include "RectCollider.h"

using namespace DevSlem::D2DEngine;

void RectCollider::ProcessCollision(Collision& collision)
{
	if (isTrigger || collision.Collider()->IsTrigger())
		return;

	// 두 충돌체가 겹칠 시 위치를 조정해서 겹치지 않도록 함.
	auto overlaps = this->Bounds().Overlaps(collision.Collider()->Bounds());
	auto deltaPos = (overlaps + Vector2::One()) * collision.normal;
	this->rigidbody->SetPosition(this->rigidbody->Position() + deltaPos);

	// 속도 변경
	auto v = rigidbody->Velocity();
	auto normal = -collision.normal;
	auto v_n = Vector2::Dot(v, normal) * normal;
	auto v_t = v - v_n;
	float k_f = (physicsMaterial.Friction() + collision.Collider()->physicsMaterial.Friction()) / 2.0f;
	float k_r = (physicsMaterial.Bounciness() + collision.Collider()->physicsMaterial.Bounciness()) / 2.0f;
	rigidbody->SetVelocity((1 - k_f) * v_t - k_r * v_n);
}

void RectCollider::TriggerCollision(Collision& collision)
{
	ProcessCollision(collision);

	for (auto listener : collisionListeners)
	{
		listener->OnCollision(collision);
	}
}

Rigidbody* DevSlem::D2DEngine::RectCollider::Rigidbody() const
{
	return rigidbody;
}

void DevSlem::D2DEngine::RectCollider::AddListener(ICollision* listener)
{
	collisionListeners.push_back(listener);
}

void RectCollider::RemoveListener(ICollision* listener)
{
	collisionListeners.erase(
		std::remove(
			collisionListeners.begin(),
			collisionListeners.end(),
			listener
		),
		collisionListeners.end()
	);
}

Vector2 DevSlem::D2DEngine::RectCollider::Position() const
{
	return rigidbody->Position();
}

void RectCollider::Render()
{
}

DevSlem::D2DEngine::RectCollider::RectCollider(DevSlem::D2DEngine::GameObject* gameObject, DevSlem::D2DEngine::Rigidbody* rigidbody, bool isTrigger) : 
	Renderer(gameObject), rigidbody(rigidbody)
{
	this->isTrigger = isTrigger;
	Updater::AddListener(this);
}

DevSlem::D2DEngine::RectCollider::~RectCollider()
{
	Updater::RemoveListener(this);
}

void DevSlem::D2DEngine::RectCollider::SetIsTrigger(bool value)
{
	isTrigger = value;
}

bool DevSlem::D2DEngine::RectCollider::IsTrigger() const
{
	return isTrigger;
}

D2D1::ColorF DevSlem::D2DEngine::RectCollider::BoundsRenderingColor()
{
	return D2D1::ColorF(D2D1::ColorF::Yellow);
}

DevSlem::D2DEngine::PhysicsMaterial::PhysicsMaterial()
{
	friction = 0;
	bounciness = 0;
}

DevSlem::D2DEngine::PhysicsMaterial::PhysicsMaterial(float friction, float bounciness)
{
	SetFriction(friction);
	SetBounciness(bounciness);
}

void DevSlem::D2DEngine::PhysicsMaterial::SetFriction(float value)
{
	friction = std::clamp(value, 0.0f, 1.0f);
}

void DevSlem::D2DEngine::PhysicsMaterial::SetBounciness(float value)
{
	bounciness = std::clamp(value, 0.0f, 1.0f);
}

float DevSlem::D2DEngine::PhysicsMaterial::Friction() const
{
	return friction;
}

float DevSlem::D2DEngine::PhysicsMaterial::Bounciness() const
{
	return bounciness;
}
