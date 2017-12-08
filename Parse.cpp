#include <iostream>
#include <cstdlib>
#include <cmath>
#include <limits>
#include "ChordGraph.h"

using namespace std;


int main(int argc, char **argv) {
  cout << "Reading " << argv[1] << "..." << endl;

  ChordGraph cg;
  parseChords(cg, argv[1]);
  cout << endl;
  cout << "Graph:" << endl;
  cout << cg.toString() << endl;
  
  //Does the flow analysis on the graph
  cg.doFordFulkerson();
  cout << endl;
  cg.findMaxDegree();

  return 0;
}
