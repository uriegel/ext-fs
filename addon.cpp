#include <nan.h>
#include "GetFilesWorker.h"
using namespace Nan;

NAN_METHOD(GetFiles) {
    AsyncQueueWorker(new GetFilesWorker(info.GetReturnValue()));
}

NAN_MODULE_INIT(Init) {
    Set(target, New<v8::String>("getFiles").ToLocalChecked(), GetFunction(New<v8::FunctionTemplate>(GetFiles)).ToLocalChecked());
}

NODE_MODULE(extension_fs, Init)
