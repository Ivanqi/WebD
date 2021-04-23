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
#include <fstream>
#include <iostream>
#include "networker/net/Buffer.h"
#include "networker/base/MutexLock.h"
#include "src/TemplateReplace.h"
#include "src/TransCode.h"

using namespace networker;
using namespace networker::net;
namespace webd
{
    class TemplateParse
    {
        private:
            struct tempInfo {
                time_t modifiy_time;
                Buffer content;
            };

            typedef std::shared_ptr<tempInfo> tempInfoPtr;
            std::map<std::string, tempInfoPtr> templateList;
            std::string tempDir_{""};
            MutexLock mutex_;
        
        public:
            TemplateParse(const std::string path)
                :tempDir_(path)
            {

            }

            void setTempDIr(const std::string path)
            {
                tempDir_ = path;
            }

            // 线程不安全
            bool checkFile(const std::string filename)
            {
                struct stat buffer;
                std::string absPath = tempDir_ + filename;
                bool ret =  (stat(absPath.c_str(), &buffer) == 0);

                if (!ret) return ret;
                tempInfoPtr fileInfo;

                {
                    MutexLockGuard lock(mutex_);
                    if (templateList.find(absPath) != templateList.end()) {
                        fileInfo = templateList[absPath];
                        if (buffer.st_mtime > fileInfo->modifiy_time) {
                            fileInfo.reset(new tempInfo);
                            fileInfo->modifiy_time = buffer.st_mtime;
                            bool ret = readFile(fileInfo->content, absPath);
                            templateList[absPath] = fileInfo;
                            return ret;
                        }

                    } else {
                        fileInfo = tempInfoPtr(new tempInfo);
                        fileInfo->modifiy_time = buffer.st_mtime;
                        bool ret =  readFile(fileInfo->content, absPath);
                        templateList[absPath] = fileInfo;
                        return ret;
                    }
                }

                return true;
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
                    if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0) {
                        continue; 
                    }

                    if (stat(entry->d_name, &statbuf) == -1) continue;
                    if (S_ISDIR(statbuf.st_mode)) continue;

                    std::string filename(entry->d_name);
                    absPath = tempDir_ + filename;

                    tempInfoPtr fileInfo(new tempInfo);

                    fileInfo->modifiy_time = statbuf.st_mtime;

                    if (!readFile(fileInfo->content, absPath)) {
                        break;
                    }
                    templateList[absPath] = fileInfo;
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

                std::cout << "absPath: " << absPath << std::endl;

                if (templateList.find(absPath) == templateList.end()) {
                    return false;
                }

                tempInfoPtr tempInfo = templateList[absPath];

                Buffer buf = tempInfo->content;
                content = buf.retrieveAllAsString();
                
                TemplateReplace replace;

                content = replace.matchByBm(content);
                return true;
            }
            
        
        private:
            bool readFile(Buffer& content, const std::string path)
            {

                std::ifstream fin(path, std::ios::binary);

                unsigned long len = static_cast<unsigned int>(fin.seekg(0, std::ios::end).tellg());

                if (len <= 0) {
                    return false;
                }

                Buffer buf;
                fin.seekg(0, std::ios::beg).read(buf.beginWrite(), len);
                fin.close();

                content.swap(buf);
                content.hasWritten(len);

                return true;
            }
    };
};
#endif