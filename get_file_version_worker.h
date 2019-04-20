#pragma once 
#include <nan.h>
#include <string>
#include <vector>
#include "version_info.h"
#if WINDOWS
#include "windows/utils.h"
#elif LINUX
#endif

class Get_file_version_worker : public Nan::AsyncWorker {
public:
    Get_file_version_worker(const std::wstring& file, Nan::Callback *callback)
    : AsyncWorker(callback)
    , file(file) {}
    ~Get_file_version_worker() {}

    // Executed inside the worker-thread.
    // It is not safe to access V8, or V8 data structures
    // here, so everything we need for input and output
    // should go on `this`.
    void Execute() {
        version = get_file_info_version(file);
    }

    // Executed when the async work is complete
    // this function will be run inside the main event loop
    // so it is safe to use V8 again
    void HandleOKCallback();
private:
    std::wstring file;
    Version_info version;
};
