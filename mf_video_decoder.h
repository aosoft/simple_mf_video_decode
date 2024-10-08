#pragma once

#include "common.h"
#include <mfapi.h>
#include <mferror.h>
#include <mfidl.h>
#include <mfreadwrite.h>

class mf_video_decoder {
private:
    com_ptr<IMFSourceReader> _source_reader;
    std::uint32_t _stream_index;

    UINT32 _width;
    UINT32 _height;
    INT32 _stride;
    MFRatio _par;

public:
    mf_video_decoder() noexcept;

    HRESULT initialize(const wchar_t* url, std::uint32_t stream_index = MF_SOURCE_READER_FIRST_VIDEO_STREAM);
    HRESULT initialize(IMFSourceReader* source_reader, std::uint32_t stream_index = MF_SOURCE_READER_FIRST_VIDEO_STREAM) noexcept;
    void finalize() noexcept;

};