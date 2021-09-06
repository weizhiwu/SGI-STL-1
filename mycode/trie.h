#ifndef _TRIE_H
#define _TRIE_H

#include <cstring>
#include <cstdlib>

struct TrieNode
{
    TrieNode(char data)
    {
        this->data = data;
    }

    char data;
    TrieNode* children[26] = {nullptr};
    bool isEndingChar = false;
};

class Trie
{
public:
    Trie()
    {
        root = new TrieNode('/'); // 存储无意义字符
    } 

    // 往Trie树中插入一个字符串
    void insert(const char* text)
    {
        TrieNode* p = root;
        for (int i = 0; i < strlen(text); ++i)
        {
            int index = text[i] - 'a';
            if (p->children[index] == nullptr)
            {
                TrieNode* newNode = new TrieNode(text[i]);
                p->children[index] = newNode;
            }
            p = p->children[index];
        }
        p->isEndingChar = true;
    }

    // 在Trie树中查找一个字符串
    bool find(const char* pattern)
    {
        TrieNode* p = root;
        for (int i = 0; i < strlen(pattern); ++i)
        {
            int index = pattern[i] - 'a';
            if (p->children[index] == nullptr)
                return false; // 不存在pattern

            p = p->children[index];
        }

        if (p->isEndingChar == false)
            return false; // 不能完全匹配，只是前缀
        else
            return true;  // 找到pattern
    }

private:
    TrieNode* root;
};

#endif