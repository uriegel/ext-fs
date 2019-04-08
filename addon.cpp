#include <nan.h>
#include "get_files_worker.h"
using namespace Nan;

NAN_METHOD(GetFiles) {
    auto isolate = info.GetIsolate();
    v8::String::Value s(isolate, info[0]);
    AsyncQueueWorker(new Get_files_worker((wchar_t*)*s, info.GetReturnValue()));
}

NAN_MODULE_INIT(init) {
    Set(target, New<v8::String>("getFiles").ToLocalChecked(), GetFunction(New<v8::FunctionTemplate>(GetFiles)).ToLocalChecked());
}

NODE_MODULE(extension_fs, init)
