#pragma once 
#include <nan.h>
#include <string>
#include <vector>
#include "file_item.h"
#if WINDOWS
#include "windows/utils.h"
#elif LINUX
#endif

class Work_worker1 : public Nan::AsyncWorker {
public:
    Work_worker1(std::wstring directory, Nan::ReturnValue<v8::Value>& returnValue)
    : AsyncWorker(nullptr), directory(directory) {
        auto resolver = v8::Promise::Resolver::New(Nan::GetCurrentContext()).ToLocalChecked();
        SaveToPersistent(1, resolver);
        returnValue.Set(resolver->GetPromise());
        auto id = GetCurrentThreadId();
        printf("UI_Thread: %ls, %d\n", directory.c_str(), id);
    }

    // Executed inside the worker-thread.
    // It is not safe to access V8, or V8 data structures
    // here, so everything we need for input and output
    // should go on `this`.
    void Execute() {
        auto id = GetCurrentThreadId();
        printf("                        Worker: %ls, %d\n", directory.c_str(), id);
        Sleep(10000);
    }

    // Executed when the async work is complete
    // this function will be run inside the main event loop
    // so it is safe to use V8 again
    void HandleOKCallback() {
        auto id = GetCurrentThreadId();
        printf("                                              Zurück: %ls, %d\n", directory.c_str(), id);

        Nan::HandleScope scope;
        auto resolver = GetFromPersistent(1).As<v8::Promise::Resolver>();
        
        resolver->Resolve(Nan::GetCurrentContext(), Nan::New<v8::String>((uint16_t*)directory.c_str()).ToLocalChecked());
        id = GetCurrentThreadId();
        printf("                                                              Ende: %ls, %d\n", directory.c_str(), id);
    }
private:
    std::wstring directory;
};

class Work_worker : public Nan::AsyncWorker {
public:
    Work_worker(std::wstring directory, Nan::Callback* callback)
    : AsyncWorker(nullptr), directory(directory)
    , callback(callback) {
        auto id = GetCurrentThreadId();
        printf("UI_Thread: %ls, %d\n", directory.c_str(), id);
    }

    // Executed inside the worker-thread.
    // It is not safe to access V8, or V8 data structures
    // here, so everything we need for input and output
    // should go on `this`.
    void Execute() {
        auto id = GetCurrentThreadId();
        printf("                        Worker: %ls, %d\n", directory.c_str(), id);
        Sleep(10000);
    }

    // Executed when the async work is complete
    // this function will be run inside the main event loop
    // so it is safe to use V8 again
    void HandleOKCallback() {
        auto id = GetCurrentThreadId();
        printf("                                              Zurück: %ls, %d\n", directory.c_str(), id);

        v8::Local<v8::Value> argv[] = { Nan::New<v8::String>((uint16_t*)directory.c_str()).ToLocalChecked() };
        callback->Call(1, argv);

        id = GetCurrentThreadId();
        printf("                                                              Ende: %ls, %d\n", directory.c_str(), id);
    }
private:
    std::wstring directory;
    Nan::Callback* callback;
};

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
        get_files(directory, results);
    }

    // Executed when the async work is complete
    // this function will be run inside the main event loop
    // so it is safe to use V8 again
    void HandleOKCallback ();
private:
    std::vector<File_item> results;
    std::wstring directory;
};
