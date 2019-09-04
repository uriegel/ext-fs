#pragma once
#define NAPI_EXPERIMENTAL
#include <napi.h>
#include <event-source-base.h>

class Services: public Napi::ObjectWrap<Services> {
    class Events : public EventSourceBase::EventSourceBase {
    public:
        Events(const Napi::Function& callback) 
            : EventSourceBase(callback)
        {}
        void OnEvent() override;
        void start();
        void stop();
    private:
        bool is_running{false};
    };

public:
    static void Init(Napi::Env& env, Napi::Object& exports);
    Services(const Napi::CallbackInfo& info);
    ~Services();

private:    
    static Napi::FunctionReference constructor;
    std::shared_ptr<Events> events;
    SC_HANDLE handle{0};

    auto get(const Napi::CallbackInfo &info) -> Napi::Value;
    auto register_events(const Napi::CallbackInfo &info)-> Napi::Value;
    auto unregister_events(const Napi::CallbackInfo &info)-> Napi::Value;
    auto get_services() -> std::tuple<std::vector<BYTE>, int>;
};

