#pragma once
#include <Windows.h>
#include <string>
#include <vector>

#include "../drive_item.h"
#include "../file_item.h"
#include "../version_info.h"

class file_handle
{
public:
	file_handle(HANDLE handle) : handle(handle) {}
	~file_handle() {
		if (handle != INVALID_HANDLE_VALUE)
			CloseHandle(handle);
	}
	operator HANDLE() const { return handle; }
private:
	HANDLE handle{ INVALID_HANDLE_VALUE };
};

uint64_t convert_windowstime_to_unixtime(const FILETIME& ft);
std::vector<Drive_item> get_drives();
std::vector<File_item> get_files(const std::wstring& directory);
Version_info get_file_info_version(const std::wstring& file_name);
std::wstring combine_path(std::wstring path, const std::wstring& path_to_combine);
std::string format_message(int last_error); 
void start_elevated();