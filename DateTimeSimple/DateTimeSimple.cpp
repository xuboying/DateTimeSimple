/*
BSD 3 - Clause License

Copyright(c) 2017, Boying Xu
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met :

*Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and / or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "DateTimeSimple.h"
#include <tuple>
//==============================================================================
std::vector<std::tuple<re::RW_PT, std::wstring, bool>> & DateTimeSimple::m_trans() {
    static std::vector<std::tuple<re::RW_PT, std::wstring, bool>> tmp;

    //formats http://www.cplusplus.com/reference/ctime/strftime/
    //%a
    //%A
    //%b
    //%B
    //%c
    tmp.push_back(std::make_tuple(re::compile(LR"((?<!%)%C)"), L"(?P<year0>([0-9]{2}))", false));
    tmp.push_back(std::make_tuple(re::compile(LR"((?<!%)%d)"), L"(?P<day>(0[1-9]|[12][0-9]|3[01]))", false));
    tmp.push_back(std::make_tuple(re::compile(LR"((?<!%)%D)"), L"%m/%d/%y", true));
    tmp.push_back(std::make_tuple(re::compile(LR"((?<!%)%e)"), L"(?P<day>( [1-9]|[12][0-9]|3[01]))", false));
    tmp.push_back(std::make_tuple(re::compile(LR"((?<!%)%F)"), L"%Y-%m-%d", true));
    //%g
    //%G
    //%h
    tmp.push_back(std::make_tuple(re::compile(LR"((?<!%)%H)"), L"(?P<hour>(0[0-9]|1[0-9]|2[0-3]))", false));
    tmp.push_back(std::make_tuple(re::compile(LR"((?<!%)%I)"), L"(?P<hour0>(0[0-9]|1[0-2]))", false));
    //%j
    tmp.push_back(std::make_tuple(re::compile(LR"((?<!%)%m)"), L"(?P<month>(0[0-9]|1[0-2]))", false));
    tmp.push_back(std::make_tuple(re::compile(LR"((?<!%)%M)"), L"(?P<minute>(0[0-9]|[1-4][0-9]|5[0-9]))", false));
    tmp.push_back(std::make_tuple(re::compile(LR"((?<!%)%n)"), L"(\\r)", false));
    tmp.push_back(std::make_tuple(re::compile(LR"((?<!%)%p)"), L"(?i)(?P<apm>(am|pm))(?-i)", false));
    tmp.push_back(std::make_tuple(re::compile(LR"((?<!%)%r)"), L"%I:%M:%S %p", true));
    tmp.push_back(std::make_tuple(re::compile(LR"((?<!%)%R)"), L"%H:%M", true));
    tmp.push_back(std::make_tuple(re::compile(LR"((?<!%)%S)"), L"(?P<second>(0[0-9]|[1-5][0-9]|6[01]))", false));
    tmp.push_back(std::make_tuple(re::compile(LR"((?<!%)%t)"), L"(\\t)", false));
    tmp.push_back(std::make_tuple(re::compile(LR"((?<!%)%T)"), L"%H:%M:%S", true));
    tmp.push_back(std::make_tuple(re::compile(LR"((?<!%)%u)"), L"(?P<weekday>([1-7]))", false));
    //%U
    //%V
    //%w
    //%W
    //%x
    //%X
    tmp.push_back(std::make_tuple(re::compile(LR"((?<!%)%y)"), L"(?P<year0>([0-9]{2}))", false));
    tmp.push_back(std::make_tuple(re::compile(LR"((?<!%)%Y)"), L"(?P<year>([0-9]{4}))", false));
    tmp.push_back(std::make_tuple(re::compile(LR"((?<!%)%z)"), L"(?P<tz>([+-](0[0-9]|1[0-2])(0[0-9]|[1-4][0-9]|5[0-9])))", false));
    return tmp;
}
//==============================================================================
std::time_t DateTimeSimple::GetEpoch() {
    std::time_t x = timegm(&m_t);
    if (m_tz_offset_minute != NO_TZ) {
        x -= m_tz_offset_minute * 60;
    }
    return x;
}
//==============================================================================
bool DateTimeSimple::SetStrptimePattern(const std::wstring & p) {
    m_strptime_regex = p;
AGAIN:
    for (const auto & c : m_trans()) {
        if (std::get<0>(c)->search(m_strptime_regex)) {
            m_strptime_regex = std::get<0>(c)->sub(std::get<1>(c), m_strptime_regex);
            if (std::get<2>(c)) {
                goto AGAIN;
            }
        }
    }
    m_strptime_regex = L"^" + m_strptime_regex + L"$";
    return true;
}
//==============================================================================
bool DateTimeSimple::strptime(const std::wstring & t) {
    m_tz_offset_minute = NO_TZ;
    auto M = re::search(m_strptime_regex, t);
    if (M) {
        std::wstring tmp;
        tmp = M->group(L"year");
        if (tmp != L"") {
            m_t.tm_year = std::stol(tmp) - 1900;
        }
        else if (M->group(L"year0") != L"") {
            m_t.tm_year = std::stol(M->group(L"year0")) + 2000 - 1900;
        }

        tmp = M->group(L"month");
        if (tmp != L"") {
            m_t.tm_mon = std::stol(tmp) - 1;
        }

        tmp = M->group(L"day");
        if (tmp != L"") {
            m_t.tm_mday = std::stol(tmp);
        }

        tmp = M->group(L"hour");
        if (tmp != L"") {
            m_t.tm_hour = std::stol(tmp);
        }
        else if (M->group(L"hour0") != L"") {
            m_t.tm_hour = std::stol(M->group(L"hour0"));
            if (M->group(L"apn") != L"") {
                std::wstring apn = M->group(L"apn");
                if (re::search(L"pm", apn, re::I)) {
                    if (m_t.tm_hour > 0) {
                        m_t.tm_hour += 12;
                    }
                }
                if (re::search(L"am", apn, re::I)) {
                    if (m_t.tm_hour == 12) {
                        m_t.tm_hour = 0;
                    }
                }
            }
        }

        tmp = M->group(L"minute");
        if (tmp != L"") {
            m_t.tm_min = std::stol(tmp);
        }
        tmp = M->group(L"second");
        if (tmp != L"") {
            m_t.tm_sec = std::stol(tmp);
        }
        
        tmp = M->group(L"tz");
        if (tmp != L"") {
            int x = std::stol(tmp);
            int hour = x / 100;
            int minute = labs(x % 100);
            m_tz_offset_minute = hour * 60 + minute;
        }
    }
    return true;
}
