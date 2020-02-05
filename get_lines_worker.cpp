#define NAPI_EXPERIMENTAL
#include <napi.h>
#include <vector>
#include <string>
#include "nullfunction.h"
#include "get_icon_worker.h"
#include <iostream>
#include <fstream>
using namespace Napi;
using namespace std;

class Get_lines_worker : public AsyncWorker {
public:
#ifdef WINDOWS
    Get_lines_worker(const Napi::Env& env, const wstring& file)
#elif LINUX
    Get_lines_worker(const Napi::Env& env, const string& file)
#endif    
    : AsyncWorker(Function::New(env, NullFunction, "nullfunction"))
    , file(file)
    , deferred(Promise::Deferred::New(Env())) {}
    ~Get_lines_worker() {}

    void Execute () { 
        ifstream myfile(file.c_str(), ios::in | ios::binary);
        if (myfile.is_open()) {
            int recent_pos = 0;
            int size = 20000001;
            myfile.seekg(0, ios::end);
            auto file_size = myfile.tellg();
            myfile.seekg(0, ios::beg);
            auto buffer = new char[size];
            while (true) {
                myfile.read(buffer, size - 1);
                auto pos = myfile.tellg();
                auto brk = pos == -1;
                if (brk)
                    pos = file_size;
                buffer[pos - recent_pos] = 0;
                recent_pos = pos;
                if (brk)
                    break;
            }
            myfile.close();
        }
    }

    void OnOK();

    Napi::Promise Promise() { return deferred.Promise(); }

private:
    Promise::Deferred deferred;
#ifdef WINDOWS
    wstring file;
#elif LINUX
    string file;
#endif
    vector<int> lines;
};

void Get_lines_worker::OnOK() {
    auto env = Env();
    HandleScope scope(env);

    // auto buffer_result = new vector<char>(move(icon_bytes));
    // auto buffer = Buffer<char>::New(env, buffer_result->data(), buffer_result->size(), [](Napi::Env, char*, vector<char>* to_delete){ delete to_delete; }, buffer_result);
    // deferred.Resolve(buffer);
}

Value GetLines(const CallbackInfo& info) {
#ifdef WINDOWS    
    auto file = info[0].As<WString>().WValue();
#elif LINUX
    auto file = (string)info[0].As<String>();
#endif

    auto worker = new Get_lines_worker(info.Env(), file);
    worker->Queue();
    return worker->Promise();
}

