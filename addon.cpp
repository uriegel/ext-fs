#define NAPI_EXPERIMENTAL
#include <napi.h>
#include "get_drives_worker.h"
#include "get_files_worker.h"
#include "get_icon_worker.h"
// #include "get_file_version_worker.h"
 #include "get_exif_date_worker.h"
using namespace Napi;

// NAN_METHOD(get_file_version) {
//     auto isolate = info.GetIsolate();
//     v8::String::Value s(isolate, info[0]);
//     auto callback = new Callback(To<v8::Function>(info[1]).ToLocalChecked());
//     AsyncQueueWorker(new Get_file_version_worker(reinterpret_cast<wchar_t*>(*s), callback));
// }

Object Init(Env env, Object exports) {
    exports.Set(String::New(env, "getFiles"), Function::New(env, GetFiles));
    exports.Set(String::New(env, "getDrives"), Function::New(env, GetDrives));
    exports.Set(String::New(env, "getIcon"), Function::New(env, GetIcon));
    exports.Set(String::New(env, "getExifDate"), Function::New(env, GetExifDate));
    return exports;
}

const char* extensionfs = "extension-fs";
NODE_API_MODULE(extensionfs, Init)

//     Set(target, New<v8::String>("getFileVersion").ToLocalChecked(), GetFunction(New<v8::FunctionTemplate>(get_file_version)).ToLocalChecked());
