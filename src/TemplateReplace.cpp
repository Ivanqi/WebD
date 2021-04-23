#include "src/TemplateReplace.h"
#include <queue>
#include <assert.h>
#include <iostream>
#include <algorithm>
using std::queue;
using namespace webd;

TemplateReplace::TemplateReplace(const unordered_map<string, string>& paramlist)
    :field_(paramlist)
{
    root_ = new TrieNode;

    resolverLeftUni_ = TransCode::decode(resolverLeft_);
    resolverRigthUni_ = TransCode::decode(resolverRigth_);
}


TemplateReplace::~TemplateReplace()
{
    if (root_) {
        deleteNode(root_);
    }
}

const TrieNode* TemplateReplace::find(Unicode::const_iterator begin, Unicode::const_iterator end) const
{
    TrieNode::NextMap::const_iterator citer;
    const TrieNode *ptNode = root_;

    for (Unicode::const_iterator it = begin; it != end; it++) {
        // build automation
        assert(ptNode);
        if (NULL == ptNode->next || ptNode->next->end() == (citer = ptNode->next->find(*it))) {
            return NULL;
        }

        ptNode = citer->second;
    }

    return ptNode;
}

void TemplateReplace::buildFailurePointer()
{
    std::queue<TrieNode*> que;
    assert(root_->next);

    root_->fail = NULL;

    for (TrieNode::NextMap::iterator iter = root_->next->begin(); iter != root_->next->end(); iter++) {
        iter->second->fail = root_;
        que.push(iter->second);
    }

    TrieNode *back = NULL;
    TrieNode::NextMap::iterator backiter;

    while (!que.empty()) {
        TrieNode *now = que.front();
        que.pop();

        if (now->next == NULL) {
            continue;
        }

        for (TrieNode::NextMap::iterator iter = now->next->begin(); iter != now->next->end(); iter++) {
            back = now->fail;
            while (back != NULL) {
                if (back->next && (backiter = back->next->find(iter->first)) != back->next->end()) {
                    iter->second->fail = backiter->second;
                    break;
                }
                back = back->fail;
            }

            if (back == NULL) {
                iter->second->fail = root_;
            }

            que.push(iter->second);
        }
    }
}

void TemplateReplace::printfFailurePointer()
{
    std::queue<TrieNode*> que;
    assert(root_->next);

    for (TrieNode::NextMap::iterator iter = root_->next->begin(); iter != root_->next->end(); iter++) {
        que.push(iter->second);
    }

    while (!que.empty()) {
        TrieNode *now = que.front();
        que.pop();

        std::cout << now->word;

        if (now->fail == NULL) {
            continue;
        } else {
            std::cout << "| fail word:" << now->fail->word << std::endl;
        }

        if (now->next == NULL) {
            continue;
        }

        for (TrieNode::NextMap::iterator iter = now->next->begin(); iter != now->next->end(); iter++) {
            que.push(iter->second);
        }

    }
}

void TemplateReplace::insertNode(const Unicode& key)
{
    TrieNode *ptNode = root_;

    TrieNode::NextMap::const_iterator kmIter;

    int len = 0;

    for (Unicode::const_iterator citer = key.begin(); citer != key.end(); citer++) {
        if (NULL == ptNode->next) {
            ptNode->next = new TrieNode::NextMap;
        }

        kmIter = ptNode->next->find(*citer);
        len += calcUnicodeLen(citer);

        if (ptNode->next->end() == kmIter) {
            TrieNode *nextNode = new TrieNode;
            nextNode->word = (*citer);
            nextNode->next = NULL;

            (*ptNode->next)[*citer] = nextNode;
            ptNode = nextNode;
        } else {
            ptNode = kmIter->second;
        }
    }

    ptNode->isEnding = true;
    ptNode->length = len;
   
}

