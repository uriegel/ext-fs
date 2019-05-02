#pragma once
#include <napi.h>

Napi::Value copy_files(const Napi::CallbackInfo& info, bool move);

inline Napi::Value CopyFiles(const Napi::CallbackInfo& info) { return copy_files(info, false); }
inline Napi::Value MoveFiles(const Napi::CallbackInfo& info) { return copy_files(info, true); }


