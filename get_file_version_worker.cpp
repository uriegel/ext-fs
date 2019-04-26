#define NAPI_EXPERIMENTAL
#include <napi.h>
#include <vector>
#include "wstring.h"
#include "nullfunction.h"
#include "version_info.h"
#if WINDOWS
#include "windows/utils.h"
#elif LINUX
#endif
using namespace Napi;
using namespace std;

class Get_file_version_worker : public AsyncWorker {
public:
    Get_file_version_worker(const Napi::Env& env, const wstring& file)
    : AsyncWorker(Function::New(env, NullFunction, "nullfunction"))
    , file(file)
    , deferred(Promise::Deferred::New(Env())) {}
    ~Get_file_version_worker() {}

    void Execute () { version = get_file_info_version(file); }

    void OnOK();

    Promise Promise() { return deferred.Promise(); }

private:
    Promise::Deferred deferred;
    wstring file;
    Version_info version;
};

void Get_file_version_worker::OnOK() {
    auto env = Env();
    HandleScope scope(env);

    auto obj = Object::New(Env());

    obj.Set("major", Number::New(Env(), static_cast<double>(version.major)));
    obj.Set("minor", Number::New(Env(), static_cast<double>(version.minor)));
    obj.Set("build", Number::New(Env(), static_cast<double>(version.build)));
    obj.Set("patch", Number::New(Env(), static_cast<double>(version.patch)));

    deferred.Resolve(obj);
}

Value GetFileVersion(const CallbackInfo& info) {
    auto file = info[0].As<WString>().WValue();

    auto worker = new Get_file_version_worker(info.Env(), file);
    worker->Queue();
    return worker->Promise();
}

