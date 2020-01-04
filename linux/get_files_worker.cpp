#define NAPI_EXPERIMENTAL
#include <napi.h>
#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
#include <vector>
#include "../nullfunction.h"
#include "../get_files_worker.h"
#include "file_item.h"
using namespace Napi;
using namespace std;

vector<File_item> get_files(const string& directory) {
    vector<File_item> result;

    auto dp = opendir(directory.c_str());
    if (dp) {
        while (auto ep = readdir(dp)) {            
            if (ep->d_type == DT_REG || ep->d_type == DT_DIR) {
                struct stat fa;
                stat((directory + "/"s + ep->d_name).c_str(), &fa);
                result.emplace_back(ep->d_name, ep->d_type == DT_DIR, ep->d_name[0] == '.', fa.st_size, fa.st_mtime * 1000);
            }
        }
        closedir(dp);
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
    auto directory = (string)info[0].As<String>();

    auto worker = new Get_files_worker(info.Env(), directory);
    worker->Queue();
    return worker->Promise();
}
