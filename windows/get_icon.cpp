#include <windows.h>
#include <Gdiplus.h>
#include <string>
#include <vector>
#include <memory>
#include "Objidl.h"
using namespace std;
using namespace Gdiplus;

ULONG_PTR gdiplus_token{0};
CLSID png_clsid{0};
CLSID get_encoder_clsid(const wstring& format);

void gdiplus_initialize()
{
	GdiplusStartupInput gdiplus_startup_input;
	gdiplus_token;
	auto status = GdiplusStartup(&gdiplus_token, &gdiplus_startup_input, nullptr);

	if (png_clsid.Data1 == 0)
		png_clsid = get_encoder_clsid(L"image/png"s);
}

void gdiplus_uninitialize() {
	GdiplusShutdown(gdiplus_token);
}

class Memory_stream : public IStream
{
public:
    Memory_stream(vector<char>* bytes) : bytes(bytes) {}
	virtual ~Memory_stream() {}
	
	virtual HRESULT __stdcall QueryInterface(REFIID iid,void **object);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();

	virtual HRESULT __stdcall Read(void *pv, ULONG cb, ULONG *pcbRead);
	virtual HRESULT __stdcall Write(const void *pv, ULONG cb, ULONG *pcbWritten);

	virtual HRESULT __stdcall Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition);
	virtual HRESULT __stdcall SetSize(ULARGE_INTEGER libNewSize);

	virtual HRESULT __stdcall CopyTo(IStream *pstm, ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten);
	virtual HRESULT __stdcall Commit(DWORD grfCommitFlags);
	virtual HRESULT __stdcall Revert();
	virtual HRESULT __stdcall LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);
	virtual HRESULT __stdcall UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);
	virtual HRESULT __stdcall Stat(STATSTG *pstatstg, DWORD grfStatFlag);
	virtual HRESULT __stdcall Clone(IStream **ppstm);

private:
	vector<char>* bytes;
};

void extract_icon(const wstring& icon_path, vector<char>* iconBytes) {
	SHFILEINFOW file_info{ 0 };
	SHGetFileInfoW(icon_path.c_str(), FILE_ATTRIBUTE_NORMAL, &file_info, sizeof(file_info),
		SHGFI_ICON | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES | SHGFI_TYPENAME);
	auto icon = file_info.hIcon;

	ICONINFO icon_info{ 0 };
	GetIconInfo(icon, &icon_info);

	auto dc = GetDC(nullptr);

	BITMAP bm{ 0 };
	GetObject(icon_info.hbmColor, sizeof(bm), &bm);

	BITMAPINFO bmi{ 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = bm.bmWidth;
	bmi.bmiHeader.biHeight = -bm.bmHeight;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	int number_of_bits = bm.bmWidth * bm.bmHeight;
	vector<int> pixels(number_of_bits);
	GetDIBits(dc, icon_info.hbmColor, 0, bm.bmHeight, pixels.data(), &bmi, DIB_RGB_COLORS);

	// Check whether the color bitmap has an alpha channel.
	// (On my Windows 7, all file icons I tried have an alpha channel.)
	auto has_alpha{ false };
	for (int i = 0; i < number_of_bits; i++)
		if ((pixels[i] & 0xff000000) != 0) {
			has_alpha = TRUE;
			break;
		}

	// If no alpha values available, apply the mask bitmap
	if (!has_alpha)
	{
		// Extract the mask bitmap
		vector<int> mask_bits(number_of_bits);
		GetDIBits(dc, icon_info.hbmMask, 0, bm.bmHeight, mask_bits.data(), &bmi, DIB_RGB_COLORS);
		// Copy the mask alphas into the color bits
		for (int i = 0; i < number_of_bits; i++)
			if (mask_bits[i] == 0)
				pixels[i] |= 0xff000000;
	}

	ReleaseDC(nullptr, dc);
	DeleteObject(icon_info.hbmColor);
	DeleteObject(icon_info.hbmMask);

	// Create GDI+ Bitmap
	auto bmp = make_unique<Bitmap>(bm.bmWidth, bm.bmHeight, bm.bmWidth * 4, PixelFormat32bppARGB, reinterpret_cast<BYTE*>(pixels.data()));

	Memory_stream ms(iconBytes);
	bmp->Save(&ms, &png_clsid);
	DestroyIcon(icon);
}

void get_icon(const wstring& extension, vector<char>* iconBytes) {
    gdiplus_initialize();

    extract_icon(extension, iconBytes); 
    gdiplus_uninitialize();
}

CLSID get_encoder_clsid(const wstring& format)
{
	UINT num{0};
	UINT size{0};
	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return {0};

	vector<char> image_codec_info_buffer(size);
	auto image_codec_info = reinterpret_cast<ImageCodecInfo*>(image_codec_info_buffer.data());
	GetImageEncoders(num, size, image_codec_info);
	for (auto i = 0u; i < num; ++i)
		if (format == image_codec_info[i].MimeType)
			return image_codec_info[i].Clsid;
	return { 0 };
}

HRESULT __stdcall Memory_stream::QueryInterface(REFIID iid, void **object)
{
	if (iid == IID_IUnknown) {
		*object = static_cast<IUnknown*>(this);
		return S_OK;
	}
	else if (iid == IID_IStream) {
		*object = static_cast<IStream*>(this);
		return S_OK;
	}
	else if (iid == IID_ISequentialStream) {
		*object = static_cast<ISequentialStream*>(this);
		return S_OK;
	}
	else {
		*object = nullptr;
		return E_NOINTERFACE;
	}
}

ULONG __stdcall Memory_stream::AddRef()
{
	return 1;
}
ULONG __stdcall Memory_stream::Release()
{
	return 0;
}

HRESULT __stdcall Memory_stream::Read(void *pv, ULONG cb, ULONG *pcbRead)
{
	return E_NOTIMPL;
}
HRESULT __stdcall Memory_stream::Write(const void *pv, ULONG cb, ULONG *pcbWritten)
{
	auto bs = reinterpret_cast<const char*>(pv);
	copy(bs, bs + cb, back_inserter(*bytes));
	*pcbWritten = cb;
	return S_OK;
}

HRESULT __stdcall Memory_stream::Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition)
{
	return E_NOTIMPL;
}

HRESULT __stdcall Memory_stream::SetSize(ULARGE_INTEGER libNewSize)
{
	return E_NOTIMPL;
}

HRESULT __stdcall Memory_stream::CopyTo(IStream *pstm, ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten)
{
	return E_NOTIMPL;
}

HRESULT __stdcall Memory_stream::Commit(DWORD grfCommitFlags)
{
	return E_NOTIMPL;
}

HRESULT __stdcall Memory_stream::Revert()
{
	return E_NOTIMPL;
}

HRESULT __stdcall Memory_stream::LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
	return E_NOTIMPL;
}

HRESULT __stdcall Memory_stream::UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
	return E_NOTIMPL;
}

HRESULT __stdcall Memory_stream::Stat(STATSTG *pstatstg, DWORD grfStatFlag)
{
	return E_NOTIMPL;
}

HRESULT __stdcall Memory_stream::Clone(IStream **ppstm)
{
	return E_NOTIMPL;
}