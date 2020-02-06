#pragma once
#include <napi.h>
#include <iostream>
#include <fstream>

struct Line_index {
    int start_pos;
    int length = 0;
    Line_index(int start_pos, int length) : start_pos(start_pos), length(length) {}
};

class Get_line_indexes_worker : public Napi::AsyncWorker {
public:
    Get_line_indexes_worker(const Napi::Env& env, std::ifstream& file);
    ~Get_line_indexes_worker() {}

    void Execute();
    void OnOK();

    Napi::Promise Promise() { return deferred.Promise(); }

private:
    Napi::Promise::Deferred deferred;
    std::vector<Line_index> line_indexes;
    std::ifstream& file;
};

