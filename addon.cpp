#define NAPI_EXPERIMENTAL
#include <napi.h>
#include <filesystem>
using namespace Napi;
using namespace std;

Napi::Value GetTest(const Napi::CallbackInfo& info) {
    return Number::New(info.Env(), static_cast<double>(567));   
}

Object Init(Env env, Object exports) {
    exports.Set(String::New(env, "getTest"), Function::New(env, GetTest));
    return exports;
}

const char* extensionfs = "extension-fs";
NODE_API_MODULE(extensionfs, Init)
