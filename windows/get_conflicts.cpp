#include <windows.h>
#include <windows.h>
#include <string>
#include <vector>
#include "get_conflicts.h"
using namespace std;

void get_conflicts(const std::wstring& sourcePath, const std::wstring& targetPath, const std::vector<std::wstring> items, 
    std::vector<std::shared_ptr<Conflict>>& conflicts)
{
    // TODO: Fang an mit 7 Dateien
    // TODO: sourcepath + items[i]: ist datei oder verzeichnis    GetFileAttribute
    // TODO: exisitert es im targetPath?
    // TODO: wenn ja: ConflicItem füllen: Name (mit Unterpath), Datum s/t, Größe s/t, (bei Exe und dll: Version s/t)
    // TODO: Unterverzeichnisse rekursiv ermitteln
}