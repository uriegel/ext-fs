#include <nan.h>
#include "get_drives_worker.h"
#include "get_files_worker.h"
#include "get_icon_worker.h"
#include "get_file_version_worker.h"
#include "add_extended_infos_worker.h"
#include "get_exif_date_worker.h"
using namespace Nan;

NAN_METHOD(get_drives) {
    AsyncQueueWorker(new Get_drives_worker(info.GetReturnValue()));
}

NAN_METHOD(get_files) {
    auto isolate = info.GetIsolate();
    v8::String::Value s(isolate, info[0]);
    AsyncQueueWorker(new Get_files_worker(reinterpret_cast<wchar_t*>(*s), info.GetReturnValue()));
}

NAN_METHOD(get_icon) {
    auto isolate = info.GetIsolate();
    v8::String::Value s(isolate, info[0]);
    AsyncQueueWorker(new Get_icon_worker(reinterpret_cast<wchar_t*>(*s), info.GetReturnValue()));
}

NAN_METHOD(get_file_version) {
    auto isolate = info.GetIsolate();
    v8::String::Value s(isolate, info[0]);
    AsyncQueueWorker(new Get_file_version_worker(reinterpret_cast<wchar_t*>(*s), info.GetReturnValue()));
}

NAN_METHOD(get_exif_date) {
    auto isolate = info.GetIsolate();
    v8::String::Value s(isolate, info[0]);
    AsyncQueueWorker(new Get_exif_date_worker(reinterpret_cast<wchar_t*>(*s), info.GetReturnValue()));
}

NAN_METHOD(add_extended_infos) {
    auto isolate = info.GetIsolate();
    v8::String::Value s(isolate, info[0]);
    auto fileInfos = info[1].As<v8::Array>();
    AsyncQueueWorker(new Add_extended_infos_worker(reinterpret_cast<wchar_t*>(*s), fileInfos, info.GetReturnValue()));
}

NAN_METHOD(work1) {
    auto isolate = info.GetIsolate();
    v8::String::Value s(isolate, info[0]);
    AsyncQueueWorker(new Work_worker1(reinterpret_cast<wchar_t*>(*s), info.GetReturnValue()));
}

NAN_METHOD(work) {
    auto isolate = info.GetIsolate();
    v8::String::Value s(isolate, info[0]);
    auto callback = new Callback(info[1].As<v8::Function>());
    AsyncQueueWorker(new Work_worker(reinterpret_cast<wchar_t*>(*s), callback));
}

NAN_METHOD(log) {
    auto isolate = info.GetIsolate();
    v8::String::Value s(isolate, info[0]);
    printf("Log: %ls\n", *s);
}

NAN_MODULE_INIT(init) {
    Set(target, New<v8::String>("getDrives").ToLocalChecked(), GetFunction(New<v8::FunctionTemplate>(get_drives)).ToLocalChecked());
    Set(target, New<v8::String>("getFiles").ToLocalChecked(), GetFunction(New<v8::FunctionTemplate>(get_files)).ToLocalChecked());
    Set(target, New<v8::String>("getIcon").ToLocalChecked(), GetFunction(New<v8::FunctionTemplate>(get_icon)).ToLocalChecked());
    Set(target, New<v8::String>("getFileVersion").ToLocalChecked(), GetFunction(New<v8::FunctionTemplate>(get_file_version)).ToLocalChecked());
    Set(target, New<v8::String>("getExifDate").ToLocalChecked(), GetFunction(New<v8::FunctionTemplate>(get_exif_date)).ToLocalChecked());
    Set(target, New<v8::String>("addExtendedInfos").ToLocalChecked(), GetFunction(New<v8::FunctionTemplate>(add_extended_infos)).ToLocalChecked());

    Set(target, New<v8::String>("work1").ToLocalChecked(), GetFunction(New<v8::FunctionTemplate>(work1)).ToLocalChecked());
    Set(target, New<v8::String>("work").ToLocalChecked(), GetFunction(New<v8::FunctionTemplate>(work)).ToLocalChecked());
    Set(target, New<v8::String>("log").ToLocalChecked(), GetFunction(New<v8::FunctionTemplate>(log)).ToLocalChecked());
}

NODE_MODULE(extension_fs, init)
