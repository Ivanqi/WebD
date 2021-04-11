#ifndef WEBD_TEMPLATE_ENGINE_H
#define WEBD_TEMPLATE_ENGINE_H

#include <sys/stat.h>
#include <map>
#include <string>
#include <unistd.h>
#include <memory>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include "networker/net/Buffer.h"

using namespace networker;
using namespace networker::net;
namespace webd
{
    class TemplateEngine
    {
        private:
            struct tempInfo {
                time_t modifiy_time;
                Buffer content;
            };

            typedef std::shared_ptr<tempInfo> tempInfoPtr;
            std::map<std::string, tempInfoPtr> templateList;
            std::string tempDir_{""};
        
        public:
            TemplateEngine(const std::string path)
                :tempDir_(path)
            {

            }

            void setTempDIr(const std::string path)
            {
                tempDir_ = path;
            }

            bool checkFile(const std::string filename)
            {
                struct stat buffer;
                std::string absPath = tempDir_ + filename;
                bool ret =  (stat(absPath.c_str(), &buffer) == 0);

                if (!ret) return ret;
                tempInfoPtr fileInfo;

                if (templateList.find(absPath) != templateList.end()) {
                    fileInfo = templateList[absPath];
                    if (buffer.st_mtime > fileInfo->modifiy_time) {
                        fileInfo.reset(new tempInfo);
                        fileInfo->modifiy_time = buffer.st_mtime;

                        return readFile(fileInfo->content, absPath);
                    }

                } else {
                    fileInfo = tempInfoPtr(new tempInfo);
                    fileInfo->modifiy_time = buffer.st_mtime;
                    
                    return readFile(fileInfo->content, absPath);

                    templateList[absPath] = fileInfo;
                }
            }

            bool preLoading()
            {
                char currPath[128];
                getcwd(currPath, sizeof(currPath) - 1);

                DIR *dp;
                struct dirent *entry;
                struct stat statbuf;

                if ((dp = opendir(tempDir_.c_str())) == NULL) {
                    return false;
                }

                chdir(tempDir_.c_str());

                std::string absPath;

                while ((entry = readdir(dp)) != NULL) {
                    if (!S_ISDIR(statbuf.st_mode)) {
                        if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name)) {
                           continue; 
                        }

                        std::string filename(entry->d_name);
                        absPath = tempDir_ + filename;

                        tempInfoPtr fileInfo(new tempInfo);
                        fileInfo->modifiy_time = statbuf.st_mtime;

                        readFile(fileInfo->content, absPath);
                        templateList[absPath] = fileInfo;
                    }
                }

                closedir(dp);
                chdir(currPath);
                return true;
            }

            bool parse(const std::string& filename, std::string& content)
            {
                std::string absPath = tempDir_ + filename;
                int ret = checkFile(filename);

                if (!ret) return ret;

                tempInfoPtr tempInfo = templateList[absPath];

                return true;
            }
            
        
        private:
            bool readFile(Buffer &content, const std::string path)
            {
                FILE *fp;
                int saveErrno = 0;

                fp = fopen(path.c_str(), "r+");
                if (fp == NULL) return false;
                    

                int n = content.readFd(fp, &saveErrno);
                if (n < 0) return false;

                return true;
            }
    };
};
#endif