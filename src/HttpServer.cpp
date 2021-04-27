#include "src/HttpServer.h"
#include "src/HttpContext.h"
#include "src/HttpRequest.h"
#include "src/HttpResponse.h"
#include "src/FastCgiContext.h"
#include "networker/base/Logging.h"
#include <any>
#include <iostream>
using namespace std::placeholders;
using namespace webd;

void defaultHttpCallback(const HttpRequest&, HttpResponse* resp)
{
    resp->setStatusCode(HttpResponse::k404NotFound);
    resp->setStatusMessage("Not Found");
    resp->setCloseConnection(true);
}

HttpServer::HttpServer(EventLoop* loop, const InetAddress& listenAddr, const string& name, const string& mode, TcpServer::Option option)
    :server_(loop, listenAddr, name, option), httpCallback_(defaultHttpCallback) , serverMode_(mode)
{
    server_.setConnectionCallback(std::bind(&HttpServer::onConnection, this, _1));

    server_.setMessageCallback(std::bind(&HttpServer::onMessage, this, _1, _2, _3));
}

void HttpServer::start()
{
    LOG_INFO << "HttpServer[" << server_.name() << "] starts listenning on " << server_.ipPort();
    server_.start();
}

void HttpServer::onConnection(const TcpConnectionPtr& conn)
{
    if (conn->connected()) {
        if (serverMode_ == cgimode_) {
            typedef std::shared_ptr<FastCgiCodec> CodecPtr;
		    CodecPtr codec(new FastCgiCodec(std::bind(&HttpServer::fastcigOnRequest, this, _1, _2, _3)));

            conn->setContext(codec);
            conn->setMessageCallback(std::bind(&FastCgiCodec::onMessage, codec, _1, _2, _3));
            conn->setTcpNoDelay(true);
        } else {
            conn->setContext(HttpContext());
        }
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

void HttpServer::fastcigOnRequest(const TcpConnectionPtr& conn, FastCgiCodec::ParamMap& params, Buffer *in)
{
	string uri = params["REQUEST_URI"];

    HttpRequest req;
	Buffer response;
    FastCgiContext cgiCxt(&req);

    cgiCxt.setMethod(params["REQUEST_METHOD"]);
    cgiCxt.setPath(params["SCRIPT_NAME"]);
    cgiCxt.setQuery(params["QUERY_STRING"]);
    cgiCxt.setVersion(params["SERVER_PROTOCOL"]);
    cgiCxt.headerSwap(params);

    std::cout << "methodString:" << req.methodString() << std::endl;
    std::cout << "pathString:" << req.path() << std::endl;
    std::cout << "queryString:" << req.query() << std::endl;


    if (in->readableBytes() > 0) {
        cgiCxt.setRequestBody(in->retrieveAllAsString());
	}


	response.append("Context->Type: text/plain\r\n\r\n");

    const string kPath = "/fastcgi/";
	if (uri.find(kPath) == 0) {
		response.append("good request!");
	} else {
		response.append("bad request!");
	}

	FastCgiCodec::respond(&response);
	conn->send(&response);
}

void HttpServer::onRequest(const TcpConnectionPtr& conn, const HttpRequest& req)
{
    const string& connection = req.getHeader("Connection");
    bool close = connection == "close" || (req.getVersion() == HttpRequest::kHttp10 && connection != "Keep-Alive");

    HttpResponse response(close);
    httpCallback_(req, &response);

    Buffer buf;
    response.appendToBuffer(&buf);
    conn->send(buf.peek(), buf.readableBytes());

    if (response.closeConnection()) {
        conn->shutdown();
    }
}