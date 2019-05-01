#pragma once
#include <napi.h>
#include <string>

void show_properties(const wchar_t* path);
void open(const wchar_t* path);
void open_as(const wchar_t* path);
void create_directory(const std::wstring& path, std::wstring& error, int& error_code);