string TemplateReplace::matchReplace(string text)
{
    bool flag = true;
    size_t found = 0;

    int leftLen = resolverLeft_.length();
    int rightLen = resolverRigth_.length();
    int textLen = text.length();
    int i = 0;

    while (flag) {
        if (textLen - i <= 0) flag = false;
        if (!leftFlag_) {
            if (text.find(resolverLeft_) != string::npos) {
                found = text.find(resolverLeft_);
                leftFlag_ = true;
            } else {
                flag = false;
            }
        } else {
            if (text.find(resolverRigth_) != string::npos) {
                size_t preFoud = found;
                found = text.find(resolverRigth_);
                string tmpStr = text.substr(preFoud + leftLen, (found - preFoud - rightLen));

                if (field_.find(tmpStr) != field_.end()) {
                    string value = field_.at(tmpStr);
                    text.replace(preFoud, found - preFoud + rightLen, value.c_str());
                }
                leftFlag_ = false;
            } else {
                flag = false;
            }
        }

        i++;
    }

    return text;
}

string TemplateReplace::matchByBm(string text)
{
    bool flag = true;
    int found = 0;

    int leftLen = resolverLeft_.length();
    int rightLen = resolverRigth_.length();
    int textLen = text.length();
    int i = 0;

    while (flag) {
        if (textLen - i <= 0) flag = false;
        Unicode textUni = TransCode::decode(text);
        if (!leftFlag_) {
            found = BM(textUni, resolverLeftUni_, resolverLeft_);
            if (found != -1) {
                leftFlag_ = true;
            } else {
                flag = false;
            }
        } else {
            int preFoud = found;
            found = BM(textUni, resolverRigthUni_, resolverRigth_);
            if (leftFlag_ && found != -1) {
                
                int leftPos = 0;
                int i;
                for (i = 0; i < preFoud; i++) {
                    leftPos += calcUnicodeLen(textUni[i]);
                }

                int rightPos = leftPos;
                for (i = preFoud; i < found; i++) {
                    rightPos += calcUnicodeLen(textUni[i]);
                }

                string tmpStr = text.substr(leftPos + leftLen, rightPos - leftPos - rightLen);
                tmpStr = StringUtil::Trim(tmpStr, ' ');
                if (field_.find(tmpStr) != field_.end()) {
                    tmpStr = field_.at(tmpStr);;
                }
                text.replace(leftPos, rightPos - leftPos + rightLen, tmpStr.c_str());
                i = 0;
                leftFlag_ = false;
            } else {
                flag = false;
            }
        }

        i++;
    }

    return text;
}

string TemplateReplace::match(Unicode::const_iterator begin, Unicode::const_iterator end, string matchStr, char replaceStr)
{
    TrieNode *ptNode = root_;
    int i = 0;

    unordered_map<int, int> check;

    for (Unicode::const_iterator citer = begin; citer != end; citer++) {
        /**
         * 使用 fail指针对 p的值进行矫正
         * 比如查询到某个模式串, p 值不是指向root. 然后主串继续遍历，主串的值在p上匹配不到值，需要fail指针把p指向root
         */
        while (ptNode != NULL && ptNode != root_ && ptNode->next != NULL && ptNode->next->find(*citer) == ptNode->next->end()) {
            ptNode = ptNode->fail;  // 失败指针发挥作用的地方
        }

        if (ptNode != NULL && ptNode->next != NULL) {
            if (ptNode->next->find(*citer) != ptNode->next->end()) {
                ptNode = ptNode->next->find(*citer)->second;
            }
        } else {
            ptNode = NULL;
        }

        if (ptNode == NULL) ptNode = root_; // 没有匹配，从root开始重新匹配

        TrieNode *tmp = ptNode;
        int len = calcUnicodeLen(citer);

        while (tmp != NULL && tmp != root_) {
            if (tmp->isEnding == true) {
                int pos = i - tmp->length + len;
                check[pos] = tmp->length;
            }
            tmp = tmp->fail;
        }
        i += len;
    }

    return replaceFun(check, matchStr, replaceStr);
}

