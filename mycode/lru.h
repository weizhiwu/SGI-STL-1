#ifndef _MY_LRU_H
#define _MY_LRU_H

/*
 *  LRU 缓存结构 （c++ 哈希双链表实现）
    https://blog.csdn.net/u014386899/article/details/108523102
 */

#include <unordered_map>
#include <vector>

struct DListNode
{
    int key = 0;
    int val = 0;
    DListNode* pre = nullptr;
    DListNode* next = nullptr;
    DListNode(int k, int v) : key(k), val(v), pre(nullptr), next(nullptr) {}
};

class Solution
{
private:
    int size = 0;
    DListNode* head = nullptr;
    DListNode* tail = nullptr;
    std::unordered_map<int, DListNode*> mp;

public:
    std::vector<int> LRU(std::vector<std::vector<int>>& operators, int k)
    {
        if (k < 1) return {};
        this->size = k;
        this->head = new DListNode(0, 0);
        this->tail = new DListNode(0, 0);
        this->head->next = this->tail;
        this->tail->pre = this->head;

        if (operators.empty()) return {};

        std::vector<int> res;
        for (auto& op : operators)
        {   
            //暂时没有检查op的size
            if (op[0] == 1) 
            {
                set(op[1], op[2]);
            } 
            else if (op[0] == 2)
            {
                int value = get(op[1]);
                res.push_back(value);
            }
        }

        return res;
    }

    void set(int key, int val)
    {
        if (mp.find(key) == mp.end())
        {
            DListNode* node = new DListNode(key, val);
            mp[key] = node;
            if (this->size <= 0)
            {
                removeLast();
            }
            else
            {
                --this->size;
            }

            insertFirst(node);
        }
        else
        {
            mp[key]->val = val; //update value
            moveToHead(mp[key]);
        }
    }

    int get(int key)
    {
        int ret = -1;
        if (mp.find(key) != mp.end())
        {
            ret = mp[key]->val;
            moveToHead(mp[key]);
        }
        return ret;
    }

    void moveToHead(DListNode* node)
    {
        if (node->pre == this->head) return;
        node->pre->next = node->next;
        node->next->pre = node->pre;
        insertFirst(node);
    }

    void removeLast()
    {
        mp.erase(this->tail->pre->key);
        this->tail->pre->pre->next = this->tail;
        this->tail->pre = this->tail->pre->pre;
    }

    void insertFirst(DListNode* node)
    {
        node->pre = this->head;
        node->next = this->head->next;
        this->head->next->pre = node;
        this->head->next = node;
    }
};

#endif