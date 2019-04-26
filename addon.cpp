#define NAPI_EXPERIMENTAL
#include <napi.h>
#include "get_drives_worker.h"
#include "get_files_worker.h"
#include "get_icon_worker.h"
#include "get_file_version_worker.h"
#include "get_exif_date_worker.h"
using namespace Napi;

Object Init(Env env, Object exports) {
    exports.Set(String::New(env, "getFiles"), Function::New(env, GetFiles));
    exports.Set(String::New(env, "getDrives"), Function::New(env, GetDrives));
    exports.Set(String::New(env, "getIcon"), Function::New(env, GetIcon));
    exports.Set(String::New(env, "getExifDate"), Function::New(env, GetExifDate));
    exports.Set(String::New(env, "getFileVersion"), Function::New(env, GetFileVersion));
    return exports;
}

const char* extensionfs = "extension-fs";
NODE_API_MODULE(extensionfs, Init)
