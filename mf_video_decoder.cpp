#include "mf_video_decoder.h"

mf_video_decoder::mf_video_decoder() noexcept
    : _stream_index(0), _width(0), _height(0), _stride(0), _par()
{
}

HRESULT mf_video_decoder::initialize(const wchar_t* url, std::uint32_t stream_index)
{
    com_ptr<IMFSourceReader> source_reader;
    CHECK_HR(MFCreateSourceReaderFromURL(url, nullptr, &source_reader));
    return initialize(source_reader.Get(), stream_index);
}

HRESULT mf_video_decoder::initialize(IMFSourceReader* source_reader, std::uint32_t stream_index) noexcept
{
    if (source_reader == nullptr) {
        return E_POINTER;
    }

    finalize();

    com_ptr<IMFMediaType> media_type;

    CHECK_HR(MFCreateMediaType(&media_type))
    CHECK_HR(media_type->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video))
    CHECK_HR(media_type->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32))
    CHECK_HR(source_reader->SetCurrentMediaType(stream_index, nullptr, media_type.Get()))
    CHECK_HR(source_reader->SetStreamSelection(stream_index, TRUE))
    media_type.Reset();
    CHECK_HR(source_reader->GetCurrentMediaType(stream_index, &media_type))

    CHECK_HR(MFGetAttributeSize(media_type.Get(), MF_MT_FRAME_SIZE, &_width, &_height));
    _stride = static_cast<INT32>(MFGetAttributeUINT32(media_type.Get(), MF_MT_DEFAULT_STRIDE, 1));
    CHECK_HR(MFGetAttributeRatio(media_type.Get(), MF_MT_PIXEL_ASPECT_RATIO, reinterpret_cast<UINT32*>(&_par.Numerator), reinterpret_cast<UINT32*>(&_par.Denominator)));

    _source_reader = source_reader;
    _stream_index = stream_index;

    return S_OK;
}

void mf_video_decoder::finalize() noexcept
{
    _source_reader = nullptr;
}