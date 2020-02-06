#define NAPI_EXPERIMENTAL
#include <napi.h>
#include <vector>
#include <string>
#include "nullfunction.h"
#include "get_line_count_worker.h"
#include <iostream>
#include <fstream>
using namespace Napi;
using namespace std;

Get_line_count_worker::Get_line_count_worker(const Napi::Env& env, function<long long()> get_line_count)
    : AsyncWorker(Function::New(env, NullFunction, "nullfunction"))
    , get_line_count(get_line_count)
    , deferred(Promise::Deferred::New(Env())) {}
    
void Get_line_count_worker::Execute () { 
    line_count = get_line_count();
}

void Get_line_count_worker::OnOK() {
    auto env = Env();
    HandleScope scope(env);
    deferred.Resolve(Number::New(Env(), static_cast<double>(line_count)));
}

