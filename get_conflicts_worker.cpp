#define NAPI_EXPERIMENTAL
#include <napi.h>
#include <vector>
#include "wstring.h"
#include "nullfunction.h"
#include "get_conflicts_worker.h"
#include "conflict_item.h"
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
    vector<Conflict_item> conflicts;
};

void Get_conflicts_worker::OnOK() {
    HandleScope scope(Env());

    auto array = Array::New(Env(), conflicts.size());
    int i{0};
    for (auto item: conflicts) {
        auto obj = Object::New(Env());

        obj.Set("name", WString::New(Env(), item.name));
        
        obj.Set("sourceSize", Number::New(Env(), static_cast<double>(item.source_size)));
        napi_value time;
        napi_create_date(Env(), static_cast<double>(item.source_time), &time);
        obj.Set("sourceTime", time);

        obj.Set("targetSize", Number::New(Env(), static_cast<double>(item.target_size)));
        napi_create_date(Env(), static_cast<double>(item.target_time), &time);
        obj.Set("targetTime", time);
        
        array.Set(i++, obj);
    }

    deferred.Resolve(array);
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

