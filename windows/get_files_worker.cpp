#define NAPI_EXPERIMENTAL
#include <napi.h>
#include <string>
#include <vector>
#include "wstring.h"
#include "nullfunction.h"
#include "get_files_worker.h"
using namespace Napi;
using namespace std;

struct File_item {
	File_item(const wstring& display_name, bool is_directory,  bool is_hidden, uint64_t size, uint64_t time)		
	: display_name(display_name)
	, is_directory(is_directory)
	, is_hidden(is_hidden)
	, size(size)
	, time(time) {}
	const wstring display_name;
	const bool is_directory;
	const bool is_hidden;
	const uint64_t size;
	const uint64_t time;
};

vector<File_item> get_files(const wstring& directory);

class Get_files_worker : public AsyncWorker {
public:
    Get_files_worker(const Napi::Env& env, const wstring& directory)
    : AsyncWorker(Function::New(env, NullFunction, "theFunction"))
    , directory(directory)
    , deferred(Promise::Deferred::New(Env())) {}
    ~Get_files_worker() {}

    void Execute () { files = move(get_files(directory)); }

    void OnOK();

    Promise Promise() { return deferred.Promise(); }

private:
    Promise::Deferred deferred;
    wstring directory;
    vector<File_item> files;
};

void Get_files_worker::OnOK() {
    HandleScope scope(Env());

    auto array = Array::New(Env(), files.size());
    int i{0};
    for(auto item: files) {
        auto obj = Object::New(Env());

        obj.Set("name", WString::New(Env(), item.display_name));
        obj.Set("size", Number::New(Env(), static_cast<double>(item.size)));
        napi_value time;
        napi_create_date(Env(), static_cast<double>(item.time), &time);
        obj.Set("time", time);
        obj.Set("isDirectory", Boolean::New(Env(), item.is_directory));
        obj.Set("isHidden", Boolean::New(Env(), item.is_hidden));

        array.Set(i++, obj);
    }

    deferred.Resolve(array);
}

Value GetFiles(const CallbackInfo& info) {
    auto directory = info[0].As<WString>().WValue();

    auto worker = new Get_files_worker(info.Env(), directory);
    worker->Queue();
    return worker->Promise();
}
