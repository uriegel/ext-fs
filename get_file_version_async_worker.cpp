#include "get_file_version_async_worker.h"
using namespace Nan;

void Get_file_version_async_worker::WorkComplete(v8::Local<v8::Promise::Resolver>& resolver) {
    v8::Local<v8::Object> result = New<v8::Object>();
    result->Set(New<v8::String>("major").ToLocalChecked(), New<v8::Number>(static_cast<double>(version.major)));
    result->Set(New<v8::String>("minor").ToLocalChecked(), New<v8::Number>(static_cast<double>(version.minor)));
    result->Set(New<v8::String>("build").ToLocalChecked(), New<v8::Number>(static_cast<double>(version.build)));
    result->Set(New<v8::String>("patch").ToLocalChecked(), New<v8::Number>(static_cast<double>(version.patch)));
    resolver->Resolve(GetCurrentContext(), result);
}

