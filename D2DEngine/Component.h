#pragma once

#include "GameObject.h"

namespace DevSlem::D2DEngine
{
	class Component abstract
	{
	private:
		// Field
		bool enabled;
		GameObject* gameObject;

	protected:
		// Method
		virtual void OnEnabledSet(bool enabled);

	public:
		// Constructor
		Component(GameObject* gameObject);
		virtual ~Component();

		// Getter
		bool Enabled() const;
		GameObject* GameObject() const;
		Transform* Transform() const;

		// Setter
		void SetEnabled(bool value);
	};
}
