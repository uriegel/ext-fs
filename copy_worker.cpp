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
    Copy_worker(const Napi::Env& env, const vector<wstring>& files, const wstring& target, bool move, bool no_ui)
    : AsyncWorker(Function::New(env, NullFunction, "theFunction"))
    , files(files)
    , target(target)
    , move(move)
    , no_ui(no_ui)
    , deferred(Promise::Deferred::New(Env())) {}
    ~Copy_worker() {}

    void Execute () { copy_files(files, target, move, no_ui, error, error_code); }

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
    vector<wstring> files;
    wstring target;
    bool move;
    bool no_ui;
    string error;
    int error_code;
    Promise::Deferred deferred;
};

Value copy_files(const CallbackInfo& info, bool move) {
    auto files_array = info[0].As<Array>();
    vector<wstring> files;
    for (auto i = 0U; i < files_array.Length(); i++) 
        files.push_back(files_array.Get(i).As<WString>());
    auto target = info[1].As<WString>();
    auto no_ui = info[2].As<Boolean>();
    auto worker = new Copy_worker(info.Env(), files, target, move, no_ui);
    worker->Queue();
    return worker->Promise();
}

