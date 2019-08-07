#include <windows.h>
#include <string>
#include <vector>
#include "get_conflicts.h"
#include "utils.h"
using namespace std;

void get_conflicts(const wstring& sourcePath, const wstring& targetPath, const vector<wstring> items, 
    vector<shared_ptr<Conflict>>& conflicts) {
    
    for (auto i = 0U; i < items.size(); i++) {
        auto path = combine_path(targetPath, items[i]);
        auto attr = GetFileAttributesW(path.c_str());
        if (attr != INVALID_FILE_ATTRIBUTES) {
            path = combine_path(sourcePath, items[i]);
            attr = GetFileAttributesW(path.c_str());
            auto isDir = (attr & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
            if (isDir) {
                // TODO: (2) Alle Dateien und Ordner ermitteln und diese Funktion rekursiv aufrufen
            } else {
                // TODO: (1) wenn ja: ConflicItem füllen: Name (mit Unterpath), Datum s/t, Größe s/t, (bei Exe und dll: Version s/t)        
            }
        }
        else {
            auto err = GetLastError();
            if (err != 2)
                ; // TODO: throw exception
        }
    }
}