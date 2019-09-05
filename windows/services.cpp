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

auto get_services(SC_HANDLE handle) -> tuple<vector<unsigned char>, int> {
    DWORD byte_count{0};
    DWORD number_of_services{0};
    DWORD resume_handle{0};
    EnumServicesStatusExW(handle, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, nullptr, 0, 
        &byte_count, &number_of_services, &resume_handle, nullptr);
    vector<BYTE> bytes(byte_count);
    EnumServicesStatusExW(handle, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, 
        reinterpret_cast<BYTE*>(bytes.data()), byte_count, &byte_count, &number_of_services, &resume_handle, nullptr);
    return make_tuple(move(bytes), number_of_services);
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

vector<ENUM_SERVICE_STATUS_PROCESSW> access(tuple<vector<unsigned char>, int>& mem) {
    auto& [bytes, number_of_services] = mem;
    return vector<ENUM_SERVICE_STATUS_PROCESSW>(reinterpret_cast<ENUM_SERVICE_STATUS_PROCESSW*>(bytes.data()), 
         reinterpret_cast<ENUM_SERVICE_STATUS_PROCESSW*>(bytes.data()) + number_of_services);
}

auto Services::get(const CallbackInfo &info) -> Napi::Value {
    services = get_services(handle);
    auto service_items = access(services);

    auto result = Array::New(info.Env(), service_items.size());
    int i{0};
    for (auto item: service_items) {
        auto obj = Object::New(info.Env());

        obj.Set("name", WString::New(info.Env(), item.lpServiceName));
        obj.Set("displayName", WString::New(info.Env(), item.lpDisplayName));
        obj.Set("status", Number::New(info.Env(), item.ServiceStatusProcess.dwCurrentState));

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
            auto new_service_items = access(new_services);
            unordered_map<wstring, ENUM_SERVICE_STATUS_PROCESSW> new_services_map;
            transform(new_service_items.begin(), new_service_items.end(), inserter(new_services_map, new_services_map.end()),
               [](auto essp) { return make_pair(essp.lpServiceName, essp); });

            auto recent_service_items = access(services.services);
            auto changes = remove_if(recent_service_items.begin(), recent_service_items.end(), [new_services_map](const ENUM_SERVICE_STATUS_PROCESSW& essp) {
                auto it = new_services_map.find(essp.lpServiceName);
                return it != new_services_map.end()
                    ? essp.ServiceStatusProcess.dwCurrentState == it->second.ServiceStatusProcess.dwCurrentState
                    : true;
            }); 

        	vector<ENUM_SERVICE_STATUS_PROCESSW> result;
	        transform(recent_service_items.begin(), changes, back_inserter(result), [](auto essp) { return essp; });

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
    args.push_back(Number::New(callback.Env(), value));
    callback.Call(args);
}

FunctionReference Services::constructor;