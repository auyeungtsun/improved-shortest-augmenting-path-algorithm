#include "isap.h"
#include <cassert>

int main() {
  // Test case 1: Simple graph
  Isap graph1(4);
  graph1.add_edge(0, 1, 10);
  graph1.add_edge(0, 2, 2);
  graph1.add_edge(1, 2, 6);
  graph1.add_edge(1, 3, 8);
  graph1.add_edge(2, 3, 10);
  assert(graph1.isap(0, 3) == 12);

  // Test case 2: Another simple graph with different capacities
  Isap graph2(6);
  graph2.add_edge(0, 1, 16);
  graph2.add_edge(0, 2, 13);
  graph2.add_edge(1, 2, 10);
  graph2.add_edge(1, 3, 12);
  graph2.add_edge(2, 1, 4);
  graph2.add_edge(2, 4, 14);
  graph2.add_edge(3, 2, 9);
  graph2.add_edge(3, 5, 20);
  graph2.add_edge(4, 3, 7);
  graph2.add_edge(4, 5, 4);
  assert(graph2.isap(0, 5) == 23);

  // Test case 3: Unreachable sink
  Isap graph3(3);
  graph3.add_edge(0, 1, 10);
  assert(graph3.isap(0, 2) == 0);

  return 0;
}