#define NAPI_EXPERIMENTAL
#include <napi.h>
#include <vector>
#include "wstring.h"
#include "nullfunction.h"
#include "get_exif_date_worker.h"
#include "file_item.h"
#include "exif_reader.h"
#if WINDOWS
#include "windows/utils.h"
#elif LINUX
#endif
using namespace Napi;
using namespace std;

class Get_exif_date_worker : public AsyncWorker {
public:
    Get_exif_date_worker(const Napi::Env& env, const wstring& file)
    : AsyncWorker(Function::New(env, NullFunction, "theFunction"))
    , file(file)
    , deferred(Promise::Deferred::New(Env())) {}
    ~Get_exif_date_worker() {}

    void Execute () { exif_date = get_exif_date(file); }

    void OnOK();

    Promise Promise() { return deferred.Promise(); }

private:
    Promise::Deferred deferred;
    wstring file;
    uint64_t exif_date;
};

void Get_exif_date_worker::OnOK() {
    HandleScope scope(Env());

    if (exif_date) {
        napi_value time;
        napi_create_date(Env(), static_cast<double>(exif_date), &time);
        deferred.Resolve(time);
    }
    else 
        deferred.Resolve(Env().Null());
}

Value GetExifDate(const CallbackInfo& info) {
    auto file = info[0].As<WString>().WValue();

    auto worker = new Get_exif_date_worker(info.Env(), file);
    worker->Queue();
    return worker->Promise();
}
