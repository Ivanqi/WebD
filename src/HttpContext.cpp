#include "src/HttpContext.h"
#include <queue>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <sstream>
using namespace webd;

bool HttpContext::processRequestLine(const char *begin, const char *end)
{
    bool succeed = false;
    const char *start = begin;
    const char *space = std::find(start, end, ' '); // 寻找第一个请求行的空格，也就是请求方法
    if (space != end && request_.setMethod(start, space)) {
        start = space + 1;                          // 地址向前增加
        space = std::find(start, end, ' ');         // 得到请求的连接

        if (space != end) {
            const char *question = std::find(start, space, '?');    // 在请求的连接中查询连接的参数
            if (question != space) {
                request_.setPath(start, question);  // 连接地址
                request_.setQuery(question, space); // 连接参数
            } else {
                request_.setPath(start, space); // 连接地址
            }

            start = space + 1;  // 地址向前增加
            succeed = end - start == 8 && std::equal(start, end - 1, "HTTP/1.");    // 请求协议

            if (succeed) {
                if (*(end - 1) == '1') {
                    request_.setVersion(HttpRequest::kHttp11);  // http1.1协议
                } else if (*(end - 1) == '0') {
                    request_.setVersion(HttpRequest::kHttp10);  // http1.0协议
                } else {
                    succeed = false;
                }
            }
        }
    }
    return succeed;
}

bool HttpContext::processRequestBodyWithWebKit(Buffer *buf, const char *crlf) 
{
    const char *webkittag = std::search(buf->peek(), crlf, webkitChar.begin(), webkitChar.end());
    bool webkitflag = (webkittag == crlf) ? false : true;
    if (webkitflag) {
        webkitfromState_ = true;
    } else if (webkitfromState_){
        string str(buf->peek(), crlf);
        size_t found = str.find('=');
        if (found != std::string::npos) {
            tmpKey = str.substr(found + 2, (str.size() - found - 3));
        } else if (!str.empty()){
            request_.setParamlist(tmpKey, str);
            webkitfromState_ = false;
        }
    }
    return true;
}

bool HttpContext::processRequestBodyWithCurl(string str, size_t eqtagfound, size_t ampetagfound) 
{
    std::istringstream iss(str);
    string temp;
    std::queue<string> Q;
    if (ampetagfound == std::string::npos) {
       if (eqtagfound != std::string::npos) {
            size_t found = str.find(eqtag);
            if (found != std::string::npos) {
                request_.setParamlist(str.substr(0, found), str.substr(found + 1));
            }
       } 
    } else {
        if (eqtagfound != std::string::npos) {
            while (getline(iss, temp, ampetag)) {
                Q.push(temp);
            }

            while (!Q.empty()) {
                string Qstr = Q.front();
                Q.pop();

                size_t found = Qstr.find(eqtag);
                if (found != std::string::npos) {
                    request_.setParamlist(Qstr.substr(0, found), Qstr.substr(found + 1));
                }
            }
        }
    }

    return true;
}

// reutrn false if any error
bool HttpContext::parseRequest(Buffer *buf, Timestamp receiveTime)
{
    bool ok = true;
    bool hasMore = true;
    while (hasMore) {
        if (state_ == kExpectRequestLine) {
            const char *crlf = buf->findCRLF(); // 找\r\n
            if (crlf) {
                ok = processRequestLine(buf->peek(), crlf); // 请求行解析
                if (ok) {
                    request_.setReceiveTime(receiveTime);
                    buf->retrieveUntil(crlf + 2);
                    state_ = kExpectHeaders;
                } else {
                    hasMore = false;
                }
            } else {
                hasMore = false;
            }
        } else if (state_ == kExpectHeaders) {
            const char *crlf = buf->findCRLF(); // 找\r\n. 意味这个每个信息头的结束
            if (crlf) {
                const char *colon = std::find(buf->peek(), crlf, ':');
                if (colon != crlf) {
                    request_.addHeader(buf->peek(), colon, crlf);
                } else {
                    // buf->retrieveUntil(crlf + 2);
                    state_ = kExpectBody;
                    // hasMore = false;
                }
                buf->retrieveUntil(crlf + 2);
            } else {
                hasMore = false;
            }
        } else if (state_ == kExpectBody) {
            const char *crlf = buf->findCRLF(); // 找\r\n
            if (crlf) {
                processRequestBodyWithWebKit(buf, crlf);
                buf->retrieveUntil(crlf + 2);
            } else {    // 无\r\n
                string str = buf->retrieveAllAsString();
                size_t eqtagfound = str.find(eqtag);
                size_t ampetagfound = str.find(ampetag);
                
                if ((eqtagfound != std::string::npos) || (ampetagfound != std::string::npos)) {
                    processRequestBodyWithCurl(str, eqtagfound, ampetagfound);
                }
                hasMore = false;
            }
        }
    }

    return ok;
}