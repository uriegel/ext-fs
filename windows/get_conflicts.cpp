#include <windows.h>
#include <string>
#include <vector>
#include <optional>
#include "get_conflicts.h"
#include "utils.h"
#include "../std_utils.h"
using namespace std;

void get_conflicts(const wstring& source_path, const wstring& target_path, const wstring& sub_path, 
                    const WIN32_FIND_DATAW& source_info, vector<Conflict_item>& conflicts) {
    auto isDir = (source_info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
    auto path = combine_path(target_path, sub_path + L"\\" + source_info.cFileName);
    WIN32_FILE_ATTRIBUTE_DATA targetInfo;
    if (GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &targetInfo)) {
        if (isDir) {
            auto dirPath = combine_path(sub_path, source_info.cFileName);
            WIN32_FIND_DATAW w32fd{ 0 };
            path = combine_path(source_path, dirPath) + L"\\*.*";
            auto ret = FindFirstFileW(path.c_str(), &w32fd);
            if (ret != INVALID_HANDLE_VALUE) {
                FindNextFileW(ret, &w32fd); // ..
                while (FindNextFileW(ret, &w32fd) == TRUE) 
                    get_conflicts(source_path, target_path, dirPath, w32fd, conflicts);
            }
            FindClose(ret);
        } else {
            optional<Version_info> source_version;
            optional<Version_info> target_version;
            if (ends_with(path, L"exe") || ends_with(path, L"dll")) {
                auto sourceFile = combine_path(source_path, sub_path + L"\\" + source_info.cFileName);
                source_version = get_file_info_version(sourceFile);
                target_version = get_file_info_version(path);
            }
            // TODO: Exception when error
            conflicts.emplace_back(
                combine_path(sub_path, source_info.cFileName),
                static_cast<uint64_t>(source_info.nFileSizeHigh) << 32 | source_info.nFileSizeLow,
                convert_windowstime_to_unixtime(source_info.ftLastWriteTime),
                source_version,
                static_cast<uint64_t>(targetInfo.nFileSizeHigh) << 32 | targetInfo.nFileSizeLow,
                convert_windowstime_to_unixtime(targetInfo.ftLastWriteTime),
                target_version
            );
        }
    }
}

void get_conflicts(const wstring& source_path, const wstring& target_path, const vector<wstring> items, 
    vector<Conflict_item>& conflicts) {
    
    for (auto it = items.cbegin(); it < items.cend(); it++) {
        auto path = combine_path(source_path, *it);
        WIN32_FIND_DATAW w32fd{ 0 };
        auto ret = FindFirstFileW(path.c_str(), &w32fd);
        get_conflicts(source_path, target_path, L"", w32fd, conflicts);
        FindClose(ret);
    }
}