#include "src/HttpServer.h"
#include "src/HttpContext.h"
#include "src/HttpRequest.h"
#include "src/HttpResponse.h"
#include <any>
#include <iostream>

using namespace webd;
void defaultHttpCallback(const HttpRequest&, HttpResponse* resp)
{
    resp->setStatusCode(HttpResponse::k404NotFound);
    resp->setStatusMessage("Not Found");
    resp->setCloseConnection(true);
}

HttpServer::HttpServer(EventLoop* loop, const InetAddress& listenAddr, const string& name, TcpServer::Option option)
    :server_(loop, listenAddr, name, option), httpCallback_(defaultHttpCallback) 
{
    server_.setConnectionCallback(std::bind(&HttpServer::onConnection, this, _1));

    server_.setMessageCallback(std::bind(&HttpServer::onMessage, this, _1, _2, _3));
}

void HttpServer::start()
{
    printf("HttpServer[%s] starts listenning on %s\n", server_.name().c_str(), server_.ipPort().c_str());
    server_.start();
}

void HttpServer::onConnection(const TcpConnectionPtr& conn)
{
    if (conn->connected()) {
        conn->setContext(HttpContext());
    }
}

void HttpServer::onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTime)
{
    HttpContext *context = std::any_cast<HttpContext>(conn->getMutableContext());

    if (!context->parseRequest(buf, receiveTime)) {
        conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
        conn->shutdown();
    }

    if (context->gotAll()) {
        onRequest(conn, context->request());
        context->reset();
    }
}

void HttpServer::onRequest(const TcpConnectionPtr& conn, const HttpRequest& req)
{
    const string& connection = req.getHeader("Connection");
    bool close = connection == "close" || (req.getVersion() == HttpRequest::kHttp10 && connection != "Keep-Alive");

    HttpResponse response(close);
    httpCallback_(req, &response);

    Buffer buf;
    response.appendToBuffer(&buf);
    conn->send(&buf);

    if (response.closeConnection()) {
        conn->shutdown();
    }
}