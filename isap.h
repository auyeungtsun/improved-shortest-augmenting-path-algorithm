#ifndef ISAP_H
#define ISAP_H

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const int INF = 1e9;

struct Edge {
    int to;
    int cap;
    int flow;
    int rev;
};

class Isap {
public:
    Isap(int n);
    void add_edge(int from, int to, int cap);
    int isap(int s, int t);

private:
    int n;
    vector<vector<Edge>> adj;
    vector<int> level;
    vector<int> gap;

    void bfs(int t);
};

#endif // ISAP_H