#pragma once
#include <string>
#include <vector>

struct Conflict
{

};

void get_conflicts(const std::wstring& sourcePath, const std::wstring& targetPath, const std::vector<std::wstring> items, 
    std::vector<std::shared_ptr<Conflict>>& conflicts);