#include <windows.h>
#include <algorithm>
#include <array>
#include "utils.h"
#include "..\std_utils.h"
using namespace std;

uint64_t convert_windowstime_to_unixtime(const FILETIME& ft) {
	ULARGE_INTEGER ull;
	ull.LowPart = ft.dwLowDateTime;
	ull.HighPart = ft.dwHighDateTime;
	return (ull.QuadPart / 10000000ULL - 11644473600ULL) * 1000;
}

string format_message(int last_error) {
    char* message{nullptr};
    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    nullptr, last_error, 0, reinterpret_cast<char*>(&message), 0, nullptr);
    string result(message);
    LocalFree(message);
    return result;
}

const wstring get_drive_description(const wstring& name) {
	array<wchar_t, 400> buffer;
	if (GetVolumeInformationW(name.c_str(), buffer.data(), static_cast<DWORD>(buffer.size()), nullptr, nullptr, nullptr, nullptr, 0))
		return wstring(buffer.data(), wcslen(buffer.data()));
	else
		return wstring();
}

uint64_t get_volume_size(const wstring& directory_on_drive) {
	ULARGE_INTEGER result{ 0 };
	GetDiskFreeSpaceExW(directory_on_drive.c_str(), nullptr, &result, nullptr);
	return result.QuadPart;
}

Drive_type get_drive_type(unsigned int type) {
	switch (type)
	{
	case 2:
		return Drive_type::REMOVABLE;
	case 3:
		return Drive_type::HARDDRIVE;
	case 4:
		return Drive_type::NETWORK;
	case 5:
		return Drive_type::ROM;
	default:
		return Drive_type::UNKNOWN;
	}
}

bool is_mounted(const file_handle& volume) {
	if (volume == INVALID_HANDLE_VALUE)
		// 2 means "no disk", anything else means by inference "disk
		// in drive, but you do not have admin privs to do a
		// CreateFile on that volume".
		return GetLastError() != 2;

	DWORD bytes_returned; // ignored
	auto result = DeviceIoControl(volume, FSCTL_IS_VOLUME_MOUNTED, nullptr, 0, nullptr, 0, &bytes_returned, nullptr);
	return result != 0;
}

vector<Drive_item> get_drives() {
  	array<wchar_t, 500> buffer;
	auto size = GetLogicalDriveStringsW(static_cast<DWORD>(buffer.size()), buffer.data());
	const wstring drive_string(buffer.data(), size);
	auto drives = split(drive_string, 0);

	vector<Drive_item> result;
	transform(drives.begin(), drives.end(), back_inserter(result), [](const wstring & val) {
		auto type = GetDriveTypeW(val.c_str());
		auto volume = wstring{ L"\\\\.\\" + val.substr(0, 2) };
		file_handle volume_handle(CreateFileW(volume.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			nullptr, OPEN_EXISTING, 0, nullptr));
		return move(Drive_item
			{
				val,
				move(get_drive_description(val)),
				get_volume_size(val),
				get_drive_type(type),
				type != 3 ? is_mounted(volume_handle) : true
			});
		});

	// auto erase_it = remove_if(drive_items.begin(), drive_items.end(), [](const Drive_item & val) {
	// 	return !val.is_mounted;
	// 	});
	// drive_items.erase(erase_it, drive_items.end());
	return result;
}

vector<File_item> get_files(const wstring& directory) {
    auto search_string = (directory[directory.length()-1] == L'\\' || directory[directory.length()-1] == L'/') 
        ? directory + L"*.*"s
        : directory + L"\\*.*"s;
    replace(search_string.begin(), search_string.end(), L'/', L'\\'); 

	vector<File_item> result;
    WIN32_FIND_DATAW w32fd{ 0 };
    auto ret = FindFirstFileW(search_string.c_str(), &w32fd);
    while (FindNextFileW(ret, &w32fd) == TRUE) {
        result.emplace_back(File_item {
            w32fd.cFileName,
            (w32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY,
            (w32fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN,
            static_cast<uint64_t>(w32fd.nFileSizeHigh) << 32 | w32fd.nFileSizeLow,
            convert_windowstime_to_unixtime(w32fd.ftLastWriteTime)
        });
    }
	FindClose(ret);
	return result;
}

Version_info get_file_info_version(const wstring& file_name) {
    DWORD _{0};
    auto size = GetFileVersionInfoSizeW(file_name.c_str(), &_);
    if (size == 0)
        return {0};
	vector<char> data(size);
	auto ok = GetFileVersionInfoW(file_name.c_str(), 0, size, data.data());
	VS_FIXEDFILEINFO *info{nullptr};
	uint32_t len{0};
	VerQueryValueW(data.data(), L"\\", reinterpret_cast<void**>(&info), &len);

    return {
        HIWORD(info->dwFileVersionMS),
        LOWORD(info->dwFileVersionMS),
        HIWORD(info->dwFileVersionLS),
        LOWORD(info->dwFileVersionLS)
    };
}

wstring combine_path(wstring path, const wstring& path_to_combine) {
    if (path.length() > 0 && path[path.length() - 1] != L'\\')
        path.append(L"\\");
    path.append(path_to_combine);
    return move(path);
}