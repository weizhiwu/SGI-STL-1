#include "graph.h"
#include <iostream>
#include <queue>

//compile: g++ .\graph.cpp -o graph -std=c++11

std::vector<int> Graph::bfs(int s, int t)
{
    std::vector<int> prev(v, -1);
    if (s == t) return prev;

    std::vector<bool> visited(v);
    visited[s] = true;
    std::queue<int> que;
    que.push(s);
    
    while (!que.empty())
    {
        int w = que.front();
        que.pop();
        for (auto q : adj[w])
        {
            if (!visited[q])
            {
                prev[q] = w;
                if (q == t)
                    return prev;
            }
            visited[q] = true;
            que.push(q);
        }
    }

    return prev;
}

void Graph::print(std::vector<int>& prev, int s, int t)
{
    if (prev[t] != -1 && t != s)
    {
        print(prev, s, prev[t]);
    }
    std::cout<<t<<'\t';
}

void Graph::dfs(int s, int t)
{
    dfsfound = false;
    std::vector<bool> visited(v);
    std::vector<int> prev(v, -1);

    recurDfs(s, t, visited, prev);
    print(prev, s, t);
}

void Graph::recurDfs(int w, int t, std::vector<bool>& visited, std::vector<int>& prev)
{
    if (dfsfound == true) return;

    visited[w] = true;
    if (w == t)
    {
        dfsfound = true;
        return;
    }    

    for (auto q : adj[w])
    {
        if (!visited[q])
        {
            prev[q] = w;
            recurDfs(q, t, visited, prev);
        }
    }
}

/*
      0 ----  1 ---- 2
      |       |      |  
      |       |      |
      3 ----  4 ---- 5
              |      |
              |      |
              6 ---- 7
*/

int main()
{
    Graph g(8);
    g.addEdge(0, 1).addEdge(0, 3).addEdge(1, 2).addEdge(1, 4).addEdge(2, 5).addEdge(4, 5).addEdge(4, 6);
    g.addEdge(5, 7).addEdge(6, 7).addEdge(3, 4);

    auto vec = g.bfs(0, 6);
    g.print(vec, 0, 6);
    std::cout<<std::endl;

    g.dfs(0, 6);
    std::cout<<std::endl;
}