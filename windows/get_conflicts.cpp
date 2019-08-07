#include <windows.h>
#include <string>
#include <vector>
#include "get_conflicts.h"
#include "utils.h"
using namespace std;

void get_conflicts(const wstring& sourcePath, const wstring& targetPath, const vector<wstring> items, 
    vector<Conflict_item>& conflicts) {
    
    for (auto i = 0U; i < items.size(); i++) {
        auto path = combine_path(targetPath, items[i]);
        WIN32_FILE_ATTRIBUTE_DATA targetInfo;
        if (GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &targetInfo)) {
            path = combine_path(sourcePath, items[i]);
            WIN32_FILE_ATTRIBUTE_DATA sourceInfo;
            GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &sourceInfo);
            auto isDir = (sourceInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
            if (isDir) {
                // TODO: (2) Alle Dateien und Ordner ermitteln und diese Funktion rekursiv aufrufen
            } else {
                // TODO: if is exe or dll getVersionInfo
                conflicts.emplace_back(Conflict_item {
                    items[i],
                    static_cast<uint64_t>(sourceInfo.nFileSizeHigh) << 32 | sourceInfo.nFileSizeLow,
                    convert_windowstime_to_unixtime(sourceInfo.ftLastWriteTime),
                    {0},
                    static_cast<uint64_t>(targetInfo.nFileSizeHigh) << 32 | targetInfo.nFileSizeLow,
                    convert_windowstime_to_unixtime(targetInfo.ftLastWriteTime),
                    {0}
                });
            }
        } else {
            auto err = GetLastError();
            if (err != 2)
                ; // TODO: throw exception
        }
    }
}