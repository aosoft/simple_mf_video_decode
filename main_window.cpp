#include "main_window.h"
LRESULT main_window::on_create(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = TRUE;
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