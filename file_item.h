#pragma once
#include <string>

struct File_item {
	const std::wstring display_name;
	const bool is_directory;
	const bool is_hidden;
	const uint64_t size;
	const uint64_t time;
};