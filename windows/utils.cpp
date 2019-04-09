#include <windows.h>
#include <algorithm>
#include "utils.h"
using namespace std;

uint64_t convert_windowstime_to_unixtime(const FILETIME& ft) {
	ULARGE_INTEGER ull;
	ull.LowPart = ft.dwLowDateTime;
	ull.HighPart = ft.dwHighDateTime;
	return (ull.QuadPart / 10000000ULL - 11644473600ULL) * 1000;
}

void get_files(const wstring& directory, vector<File_item>& file_items) {
    auto search_string = (directory[directory.length()-1] == L'\\' || directory[directory.length()-1] == L'/') 
        ? directory + L"*.*"s
        : directory + L"\\*.*"s;
    replace(search_string.begin(), search_string.end(), L'/', L'\\'); 

    WIN32_FIND_DATAW w32fd{ 0 };
    auto ret = FindFirstFileW(search_string.c_str(), &w32fd);
    while (FindNextFileW(ret, &w32fd) == TRUE) {
        file_items.emplace_back(File_item {
            w32fd.cFileName,
            (w32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY,
            (w32fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN,
            static_cast<uint64_t>(w32fd.nFileSizeHigh) << 32 | w32fd.nFileSizeLow,
            convert_windowstime_to_unixtime(w32fd.ftLastWriteTime)
        });
    }
}

