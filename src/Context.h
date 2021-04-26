#ifndef WEBD_CONTEXT_H
#define WEBD_CONTEXT_H
#include "src/HttpRequest.h"
#include "src/StringUtil.h"
#include "networker/net/Buffer.h"

#include <string>
#include <algorithm>
#include <queue>
#include <sstream>

using namespace networker;
using namespace networker::net;

namespace webd
{
    class Context
    {
        private:
            HttpRequest *request_;
            std::string webkitChar{"WebKitForm"};
            bool webkitfromState_{false};
            std::string tmpKey{""};
            const char eqtag{'='};
            const char ampetag{'&'};
        
        public:
            Context(HttpRequest* request)
                :request_(request)
            {

            }

            std::string removeQuotationMarks(string str)
            {
                return StringUtil::Trim(StringUtil::Trim(str, '\"'), '\'');
            }

            bool processRequestBodyWithWebKit(const char* start, const char *crlf) 
            {
                const char *webkittag = std::search(start, crlf, webkitChar.begin(), webkitChar.end());
                bool webkitflag = (webkittag == crlf) ? false : true;
                if (webkitflag) {
                    webkitfromState_ = true;
                } else if (webkitfromState_){
                    string str(start, crlf);
                    size_t found = str.find('=');
                    if (found != std::string::npos) {
                        tmpKey = str.substr(found + 2, (str.size() - found - 3));
                    } else if (!str.empty()){
                        request_->setParamlist(removeQuotationMarks(tmpKey), removeQuotationMarks(str));
                        webkitfromState_ = false;
                    }
                }
                return true;
            }

            bool processRequestBodyWithCurl(string str, size_t eqtagfound, size_t ampetagfound) 
            {
                std::istringstream iss(str);
                string temp;
                std::queue<string> Q;
                if (ampetagfound == std::string::npos) {
                if (eqtagfound != std::string::npos) {
                        size_t found = str.find(eqtag);
                        if (found != std::string::npos) {
                            request_->setParamlist(removeQuotationMarks(str.substr(0, found)), removeQuotationMarks(str.substr(found + 1)));
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
                                request_->setParamlist(removeQuotationMarks(Qstr.substr(0, found)), removeQuotationMarks(Qstr.substr(found + 1)));
                            }
                        }
                    }
                }

                return true;
            }
    };
}

#endif