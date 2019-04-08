#pragma once
#include <string>
#include <vector>

#include "../file_item.h"

std::vector<File_item> get_files(const std::wstring& directory);
void get_icon(const std::wstring& extension);