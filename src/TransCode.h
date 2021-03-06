#ifndef WEBD_TRANSCODE_H
#define WEBD_TRANSCODE_H

#include <vector>
#include "StringUtil.h"

namespace webd
{
    typedef uint16_t UnicodeValueType;
    typedef std::vector<UnicodeValueType> Unicode;

    namespace TransCode
    {
        inline Unicode decode(const std::string& str)
        {
            Unicode ret;
            StringUtil::utf8ToUnicode(str, ret);
            return ret;
        }

        inline void encode(Unicode::const_iterator begin, Unicode::const_iterator end, std::string& res) 
        {
            StringUtil::unicodeToUtf8(begin, end, res);
        }
    };
};
#endif