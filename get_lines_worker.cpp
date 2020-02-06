#define NAPI_EXPERIMENTAL
#include <napi.h>
#include <vector>
#include <string>
#include "nullfunction.h"
#include "get_lines_worker.h"
#include <iostream>
#include <fstream>
using namespace Napi;
using namespace std;

Get_lines_worker::Get_lines_worker(const Napi::Env& env, vector<Line_index>&& line_indexes, ifstream& file)
    : AsyncWorker(Function::New(env, NullFunction, "nullfunction"))
    , file(file)
    , line_indexes(line_indexes)
    , deferred(Promise::Deferred::New(Env())) {}

void Get_lines_worker::Execute () { 
    for (auto item: line_indexes) {
        file.seekg(item.start_pos, ios::beg);
        vector<char> buffer(item.length);
        file.read(buffer.data(), item.length);
        string str(buffer.data(), item.length);
        auto auot = str.c_str();
        lines.emplace_back(buffer.data(), item.length);
        auto auo = lines[0].c_str();
        auto uuu = 9;
    }
}

void Get_lines_worker::OnOK() {
    auto env = Env();
    HandleScope scope(env);

    auto array = Array::New(Env(), lines.size());
    int i{0};
    for (auto item: lines) {
        array.Set(i++, String::New(Env(), item));
    }

    deferred.Resolve(array);
}

