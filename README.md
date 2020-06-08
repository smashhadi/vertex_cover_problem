# vertex_cover_problem

## Construct Graph
Code expects a series of inputs:
Combination of a pair of vertices which can signify a line

Output:An undirected graph with vertices and edges

## Find Shortest Path
Expects a series of inputs in a specific format:
Number of vertices Pair of edges start and end vertex

Output: shortest path in an undirected graph from starting vertex to end vertex.

Code runs till EOF is reached.

## Inter-Process Communication
Link the output of the random input generator to the input of construct_graph.py, and the output of this Python script to the input of shortest_path.cpp
