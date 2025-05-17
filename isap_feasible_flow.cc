#include "isap.h"
#include <vector>
#include <numeric>
#include <cassert>
#include <tuple>

struct OriginalEdge {
    // u, v: The start and end vertices of the edge.
    // lower: The minimum flow required on this edge.
    // upper: The maximum flow allowed on this edge.
    int u, v, lower, upper;
};

/**
 * @brief Checks if a feasible flow exists in a flow network with lower and upper bounds on edge capacities.
 * 
 * This function determines whether a flow network, defined by a set of edges with minimum and maximum capacity constraints, 
 * can have a feasible flow from source 's' to sink 't'. It uses the maximum flow algorithm (Isap) on an auxiliary graph.
 * 
 * @param n The number of vertices in the graph.
 * @param s The source vertex.
 * @param t The sink vertex.
 * @param edges A vector of OriginalEdge structs, where each struct represents an edge with its start vertex (u), end vertex (v), 
 *              minimum flow (lower), and maximum flow (upper).
 * 
 * @note To find the maximum flow when a feasible flow exists:
 *       1. Determine the flow value on the edge (t, s). This value represents the net flow from s to t in the specific feasible flow implicitly found by the SS->TT calculation. 
 *       2. Remove the (t, s) and (s, t) edges related to the infinite capacity auxiliary edge
 *       3. Calculate the maximum flow directly from the original source s to the original sink t in the current residual graph. It finds how much more flow can be pushed from s to t.
 * 
 * @note To find the minimum flow when a feasible flow exists:
 *       1. Determine the flow value on the edge (t, s). This value represents the net flow from s to t in the specific feasible flow implicitly found by the SS->TT calculation. 
 *       2. Remove the (t, s) and (s, t) edges related to the infinite capacity auxiliary edge
 *       3. Calculate the maximum flow from the original sink t to the original source s in the current residual graph. This finds the maximum amount by which the s -> t flow can be reduced by pushing flow "backwards".
 * 
 * @return true if a feasible flow exists, false otherwise.
 */
bool has_feasible_flow(int n, int s, int t, const vector<OriginalEdge>& edges) {
    for (const auto& edge : edges) {
        if (edge.lower > edge.upper) {
            return false;
        }
    }
    vector<int> demand(n, 0);
    for (const auto& edge : edges) {
        demand[edge.u] -= edge.lower;
        demand[edge.v] += edge.lower;
    }

    // Create the auxiliary graph G' with n+2 nodes
    // Node indices: 0 to n-1 are original nodes
    // n   is the supersource SS
    // n+1 is the supersink TT
    int SS = n;
    int TT = n + 1;
    Isap isap_aux(n + 2);

    int total_positive_demand = 0;

    // 1. Edges for adjustable flow (upper - lower)
    for (const auto& edge : edges) {
        if (edge.upper - edge.lower > 0) {
             isap_aux.add_edge(edge.u, edge.v, edge.upper - edge.lower);
        }
    }

    // 2. Edges from SS to nodes with positive demand
    // 3. Edges from nodes with negative demand to TT
    for (int i = 0; i < n; ++i) {
        if (demand[i] > 0) {
            isap_aux.add_edge(SS, i, demand[i]);
            total_positive_demand += demand[i];
        } else if (demand[i] < 0) {
            isap_aux.add_edge(i, TT, -demand[i]);
        }
    }

    // 4. Edge from original sink t to original source s
    // Use a sufficiently large capacity
    isap_aux.add_edge(t, s, INF);

    int max_flow_aux = isap_aux.isap(SS, TT);

    return max_flow_aux == total_positive_demand;
}

void test_has_feasible_flow() {
    // Test case 1: Feasible flow
    int n1 = 4;
    int s1 = 0;
    int t1 = 3;
    vector<OriginalEdge> edges1 = {
        {0, 1, 5, 10},
        {0, 2, 2, 8},
        {1, 3, 3, 6},
        {2, 3, 4, 9}
    };
    assert(has_feasible_flow(n1, s1, t1, edges1));

    // Test case 2: Infeasible flow (lower > upper)
    int n2 = 3;
    int s2 = 0;
    int t2 = 2;
    vector<OriginalEdge> edges2 = {
        {0, 1, 5, 3},
        {1, 2, 1, 4},
    };
    assert(!has_feasible_flow(n2, s2, t2, edges2));
    // Test case 3: Infeasible flow (not enough capacity)
    int n3 = 4;
    int s3 = 0;
    int t3 = 3;
    vector<OriginalEdge> edges3 = {
        {0, 1, 6, 10}, {0, 2, 4, 8}, {1, 3, 1, 3}, {2, 3, 2, 3}
    };
    assert(!has_feasible_flow(n3, s3, t3, edges3));

    // Test case 4: Feasible flow
    int n4 = 4;
    int s4 = 0;
    int t4 = 3;
    vector<OriginalEdge> edges4 = {
        {0, 1, 2, 5},
        {0, 2, 1, 3},
        {1, 3, 1, 3},
        {2, 3, 2, 4}
    };
    assert(has_feasible_flow(n4, s4, t4, edges4));
    cout << "All test cases passed!" << endl;
}

void run_has_feasible_flow_sample() {
    int n = 4;
    int s = 0;
    int t = 3;
    vector<OriginalEdge> edges = {
        {0, 1, 2, 5},
        {0, 2, 1, 3},
        {1, 3, 1, 3},
        {2, 3, 2, 4}
    };
    bool feasible = has_feasible_flow(n, s, t, edges);
    if(feasible){
        cout << "Sample has a feasible flow." << endl;
    }else{
        cout << "Sample does not have a feasible flow." << endl;
    }
}

int main() {
    test_has_feasible_flow();
    run_has_feasible_flow_sample();
    return 0;
}
