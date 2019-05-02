#pragma once
#include <string>
#include <sstream>
#include <vector>

const std::vector<std::wstring> split(const std::wstring& s, wchar_t delim);
std::wstring join(const std::vector<std::wstring>& arr, wchar_t delim);
int findString(const std::wstring &text, const std::wstring& searchText);