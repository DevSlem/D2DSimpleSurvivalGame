#pragma once
#include <d2d1.h>
#include <wincodec.h>

// 현재 모듈의 시작주소 얻기.
#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

HRESULT LoadBitmapFromResource(ID2D1RenderTarget* pRenderTarget, IWICImagingFactory* pIWICFactory, PCWSTR resourceName, PCWSTR resourceType, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap);

class D2DResource
{
private:
	static IWICImagingFactory* wicImagingFactory;
	static ID2D1Factory* d2dFactory;
	static ID2D1RenderTarget* renderTarget;

public:
	static void SetWICImagingFactory(IWICImagingFactory* value) { wicImagingFactory = value; }
	static IWICImagingFactory* WICImagingFactory() { return wicImagingFactory; }
	static void SetD2DFactory(ID2D1Factory* value) { d2dFactory = value; }
	static ID2D1Factory* D2DFactory() { return d2dFactory; }
	static void SetRenderTarget(ID2D1RenderTarget* value) { renderTarget = value; }
	static ID2D1RenderTarget* RenderTarget() { return renderTarget; }
};

inline IWICImagingFactory* D2DResource::wicImagingFactory = nullptr;
inline ID2D1Factory* D2DResource::d2dFactory = nullptr;
inline ID2D1RenderTarget* D2DResource::renderTarget = nullptr;
