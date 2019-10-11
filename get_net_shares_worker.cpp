#define NAPI_EXPERIMENTAL
#include <napi.h>
#include <vector>
#include "wstring.h"
#include "nullfunction.h"
#include "get_net_shares_worker.h"
#if WINDOWS
#include "windows/utils.h"
#include <lm.h>
#elif LINUX
#endif
using namespace Napi;
using namespace std;

enum class Share_type {
	DISKTREE,
	PRINTQ,
	DEVICE,
	IPC
	//MASK = 0x000000FF,              // AND with shi_type to
	//RESERVED1 = 0x01000000,              // Reserved for internal processing
	//RESERVED2 = 0x02000000,
	//RESERVED3 = 0x04000000,
	//RESERVED4 = 0x08000000,
	//RESERVED5 = 0x00100000,
	//RESERVED_ALL = 0x3FFFFF00,
	//TEMPORARY = 0x40000000,
	//SPECIAL = 0x80000000,
	//USES_UNLIMITED = (DWORD)-1
};

struct Share
{
	wstring name;
	wstring description;
	Share_type type;

	Share(const wstring& name, const wstring& description, Share_type type)
		: name(name)
		, description(description)
		, type(type) {}

};

vector<Share> get_net_shares(const wstring& server) {
   	auto buffer = (LPBYTE)nullptr;
	auto entries = (DWORD)0;
	auto total_entries = (DWORD)0;
	auto ret = NetShareEnum(const_cast<LPWSTR>(server.c_str()), 1, &buffer, MAX_PREFERRED_LENGTH,
		&entries, &total_entries, nullptr);

	auto info = (SHARE_INFO_1*)buffer;

	vector<Share> result;
	for (auto i = 0u; i < entries; i++) {
		result.emplace_back(info[i].shi1_netname, info[i].shi1_remark, (Share_type)(info[i].shi1_type & 0x000000FF));
	}

	NetApiBufferFree(buffer);

    return result;
}

class Get_net_shares_worker : public AsyncWorker {
public:
    Get_net_shares_worker(const Napi::Env& env, const wstring& server)
    : AsyncWorker(Function::New(env, NullFunction, "theFunction"))
    , server(server)
    , deferred(Promise::Deferred::New(Env())) {}
    ~Get_net_shares_worker() {}

    void Execute () { net_shares = move(get_net_shares(server)); }

    void OnOK();

    Promise Promise() { return deferred.Promise(); }

private:
    Promise::Deferred deferred;
    wstring server;
    vector<Share> net_shares;
};

void Get_net_shares_worker::OnOK() {
    HandleScope scope(Env());

    auto array = Array::New(Env(), net_shares.size());
    int i{0};
    for(auto item: net_shares) {
        auto obj = Object::New(Env());

        obj.Set("name", WString::New(Env(), item.name));
        obj.Set("description", WString::New(Env(), item.description));
        obj.Set("type", Number::New(Env(), static_cast<double>(item.type)));

        array.Set(i++, obj);
    }

    deferred.Resolve(array);
}

Value GetNetShares(const CallbackInfo& info) {
    auto server = info[0].As<WString>().WValue();

    auto worker = new Get_net_shares_worker(info.Env(), server);
    worker->Queue();
    return worker->Promise();
}
