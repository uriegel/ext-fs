#pragma once

struct Version_info {
    Version_info(int major, int minor, int build, int patch) 
    : major(major)
    , minor(minor)
    , build(build)
    , patch(patch) {}

    int major;
    int minor;
    int build;
    int patch;
};
