#pragma once
#include <string>

struct File_item {
	File_item(std::wstring&& display_name, bool is_directory,  bool is_hidden, uint64_t size, uint64_t time)		
	: display_name(display_name)
	, is_directory(is_directory)
	, is_hidden(is_hidden)
	, size(size)
	, time(time) {}
	const std::wstring display_name;
	const bool is_directory;
	const bool is_hidden;
	const uint64_t size;
	const uint64_t time;
};
