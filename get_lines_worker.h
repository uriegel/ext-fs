#pragma once
#include <napi.h>
#include <iostream>
#include <fstream>
#include "line_index.h"

class Get_lines_worker : public Napi::AsyncWorker {
public:
    Get_lines_worker(const Napi::Env& env, std::vector<long long>& lines, std::vector<Line_index>& line_indexes, std::ifstream& file);
    ~Get_lines_worker() {}

    void Execute();
    void OnOK();

    Napi::Promise Promise() { return deferred.Promise(); }

private:
    Napi::Promise::Deferred deferred;
    std::vector<long long> lines;
    std::vector<Line_index>& line_indexes;
    std::vector<std::string> result;
    std::ifstream& file;
};

