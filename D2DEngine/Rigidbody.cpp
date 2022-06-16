#include "Rigidbody.h"

using namespace DevSlem::D2DEngine;

Rigidbody::Rigidbody(DevSlem::D2DEngine::GameObject* gameObject) : Component(gameObject)
{
	gravityScale = 1;
	mass = 1;

	positionUpdated = false;
	freezePositionX = false;
	freezePositionY = false;
}

void Rigidbody::AddForce(const Vector2& force)
{
	if (!Enabled())
		return;

	totalForce += force;
}

void Rigidbody::SetVelocity(const Vector2& value)
{
	velocity = value;
	momentum = mass * velocity;
}

Vector2 DevSlem::D2DEngine::Rigidbody::Velocity() const
{
	return velocity;
}

Vector2 DevSlem::D2DEngine::Rigidbody::Position() const
{
	return Transform()->position;
}

void DevSlem::D2DEngine::Rigidbody::SetPosition(const Vector2& position)
{
	//this->position = position;
	//positionUpdated = true;

	auto temp = Transform()->position;
	Transform()->position.Set(freezePositionX ? temp.x : position.x, freezePositionY ? temp.y : position.y);
}

void DevSlem::D2DEngine::Rigidbody::MovePosition(const Vector2& deltaPosition)
{
	SetPosition(Position() + deltaPosition);
}

void DevSlem::D2DEngine::Rigidbody::SetRotation(float rotation)
{
	Transform()->rotation = rotation;
}

float DevSlem::D2DEngine::Rigidbody::Rotation() const
{
	return Transform()->rotation;
}

void Rigidbody::FixedUpdate(float fixedDeltaTime)
{
	if (!Enabled())
		return;

	// 중력 적용
	AddForce(mass * gravityScale * Gravity);

	// 변경된 position 반영
	//if (positionUpdated)
	//{
	//	auto temp = Transform()->position;
	//	position.Set(freezePositionX ? temp.x : position.x, freezePositionY ? temp.y : position.y);
	//	Transform()->position = position;
	//	positionUpdated = false;
	//}

	// 물리 상태 처리
	momentum += totalForce * fixedDeltaTime;
	if (freezePositionX)
	{
		momentum.x = 0;
	}
	if (freezePositionY)
	{
		momentum.y = 0;
	}
	velocity = momentum / mass;
	Transform()->position += velocity * fixedDeltaTime;

	totalForce = Vector2::Zero();
}