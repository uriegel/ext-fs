#include "services.h"
#include <Windows.h>
#include <string>
#include <algorithm>
#include <iterator>
#include <thread>
#include <vector>
#include "../wstring.h"
using namespace std;
using namespace Napi;

auto get_services(SC_HANDLE handle) {
    unsigned long byte_count{0};
    unsigned long number_of_services{0};
    unsigned long resume_handle{0};
    EnumServicesStatusExW(handle, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, nullptr, 0, 
        &byte_count, &number_of_services, &resume_handle, nullptr);
    vector<unsigned char> bytes(byte_count);
    EnumServicesStatusExW(handle, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, 
        reinterpret_cast<unsigned char*>(bytes.data()), byte_count, &byte_count, &number_of_services, &resume_handle, nullptr);

    auto services = vector<ENUM_SERVICE_STATUS_PROCESSW>(reinterpret_cast<ENUM_SERVICE_STATUS_PROCESSW*>(bytes.data()), 
         reinterpret_cast<ENUM_SERVICE_STATUS_PROCESSW*>(bytes.data()) + number_of_services);

    vector<Service_item> result;
    transform(services.cbegin(), services.cend(), back_inserter(result), [](ENUM_SERVICE_STATUS_PROCESSW item) { 
        return Service_item(item.lpServiceName, item.lpDisplayName, item.ServiceStatusProcess.dwCurrentState);
    });    
    return result;
}

void Services::Init(Napi::Env& env, Object& exports) {

    Function ctor = DefineClass(env, "Services", {
        InstanceMethod("get", &get),
        InstanceMethod("unregisterEvents", &unregister_events)
//     InstanceAccessor("type", &Canvas::GetType, nullptr),
//     InstanceValue("PNG_NO_FILTERS", Napi::Number::New(env, PNG_NO_FILTERS)),
//     StaticMethod("_registerFont", &Canvas::RegisterFont),
    });

    // Create a peristent reference to the class constructor. This will allow
    // a function called on a class prototype and a function
    // called on instance of a class to be distinguished from each other.
    constructor = Persistent(ctor);
    // Call the SuppressDestruct() method on the static data prevent the calling
    // to this destructor to reset the reference when the environment is no longer
    // available.    
    constructor.SuppressDestruct();
    exports.Set("Services", ctor);
}

Services::Services(const CallbackInfo& info) 
    : ObjectWrap<Services>(info)
    , events(info[0].IsFunction() ? make_shared<Events>(*this, info[0].As<Function>()) : nullptr) {
    Napi::Env env = info.Env();
    handle = OpenSCManagerW(nullptr, nullptr, SC_MANAGER_ENUMERATE_SERVICE);
}

Services::~Services() {
    CloseServiceHandle(handle);
}

auto Services::get(const CallbackInfo &info) -> Napi::Value {
    services = get_services(handle);

    auto result = Array::New(info.Env(), services.size());
    int i{0};
    for (auto item: services) {
        auto obj = Object::New(info.Env());

        obj.Set("name", WString::New(info.Env(), item.name));
        obj.Set("displayName", WString::New(info.Env(), item.display_name));
        obj.Set("status", Number::New(info.Env(), item.status));

        result.Set(i++, obj);
    }

    if (events)
        events->start();

    return result;
}

auto Services::unregister_events(const Napi::CallbackInfo &info) -> Napi::Value {
    if (events)
        events->stop();
    return Object::New(info.Env());
}

void Services::Events::start() {
    Initialize();
    is_running = true;
     thread thread([this] {
        while (true) {
            Sleep(500);
            if (!is_running)
                break;
            auto new_services = get_services(services.handle);
            unordered_map<wstring, Service_item> new_services_map;
            transform(new_services.begin(), new_services.end(), inserter(new_services_map, new_services_map.end()),
               [](auto service_item) { return make_pair(service_item.name, service_item); });

            auto changes = remove_if(services.services.begin(), services.services.end(), [new_services_map](auto service_item) {
                auto it = new_services_map.find(service_item.name);
                return it != new_services_map.end()
                    ? service_item.status == it->second.status
                    : true;
            }); 

	        transform(services.services.begin(), changes, back_inserter(result), [](auto item){ return item; });

            if (result.size() > 0) {
                SendEvent();
                // TODO: get_services has to return Service_item with wstrings!!!
                services.services = new_services;
            }
        }
    });
    thread.detach();        
}

void Services::Events::stop() {
    is_running = false;
    Dispose();
}

void Services::Events::OnEvent() { 
    HandleScope scope(callback.Env());
    vector<napi_value> args;
//    args.push_back(Number::New(callback.Env(), value));
    callback.Call(args);
}

FunctionReference Services::constructor;