#include "src/Entry.h"
#include "src/HttpServer.h"
#include "src/HttpRequest.h"
#include "src/HttpResponse.h"
#include "src/StringUtil.h"
#include "networker/base/Thread.h"
#include "networker/net/EventLoop.h"
#include "networker/base/Logging.h"
#include "networker/base/AsyncLogging.h"
#include "src/Configure.h"


using namespace webd;
using namespace std::placeholders;

std::unique_ptr<AsyncLogging> g_asyncLog_;

Entry::Entry()
    : kRollSize_(500 * 1000 * 1000), parse_(new TemplateParse("/"))
{
    MimeType[".html"] = "text/html";
    MimeType[".avi"] = "video/x-msvideo";
    MimeType[".bmp"] = "image/bmp";
    MimeType[".c"] = "text/plain";
    MimeType[".doc"] = "application/msword";
    MimeType[".gif"] = "image/gif";
    MimeType[".gz"] = "application/x-gzip";
    MimeType[".htm"] = "text/html";
    MimeType[".ico"] = "image/x-icon";
    MimeType[".jpg"] = "image/jpeg";
    MimeType[".png"] = "image/png";
    MimeType[".txt"] = "text/plain";
    MimeType[".mp3"] = "audio/mp3";
    MimeType["default"] = "text/html";
}

void Entry::start(char* argv[])
{
    int numTreads = 0;
    Configure conf(argv[1]);	//加载配置文件

    string confNumTreads = conf.getConf("numTreads");
    string confIp = conf.getConf("ip");
    string confPort = conf.getConf("port");
    string webRoot = conf.getConf("web_root");
    string logLevel = conf.getConf("log_level");
    string logDir = conf.getConf("log_dir");

    if (webRoot.empty()) {
        printf("Web Root 目录不存在");
        return;
    }

    if ((!logLevel.empty()) && (!logDir.empty())) {
       setLogging(logDir, logLevel);
    }

    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();

    parse_->setTempDIr(webRoot);
    parse_->preLoading();
    
    numTreads = confNumTreads.empty() ? numTreads : ::atoi(confNumTreads.c_str());

    EventLoop loop;
    HttpServer server(&loop, InetAddress(confIp, atoi(confPort.c_str())), "webd");

    HttpServer::HttpCallback cb = std::bind(&Entry::onRequest, this, _1, _2);
    server.setHttpCallback(cb);
    server.setThreadNum(numTreads);

    server.start();
    loop.loop();
}

void Entry::onRequest(const HttpRequest& req, HttpResponse* resp)
{
    std::string context;

    string path = req.path();
    
    bool ret = parse_->parse(path, context, req.paramlist());

    LOG_INFO << "path:" << path << "ret:" << ret;

    size_t found = path.find('.');

    string ContentType;
    ContentType = MimeType["default"];

    if (found != std::string::npos) {
        string suffix = path.substr(found);
        LOG_INFO << suffix;
        if (MimeType.find(suffix) != MimeType.end()) {
            ContentType = MimeType[suffix];
        }
    } 

    if (ret) {
        resp->setStatusCode(HttpResponse::k200OK);
        resp->setStatusMessage("OK");
        resp->setContentType(ContentType);
        resp->addHeader("Server", "WEBD");
        resp->addHeader("Content-Length", "41687");
        resp->setBody(context);

    } else {
        resp->setStatusCode(HttpResponse::k404NotFound);
        resp->setStatusMessage("Not Found");
        resp->setCloseConnection(true);
    }
}

void Entry::asyncOutput(const char *msg, int len)
{
    g_asyncLog_->append(msg, len);
}

Entry::hash_t Entry::hash(char const* str)
{
    hash_t ret{basis_};  
   
    while(*str){  
        ret ^= *str;  
        ret *= prime_;  
        str++;  
    }  
   
    return ret;
}

constexpr Entry::hash_t Entry::hashCompileTime(char const* str, Entry::hash_t lastValue)
{
    return *str ? hashCompileTime(str + 1, (*str ^ lastValue) * prime_) : lastValue;  
}

Logger::LogLevel Entry::getLogLevel(const string& levelStr)
{
    switch (hash(levelStr.c_str())) {
        case hashCompileTime("info"):
            return Logger::INFO;
            break;
        
        case hashCompileTime("debug"):
            return Logger::DEBUG;
            break;
        
        case hashCompileTime("trace"):
            return Logger::TRACE;
            break;
        
        case hashCompileTime("warn"):
            return Logger::WARN;
            break;
        
        case hashCompileTime("error"):
            return Logger::ERROR;
            break;
        
        case hashCompileTime("fatal"):
            return Logger::FATAL;
            break;
        default:
            return Logger::INFO;
            break;
    }
}

void Entry::setLogging(string logDir, const string& logLevel)
{
    Logger::setLogLevel(getLogLevel(logLevel));
    Logger::setOutput(&Entry::asyncOutput);
    
    g_asyncLog_.reset(new AsyncLogging(logDir, kRollSize_));
    g_asyncLog_->start();
}