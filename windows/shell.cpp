#include <Windows.h>
#include "shell.h"
#include "utils.h"
#include "..\std_utils.h"
using namespace std;
using namespace Napi;

void open(const wchar_t* path) {
    SHELLEXECUTEINFOW info{0};
    info.cbSize = sizeof(SHELLEXECUTEINFOW);
    info.lpFile = path;
    info.nShow = SW_SHOW;
    info.fMask = SEE_MASK_INVOKEIDLIST;
    info.lpVerb = L"open";
    ShellExecuteExW(&info);
}

void open_as(const wchar_t* path) {
    SHELLEXECUTEINFOW info{0};
    info.cbSize = sizeof(SHELLEXECUTEINFOW);
    info.lpFile = path;
    info.nShow = SW_SHOW;
    info.fMask = SEE_MASK_INVOKEIDLIST;
    info.lpVerb = L"openas";
    ShellExecuteExW(&info);
}

void show_properties(const wchar_t* path) {
    SHELLEXECUTEINFOW info{0};
    info.cbSize = sizeof(SHELLEXECUTEINFOW);
    info.lpFile = path;
    info.nShow = SW_SHOW;
    info.fMask = SEE_MASK_INVOKEIDLIST;
    info.lpVerb = L"properties";
    ShellExecuteExW(&info);
}

void delete_directory(wstring path) {
    SHFILEOPSTRUCTW op;
    op.hwnd = nullptr;
    op.wFunc = FO_DELETE;
    path.append(L"A");
    path[path.size() - 1] = 0;
	op.pFrom = path.c_str();
    op.pTo = nullptr;
    op.fFlags = FOF_NOCONFIRMATION;
    op.fAnyOperationsAborted = FALSE;
    op.hNameMappings = nullptr;
    op.lpszProgressTitle = nullptr;
    auto result = SHFileOperationW(&op);
}

void create_directory(const Env& env, const wstring& path) {
    int result = CreateDirectoryW(path.c_str(), nullptr) ? 0 : GetLastError();
    if (result == 5) {
        wchar_t temp[MAX_PATH];
        GetTempPathW(MAX_PATH, temp);
        wchar_t buffer[MAX_PATH];
        GetTempFileNameW(temp, L"xxx", 0, buffer);
        wstring tempDirectory(buffer);
		DeleteFileW(tempDirectory.c_str());
        CreateDirectoryW(tempDirectory.c_str(), nullptr);

        
        wstring newFolder(tempDirectory);
		newFolder.append(L"\\");
        auto parts = split(path, L'\\');
        newFolder.append(parts[parts.size() - 1]);
        newFolder.append(L"\\A");
        newFolder[newFolder.length() - 1] = 0;
        CreateDirectoryW(newFolder.c_str(), nullptr);
        
        SHFILEOPSTRUCTW op;
        op.hwnd = nullptr;
        op.wFunc = FO_MOVE;
		op.pFrom = newFolder.c_str();
	    wstring target = parts[0];
		for (auto i = 1; i < parts.size() - 1; i++) {
		 	target.append(L"\\");
		 	target.append(parts[i]);
		}
		target.append(L"\\A");
        target[target.length() - 1] = 0;
        op.pTo = target.c_str();
        op.fFlags = 0;
        op.fAnyOperationsAborted = FALSE;
        op.hNameMappings = nullptr;
        op.lpszProgressTitle = nullptr;
        result = SHFileOperationW(&op);
        delete_directory(tempDirectory);

        if (result != 0) {
            char buffer[400];
            sprintf(buffer, "Konnte den Ordner nicht anlegen: %d", result);
            throw Error::New(env, buffer);
        }
    }
    else if (result != 0)
        throw Error::New(env, format_message(result).c_str());
}

