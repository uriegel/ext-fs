#define NAPI_EXPERIMENTAL
#include <napi.h>
#include <vector>
#include "wstring.h"
#include "nullfunction.h"
#include "get_drives_worker.h"
#include "drive_item.h"
#if WINDOWS
#include "windows/utils.h"
#elif LINUX
#endif
using namespace Napi;
using namespace std;

class Get_drives_worker : public AsyncWorker {
public:
    Get_drives_worker(const Napi::Env& env)
    : AsyncWorker(Function::New(env, NullFunction, "theFunction"))
    , deferred(Promise::Deferred::New(Env())) {}
    ~Get_drives_worker() {}

    void Execute () { get_drives(drives); }

    void OnOK();

    Promise Promise() { return deferred.Promise(); }

private:
    Promise::Deferred deferred;
    vector<Drive_item> drives;
};

void Get_drives_worker::OnOK() {
    HandleScope scope(Env());

    auto array = Array::New(Env(), drives.size());
    int i{0};
    for(auto item: drives) {
        auto obj = Object::New(Env());

        obj.Set("name", WString::New(Env(), item.name));
        obj.Set("description", WString::New(Env(), item.description));
        obj.Set("size", Number::New(Env(), static_cast<double>(item.size)));
        obj.Set("type", Number::New(Env(), static_cast<double>(item.type)));
        obj.Set("isMounted", Boolean::New(Env(), item.is_mounted));

        array.Set(i++, obj);
    }

    deferred.Resolve(array);
}

Value GetDrives(const CallbackInfo& info) {
    auto worker = new Get_drives_worker(info.Env());
    worker->Queue();
    return worker->Promise();
}

