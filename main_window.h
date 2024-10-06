#pragma once

#define _ATL_NO_AUTOMATIC_NAMESPACE

#include <atlbase.h>
#include <atlwin.h>

class main_window : public ATL::CWindowImpl<main_window> {
public:
    DECLARE_WND_CLASS("main_window");

private:
    BEGIN_MSG_MAP(main_window)
    MESSAGE_HANDLER(WM_CREATE, on_create)
    MESSAGE_HANDLER(WM_CLOSE, on_close)
    MESSAGE_HANDLER(WM_DESTROY, on_destroy)
    END_MSG_MAP()

    LRESULT on_create(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT on_close(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT on_destroy(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

};