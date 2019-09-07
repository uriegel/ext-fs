#include <string>
#include <algorithm>
#include <iterator>
#include <thread>
#include <vector>
#include <event-source-base.h>
#include "services.h"
#include "../wstring.h"
#include <Windows.h>
using namespace std;
using namespace Napi;
using namespace EventSourceBase;

struct Service_item {
    Service_item(wstring&& name, wstring&& display_name, int status) 
    : name(name), display_name(display_name), status(status) {}
    wstring name;
    wstring display_name;
    int status;
};

class Events : public EventSourceBase {
public:
    Events(const Napi::Function& callback)
    : EventSourceBase(callback) { }

    void OnEvent() override;
    void start();
    void stop();
    void send_event() { SendEvent(); }
private:
    bool is_running{false};
};

vector<Service_item> service_items;
vector<Service_item> event_result;
SC_HANDLE service_handle{0};
vector<Events*> events;

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

auto GetServices(const CallbackInfo& info) -> Value {
    auto handle = service_handle ? service_handle : OpenSCManagerW(nullptr, nullptr, SC_MANAGER_ENUMERATE_SERVICE);
    service_items = get_services(handle);

    auto result = Array::New(info.Env(), service_items.size());
    int i{0};
    for (auto item: service_items) {
        auto obj = Object::New(info.Env());

        obj.Set("name", WString::New(info.Env(), item.name));
        obj.Set("displayName", WString::New(info.Env(), item.display_name));
        obj.Set("status", Number::New(info.Env(), item.status));

        result.Set(i++, obj);
    }

    if (!service_handle)
        CloseServiceHandle(handle);
    return result;
}

auto RegisterServiceEvents(const CallbackInfo& info) -> Value { 
    if (events.empty()) 
        service_handle = OpenSCManagerW(nullptr, nullptr, SC_MANAGER_ENUMERATE_SERVICE);

    auto event = new Events(info[0].As<Function>());
    events.push_back(event);
    event->start();
    if (events.size() == 1) {
        thread thread([] {
            while (true) {
                Sleep(500);
                if (events.empty())
                    break;
                auto new_service_items = get_services(service_handle);
                unordered_map<wstring, Service_item> old_services_map;
                transform(service_items.begin(), service_items.end(), inserter(old_services_map, old_services_map.end()),
                    [](auto service_item) { return make_pair(service_item.name, service_item); });

                auto changes = remove_if(new_service_items.begin(), new_service_items.end(), [old_services_map](auto service_item) {
                    auto it = old_services_map.find(service_item.name);
                    return it != old_services_map.end()
                    ? service_item.status == it->second.status
                    : true;
                }); 

                event_result.clear();
                transform(new_service_items.begin(), changes, back_inserter(event_result), [](auto item){ return item; });

                if (event_result.size() > 0) {
                    for (auto item: events) 
                        item->send_event();
                    service_items = new_service_items;
                }
            }
        });
        thread.detach();        
    }
    return Number::New(info.Env(), (long long)event);
}

auto UnregisterServiceEvents(const CallbackInfo& info) -> Value { 
    auto event = reinterpret_cast<Events*>(info[0].As<Number>().Int64Value());
    auto it = find(events.begin(), events.end(), event);
    events.erase(it);
    // TODO: race condition (thread)
    event->stop();

    if (events.empty()) 
        CloseServiceHandle(service_handle);
    return Value(); 
}
#include <iostream>
void service_action(const Env& env, const wstring& name, function<void(SC_HANDLE handle)> action) {
    auto sc_handle = OpenSCManagerW(nullptr, nullptr, GENERIC_READ);
    auto service =  OpenServiceW(sc_handle, name.c_str(), GENERIC_READ | SERVICE_START);
    auto error = GetLastError();
    cout << "first error: " << error << endl;    
    if (!service && error == 5) {
        Error::New(env, "5").ThrowAsJavaScriptException();
        CloseServiceHandle(sc_handle);
    }
    action(service);
    CloseServiceHandle(service);
    CloseServiceHandle(sc_handle);
}

auto StartService1(const CallbackInfo& info) -> Value { 
    auto name = info[0].As<WString>().WValue();
    service_action(info.Env(), name, [](SC_HANDLE service){ 
        auto res = StartServiceW(service, 0, nullptr); 
        auto error = GetLastError();
        cout << "first error: " << error << endl;        
    });
    return Value(); 
}

Napi::Value StopService(const Napi::CallbackInfo& info) {
    auto name = info[0].As<WString>().WValue();
    service_action(info.Env(), name, [](SC_HANDLE service){ 
        SERVICE_STATUS srv_stat{0};
        ControlService(service, SERVICE_CONTROL_STOP, &srv_stat); 
    });
    return Value(); 
}

void Events::start() { Initialize(); }

void Events::stop() { 
    Dispose();
}

void Events::OnEvent() { 
    HandleScope scope(callback.Env());
    vector<napi_value> args;

    auto result = Array::New(callback.Env(), event_result.size());
    int i{0};
    for (auto item: event_result) {
        auto obj = Object::New(callback.Env());

        obj.Set("name", WString::New(callback.Env(), item.name));
        obj.Set("displayName", WString::New(callback.Env(), item.display_name));
        obj.Set("status", Number::New(callback.Env(), item.status));

        result.Set(i++, obj);
    }

    args.push_back(result);
    callback.Call(args);
}

