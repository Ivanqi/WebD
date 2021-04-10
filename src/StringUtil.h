#ifndef WEBD_STRING_UTIL_H
#define WEBD_STRING_UTIL_H

#include <string>
#include <algorithm>
#include <functional>

class StringUtil
{
    public:

        inline std::string& LTrim(std::string &s, char x)
        {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::bind2nd(std::equal_to<char>(), x))));
            return s;
        }

        inline std::string& RTrim(std::string &s, char x)
        {
            s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::bind2nd(std::equal_to<char>(), x))).base(), s.end());
            return s;
        }

        inline std::string& Trim(std::string &s, char x)
        {
            return LTrim(RTrim(s, x), x);
        }
};

#endif