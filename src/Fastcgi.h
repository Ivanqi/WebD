#ifndef WEBD_FASTCGI_H
#define WEBD_FASTCGI_H

#include "networker/net/TcpConnection.h"
#include <map>
using namespace networker;
using namespace networker::net;

namespace webd
{
    class FastCgiCodec
    {
        public:
            typedef std::map<std::string, std::string> ParamMap;
            typedef std::function<void (const TcpConnectionPtr& conn, ParamMap&, Buffer*)> Callback;

        private:
            struct RecordHeader;
            Callback cb_;
            bool gotRequest_;
            bool keepConn_;
            Buffer stdin_;
            Buffer paramsStream_;
            ParamMap params_;
            const static unsigned kRecordHeader;
        
        public:
            explicit FastCgiCodec(const Callback& cb)
                :cb_(cb), gotRequest_(false), keepConn_(false)
            {

            }

            void onMessage(const TcpConnectionPtr& conn, Buffer *buf, Timestamp receiveTime)
            {
                parseRequest(buf);
                if (gotRequest_) {
                    cb_(conn, params_, &stdin_);
                    stdin_.retrieveAll();
                    paramsStream_.retrieveAll();
                    params_.clear();
                    gotRequest_ = false;
                    if (!keepConn_) {
                        conn->shutdown();
                    }
                }
            }

            static void respond(Buffer *esponse);
        
        private:
            bool parseRequest(Buffer *buf);
            bool onBeginRequest(const RecordHeader& header, const Buffer *buf);
            void onStdin(const char *content, uint16_t length);
            bool onParams(const char *content, uint16_t length);
            bool parseAllParams();
            uint32_t readLen();

            static void endStdout(Buffer *buf);
            static void endRequest(Buffer *buf);
    };
};


#endif