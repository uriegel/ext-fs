#pragma once 
#include <nan.h>
#include <string>
#include <vector>
#if WINDOWS
#include "windows/win.h"
#elif LINUX
#endif

struct File_item {
	std::wstring display_name;
	bool is_directory;
	bool is_hidden;
	uint64_t size;
	uint64_t time;
};

class GetFilesWorker : public Nan::AsyncWorker {
public:
    GetFilesWorker(Nan::ReturnValue<v8::Value>& returnValue)
    : AsyncWorker(nullptr) {
        auto resolver = v8::Promise::Resolver::New(Nan::GetCurrentContext()).ToLocalChecked();
        SaveToPersistent(1, resolver);
        returnValue.Set(resolver->GetPromise());
    }
    ~GetFilesWorker() {}

    // Executed inside the worker-thread.
    // It is not safe to access V8, or V8 data structures
    // here, so everything we need for input and output
    // should go on `this`.
    void Execute() {
        GetFiles(L"", results);
    }

    // Executed when the async work is complete
    // this function will be run inside the main event loop
    // so it is safe to use V8 again
    void HandleOKCallback ();
private:
    std::vector<File_item> results;
};
