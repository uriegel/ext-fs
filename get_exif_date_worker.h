#pragma once 
#include <nan.h>
#include <string>
#include <vector>
#include "exif_reader.h"

class Get_exif_date_worker : public Nan::AsyncWorker {
public:
    Get_exif_date_worker(const std::wstring& file, Nan::Callback *callback)
    : AsyncWorker(callback)
    , file(file) {}
    ~Get_exif_date_worker() {}

    // Executed inside the worker-thread.
    // It is not safe to access V8, or V8 data structures
    // here, so everything we need for input and output
    // should go on `this`.
    void Execute() {
        exif_date = get_exif_date(file);
    }

    // Executed when the async work is complete
    // this function will be run inside the main event loop
    // so it is safe to use V8 again
    void HandleOKCallback();
private:
    std::wstring file;
    uint64_t exif_date;
};
