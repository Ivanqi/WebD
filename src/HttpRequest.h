#ifndef WEBD_HTTPREQUEST_H
#define WEBD_HTTPREQUEST_H
#include "networker/base/Timestamp.h"
#include "networker/base/Types.h"
#include "networker/base/Logging.h"

#include <unordered_map>
#include <stdio.h>
#include <assert.h>

using std::string;
using namespace networker;
namespace webd
{
    // 解决传输中文问题的编码
    static const char safe[] = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,
        0,0,0,0,0,0,0,1,0,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,0,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0
    };

    class HttpRequest
    {
        public:
            enum Method
            {
                kInvalid,   // 无效请求
                kGet,       // GET请求
                kPost,      // POST请求
                kHead,      // Header请求
                kPut,       // PUT请求
                kDelete     // DELETE请求
            };

            enum Version
            {
                kUnknown, kHttp10, kHttp11
            };
        
        private:
            Method method_;
            Version version_;
            string path_;
            string query_;
            Timestamp receiveTime_;
            std::unordered_map<string, string> headers_;
            std::unordered_map<string , string> paramlist_;

        
        public:
            HttpRequest()
                :method_(kInvalid), version_(kUnknown)
            {
                
            }

            void setVersion(Version v)
            {
                version_ = v;
            }

            Version getVersion() const
            {
                return version_;
            }

            bool setMethod(const char* start, const char* end)
            {
                assert(method_ == kInvalid);
                string m(start, end);

                if (m == "GET") {
                    method_ = kGet;
                } else if (m == "POST") {
                    method_ = kPost;
                } else if (m == "HEAD") {
                    method_ = kHead;
                } else if (m == "PUT") {
                    method_ = kPut;
                } else if (m == "DELETE") {
                    method_ = kDelete;
                } else {
                    method_ = kInvalid;
                }
                return method_ != kInvalid;
            }

            Method method() const
            {
                return method_;
            }

            const char* methodString() const
            {
                const char* result = "UNKNOWN";
                switch (method_) {
                    case kGet:
                        result = "GET";
                        break;
                    case kPost:
                        result = "POST";
                        break;
                    case kHead:
                        result = "HEAD";
                        break;
                    case kPut:
                        result = "PUT";
                        break;
                    case kDelete:
                        result = "DELETE";
                    default:
                        break;
                }
                return result;
            }

            void setPath(const char* start, const char* end)
            {
                path_.assign(start, end);
            }

            const string& path() const
            {
                return path_;
            }

            void setQuery(const char* start, const char* end)
            {
                query_.assign(start, end);
            }

            const string& query() const
            {
                return query_;
            }

            void setReceiveTime(Timestamp t)
            {
                receiveTime_ = t;
            }

            Timestamp receiveTime() const
            {
                return receiveTime_;
            }

            void addHeader(const char* start, const char* colon, const char* end)
            {
                string field(start, colon);
                ++colon;

                while (colon < end && isspace(*colon)) {
                    ++colon;
                }

                string value(colon, end);
                while (!value.empty() && isspace(value[value.size() - 1])) {
                    value.resize(value.size() - 1);
                }
                headers_[field] = value;
            }

            string getHeader(const string& field) const
            {
                string result;
                std::unordered_map<string, string>::const_iterator it = headers_.find(field);
                if (it != headers_.end()) {
                    result = it->second;
                }
                return result;
            }

            const std::unordered_map<string, string>& headers() const
            {
                return headers_;
            }
            
            std::string urlEncode(const std::string &uri)
            {
                string ret;
                const unsigned char *ptr = (const unsigned char *)uri.c_str();
                ret.reserve(uri.length());

                for (; *ptr ; ++ptr) {
                    if (!safe[*ptr]) {
                        char buf[5];
                        memset(buf, 0, 5);
                        snprintf(buf, 5, "%%%X", (*ptr)); 
                        ret.append(buf);
                    } else if (*ptr == ' ') {
                        ret += '+'; 
                    } else {
                        ret += *ptr;
                    }
                }
                return ret;
            }

            std::string urlDecode(const std::string &uri)
            {
                const unsigned char *ptr = (const unsigned char *)uri.c_str();
                string ret;
                ret.reserve(uri.length());  

                for (; *ptr; ++ptr) {
                    if (*ptr == '%') {
                        if (*(ptr + 1)) {
                            char a = *(ptr + 1);
                            char b = *(ptr + 2);
                            if (!((a >= 0x30 && a < 0x40) || (a >= 0x41 && a < 0x47))) continue;
                            if (!((b >= 0x30 && b < 0x40) || (b >= 0x41 && b < 0x47))) continue;
                            char buf[3];
                            buf[0] = a;
                            buf[1] = b;
                            buf[2] = 0;
                            ret += (char)strtoul(buf, NULL, 16);
                            ptr += 2;
                            continue;
                        }
                    }

                    if (*ptr == '+') {  
                        ret += ' ';
                        continue;  
                    }  
                    ret += *ptr;  
                }
                return ret; 
            }

            void setParamlist(string key, string val) 
            {
                
                paramlist_[key] = urlDecode(val);
            }

            const std::unordered_map<string, string>& paramlist() const
            {
                return paramlist_;
            }

            void swap(HttpRequest& that)
            {
                std::swap(method_, that.method_);
                std::swap(version_, that.version_);

                path_.swap(that.path_);
                query_.swap(that.query_);
                receiveTime_.swap(that.receiveTime_);
                headers_.swap(that.headers_);
            }
    };
};

#endif