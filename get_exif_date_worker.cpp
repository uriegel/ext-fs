#include "get_exif_date_worker.h"
using namespace Nan;
using namespace std;

void Get_exif_date_worker::HandleOKCallback() {
    HandleScope scope;

    auto date = New<v8::Date>(static_cast<double>(exif_date)).ToLocalChecked();

    auto resolver = GetFromPersistent(1).As<v8::Promise::Resolver>();
    resolver->Resolve(GetCurrentContext(), date);
}