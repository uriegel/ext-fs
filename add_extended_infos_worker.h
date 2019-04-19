#pragma once 
#include <nan.h>
#include <string>
#include <vector>
#include "extended_info.h"

class Add_extended_infos_worker : public Nan::AsyncWorker {
public:
    Add_extended_infos_worker(const std::wstring& path, v8::Local<v8::Array>& fileItems, Nan::ReturnValue<v8::Value>& returnValue);
    ~Add_extended_infos_worker() {}

    // Executed inside the worker-thread.
    // It is not safe to access V8, or V8 data structures
    // here, so everything we need for input and output
    // should go on `this`.
    void Execute();

    // Executed when the async work is complete
    // this function will be run inside the main event loop
    // so it is safe to use V8 again
    void HandleOKCallback ();
private:
    std::vector<Extended_info> result;
    std::vector<std::wstring> pathes;
    std::wstring path;
};
