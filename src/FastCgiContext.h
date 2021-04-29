#ifndef WEBD_FASTCGI_CONTEXT_H
#define WEBD_FASTCGI_CONTEXT_H
#include "src/HttpRequest.h"
#include "src/Context.h"
#include <string>
#include <iostream>

namespace webd
{
    class FastCgiContext
    {
        private:
            HttpRequest *request_;
            Context cxt_;
            const char eqtag{'='};
            const char ampetag{'&'};

        public:

            FastCgiContext(HttpRequest* req)
                :request_(req), cxt_(Context(request_))
            {
            }

            ~FastCgiContext()
            {

            }

            void setQueryString(std::string queryStr)
            {
                size_t eqtagfound = queryStr.find(eqtag);
                size_t ampetagfound = queryStr.find(ampetag);
    
                if ((eqtagfound != std::string::npos) || (ampetagfound != std::string::npos)) {
                    cxt_.processRequestBodyWithCurl(queryStr, eqtagfound, ampetagfound);
                }
            }

            bool setMethod(const string& methodStr)
            {
                return request_->setMethod(methodStr);
            }

            void setPath(const string& pathStr)
            {
                request_->setPath(pathStr);
            }

            void setQuery(const string& queryStr)
            {
                request_->setQuery(queryStr);
                setQueryString(queryStr);
            }

            void setVersion(string& version)
            {
                size_t found = version.find('.');
                if (found != std::string::npos) {
                    string m = version.substr(found + 1);
                    if (m[0] == '1') {
                        request_->setVersion(HttpRequest::kHttp11);
                    }  else if (m[0] == '0') {
                        request_->setVersion(HttpRequest::kHttp10);
                    }
                }
            }

            void headerSwap(std::map<std::string,std::string> other)
            {
                request_->headerSwap(other);
            }

            void setRequestBody(const std::string& bodyStr)
            {
                if (bodyStr.find(cxt_.webkitChar_) != std::string::npos) {
                    std::vector<string> reqBody;
                    StringUtil::Split(bodyStr, reqBody, "\n");
       
                    for (size_t i = 0; i < reqBody.size(); i++) {
                        cxt_.processRequestBodyWithWebKit(reqBody[i]);
                    }
                } else {
                    setQueryString(bodyStr);
                }
            }
    };
};


#endif