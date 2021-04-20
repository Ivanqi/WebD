#ifndef WEBD_STRING_UTIL_H
#define WEBD_STRING_UTIL_H

#include <string>
#include <algorithm>
#include <functional>
namespace webd
{
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

            template <class Uint16Container>
            bool static Utf8ToUnicode(const char* const str, size_t len, Uint16Container& vec)
            {
                if (!str) {
                    return false;
                }

                char ch1, ch2;
                uint16_t tmp;
                vec.clear();

                for (size_t i = 0; i < len;) {
                    if (!(str[i] & 0x80)) {     // 0xxxxxxx
                        vec.push_back((str[i] & 0x7f));
                        i++;
                    } else if ((uint8_t)str[i] <= 0xdf && i + 1 < len) {    // 110xxxxxx
                        ch1 = (str[i] >> 2) & 0x07;
                        ch2 = (str[i + 1] & 0x3f) | ((str[i] & 0x03) << 6);
                        tmp = (((uint16_t(ch1) & 0x00ff) << 8) | (uint16_t(ch2) & 0x00ff));
                        vec.push_back(tmp);
                        i += 2;
                    } else if ((uint8_t)str[i] <= 0xef && i + 2 < len) {
                        // 方法一
                        // ch1 = ((uint8_t)str[i] << 4) | ((str[i + 1] >> 2) & 0x0f );
                        // ch2 = (((uint8_t)str[i + 1] << 6) & 0xc0) | (str[i+2] & 0x3f);
                        // tmp = (((uint16_t(ch1) & 0x00ff ) << 8) | (uint16_t(ch2) & 0x00ff));
                        
                        // 方法二
                        tmp = (uint8_t)(str[i]) & 0x0f;

                        tmp <<= 6;
                        tmp |= (uint8_t)(str[i + 1]) & 0x3f;

                        tmp <<= 6;
                        tmp |= (uint8_t)(str[i + 2]) & 0x3f;

                        vec.push_back(tmp);
                        i += 3;
                    } else {
                        return false;
                    }
                }

                return true;
            }

            template <class Uint16Container>
            bool static utf8ToUnicode(const std::string& str, Uint16Container& vec)
            {
                return Utf8ToUnicode(str.c_str(), str.size(), vec);
            }

            template <class Uint16ContainerConIter>
            void static unicodeToUtf8(Uint16ContainerConIter begin, Uint16ContainerConIter end, std::string& res)
            {
                res.clear();
                uint16_t ui;

                while (begin != end) {
                    ui = *begin;

                    if (ui <= 0x7f) {
                        res += char(ui);
                    } else if (ui <= 0x7ff) {
                        res += char(((ui >> 6) & 0x1f) | 0xc0);
                        res += char((ui & 0x3f) | 0x80);
                    } else {
                        res += char(((ui >> 12) & 0x0f) | 0xe0);
                        res += char(((ui >> 6) & 0x3f) | 0x80);
                        res += char((ui & 0x3f) | 0x80);
                    }

                    begin++;
                }
            }
    };
};
#endif