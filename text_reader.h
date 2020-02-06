#pragma once
#include <iostream>
#include <fstream>
#include <napi.h>
#include <string>
#include "line_index.h"

class Text_reader: public Napi::ObjectWrap<Text_reader> {
public:
    static void Init(Napi::Env env, Napi::Object& exports);

    Text_reader(const Napi::CallbackInfo& info); 
    ~Text_reader();

private:
    static Napi::FunctionReference constructor;

    Napi::Value get_line_count(const Napi::CallbackInfo& info);
    Napi::Value get_lines(const Napi::CallbackInfo& info);
   
    void get_line_indexes();

    std::vector<Line_index> line_indexes;

#ifdef WINDOWS
    std::wstring file_name;
#elif LINUX
    std::string file_name;
#endif    
    std::ifstream file;
};