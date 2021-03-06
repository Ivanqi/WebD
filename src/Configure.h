#ifndef WEBD_CONFIGURE_H
#define WEBD_CONFIGURE_H

#include <fstream>
#include <sstream>
#include <utility>
#include <map>
#include <string>
#include <iostream>
#include <stdio.h>

namespace webd
{
    class Configure
    {
        private:
            std::map<std::string, std::string> conf_;
            const char eqTag{'='};
            const char remarksTag{'#'};

        public:
            Configure(std::string confPath)
            {
                std::ifstream ifs(confPath.c_str(), std::ios::in);
                std::string line;
                std::string key, val;

                while(getline(ifs, line)) {
                    
                    if (line.find(remarksTag) != std::string::npos) continue;
                    
                    size_t found = line.find(eqTag);
                    if (found != std::string::npos) {
                        key = line.substr(0, found);
                        val = line.substr(found + 1);
                        conf_.insert(std::pair<std::string, std::string>(key, val));
                    }			    
                }
            }

            std::string getConf(string key, string defaultVal = "")
            {
                if (conf_.find(key) != conf_.end()) {
                    return conf_[key];
                } else {
                    return defaultVal;
                }
            }
    };
};
#endif