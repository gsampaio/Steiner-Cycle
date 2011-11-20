# Steiner Cycle Problem Heuristics

This is an undergraduation project for an lessions of analysis of algorithims. Here you will find mainly two implementations for the Steiner Cycle Problem.

## Steiner Cycle 

The steiner cycle problem consist on given an Graph and a subset of vertices we have to make an cycle that pass through all the vertices on the subset. This is an NP-D problem with can be simply verified by making an reduction of the TSP problem 

## GRASP Heuristic

For an rapid search I am going to implement an GRASP heuristic with muilti-start-search. 

## Greedy Heuristic

This heuristic take the time of O(V^4) to create an solution. It is based on execute Dijkstra for all the terminal nodes searching the best path for each tuple of terminal nodes.

## Executing

Make sure you have lemon installed on your computer then just execute run.sh script and check the out.out file. Make sure to add u+x permission to execute run.sh

## Dependencies

This project mainly depends on the LEMON C++ Library for Graphs.
