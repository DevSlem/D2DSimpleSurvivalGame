#pragma once

#include "Vector2.h"

namespace DevSlem::D2DEngine
{
	class Rigidbody;
	class RectCollider;

	class Collision final
	{
	private:
		Rigidbody* rigidbody;
		RectCollider* collider;

	public:
		const Vector2 contact;
		const Vector2 normal;
		const Vector2 relativeVelocity;

		Collision(Rigidbody* rigidbody = nullptr,
			RectCollider* collider = nullptr,
			const Vector2& contact = Vector2::Zero(),
			const Vector2& normal = Vector2::Zero(),
			const Vector2& relativeVelocity = Vector2::Zero()) :
			rigidbody(rigidbody),
			collider(collider),
			contact(contact),
			normal(normal),
			relativeVelocity(relativeVelocity) { }

		Rigidbody* Rigidbody() { return rigidbody; }
		RectCollider* Collider() { return collider; }
	};
}
