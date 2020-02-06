#pragma once 

struct Line_index {
    long long start_pos;
    int length = 0;
    Line_index(long long start_pos, int length) : start_pos(start_pos), length(length) {}
};