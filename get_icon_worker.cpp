#include "get_icon_worker.h"
using namespace Nan;
using namespace std;

void buffer_delete_callback(char* unused, void* the_vector) {
    delete reinterpret_cast<vector<char>*>(the_vector);
}

void Get_icon_worker::HandleOKCallback() {
    auto iconPtr = icon;
    icon = nullptr;
    auto buffer = Nan::NewBuffer(iconPtr->data(), iconPtr->size(), buffer_delete_callback, iconPtr).ToLocalChecked();
    
    v8::Local<v8::Value> argv[] = { Null(), buffer};
    callback->Call(2, argv, async_resource);
}