#define NAPI_EXPERIMENTAL
#include <napi.h>
#include <vector>
#include <string>
#include "nullfunction.h"
#include "Get_line_indexes_worker.h"
#include <iostream>
#include <fstream>
using namespace Napi;
using namespace std;

Get_line_indexes_worker::Get_line_indexes_worker(const Napi::Env& env, ifstream& file)
    : AsyncWorker(Function::New(env, NullFunction, "nullfunction"))
    , file(file)
    , deferred(Promise::Deferred::New(Env())) {}
    
void Get_line_indexes_worker::Execute () { 
    int file_offest{0};
    int size{20000001};
    file.seekg(0, ios::end);
    auto file_size{static_cast<int>(file.tellg())};
    file.seekg(0, ios::beg);
    auto buffer{new char[size]};
    int line_index{0};
    while (true) {
        file.read(buffer, size - 1);
        auto pos = static_cast<int>(file.tellg());
        auto brk = pos == -1;
        if (brk)
            pos = file_size;
        buffer[pos - file_offest] = 0;
        auto char_offset{0};
        while (true) {
            auto found = strchr(buffer + char_offset, '\n');
            if (found == nullptr)
                break;
            auto next_line_index = found - buffer + file_offest;
            char_offset = next_line_index + 1;
            line_indexes.emplace_back(line_index, next_line_index - line_index - file_offest - 1);
            line_index = next_line_index;
        }
        file_offest = pos;
        if (brk)
            break;
    }
}

void Get_line_indexes_worker::OnOK() {
    auto env = Env();
    HandleScope scope(env);

    // auto buffer_result = new vector<char>(move(icon_bytes));
    // auto buffer = Buffer<char>::New(env, buffer_result->data(), buffer_result->size(), [](Napi::Env, char*, vector<char>* to_delete){ delete to_delete; }, buffer_result);
    // deferred.Resolve(buffer);
}

