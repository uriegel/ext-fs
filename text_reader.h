#pragma once
#include <iostream>
#include <fstream>
#include <napi.h>
#include <string>

class Text_reader: public Napi::ObjectWrap<Text_reader> {
public:
    static void Init(Napi::Env env, Napi::Object& exports);

    Text_reader(const Napi::CallbackInfo& info); 
    ~Text_reader();

private:
    static Napi::FunctionReference constructor;

    Napi::Value GetLineIndexes(const Napi::CallbackInfo& info);
    Napi::Value GetLines(const Napi::CallbackInfo& info);
   
public:    
#ifdef WINDOWS
    std::wstring file_name;
#elif LINUX
    std::string file_name;
#endif    
    std::ifstream file;
};