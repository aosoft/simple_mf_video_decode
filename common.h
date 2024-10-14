#pragma once

#include <crtdbg.h>
#include <exception>
#include <memory>
#include <optional>
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

template <typename Func>
class scope_guard {
private:
    std::optional<Func> _func;

public:
    explicit scope_guard(Func&& func)
        : _func(std::move(func))
    {
    }
    scope_guard() = delete;
    scope_guard(const scope_guard&) = delete;
    scope_guard(scope_guard&& other)
        : _func(std::move(other._func))
    {
        other._func.reset();
    }

    ~scope_guard()
    {
        if (_func.has_value()) {
            _func.value()();
        }
    }
};

template <typename Func>
inline scope_guard<Func> make_defer(Func&& func)
{
    return std::move(scope_guard<Func>(std::move(func)));
}

inline auto co_initialize(DWORD dwCoInit)
{
    ::CoInitializeEx(nullptr, dwCoInit);
    return make_defer([] { ::CoUninitialize(); });
}
