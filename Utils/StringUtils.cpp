#include "StringUtils.h"

#include <codecvt>

#include "Assert.h"

namespace Utils {
	void ToWideString(const std::string& source, std::wstring& dest) {
		dest.assign(source.begin(), source.end());
	}

	std::wstring ToWideString(const std::string& source) {
		std::wstring dest;
		dest.assign(source.begin(), source.end());
		return dest;
	}

	std::string ToString(const std::wstring& source) {
		//setup converter
		typedef std::codecvt_utf8<wchar_t> convert_type;
		std::wstring_convert<convert_type, wchar_t> converter;
		//use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
		return converter.to_bytes(source);
	}
}