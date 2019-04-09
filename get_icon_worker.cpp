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
    
    auto resolver = GetFromPersistent(1).As<v8::Promise::Resolver>();
    resolver->Resolve(GetCurrentContext(), buffer);
}