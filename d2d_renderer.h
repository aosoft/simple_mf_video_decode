#pragma once

#include <d2d1.h>

#include "common.h"

class d2d_renderer {
private:
    com_ptr<ID2D1Factory> _factory;
    com_ptr<ID2D1HwndRenderTarget> _render_target;

public:
    HRESULT initialize(HWND hwnd);
    HRESULT render();
};