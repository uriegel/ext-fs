#pragma once
#include <string>

struct File_item {
	std::wstring display_name;
	bool is_directory;
	bool is_hidden;
	uint64_t size;
	uint64_t time;
};