#pragma once
#define NAPI_EXPERIMENTAL
#include <napi.h>

class Services: public Napi::ObjectWrap<Services> {
public:
    static void Init(Napi::Env& env, Napi::Object& exports);
    Services(const Napi::CallbackInfo& info);
    ~Services();

private:    
    static Napi::FunctionReference constructor;
    SC_HANDLE handle{0};

    Napi::Value Get(const Napi::CallbackInfo &info);
    auto get_services() -> std::tuple<std::vector<BYTE>, int>;
};
