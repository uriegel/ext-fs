#define NAPI_EXPERIMENTAL
#include <napi.h>
#include <vector>
#include "wstring.h"
#include "nullfunction.h"
#include "copy_worker.h"
#if WINDOWS
#include "windows/utils.h"
#include "windows/shell.h"
#elif LINUX
#endif
using namespace Napi;
using namespace std;

class Copy_worker : public AsyncWorker {
public:
    Copy_worker(const Napi::Env& env, const wstring& source_path, const wstring& target_path, const vector<wstring>& files,
        const vector<wstring>& exceptions, bool move)
    : AsyncWorker(Function::New(env, NullFunction, "theFunction"))
    , source_path(source_path)
    , target_path(target_path)
    , files(files)
    , exceptions(exceptions)
    , move(move)
    , deferred(Promise::Deferred::New(Env())) {}
    ~Copy_worker() {}

    void Execute () { copy_files(source_path, target_path, files, exceptions, move, error, error_code); }

    void OnOK() {
        HandleScope scope(Env());
        if (error.empty())
            deferred.Resolve(Env().Null());
        else {
            auto obj = Object::New(Env());
            obj.Set("description", String::New(Env(), error.c_str()));
            obj.Set("code", Number::New(Env(), static_cast<double>(error_code)));
            deferred.Reject(obj);
        }
    }

    Promise Promise() { return deferred.Promise(); }

private:
    wstring source_path;
    wstring target_path;
    vector<wstring> files;
    vector<wstring> exceptions;
    bool move;
    string error;
    int error_code;
    Promise::Deferred deferred;
};

Value copy_files(const CallbackInfo& info, bool move) {
    auto source_path = info[0].As<WString>();
    auto target_path = info[1].As<WString>();
    auto files_array = info[2].As<Array>();
    vector<wstring> files;
    for (auto i = 0U; i < files_array.Length(); i++) 
        files.push_back(files_array.Get(i).As<WString>());
    auto exceptions_array = info[3].As<Array>();
    vector<wstring> exceptions;
    for (auto i = 0U; i < exceptions_array.Length(); i++) 
        exceptions.push_back(exceptions_array.Get(i).As<WString>());
    auto worker = new Copy_worker(info.Env(), source_path, target_path, files, exceptions, move);
    worker->Queue();
    return worker->Promise();
}

