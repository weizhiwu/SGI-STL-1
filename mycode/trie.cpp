#include <iostream>
#include "trie.h"

//compile: g++ trie.cpp -o trie -std=c++11

int main()
{
    Trie trie;
    trie.insert("hello");
    trie.insert("her");
    trie.insert("hi");
    trie.insert("so");
    trie.insert("see");

    std::cout<<trie.find("her")<<std::endl;
    std::cout<<trie.find("her1")<<std::endl;
}