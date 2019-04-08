#include <windows.h>
#include <Gdiplus.h>
#include <algorithm>
#include "utils.h"
using namespace std;
using namespace Gdiplus;

uint64_t convert_windowstime_to_unixtime(const FILETIME& ft) {
	ULARGE_INTEGER ull;
	ull.LowPart = ft.dwLowDateTime;
	ull.HighPart = ft.dwHighDateTime;
	return (ull.QuadPart / 10000000ULL - 11644473600ULL) * 1000;
}

vector<File_item> get_files(const wstring& directory) {
    auto search_string = (directory[directory.length()-1] == L'\\' || directory[directory.length()-1] == L'/') 
        ? directory + L"*.*"s
        : directory + L"\\*.*"s;
    replace(search_string.begin(), search_string.end(), L'/', L'\\'); 

    vector<File_item> results;
    WIN32_FIND_DATAW w32fd{ 0 };
    auto ret = FindFirstFileW(search_string.c_str(), &w32fd);
    while (FindNextFileW(ret, &w32fd) == TRUE) {
        results.emplace_back(File_item {
            w32fd.cFileName,
            (w32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY,
            (w32fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN,
            static_cast<uint64_t>(w32fd.nFileSizeHigh) << 32 | w32fd.nFileSizeLow,
            convert_windowstime_to_unixtime(w32fd.ftLastWriteTime)
        });
    }
    return results;
}

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

void get_icon(const std::wstring& extension) {
    gdiplus_initialize();
    gdiplus_uninitialize();
}

CLSID get_encoder_clsid(const wstring& format)
{
	UINT num{ 0 };
	UINT  size{ 0 };
	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return { 0 };

	vector<char> image_codec_info_buffer(size);
	auto image_codec_info = reinterpret_cast<ImageCodecInfo*>(image_codec_info_buffer.data());
	GetImageEncoders(num, size, image_codec_info);
	for (auto i = 0u; i < num; ++i)
		if (format == image_codec_info[i].MimeType)
			return image_codec_info[i].Clsid;
	return { 0 };
}