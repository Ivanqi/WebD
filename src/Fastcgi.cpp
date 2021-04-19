#include "src/Fastcgi.h"
#include "networker/net/Buffer.hEndian.h"
#include <stdio.h>

using namespace networker;
using namespace networker::net;
using namespace webd;
/**
 * 参考资料: 
 *  https://blog.csdn.net/hepangda/article/details/81560515
 *  https://www.infoq.cn/article/vIcwtItzVK7B4YNOEj3e
 * */
struct FastCgiCodec::RecordHeader
{
    uint8_t version;    // 表示 FastCGI 协议的版本号
    uint8_t type;       // 表示此记录的类型，预示着此记录的主要功能
    uint16_t id;        // 表示此记录属于哪个 FastCGI 请求
    uint16_t length;
    uint8_t padding;
    uint8_t unused;
};

const unsigned FastCgiCodec::kRecordHeader = static_cast<unsigned>(sizeof(FastCgiCodec::RecordHeader));

enum FcgiType
{
    kFcgiInvalid = 0,
    kFcgiBeginRequest = 1,  // (WEB->FastCGI) 表示一次请求的开始
    kFcgiAbortRequest = 2,  // (WEB->FastCGI) 表示终止一次请求
    kFcgiEndRequest = 3,    // (FastCGI->WEB) 请求已被处理完毕
    kFcgiParams = 4,        // (WEB->FastCGI) 表示一个向CGI程序传递的环境变量
    kFcgiStdin = 5,         // (WEB->FastCGI) 表示向CGI程序传递的标准输入
    kFcgiStdout = 6,        // (FastCGI->WEB) 表示CGI程序的标准输出  
    kFcgiStderr = 7,        // (FastCGI->WEB) 表示CGI程序的标准错误输出
    kFcgiData = 8,          // (WEB->FastCGI) 向CGI程序传递的额外数据
    kFcgiGetValues = 9,     // (WEB->FastCGI) 向FastCGI程序询问一些环境变量
    kFcgiGetValuesResult = 10,  // (FastCGI->WEB) 询问环境变量的结果
};

enum FcgiRole
{
    kFcgiResponder = 1,     // 响应器
    kFcgiAuthorizer = 2,    // 认证器
    kFcgiFilter = 3         // 过滤器
};

enum FcgiConstant
{
    kFcgiKeepConn = 1,  // 当flags与FLAGS_KEEP_CONN按位与结果为1时，则表示FastCGI服务器处理完本次请求之后不关闭连接，否则则关闭该Socket连接
};

bool FastCgiCodec::onParams(const char *content, uint16_t length)
{
    if (length > 0) {
        paramsStream_.append(content, length);
    } else if (!parseAllParams()){
        printf("FastCgiCodec::onParams parseAllParams() failed \n");
        return false;
    }
    return true;
}

void FastCgiCodec::onStdin(const char *content, uint16_t length)
{
    if (length > 0) {
        stdin_.append(content, length);
    } else {
        gotRequest_ = true;
    }
}

bool FastCgiCodec::parseAllParams()
{
    while (paramsStream_.readableBytes() > 0) {
        uint32_t nameLen = readLen();
        if (nameLen == static_cast<uint32_t>(-1)) {
            return false;
        }

        uint32_t valueLen = readLen();
        if (valueLen == static_cast<uint32_t>(-1)) {
            return false;
        }

        printf("nameLen:%d, valueLen:%d\n", nameLen, valueLen);

        if (paramsStream_.readableBytes() >= nameLen + valueLen) {
            std::string name = paramsStream_.retrieveAsString(nameLen);
            params_[name] = paramsStream_.retrieveAsString(valueLen);
        } else {
            return false;
        }
    }

    return true;
}

uint32_t FastCgiCodec::readLen()
{
    if (paramsStream_.readableBytes() >= 1) {
        uint8_t byte = paramsStream_.peekInt8();
        if (byte & 0x80) {
            if (paramsStream_.readableBytes() >= sizeof(uint32_t)) {
                return paramsStream_.readableBytes() & 0x7fffffff;
            } else {
                return -1;
            }
        } else {
            return paramsStream_.readInt8();
        }
    } else {
        return -1;
    }
}

void FastCgiCodec::endStdout(Buffer *buf)
{
    RecordHeader header = {
        1,
        kFcgiStdout,
        hostToNetwork16(1),
        0,
        0,
        0,
    };
    buf->append(&header, kRecordHeader);
}

void FastCgiCodec::endRequest(Buffer *buf)
{
    RecordHeader header = {
        1,
        kFcgiEndRequest,
        hostToNetwork16(1),
        hostToNetwork16(kRecordHeader),
        0,
        0,
    };
    buf->append(&header, kRecordHeader);
    buf->appendInt32(0);
    buf->appendInt32(0);
}

void FastCgiCodec::respond(Buffer *response)
{
    if (response->readableBytes() < 65536 && response->prependableBytes() >= kRecordHeader)
    {
        RecordHeader header = {
            1,
            kFcgiStdout,
            hostToNetwork16(1),
            hostToNetwork16(static_cast<uint16_t>(response->readableBytes())),
            static_cast<uint8_t>(-response->readableBytes() & 7),
            0,
        };
        response->prepend(&header, kRecordHeader);
        response->append("\0\0\0\0\0\0\0\0", header.padding);
    } else {
    // FIXME:
  }

  endStdout(response);
  endRequest(response);
}

bool FastCgiCodec::parseRequest(Buffer *buf)
{
    while (buf->readableBytes() >= kRecordHeader) {
        RecordHeader header;
        memcpy(&header, buf->peek(), kRecordHeader);
        header.id = networkToHost16(header.id);
        header.length = networkToHost16(header.length);

        printf("version = %d, type = %d\n", header.version, header.type);

        size_t total = kRecordHeader + header.length + header.padding;

        if (buf->readableBytes() >= total) {
            switch (header.type) {
                case kFcgiBeginRequest:
                    onBeginRequest(header, buf);
                    break;
                
                case kFcgiParams:
                    onParams(buf->peek() + kRecordHeader, header.length);
                    break;

                case kFcgiStdin:
                    onStdin(buf->peek() + kRecordHeader, header.length);
                    break;
                
                case kFcgiData:
                    break;

                case kFcgiGetValues:
                    break;

                default:
                    break;
            }
            buf->retrieve(total);
        } else {
            break;
        }
    }

    return true;
}

uint16_t readInt16(const void *p)
{
    uint16_t be16 = 0;
    ::memcpy(&be16, p, sizeof be16);
    return networkToHost16(be16);
}

bool FastCgiCodec::onBeginRequest(const RecordHeader& header, const Buffer *buf)
{
    assert(buf->readableBytes() >= header.length);
    assert(header.type == kFcgiBeginRequest);

    if (header.length >= kRecordHeader) {
        uint16_t role = readInt16(buf->peek()+kRecordHeader);
        uint8_t flags = buf->peek()[kRecordHeader + sizeof(int16_t)];

        if (role == kFcgiResponder) {
            keepConn_ = flags == kFcgiKeepConn;
            return true;
        }
    }

    return false;
}
