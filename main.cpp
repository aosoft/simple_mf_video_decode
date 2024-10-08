#define _ATL_NO_AUTOMATIC_NAMESPACE

#include <atlbase.h>

ATL::CAtlWinModule _Module;

#include <atlwin.h>

#include <windows.h>
#include <mfapi.h>

#include "common.h"
#include "main_window.h"

#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "mfreadwrite.lib")

class mf_initializer {
private:
    HRESULT _hr;

public:
    mf_initializer()
        : _hr(::MFStartup(MF_VERSION, 0))
    {
    }
    ~mf_initializer()
    {
        if (SUCCEEDED(_hr)) {
            ::MFShutdown();
        }
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
    co_initializer coinit(COINIT_MULTITHREADED);
    mf_initializer mfinit;

    main_window win;
    if (win.Create(nullptr, ATL::CWindow::rcDefault, "simple_mf_video_decode", WS_OVERLAPPEDWINDOW) == nullptr) {
        return 0;
    }
    win.ShowWindow(SW_SHOW);
    while (true) {
        MSG msg;
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
        }
        if (msg.message == WM_QUIT) {
            break;
        }
    }
    _Module.Term();
    return 0;
}
