#pragma once
#include <string>
#include <vector>
#include "..\conflict_item.h"

void get_conflicts(const std::wstring& sourcePath, const std::wstring& targetPath, const std::vector<std::wstring> items, 
    std::vector<Conflict_item>& conflicts);