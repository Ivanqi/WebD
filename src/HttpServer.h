#ifndef WEBD_HTTPSERVER_H
#define WEBD_HTTPSERVER_H

#include "networker/net/TcpServer.h"
#include "networker/net/EventLoop.h"
#include "src/Fastcgi.h"

using std::string;
using namespace networker;
using namespace networker::net;

namespace webd
{
    class HttpRequest;
    class HttpResponse;

    class HttpServer
    {
        public:
            typedef std::function<void (const HttpRequest&, HttpResponse*)> HttpCallback;

        private:
            TcpServer server_;
            HttpCallback httpCallback_;
            string cgimode_{"cgi"};
            const string serverMode_;
        
        public:
            HttpServer(EventLoop* loop, const InetAddress& listenAddr, const string& name, const string& mode, TcpServer::Option option = TcpServer::kNoReusePort);

            EventLoop* getLoop() const
            {
                return server_.getLoop();
            }

            // 不是线程安全。调用start()之前注册回调
            void setHttpCallback(const HttpCallback& cb)
            {
                httpCallback_ = cb;
            }

            void setThreadNum(int numThreads)
            {
                server_.setThreadNum(numThreads);
            }

            void start();
        
        private:
            void onConnection(const TcpConnectionPtr& conn);

            void onMessage(const TcpConnectionPtr& conn, Buffer *buf, Timestamp receiveTime);

            void onRequest(const TcpConnectionPtr& , const HttpRequest&);

            void fastcigOnRequest(const TcpConnectionPtr& conn, FastCgiCodec::ParamMap& params, Buffer *in);
    };
};
#endif