#pragma once
#include <algorithm>
#include "Rigidbody.h"
#include "Renderer.h"
#include "Collision.h"
#include "Updater.h"
#include "Utility.h"

namespace DevSlem::D2DEngine
{
	//class GameObject;

	class ICollision abstract
	{
	public:
		virtual void OnCollision(Collision& collision) = 0;
	};

	struct PhysicsMaterial
	{
	private:
		// Field
		float friction;
		float bounciness;

	public:
		// Constructor
		PhysicsMaterial();
		PhysicsMaterial(float friction, float bounciness);

		// Getter
		float Friction() const;
		float Bounciness() const;

		// Setter
		void SetFriction(float value);
		void SetBounciness(float value);
	};

	class RectCollider final : public Renderer
	{
	private:
		// Field
		Rigidbody* rigidbody;
		std::vector<ICollision*> collisionListeners;
		bool isTrigger;

		// Method
		void ProcessCollision(Collision& collision);

	protected:
		// Method
		virtual Vector2 Position() const override;
		virtual D2D1::ColorF BoundsRenderingColor() override;
		virtual void Render() override;

	public:
		// Field
		PhysicsMaterial physicsMaterial;

		RectCollider(DevSlem::D2DEngine::GameObject* gameObject, Rigidbody* rigidbody, bool isTrigger = false);
		~RectCollider();

		void SetIsTrigger(bool value);
		bool IsTrigger() const;
		void TriggerCollision(Collision& collision);

		Rigidbody* Rigidbody() const;
		void AddListener(ICollision* listener);
		void RemoveListener(ICollision* listener);
	};

}
