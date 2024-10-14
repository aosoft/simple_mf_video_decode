#pragma once

#include <d2d1.h>

#include "common.h"
#include "mf_video_decoder.h"

class d2d_renderer {
private:
    com_ptr<ID2D1Factory> _factory;
    com_ptr<ID2D1HwndRenderTarget> _render_target;
    com_ptr<ID2D1Bitmap> _bitmap;

public:
    HRESULT initialize(HWND hwnd);
    HRESULT render(const video_sample_info& info, IMF2DBuffer* buffer);
};