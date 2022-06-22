#pragma once
#include "Component.h"
#include "Updater.h"

namespace DevSlem::D2DEngine
{
	class Rigidbody final : public Component, public IFixedUpdate
	{
	public:
		/* === Static Members === */
		static const Vector2 Gravity;

	private:
		// Field
		//bool positionUpdated;
		//Vector2 position;
		Vector2 velocity;
		Vector2 totalForce;
		Vector2 momentum;

	public:
		// Field
		float mass;
		float gravityScale;
		bool freezePositionX;
		bool freezePositionY;

		// Constructor
		Rigidbody(DevSlem::D2DEngine::GameObject* gameObject);

		// Getter
		Vector2 Velocity() const;
		Vector2 Position() const;
		float Rotation() const;

		// Setter
		void SetVelocity(const Vector2& value);
		void SetPosition(const Vector2& position);
		void SetRotation(float rotation);

		// Method
		void AddForce(const Vector2& force);
		void MovePosition(const Vector2& deltaPosition);
		virtual void FixedUpdate(float fixedDeltaTime) override;
	};

	inline const Vector2 Rigidbody::Gravity = Vector2(0, 9.8f);
}
