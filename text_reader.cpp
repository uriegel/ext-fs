#include <thread>
#include "get_line_indexes_worker.h"
#include "get_lines_worker.h"
#include "text_reader.h"
#include "wstring.h"
using namespace std;
using namespace Napi;

void Text_reader::Init(Napi::Env env, Object& exports) {
    HandleScope scope(env);

    Function func = DefineClass(
        env,
        "TextReader", {
            InstanceMethod("getLineIndexes", &Text_reader::GetLineIndexes),
            InstanceMethod("getLines", &Text_reader::GetLines)
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

Napi::Value Text_reader::GetLines(const Napi::CallbackInfo& info) {
    auto items_array = info[0].As<Array>();
    vector<Line_index> line_indexes;
    for (auto i = 0U; i < items_array.Length(); i++) {
        auto obj = items_array.Get(i).As<Napi::Object>();
        auto pos = obj.Get("index").As<Number>().Int32Value();
        auto length = obj.Get("length").As<Number>().Int32Value();
        line_indexes.emplace_back(pos, length);
    }
    auto worker = new Get_lines_worker(info.Env(), move(line_indexes), file);
    worker->Queue();
    return worker->Promise();
}

Napi::FunctionReference Text_reader::constructor;