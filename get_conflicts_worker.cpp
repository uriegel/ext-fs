#define NAPI_EXPERIMENTAL
#include <napi.h>
#include <vector>
#include "wstring.h"
#include "nullfunction.h"
#include "get_drives_worker.h"
#include "drive_item.h"
#if WINDOWS
#include "windows/utils.h"
#include "windows/get_conflicts.h"
#elif LINUX
#endif
using namespace Napi;
using namespace std;

class Get_conflicts_worker : public AsyncWorker {
public:
    Get_conflicts_worker(const Napi::Env& env, const wstring& sourcePath, const wstring& targetPath, const vector<wstring>& items)
    : AsyncWorker(Function::New(env, NullFunction, "theFunction"))
    , deferred(Promise::Deferred::New(Env()))
    , sourcePath(sourcePath)
    , targetPath(targetPath)
    , items(items) {}
    ~Get_conflicts_worker() {}

    void Execute () { get_conflicts(sourcePath, targetPath, items, conflicts); }

    void OnOK();

    Promise Promise() { return deferred.Promise(); }

private:
    wstring sourcePath;
    wstring targetPath;
    vector<wstring> items;
    Promise::Deferred deferred;
    vector<std::shared_ptr<Conflict>> conflicts;
};

void Get_conflicts_worker::OnOK() {
    HandleScope scope(Env());

    // auto array = Array::New(Env(), drives.size());
    // int i{0};
    // for(auto item: drives) {
    //     auto obj = Object::New(Env());

    //     obj.Set("name", WString::New(Env(), item.name));
    //     obj.Set("description", WString::New(Env(), item.description));
    //     obj.Set("size", Number::New(Env(), static_cast<double>(item.size)));
    //     obj.Set("type", Number::New(Env(), static_cast<double>(item.type)));
    //     obj.Set("isMounted", Boolean::New(Env(), item.is_mounted));

    //     array.Set(i++, obj);
    // }

    // deferred.Resolve(array);
}

Value GetConflicts(const CallbackInfo& info) {
    auto sourcePath = info[0].As<WString>().WValue();
    auto targetPath = info[1].As<WString>().WValue();
    auto items_array = info[2].As<Array>();
    vector<wstring> items;
    for (auto i = 0U; i < items_array.Length(); i++) 
        items.push_back(items_array.Get(i).As<WString>());
    auto worker = new Get_conflicts_worker(info.Env(), sourcePath, targetPath, items);
    worker->Queue();
    return worker->Promise();
}

