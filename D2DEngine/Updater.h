#pragma once
#include <vector>
#include "Collision.h"

namespace DevSlem::D2DEngine
{
	class RectCollider;

	class IUpdate abstract
	{
	public:
		IUpdate();
		virtual ~IUpdate();
		virtual void Update(float deltaTime) = 0;
	};

	class IFixedUpdate abstract
	{
	public:
		IFixedUpdate();
		virtual ~IFixedUpdate();
		virtual void FixedUpdate(float fixedDeltaTime) = 0;
	};

	class Updater final
	{
	private:
		static std::vector<IUpdate*> updateListeners;
		static std::vector<IFixedUpdate*> fixedUpdateListeners;
		static std::vector<RectCollider*> colliderListeners;
		static float accumulator;

		static void TriggerUpdateEvent(float deltaTime);
		static void TriggerFixedUpdateEvent(float fixedDeltaTime);
		static void DetectCollision();

		template <class T> static void AddListener(std::vector<T>& vector, T listener)
		{
			if (std::find(vector.begin(), vector.end(), listener) != vector.end())
				return;

			vector.push_back(listener);
		}

		template <class T> static void RemoveListener(std::vector<T>& vector, T listener)
		{
			vector.erase(
				std::remove(
					vector.begin(),
					vector.end(),
					listener
				),
				vector.end()
			);
		}

		Updater() {}

	public:
		static void AddListener(IUpdate* listener);
		static void AddListener(IFixedUpdate* listener);
		static void AddListener(RectCollider* listener);
		static void RemoveListener(IUpdate* listener);
		static void RemoveListener(IFixedUpdate* listener);
		static void RemoveListener(RectCollider* listener);

		static void Update();
	};
}
