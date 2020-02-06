#pragma once
#include <napi.h>
#include <iostream>
#include <fstream>

class Get_line_count_worker : public Napi::AsyncWorker {
public:
    Get_line_count_worker(const Napi::Env& env, std::function<long long()> get_line_count);
    ~Get_line_count_worker() {}

    void Execute();
    void OnOK();

    Napi::Promise Promise() { return deferred.Promise(); }

private:
    Napi::Promise::Deferred deferred;
    std::function<long long()> get_line_count;
    long long line_count;
};

