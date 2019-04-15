#pragma once
#include <string>

enum class Drive_type
{
	UNKNOWN,
	HARDDRIVE,
	ROM,
	REMOVABLE,
	NETWORK
};

struct Drive_item {
	const std::wstring name;
	const std::wstring description;
	const uint64_t size;
	const Drive_type type;
	const bool is_mounted;
};