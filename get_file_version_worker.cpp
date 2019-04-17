#include "get_file_version_worker.h"
using namespace Nan;
using namespace std;

void Get_file_version_worker::HandleOKCallback() {
    HandleScope scope;

    v8::Local<v8::Object> result = New<v8::Object>();
    result->Set(New<v8::String>("major").ToLocalChecked(), New<v8::Number>(static_cast<double>(version.major)));
    result->Set(New<v8::String>("minor").ToLocalChecked(), New<v8::Number>(static_cast<double>(version.minor)));
    result->Set(New<v8::String>("build").ToLocalChecked(), New<v8::Number>(static_cast<double>(version.build)));
    result->Set(New<v8::String>("patch").ToLocalChecked(), New<v8::Number>(static_cast<double>(version.patch)));

    auto resolver = GetFromPersistent(1).As<v8::Promise::Resolver>();
    resolver->Resolve(GetCurrentContext(), result);
}