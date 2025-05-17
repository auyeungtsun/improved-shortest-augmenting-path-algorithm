#include "isap.h"
#include <iostream>

using namespace std;

int main() {
  Isap graph(6); // Create a graph with 6 nodes

  // Add edges to the graph
  graph.add_edge(0, 1, 16);
  graph.add_edge(0, 2, 13);
  graph.add_edge(1, 2, 10);
  graph.add_edge(1, 3, 12);
  graph.add_edge(2, 1, 4);
  graph.add_edge(2, 4, 14);
  graph.add_edge(3, 2, 9);
  graph.add_edge(3, 5, 20);
  graph.add_edge(4, 3, 7);
  graph.add_edge(4, 5, 4);

  // Calculate the maximum flow from source 0 to sink 5
  int max_flow = graph.isap(0, 5);

  // Print the result
  cout << "Maximum flow: " << max_flow << endl;

  return 0;
}