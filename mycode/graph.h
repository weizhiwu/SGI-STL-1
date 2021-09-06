#ifndef _GRAPH_H
#define _GRAPH_H

#include <vector>
#include <list>

//无向图
class Graph
{
public:
    Graph(int _v)
    {
        v = _v;
        adj.resize(v);
    }

    Graph& addEdge(int s, int t)
    {
        adj[s].push_back(t);
        adj[t].push_back(s);
        return *this;
    }

    std::vector<int> bfs(int s, int t);
    void print(std::vector<int>& prev, int s, int t);

    void dfs(int s, int t);
    void recurDfs(int w, int t, std::vector<bool>& visited, std::vector<int>& prev);

private:
    int v = 0;
    std::vector<std::list<int>> adj;
    bool dfsfound = false;
};

#endif