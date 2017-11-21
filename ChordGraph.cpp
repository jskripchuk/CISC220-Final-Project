#include "ChordGraph.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <algorithm>
#include <vector>
#include <limits>
#include <string>
#include <sstream>

using namespace std;

// Code that creates a vector of strings using delimiters. Each index of the vector will contain a chord.
void line_populate(vector<string> &record, const string& line, char delimiter) {
  int linepos=0;
  char c;
  int i;
  int linemax=line.length();
  string curstring;
  record.clear();

  while(line[linepos]!=0 && linepos < linemax) {
    c = line[linepos];
    if (c==delimiter) {
      //end of field
      record.push_back( curstring );
      curstring="";
    }
    else if ((c=='\r' || c=='\n')) {
      record.push_back( curstring );
      return;
    }
    else {
      curstring.push_back(c);
    }
    linepos++;
  }
  record.push_back( curstring );
  return;
}

// Code that reads a file of chords, and makes a ChordGraph of it. The file must contain chords separated by spaces or newlines only!
// The very last line of the file will not be read, song information can be stored there.
bool parseChords(ChordGraph & cg, char * fileName){
  
  ifstream inFile(fileName); // Input File Stream
  
  // Error stuff
  if (!inFile){
    cerr << "Error with opening file" << endl;
    return false;
  }
  
  if (inFile.fail()) {
    cerr << "No file with that name" <<endl;
    return false;
  }
   
  string line; // Holds the current line from getLine
  vector<string> row; // Holds the parsed chords of the current line
  string carry = ""; // Holds the chord in the last index of the last row
  
  while (getline(inFile, line, '\n') && inFile.good() ){
    line_populate(row, line, ' '); // Read the current line
    
    // If this isn't the first line of the file
    if (carry != ""){
      // Make a progression from to last chord of the last line to the first chord of this line
      cout << "--------------------" << endl;
      cg.addProgression(carry, row[0]);
    }
    
    // For every chord except the last in this line
    for (int i = 0; i < row.size() - 1; i++){
      // Make a progression from first chord to next chord
      cout << "--------------------" << endl;
      cg.addProgression(row[i], row[i+1]);
    }

    // Set the carry chord to the last chord of this line that a progression can be made to the first chord of the next line
    carry = row.back();
  }
  
  return true;
}