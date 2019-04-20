#include "get_exif_date_worker.h"
using namespace Nan;
using namespace std;

void Get_exif_date_worker::HandleOKCallback() {
    HandleScope scope;

    if (exif_date) {
        auto date = New<v8::Date>(static_cast<double>(exif_date)).ToLocalChecked();
        v8::Local<v8::Value> argv[] = { Null(), date };
        callback->Call(2, argv, async_resource);
    }
    else {
        v8::Local<v8::Value> argv[] = { Null(), Null() };
        callback->Call(2, argv, async_resource);
    }
}

