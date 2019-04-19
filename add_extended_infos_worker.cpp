#include "add_extended_infos_worker.h"
#include "std_utils.h"
#include "windows/utils.h"
#include "exif_reader.h"
using namespace Nan;
using namespace std;

Add_extended_infos_worker::Add_extended_infos_worker(const wstring& path, v8::Local<v8::Array>& fileItems, ReturnValue<v8::Value>& returnValue) 
    : AsyncWorker(nullptr)
    , path(path) {
        auto context = GetCurrentContext();
        auto resolver = v8::Promise::Resolver::New(context).ToLocalChecked();
        SaveToPersistent(1, resolver);
        SaveToPersistent(2, fileItems);
        returnValue.Set(resolver->GetPromise());

        auto length = fileItems->Length();
        for (auto i = 0u; i < length; i++) {
            auto value = fileItems->Get(i);
            auto item = value.As<v8::Object>();

			auto prop_name = New<v8::String>("isDirectory").ToLocalChecked();
			auto isMaybeDirectory = item->Get(prop_name)->BooleanValue(context);
            if (isMaybeDirectory.IsJust() && isMaybeDirectory.FromJust())
                pathes.push_back(L""s);
            else {
                prop_name = New<v8::String>("name").ToLocalChecked();
                v8::String::Value s(context->GetIsolate(), item->Get(prop_name));            
                pathes.push_back(reinterpret_cast<wchar_t*>(*s));
            }
        }
    }

void Add_extended_infos_worker::Execute() {
    for (auto i = 0; i < pathes.size(); i++) {
        if (!pathes[i].empty() && (findString(pathes[i], L".exe"s) != -1 || findString(pathes[i], L".dll"s) != -1)) {
            auto file = path;
            combine_path(file, pathes[i]);
            Extended_info info {0, get_file_info_version(file) };
            result.push_back(info);
        } else if (!pathes[i].empty() && findString(pathes[i], L".jpg"s) != -1) {
            auto file = path;
            combine_path(file, pathes[i]);
            Extended_info info {get_exif_date(file), 0 };
            result.push_back(info);
        } else
            result.push_back({0, 0});
    }
}   

void Add_extended_infos_worker::HandleOKCallback () {
    HandleScope scope;

    auto resolver = GetFromPersistent(1).As<v8::Promise::Resolver>();
    auto fileItems = GetFromPersistent(2).As<v8::Array>();

    for (auto i = 0; i < result.size(); i++) {
        auto item = result[i];
        if (item.exif_date) {
            auto date = New<v8::Date>(static_cast<double>(item.exif_date)).ToLocalChecked();    
            auto value = fileItems->Get(i);
            auto obj = value.As<v8::Object>();       
 			auto prop_name = New<v8::String>("time").ToLocalChecked();
			obj->Set(prop_name, date);
 			prop_name = New<v8::String>("isExifDate").ToLocalChecked();
			obj->Set(prop_name, New<v8::Boolean>(true));
        } else if (item.version.major > 0 || item.version.minor > 0 || item.version.build > 0 || item.version.patch > 0 ) {
            auto value = fileItems->Get(i);
            auto obj = value.As<v8::Object>();       
 			auto prop_name = New<v8::String>("version").ToLocalChecked();

            v8::Local<v8::Object> version = New<v8::Object>();
            version->Set(New<v8::String>("major").ToLocalChecked(), New<v8::Number>(static_cast<double>(item.version.major)));
            version->Set(New<v8::String>("minor").ToLocalChecked(), New<v8::Number>(static_cast<double>(item.version.minor)));
            version->Set(New<v8::String>("build").ToLocalChecked(), New<v8::Number>(static_cast<double>(item.version.build)));
            version->Set(New<v8::String>("patch").ToLocalChecked(), New<v8::Number>(static_cast<double>(item.version.patch)));
			obj->Set(prop_name, version);
        }
    }
    resolver->Resolve(GetCurrentContext(), Undefined());
}