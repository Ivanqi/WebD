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
#include "networker/base/Logging.h"
#include "src/TemplateReplace.h"
#include "src/TransCode.h"
#include "src/ActiveFile.h"

using namespace networker;
using namespace networker::net;
namespace webd
{
    // 线程不安全
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
            std::string defaultHtml{"index.html"};
            std::map<std::string, bool> allowMimeType_;
            std::map<string, string> paramlist_;
            ActiveFile activeFile_;
        
        public:
            TemplateParse(const std::string path)
                :tempDir_(path)
            {

            }

            void setAllowMimeType(std::map<std::string, bool>& allowMimeType)
            {
                allowMimeType_ = allowMimeType;
            }

            void setParamlist(const std::map<string, string>& paramlist)
            {
                paramlist_ = paramlist;
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


                if (templateList.find(absPath) != templateList.end()) {
                    fileInfo = templateList[absPath];
                    if (buffer.st_mtime > fileInfo->modifiy_time) {
                        fileInfo.reset(new tempInfo);
                        fileInfo->modifiy_time = buffer.st_mtime;
                        bool ret = readFile(fileInfo->content, absPath);
                        templateList[absPath] = fileInfo;
                        activeFile_.push(absPath, buffer.st_mtime);
                        return ret;
                    }

                } else {
                    fileInfo = tempInfoPtr(new tempInfo);
                    fileInfo->modifiy_time = buffer.st_mtime;
                    bool ret =  readFile(fileInfo->content, absPath);
                    templateList[absPath] = fileInfo;
                    bool activeRet = activeFile_.push(absPath, buffer.st_mtime);
                    if (!activeRet) {
                        std::string popStr = activeFile_.pop();
                        templateList.erase(popStr);
                    }
                    return ret;
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

                    activeFile_.push(absPath, statbuf.st_mtime);
                    templateList[absPath] = fileInfo;
                }

                closedir(dp);
                chdir(currPath);

                return true;
            }

            /**
             * 文件解析
             *  1. 读取目标文件
             *  2. 目标文件的特定标志进行BM字符串替换
             *  3. 返回替换后的内容
             */
            bool parse(std::string& filename, std::string& content, std::string& suffix)
            {
                if (filename == "/") {
                    filename = defaultHtml;
                }

                std::string absPath = tempDir_ + filename;
                int ret = checkFile(filename);

                if (!ret) return ret;

                // LOG_INFO << "absPath: " << absPath;

                if (templateList.find(absPath) == templateList.end()) {
                    return false;
                }

                tempInfoPtr tempInfo = templateList[absPath];

                Buffer buf = tempInfo->content;

                // 获取所有内容
                content = buf.retrieveAllAsString();

                // 判断 suffix 是否存在
                if (allowMimeType_.find(suffix) != allowMimeType_.end()) {
                    TemplateReplace replace(paramlist_);
                    content = replace.matchByBm(content);
                
                }

                return true;
            }
            
        
        private:
            // 读取文件
            bool readFile(Buffer& content, const std::string path)
            {
                std::ifstream fin(path, std::ios::binary);
                unsigned long len = static_cast<unsigned int>(fin.seekg(0, std::ios::end).tellg());

                if (len <= 0) {
                    return false;
                }

                char ich;
                Buffer buf(len);

                fin.seekg(0);
                while (fin.get(ich)) {
                    buf.append(&ich, 1);
                }

                fin.close();

                content.swap(buf);
                return true;
            }
    };
};
#endif