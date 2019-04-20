#pragma once 
#include <nan.h>
#include <string>
#include <vector>
#include "drive_item.h"
#if WINDOWS
#include "windows/utils.h"
#elif LINUX
#endif

class Get_drives_worker : public Nan::AsyncWorker {
public:
    Get_drives_worker(Nan::Callback *callback)
    : AsyncWorker(callback) { }
    ~Get_drives_worker() {}

    // Executed inside the worker-thread.
    // It is not safe to access V8, or V8 data structures
    // here, so everything we need for input and output
    // should go on `this`.
    void Execute() {
        get_drives(results);
    }

    // Executed when the async work is complete
    // this function will be run inside the main event loop
    // so it is safe to use V8 again
    void HandleOKCallback ();
private:
    std::vector<Drive_item> results;
};
