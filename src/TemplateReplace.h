#ifndef WEBD_TEMPLATE_REPLACE_H
#define WEBD_TEMPLATE_REPLACE_H

#include "TransCode.h"
#include <map>
#include <string>
#include <vector>
using std::map;
using std::string;
using std::vector;

namespace webd
{
    typedef UnicodeValueType TrieKey;
    #define BM_MAX_SIZE 65535

    class TrieNode
    {
        public:
            typedef map<TrieKey, TrieNode*> NextMap;
            TrieNode *fail; // 失败指针
            NextMap *next;
            UnicodeValueType word;
            bool isEnding{false};  // 结尾字符为true
            int length{0};

        public:
            TrieNode()
                :fail(NULL), next(NULL)
            {

            }

            const TrieNode* findNext(TrieKey key) const
            {
                if (next == NULL) {
                    return NULL;
                }

                NextMap::const_iterator iter = next->find(key);
                if (iter == next->end()) {
                    return NULL;
                }

                return iter->second;
            }
    };


    class TemplateReplace
    {
        private:
            std::string resolverLeft_{"{{"};
            std::string resolverRigth_{"}}"};
            bool leftFlag_{false};
            const map<string, string> field_;
            Unicode resolverLeftUni_;
            Unicode resolverRigthUni_;
            map<std::string, std::vector<int>> saveBc_;

            TrieNode *root_;

        public:
            TemplateReplace(const map<string, string>& paramlist);

            ~TemplateReplace();

            const TrieNode* find(Unicode::const_iterator begin, Unicode::const_iterator end) const;

            void buildFailurePointer();

            void printfFailurePointer();

            void insertNode(const Unicode& key);

            string match(Unicode::const_iterator begin, Unicode::const_iterator end, string matchStr, char replaceStr);

            string matchReplace(string text);

            string matchByBm(string text);

            void deleteNode(TrieNode* node);
        private:
            int calcUnicodeLen(Unicode::const_iterator uni);
            
            int calcUnicodeLen(TrieKey uni);


            string replaceFun(map<int, int> check, string text, char replaceStr);

            // 构建散列表
            void generateBc(Unicode p, vector<int>* bc);

            /**
             * 构建 suffix 和 prefix 数组
             *  模式串的后缀子串和前缀子串
             *  后缀子串是用于与好后缀子串进行匹配的。 
             *  前缀子串是用于与好后缀中最长的子串进行匹配。
            */
            void generateGS(Unicode pUni, int *suffix, bool *prefix);

            /* 好后缀移动规则
             *  j 表示坏字符所在位置
             *  pLen 表示模式串的长度
             */
            int moveByGs(int j, int pLen, int *suffix, bool *prefix);

            // bm 字符串替换算法
            int BM(Unicode sUni, Unicode patternUni, std::string pattern);
    };
};
#endif