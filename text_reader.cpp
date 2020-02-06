#include <thread>
#include "Get_line_indexes_worker.h"
#include "text_reader.h"
#include "wstring.h"
using namespace std;
using namespace Napi;

void Text_reader::Init(Napi::Env env, Object& exports) {
    HandleScope scope(env);

    Function func = DefineClass(
        env,
        "TextReader", {
            InstanceMethod("getLineIndexes", &Text_reader::GetLineIndexes)
        }
    );

    constructor = Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("TextReader", func);
} 

Text_reader::Text_reader(const CallbackInfo& info)
    : ObjectWrap<Text_reader>(info) {
    auto env = info.Env();
    HandleScope scope(env);

    int length = info.Length();

    if (length <= 0 || !info[0].IsString()) {
        TypeError::New(env, "String expected").ThrowAsJavaScriptException();
        return;
    }

#ifdef WINDOWS    
    file_name = info[0].As<WString>().WValue();
#elif LINUX
    file_name = (string)info[0].As<String>();
#endif
    file.open(file_name.c_str(), ios::in | ios::binary);
    if (!file.is_open()) {
        Error::New(env, "Could not open file").ThrowAsJavaScriptException();
    }
}
  
Text_reader::~Text_reader() {
    file.close();
}

Napi::Value Text_reader::GetLineIndexes(const Napi::CallbackInfo& info) {

    auto worker = new Get_line_indexes_worker(info.Env(), file);
    worker->Queue();
    return worker->Promise();
}

Napi::FunctionReference Text_reader::constructor;