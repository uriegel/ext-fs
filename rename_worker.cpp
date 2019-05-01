#define NAPI_EXPERIMENTAL
#include <napi.h>
#include <vector>
#include "wstring.h"
#include "nullfunction.h"
#include "rename_worker.h"
#if WINDOWS
#include "windows/utils.h"
#include "windows/shell.h"
#elif LINUX
#endif
using namespace Napi;
using namespace std;

class Rename_worker : public AsyncWorker {
public:
    Rename_worker(const Napi::Env& env, const wstring& path, const wstring& name, const wstring& new_name)
    : AsyncWorker(Function::New(env, NullFunction, "theFunction"))
    , name(combine_path(path, name))
    , new_name(combine_path(path, new_name))
    , deferred(Promise::Deferred::New(Env())) {}
    ~Rename_worker() {}

    void Execute () { rename(name, new_name, error, error_code); }

    void OnOK() {
        HandleScope scope(Env());
        if (error.empty())
            deferred.Resolve(Env().Null());
        else {
            auto obj = Object::New(Env());
            obj.Set("description", WString::New(Env(), error.c_str()));
            obj.Set("code", Number::New(Env(), static_cast<double>(error_code)));
            deferred.Reject(obj);
        }
    }

    Promise Promise() { return deferred.Promise(); }

private:
    wstring name;
    wstring new_name;
    wstring error;
    int error_code;
    Promise::Deferred deferred;
};

Value Rename(const CallbackInfo& info) {
    auto path = info[0].As<WString>().WValue();
    auto name = info[1].As<WString>().WValue();
    auto new_name = info[2].As<WString>().WValue();
    auto worker = new Rename_worker(info.Env(), path, name, new_name);
    worker->Queue();
    return worker->Promise();
}

