#define NAPI_EXPERIMENTAL
#include <napi.h>
#include <vector>
#include "wstring.h"
#include "nullfunction.h"
#include "delete_files_worker.h"
#if WINDOWS
#include "windows/utils.h"
#include "windows/shell.h"
#elif LINUX
#endif
using namespace Napi;
using namespace std;

class Delete_files_worker : public AsyncWorker {
public:
    Delete_files_worker(const Napi::Env& env, const vector<wstring>& files)
    : AsyncWorker(Function::New(env, NullFunction, "theFunction"))
    , files(files)
    , deferred(Promise::Deferred::New(Env())) {}
    ~Delete_files_worker() {}

    void Execute () { delete_files(files, error, error_code); }

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
    vector<wstring> files;
    wstring error;
    int error_code;
    Promise::Deferred deferred;
};

Value DeleteFiles(const CallbackInfo& info) {
    auto files_array = info[0].As<Array>();
    vector<wstring> files;
    for (auto i = 0U; i < files_array.Length(); i++) 
        files.push_back(files_array.Get(i).As<WString>());
    auto worker = new Delete_files_worker(info.Env(), files);
    worker->Queue();
    return worker->Promise();
}

