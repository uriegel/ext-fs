#pragma once
#include <napi.h>
#include <string>

void show_properties(const wchar_t* path);
void open(const wchar_t* path);
void open_as(const wchar_t* path);
void create_directory(const std::wstring& path, std::wstring& error, int& error_code);
void rename(std::wstring name, std::wstring new_name, std::wstring& error, int& error_code);
void delete_files(const std::vector<std::wstring>& files, std::wstring& error, int& error_code);
void copy_files(const std::vector<std::wstring>& files, const std::vector<std::wstring>& targets, bool move, bool no_ui, std::wstring& error, int& error_code);