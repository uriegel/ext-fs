#define NAPI_EXPERIMENTAL
#include <napi.h>
#include <vector>
#include "wstring.h"
#include "nullfunction.h"
#include "get_icon_worker.h"
#if WINDOWS
#include "windows/utils.h"
#include "windows/get_icon.h"
#elif LINUX
#endif
using namespace Napi;
using namespace std;

class Get_icon_worker : public AsyncWorker {
public:
    Get_icon_worker(const Napi::Env& env, const wstring& extension)
    : AsyncWorker(Function::New(env, NullFunction, "nullfunction"))
    , extension(extension)
    , deferred(Promise::Deferred::New(Env())) {}
    ~Get_icon_worker() {}

    void Execute () {
        iconBytes = new vector<char>();
        get_icon(extension, iconBytes);
    }

    void OnOK();

    Promise Promise() { return deferred.Promise(); }

private:
    Promise::Deferred deferred;
    wstring extension;
    vector<char>* iconBytes{nullptr};
};

void finalizer(Env env, char* data, vector<char>* hint) {
    delete hint;
}

void Get_icon_worker::OnOK() {
    auto env = Env();
    HandleScope scope(env);

    auto buffer = Buffer<char>::New(env, iconBytes->data(), iconBytes->size(), finalizer, iconBytes);
    deferred.Resolve(buffer);
}

Value GetIcon(const CallbackInfo& info) {
    auto extension = info[0].As<WString>().WValue();

    auto worker = new Get_icon_worker(info.Env(), extension);
    worker->Queue();
    return worker->Promise();
}

