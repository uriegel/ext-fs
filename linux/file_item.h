#pragma once

struct File_item {
	File_item(std::string&& display_name, bool is_directory,  bool is_hidden, uint64_t size, uint64_t time)		
	: display_name(display_name)
	, is_directory(is_directory)
	, is_hidden(is_hidden)
	, size(size)
	, time(time) {}
	const std::string display_name;
	const bool is_directory;
	const bool is_hidden;
	const uint64_t size;
	const uint64_t time;
};
