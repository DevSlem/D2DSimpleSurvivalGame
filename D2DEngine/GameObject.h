#pragma once
#include <string>
#include "Transform.h"

namespace DevSlem::D2DEngine
{
	class GameObject abstract
	{
	private:
		// field
		Transform transform;
		bool isActive;
		std::wstring name;

	protected:
		// Method
		virtual void OnActiveSet(bool isActive);

	public:
		// Constructor
		GameObject();
		virtual ~GameObject();

		// Getter
		bool IsActive() const;
		Transform* Transform();
		std::wstring_view Name();
		
		// Setter
		void SetActive(bool value);
		void SetName(std::wstring_view value);

		// Method
		virtual void CreateDeviceIndependentResources();
		virtual void ReleaseDeviceIndependentResources();
		virtual void CreateDeviceResources();
		virtual void ReleaseDeviceResources();
	};
}
