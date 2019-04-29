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

void create_directory(const Env& env, wstring& path) {
    int result = CreateDirectoryW(path.c_str(), nullptr) ? 0 : GetLastError();
    if (result == 5) {
        wchar_t temp[MAX_PATH];
        GetTempPathW(MAX_PATH, temp);
        wchar_t filename[MAX_PATH];
        GetTempFileNameW(temp, L"xxx", 0, filename);
		DeleteFileW(filename);
        CreateDirectoryW(filename, nullptr);
        wcscat(filename, L"\\");

        wstring newFolder(filename);

        auto parts = split(path, L'\\');
        newFolder.append(parts[parts.size() - 1]);
        newFolder.append(L"\\\0\0");
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
		target.append(L"\0\0");
        op.pTo = target.c_str();
        op.fFlags = 0;
        op.fAnyOperationsAborted = FALSE;
        op.hNameMappings = nullptr;
        op.lpszProgressTitle = nullptr;
        result = SHFileOperationW(&op);
        DeleteFileW(filename);

        if (result != 0) {
            char buffer[400];
            sprintf(buffer, "Konnte den Ordner nicht anlegen: %d", result);
            throw Error::New(env, buffer);
        }
    }
    else if (result != 0)
        throw Error::New(env, format_message(result).c_str());
}

