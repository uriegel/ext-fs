#pragma once
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

void get_drives(std::vector<Drive_item>& drive_items);
void get_files(const std::wstring& directory, std::vector<File_item>& file_items);
Version_info get_file_info_version(const std::wstring& file_name);
void combine_path(std::wstring& path, const std::wstring& path_to_combine);
