#pragma once
#include <napi.h>
#include <iostream>
#include <fstream>
#include "get_line_indexes_worker.h"

class Get_lines_worker : public Napi::AsyncWorker {
public:
    Get_lines_worker(const Napi::Env& env, std::vector<Line_index>&& line_indexes, std::ifstream& file);
    ~Get_lines_worker() {}

    void Execute();
    void OnOK();

    Napi::Promise Promise() { return deferred.Promise(); }

private:
    Napi::Promise::Deferred deferred;
    std::vector<Line_index> line_indexes;
    std::vector<std::string> lines;
    std::ifstream& file;
};

