# CISC220-Final-Project
### About
A C++ program that generates a graph from a list of chords that represents a song. The generated graph's vertices represent chords in the song, and edges represent the progression from one chord to another. For a given song, the program will find the chord vertex with the maximum degree, and it will find the chord progression with the maximum flow using the Ford–Fulkerson algorithm.

### To run the code: 
- Clone or download this GitHub repository
- Navigate to the cloned/downloaded project folder
- Run ```make```
- Run ```./parse rs200_harmony_exp/```
- Three text files will be generated:
  - graphs.txt: The adjacency list representation of every graph for every song
  - flow.txt: The maximum flow and degree for every song
  - stats.txt: A compiled list of the number of maximum flow chords and maximum degree chords of every song
