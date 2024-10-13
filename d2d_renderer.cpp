#include "d2d_renderer.h"

#pragma comment(lib, "d2d1.lib")

HRESULT d2d_renderer::initialize(HWND hwnd)
{
    D2D1_FACTORY_OPTIONS options = {};
#ifdef _DEBUG
    options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

    CHECK_HR(D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        __uuidof(ID2D1Factory),
        &options,
        &_factory));

    RECT rc;
    GetClientRect(hwnd, &rc);
    CHECK_HR(_factory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(rc.right, rc.bottom)),
        &_render_target));

    return S_OK;
}

HRESULT d2d_renderer::render()
{
    if (!_render_target) {
        return S_OK;
    }

    com_ptr<ID2D1SolidColorBrush> brush;
    _render_target->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.0f, 0.0f), &brush);

    _render_target->BeginDraw();
    _render_target->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(100.0f, 100.0f), 50.0f, 50.0f), brush.Get(), 4.0f);
    _render_target->EndDraw();
    return S_OK;
}