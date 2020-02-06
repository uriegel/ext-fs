#define NAPI_EXPERIMENTAL
#include <napi.h>
#include <vector>
#include <string>
#include "nullfunction.h"
#include "get_line_indexes_worker.h"
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
    //int size{1001};
    file.seekg(0, ios::end);
    auto file_size{static_cast<int>(file.tellg())};
    file.seekg(0, ios::beg);
    auto buffer{new char[size]};
    int line_index{0};
    while (true) {
        auto buffer_size = min(size - 1, file_size - file_offest);
        file.read(buffer, buffer_size);
        buffer[buffer_size] = 0;
        
        while (true) {
            auto found = strchr(buffer + max(0, line_index), '\n');
            if (found == nullptr)
                break;
            auto next_line_index = found - buffer + 1;
            line_indexes.emplace_back(line_index + file_offest, next_line_index - line_index - 1);
            line_index = next_line_index;
        }
        file_offest += buffer_size;
        line_index -= buffer_size;
        if (file_offest == file_size)
            break;
    }
}

void Get_line_indexes_worker::OnOK() {
    auto env = Env();
    HandleScope scope(env);

    auto array = Array::New(Env(), line_indexes.size());
    int i{0};
    for(auto item: line_indexes) {
        auto obj = Object::New(Env());

        obj.Set("index", Number::New(Env(), static_cast<double>(item.start_pos)));
        obj.Set("length", Number::New(Env(), static_cast<double>(item.length)));

        array.Set(i++, obj);
    }

    deferred.Resolve(array);}

