#include "Get_files_worker.h"
using namespace std;
using namespace Nan;

void Get_files_worker::HandleOKCallback () {
    HandleScope scope;

    v8::Local<v8::Array> result_list = New<v8::Array>(results.size());
    int i{0};
    for(auto item: results) {
        v8::Local<v8::Object> result = New<v8::Object>();

        result->Set(New<v8::String>("name").ToLocalChecked(), 
            New<v8::String>((uint16_t*)item.display_name.c_str()).ToLocalChecked()); 
            result->Set(New<v8::String>("size").ToLocalChecked(), New<v8::Number>(static_cast<double>(item.size)));
            result->Set(New<v8::String>("time").ToLocalChecked(), New<v8::Date>(static_cast<double>(item.time)).ToLocalChecked());
            result->Set(New<v8::String>("isDirectory").ToLocalChecked(), New<v8::Boolean>(item.is_directory));
            result->Set(New<v8::String>("isHidden").ToLocalChecked(), New<v8::Boolean>(item.is_hidden));
        result_list->Set(i++, result);
    }        

    v8::Local<v8::Value> argv[] = { Null(), result_list};
    callback->Call(2, argv, async_resource);
}