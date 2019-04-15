#include "Get_drives_worker.h"
using namespace std;
using namespace Nan;

void Get_drives_worker::HandleOKCallback () {
    HandleScope scope;

    v8::Local<v8::Array> result_list = New<v8::Array>(results.size());
    int i{0};
    for(auto item: results) {
        v8::Local<v8::Object> result = New<v8::Object>();

        result->Set(New<v8::String>("name").ToLocalChecked(), 
            New<v8::String>((uint16_t*)item.name.c_str()).ToLocalChecked()); 
        result->Set(New<v8::String>("description").ToLocalChecked(), 
            New<v8::String>((uint16_t*)item.description.c_str()).ToLocalChecked()); 
        result->Set(New<v8::String>("size").ToLocalChecked(), New<v8::Number>(static_cast<double>(item.size)));
        result->Set(New<v8::String>("type").ToLocalChecked(), New<v8::Number>(static_cast<double>(item.type)));
        result->Set(New<v8::String>("isMounted").ToLocalChecked(), New<v8::Boolean>(item.is_mounted));
        result_list->Set(i++, result);
    }        

    auto resolver = GetFromPersistent(1).As<v8::Promise::Resolver>();
    resolver->Resolve(GetCurrentContext(), result_list);
}