string TemplateReplace::replaceFun(unordered_map<int, int> check, string text, char replaceStr)
{
    unordered_map<int, int>::iterator it;
    for (it = check.begin(); it != check.end(); it++) {                
        text.replace(it->first, it->second, it->second, replaceStr);
    }
    return text;

    return text;
}

void TemplateReplace::deleteNode(TrieNode* node)
{
    if (!node) {
        return;
    }

    if (node->next) {
        TrieNode::NextMap::iterator it;
        for (it = node->next->begin(); it != node->next->end(); it++) {
            deleteNode(it->second);
        }

        delete node->next;
    }

    delete node;
}

// 构建散列表
void TemplateReplace::generateBc(Unicode p, vector<int>* bc)
{
    int pLen = p.size();
    for (int i = 0; i < pLen; i++) {
        uint16_t as = p[i];
        bc->at(as) = i;
    }
}

/**
 * 构建 suffix 和 prefix 数组
 *  模式串的后缀子串和前缀子串
 *  后缀子串是用于与好后缀子串进行匹配的。 
 *  前缀子串是用于与好后缀中最长的子串进行匹配。
*/
void TemplateReplace::generateGS(Unicode pUni, int *suffix, bool *prefix)
{
    int pLen = pUni.size();
    int i = 0;
    for (i = 0; i < pLen; i++) {
        suffix[i] = -1;
        prefix[i] = false;
    }

    for (i = 0; i < pLen - 1; i++) {
       int j = i;
       int k = 0;
        while (j >= 0 && pUni[j] == pUni[pLen - 1 - k]) {
           --j;
           ++k;
           suffix[k] = j + 1;
        }

        if (j == -1) prefix[k] = true;
    }
}

/* 好后缀移动规则
    *  j 表示坏字符所在位置
    *  pLen 表示模式串的长度
    */
int TemplateReplace::moveByGs(int j, int pLen, int *suffix, bool *prefix)
{
    int k = pLen - 1 - j;   // 好后缀长度

    // 情况1，suffix中存在好后缀
    if (suffix[k] != -1) return j - suffix[k] + 1;
    int r;

    // 情况2，查找好后缀的最长字符串是否和prefix的前缀子串匹配
    for (r = j + 2; r <= pLen - 1; r++) {
        if (prefix[pLen - r] == true) return r;
    }

    // 情况3， 什么情况都没有匹配.返回模式串的长度
    return pLen;
}

// bm 字符串替换算法
int TemplateReplace::BM(Unicode sUni, Unicode patternUni, std::string pattern)
{
    vector<int> bc;
    if (saveBc_.find(pattern) != saveBc_.end()) {
        bc = saveBc_.find(pattern)->second;
    } else {
        vector<int> tmp(BM_MAX_SIZE, -1);
        bc = tmp;
        generateBc(patternUni, &bc);
    }

    int sLen = sUni.size();
    int pLen = patternUni.size();
    int suffix[pLen];
    bool prefix[pLen];


    generateGS(patternUni, suffix, prefix);

    int i = 0;  
    while (i <= sLen - pLen) {
        int j;                                      // j表示主串与模式串匹配的第一个字符
        for (j = pLen - 1; j >= 0; j--) {           // 模式串从后往前匹配
            if (sUni[i + j] != patternUni[j]) break; // 坏字符对应模式串的下标是j
        }

        if (j < 0) return i;    // 匹配成功，返回主串与模式串第一个匹配的字符的位置

        int x = (j - bc[sUni[i + j]]);
        int y = 0;

        if (j < pLen - 1) {     // 如果有好后缀的话
           y = moveByGs(j, pLen, suffix, prefix);
        } 
        i = i + (x > y ? x : y);
    }

    return -1;
}

int TemplateReplace::calcUnicodeLen(Unicode::const_iterator uni)
{
    TrieKey ui = *uni;

    return calcUnicodeLen(ui);
}

int TemplateReplace::calcUnicodeLen(TrieKey ui) {
    if (ui <= 0x7f) {
        return 1;
    } else if (ui <= 0x7ff) {
        return 2;
    } else {
        return 3;
    }
}