#ifndef WEBD_HTTPREQUEST_H
#define WEBD_HTTPREQUEST_H
#include "networker/base/Timestamp.h"
#include "networker/base/Types.h"

#include <map>
#include <stdio.h>
#include <assert.h>

using std::string;
using namespace networker;
namespace webd
{
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
            std::map<string, string> headers_;
            std::map<string , string> paramlist_;
        
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
                std::map<string, string>::const_iterator it = headers_.find(field);
                if (it != headers_.end()) {
                    result = it->second;
                }
                return result;
            }

            const std::map<string, string>& headers() const
            {
                return headers_;
            }

            void setParamlist(string key, string val) 
            {
                paramlist_[key] = val;
            }

            const std::map<string, string>& paramlist() const
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