#include <nan.h>
#include "get_drives_worker.h"
#include "get_files_worker.h"
#include "get_icon_worker.h"
using namespace Nan;

NAN_METHOD(get_drives) {
    AsyncQueueWorker(new Get_drives_worker(info.GetReturnValue()));
}

NAN_METHOD(get_files) {
    auto isolate = info.GetIsolate();
    v8::String::Value s(isolate, info[0]);
    AsyncQueueWorker(new Get_files_worker((wchar_t*)*s, info.GetReturnValue()));
}

NAN_METHOD(get_icon) {
    auto isolate = info.GetIsolate();
    v8::String::Value s(isolate, info[0]);
    AsyncQueueWorker(new Get_icon_worker((wchar_t*)*s, info.GetReturnValue()));
}

NAN_MODULE_INIT(init) {
    Set(target, New<v8::String>("getDrives").ToLocalChecked(), GetFunction(New<v8::FunctionTemplate>(get_drives)).ToLocalChecked());
    Set(target, New<v8::String>("getFiles").ToLocalChecked(), GetFunction(New<v8::FunctionTemplate>(get_files)).ToLocalChecked());
    Set(target, New<v8::String>("getIcon").ToLocalChecked(), GetFunction(New<v8::FunctionTemplate>(get_icon)).ToLocalChecked());
}

NODE_MODULE(extension_fs, init)
