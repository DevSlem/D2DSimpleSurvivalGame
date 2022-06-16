#include <algorithm>
#include "Time.h"
#include "Updater.h"
#include "Utility.h"
#include "RectCollider.h"

using namespace DevSlem::D2DEngine;

std::vector<IUpdate*> Updater::updateListeners;
std::vector<IFixedUpdate*> Updater::fixedUpdateListeners;
std::vector<RectCollider*> Updater::colliderListeners;
float Updater::accumulator = 0;

void Updater::TriggerUpdateEvent(float deltaTime)
{
	for (auto listener : updateListeners)
	{
		listener->Update(deltaTime);
	}
}

void Updater::TriggerFixedUpdateEvent(float fixedDeltaTime)
{
	for (auto listener : fixedUpdateListeners)
	{
		listener->FixedUpdate(fixedDeltaTime);
	}
}

// AABB 충돌 처리 References
// [1] https://kareus.tistory.com/m/15
// [2] https://research.ncl.ac.uk/game/mastersdegree/gametechnologies/physicstutorials/4collisiondetection/Physics%20-%20Collision%20Detection.pdf
void Updater::DetectCollision()
{
	for (size_t i = 0; i < colliderListeners.size() - 1; i++)
	{
		auto listener1 = colliderListeners[i];
		if (!listener1->Enabled() || !listener1->Rigidbody()->Enabled())
			continue;

		auto b1 = listener1->Bounds();
		for (size_t j = i + 1; j < colliderListeners.size(); j++)
		{
			auto listener2 = colliderListeners[j];
			auto b2 = listener2->Bounds();
			if (b1.Intersects(b2))
			{
				auto overlaps = b1.Overlaps(b2);
				auto delta_pos = b2.center - b1.center;
				Vector2 normal1;
				Vector2 normal2;

				if (overlaps.x < overlaps.y)
				{
					bool check = delta_pos.x < 0;
					normal1 = check ? Vector2::Left() : Vector2::Right();
					normal2 = check ? Vector2::Right() : Vector2::Left();
				}
				else
				{
					bool check = delta_pos.y < 0;
					normal1 = check ? Vector2::Up() : Vector2::Down();
					normal2 = check ? Vector2::Down() : Vector2::Up();
				}

				Vector2 contact1(delta_pos.x < 0 ? b1.Min().x : b1.Max().x, max(b1.Min().y, b2.Min().y));
				Vector2 contact2(contact1.x, min(b1.Max().y, b2.Max().y));
				Vector2 contact = (contact1 + contact2) / 2.0f; // 두 양 끝점의 평균 위치

				auto rigid1 = listener1->Rigidbody();
				auto rigid2 = listener2->Rigidbody();

				auto collision1 = Collision(
					rigid2, // target rigidbody
					listener2, // target collider
					contact, // collision contact point
					normal1, // target normal vector
					rigid1->Velocity() - rigid2->Velocity() // relative velocity
				);

				auto collision2 = Collision(
					rigid1, // target rigidbody
					listener1, // target collider
					contact, // collision contact point
					normal2, // target normal vector
					rigid2->Velocity() - rigid1->Velocity() // relative velocity
				);

				listener1->TriggerCollision(collision1);
				listener2->TriggerCollision(collision2);
			}
		}
	}
}

void Updater::AddListener(IUpdate* listener)
{
	AddListener(updateListeners, listener);
}

void Updater::AddListener(IFixedUpdate* listener)
{
	AddListener(fixedUpdateListeners, listener);
}

void Updater::AddListener(RectCollider* listener)
{
	AddListener(colliderListeners, listener);
}

void Updater::RemoveListener(IUpdate* listener)
{
	RemoveListener(updateListeners, listener);
}

void Updater::RemoveListener(IFixedUpdate* listener)
{
	RemoveListener(fixedUpdateListeners, listener);
}

void Updater::RemoveListener(RectCollider* listener)
{
	RemoveListener(colliderListeners, listener);
}

void Updater::Update()
{
	Time::SetDeltaTime();
	float deltaTime = Time::DeltaTime();
	float fixedDeltaTime = Time::FixedDeltaTime();

	accumulator += deltaTime;
	bool fixedUpdateTriggered = accumulator >= fixedDeltaTime;
	while (accumulator >= fixedDeltaTime)
	{
		TriggerFixedUpdateEvent(fixedDeltaTime);
		DetectCollision();
		accumulator -= fixedDeltaTime;
	}
	if (fixedUpdateTriggered && accumulator > 0)
	{
		TriggerFixedUpdateEvent(accumulator);
	}
	TriggerUpdateEvent(deltaTime);
}

DevSlem::D2DEngine::IUpdate::IUpdate()
{
	Updater::AddListener(this);
}

DevSlem::D2DEngine::IUpdate::~IUpdate()
{
	Updater::RemoveListener(this);
}

DevSlem::D2DEngine::IFixedUpdate::IFixedUpdate()
{
	Updater::AddListener(this);
}

DevSlem::D2DEngine::IFixedUpdate::~IFixedUpdate()
{
	Updater::RemoveListener(this);
}
