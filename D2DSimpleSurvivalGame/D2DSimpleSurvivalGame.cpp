#include <algorithm>
//#include <winuser.h>
#include "D2DSimpleSurvivalGame.h"
#include "Time.h"
#include "Input.h"
#include "D2DResource.h"
//#include "Utility.h"

using namespace DevSlem::D2DSimpleSurvivalGame;
using namespace DevSlem::D2DEngine;

D2DSimpleSurvivalGameApp::D2DSimpleSurvivalGameApp()
{
    // Device Independent Resources
    hWnd = nullptr;
    pD2DFactory = nullptr;
    pWICFactory = nullptr;
    pDWriteFactory = nullptr;
    pTextFormat = nullptr;
    soundManager = nullptr;

    // Device Dependent Resources
    pRenderTarget = nullptr;
    sceneBrush = nullptr;
    textBrush = nullptr;

    // game objects
    gameObjects = nullptr;
    
    // Window Setting
    minWindowSize = Point2L(1280, 720);
    maxWindowSize = Point2L(1280, 720);

    // Levels
    targetMovingArea = D2D1::RectF(300, 500, 980, 600);

    maxSurvivalTime = 0;
    survivalTime = 0;

    rtSize = { 0, 0 };
}

D2DSimpleSurvivalGameApp::~D2DSimpleSurvivalGameApp()
{
    DiscardDeviceResources();
    SAFE_RELEASE(pD2DFactory);
    SAFE_RELEASE(pWICFactory);
    SAFE_RELEASE(pDWriteFactory);
    SAFE_RELEASE(pTextFormat);

    D2DResource::SetD2DFactory(nullptr);
    D2DResource::SetWICImagingFactory(nullptr);

    soundManager->release();

    DeleteGameObjects();
}

HRESULT D2DSimpleSurvivalGameApp::Initialize(HINSTANCE hInstance)
{
    HRESULT hr;

    // 장치 독립적 자원을 생성함.
    hr = CreateDeviceIndependentResources();

    if (SUCCEEDED(hr))
    {
        // 윈도우 클래스를 등록함..
        WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = D2DSimpleSurvivalGameApp::WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = sizeof(LONG_PTR);
        wcex.hInstance = hInstance;
        wcex.hbrBackground = NULL;
        wcex.lpszMenuName = NULL;
        wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
        wcex.lpszClassName = L"D2DSimpleSurvivalGame";
        RegisterClassEx(&wcex);

        // 윈도우를 생성함.
        this->hWnd = CreateWindow(
            L"D2DSimpleSurvivalGame", L"D2D Simple Survival Game by DevSlem",
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
            1280, 720, NULL, NULL, hInstance, this
        );
        hr = this->hWnd != nullptr ? S_OK : E_FAIL;
        if (SUCCEEDED(hr))
        {
            CreateGameObjects();

            ShowWindow(this->hWnd, SW_SHOWNORMAL);
            UpdateWindow(this->hWnd);
        }
    }

    return hr;
}

void D2DSimpleSurvivalGameApp::RunMessageLoop()
{
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT D2DSimpleSurvivalGameApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_CREATE)
    {
        LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
        D2DSimpleSurvivalGameApp* pMainApp = (D2DSimpleSurvivalGameApp*)pcs->lpCreateParams;
        SetWindowLongPtrW(hWnd, GWLP_USERDATA, PtrToUlong(pMainApp));

        return 1;
    }

    if (message == WM_DESTROY)
    {
        ::PostQuitMessage(0);
        return 1;
    }

    auto app = GetMainApp(hWnd);
    LRESULT result = 0;
    if (app == nullptr)
    {
        result = DefWindowProc(hWnd, message, wParam, lParam);
        return result;
    }


    switch (message)
    {
    case WM_SIZE:
    {
        app->OnResize(LOWORD(lParam), HIWORD(lParam));
        break;
    }
    case WM_DISPLAYCHANGE:
    {
        ::InvalidateRect(hWnd, nullptr, false);
        break;
    }
    case WM_PAINT:
    {
        app->OnRender();
        //::ValidateRect(hWnd, nullptr);
        break;
    }
    case WM_GETMINMAXINFO:
    {
        MINMAXINFO* mmi = (MINMAXINFO*)lParam;
        mmi->ptMinTrackSize = app->minWindowSize;
        mmi->ptMaxTrackSize = app->maxWindowSize;
        break;
    }
    case WM_LBUTTONDOWN:
    {
        Input::SetMouseLeftDown(true);
        break;
    }
    case WM_MOUSEMOVE:
    {
        Input::SetMousePosition(Vector2(LOWORD(lParam), HIWORD(lParam)));
        break;
    }
    case WM_KEYDOWN:
    {
        auto key = static_cast<SHORT>(wParam);
        switch (key)
        {
        // Set move inputs
        case 'w':
        case 'W':
            Input::SetVertical(1.0f);
            break;
        case 'a':
        case 'A':
            Input::SetHorizontal(-1.0f);
            break;
        case 's':
        case 'S':
            Input::SetVertical(-1.0f);
            break;
        case 'd':
        case 'D':
            Input::SetHorizontal(1.0f);
            break;
        // Set volume
        case VK_SPACE:
            Input::SetSpace(true);
            break;
        case VK_UP:
            Sound::ChangeSystemVolume(10);
            break;
        case VK_DOWN:
            Sound::ChangeSystemVolume(-10);
            break;
        case 'p':
        case 'P':
            Sound::SetMute(!Sound::IsMute());
            break;
        default:
            break;
        }
        break;
    }
    case WM_KEYUP:
    {
        auto key = static_cast<SHORT>(wParam);
        switch (key)
        {
            // Set move inputs
        case 'w':
        case 'W':
            Input::SetVertical(0);
            break;
        case 'a':
        case 'A':
            Input::SetHorizontal(0);
            break;
        case 's':
        case 'S':
            Input::SetVertical(0);
            break;
        case 'd':
        case 'D':
            Input::SetHorizontal(0);
            break;
        case VK_SPACE:
            Input::SetSpace(false);
            break;
        default:
            break;
        }
        break;
    }
    default:
    {
        //Input::SetMouseLeftDown(false);s
        result = DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }
    }

    return result;
}


