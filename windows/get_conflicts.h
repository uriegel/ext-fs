#pragma once
#include <string>
#include <vector>
#include "..\conflict_item.h"

void get_conflicts(const std::wstring& source_path, const std::wstring& target_path, const std::vector<std::wstring> items, 
    std::vector<Conflict_item>& conflicts);