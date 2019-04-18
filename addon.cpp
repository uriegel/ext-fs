#include <napi.h>
#include <string>
#include "windows/utils.h"
// #include "get_drives_worker.h"
// #include "get_files_worker.h"
// #include "get_icon_worker.h"
// #include "get_file_version_worker.h"
// #include "get_exif_date_worker.h"
using namespace Napi;
using namespace std;




#include <v8.h>

// This asserts v8::Local<> will always be implemented with a single
// pointer field so that we can pass it around as a void*.
static_assert(sizeof(v8::Local<v8::Value>) == sizeof(napi_value),
  "Cannot convert between v8::Local<v8::Value> and napi_value");

napi_status napi_create_date_by_v8(
    double time,
    napi_value* result
) {
  v8::Isolate* isolate = v8::Isolate::GetCurrent();

  v8::MaybeLocal<v8::Value> maybe_date = v8::Date::New(isolate, time);
  v8::Local<v8::Value> local = maybe_date.ToLocalChecked();
  *result = reinterpret_cast<napi_value>(*local);
  return napi_ok;
}

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

Array GetFiles(const CallbackInfo& info) {
    Env env = info.Env();

    auto path = static_cast<u16string>(info[0].As<String>());
    vector<File_item> results;
    get_files(path, results);

    int index{0};
    auto result = Array::New(env);
    for(auto item: results) {
        auto obj = Object::New(env);
        obj.Set(String::New(env, u"name"), String::New(env, item.display_name.c_str()));
        obj.Set(String::New(env, u"size"), Number::New(env, item.size));
        napi_value date;
        napi_create_date_by_v8(item.time, &date);
        obj.Set(String::New(env, u"time"), date);
        obj.Set(String::New(env, u"isDirectory"), Boolean::New(env, item.is_directory));
        obj.Set(String::New(env, u"isHidden"), Boolean::New(env, item.is_hidden));
        result.Set(index++, obj);
    }

    return result;
}

Object InitAll(Env env, Object exports) {
    exports.Set("getFiles", Function::New(env, GetFiles));
    return exports;
}

NODE_API_MODULE(extension_fs, InitAll)