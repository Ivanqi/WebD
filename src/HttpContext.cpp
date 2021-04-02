#include "src/HttpContext.h"
using namespace webd;

bool HttpContext::processRequestLine(const char *begin, const char *end)
{
    bool succeed = false;
    const char *start = begin;
    const char *space = std::find(start, end, ' '); // 寻找第一个请求行的空格，也就是请求方法

    if (space != end && request_.setMethod(start, space)) {
        start = space + 1;  // 地址向前增加
        space = std::find(start, end, ' '); // 得到请求的连接

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
            const char *crlf = buf->findCRLF(); // 找\r\n
            if (crlf) {
                const char *colon = std::find(buf->peek(), crlf, ':');
                if (colon != crlf) {
                    request_.addHeader(buf->peek(), colon, crlf);
                } else {
                    state_ = kGotAll;
                    hasMore = false;
                }
                buf->retrieveUntil(crlf + 2);
            } else {
                hasMore = false;
            }
        } else if (state_ == kExpectBody) {

        }
    }

    return ok;
}