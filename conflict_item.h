#pragma once
#include <string>
#include "version_info.h"

struct Conflict_item {
	const std::wstring name;
	const uint64_t source_size;
	const uint64_t source_time;
	Version_info source_version;
	const uint64_t target_size;
	const uint64_t target_time;
	Version_info target_version;
};