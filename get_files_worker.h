#pragma once 
#include <nan.h>
#include <string>
#include <vector>
#include "file_item.h"
#if WINDOWS
#include "windows/utils.h"
#elif LINUX
#endif

class Get_files_worker : public Nan::AsyncWorker {
public:
    Get_files_worker(std::wstring directory, Nan::ReturnValue<v8::Value>& returnValue)
    : AsyncWorker(nullptr), directory(directory) {
        auto resolver = v8::Promise::Resolver::New(Nan::GetCurrentContext()).ToLocalChecked();
        SaveToPersistent(1, resolver);
        returnValue.Set(resolver->GetPromise());
    }
    ~Get_files_worker() {}

    // Executed inside the worker-thread.
    // It is not safe to access V8, or V8 data structures
    // here, so everything we need for input and output
    // should go on `this`.
    void Execute() {
        results = get_files(directory);
    }

    // Executed when the async work is complete
    // this function will be run inside the main event loop
    // so it is safe to use V8 again
    void HandleOKCallback ();
private:
    std::vector<File_item> results;
    std::wstring directory;
};
