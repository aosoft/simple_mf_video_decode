#pragma once

#include <crtdbg.h>
#include <exception>
#include <memory>
#include <wrl/client.h>

template <class Intf>
using com_ptr = Microsoft::WRL::ComPtr<Intf>;

#define CHECK_HR(hr)         \
    {                        \
        HRESULT hrtmp = hr;  \
        if (FAILED(hrtmp)) { \
            _CrtDbgBreak();  \
            return hrtmp;    \
        }                    \
    }

#define CHECK_HR_THROW(hr)                  \
    {                                       \
        HRESULT hrtmp = hr;                 \
        if (FAILED(hrtmp)) {                \
            _CrtDbgBreak();                 \
            throw hresult_exception(hrtmp); \
        }                                   \
    }

#define CHECK_HR_EXPECTED(hr)              \
    {                                      \
        HRESULT hrtmp = hr;                \
        if (FAILED(hrtmp)) {               \
            _CrtDbgBreak();                \
            return std::unexpected(hrtmp); \
        }                                  \
    }

#define CHECK_POINTER(p) CHECK_HR(p != nullptr ? S_OK : E_POINTER);
#define CHECK_POINTER_THROW(p) CHECK_HR_THROW(p != nullptr ? S_OK : E_POINTER);

struct handle_deleter {
    void operator()(HANDLE p) const
    {
        CloseHandle(p);
    }
};

using unique_handle = std::unique_ptr<std::remove_pointer<HANDLE>::type, handle_deleter>;

class hresult_exception : public std::exception {
private:
    HRESULT _hr;

public:
    hresult_exception(HRESULT hr)
        : _hr(hr)
    {
    }

    [[nodiscard]] HRESULT result() const
    {
        return _hr;
    }
};

class co_initializer {
public:
    co_initializer(DWORD dwCoInit)
    {
        ::CoInitializeEx(nullptr, dwCoInit);
    }

    ~co_initializer()
    {
        ::CoUninitialize();
    }
};
