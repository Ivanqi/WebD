#ifndef WEBD_ENTRY_H
#define WEBD_ENTRY_H

#include "src/TemplateParse.h"
#include "networker/base/Thread.h"
#include "networker/net/EventLoop.h"
#include "networker/base/Logging.h"
#include "networker/base/AsyncLogging.h"

#include <functional>
#include <utility>

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <memory>

using namespace networker;
using namespace networker::net;

namespace webd
{
    class HttpRequest;
    class HttpResponse;

    class Entry
    {
        public:
            typedef std::uint64_t hash_t;
        private:
            int kRollSize_;
            constexpr hash_t static prime_{0x100000001B3ull};  
            constexpr hash_t static basis_{0xCBF29CE484222325ull};
            std::unique_ptr<TemplateParse> parse_;

        public:
            Entry();

            void start(char* argv[]);

            void onRequest(const HttpRequest& req, HttpResponse* resp);

            void static asyncOutput(const char *msg, int len);
                    
        private:
            hash_t hash(char const* str);

            constexpr hash_t static hashCompileTime(char const* str, hash_t lastValue = basis_);

            Logger::LogLevel getLogLevel(const string& levelStr);

            void setLogging(string logDir, const string& logLevel);  
    };
};


#endif