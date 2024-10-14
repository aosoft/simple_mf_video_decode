#include "main_window.h"
LRESULT main_window::on_create(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = TRUE;
    if (FAILED(_renderer.initialize(m_hWnd))) {
        CloseWindow(m_hWnd);
        return 0;
    }
    if (FAILED(_decoder.initialize(_source_file_path.c_str()))) {
        CloseWindow(m_hWnd);
        return 0;
    }
    return 0;
}

LRESULT main_window::on_close(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    DestroyWindow();
    bHandled = TRUE;
    return 0;
}

LRESULT main_window::on_destroy(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    PostQuitMessage(0);
    bHandled = TRUE;
    return 0;
}

HRESULT main_window::render()
{
    video_sample_info info = {};
    com_ptr<IMF2DBuffer> buffer;
    if (SUCCEEDED(_decoder.read_sample(info, &buffer)) && buffer) {
        _renderer.render(info, buffer.Get());
    }
    return S_OK;
}