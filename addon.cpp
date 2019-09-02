#define NAPI_EXPERIMENTAL
#include <napi.h>
#include <filesystem>
#include "get_drives_worker.h"
#include "get_files_worker.h"
#include "get_icon_worker.h"
#include "get_file_version_worker.h"
#include "get_exif_date_worker.h"
#include "get_conflicts_worker.h"
#include "create_directory_worker.h"
#include "rename_worker.h"
#include "delete_files_worker.h"
#include "copy_worker.h"
#include "wstring.h"
#if WINDOWS
#include "windows/shell.h"
#include "windows/utils.h"
#include "windows/services.h"
#elif LINUX
#endif
using namespace Napi;
using namespace std;

#if WINDOWS
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

Value ExistsFile(const CallbackInfo& info) {
    auto file = info[0].As<WString>().WValue();
    return Boolean::New(info.Env(), filesystem::exists(file));
}

Value GetServices(const CallbackInfo& info) {
    auto file = info[0].As<WString>().WValue();
    return Boolean::New(info.Env(), filesystem::exists(file));
}

#endif

Object Init(Env env, Object exports) {
#if WINDOWS    
    exports.Set(String::New(env, "getFiles"), Function::New(env, GetFiles));
    exports.Set(String::New(env, "getDrives"), Function::New(env, GetDrives));
    exports.Set(String::New(env, "getIcon"), Function::New(env, GetIcon));
#endif    
    exports.Set(String::New(env, "getExifDate"), Function::New(env, GetExifDate));
    exports.Set(String::New(env, "getConflicts"), Function::New(env, GetConflicts));
#if WINDOWS        
    exports.Set(String::New(env, "getFileVersion"), Function::New(env, GetFileVersion));
    exports.Set(String::New(env, "showInfo"), Function::New(env, ShowInfo));
    exports.Set(String::New(env, "open"), Function::New(env, Open));
    exports.Set(String::New(env, "openAs"), Function::New(env, OpenAs));
    exports.Set(String::New(env, "createDirectory"), Function::New(env, CreateDirectory1));
    exports.Set(String::New(env, "rename"), Function::New(env, Rename));
    exports.Set(String::New(env, "deleteFiles"), Function::New(env, DeleteFiles));
    exports.Set(String::New(env, "copyFiles"), Function::New(env, CopyFiles));
    exports.Set(String::New(env, "moveFiles"), Function::New(env, MoveFiles));
    exports.Set(String::New(env, "existsFile"), Function::New(env, ExistsFile));
    Services::Init(env, exports);
#endif        
    return exports;
}

const char* extensionfs = "extension-fs";
NODE_API_MODULE(extensionfs, Init)
