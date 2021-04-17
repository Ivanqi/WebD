#ifndef WEBD_TRANSCODE_H
#define WEBD_TRANSCODE_H

// #include "LocalVector.h"
#include <vector>
#include "StringUtil.h"
using std::vector;

typedef uint16_t UnicodeValueType;
typedef vector<UnicodeValueType> Unicode;

namespace TransCode
{
    inline Unicode decode(const std::string& str)
    {
        Unicode ret;
        utf8ToUnicode(str, ret);
        return ret;
    }

    inline void encode(Unicode::const_iterator begin, Unicode::const_iterator end, std::string& res) 
    {
        unicodeToUtf8(begin, end, res);
    }
};

#endif