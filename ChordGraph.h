#ifndef CHORDGRAPH_H
#define CHORDGRAPH_H

#ifndef NULL
#define NULL 0
#endif

#include <vector>
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <map>

using namespace std;

class Vertex;
class Edge;

#ifndef DEFAULT_VERTEX_STATUS
#define UNKNOWN 0
#define IDENTIFIED 1
#define VISITED 2
#define DEFAULT_VERTEX_STATUS 0
#endif

// The progression from one Chord to another
class Edge {
  public:
    Vertex * source;
    Vertex * target;
    double weight; // The total number of times a progression from the source to the target occurs

  // Constructor
    Edge(Vertex * pSource, Vertex * pTarget, double pWeight)
        : source(pSource), target(pTarget), weight(pWeight){ }

  //Destructor
    ~Edge() {}
};

// The Chord
class Vertex {
  public:
  int id; // id represents the order this Vertex was added to a graph
  string label; // label/chord (unique)
  int status; // or color
  int repeat; // The number of times a chord repeats in a row
  
  vector<Edge*> edges; // A vector of all the progressions from this Chord
  
  // Constructor
  Vertex(int pId, string pLabel) :
    id(pId), label(pLabel), status(DEFAULT_VERTEX_STATUS), repeat(0) {}

  // Destructor
  ~Vertex() {
    for (auto iter = edges.begin(); iter != edges.end(); iter++) {
      
      Edge * l = *iter;
      delete l;
    }
  }
};

// A representation of the entire song containing all chords and their progressions
class ChordGraph {
  
  public:
  
  map<string, Vertex*> vertices; // The list of Chords
  
  // Constructor
  ChordGraph(){}
  
  // Destructor
  ~ChordGraph(){
    for (auto iter = vertices.begin(); iter != vertices.end(); iter++) {
      
      Vertex * v = iter->second;
      delete v;
    }
  }
  
  // Function for adding a chord progression to the graph
  void addProgression(string chord1, string chord2){
    
    if (vertices.find(chord1) == vertices.end()){ // If there is not already a vertex of chord1
      // Create new vertex for chord1
      vertices[chord1] = new Vertex(vertices.size(), chord1);
      cout << "Vertex created for " << chord1 << endl;
    } 
    
    if (vertices.find(chord2) == vertices.end()){ // If there is not already a vertex of chord2
      // Create new vertex for chord2
      vertices[chord2] = new Vertex(vertices.size(), chord2);
      cout << "Vertex created for " << chord2 << endl;
    }
    
    // v1 and v2 refer to chord1 and chord2
    Vertex * v1 = vertices[chord1];
    Vertex * v2 = vertices[chord2];
    
    // If this is a repetition
    if (chord1 == chord2){
      (v1->repeat)++;;
      cout << "Chord " << v1->label << " repeated " << v1->repeat << " times" << endl; 
    } else { // if not
    
    bool found = false;
    int edgeIndex;
    // Search if this progression already exists
    for (int i = 0; i < v1->edges.size(); i++){
      if (v1->edges[i]->target->label == chord2){
        found = true;
        edgeIndex = i;
        break;
      }
    }

    // If it doesn't exist
    if (!found){
      // Make the new edge
      v1->edges.push_back(new Edge(v1, v2, 1));
      cout << "Made new edge from " << v1->label << " to " << v2->label << endl;
      cout << "Weight: " << v1->edges.back()->weight << endl;
    } else { // If it does exists
      // Update the edge by adding 1 to the weight
      (v1->edges[edgeIndex]->weight)++;
      cout << "Updated edge from " << v1->label << " to " << v2->label << endl;
      cout << "New Weight: " << v1->edges[edgeIndex]->weight << endl;
    }
    
    }
    return;
  }
  
  // A print-out representation of the graph
  string toString() const {
    stringstream s;
    for (auto iterV = vertices.begin(); iterV != vertices.end(); iterV++) {
      
      Vertex * v = iterV->second;
      s << v->label << "(" << v->repeat << "): ";
      for (auto iterE = v->edges.begin(); iterE != v->edges.end(); iterE++) {
        
          Edge * e = *iterE;
          s << e->target->label << "(" << e->weight << ") ";
      }
      
      s << endl;
    }
    
    return s.str();
  }
};

// Defined in ChordGraph.cpp
bool parseChords(ChordGraph & cg, char * fileName);

#endif