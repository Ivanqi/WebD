#ifndef WEBD_HTTPCONTEXT_H
#define WEBD_HTTPCONTEXT_H

#include "src/HttpRequest.h"
#include "src/Context.h"
#include "networker/net/Buffer.h"
using namespace networker;
using namespace networker::net;

namespace webd
{
    class HttpContext
    {
        public:
            enum HttpRequestParseState
            {
                kExpectRequestLine, // 请求行
                kExpectHeaders, // 请求头
                kExpectBody,    // 请求体
                kGotAll // 已经获取所有信息
            };

        private:
            HttpRequestParseState state_;
            HttpRequest *request_;
            const char eqtag{'='};
            const char ampetag{'&'};
            Context cxt_;
        
        public:
            HttpContext()
                :state_(kExpectRequestLine), cxt_(Context(request_)) 
            {
            }

            bool parseRequest(Buffer *buf, Timestamp receiveTime);

            bool gotAll() const
            {
                return state_ == kGotAll;
            }

            // 复位
            void reset()
            {
                state_ = kExpectRequestLine;
                HttpRequest dummy;
                request_->swap(dummy);
            }

            const HttpRequest& request() const
            {
                return *request_;
            }

            HttpRequest& request()
            {
                return *request_;
            }
        
        private:

            void setQueryParams(string queryStr);

            bool processRequestLine(const char* begin, const char* end);
    };
};

#endif