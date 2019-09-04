#pragma once
#define NAPI_EXPERIMENTAL
#include <napi.h>
#include <event-source-base.h>

class Services: public Napi::ObjectWrap<Services> {
    class Events : public EventSourceBase::EventSourceBase {
    public:
        Events(Services& services, const Napi::Function& callback)
        : EventSourceBase(callback)
        , services(services) { }

        void OnEvent() override;
        void start();
        void stop();
    private:
        bool is_running{false};
        Services& services;
    };

public:
    static void Init(Napi::Env& env, Napi::Object& exports);
    Services(const Napi::CallbackInfo& info);
    ~Services();

private:    
    static Napi::FunctionReference constructor;
    std::shared_ptr<Events> events;
    std::tuple<std::vector<unsigned char>, int> services;
    SC_HANDLE handle{0};

    auto get(const Napi::CallbackInfo &info) -> Napi::Value;
    auto unregister_events(const Napi::CallbackInfo &info)-> Napi::Value;
};

