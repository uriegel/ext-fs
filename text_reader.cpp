#include <thread>
#include "text_reader.h"
#include "get_line_count_worker.h"
#include "get_lines_worker.h"
#include "wstring.h"
using namespace std;
using namespace Napi;

void Text_reader::Init(Napi::Env env, Object& exports) {
    HandleScope scope(env);

    Function func = DefineClass(
        env,
        "TextReader", {
            InstanceMethod("getLineCount", &Text_reader::get_line_count),
            InstanceMethod("getLines", &Text_reader::get_lines)
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

void Text_reader::get_line_indexes() {
    long long file_offest{0};
    int size{20000001};
    //int size{1001};
    file.seekg(0, ios::end);
    auto file_size{static_cast<long long>(file.tellg())};
    file.seekg(0, ios::beg);
    auto buffer{new char[size]};
    long long line_index{0};
    while (true) {
        auto buffer_size = min((long long)size - 1, file_size - file_offest);
        file.read(buffer, buffer_size);
        buffer[buffer_size] = 0;
        
        while (true) {
            auto found = strchr(buffer + max((long long)0, line_index), '\n');
            if (found == nullptr)
                break;
            auto next_line_index = found - buffer + (long long)1;
            line_indexes.emplace_back(line_index + file_offest, next_line_index - line_index - 1);
            line_index = next_line_index;
        }
        file_offest += buffer_size;
        line_index -= buffer_size;
        if (file_offest == file_size)
            break;
    }    
}

Napi::Value Text_reader::get_line_count(const Napi::CallbackInfo& info) {
    auto worker = new Get_line_count_worker(info.Env(), [this]() {
        if (line_indexes.empty())
            get_line_indexes();
        return line_indexes.size();
    });
    worker->Queue();
    return worker->Promise();
}

Napi::Value Text_reader::get_lines(const Napi::CallbackInfo& info) {
    auto items_array = info[0].As<Array>();
    vector<long long> lines;
    for (auto i = 0U; i < items_array.Length(); i++) {
        auto val = items_array.Get(i).As<Number>().Int32Value();
        lines.push_back(val);
    }
    auto worker = new Get_lines_worker(info.Env(), lines, line_indexes, file);
    worker->Queue();
    return worker->Promise();
}

Napi::FunctionReference Text_reader::constructor;