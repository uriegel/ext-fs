#define NAPI_EXPERIMENTAL
#include <napi.h>
#include <filesystem>
#include <iostream>
using namespace Napi;
using namespace std;

template<typename... Args>
bool all(Args... args) { return (... && args); }

Napi::Value GetTest(const Napi::CallbackInfo& info) {

auto helloworld {"Hallöchen Wörlt👶🙀"s};

    cout << helloworld << ", " << helloworld.length() << endl;

    vector<string> msg {"Hello", "C++", "World", "from", "VS Code!"};

    for (const string& word : msg)
        cout << word << " ";
    cout << endl;

    cout << boolalpha;

    cout << "all(): " << all() << endl;
    cout << "all(true): " << all(true) << endl;
    cout << "all(true, true, true, false): " << all(true, true, true, false) << std::endl;
    
    return Number::New(info.Env(), static_cast<double>(567));   
    
    }

Object Init(Env env, Object exports) {
    exports.Set(String::New(env, "getTest"), Function::New(env, GetTest));
    return exports;
}

const char* extensionfs = "extension-fs";
NODE_API_MODULE(extensionfs, Init)
