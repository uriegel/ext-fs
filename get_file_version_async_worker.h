#pragma once 
#include <nan.h>
#include <string>
#include <vector>
#include <nan-async.h>
#include "version_info.h"
#if WINDOWS
#include "windows/utils.h"
#elif LINUX
#endif

class Get_file_version_async_worker : public NanAsync::Worker {
public:
    Get_file_version_async_worker(const std::wstring& file, Nan::ReturnValue<v8::Value>& returnValue)
    : NanAsync::Worker(returnValue)
    , file(file) { }

    void Execute() {
        version = get_file_info_version(file);
        ExecuteAction();
    }
    void WorkComplete(v8::Local<v8::Promise::Resolver>& resolver);
private:
    std::wstring file;
    Version_info version;
};