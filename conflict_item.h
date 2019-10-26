#pragma once
#include <string>
#include <optional>
#include "version_info.h"

struct Conflict_item {
	Conflict_item(const std::wstring& name, uint64_t source_size, uint64_t source_time, const std::optional<Version_info>& source_version,
		uint64_t target_size, uint64_t target_time, const std::optional<Version_info>& target_version)
	: name(name)
	, source_size(source_size)
	, source_time(source_time)
	, source_version(source_version)
	, target_size(target_size)
	, target_time(target_time)
	, target_version(target_version) {}
	const std::wstring name;
	const uint64_t source_size;
	const uint64_t source_time;
	std::optional<Version_info> source_version;
	const uint64_t target_size;
	const uint64_t target_time;
	std::optional<Version_info> target_version;
};