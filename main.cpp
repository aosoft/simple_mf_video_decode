#define _ATL_NO_AUTOMATIC_NAMESPACE

#include <atlbase.h>

ATL::CAtlWinModule _Module;

#include <atlwin.h>

#include <mfapi.h>
#include <shellapi.h>
#include <windows.h>

#include "common.h"
#include "main_window.h"

#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "Shell32.lib")

inline auto mf_initialize()
{
    HRESULT hr = ::MFStartup(MF_VERSION, 0);
    return make_scope_guard([hr] {
        if (SUCCEEDED(hr)) {
            ::MFShutdown();
        }
    });
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    auto sg1 = co_initialize(COINIT_MULTITHREADED);
    auto sg2 = mf_initialize();

    int argc;
    auto argv = ::CommandLineToArgvW(GetCommandLineW(), &argc);
    auto sg3 = make_scope_guard([argv] { ::LocalFree(argv); });

    if (argc < 2) {
        return 0;
    }

    main_window win;
    win.set_source_file_path(argv[1]);

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
            win.render();
        }
        if (msg.message == WM_QUIT) {
            break;
        }
    }
    _Module.Term();
    return 0;
}
