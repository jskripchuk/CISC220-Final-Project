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
#include <queue>
#include <limits.h>
#include <string.h>


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
    int flux;

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
  int status = 0; // or color
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
  //int[][] matrix;
  
  // Constructor
  ChordGraph(){}
  
  // Destructor
  ~ChordGraph(){
    for (auto iter = vertices.begin(); iter != vertices.end(); iter++) {
      
      Vertex * v = iter->second;
      delete v;
    }
  }
  
  //Creates an adjacency matrix from an adjacency list
  vector<vector<int>> getMatrix(){
    int len = vertices.size();
    vector< vector<int>> vec(len, vector<int>(len));
    
    //int row = 0;
    
    
    for (auto iter = vertices.begin();
      iter != vertices.end(); iter++) {
      Vertex * v = iter->second;
      cout << v->label;
      cout << ": ";
      cout << v->id;
      cout << " ";
      //cout << v->label << endl;
      int row = v->id;
      
      for(int i = 0; i < v->edges.size(); i++){
         Vertex* n = v->edges[i]->target;
          int col = n->id;
        
         vec[row][col] = v->edges[i]->weight;
        //cout << n->id << endl;
     }
   }
    cout << endl;
    
    
    
    //cout << "MEME" <<endl;
    return vec;
  }
  
  
  //BEGIN FORD FULKENSON STUFF
  
  /* Returns true if there is a path from source 's' to sink 't' in
  residual graph. Also fills parent[] to store the path */
  //#define V 11
bool bfs(int** rGraph, int s, int t, int parent[], int size)
{
    // Create a visited array and mark all vertices as not visited
    bool visited[size];
    memset(visited, 0, sizeof(visited));
 
    // Create a queue, enqueue source vertex and mark source vertex
    // as visited
    queue <int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;
 
    // Standard BFS Loop
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
 
        for (int v=0; v<size; v++)
        {
            if (visited[v]==false && rGraph[u][v] > 0)
            {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }
 
    // If we reached sink in BFS starting from source, then return
    // true, else false
    return (visited[t] == true);
}
  
  int fordFulkerson(int** graph, int s, int t, int size)
{
    int u, v;
 
    // Create a residual graph and fill the residual graph with
    // given capacities in the original graph as residual capacities
    // in residual graph
   // int rGraph[V][V]; // Residual graph where rGraph[i][j] indicates 
                     // residual capacity of edge from i to j (if there
                     // is an edge. If rGraph[i][j] is 0, then there is not)  
    int ** rGraph;
    rGraph = new int*[size];
      
    for (int i = 0; i < size; ++i) {
       rGraph[i] = new int[size];
    }
    
    for (u = 0; u < size; u++)
        for (v = 0; v < size; v++)
             rGraph[u][v] = graph[u][v];
 
    int parent[size];  // This array is filled by BFS and to store path
 
    int max_flow = 0;  // There is no flow initially
 
    // Augment the flow while tere is path from source to sink
    while (bfs(rGraph, s, t, parent, size))
    {
        // Find minimum residual capacity of the edges along the
        // path filled by BFS. Or we can say find the maximum flow
        // through the path found.
        int path_flow = INT_MAX;
        for (v=t; v!=s; v=parent[v])
        {
            u = parent[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }
 
        // update residual capacities of the edges and reverse edges
        // along the path
        for (v=t; v != s; v=parent[v])
        {
            u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }
 
        // Add path flow to overall flow
        max_flow += path_flow;
    }
  
  //for(int i = 0; i < size; i++){
    //cout << parent[i];
    //cout << ",";
 // }
  //cout << endl;
 
    // Return the overall flow
    return max_flow;
}
  
  //Finds the chord veretex with highest degree (does not include weighting)
  //This is the chord that has the most diversity coming and leaving it
  int findMaxDegree(){
    //auto iter = vertices.begin();
    Vertex* max = vertices.begin()->second;
    
    
     for (auto iter = vertices.begin();
      iter != vertices.end(); iter++) {
      Vertex * v = iter->second;
      //cout << v->label << endl;
      
      if(v->edges.size() > max->edges.size()){
        max = v;
      }
   }
    cout << "MAXIMUM DEGREE IS: " << max->label << " WITH DEGREE OF " << max->edges.size() << endl;
    //cout << max->label << endl;
   // cout << max->edges.size() << endl;
    return max->edges.size();
  }
  
  void bruteForceMaximumFlow(int** arr, int size, string* chordArr){
    
    int maximumFlow = -1;
    int maxSource = -1;
    int maxSink = -1;
    
    //Brute force solution between the total maximum flow between any two source sink permutation
    //Total checks: (V)*(V-1), O(V^2) V = number of chords in the song.
    //Edmonds Karp is O(V*E^2)
    //So thus our complexity is O(V^3*E^2)
    for (auto sourceIter = vertices.begin();
      sourceIter != vertices.end(); sourceIter++) {
      Vertex* sourceVertex = sourceIter->second;
      int s = sourceVertex->id;
      
      for (auto sinkIter = vertices.begin();
      sinkIter != vertices.end(); sinkIter++) {
        Vertex* sinkVertex = sinkIter->second;
        
        if(sourceVertex != sinkVertex){
          int t = sinkVertex->id;
          //Do the Edmond Karp
          int flow = fordFulkerson(arr, s, t, size);
          //cout << flow << endl;
          
          if(flow > maximumFlow){
            maximumFlow = flow;
            maxSource = s;
            maxSink = t;
          }
          
          //cout << x << endl;
          //maximumFlow = edmonds(g, source, sink)
        }
        
      }
      //cout << endl;
    }
    cout << "MAX FLOW IS: " << maximumFlow << endl;
    cout << "SOURCE: " << chordArr[maxSource] << endl;
    cout << "SINK: " << chordArr[maxSink] << endl;
  }
  
  //Gets the matrix in vector form and then turns it into array form
  void doFordFulkerson(){
    vector<vector<int>> vec = getMatrix();
    int size = vec.size();
    int **arr;
    arr = new int*[size];
    
    string* chordArr = new string[size];
    
    //a chord dictionary
    for (auto iter = vertices.begin();
      iter != vertices.end(); iter++) {
      Vertex * v = iter->second;
      
      chordArr[v->id] = v->label;
    }
    
      
    for (int i = 0; i < size; ++i) {
       arr[i] = new int[size];
    }
    
    
    for(int i = 0; i < vec.size(); i++){
      for(int j = 0; j < vec.size(); j++){
        arr[i][j] = vec[i][j];
      }
    }
    
    cout << "Brute forcing..." << endl;
    bruteForceMaximumFlow(arr, size, chordArr);
    //cout << x << e
  }
 
// Returns the maximum flow from s to t in the given graph

  
  //END FORD FULKINSUN STUFF
 
  
  
  bool bfs(Vertex* source){
//     queue<Vertex*> q;
//     q.push(source);
    
//     source->status = 1;
//     while(!q.empty()){
//       Vertex* v = q.front();
//       q.pop();
      
//       cout << v->label << endl;
      
//       for(int i = 0; i < v->edges.size(); i++){
//         Vertex* n = v->edges[i]->target;
//         //not visited
//         if(n->status != 1){
//             q.push(n);
//             n->status = 1;
//         }
//       }
//     }
//     //q.push(source);
//     //cout << q.front()->label << endl;
    return true;
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