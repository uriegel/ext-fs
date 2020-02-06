#include <thread>
#include "text_reader.h"
//#include "AsyncContext.h"
#include "wstring.h"
using namespace std;
using namespace Napi;

void Text_reader::Init(Napi::Env env, Object& exports) {
    HandleScope scope(env);

    Function func = DefineClass(
        env,
        "TextReader", {
            InstanceMethod("getFileIndexes", &Text_reader::GetFileIndexes)
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

Napi::Value Text_reader::GetFileIndexes(const Napi::CallbackInfo& info) {
    int recent_pos = 0;
    int size = 20000001;
    file.seekg(0, ios::end);
    auto file_size = static_cast<int>(file.tellg());
    file.seekg(0, ios::beg);
    auto buffer = new char[size];
    while (true) {
        file.read(buffer, size - 1);
        auto pos = static_cast<int>(file.tellg());
        auto brk = pos == -1;
        if (brk)
            pos = file_size;
        buffer[pos - recent_pos] = 0;
        recent_pos = pos;
        if (brk)
            break;
    }

    return Napi::Number::New(info.Env(), 23);
}

Napi::FunctionReference Text_reader::constructor;