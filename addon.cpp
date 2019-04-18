#include <napi.h>
// #include "get_drives_worker.h"
// #include "get_files_worker.h"
// #include "get_icon_worker.h"
// #include "get_file_version_worker.h"
// #include "get_exif_date_worker.h"
using namespace Napi;

// NAN_METHOD(get_drives) {
//     AsyncQueueWorker(new Get_drives_worker(info.GetReturnValue()));
// }

// NAN_METHOD(get_files) {
//     auto isolate = info.GetIsolate();
//     v8::String::Value s(isolate, info[0]);
//     AsyncQueueWorker(new Get_files_worker((wchar_t*)*s, info.GetReturnValue()));
// }

// NAN_METHOD(get_icon) {
//     auto isolate = info.GetIsolate();
//     v8::String::Value s(isolate, info[0]);
//     AsyncQueueWorker(new Get_icon_worker((wchar_t*)*s, info.GetReturnValue()));
// }

// NAN_METHOD(get_file_version) {
//     auto isolate = info.GetIsolate();
//     v8::String::Value s(isolate, info[0]);
//     AsyncQueueWorker(new Get_file_version_worker((wchar_t*)*s, info.GetReturnValue()));
// }

// NAN_METHOD(get_exif_date) {
//     auto isolate = info.GetIsolate();
//     v8::String::Value s(isolate, info[0]);
//     AsyncQueueWorker(new Get_exif_date_worker((wchar_t*)*s, info.GetReturnValue()));
// }

// NAN_MODULE_INIT(init) {
//     Set(target, New<v8::String>("getDrives").ToLocalChecked(), GetFunction(New<v8::FunctionTemplate>(get_drives)).ToLocalChecked());
//     Set(target, New<v8::String>("getFiles").ToLocalChecked(), GetFunction(New<v8::FunctionTemplate>(get_files)).ToLocalChecked());
//     Set(target, New<v8::String>("getIcon").ToLocalChecked(), GetFunction(New<v8::FunctionTemplate>(get_icon)).ToLocalChecked());
//     Set(target, New<v8::String>("getFileVersion").ToLocalChecked(), GetFunction(New<v8::FunctionTemplate>(get_file_version)).ToLocalChecked());
//     Set(target, New<v8::String>("getExifDate").ToLocalChecked(), GetFunction(New<v8::FunctionTemplate>(get_exif_date)).ToLocalChecked());
// }


#include <string>
std::string hello(){
    return "Hello World";
}

String HelloWrapped(const CallbackInfo& info) 
{
    Env env = info.Env();
    String returnValue = String::New(env, hello());
    return returnValue;
}

Object InitAll(Env env, Object exports) {
    exports.Set("hello", Function::New(env, HelloWrapped));
    return exports;
}

NODE_API_MODULE(extension_fs, InitAll)