HRESULT D2DSimpleSurvivalGameApp::CreateDeviceIndependentResources()
{
    HRESULT hr = S_OK;

    // D2D 팩토리를 생성함.
    hr = ::D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
    hr = ::CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pWICFactory));

    if (SUCCEEDED(hr))
    {
        D2DResource::SetD2DFactory(pD2DFactory);
        D2DResource::SetWICImagingFactory(pWICFactory);
        hr = ::DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pDWriteFactory));
    }
    if (SUCCEEDED(hr))
    {
        hr = pDWriteFactory->CreateTextFormat(L"Verdana", nullptr, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 18.0F, L"en-us", &pTextFormat);
    }

    Time::Initialize();

    return hr;
}

HRESULT D2DSimpleSurvivalGameApp::CreateDeviceResources()
{
    HRESULT hr = S_OK;

    if (pRenderTarget == nullptr)
    {
        RECT rc;
        GetClientRect(hWnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

        // D2D 렌더타겟을 생성함.
        hr = pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hWnd, size), &pRenderTarget);
        ID2D1SolidColorBrush* solidBrush;
        hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White, 1.f), &solidBrush);

        if (SUCCEEDED(hr))
        {
            //hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White, 1.f), &sceneBrush);
            D2DResource::SetRenderTarget(pRenderTarget);
            Renderer::SetRenderTarget(pRenderTarget);
            Renderer::SetSolidBrush(solidBrush);

            if (gameObjects != nullptr)
            {
                gameObjects->CreateDeviceResources();
            }
        }

        if (SUCCEEDED(hr))
        {
            hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &textBrush);
        }
    }

    return hr;
}

void D2DSimpleSurvivalGameApp::DiscardDeviceResources()
{
    SAFE_RELEASE(pRenderTarget);
    SAFE_RELEASE(sceneBrush);
    SAFE_RELEASE(textBrush);

    D2DResource::SetRenderTarget(nullptr);
    Renderer::SetRenderTarget(nullptr);
    Renderer::SetSolidBrush(nullptr);

    if (gameObjects != nullptr)
    {
        gameObjects->ReleaseDeviceResources();
    }
}

void D2DSimpleSurvivalGameApp::CreateGameObjects()
{
    gameObjects = new GameObjects;
    if (gameObjects != nullptr)
    {
        InitializeSound(hWnd);
        backgroundSound.Play(true);
        gameObjects->CreateDeviceIndependentResources();
    }
}

void D2DSimpleSurvivalGameApp::DeleteGameObjects()
{
    if (gameObjects != nullptr)
    {
        gameObjects->ReleaseDeviceIndependentResources();
        delete gameObjects;
    }
}

void D2DSimpleSurvivalGameApp::OnResize(UINT width, UINT height)
{
    if (pRenderTarget != nullptr)
    {
        // 참고: 아래의 함수는 실행 시에 실패할 수도 있지만, 여기에서 실행 에러를 체크하지 않고 넘어가도 됨. 다음번 EndDraw 함수가 호출될 때에 에러가 확인될 것이므로 그때 처리하면 됨.
        pRenderTarget->Resize(D2D1::SizeU(width, height));
    }
}

