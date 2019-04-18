#include "get_exif_date_worker.h"
using namespace Nan;
using namespace std;

void Get_exif_date_worker::HandleOKCallback() {
    HandleScope scope;

    auto resolver = GetFromPersistent(1).As<v8::Promise::Resolver>();

    if (exif_date) {
        auto date = New<v8::Date>(static_cast<double>(exif_date)).ToLocalChecked();
        resolver->Resolve(GetCurrentContext(), date);
    }
    else
        resolver->Resolve(GetCurrentContext(), Undefined());
}