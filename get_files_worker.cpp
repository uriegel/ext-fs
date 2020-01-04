#define NAPI_EXPERIMENTAL
#include <napi.h>
#include <filesystem>
#include <vector>
#include "wstring.h"
#include "nullfunction.h"
#include "get_files_worker.h"
using namespace Napi;
using namespace std;
using namespace filesystem;


struct File_item {
	File_item(string&& display_name, bool is_directory,  bool is_hidden, uint64_t size, uint64_t time)		
	: display_name(display_name)
	, is_directory(is_directory)
	, is_hidden(is_hidden)
	, size(size)
	, time(time) {}
	const string display_name;
	const bool is_directory;
	const bool is_hidden;
	const uint64_t size;
	const uint64_t time;
};

vector<File_item> get_files(const string& directory) {
    vector<File_item> result;
    for (auto entry : directory_iterator(directory)) {
        result.emplace_back(entry.path().filename().string(), entry.is_directory(), false, entry.file_size(), 0);
    }
    return result;
}

class Get_files_worker : public AsyncWorker {
public:
    Get_files_worker(const Napi::Env& env, const string& directory)
    : AsyncWorker(Function::New(env, NullFunction, "theFunction"))
    , directory(directory)
    , deferred(Promise::Deferred::New(Env())) {}
    ~Get_files_worker() {}

    void Execute () { files = move(get_files(directory)); }

    void OnOK();

    Napi::Promise Promise() { return deferred.Promise(); }

private:
    string directory;
    Napi::Promise::Deferred deferred;
    vector<File_item> files;
};

void Get_files_worker::OnOK() {
    HandleScope scope(Env());

    auto array = Array::New(Env(), files.size());
    int i{0};
    for(auto item: files) {
        auto obj = Object::New(Env());

        obj.Set("name", String::New(Env(), item.display_name));
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
    auto directorys = (string)info[0].As<String>();

    auto worker = new Get_files_worker(info.Env(), directorys);
    worker->Queue();
    return worker->Promise();
}