HRESULT D2DSimpleSurvivalGameApp::SetQuadraticBeziers(ID2D1PathGeometry** path, D2D1_POINT_2F start, const D2D1_QUADRATIC_BEZIER_SEGMENT* segments, UINT bezierCount)
{
    SAFE_RELEASE(*path);
    pD2DFactory->CreatePathGeometry(path);

    ID2D1GeometrySink* sink = nullptr;
    auto hr = (*path)->Open(&sink);
    if (SUCCEEDED(hr))
    {
        sink->BeginFigure(start, D2D1_FIGURE_BEGIN_FILLED);
        sink->AddQuadraticBeziers(segments, bezierCount);
        sink->EndFigure(D2D1_FIGURE_END_OPEN);
        hr = sink->Close();
        SAFE_RELEASE(sink);
    }

    return hr;
}

HRESULT D2DSimpleSurvivalGameApp::OnRender()
{
    HRESULT hr = S_OK;
    hr = CreateDeviceResources();

    if (SUCCEEDED(hr))
    {
        pRenderTarget->BeginDraw();

        pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
        pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

        Updater::Update();

        // Update records
        UpdateSurvivalTime();
        DisplayInfo();

        hr = pRenderTarget->EndDraw();

        Input::Reset();
    }

    if (hr == D2DERR_RECREATE_TARGET)
    {
        hr = S_OK;
        DiscardDeviceResources();
    }

    return hr;
}

HRESULT D2DSimpleSurvivalGameApp::CreateSharpTrap(const D2D1_RECT_F& rect, int count, ID2D1PathGeometry** trap)
{
    if (count <= 0)
        return E_FAIL;

    HRESULT hr = pD2DFactory->CreatePathGeometry(trap);
    if (SUCCEEDED(hr))
    {
        ID2D1GeometrySink* sink = nullptr;
        hr = (*trap)->Open(&sink);
        if (SUCCEEDED(hr))
        {
            sink->SetFillMode(D2D1_FILL_MODE_WINDING);
            sink->BeginFigure(D2D1::Point2F(rect.left, rect.bottom), D2D1_FIGURE_BEGIN_FILLED);

            float interval = (rect.right - rect.left) / count;
            std::vector<D2D1_POINT_2F> points;
            for (int i = 0; i < count; i++)
            {
                points.push_back(D2D1::Point2F(rect.left + interval / 2 + interval * i, rect.top));
                points.push_back(D2D1::Point2F(rect.left + interval * (i + 1), rect.bottom));
            }
            sink->AddLines(points.data(), points.size());
            sink->EndFigure(D2D1_FIGURE_END_CLOSED);
        }
        hr = sink->Close();
        SAFE_RELEASE(sink);
    }

    return hr;
}

void D2DSimpleSurvivalGameApp::DisplayInfo()
{
    pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    auto mousePos = Input::MousePosition();
    auto position = gameObjects->player.Transform()->position;
    std::wstring info = ::format_wstring(
L"플레이어 위치: (%.2f, %.2f)\n\
체력: %.2f/%.2f\n\
최대 생존 시간: %.2f\n\
생존 시간: %.2f\n\
사운드 크기: %d\n\
사운드 %s\n\
마우스 위치: (%.0f, %.0f)",
        position.x, position.y, 
        gameObjects->player.entity.Health(), gameObjects->player.entity.MaxHealth(), 
        maxSurvivalTime, 
        survivalTime, 
        Sound::SystemVolume(), 
        Sound::IsMute() ? L"Off" : L"On",
        mousePos.x, mousePos.y);

    auto rtSize = Renderer::RenderTarget()->GetSize();
    pRenderTarget->DrawText(info.c_str(), info.size(), pTextFormat, D2D1::RectF(rtSize.width / 2.0f -150, 10, rtSize.width / 2.0f + 150, 200), textBrush);
}

void D2DSimpleSurvivalGameApp::UpdateSurvivalTime()
{
    survivalTime += Time::DeltaTime();
    if (gameObjects->player.entity.IsDead())
    {
        maxSurvivalTime = max(maxSurvivalTime, survivalTime);
        gameObjects->player.entity.SetHealth(gameObjects->player.entity.MaxHealth());
        survivalTime = 0;
    }
}

void D2DSimpleSurvivalGameApp::InitializeSound(HWND hWnd)
{
    soundManager = new CSoundManager;
    if (!soundManager->init(hWnd))
        return;

    Sound::SetSoundManager(soundManager);
    Sound::SetSystemVolume(90);

    if (!backgroundSound.CreateSound(L"./assets/sounds/background.wav"))
        return;
}
