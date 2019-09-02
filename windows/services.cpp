#include <Windows.h>
#include <string>
#include <vector>
#include "services.h"
#include "../wstring.h"
using namespace std;
using namespace Napi;

void Services::Init(Napi::Env& env, Object& exports) {

    Function ctor = DefineClass(env, "Services", {
        InstanceMethod("get", &Get)
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

Services::Services(const CallbackInfo& info) : ObjectWrap<Services>(info) {
    Napi::Env env = info.Env();
    handle = OpenSCManagerW(nullptr, nullptr, SC_MANAGER_ENUMERATE_SERVICE);
}

Services::~Services() {
    CloseServiceHandle(handle);
}

vector<ENUM_SERVICE_STATUS_PROCESSW> access(vector<BYTE>& bytes, int number_of_services) {
    return vector<ENUM_SERVICE_STATUS_PROCESSW>(reinterpret_cast<ENUM_SERVICE_STATUS_PROCESSW*>(bytes.data()), 
         reinterpret_cast<ENUM_SERVICE_STATUS_PROCESSW*>(bytes.data()) + number_of_services);
}

Value Services::Get(const CallbackInfo &info) {
    auto [bytes, count] = get_services();
    auto services = access(bytes, count);

    auto result = Array::New(info.Env(), services.size());
    int i{0};
    for(auto item: services) {
        auto obj = Object::New(Env());

        obj.Set("name", WString::New(Env(), item.lpServiceName));
        obj.Set("displayName", WString::New(Env(), item.lpDisplayName));
        obj.Set("status", Number::New(Env(), item.ServiceStatusProcess.dwCurrentState));

        result.Set(i++, obj);
    }

    return result;
}

auto Services::get_services() -> tuple<vector<BYTE>, int> {
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

FunctionReference Services::constructor;