#include "std_utils.h"
#include <algorithm>
#include <string>
#include <cctype>
using namespace std;

const vector<wstring> split(const wstring& s, wchar_t delim) {
	wstringstream wss(s);
	wstring item;
	vector<wstring> elems;
	while (getline(wss, item, delim))
		elems.push_back(move(item));

	return move(elems);
}

wstring join(const vector<wstring>& arr, wchar_t delim) {
    wstring result;
    for (auto it = arr.begin(); it < arr.end(); it++) {
        if (!result.empty())
            result += delim;
        result.append(*it);
    }
    return move(result);
}

int findString(wstring const& text, wstring const& searchText) {
    auto it = search(text.begin(), text.end(),
        searchText.begin(), searchText.end(),
        [](auto ch1, auto ch2) { return toupper(ch1) == toupper(ch2); }
    );
	auto pos = static_cast<int>(it - text.begin());
	return pos < text.length() ? pos : -1;
}

bool ends_with(wstring const& value, wstring const& ending)
{
    if (ending.size() > value.size())
        return false;
    return equal(ending.rbegin(), ending.rend(), value.rbegin(), [](wchar_t a, wchar_t b){ return tolower(a) == tolower(b); });
}

