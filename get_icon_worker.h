#pragma once 
#include <nan.h>
#include <string>
#include <vector>
#if WINDOWS
#include "windows/get_icon.h"
#elif LINUX
#endif

class Get_icon_worker : public Nan::AsyncWorker {
public:
    Get_icon_worker(std::wstring extension, Nan::ReturnValue<v8::Value>& returnValue)
    : AsyncWorker(nullptr), extension(extension) {
        auto resolver = v8::Promise::Resolver::New(Nan::GetCurrentContext()).ToLocalChecked();
        SaveToPersistent(1, resolver);
        returnValue.Set(resolver->GetPromise());
    }
    ~Get_icon_worker() { delete icon;}

    // Executed inside the worker-thread.
    // It is not safe to access V8, or V8 data structures
    // here, so everything we need for input and output
    // should go on `this`.
    void Execute() {
        icon = new std::vector<char>();
        get_icon(extension, icon);
    }

    // Executed when the async work is complete
    // this function will be run inside the main event loop
    // so it is safe to use V8 again
    void HandleOKCallback();
private:
    std::wstring extension;
    std::vector<char>* icon{nullptr};
};
