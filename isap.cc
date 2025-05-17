#include "isap.h"
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

/**
 * @brief Constructor for the Isap class.
 * @brief adj is the adjacency list representing the graph.
 *        level is the distance label of each node from the sink node in the residual graph.
 *        gap is an array that stores the number of nodes at each level.
 * @brief In the context of network flow algorithms like ISAP, the "residual graph" is a graph that represents the remaining capacity along the edges of the original graph after some flow has been pushed through.
 *        It contains the same vertices as the original graph but has modified edges. Each edge in the residual graph represents either the remaining capacity along an original edge, or the ability to "undo" flow along that edge.
 *        The residual graph is used to determine if there is still a path to push more flow through, and it is updated after each flow push.
 * @param n The number of nodes in the graph.
 */
Isap::Isap(int n) : adj(n), level(n), gap(n) {
    this->n = n;
}

/**
 * @brief Adds an edge from node u to node v with the given capacity. Also creates a reverse edge with capacity 0.
 * @param u The source node.
 * @param v The destination node.
 * @param cap The capacity of the edge.
 */
void Isap::add_edge(int u, int v, int cap) {
    adj[u].push_back({v, cap, 0, (int)adj[v].size()});
    adj[v].push_back({u, 0, 0, (int)adj[u].size() - 1});
}

void Isap::bfs(int t) {
    fill(level.begin(), level.end(), -1);
    fill(gap.begin(), gap.end(), 0);
    queue<int> q;
    q.push(t);
    level[t] = 0;
    gap[0] = 1;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto& e : adj[u]) {
            int v = e.to;
            if (level[v] == -1 && adj[v][e.rev].cap > adj[v][e.rev].flow) {
                level[v] = level[u] + 1;
                gap[level[v]]++;
                q.push(v);
            }
        }
    }
}

/**
 * @brief Calculates the maximum flow using the Improved Shortest Augmenting Path (ISAP) algorithm.
 * @note Time Complexity: O(V^2 * E) where V is the number of vertices and E is the number of edges.
 *       ISAP tends to be slightly faster than a standard Dinic implementation. It tends to have lower constant factors, avoids the overhead of repeated BFS phases, and its gap heuristic is highly effective. 
 *       Dinic has better proven complexity bounds on specific graph types, like unit capacity networks (O(min(V^(2/3), E^(1/2)) * E)).
 * @note Space Complexity: O(V + E) due to storing adjacency lists and level/gap arrays.
 *       In the worst case, O(E) for storing the edges.
 * 
 * @param s The source node.
 * @param t The sink node.
 * @return The maximum flow from s to t.
 *
 * @note This algorithm can be used for solving Directed Graph Maximum Weight Closure.
 *       Let G = (V, E) be a directed graph with weights w(v) on the vertices, where w(v) can be positive or negative.
 *       A closure of G is a subset of vertices C ⊆ V, such that for every vertex v in C, all of its successors are also in C.
 *       The weight of the closure is the sum of the weights of the vertices in it.
 *       The goal is to find the closure of maximum weight.
 *       This can be reduced to a min-cut problem (max-flow problem) as follows:
 *       1. Construct a new graph G' from G.
 *       2. Add a source node 's' and a sink node 't' to G'.
 *       3. For each vertex v in G:
 *          - If w(v) > 0, add an edge from 's' to v with capacity w(v).
 *          - If w(v) < 0, add an edge from v to 't' with capacity -w(v).
 *          - If w(v) = 0, no edges are added regarding the node weight.
 *       4. For each edge (u, v) in G, add an edge from u to v in G' with infinite capacity.
 *       5. Let W_pos be the sum of all positive w(v).
 *       6. Compute the maximum flow F from 's' to 't' in G'.
 *       7. The maximum closure weight is W_pos - F.
 *       8. The maximum weight closure is all the vertices that can be reachable from s, in the residual graph.
 */
int Isap::isap(int s, int t) {
    // Compute distance labels using BFS from the sink
    bfs(t);
    // If there is no path from source to sink, return 0
    if (level[s] == -1) {
        return 0;
    }
    /**
     * flow: A variable to keep track of the current maximum flow.
     * u: The current vertex in the graph which starts from source s.
     * cur: The current edge in the adjacency list of each node being considered for augmenting path.
     * path: Stores the nodes in the current augmenting path from source to the current node u.
     *      It's used to track the path's edges and their capacities for flow augmentation.
     */
    int flow = 0, u = s;
    vector<int> cur(n, 0);
    vector<int> path;
    while (level[s] < n) {
        if (u == t) {
            int f = INF;
            for (int i = 0; i < path.size(); i++) {
                f = min(f, adj[path[i]][cur[path[i]]].cap - adj[path[i]][cur[path[i]]].flow);
            }
            for (int i = 0; i < path.size(); i++) {
                int u = path[i];
                int rev = adj[u][cur[u]].rev;
                adj[u][cur[u]].flow += f;
                adj[adj[u][cur[u]].to][rev].flow -= f;
            }
            flow += f;
            u = s;
            path.clear();
        }
        bool advanced = false;
        for (; cur[u] < adj[u].size(); cur[u]++) {
            Edge &e = adj[u][cur[u]];
            if (e.cap > e.flow && level[u] == level[e.to] + 1) {
                path.push_back(u);
                u = e.to;
                advanced = true;
                break;
            }
        }
        if (!advanced) {
            int min_level = n;
            for (auto &e : adj[u]) {
                if (e.cap > e.flow) {
                    min_level = min(min_level, level[e.to]);
                }
            }
            if (--gap[level[u]] == 0) break;
            gap[level[u] = min_level + 1]++;
            cur[u] = 0;
            if (!path.empty()) {
                u = path.back();
                path.pop_back();
            }
        }
    }
    return flow;
}
