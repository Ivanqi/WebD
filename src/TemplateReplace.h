#ifndef WEBD_TEMPLATE_REPLACE_H
#define WEBD_TEMPLATE_REPLACE_H

#include <string>
#include <map>

class AcNode
{
    public:
        int isEndingChar;
        std::map<std::string, AcNode*> child;   // 键值为当前汉字，实值为后面汉字节点的指针
        AcNode *fail;                           // 失败指针


};


class TemplateReplace
{
    private:
        std::string resolverLeft{"{{"};
        std::string resolverRigth{"}}"};

    public:
        TemplateReplace()
        {

        }

        bool match(std::string& str)
        {

        }
};

#endif