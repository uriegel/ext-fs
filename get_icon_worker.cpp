#include "get_icon_worker.h"
using namespace Nan;
using namespace std;

void Get_icon_worker::HandleOKCallback() {
    auto buffer = Nan::NewBuffer(icon.data(), icon.size()).ToLocalChecked();

    auto resolver = GetFromPersistent(1).As<v8::Promise::Resolver>();
    resolver->Resolve(GetCurrentContext(), buffer);
}