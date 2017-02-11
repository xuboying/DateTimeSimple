#include "DateTimeSimple.h"
#include "stdafx.h"
#include <iostream>
#include <codecvt>
#include <locale>
#include <clocale>
#if defined(_MSC_VER)
#include <io.h>
#include <fcntl.h>
#endif

std::string ws2s(const std::wstring & wstr) {
    typedef std::codecvt_utf8<wchar_t> convert_typeX;
    std::wstring_convert<convert_typeX, wchar_t> converterX;
    return converterX.to_bytes(wstr);
}

int _tmain(int argc, _TCHAR * argv[]) {
    DateTimeSimple t;
    t.SetStrptimePattern(L"%FT%T%z");
    t.strptime(L"2017-01-09T00:23:00+0800");
    std::cout << t.GetEpoch() << std::endl;
    std::cout << ws2s(t.m_strptime_regex) << std::endl;
    return 0;
}
