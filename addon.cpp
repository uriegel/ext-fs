#define NAPI_EXPERIMENTAL
#include <napi.h>
#include "get_drives_worker.h"
#include "get_files_worker.h"
#include "get_icon_worker.h"
#include "get_file_version_worker.h"
#include "get_exif_date_worker.h"
#include "wstring.h"
#if WINDOWS
#include "windows/shell.h"
#include "windows/utils.h"
#elif LINUX
#endif
using namespace Napi;

Value ShowInfo(const CallbackInfo& info) {
    auto file = info[0].As<WString>().WValue();
    show_properties(file.c_str());
    return info.Env().Undefined();
}

Value Open(const CallbackInfo& info) {
    auto file = info[0].As<WString>().WValue();
    open(file.c_str());
    return info.Env().Undefined();
}

Value OpenAs(const CallbackInfo& info) {
    auto file = info[0].As<WString>().WValue();
    open_as(file.c_str());
    return info.Env().Undefined();
}

Value CreateFolder(const CallbackInfo& info) {
    auto path = info[0].As<WString>().WValue();
    auto result = create_directory(info.Env(), path);
    return info.Env().Undefined();
}

Object Init(Env env, Object exports) {
    exports.Set(String::New(env, "getFiles"), Function::New(env, GetFiles));
    exports.Set(String::New(env, "getDrives"), Function::New(env, GetDrives));
    exports.Set(String::New(env, "getIcon"), Function::New(env, GetIcon));
    exports.Set(String::New(env, "getExifDate"), Function::New(env, GetExifDate));
    exports.Set(String::New(env, "getFileVersion"), Function::New(env, GetFileVersion));
    exports.Set(String::New(env, "showInfo"), Function::New(env, ShowInfo));
    exports.Set(String::New(env, "open"), Function::New(env, Open));
    exports.Set(String::New(env, "openAs"), Function::New(env, OpenAs));
    exports.Set(String::New(env, "createFolder"), Function::New(env, CreateFolder));
    return exports;
}

const char* extensionfs = "extension-fs";
NODE_API_MODULE(extensionfs, Init)
