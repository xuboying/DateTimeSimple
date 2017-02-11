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

#ifndef DATETIMESIMPLE_H
#define DATETIMESIMPLE_H
#include <string>
#include <ctime>
#include <vector>
#include "../PCRE2Plus/src/PCRE2Plus.h"
using namespace PCRE2Plus;
#ifdef _MSC_VER
#define timegm _mkgmtime
#endif
class DateTimeSimple {
    //==========================================================================
    private:
    const int NO_TZ = 65535;
    std::wstring m_strptime_pattern;
    int          m_tz_offset_minute;
    std::vector<std::tuple<re::RW_PT, std::wstring, bool>> & m_trans();
    //==========================================================================
    public:
    std::wstring m_strptime_regex;
    std::tm      m_t;
    bool SetStrptimePattern(const std::wstring & p);
    bool strptime(const std::wstring & t);
    std::time_t GetEpoch();
    //--------------------------------------------------------------------------
    DateTimeSimple() : m_t({0}), m_tz_offset_minute(0) {
    }
};

#endif