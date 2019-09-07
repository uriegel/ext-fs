#pragma once
#define NAPI_EXPERIMENTAL
#include <napi.h>

Napi::Value GetServices(const Napi::CallbackInfo& info);
Napi::Value RegisterServiceEvents(const Napi::CallbackInfo& info);
Napi::Value UnregisterServiceEvents(const Napi::CallbackInfo& info);
Napi::Value StartService1(const Napi::CallbackInfo& info); 
Napi::Value StopService(const Napi::CallbackInfo& info); 

