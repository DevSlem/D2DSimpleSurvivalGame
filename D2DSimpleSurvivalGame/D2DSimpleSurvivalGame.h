#pragma once

#include <d2d1.h>
#include <Windows.h>
#include <dwrite.h>
#include <wincodec.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <random>

#include "Animation.h"
#include "Entity.h"
#include "Utility.h"
#include "GameObjects.h"
#include "SoundManager.h"

/* =================================================================================== */
/* D2DSimpleSurvivalGame */
/* =================================================================================== */

namespace DevSlem::D2DSimpleSurvivalGame
{
	class D2DSimpleSurvivalGameApp
	{
	public:
		D2DSimpleSurvivalGameApp();
		~D2DSimpleSurvivalGameApp();
		HRESULT Initialize(HINSTANCE hInstance);
		void RunMessageLoop();

		// Private static methods
	private:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);  // The windows procedure.
		static D2DSimpleSurvivalGameApp* GetMainApp(HWND hWnd) { return reinterpret_cast<D2DSimpleSurvivalGameApp*>(static_cast<LONG_PTR>(GetWindowLongPtrW(hWnd, GWLP_USERDATA))); }

	private:
		HRESULT CreateDeviceIndependentResources();  // Initialize device-independent resources.
		HRESULT CreateDeviceResources();  // Initialize device-dependent resources.
		void DiscardDeviceResources();  // Release device-dependent resource.

		void OnResize(UINT width, UINT height);  // Resize the render target.

		HRESULT SetQuadraticBeziers(ID2D1PathGeometry** path, D2D1_POINT_2F start, const D2D1_QUADRATIC_BEZIER_SEGMENT* segments, UINT bezierCount);
		// Methods for rendering
		HRESULT OnRender(); // Draw all contents.
		HRESULT CreateSharpTrap(const D2D1_RECT_F& rect, int count, ID2D1PathGeometry** trap);

		void DisplayInfo();
		void UpdateSurvivalTime();

		// Sounds
		void InitializeSound(HWND hWnd);
		DevSlem::D2DEngine::Sound backgroundSound;
		DevSlem::D2DEngine::Sound lightAttackSound;
		DevSlem::D2DEngine::Sound heavyAttackSound;

	private:
		// Device Independent Resources
		HWND hWnd;
		ID2D1Factory* pD2DFactory;
		IWICImagingFactory* pWICFactory;
		IDWriteFactory* pDWriteFactory;
		IDWriteTextFormat* pTextFormat;

		// Device Dependent Resources
		ID2D1HwndRenderTarget* pRenderTarget;
		CSoundManager* soundManager;

		ID2D1SolidColorBrush* sceneBrush;
		ID2D1SolidColorBrush* textBrush;

		D2D1_SIZE_F rtSize;

		// Window Setting
		POINT minWindowSize;
		POINT maxWindowSize;

		// Levels
		D2D1_RECT_F targetMovingArea;
		//float groundPosY;

		// GameObjects
		GameObjects* gameObjects;
		void CreateGameObjects();
		void DeleteGameObjects();

		float maxSurvivalTime;
		float survivalTime;
	};
}
 