#define NAPI_EXPERIMENTAL
#include <napi.h>
// #include "get_drives_worker.h"
#include "get_files_worker.h"
#include "get_icon_worker.h"
// #include "get_file_version_worker.h"
// #include "get_exif_date_worker.h"
using namespace Napi;

// NAN_METHOD(get_drives) {
//     auto callback = new Callback(To<v8::Function>(info[0]).ToLocalChecked());
//     AsyncQueueWorker(new Get_drives_worker(callback));
// }

// NAN_METHOD(get_file_version) {
//     auto isolate = info.GetIsolate();
//     v8::String::Value s(isolate, info[0]);
//     auto callback = new Callback(To<v8::Function>(info[1]).ToLocalChecked());
//     AsyncQueueWorker(new Get_file_version_worker(reinterpret_cast<wchar_t*>(*s), callback));
// }

// NAN_METHOD(get_exif_date) {
//     auto isolate = info.GetIsolate();
//     v8::String::Value s(isolate, info[0]);
//     auto callback = new Callback(To<v8::Function>(info[1]).ToLocalChecked());
//     AsyncQueueWorker(new Get_exif_date_worker(reinterpret_cast<wchar_t*>(*s), callback));
// }

Object Init(Env env, Object exports) {
    exports.Set(String::New(env, "getFiles"), Function::New(env, GetFiles));
    exports.Set(String::New(env, "getIcon"), Function::New(env, GetIcon));
    return exports;
}

const char* extensionfs = "extension-fs";
NODE_API_MODULE(extensionfs, Init)

// NAN_MODULE_INIT(init) {
//     Set(target, New<v8::String>("getDrives").ToLocalChecked(), GetFunction(New<v8::FunctionTemplate>(get_drives)).ToLocalChecked());
//     Set(target, New<v8::String>("getFileVersion").ToLocalChecked(), GetFunction(New<v8::FunctionTemplate>(get_file_version)).ToLocalChecked());
//     Set(target, New<v8::String>("getExifDate").ToLocalChecked(), GetFunction(New<v8::FunctionTemplate>(get_exif_date)).ToLocalChecked());
// }
