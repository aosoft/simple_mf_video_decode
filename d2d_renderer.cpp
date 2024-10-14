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

HRESULT d2d_renderer::render(const video_sample_info& info, IMF2DBuffer* buffer)
{
    if (!_render_target) {
        return S_OK;
    }

    if (_bitmap) {
        auto size = _bitmap->GetPixelSize();
        if (size.width != info.width || size.height != info.height) {
            _bitmap.Reset();
        }
    }

    if (!_bitmap) {
        CHECK_HR(_render_target->CreateBitmap(
            D2D1::Size(info.width, info.height),
            D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)),
            &_bitmap))
    }

    BYTE* scan0;
    LONG pitch;
    if (SUCCEEDED(buffer->Lock2D(&scan0, &pitch))) {
        auto sg1 = make_scope_guard([buffer] {
            buffer->Unlock2D();
        });
        _bitmap->CopyFromMemory(nullptr, scan0, pitch);
    }

    _render_target->BeginDraw();
    _render_target->DrawBitmap(_bitmap.Get());
    _render_target->EndDraw();
    return S_OK;
}