#ifndef WEBD_TEMPLATE_REPLACE_H
#define WEBD_TEMPLATE_REPLACE_H

#include "TransCode.h"
#include <unordered_map>
#include <string>
using std::unordered_map;
using std::string;


namespace webd
{
    typedef UnicodeValueType TrieKey;

    class TrieNode
    {
        public:
            typedef unordered_map<TrieKey, TrieNode*> NextMap;
            TrieNode *fail; // 失败指针
            NextMap *next;
            UnicodeValueType word;
            bool isEnding;  // 结尾字符为true
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
            std::string resolverLeft{"{{"};
            std::string resolverRigth{"}}"};

            TrieNode *root_;

        public:
            TemplateReplace();

            ~TemplateReplace();

            const TrieNode* find(Unicode::const_iterator begin, Unicode::const_iterator end) const;

            void buildFailurePointer();

            void printfFailurePointer();

            void insertNode(const Unicode& key);

            string match(Unicode::const_iterator begin, Unicode::const_iterator end, string matchStr, char replaceStr);

            string replaceFun(unordered_map<int, int> check, string text, char replaceStr);

            void deleteNode(TrieNode* node);
        private:
            int calcUnicodeLen(Unicode::const_iterator uni);
    };
};
#endif